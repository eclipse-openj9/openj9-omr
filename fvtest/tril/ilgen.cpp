/*******************************************************************************
 *
 * (c) Copyright IBM Corp. 2017, 2017
 *
 *  This program and the accompanying materials are made available
 *  under the terms of the Eclipse Public License v1.0 and
 *  Apache License v2.0 which accompanies this distribution.
 *
 *      The Eclipse Public License is available at
 *      http://www.eclipse.org/legal/epl-v10.html
 *
 *      The Apache License v2.0 is available at
 *      http://www.opensource.org/licenses/apache2.0.php
 *
 * Contributors:
 *    Multiple authors (IBM Corp.) - initial implementation and documentation
 ******************************************************************************/

#include "ilgen.hpp"

#include "il/Block.hpp"
#include "il/Node.hpp"
#include "il/Node_inlines.hpp"

#include <unordered_map>
#include <string>

#define TraceEnabled    (comp()->getOption(TR_TraceILGen))
#define TraceIL(m, ...) {if (TraceEnabled) {traceMsg(comp(), m, ##__VA_ARGS__);}}

class OpCodeTable : public TR::ILOpCode {
   public:
      explicit OpCodeTable(TR::ILOpCodes opcode) : TR::ILOpCode{opcode} {}
      explicit OpCodeTable(const std::string& name) : TR::ILOpCode{getOpCodeFromName(name)} {}

      static TR::ILOpCodes getOpCodeFromName(const std::string& name) {
         auto opcode = _opcodeNameMap.find(name);
         if (opcode == _opcodeNameMap.cend()) {
            for (const auto& p : _opCodeProperties) {
               if (name == p.name) {
                  _opcodeNameMap[name] = p.opcode;
                  return p.opcode;
               }
            }
            return TR::BadILOp;
         }
         else {
            return opcode->second;
         }
      }

   private:
      static std::unordered_map<std::string, TR::ILOpCodes> _opcodeNameMap;
};

std::unordered_map<std::string, TR::ILOpCodes> OpCodeTable::_opcodeNameMap;

TRLangBuilder::TRLangBuilder(ASTNode* trees, TR::TypeDictionary* d) : _trees(trees), TR::MethodBuilder(d) {
    DefineLine(__LINE__);
    DefineFile(__FILE__);

    DefineName("treeMethod");
    DefineParameter("arg0", Int32);
    DefineParameter("arg1", Int32);
    DefineParameter("arg2", d->PointerTo(d->PointerTo(Int32)));
    DefineReturnType(Int32);
}

static uint16_t countNodes(const ASTNode* n) {
    uint16_t count = 0;
    while (n) {
       ++count;
       n = n->next;
    }
    return count;
}

TR::Node* TRLangBuilder::toTRNode(const ASTNode* const tree) {
     TR::Node* node = nullptr;

     auto childCount = countNodes(tree->children);
     auto opcode = OpCodeTable{tree->name};

     if (opcode.isLoadConst()) {
        node = TR::Node::create(opcode.getOpCodeValue(), childCount);
        if (opcode.isIntegerOrAddress()) {
           node->set64bitIntegralValue(tree->args->value.value.int64);
        }
        else {
           switch (opcode.getType()) {
              case TR::Float:
                 node->setFloat(static_cast<float>(tree->args->value.value.f64));
                 break;
              case TR::Double:
                 node->setDouble(tree->args->value.value.f64);
                 break;
              default:
                 return nullptr;
           }
        }
        TraceIL("Created %s (%p)\n", opcode.getName(), node);
     }
     else if (opcode.isLoadDirect()) {
        if (strcmp("parm", tree->args->name) == 0) {
             auto arg = tree->args->value.value.int64;
             auto symref = symRefTab()->findOrCreateAutoSymbol(_methodSymbol,
                                                               static_cast<int32_t>(arg),
                                                               opcode.getType() );
             node = TR::Node::createLoad(symref);
             TraceIL("Created %s of parm #%d (%p)\n", opcode.getName(), arg, node);
         }
         else if (strcmp("temp", tree->args->name) == 0) {
             const auto sym = tree->args->value.value.str;
             node = TR::Node::createLoad(_symRefMap[sym]);
             TraceIL("Created %s of temp \"%s\" (%p) [symref @ %p]\n", opcode.getName(), sym, node, _symRefMap[sym]);
         }
         else {
             return nullptr;
         }
     }
     else if (opcode.isStoreDirect()) {
         if (strcmp("temp", tree->args->name) == 0) {
             const auto sym = tree->args->value.value.str;
             if (_symRefMap.find(sym) == _symRefMap.end()) {
                 _symRefMap[sym] = symRefTab()->createTemporary(methodSymbol(), opcode.getDataType());
                 TraceIL("Created temp symref \"%s\" (%p)\n", sym, _symRefMap[sym]);
             }
             node = TR::Node::createWithSymRef(opcode.getOpCodeValue(), childCount, _symRefMap[sym]);
             TraceIL("Created %s of temp \"%s\" (%p) [symref @ %p]\n", opcode.getName(), sym, node, _symRefMap[sym]);
         }
         else {
             return nullptr;
         }
     }
     else if (opcode.isLoadIndirect() || opcode.isStoreIndirect()) {
         auto offset = tree->args->value.value.int64;
         const auto name = tree->name;
         auto type = opcode.getType();
         auto compilation = TR::comp();
         TR::Symbol *symbol = TR::Symbol::createNamedShadow(compilation->trHeapMemory(), type, TR::DataType::getSize(opcode.getType()), name);
         TR::SymbolReference *symRef = new (compilation->trHeapMemory()) TR::SymbolReference(compilation->getSymRefTab(), symbol, compilation->getMethodSymbol()->getResolvedMethodIndex(), -1);
         symRef->setOffset(offset);
         node = TR::Node::createWithSymRef(opcode.getOpCodeValue(), childCount, symRef);
         TraceIL("Created %s with offset %d (%p) [symref @ %p]\n", opcode.getName(), offset, node, symRef);
     }
     else if (opcode.isIf()) {
         const auto targetName = tree->args->value.value.str;
         auto targetId = _blockMap[targetName];
         auto c1 = TR::Node::create(TR::BadILOp);
         auto c2 = c1;
         node = TR::Node::createif(opcode.getOpCodeValue(), c1, c2, _blocks[targetId]->getEntry());
         TraceIL("Created %s (%p) to block \"%s\"\n", opcode.getName(), node, targetName);
     }
     else if (opcode.isBranch()) {
         const auto targetName = tree->args->value.value.str;
         auto targetId = _blockMap[targetName];
         node = TR::Node::create(opcode.getOpCodeValue(), childCount);
         node->setBranchDestination(_blocks[targetId]->getEntry());
         TraceIL("Created %s (%p) to block \"%s\"\n", opcode.getName(), node, targetName);
     }
     else {
        node = TR::Node::create(opcode.getOpCodeValue(), childCount);
        TraceIL("Created %s (%p)\n", opcode.getName(), node);
     }

     const ASTNode* t = tree->children;
     int i = 0;
     while (t) {
         node->setAndIncChild(i, toTRNode(t));
         t = t->next;
         ++i;
     }

     return node;
}

void TRLangBuilder::cfgFor(const ASTNode* const tree) {
   const ASTNode* t = tree->children;
   while (t) {
       cfgFor(t);
       t = t->next;
   }

   auto opcode = OpCodeTable{tree->name};

   if (opcode.isReturn()) {
       cfg()->addEdge(_currentBlock, cfg()->getEnd());
       TraceIL("Added CFG edge from block %d to {exit} -> %s\n", _currentBlockNumber, tree->name);
   }
   else if (opcode.isBranch()) {
      const auto targetName = tree->args->value.value.str;
      auto targetId = _blockMap[targetName];
      cfg()->addEdge(_currentBlock, _blocks[targetId]);
      TraceIL("Added CFG edge from block %d to block %d (\"%s\") -> %s\n", _currentBlockNumber, targetId, targetName, tree->name);
   }
}

bool TRLangBuilder::injectIL() {
   TraceIL("=== %s ===\n", "Generating Blocks");
    createBlocks(countNodes(_trees));
    const ASTNode* block = _trees;
    auto blockIndex = 0;
    while (block) {
       const ASTNodeArg* a = block->args;
       while (a) {
           if (strcmp("name", a->name) == 0) {
               _blockMap[a->value.value.str] = blockIndex;
               TraceIL("Name of block %d set to \"%s\" (%p)\n", blockIndex, a->value.value.str);
           }
           a = a->next;
       }
       ++blockIndex;
       block = block->next;
    }

    TraceIL("=== %s ===\n", "Generating IL");
    block = _trees;
    generateToBlock(0);
    while (block) {
       const ASTNode* t = block->children;
       while (t) {
           const auto tt = genTreeTop(toTRNode(t));
           TraceIL("Created TreeTop (%p)\n", tt);
           t = t->next;
       }
       generateToBlock(_currentBlockNumber + 1);
       block = block->next;
    }

    TraceIL("=== %s ===\n", "Generating CFG");
    block = _trees;
    generateToBlock(0);
    while (block) {
       const ASTNodeArg* a = block->args;
       while (a) {
           if (strcmp("fallthrough", a->name) == 0) {
               if (strcmp("{exit}", a->value.value.str) == 0) {
                   cfg()->addEdge(_currentBlock, cfg()->getEnd());
                   TraceIL("Added fallthrough edge from block %d to \"%s\"\n", _currentBlockNumber, a->value.value.str);
               }
               else {
                   auto destBlock = _blockMap.at(a->value.value.str);
                   cfg()->addEdge(_currentBlock, _blocks[destBlock]);
                   TraceIL("Added fallthrough edge from block %d to block %d \"%s\"\n", _currentBlockNumber, destBlock, a->value.value.str);
               }
           }
           a = a->next;
       }

       const ASTNode* t = block->children;
       while (t) {
           cfgFor(t);
           t = t->next;
       }

       generateToBlock(_currentBlockNumber + 1);
       block = block->next;
    }

    return true;
}
