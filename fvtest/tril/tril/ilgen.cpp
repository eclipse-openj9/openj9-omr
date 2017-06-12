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

     TraceIL("Creating %s from ASTNode %p\n", opcode.getName(), tree);
     if (opcode.isLoadConst()) {
        TraceIL("  is load const of ", "");
        node = TR::Node::create(opcode.getOpCodeValue(), childCount);
        if (opcode.isIntegerOrAddress()) {
           node->set64bitIntegralValue(tree->args->value.value.int64);
           TraceIL("integral value %d\n", tree->args->value.value.int64);
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
           TraceIL("floating point value %f\n", tree->args->value.value.f64);
        }
     }
     else if (opcode.isLoadDirect()) {
        TraceIL("  is direct load of ", "");
        if (strcmp("parm", tree->args->name) == 0) {
             auto arg = tree->args->value.value.int64;
             TraceIL("parameter %d\n", arg);
             auto symref = symRefTab()->findOrCreateAutoSymbol(_methodSymbol,
                                                               static_cast<int32_t>(arg),
                                                               opcode.getType() );
             node = TR::Node::createLoad(symref);
         }
         else if (strcmp("temp", tree->args->name) == 0) {
             const auto symName = tree->args->value.value.str;
             TraceIL("temporary %s\n", symName);
             auto symref = _symRefMap[symName];
             node = TR::Node::createLoad(symref);
         }
         else {
             return nullptr;
         }
     }
     else if (opcode.isStoreDirect()) {
        TraceIL("  is direct store of ", "");
        if (strcmp("temp", tree->args->name) == 0) {
            const auto symName = tree->args->value.value.str;
            TraceIL("temporary %s\n", symName);
            if (_symRefMap.find(symName) == _symRefMap.end()) {
                _symRefMap[symName] = symRefTab()->createTemporary(methodSymbol(), opcode.getDataType());
            }
            auto symref =_symRefMap[symName];
            node = TR::Node::createWithSymRef(opcode.getOpCodeValue(), childCount, symref);
        }
        else {
            return nullptr;
        }
     }
     else if (opcode.isLoadIndirect() || opcode.isStoreIndirect()) {
         auto offset = tree->args->value.value.int64;
         TraceIL("  is indirect store/load with offset %d\n", offset);
         const auto name = tree->name;
         auto type = opcode.getType();
         auto compilation = TR::comp();
         TR::Symbol *sym = TR::Symbol::createNamedShadow(compilation->trHeapMemory(), type, TR::DataType::getSize(opcode.getType()), name);
         TR::SymbolReference *symref = new (compilation->trHeapMemory()) TR::SymbolReference(compilation->getSymRefTab(), sym, compilation->getMethodSymbol()->getResolvedMethodIndex(), -1);
         symref->setOffset(offset);
         node = TR::Node::createWithSymRef(opcode.getOpCodeValue(), childCount, symref);
     }
     else if (opcode.isIf()) {
         const auto targetName = tree->args->value.value.str;
         auto targetId = _blockMap[targetName];
         auto targetEntry = _blocks[targetId]->getEntry();
         TraceIL("  is if with target block %d (%s, entry = %p", targetId, targetName, targetEntry);
         auto c1 = TR::Node::create(TR::BadILOp);
         auto c2 = c1;
         TraceIL("  created temporary %s n%dn (%p)\n", c1->getOpCode().getName(), c1->getGlobalIndex(), c1);
         node = TR::Node::createif(opcode.getOpCodeValue(), c1, c2, targetEntry);
     }
     else if (opcode.isBranch()) {
         const auto targetName = tree->args->value.value.str;
         auto targetId = _blockMap[targetName];
         auto targetEntry = _blocks[targetId]->getEntry();
         TraceIL("  is branch to target block %d (%s, entry = %p", targetId, targetName, targetEntry);
         node = TR::Node::create(opcode.getOpCodeValue(), childCount);
         node->setBranchDestination(targetEntry);
     }
     else {
        TraceIL("  unrecognized opcode; using default creation mechanism\n", "");
        node = TR::Node::create(opcode.getOpCodeValue(), childCount);
     }
     TraceIL("  node address %p\n", node);
     TraceIL("  node index n%dn\n", node->getGlobalIndex());

     const ASTNode* t = tree->children;
     int i = 0;
     while (t) {
         auto child = toTRNode(t);
         TraceIL("Setting n%dn (%p) as child %d of n%dn (%p)\n", child->getGlobalIndex(), child, i, node->getGlobalIndex(), node);
         node->setAndIncChild(i, child);
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
           auto node = toTRNode(t);
           const auto tt = genTreeTop(node);
           TraceIL("Created TreeTop %p for node n%dn (%p)\n", tt, node->getGlobalIndex(), node);
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
