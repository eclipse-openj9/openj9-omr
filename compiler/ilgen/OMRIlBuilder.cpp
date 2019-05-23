/*******************************************************************************
 * Copyright (c) 2000, 2019 IBM Corp. and others
 *
 * This program and the accompanying materials are made available under
 * the terms of the Eclipse Public License 2.0 which accompanies this
 * distribution and is available at http://eclipse.org/legal/epl-2.0
 * or the Apache License, Version 2.0 which accompanies this distribution
 * and is available at https://www.apache.org/licenses/LICENSE-2.0.
 *
 * This Source Code may also be made available under the following Secondary
 * Licenses when the conditions for such availability set forth in the
 * Eclipse Public License, v. 2.0 are satisfied: GNU General Public License,
 * version 2 with the GNU Classpath Exception [1] and GNU General Public
 * License, version 2 with the OpenJDK Assembly Exception [2].
 *
 * [1] https://www.gnu.org/software/classpath/license.html
 * [2] http://openjdk.java.net/legal/assembly-exception.html
 *
 * SPDX-License-Identifier: EPL-2.0 OR Apache-2.0 OR GPL-2.0 WITH Classpath-exception-2.0 OR LicenseRef-GPL-2.0 WITH Assembly-exception
 *******************************************************************************/

#include "env/TRMemory.hpp"    // must precede IlBuilder.hpp to get TR_ALLOC
#include "ilgen/IlBuilder.hpp"

#include <stdint.h>
#include <stdarg.h>
#include <string.h>
#include "codegen/CodeGenerator.hpp"
#include "compile/Compilation.hpp"
#include "compile/Method.hpp"
#include "compile/SymbolReferenceTable.hpp"
#include "control/Recompilation.hpp"
#include "env/CompilerEnv.hpp"
#include "env/FrontEnd.hpp"
#include "il/Block.hpp"
#include "il/SymbolReference.hpp"
#include "il/ILOps.hpp"
#include "il/Node.hpp"
#include "il/Node_inlines.hpp"
#include "il/TreeTop.hpp"
#include "il/TreeTop_inlines.hpp"
#include "il/symbol/AutomaticSymbol.hpp"
#include "ilgen/IlGeneratorMethodDetails_inlines.hpp"
#include "ilgen/TypeDictionary.hpp"
#include "ilgen/IlInjector.hpp"
#include "ilgen/IlReference.hpp"
#include "ilgen/MethodBuilder.hpp"
#include "ilgen/BytecodeBuilder.hpp"
#include "infra/Cfg.hpp"
#include "infra/List.hpp"

#define OPT_DETAILS "O^O ILBLD: "

#define TraceEnabled    (comp()->getOption(TR_TraceILGen))
#define TraceIL(m, ...) {if (TraceEnabled) {traceMsg(comp(), m, ##__VA_ARGS__);}}


// IlBuilder is a class designed to help build Testarossa IL quickly without
// a lot of knowledge of the intricacies of commoned references, symbols,
// symbol references, or blocks. You can add operations to an IlBuilder via
// a set of services, for example: Add, Sub, Load, Store. These services
// operate on TR::IlValues, which currently correspond to SymbolReferences.
// However, IlBuilders also incorporate a notion of "symbols" that are
// identified by strings (i.e. arbitrary symbolic names).
//
// So Load("a") returns a TR::IlValue. Load("b") returns a different
// TR::IlValue. The value of a+b can be computed by Add(Load("a"),Load("b")),
// and that value can be stored into c by Store("c", Add(Load("a"),Load("b"))).
//
// More complicated services exist to construct control flow by linking
// together sets of IlBuilder objects. A simple if-then construct, for
// example, can be built with the following code:
//    TR::IlValue *condition = NotEqualToZero(Load("a"));
//    TR::IlBuilder *then = NULL;
//    IfThen(&then, condition);
//    then->Return(then->ConstInt32(0));
//
//
// An IlBuilder is really a sequence of Blocks and other IlBuilders, but an
// IlBuilder is also a sequence of TreeTops connecting together a set of
// Blocks that are arranged in the CFG. Another way to think of an IlBuilder
// is as representing the code needed to execute a particular code path
// (which may itself have sub control flow handled by embedded IlBuilders).
// All IlBuilders exist within the single control flow graph, but the full
// control flow graph will not be connected together until all IlBuilder
// objects have had injectIL() called.


OMR::IlBuilder::IlBuilder(TR::IlBuilder *source)
   : TR::IlInjector(source),
   _client(0),
   _clientCallbackBuildIL(0),
   _methodBuilder(source->_methodBuilder),
   _sequence(0),
   _sequenceAppender(0),
   _entryBlock(0),
   _exitBlock(0),
   _count(-1),
   _partOfSequence(false),
   _connectedTrees(false),
   _comesBack(true)
   {
   }

bool
OMR::IlBuilder::TraceEnabled_log(){
    bool traceEnabled = _comp->getOption(TR_TraceILGen);
    return traceEnabled;
}

void
OMR::IlBuilder::TraceIL_log(const char* s, ...){
    va_list argp;
    va_start (argp, s);
    traceMsgVarArgs(_comp, s, argp);
    va_end(argp);
}

void
OMR::IlBuilder::initSequence()
   {
   _sequence = new (_comp->trMemory()->trHeapMemory()) List<SequenceEntry>(_comp->trMemory());
   _sequenceAppender = new (_comp->trMemory()->trHeapMemory()) ListAppender<SequenceEntry>(_sequence);
   }

bool
OMR::IlBuilder::injectIL()
   {
   TraceIL("Inside injectIL()\n");
   TraceIL("original entry %p\n", cfg()->getStart());
   TraceIL("original exit %p\n", cfg()->getEnd());

   setupForBuildIL();

   bool rc = buildIL();
   TraceIL("buildIL() returned %d\n", rc);
   if (!rc)
      return false;

   rc = connectTrees();
   if (TraceEnabled)
      comp()->dumpMethodTrees("after connectTrees");
   cfg()->removeUnreachableBlocks();
   if (TraceEnabled)
      comp()->dumpMethodTrees("after removing unreachable blocks");
   return rc;
   }

void
OMR::IlBuilder::setupForBuildIL()
   {
   initSequence();
   appendBlock(NULL, false);
   _entryBlock = _currentBlock;
   _exitBlock = emptyBlock();
   }

void
OMR::IlBuilder::print(const char *title, bool recurse)
   {
   if (!TraceEnabled)
      return;

   if (title != NULL)
      TraceIL("[ %p ] %s\n", this, title);
   TraceIL("[ %p ] _methodBuilder %p\n", this, _methodBuilder);
   TraceIL("[ %p ] _entryBlock %p\n", this, _entryBlock);
   TraceIL("[ %p ] _exitBlock %p\n", this, _exitBlock);
   TraceIL("[ %p ] _connectedBlocks %d\n", this, _connectedTrees);
   TraceIL("[ %p ] _comesBack %d\n", this, _comesBack);
   TraceIL("[ %p ] Sequence:\n", this);
   ListIterator<SequenceEntry> iter(_sequence);
   for (SequenceEntry *entry = iter.getFirst(); !iter.atEnd(); entry = iter.getNext())
      {
      if (entry->_isBlock)
         {
         printBlock(entry->_block);
         }
      else
         {
         if (recurse)
            {
            TraceIL("[ %p ] Inner builder %p", this, entry->_builder);
            entry->_builder->print(NULL, recurse);
            }
         else
            {
            TraceIL("[ %p ] Builder %p\n", this, entry->_builder);
            }
         }
      }
   }

void
OMR::IlBuilder::printBlock(TR::Block *block)
   {
   if (!TraceEnabled)
      return;

   TraceIL("[ %p ] Block %p\n", this, block);
   TR::TreeTop *tt = block->getEntry();
   while (tt != block->getExit())
      {
      comp()->getDebug()->print(comp()->getOutFile(), tt);
      tt = tt->getNextTreeTop();
      }
   comp()->getDebug()->print(comp()->getOutFile(), tt);
   }

TR::SymbolReference *
OMR::IlBuilder::lookupSymbol(const char *name)
   {
   TR_ASSERT(_methodBuilder, "cannot look up symbols in an IlBuilder that has no MethodBuilder");
   return _methodBuilder->lookupSymbol(name);
   }

void
OMR::IlBuilder::defineSymbol(const char *name, TR::SymbolReference *symRef)
   {
   TR_ASSERT(_methodBuilder, "cannot define symbols in an IlBuilder that has no MethodBuilder");
   _methodBuilder->defineSymbol(name, symRef);
   }

void
OMR::IlBuilder::defineValue(const char *name, TR::IlType *type)
   {
   TR::DataType dt = type->getPrimitiveType();
   TR::SymbolReference *newSymRef = symRefTab()->createTemporary(methodSymbol(), dt);
   newSymRef->getSymbol()->setNotCollected();
   defineSymbol(name, newSymRef);
   }

TR::IlValue *
OMR::IlBuilder::newValue(TR::DataType dt, TR::Node *n)
   {
   // make sure TreeTop is well formed
   TR::Node *ttNode = n;
   if (!ttNode->getOpCode().isTreeTop())
      ttNode = TR::Node::create(TR::treetop, 1, n);

   TR::TreeTop *tt = TR::TreeTop::create(_comp, ttNode);
   _currentBlock->append(tt);
   TR::IlValue *value = new (_comp->trHeapMemory()) TR::IlValue(n, tt, _currentBlock, _methodBuilder);
   return value;
   }

TR::IlValue *
OMR::IlBuilder::newValue(TR::IlType *dt, TR::Node *n)
   {
   return newValue(dt->getPrimitiveType(), n);
   }

TR::IlValue *
OMR::IlBuilder::NewValue(TR::IlType *dt)
   {
   TR_ASSERT_FATAL(0, "should not create a value without a TR::Node");
   }

TR::IlValue *
OMR::IlBuilder::Copy(TR::IlValue *value)
   {
   TR::DataType dt = value->getDataType();
   TR::SymbolReference *newSymRef = symRefTab()->createTemporary(_methodSymbol, dt);
   char *name = (char *) _comp->trMemory()->allocateHeapMemory((2+10+1) * sizeof(char)); // 2 ("_T") + max 10 digits + trailing zero
   sprintf(name, "_T%u", newSymRef->getCPIndex());
   newSymRef->getSymbol()->getAutoSymbol()->setName(name);
   newSymRef->getSymbol()->setNotCollected();
   _methodBuilder->defineSymbol(name, newSymRef);

   storeToTemp(newSymRef, loadValue(value));

   TR::IlValue *newVal = newValue(newSymRef->getSymbol()->getDataType(), loadTemp(newSymRef));

   TraceIL("IlBuilder[ %p ]::Copy value (%d) dataType (%d) to newVal (%d) at cpIndex (%d)\n", this, value->getID(), dt, newVal->getID(), newSymRef->getCPIndex());

   return newVal;
   }

TR::TreeTop *
OMR::IlBuilder::getFirstTree()
   {
   TR_ASSERT(_blocks, "_blocks not created yet");
   TR_ASSERT(_blocks[0], "_blocks[0] not created yet");
   return _blocks[0]->getEntry();
   }

TR::TreeTop *
OMR::IlBuilder::getLastTree()
   {
   TR_ASSERT(_blocks, "_blocks not created yet");
   TR_ASSERT(_blocks[_numBlocks], "_blocks[0] not created yet");
   return _blocks[_numBlocks]->getExit();
   }

uint32_t
OMR::IlBuilder::countBlocks()
   {
   // only count each block once
   if (_count > -1)
      return _count;

   TraceIL("[ %p ] TR::IlBuilder::countBlocks 0 at entry\n", this);

   _count = 0; // prevent recursive counting; will be updated to real value before returning

   uint32_t count=0;
   ListIterator<SequenceEntry> iter(_sequence);
   for (SequenceEntry *entry = iter.getFirst(); !iter.atEnd(); entry = iter.getNext())
      {
      if (entry->_isBlock)
         count++;
      else
         count += entry->_builder->countBlocks();
      }

   if (this != _methodBuilder || _methodBuilder->callerMethodBuilder() != NULL)
      {
      // exit block isn't in the sequence except for method builders
      //            gets tacked in late by connectTrees
      count++;
      }

   TraceIL("[ %p ] TR::IlBuilder::countBlocks %d\n", this, count);

   _count = count;
   return count;
   }

void
OMR::IlBuilder::pullInBuilderTrees(TR::IlBuilder *builder,
                                   uint32_t *currentBlock,
                                   TR::TreeTop **firstTree,
                                   TR::TreeTop **newLastTree)
   {
   TraceIL("\n[ %p ] Calling connectTrees on inner builder %p\n", this, builder);
   builder->connectTrees();
   TraceIL("\n[ %p ] Returned from connectTrees on %p\n", this, builder);

   TR::Block **innerBlocks = builder->blocks();
   uint32_t innerNumBlocks = builder->numBlocks();

   uint32_t copyBlock = *currentBlock;
   for (uint32_t i=0;i < innerNumBlocks;i++, copyBlock++)
      {
      if (TraceEnabled)
         {
         TraceIL("[ %p ] copying inner block B%d to B%d\n", this, i, copyBlock);
         printBlock(innerBlocks[i]);
         }
      _blocks[copyBlock] = innerBlocks[i];
      }

   *currentBlock += innerNumBlocks;
   *firstTree = innerBlocks[0]->getEntry();
   *newLastTree = innerBlocks[innerNumBlocks-1]->getExit();
   }

bool
OMR::IlBuilder::connectTrees()
   {
   // don't do this more than once per builder object
   if (_connectedTrees)
      return true;

   _connectedTrees = true;

   TraceIL("[ %p ] TR::IlBuilder::connectTrees():\n", this);

   // figure out how many blocks we need
   uint32_t numBlocks = countBlocks();
   TraceIL("[ %p ] Total %d blocks\n", this, numBlocks);

   allocateBlocks(numBlocks);
   TraceIL("[ %p ] Allocated _blocks %p\n", this, _blocks);

   // now add all the blocks from the sequence into the _blocks array
   // and connect the trees into a single sequence
   uint32_t currentBlock = 0;
   TR::Block **blocks = _blocks;
   TR::TreeTop *lastTree = NULL;

   TraceIL("[ %p ] Connecting trees one entry at a time:\n", this);
   ListIterator<SequenceEntry> iter(_sequence);
   for (SequenceEntry *entry = iter.getFirst(); !iter.atEnd(); entry = iter.getNext())
      {
      TraceIL("[ %p ] currentBlock = %d\n", this, currentBlock);
      TraceIL("[ %p ] lastTree = %p\n", this, lastTree);
      TR::TreeTop *firstTree = NULL;
      TR::TreeTop *newLastTree = NULL;
      if (entry->_isBlock)
         {
         if (TraceEnabled)
            {
            TraceIL("[ %p ] Block entry %p becomes block B%d\n", this, entry->_block, currentBlock);
            printBlock(entry->_block);
            }
         blocks[currentBlock++] = entry->_block;
         firstTree = entry->_block->getEntry();
         newLastTree = entry->_block->getExit();
         }
      else
         {
         TR::IlBuilder *builder = entry->_builder;
         pullInBuilderTrees(builder, &currentBlock, &firstTree, &newLastTree);
         }

      TraceIL("[ %p ] First tree is %p [ node %p ]\n", this, firstTree, firstTree->getNode());
      TraceIL("[ %p ] Last tree will be %p [ node %p ]\n", this, newLastTree, newLastTree->getNode());

      // connect the trees
      if (lastTree)
         {
         TraceIL("[ %p ] Connecting tree %p [ node %p ] to new tree %p [ node %p ]\n", this, lastTree, lastTree->getNode(), firstTree, firstTree->getNode());
         lastTree->join(firstTree);
         }

      lastTree = newLastTree;
      }

   if (_methodBuilder != this || _methodBuilder->callerMethodBuilder() != NULL)
      {
      // non-method builders need to append the "EXIT" block trees
      // (method builders have EXIT as the special block 1)
      TraceIL("[ %p ] Connecting last tree %p [ node %p ] to exit block entry %p [ node %p ]\n", this, lastTree, lastTree->getNode(), _exitBlock->getEntry(), _exitBlock->getEntry()->getNode());
      lastTree->join(_exitBlock->getEntry());

      // also add exit block to blocks array and add an edge from last block to EXIT if the builder comes back (i.e. doesn't return or branch off somewhere)
      if (comesBack())
         cfg()->addEdge(blocks[currentBlock-1], getExit());

      blocks[currentBlock++] = _exitBlock;
      TraceIL("[ %p ] exit block %d is %p (%p -> %p)\n", this, _exitBlock->getNumber(), _exitBlock->getEntry(), _exitBlock->getExit());
      lastTree = _exitBlock->getExit();

      _numBlocks = currentBlock;
      }

   TraceIL("[ %p ] last tree %p [ node %p ]\n", this, lastTree, lastTree->getNode());
   lastTree->setNextTreeTop(NULL);
   TraceIL("[ %p ] blocks[%d] exit tree is %p\n", this, currentBlock-1, blocks[currentBlock-1]->getExit());

   return true;
   }

TR::Node *
OMR::IlBuilder::zero(TR::DataType dt)
   {
   switch (dt)
      {
      case TR::Int8 :  return TR::Node::bconst(0);
      case TR::Int16 : return TR::Node::sconst(0);
      case TR::Int32 : return TR::Node::iconst(0);
      case TR::Int64 : return TR::Node::lconst(0);
      default :        return TR::Node::create(TR::ILOpCode::constOpCode(dt), 0, 0);
      }
   TR_ASSERT(0, "should not reach here");
   }

TR::Node *
OMR::IlBuilder::zero(TR::IlType *dt)
   {
   return zero(dt->getPrimitiveType());
   }

TR::Node *
OMR::IlBuilder::zeroNodeForValue(TR::IlValue *v)
   {
   return zero(v->getDataType());
   }

TR::IlValue *
OMR::IlBuilder::zeroForValue(TR::IlValue *v)
   {
   TR::IlValue *returnValue = newValue(v->getDataType(), zeroNodeForValue(v));
   return returnValue;
   }


TR::IlBuilder *
OMR::IlBuilder::OrphanBuilder()
   {
   TR::IlBuilder *orphan = new (comp()->trHeapMemory()) TR::IlBuilder(_methodBuilder, _types);
   orphan->initialize(_details, _methodSymbol, _fe, _symRefTab);
   orphan->setupForBuildIL();
   return orphan;
   }

TR::BytecodeBuilder *
OMR::IlBuilder::OrphanBytecodeBuilder(int32_t bcIndex, char *name)
   {
   TR::BytecodeBuilder *orphan = new (comp()->trHeapMemory()) TR::BytecodeBuilder(_methodBuilder, bcIndex, name);
   orphan->initialize(_details, _methodSymbol, _fe, _symRefTab);
   orphan->setupForBuildIL();
   return orphan;
   }

TR::Block *
OMR::IlBuilder::emptyBlock()
   {
   TR::Block *empty = TR::Block::createEmptyBlock(NULL, _comp);
   cfg()->addNode(empty);
   return empty;
   }


TR::IlBuilder *
OMR::IlBuilder::createBuilderIfNeeded(TR::IlBuilder *builder)
   {
   if (builder == NULL)
      builder = OrphanBuilder();
   return builder;
   }

OMR::IlBuilder::SequenceEntry *
OMR::IlBuilder::blockEntry(TR::Block *block)
   {
   return new (_comp->trMemory()->trHeapMemory()) IlBuilder::SequenceEntry(block);
   }

OMR::IlBuilder::SequenceEntry *
OMR::IlBuilder::builderEntry(TR::IlBuilder *builder)
   {
   return new (_comp->trMemory()->trHeapMemory()) IlBuilder::SequenceEntry(builder);
   }

void
OMR::IlBuilder::appendBlock(TR::Block *newBlock, bool addEdge)
   {
   if (newBlock == NULL)
      {
      newBlock = emptyBlock();
      }

   _sequenceAppender->add(blockEntry(newBlock));

   if (_currentBlock && addEdge)
      {
      // if current block does not fall through to new block, use appendNoFallThroughBlock() rather than appendBlock()
      cfg()->addEdge(_currentBlock, newBlock);
      }

   // subsequent IL should generate to appended block
   _currentBlock = newBlock;
   }

void
OMR::IlBuilder::AppendBuilder(TR::IlBuilder *builder)
   {
   TR_ASSERT(builder->_partOfSequence == false, "builder cannot be in two places");

   builder->_partOfSequence = true;
   _sequenceAppender->add(builderEntry(builder));
   if (_currentBlock != NULL)
      {
      cfg()->addEdge(_currentBlock, builder->getEntry());
      _currentBlock = NULL;
      }

   TraceIL("IlBuilder[ %p ]::AppendBuilder %p\n", this, builder);

   // when trees are connected, exit block will swing down to become last trees in builder, so it can fall through to this block we're about to create
   // need to add edge explicitly because of this exit block sleight of hand
   appendNoFallThroughBlock();
   cfg()->addEdge(builder->getExit(), _currentBlock);
   }

TR::Node *
OMR::IlBuilder::loadValue(TR::IlValue *v)
   {
   return v->load(_currentBlock);
   }

void
OMR::IlBuilder::storeNode(TR::SymbolReference *symRef, TR::Node *v)
   {
   genTreeTop(TR::Node::createStore(symRef, v));
   }

void
OMR::IlBuilder::indirectStoreNode(TR::Node *addr, TR::Node *v)
   {
   TR::DataType dt = v->getDataType();
   TR::SymbolReference *storeSymRef = symRefTab()->findOrCreateArrayShadowSymbolRef(dt, addr);
   TR::ILOpCodes storeOp = comp()->il.opCodeForIndirectArrayStore(dt);
   genTreeTop(TR::Node::createWithSymRef(storeOp, 2, addr, v, 0, storeSymRef));
   }

TR::IlValue *
OMR::IlBuilder::indirectLoadNode(TR::IlType *dt, TR::Node *addr, bool isVectorLoad)
   {
   TR_ASSERT(dt->isPointer(), "indirectLoadNode must apply to pointer type");
   TR::IlType * baseType = dt->baseType();
   TR::DataType primType = baseType->getPrimitiveType();
   TR_ASSERT(primType != TR::NoType, "Dereferencing an untyped pointer.");
   TR::DataType symRefType = primType;
   if (isVectorLoad)
      symRefType = symRefType.scalarToVector();

   TR::SymbolReference *storeSymRef = symRefTab()->findOrCreateArrayShadowSymbolRef(symRefType, addr);

   TR::ILOpCodes loadOp = comp()->il.opCodeForIndirectArrayLoad(primType);
   if (isVectorLoad)
      {
      loadOp = TR::ILOpCode::convertScalarToVector(loadOp);
      baseType = _types->PrimitiveType(symRefType);
      }

   TR::Node *loadNode = TR::Node::createWithSymRef(loadOp, 1, 1, addr, storeSymRef);

   TR::IlValue *loadValue = newValue(baseType, loadNode);
   return loadValue;
   }

/**
 * @brief Store an IlValue into a named local variable
 * @param varName the name of the local variable to be stored into. If the name has not been used before, this local variable will
 *                take the same data type as the value being written to it.
 * @param value IlValue that should be written to the local variable, which should be the same data type
 */
void
OMR::IlBuilder::Store(const char *varName, TR::IlValue *value)
   {
   if (!_methodBuilder->symbolDefined(varName))
      _methodBuilder->defineValue(varName, _types->PrimitiveType(value->getDataType()));
   TR::SymbolReference *symRef = lookupSymbol(varName);

   TraceIL("IlBuilder[ %p ]::Store %s %d (%d) gets %d\n", this, varName, symRef->getCPIndex(), symRef->getReferenceNumber(), value->getID());
   storeNode(symRef, loadValue(value));
   }

/**
 * @brief Store an IlValue into the same local value as another IlValue
 * @param dest IlValue that should now hold the same value as "value"
 * @param value IlValue that should overwrite "dest"
 */
void
OMR::IlBuilder::StoreOver(TR::IlValue *dest, TR::IlValue *value)
   {
   TraceIL("IlBuilder[ %p ]::StoreOver %d gets %d\n", this, dest->getID(), value->getID());
   dest->storeOver(value, _currentBlock);
   }

/**
 * @brief Store a vector IlValue into a named local variable
 * @param varName the name of the local variable to be vector stored into. if the name has not been used before, this local variable will
 *                take the same data type as the value being written to it. The width of this data will be determined by the vector
 *                data type of IlValue.
 * @param value IlValue with the vector data that should be written to the local variable, and should have the same data type
 */
void
OMR::IlBuilder::VectorStore(const char *varName, TR::IlValue *value)
   {
   TR::Node *valueNode = loadValue(value);
   TR::DataType dt = valueNode->getDataType();
   if (!dt.isVector())
      {
      valueNode = TR::Node::create(TR::vsplats, 1, valueNode);
      dt = dt.scalarToVector();
      }

   if (!_methodBuilder->symbolDefined(varName))
      _methodBuilder->defineValue(varName, _types->PrimitiveType(dt));
   TR::SymbolReference *symRef = lookupSymbol(varName);

   TraceIL("IlBuilder[ %p ]::VectorStore %s %d gets %d\n", this, varName, symRef->getCPIndex(), value->getID());
   storeNode(symRef, valueNode);
   }

/**
 * @brief Store an IlValue through a pointer
 * @param address the pointer address through which the value will be written
 * @param value IlValue that should be written at "address"
 */
void
OMR::IlBuilder::StoreAt(TR::IlValue *address, TR::IlValue *value)
   {
   TR_ASSERT(address->getDataType() == TR::Address, "StoreAt needs an address operand");

   TraceIL("IlBuilder[ %p ]::StoreAt address %d gets %d\n", this, address->getID(), value->getID());
   indirectStoreNode(loadValue(address), loadValue(value));
   }

/**
 * @brief Store a vector IlValue through a pointer
 * @param address the pointer address through which the vector value will be written. The width of the store will be determined
 *                by the vector data type of IlValue.
 * @param value IlValue with the vector data that should be written  at "address"
 */
void
OMR::IlBuilder::VectorStoreAt(TR::IlValue *address, TR::IlValue *value)
   {
   TR_ASSERT(address->getDataType() == TR::Address, "VectorStoreAt needs an address operand");

   TraceIL("IlBuilder[ %p ]::VectorStoreAt address %d gets %d\n", this, address->getID(), value->getID());

   TR::Node *valueNode = loadValue(value);

   if (!valueNode->getDataType().isVector())
      valueNode = TR::Node::create(TR::vsplats, 1, valueNode);

   indirectStoreNode(loadValue(address), valueNode);
   }

TR::IlValue *
OMR::IlBuilder::CreateLocalArray(int32_t numElements, TR::IlType *elementType)
   {
   uint32_t size = numElements * elementType->getSize();
   TR::SymbolReference *localArraySymRef = symRefTab()->createLocalPrimArray(size,
                                                                             methodSymbol(),
                                                                             8 /*FIXME: JVM-specific - byte*/);
   char *name = (char *) _comp->trMemory()->allocateHeapMemory((2+10+1) * sizeof(char)); // 2 ("_T") + max 10 digits + trailing zero
   sprintf(name, "_T%u", localArraySymRef->getCPIndex());
   localArraySymRef->getSymbol()->getAutoSymbol()->setName(name);
   localArraySymRef->setStackAllocatedArrayAccess();
   _methodBuilder->defineSymbol(name, localArraySymRef);

   TR::Node *arrayAddress = TR::Node::createWithSymRef(TR::loadaddr, 0, localArraySymRef);
   TR::IlValue *arrayAddressValue = newValue(TR::Address, arrayAddress);

   TraceIL("IlBuilder[ %p ]::CreateLocalArray array allocated %d bytes, address in %d\n", this, size, arrayAddressValue->getID());
   return arrayAddressValue;

   }

TR::IlValue *
OMR::IlBuilder::CreateLocalStruct(TR::IlType *structType)
   {
   //similar to CreateLocalArray except writing a method in StructType to get the struct size
   uint32_t size = structType->getSize();
   TR::SymbolReference *localStructSymRef = symRefTab()->createLocalPrimArray(size,
                                                                             methodSymbol(),
                                                                             8 /*FIXME: JVM-specific - byte*/);
   char *name = (char *) _comp->trMemory()->allocateHeapMemory((2+10+1) * sizeof(char)); // 2 ("_T") + max 10 digits + trailing zero
   sprintf(name, "_T%u", localStructSymRef->getCPIndex());
   localStructSymRef->getSymbol()->getAutoSymbol()->setName(name);
   localStructSymRef->setStackAllocatedArrayAccess();
   _methodBuilder->defineSymbol(name, localStructSymRef);

   TR::Node *structAddress = TR::Node::createWithSymRef(TR::loadaddr, 0, localStructSymRef);
   TR::IlValue *structAddressValue = newValue(TR::Address, structAddress);

   TraceIL("IlBuilder[ %p ]::CreateLocalStruct struct allocated %d bytes, address in %d\n", this, size, structAddressValue->getID());
   return structAddressValue;
   }

void
OMR::IlBuilder::StoreIndirect(const char *type, const char *field, TR::IlValue *object, TR::IlValue *value)
   {
   TR::IlReference *fieldRef = _types->FieldReference(type, field);
   TR::SymbolReference *symRef = fieldRef->symRef();
   TR::DataType fieldType = symRef->getSymbol()->getDataType();
   TraceIL("IlBuilder[ %p ]::StoreIndirect %s.%s (%d) into (%d)\n", this, type, field, value->getID(), object->getID());
   TR::ILOpCodes storeOp = comp()->il.opCodeForIndirectStore(fieldType);
   genTreeTop(TR::Node::createWithSymRef(storeOp, 2, loadValue(object), loadValue(value), 0, symRef));
   }

TR::IlValue *
OMR::IlBuilder::Load(const char *name)
   {
   TR::SymbolReference *symRef = lookupSymbol(name);
   TR::Node *valueNode = TR::Node::createLoad(symRef);
   TR::IlValue *returnValue = newValue(symRef->getSymbol()->getDataType(), valueNode);
   TraceIL("IlBuilder[ %p ]::Load %s into %d from symref %d\n", this, name, returnValue->getID(), symRef->getReferenceNumber());
   return returnValue;
   }

TR::IlValue *
OMR::IlBuilder::VectorLoad(const char *name)
   {
   TR::SymbolReference *nameSymRef = lookupSymbol(name);
   TR::DataType returnType = nameSymRef->getSymbol()->getDataType();
   TR_ASSERT(returnType.isVector(), "VectorLoad must load symbol with a vector type");

   TR::Node *loadNode = TR::Node::createWithSymRef(0, TR::comp()->il.opCodeForDirectLoad(returnType), 0, nameSymRef);
   TR::IlValue *returnValue = newValue(returnType, loadNode);
   TraceIL("IlBuilder[ %p ]::%d is VectorLoad %s (%d)\n", this, returnValue->getID(), name, nameSymRef->getCPIndex());

   return returnValue;
   }

TR::IlValue *
OMR::IlBuilder::LoadIndirect(const char *type, const char *field, TR::IlValue *object)
   {
   TR::IlReference *fieldRef = _types->FieldReference(type, field);
   TR::SymbolReference *symRef = fieldRef->symRef();
   TR::DataType fieldType = symRef->getSymbol()->getDataType();
   TR::IlValue *returnValue = newValue(fieldType, TR::Node::createWithSymRef(comp()->il.opCodeForIndirectLoad(fieldType), 1, loadValue(object), 0, symRef));
   TraceIL("IlBuilder[ %p ]::%d is LoadIndirect %s.%s from (%d)\n", this, returnValue->getID(), type, field, object->getID());
   return returnValue;
   }

TR::IlValue *
OMR::IlBuilder::LoadAt(TR::IlType *dt, TR::IlValue *address)
   {
   TR_ASSERT(address->getDataType() == TR::Address, "LoadAt needs an address operand");
   TR::IlValue *returnValue = indirectLoadNode(dt, loadValue(address));
   TraceIL("IlBuilder[ %p ]::%d is LoadAt type %d address %d\n", this, returnValue->getID(), dt->getPrimitiveType(), address->getID());
   return returnValue;
   }

TR::IlValue *
OMR::IlBuilder::VectorLoadAt(TR::IlType *dt, TR::IlValue *address)
   {
   TR_ASSERT(address->getDataType() == TR::Address, "LoadAt needs an address operand");
   TR::IlValue *returnValue = indirectLoadNode(dt, loadValue(address), true);
   TraceIL("IlBuilder[ %p ]::%d is VectorLoadAt type %d address %d\n", this, returnValue->getID(), dt->getPrimitiveType(), address->getID());
   return returnValue;
   }

TR::IlValue *
OMR::IlBuilder::IndexAt(TR::IlType *dt, TR::IlValue *base, TR::IlValue *index)
   {
   TR::IlType *elemType = dt->baseType();
   TR_ASSERT(base->getDataType() == TR::Address, "IndexAt must be called with a pointer base");
   TR_ASSERT(elemType != NULL, "IndexAt should be called with pointer type");
   TR_ASSERT(elemType->getPrimitiveType() != TR::NoType, "Cannot use IndexAt with pointer to NoType.");
   TR::Node *baseNode = loadValue(base);
   TR::Node *indexNode = loadValue(index);
   TR::Node *elemSizeNode;
   TR::ILOpCodes addOp, mulOp;
   TR::DataType indexType = indexNode->getDataType();
   if (TR::Compiler->target.is64Bit())
      {
      if (indexType != TR::Int64)
         {
         TR::ILOpCodes op = TR::DataType::getDataTypeConversion(indexType, TR::Int64);
         indexNode = TR::Node::create(op, 1, indexNode);
         }
      elemSizeNode = TR::Node::lconst(elemType->getSize());
      addOp = TR::aladd;
      mulOp = TR::lmul;
      }
   else
      {
      TR::DataType targetType = TR::Int32;
      if (indexType != targetType)
         {
         TR::ILOpCodes op = TR::DataType::getDataTypeConversion(indexType, targetType);
         indexNode = TR::Node::create(op, 1, indexNode);
         }
      elemSizeNode = TR::Node::iconst(elemType->getSize());
      addOp = TR::aiadd;
      mulOp = TR::imul;
      }

   TR::Node *offsetNode = TR::Node::create(mulOp, 2, indexNode, elemSizeNode);
   TR::Node *addrNode = TR::Node::create(addOp, 2, baseNode, offsetNode);

   TR::IlValue *address = newValue(Address, addrNode);

   TraceIL("IlBuilder[ %p ]::%d is IndexAt(%s) base %d index %d\n", this, address->getID(), dt->getName(), base->getID(), index->getID());

   return address;
   }

/**
 * @brief Generate IL to load the address of a struct field.
 *
 * The address of the field is calculated by adding the field's offset to the
 * base address of the struct. The address is also converted (type casted) to
 * a pointer to the type of the field.
 */
TR::IlValue *
OMR::IlBuilder::StructFieldInstanceAddress(const char* structName, const char* fieldName, TR::IlValue* obj) {
   auto offset = typeDictionary()->OffsetOf(structName, fieldName);
   auto ptype = typeDictionary()->PointerTo(typeDictionary()->GetFieldType(structName, fieldName));
   TR::IlValue* offsetValue = NULL;
   if (TR::Compiler->target.is64Bit())
      {
      offsetValue = ConstInt64(offset);
      }
   else
      {
      offsetValue = ConstInt32(offset);
      }
   auto addr = Add(obj, offsetValue);
   return ConvertTo(ptype, addr);
}

/**
 * @brief Generate IL to load the address of a union field.
 *
 * The address of the field is simply the base address of the union, since the
 * offset of all union fields is zero.
 */
TR::IlValue *
OMR::IlBuilder::UnionFieldInstanceAddress(const char* unionName, const char* fieldName, TR::IlValue* obj) {
   auto ptype = typeDictionary()->PointerTo(typeDictionary()->UnionFieldType(unionName, fieldName));
   return ConvertTo(ptype, obj);
}

TR::IlValue *
OMR::IlBuilder::NullAddress()
   {
   TR::IlValue *returnValue = newValue(Address, TR::Node::aconst(0));
   TraceIL("IlBuilder[ %p ]::%d is NullAddress\n", this, returnValue->getID());
   return returnValue;
   }

TR::IlValue *
OMR::IlBuilder::ConstInt8(int8_t value)
   {
   TR::IlValue *returnValue = newValue(Int8, TR::Node::bconst(value));
   TraceIL("IlBuilder[ %p ]::%d is ConstInt8 %d\n", this, returnValue->getID(), value);
   return returnValue;
   }

TR::IlValue *
OMR::IlBuilder::ConstInt16(int16_t value)
   {
   TR::IlValue *returnValue = newValue(Int16, TR::Node::sconst(value));
   TraceIL("IlBuilder[ %p ]::%d is ConstInt16 %d\n", this, returnValue->getID(), value);
   return returnValue;
   }

TR::IlValue *
OMR::IlBuilder::ConstInt32(int32_t value)
   {
   TR::IlValue *returnValue = newValue(Int32, TR::Node::iconst(value));
   TraceIL("IlBuilder[ %p ]::%d is ConstInt32 %d\n", this, returnValue->getID(), value);
   return returnValue;
   }

TR::IlValue *
OMR::IlBuilder::ConstInt64(int64_t value)
   {
   TR::IlValue *returnValue = newValue(Int64, TR::Node::lconst(value));
   TraceIL("IlBuilder[ %p ]::%d is ConstInt64 %d\n", this, returnValue->getID(), value);
   return returnValue;
   }

TR::IlValue *
OMR::IlBuilder::ConstFloat(float value)
   {
   TR::Node *fconstNode = TR::Node::create(0, TR::fconst, 0);
   fconstNode->setFloat(value);
   TR::IlValue *returnValue = newValue(Float, fconstNode);
   TraceIL("IlBuilder[ %p ]::%d is ConstFloat %f\n", this, returnValue->getID(), value);
   return returnValue;
   }

TR::IlValue *
OMR::IlBuilder::ConstDouble(double value)
   {
   TR::Node *dconstNode = TR::Node::create(0, TR::dconst, 0);
   dconstNode->setDouble(value);
   TR::IlValue *returnValue = newValue(Double, dconstNode);
   TraceIL("IlBuilder[ %p ]::%d is ConstDouble %lf\n", this, returnValue->getID(), value);
   return returnValue;
   }

TR::IlValue *
OMR::IlBuilder::ConstString(const char * const value)
   {
   TR::IlValue *returnValue = newValue(Address, TR::Node::aconst((uintptrj_t)value));
   TraceIL("IlBuilder[ %p ]::%d is ConstString %p\n", this, returnValue->getID(), value);
   return returnValue;
   }

TR::IlValue *
OMR::IlBuilder::ConstAddress(const void * const value)
   {
   TR::IlValue *returnValue = newValue(Address, TR::Node::aconst((uintptrj_t)value));
   TraceIL("IlBuilder[ %p ]::%d is ConstAddress %p\n", this, returnValue->getID(), value);
   return returnValue;
   }

TR::IlValue *
OMR::IlBuilder::ConstInteger(TR::IlType *intType, int64_t value)
   {
   if      (intType == Int8)  return ConstInt8 ((int8_t)  value);
   else if (intType == Int16) return ConstInt16((int16_t) value);
   else if (intType == Int32) return ConstInt32((int32_t) value);
   else if (intType == Int64) return ConstInt64(          value);

   TR_ASSERT(0, "unknown integer type");
   return NULL;
   }

TR::IlValue *
OMR::IlBuilder::ConvertTo(TR::IlType *t, TR::IlValue *v)
   {
   TR::DataType typeFrom = v->getDataType();
   TR::DataType typeTo = t->getPrimitiveType();
   if (typeFrom == typeTo)
      {
      TraceIL("IlBuilder[ %p ]::%d is ConvertTo (already has type %s) %d\n", this, v->getID(), t->getName(), v->getID());
      return v;
      }
   TR::IlValue *convertedValue = convertTo(typeTo, v, false);
   TraceIL("IlBuilder[ %p ]::%d is ConvertTo(%s) %d\n", this, convertedValue->getID(), t->getName(), v->getID());
   return convertedValue;
   }

TR::IlValue *
OMR::IlBuilder::UnsignedConvertTo(TR::IlType *t, TR::IlValue *v)
   {
   TR::DataType typeFrom = v->getDataType();
   TR::DataType typeTo = t->getPrimitiveType();
   if (typeFrom == typeTo)
      {
      TraceIL("IlBuilder[ %p ]::%d is UnsignedConvertTo (already has type %s) %d\n", this, v->getID(), t->getName(), v->getID());
      return v;
      }
   TR::IlValue *convertedValue = convertTo(typeTo, v, true);
   TraceIL("IlBuilder[ %p ]::%d is UnsignedConvertTo(%s) %d\n", this, convertedValue->getID(), t->getName(), v->getID());
   return convertedValue;
   }

TR::IlValue *
OMR::IlBuilder::Negate(TR::IlValue *v)
   {
   TR::DataType dataType = v->getDataType();

   TR::ILOpCodes negateOp = ILOpCode::negateOpCode(dataType);
   TR_ASSERT(negateOp != TR::BadILOp, "Builder [ %p ] cannot negate value %d of type %s", this, v->getID(), dataType.toString());

   TR::Node *result = TR::Node::create(negateOp, 1, loadValue(v));
   TR::IlValue *negatedValue = newValue(dataType, result);
   TraceIL("IlBuilder[ %p ]::%d is Negated %d\n", this, negatedValue->getID(), v->getID());
   return negatedValue;
   }

TR::IlValue *
OMR::IlBuilder::convertTo(TR::DataType typeTo, TR::IlValue *v, bool needUnsigned)
   {
   TR::DataType typeFrom = v->getDataType();

   TR::ILOpCodes convertOp = ILOpCode::getProperConversion(typeFrom, typeTo, needUnsigned);
   TR_ASSERT(convertOp != TR::BadILOp, "Builder [ %p ] unknown conversion requested for value %d %s to %s", this, v->getID(), typeFrom.toString(), typeTo.toString());

   TR::Node *result = TR::Node::create(convertOp, 1, loadValue(v));
   TR::IlValue *convertedValue = newValue(typeTo, result);
   return convertedValue;
   }

TR::IlValue*
OMR::IlBuilder::ConvertBitsTo(TR::IlType* t, TR::IlValue* v)
   {
   TR::DataType typeFrom = v->getDataType();
   TR::DataType typeTo = t->getPrimitiveType();

   if (typeTo == typeFrom)
      {
      TraceIL("IlBuilder[ %p ]::%d is ConvertBitsTo (already has type %s) %d\n", this, v->getID(), t->getName(), v->getID());
      return v;
      }

   TR::ILOpCodes convertOpcode = TR::DataType::getDataTypeBitConversion(typeFrom, typeTo);
   TR_ASSERT(convertOpcode != TR::BadILOp && TR::DataType::getSize(typeTo) == TR::DataType::getSize(typeFrom),
             "Builder [ %p ] requested bit conversion for value %d from type of size %d (%s) to type of size %d (%s) (consider using ConvertTo() to for narrowing/widening)",
             this, v->getID(), TR::DataType::getSize(typeFrom), typeFrom.toString(), TR::DataType::getSize(typeTo), typeTo.toString());
   TR_ASSERT(convertOpcode != TR::BadILOp, "Builder [ %p ] unknown bit conversion requested for value %d (%s) to type %s", this, v->getID(), typeFrom.toString(), t->getName());

   TR::Node *result = TR::Node::create(convertOpcode, 1, loadValue(v));
   TR::IlValue *convertedValue = newValue(t, result);
   TraceIL("IlBuilder[ %p ]::%d is CoerceTo(%s) %d\n", this, convertedValue->getID(), t->getName(), v->getID());
   return convertedValue;
   }

TR::IlValue *
OMR::IlBuilder::unaryOp(TR::ILOpCodes op, TR::IlValue *v)
   {
   TR::Node *valueNode = loadValue(v);
   TR::Node *result = TR::Node::create(op, 1, valueNode);

   TR::IlValue *returnValue = newValue(v->getDataType(), result);
   return returnValue;
   }

void
OMR::IlBuilder::doVectorConversions(TR::Node **leftPtr, TR::Node **rightPtr)
   {
   TR::Node *    left  = *leftPtr;
   TR::DataType lType = left->getDataType();

   TR::Node *    right = *rightPtr;
   TR::DataType rType = right->getDataType();

   if (lType.isVector() && !rType.isVector())
      *rightPtr = TR::Node::create(TR::vsplats, 1, right);

   if (!lType.isVector() && rType.isVector())
      *leftPtr = TR::Node::create(TR::vsplats, 1, left);
   }

TR::IlValue *
OMR::IlBuilder::widenIntegerTo32Bits(TR::IlValue *v)
   {
   if (v->getDataType() == TR::Int8 || v->getDataType() == TR::Int16)
      return ConvertTo(Int32, v);

   return v;
   }

TR::IlValue *
OMR::IlBuilder::widenIntegerTo32BitsUnsigned(TR::IlValue *v)
   {
   if (v->getDataType() == TR::Int8 || v->getDataType() == TR::Int16)
      return UnsignedConvertTo(Int32, v);

   return v;
   }

TR::Node*
OMR::IlBuilder::binaryOpNodeFromNodes(TR::ILOpCodes op,
                                 TR::Node *leftNode,
                                 TR::Node *rightNode) 
   {
   TR::DataType leftType = leftNode->getDataType();
   TR::DataType rightType = rightNode->getDataType();
   bool isAddressBump = ((leftType == TR::Address) &&
                            (rightType == TR::Int32 || rightType == TR::Int64));
   bool isRevAddressBump = ((rightType == TR::Address) &&
                               (leftType == TR::Int32 || leftType == TR::Int64));
   TR_ASSERT(leftType == rightType || isAddressBump || isRevAddressBump, "binaryOp requires both left and right operands to have same type or one is address and other is Int32/64");

   if (isRevAddressBump) // swap them
      {
      TR::Node *save = leftNode;
      leftNode = rightNode;
      rightNode = save;
      }
   
   return TR::Node::create(op, 2, leftNode, rightNode);
   }

TR::IlValue *
OMR::IlBuilder::binaryOpFromNodes(TR::ILOpCodes op,
                             TR::Node *leftNode,
                             TR::Node *rightNode) 
   {
   TR::Node *result = binaryOpNodeFromNodes(op, leftNode, rightNode);
   TR::IlValue *returnValue = newValue(result->getDataType(), result);
   return returnValue;
   } 

TR::IlValue *
OMR::IlBuilder::binaryOpFromOpMap(OpCodeMapper mapOp,
                             TR::IlValue *left,
                             TR::IlValue *right)
   {
   TR::Node *leftNode = loadValue(left);
   TR::Node *rightNode = loadValue(right);

   doVectorConversions(&leftNode, &rightNode);

   TR::DataType leftType = leftNode->getDataType();
   return binaryOpFromNodes(mapOp(leftType), leftNode, rightNode);
   }

TR::IlValue *
OMR::IlBuilder::binaryOpFromOpCode(TR::ILOpCodes op,
                              TR::IlValue *left,
                              TR::IlValue *right)
   {
   TR::Node *leftNode = loadValue(left);
   TR::Node *rightNode = loadValue(right);
   //doVectorConversions(&left, &right);
   return binaryOpFromNodes(op, leftNode, rightNode);
   }

TR::IlValue *
OMR::IlBuilder::compareOp(TR_ComparisonTypes ct,
                     bool needUnsigned,
                     TR::IlValue *left,
                     TR::IlValue *right)
   {
   TR::Node *leftNode = loadValue(left);
   TR::Node *rightNode = loadValue(right);
   TR::ILOpCodes op = TR::ILOpCode::compareOpCode(leftNode->getDataType(), ct, needUnsigned);
   return binaryOpFromOpCode(op, left, right);
   }

TR::IlValue *
OMR::IlBuilder::NotEqualTo(TR::IlValue *left, TR::IlValue *right)
   {
   TR::IlValue *returnValue=compareOp(TR_cmpNE, false, left, right);
   TraceIL("IlBuilder[ %p ]::%d is NotEqualTo %d != %d?\n", this, returnValue->getID(), left->getID(), right->getID());
   return returnValue;
   }

void
OMR::IlBuilder::Goto(TR::IlBuilder **dest)
   {
   *dest = createBuilderIfNeeded(*dest);
   Goto(*dest);
   }

void
OMR::IlBuilder::Goto(TR::IlBuilder *dest)
   {
   TR_ASSERT(dest != NULL, "This goto implementation requires a non-NULL builder object");
   TraceIL("IlBuilder[ %p ]::Goto %p\n", this, dest);
   appendGoto(dest->getEntry());
   setDoesNotComeBack();
   }

void
OMR::IlBuilder::Return()
   {
   TR::IlBuilder *returnBuilder = _methodBuilder->returnBuilder();
   if (returnBuilder != NULL)
      {
      TR_ASSERT(_methodBuilder->returnSymbol() == NULL, "Return() from inlined call did not expect a pre-existing returnSymbol");
      TraceIL("IlBuilder[ %p ]::Return back to caller's returnBuilder [ %p ]\n", this, returnBuilder);

      // redirect flow back to the caller's return block
      Goto(returnBuilder);
      }
   else
      {
      TraceIL("IlBuilder[ %p ]::Return\n", this);
      TR::Node *returnNode = TR::Node::create(TR::ILOpCode::returnOpCode(TR::NoType));
      genTreeTop(returnNode);
      cfg()->addEdge(_currentBlock, cfg()->getEnd());
      setDoesNotComeBack();
      }
   }

void
OMR::IlBuilder::Return(TR::IlValue *value)
   { 
   TR::IlBuilder *returnBuilder = _methodBuilder->returnBuilder();
   if (returnBuilder != NULL)
      {
      char *returnSymbol = (char *)_methodBuilder->returnSymbol();
      if (returnSymbol == NULL)
         {
         TR::DataType dt = value->getDataType();
         TR::SymbolReference *newSymRef = symRefTab()->createTemporary(_methodSymbol, dt);
         returnSymbol = (char *) _comp->trMemory()->allocateHeapMemory((3+10+1) * sizeof(char)); // 3 ("_RV") + max 10 digits + trailing zero
         sprintf(returnSymbol, "_RV%u", newSymRef->getCPIndex());
         newSymRef->getSymbol()->getAutoSymbol()->setName(returnSymbol);
         newSymRef->getSymbol()->setNotCollected();
         _methodBuilder->defineSymbol(returnSymbol, newSymRef);
         _methodBuilder->setReturnSymbol(returnSymbol);

         // also proactively define this symbol in the caller, so that it can access it after the inlined body!
         _methodBuilder->callerMethodBuilder()->defineSymbol(returnSymbol, newSymRef);
         }

      TraceIL("IlBuilder[ %p ]::Return %d back to caller's returnBuilder [ %p ] via return symbol called %s\n", this, value->getID(), returnBuilder, returnSymbol);
      Store(returnSymbol, value);
      Goto(returnBuilder);
      }
   else
      {
      TraceIL("IlBuilder[ %p ]::Return %d\n", this, value->getID());
      TR::Node *returnNode = TR::Node::create(TR::ILOpCode::returnOpCode(value->getDataType()), 1, loadValue(value));
      genTreeTop(returnNode);
      cfg()->addEdge(_currentBlock, cfg()->getEnd());
      setDoesNotComeBack();
      }
   }

TR::IlValue *
OMR::IlBuilder::Sub(TR::IlValue *left, TR::IlValue *right)
   {
   TR::IlValue *returnValue = NULL;
   if (left->getDataType() == TR::Address)
      {
      if (TR::Compiler->target.is64Bit() && right->getDataType() == TR::Int32)
         {
         right = unaryOp(TR::i2l, right);
         }
      else if (TR::Compiler->target.is32Bit() && right->getDataType() == TR::Int64)
         {
         right = unaryOp(TR::l2i, right);
         }
      right = Sub(TR::Compiler->target.is32Bit() ? ConstInt32(0) : ConstInt64(0), right);
      returnValue = binaryOpFromNodes(TR::Compiler->target.is32Bit() ? TR::aiadd : TR::aladd, loadValue(left), loadValue(right));
      }
   else
      {
      returnValue=binaryOpFromOpMap(TR::ILOpCode::subtractOpCode, left, right);
      }
   TraceIL("IlBuilder[ %p ]::%d is Sub %d - %d\n", this, returnValue->getID(), left->getID(), right->getID());
   return returnValue;
   }

static TR::ILOpCodes addOpCode(TR::DataType type)
   {
   return TR::ILOpCode::addOpCode(type, TR::Compiler->target.is64Bit());
   }

static TR::ILOpCodes unsignedAddOpCode(TR::DataType type)
   {
   return TR::ILOpCode::unsignedAddOpCode(type, TR::Compiler->target.is64Bit());
   }

TR::IlValue *
OMR::IlBuilder::Add(TR::IlValue *left, TR::IlValue *right)
   {
   TR::IlValue *returnValue = NULL;
   if (left->getDataType() == TR::Address)
      {
      if (TR::Compiler->target.is64Bit() && right->getDataType() == TR::Int32)
         {
         right = unaryOp(TR::i2l, right);
         }
      else if (TR::Compiler->target.is32Bit() && right->getDataType() == TR::Int64)
         {
         right = unaryOp(TR::l2i, right);
         }
      returnValue = binaryOpFromNodes(TR::Compiler->target.is32Bit() ? TR::aiadd : TR::aladd, loadValue(left), loadValue(right));
      }
   else
      {
      returnValue = binaryOpFromOpMap(addOpCode, left, right);
      }
   TraceIL("IlBuilder[ %p ]::%d is Add %d + %d\n", this, returnValue->getID(), left->getID(), right->getID());
   return returnValue;
   }

/*
 * blockThrowsException:
 * ....
 * goto blockAfterExceptionHandler 
 * Handler:
 * ....
 * goto blockAfterExceptionHandler 
 * blockAfterExceptionHandler:
 */
void
OMR::IlBuilder::appendExceptionHandler(TR::Block *blockThrowsException, TR::IlBuilder **handler, uint32_t catchType)
   {
   //split block after overflowCHK, and add an goto to blockAfterExceptionHandler
   appendBlock();
   TR::Block *blockWithGoto = _currentBlock;
   TR::Node *gotoNode = TR::Node::create(NULL, TR::Goto);
   genTreeTop(gotoNode);
   _currentBlock = NULL;
   _currentBlockNumber = -1;
 
   //append handler, add exception edge and merge edge
   *handler = createBuilderIfNeeded(*handler);
   TR_ASSERT(*handler != NULL, "exception handler cannot be NULL\n");
   (*handler)->_isHandler = true;
   cfg()->addExceptionEdge(blockThrowsException, (*handler)->getEntry());
   AppendBuilder(*handler);
   (*handler)->setHandlerInfo(catchType);

   TR::Block *blockAfterExceptionHandler = _currentBlock;
   TraceIL("blockAfterExceptionHandler block_%d, blockWithGoto block_%d \n", blockAfterExceptionHandler->getNumber(), blockWithGoto->getNumber());
   gotoNode->setBranchDestination(blockAfterExceptionHandler->getEntry());
   cfg()->addEdge(blockWithGoto, blockAfterExceptionHandler);
   }

void
OMR::IlBuilder::setHandlerInfo(uint32_t catchType)
   {
   TR::Block *catchBlock = getEntry();
   catchBlock->setIsCold();
   catchBlock->setHandlerInfoWithOutBCInfo(catchType, comp()->getInlineDepth(), -1, _methodSymbol->getResolvedMethod(), comp());
   }

TR::Node*
OMR::IlBuilder::genOverflowCHKTreeTop(TR::Node *operationNode, TR::ILOpCodes overflow)
   {
   TR::Node *overflowChkNode = TR::Node::createWithRoomForOneMore(overflow, 3, symRefTab()->findOrCreateOverflowCheckSymbolRef(_methodSymbol), operationNode, operationNode->getFirstChild(), operationNode->getSecondChild());
   overflowChkNode->setOverflowCheckOperation(operationNode->getOpCodeValue());
   genTreeTop(overflowChkNode);
   return overflowChkNode;
   }

TR::IlValue *
OMR::IlBuilder::genOperationWithOverflowCHK(TR::ILOpCodes op, TR::Node *leftNode, TR::Node *rightNode, TR::IlBuilder **handler, TR::ILOpCodes overflow)
   {
   /*
    * BB1:
    *    overflowCHK
    *       operation(add/sub/mul)
    *          child1
    *          child2
    *       =>child1
    *       =>child2
    *    store
    *       => operation
    * BB2:
    *    goto BB3 
    * Handler:
    *    ...
    * BB3:
    *    continue
    */
   TR::Node *operationNode = binaryOpNodeFromNodes(op, leftNode, rightNode);
   TR::Node *overflowChkNode = genOverflowCHKTreeTop(operationNode, overflow);

   TR::Block *blockWithOverflowCHK = _currentBlock;
   TR::IlValue *resultValue = newValue(operationNode->getDataType(), operationNode);
   genTreeTop(TR::Node::createStore(resultValue->getSymbolReference(), operationNode));

   appendExceptionHandler(blockWithOverflowCHK, handler, TR::Block::CanCatchOverflowCheck);
   return resultValue;
   }

// This function takes 4 arguments and generate the addValue.
// This function is called by AddWithOverflow and AddWithUnsignedOverflow.
TR::ILOpCodes 
OMR::IlBuilder::getOpCode(TR::IlValue *leftValue, TR::IlValue *rightValue)
   {
   TR::ILOpCodes op;
   if (leftValue->getDataType() == TR::Address)
      {
      TR_ASSERT((TR::Compiler->target.is64Bit() && rightValue->getDataType() == TR::Int64) || (TR::Compiler->target.is32Bit() && rightValue->getDataType() == TR::Int32),
                "the right child type must be either TR::Int32 (on 32-bit ISA) or TR::Int64 (on 64-bit ISA) when the left child of Add is TR::Address\n");
      op = TR::Compiler->target.is32Bit() ? TR::aiadd : TR::aladd;
      }
   else 
      {
      op = addOpCode(leftValue->getDataType());
      }
   return op; 
   }

TR::IlValue *
OMR::IlBuilder::AddWithOverflow(TR::IlBuilder **handler, TR::IlValue *left, TR::IlValue *right)
   {
   TR::Node *leftNode = loadValue(left);
   TR::Node *rightNode = loadValue(right);
   TR::ILOpCodes opcode = getOpCode(left, right);
   TR::IlValue *addValue = genOperationWithOverflowCHK(opcode, leftNode, rightNode, handler, TR::OverflowCHK);
   TraceIL("IlBuilder[ %p ]::%d is AddWithOverflow %d + %d\n", this, addValue->getID(), left->getID(), right->getID());
   return addValue;
   }

TR::IlValue *
OMR::IlBuilder::AddWithUnsignedOverflow(TR::IlBuilder **handler, TR::IlValue *left, TR::IlValue *right)
   {
   TR::Node *leftNode = loadValue(left);
   TR::Node *rightNode = loadValue(right);
   TR::ILOpCodes opcode = getOpCode(left, right);
   TR::IlValue *addValue = genOperationWithOverflowCHK(opcode, leftNode, rightNode, handler, TR::UnsignedOverflowCHK);
   TraceIL("IlBuilder[ %p ]::%d is AddWithUnsignedOverflow %d + %d\n", this, addValue->getID(), left->getID(), right->getID());
   return addValue;
   }

TR::IlValue *
OMR::IlBuilder::SubWithOverflow(TR::IlBuilder **handler, TR::IlValue *left, TR::IlValue *right)
   {
   TR::Node *leftNode = loadValue(left);
   TR::Node *rightNode = loadValue(right);
   TR::IlValue *subValue = genOperationWithOverflowCHK(TR::ILOpCode::subtractOpCode(leftNode->getDataType()), leftNode, rightNode, handler, TR::OverflowCHK);
   TraceIL("IlBuilder[ %p ]::%d is SubWithOverflow %d + %d\n", this, subValue->getID(), left->getID(), right->getID());
   return subValue;
   }

TR::IlValue *
OMR::IlBuilder::SubWithUnsignedOverflow(TR::IlBuilder **handler, TR::IlValue *left, TR::IlValue *right)
   {
   TR::Node *leftNode = loadValue(left);
   TR::Node *rightNode = loadValue(right);
   TR::IlValue *unsignedSubValue = genOperationWithOverflowCHK(TR::ILOpCode::subtractOpCode(leftNode->getDataType()), leftNode, rightNode, handler, TR::UnsignedOverflowCHK);
   TraceIL("IlBuilder[ %p ]::%d is UnsignedSubWithOverflow %d + %d\n", this, unsignedSubValue->getID(), left->getID(), right->getID());
   return unsignedSubValue;
   }

TR::IlValue *
OMR::IlBuilder::MulWithOverflow(TR::IlBuilder **handler, TR::IlValue *left, TR::IlValue *right)
   {
   TR::Node *leftNode = loadValue(left);
   TR::Node *rightNode = loadValue(right);
   TR::IlValue *mulValue = genOperationWithOverflowCHK(TR::ILOpCode::multiplyOpCode(leftNode->getDataType()), leftNode, rightNode, handler, TR::OverflowCHK);
   TraceIL("IlBuilder[ %p ]::%d is MulWithOverflow %d + %d\n", this, mulValue->getID(), left->getID(), right->getID());
   return mulValue;
   }

TR::IlValue *
OMR::IlBuilder::Mul(TR::IlValue *left, TR::IlValue *right)
   {
   TR::IlValue *returnValue=binaryOpFromOpMap(TR::ILOpCode::multiplyOpCode, left, right);
   TraceIL("IlBuilder[ %p ]::%d is Mul %d * %d\n", this, returnValue->getID(), left->getID(), right->getID());
   return returnValue;
   }

TR::IlValue *
OMR::IlBuilder::Div(TR::IlValue *left, TR::IlValue *right)
   {
   TR::IlValue *returnValue=binaryOpFromOpMap(TR::ILOpCode::divideOpCode, left, right);
   TraceIL("IlBuilder[ %p ]::%d is Div %d / %d\n", this, returnValue->getID(), left->getID(), right->getID());
   return returnValue;
   }

TR::IlValue *
OMR::IlBuilder::UnsignedDiv(TR::IlValue *left, TR::IlValue *right)
   {
   TR::DataType returnType = left->getDataType();

   // There are no opcodes for performing unsigned division on 8-bit or 16-bit
   // integers. Instead, widen operands to 32-bit integers, use iudiv, then
   // truncate the result.
   left = widenIntegerTo32BitsUnsigned(left);
   right = widenIntegerTo32BitsUnsigned(right);

   TR::IlValue *returnValue = binaryOpFromOpMap(TR::ILOpCode::unsignedDivideOpCode, left, right);
   TraceIL("IlBuilder[ %p ]::%d is UnsignedDiv %d / %d\n", this, returnValue->getID(), left->getID(), right->getID());

   if (returnValue->getDataType() != returnType)
      returnValue = UnsignedConvertTo(_types->PrimitiveType(returnType), returnValue);

   return returnValue;
   }

TR::IlValue *
OMR::IlBuilder::Rem(TR::IlValue *left, TR::IlValue *right)
   {
   TR::DataType returnType = left->getDataType();

   // No code generators currently support the brem or srem opcodes. If we
   // encounter types that would emit those opcodes, simulate correct behaviour
   // by widening them to 32-bit integers, using irem to calculate the
   // remainder, then truncating the result.
   left = widenIntegerTo32Bits(left);
   right = widenIntegerTo32Bits(right);

   TR::IlValue *returnValue = binaryOpFromOpMap(TR::ILOpCode::remainderOpCode, left, right);
   TraceIL("IlBuilder[ %p ]::%d is Rem %d %% %d\n", this, returnValue->getID(), left->getID(), right->getID());

   if (returnValue->getDataType() != returnType)
      returnValue = ConvertTo(_types->PrimitiveType(returnType), returnValue);

   return returnValue;
   }

TR::IlValue *
OMR::IlBuilder::UnsignedRem(TR::IlValue *left, TR::IlValue *right)
   {
   TR::DataType returnType = left->getDataType();
   TR::IlValue *returnValue;

   if (returnType.isInt64())
      {
      // There is no opcode for performing unsigned remainder on 64-bit
      // integers, so we must instead simulate the correct behaviour. First, we
      // truncate the dividend modulo the divisor by performing unsigned
      // division followed by multiplication...
      TR::IlValue *truncLeft = binaryOpFromOpCode(TR::ILOpCodes::lmul, binaryOpFromOpCode(TR::ILOpCodes::ludiv, left, right), right);

      // ...then we subtract that value from the original dividend to get the
      // remainder.
      returnValue = binaryOpFromOpCode(TR::ILOpCodes::lsub, left, truncLeft);
      }
   else
      {
      // There are no opcodes for performing unsigned remainder on 8-bit or
      // 16-bit integers. Instead, widen operands to 32-bit integers, use iurem,
      // then truncate the result.
      left = widenIntegerTo32BitsUnsigned(left);
      right = widenIntegerTo32BitsUnsigned(right);

      returnValue = binaryOpFromOpCode(TR::ILOpCodes::iurem, left, right);
      }

   TraceIL("IlBuilder[ %p ]::%d is UnsignedRem %d %% %d\n", this, returnValue->getID(), left->getID(), right->getID());

   if (returnValue->getDataType() != returnType)
      returnValue = UnsignedConvertTo(_types->PrimitiveType(returnType), returnValue);

   return returnValue;
   }

TR::IlValue *
OMR::IlBuilder::And(TR::IlValue *left, TR::IlValue *right)
   {
   TR::IlValue *returnValue=binaryOpFromOpMap(TR::ILOpCode::andOpCode, left, right);
   TraceIL("IlBuilder[ %p ]::%d is And %d & %d\n", this, returnValue->getID(), left->getID(), right->getID());
   return returnValue;
   }

TR::IlValue *
OMR::IlBuilder::Or(TR::IlValue *left, TR::IlValue *right)
   {
   TR::IlValue *returnValue=binaryOpFromOpMap(TR::ILOpCode::orOpCode, left, right);
   TraceIL("IlBuilder[ %p ]::%d is Or %d | %d\n", this, returnValue->getID(), left->getID(), right->getID());
   return returnValue;
   }

TR::IlValue *
OMR::IlBuilder::Xor(TR::IlValue *left, TR::IlValue *right)
   {
   TR::IlValue *returnValue=binaryOpFromOpMap(TR::ILOpCode::xorOpCode, left, right);
   TraceIL("IlBuilder[ %p ]::%d is Xor %d ^ %d\n", this, returnValue->getID(), left->getID(), right->getID());
   return returnValue;
   }

TR::Node*
OMR::IlBuilder::shiftOpNodeFromNodes(TR::ILOpCodes op,
                                TR::Node *leftNode,
                                TR::Node *rightNode) 
   {
   TR::DataType leftType = leftNode->getDataType();
   TR::DataType rightType = rightNode->getDataType();
   TR_ASSERT(leftType.isIntegral() && rightType.isInt32(), "shift operation first operand must be an integer, and shift amount must be 32-bit integer");

   return TR::Node::create(op, 2, leftNode, rightNode);
   }

TR::IlValue *
OMR::IlBuilder::shiftOpFromNodes(TR::ILOpCodes op,
                            TR::Node *leftNode,
                            TR::Node *rightNode) 
   {
   TR::Node *result = shiftOpNodeFromNodes(op, leftNode, rightNode);
   TR::IlValue *returnValue = newValue(result->getDataType(), result);
   return returnValue;
   } 

TR::IlValue *
OMR::IlBuilder::shiftOpFromOpMap(OpCodeMapper mapOp,
                            TR::IlValue *left,
                            TR::IlValue *right)
   {
   TR::Node *leftNode = loadValue(left);
   TR::DataType leftType = leftNode->getDataType();
   TR_ASSERT(leftType.isIntegral(), "left operand of shift must be integer type");

   TR::Node *rightNode = loadValue(right);
   if (!rightNode->getDataType().isInt32())
      right = ConvertTo(Int32, right);

   doVectorConversions(&leftNode, &rightNode);

   return shiftOpFromNodes(mapOp(leftType), leftNode, rightNode);
   }

TR::IlValue *
OMR::IlBuilder::ShiftL(TR::IlValue *v, TR::IlValue *amount)
   {
   TR::IlValue *returnValue=shiftOpFromOpMap(TR::ILOpCode::shiftLeftOpCode, v, amount);
   TraceIL("IlBuilder[ %p ]::%d is shr %d << %d\n", this, returnValue->getID(), v->getID(), amount->getID());
   return returnValue;
   }

TR::IlValue *
OMR::IlBuilder::ShiftR(TR::IlValue *v, TR::IlValue *amount)
   {
   TR::IlValue *returnValue=shiftOpFromOpMap(TR::ILOpCode::shiftRightOpCode, v, amount);
   TraceIL("IlBuilder[ %p ]::%d is shr %d >> %d\n", this, returnValue->getID(), v->getID(), amount->getID());
   return returnValue;
   }

TR::IlValue *
OMR::IlBuilder::UnsignedShiftR(TR::IlValue *v, TR::IlValue *amount)
   {
   TR::IlValue *returnValue=shiftOpFromOpMap(TR::ILOpCode::unsignedShiftRightOpCode, v, amount);
   TraceIL("IlBuilder[ %p ]::%d is unsigned shr %d >> %d\n", this, returnValue->getID(), v->getID(), amount->getID());
   return returnValue;
   }

/*
 * @brief IfAnd service for constructing short circuit AND conditional nests (like the && operator)
 * @param allTrueBuilder builder containing operations to execute if all conditional tests evaluate 
 *        to true (automatically allocated if pointed-to pointer is null)
 * @param anyFalseBuilder builder containing operations to execute if any conditional test is false
 *        (automatically allocated if pointed-to pointer is null)
 * @param numTerms the number of conditional terms
 * @param terms array of JBCondition instances that evaluate a condition
 *
 * Example (should be moved to client API):
 * TR::IlBuilder *cond1Builder = OrphanBuilder();
 * TR::IlValue *cond1 = cond1Builder->GreaterOrEqual(
 *                      cond1Builder->   Load("x"),
 *                      cond1Builder->   Load("lower"));
 * TR::IlBuilder *cond2Builder = OrphanBuilder();
 * TR::IlValue *cond2 = cond2Builder->LessThan(
 *                      cond2Builder->   Load("x"),
 *                      cond2Builder->   Load("upper"));
 * TR::IlBuilder *inRange = NULL, *outOfRange = NULL;
 * TR::IlBuilder *conditions[] = {MakeCondition(cond1Builder, cond1), MakeCondition(cond2Builder, cond2)};
 * IfAnd(&inRange, &outOfRange, 2, conditions);
 */
void
OMR::IlBuilder::IfAnd(TR::IlBuilder **allTrueBuilder, TR::IlBuilder **anyFalseBuilder, int32_t numTerms, JBCondition **terms)
   {
   TR::IlBuilder *mergePoint = OrphanBuilder();
   *allTrueBuilder = createBuilderIfNeeded(*allTrueBuilder);
   *anyFalseBuilder = createBuilderIfNeeded(*anyFalseBuilder);

   for (int32_t t=0;t < numTerms;t++)
      {
      TR::IlBuilder *condBuilder = terms[t]->_builder;
      TR::IlValue *condValue = terms[t]->_condition;
      AppendBuilder(condBuilder);
      condBuilder->IfCmpEqualZero(anyFalseBuilder, condValue);
      // otherwise fall through to test next term
      }

   // if control gets here, all the provided terms were true
   AppendBuilder(*allTrueBuilder);
   Goto(mergePoint);

   // also need to handle the false case
   AppendBuilder(*anyFalseBuilder);
   Goto(mergePoint);

   AppendBuilder(mergePoint);

   // return state for "this" can get confused by the Goto's in this service
   setComesBack();
   }

/**
 * @brief Overload taking a varargs instead of an array of JBCondition pointers
 *
 * @param allTrueBuilder builder containing operations to execute if all conditional tests 
 *        evaluate to true (automatically allocated if pointed-to pointer is null)
 * @param anyFalseBuilder builder containing operations to execute if any conditional test
 *        is false (automatically allocated if pointed-to pointer is null)
 * @param numTerms the number of conditional terms
 * @param ... for each term, provide a TR::IlBuilder::JBCondition object for the condition
 *
 * Example:
 * TR::IlBuilder *cond1Builder = OrphanBuilder();
 * TR::IlValue *cond1 = cond1Builder->GreaterOrEqual(
 *                      cond1Builder->   Load("x"),
 *                      cond1Builder->   Load("lower"));
 * TR::IlBuilder *cond2Builder = OrphanBuilder();
 * TR::IlValue *cond2 = cond2Builder->LessThan(
 *                      cond2Builder->   Load("x"),
 *                      cond2Builder->   Load("upper"));
 * TR::IlBuilder *inRange = NULL, *outOfRange = NULL;
 * IfAnd(&inRange, &outOfRange, 2, MakeCondition(cond1Builder, cond1), MakeCondition(cond2Builder, cond2));
 */
void
OMR::IlBuilder::IfAnd(TR::IlBuilder **allTrueBuilder, TR::IlBuilder **anyFalseBuilder, int32_t numTerms, ...)
   {
   JBCondition **terms = (JBCondition **) _comp->trMemory()->allocateHeapMemory(numTerms * sizeof(JBCondition *));
   TR_ASSERT(NULL != terms, "out of memory");

   va_list args;
   va_start(args, numTerms);
   for (int32_t c = 0; c < numTerms; ++c)
      {
      terms[c] = va_arg(args, JBCondition *);
      }
   va_end(args);

   IfAnd(allTrueBuilder, anyFalseBuilder, numTerms, terms);
   }

/*
 * @brief IfOr service for constructing short circuit OR conditional nests (like the || operator)
 * @param anyTrueBuilder builder containing operations to execute if any conditional test evaluates
 *        to true (automatically allocated if pointed-to pointer is null)
 * @param allFalseBuilder builder containing operations to execute if all conditional tests are
 *        false (automatically allocated if pointed-to pointer is null)
 * @param numTerms the number of conditional terms
 * @param terms array of JBCondition instances that evaluate a condition
 *
 * Example (should be moved to client API):
 * TR::IlBuilder *cond1Builder = OrphanBuilder();
 * TR::IlValue *cond1 = cond1Builder->LessThan(
 *                      cond1Builder->   Load("x"),
 *                      cond1Builder->   Load("lower"));
 * TR::IlBuilder *cond2Builder = OrphanBuilder();
 * TR::IlValue *cond2 = cond2Builder->GreaterOrEqual(
 *                      cond2Builder->   Load("x"),
 *                      cond2Builder->   Load("upper"));
 * TR::IlBuilder *inRange = NULL, *outOfRange = NULL;
 * TR::IlBuilder *conditions[] = {MakeCondition(cond1Builder, cond1), MakeCondition(cond2Builder, cond2)};
 * IfOr(&outOfRange, &inRange, 2, conditions);
 */
void
OMR::IlBuilder::IfOr(TR::IlBuilder **anyTrueBuilder, TR::IlBuilder **allFalseBuilder, int32_t numTerms, JBCondition **terms)
   {
   TR::IlBuilder *mergePoint = OrphanBuilder();
   *anyTrueBuilder = createBuilderIfNeeded(*anyTrueBuilder);
   *allFalseBuilder = createBuilderIfNeeded(*allFalseBuilder);

   for (int32_t t=0;t < numTerms-1;t++)
      {
      TR::IlBuilder *condBuilder = terms[t]->_builder;
      TR::IlValue *condValue = terms[t]->_condition;
      AppendBuilder(condBuilder);
      condBuilder->IfCmpNotEqualZero(anyTrueBuilder, condValue);
      // otherwise fall through to test next term
      }

   // reverse condition on last term so that it can fall through to anyTrueBuilder
   TR::IlBuilder *condBuilder = terms[numTerms - 1]->_builder;
   TR::IlValue *condValue = terms[numTerms - 1]->_condition;
   AppendBuilder(condBuilder);
   condBuilder->IfCmpEqualZero(allFalseBuilder, condValue);

   // any true term will end up here
   AppendBuilder(*anyTrueBuilder);
   Goto(mergePoint);

   // if control gets here, all the provided terms were false
   AppendBuilder(*allFalseBuilder);
   Goto(mergePoint);

   AppendBuilder(mergePoint);

   // return state for "this" can get confused by the Goto's in this service
   setComesBack();
   }

/**
 * @brief Overload taking a varargs instead of an array of JBCondition pointers
 *
 * @param anyTrueBuilder builder containing operations to execute if any conditional test 
 *        evaluates to true (automatically allocated if pointed-to pointer is null)
 * @param allFalseBuilder builder containing operations to execute if all conditional
 *        tests are false (automatically allocated if pointed-to pointer is null)
 * @param numTerms the number of conditional terms
 * @param ... for each term, provide a TR::IlBuilder::JBCondition object for the condition
 *
 * Example:
 * TR::IlBuilder *cond1Builder = OrphanBuilder();
 * TR::IlValue *cond1 = cond1Builder->LessThan(
 *                      cond1Builder->   Load("x"),
 *                      cond1Builder->   Load("lower"));
 * TR::IlBuilder *cond2Builder = OrphanBuilder();
 * TR::IlValue *cond2 = cond2Builder->GreaterOrEqual(
 *                      cond2Builder->   Load("x"),
 *                      cond2Builder->   Load("upper"));
 * TR::IlBuilder *inRange = NULL, *outOfRange = NULL;
 * IfOr(&outOfRange, &inRange, 2, MakeCondition(cond1Builder, cond1), MakeCondition(cond2Builder, cond2));
 */
void
OMR::IlBuilder::IfOr(TR::IlBuilder **anyTrueBuilder, TR::IlBuilder **allFalseBuilder, int32_t numTerms, ...)
   {
   JBCondition **terms = (JBCondition **) _comp->trMemory()->allocateHeapMemory(numTerms * sizeof(JBCondition *));
   TR_ASSERT(NULL != terms, "out of memory");

   va_list args;
   va_start(args, numTerms);
   for (int32_t c = 0; c < numTerms; ++c)
      {
      terms[c] = va_arg(args, JBCondition *);
      }
   va_end(args);

   IfOr(anyTrueBuilder, allFalseBuilder, numTerms, terms);
   }

TR::IlBuilder::JBCondition *
OMR::IlBuilder::MakeCondition(TR::IlBuilder *conditionBuilder, TR::IlValue *conditionValue)
   {
   TR_ASSERT(conditionBuilder != NULL, "MakeCondition needs to have non-null conditionBuilder");
   TR_ASSERT(conditionValue != NULL, "MakeCondition needs to have non-null conditionValue");
   return new (_comp->trHeapMemory()) JBCondition(conditionBuilder, conditionValue);
   }

TR::IlValue *
OMR::IlBuilder::EqualTo(TR::IlValue *left, TR::IlValue *right)
   {
   TR::IlValue *returnValue=compareOp(TR_cmpEQ, false, left, right);
   TraceIL("IlBuilder[ %p ]::%d is EqualTo %d == %d?\n", this, returnValue->getID(), left->getID(), right->getID());
   return returnValue;
   }

void
OMR::IlBuilder::integerizeAddresses(TR::IlValue **leftPtr, TR::IlValue **rightPtr)
   {
   TR::IlValue *left = *leftPtr;
   if (left->getDataType() == TR::Address)
      *leftPtr = ConvertTo(Word, left);

   TR::IlValue *right = *rightPtr;
   if (right->getDataType() == TR::Address)
      *rightPtr = ConvertTo(Word, right);
   }

TR::IlValue *
OMR::IlBuilder::LessThan(TR::IlValue *left, TR::IlValue *right)
   {
   integerizeAddresses(&left, &right);
   TR::IlValue *returnValue=compareOp(TR_cmpLT, false, left, right);
   TraceIL("IlBuilder[ %p ]::%d is LessThan %d < %d?\n", this, returnValue->getID(), left->getID(), right->getID());
   return returnValue;
   }

TR::IlValue *
OMR::IlBuilder::UnsignedLessThan(TR::IlValue *left, TR::IlValue *right)
   {
   integerizeAddresses(&left, &right);
   TR::IlValue *returnValue=compareOp(TR_cmpLT, true, left, right);
   TraceIL("IlBuilder[ %p ]::%d is UnsignedLessThan %d < %d?\n", this, returnValue->getID(), left->getID(), right->getID());
   return returnValue;
   }

TR::IlValue *
OMR::IlBuilder::LessOrEqualTo(TR::IlValue *left, TR::IlValue *right)
   {
   integerizeAddresses(&left, &right);
   TR::IlValue *returnValue=compareOp(TR_cmpLE, false, left, right);
   TraceIL("IlBuilder[ %p ]::%d is LessOrEqualTo %d <= %d?\n", this, returnValue->getID(), left->getID(), right->getID());
   return returnValue;
   }

TR::IlValue *
OMR::IlBuilder::UnsignedLessOrEqualTo(TR::IlValue *left, TR::IlValue *right)
   {
   integerizeAddresses(&left, &right);
   TR::IlValue *returnValue=compareOp(TR_cmpLE, true, left, right);
   TraceIL("IlBuilder[ %p ]::%d is UnsignedLessOrEqualTo %d <= %d?\n", this, returnValue->getID(), left->getID(), right->getID());
   return returnValue;
   }

TR::IlValue *
OMR::IlBuilder::GreaterThan(TR::IlValue *left, TR::IlValue *right)
   {
   integerizeAddresses(&left, &right);
   TR::IlValue *returnValue=compareOp(TR_cmpGT, false, left, right);
   TraceIL("IlBuilder[ %p ]::%d is GreaterThan %d > %d?\n", this, returnValue->getID(), left->getID(), right->getID());
   return returnValue;
   }

TR::IlValue *
OMR::IlBuilder::UnsignedGreaterThan(TR::IlValue *left, TR::IlValue *right)
   {
   integerizeAddresses(&left, &right);
   TR::IlValue *returnValue=compareOp(TR_cmpGT, true, left, right);
   TraceIL("IlBuilder[ %p ]::%d is UnsignedGreaterThan %d > %d?\n", this, returnValue->getID(), left->getID(), right->getID());
   return returnValue;
   }

TR::IlValue *
OMR::IlBuilder::GreaterOrEqualTo(TR::IlValue *left, TR::IlValue *right)
   {
   integerizeAddresses(&left, &right);
   TR::IlValue *returnValue=compareOp(TR_cmpGE, false, left, right);
   TraceIL("IlBuilder[ %p ]::%d is GreaterOrEqualTo %d >= %d?\n", this, returnValue->getID(), left->getID(), right->getID());
   return returnValue;
   }

TR::IlValue *
OMR::IlBuilder::UnsignedGreaterOrEqualTo(TR::IlValue *left, TR::IlValue *right)
   {
   integerizeAddresses(&left, &right);
   TR::IlValue *returnValue=compareOp(TR_cmpGE, true, left, right);
   TraceIL("IlBuilder[ %p ]::%d is UnsignedGreaterOrEqualTo %d >= %d?\n", this, returnValue->getID(), left->getID(), right->getID());
   return returnValue;
   }

TR::IlValue** 
OMR::IlBuilder::processCallArgs(TR::Compilation *comp, int numArgs, va_list args)
   {
   TR::IlValue ** argValues = (TR::IlValue **) comp->trMemory()->allocateHeapMemory(numArgs * sizeof(TR::IlValue *));
   for (int32_t a=0;a < numArgs;a++)
      {
      argValues[a] = va_arg(args, TR::IlValue*);
      }
   return argValues;
   }

/*
 * \param numArgs 
 *    Number of actual arguments for the method  plus 1 
 * \param ... 
 *    The list is a computed address followed by the actual arguments
 */
TR::IlValue *
OMR::IlBuilder::ComputedCall(const char *functionName, int32_t numArgs, ...)
   {
   TraceIL("IlBuilder[ %p ]::ComputedCall %s\n", this, functionName);
   va_list args;
   va_start(args, numArgs);
   TR::IlValue **argValues = processCallArgs(_comp, numArgs, args);
   va_end(args);
   TR::ResolvedMethod *resolvedMethod = _methodBuilder->lookupFunction(functionName);
   if (resolvedMethod == NULL && _methodBuilder->RequestFunction(functionName))
      resolvedMethod = _methodBuilder->lookupFunction(functionName);
   TR_ASSERT(resolvedMethod, "Could not identify function %s\n", functionName);

   TR::SymbolReference *methodSymRef = symRefTab()->findOrCreateComputedStaticMethodSymbol(JITTED_METHOD_INDEX, -1, resolvedMethod);
   return genCall(methodSymRef, numArgs, argValues, false /*isDirectCall*/);
   }

/*
 * \param numArgs 
 *    Number of actual arguments for the method  plus 1 
 * \param argValues
 *    the computed address followed by the actual arguments
 */
TR::IlValue *
OMR::IlBuilder::ComputedCall(const char *functionName, int32_t numArgs, TR::IlValue **argValues)
   {
   TraceIL("IlBuilder[ %p ]::ComputedCall %s\n", this, functionName);
   TR::ResolvedMethod *resolvedMethod = _methodBuilder->lookupFunction(functionName);
   if (resolvedMethod == NULL && _methodBuilder->RequestFunction(functionName))
      resolvedMethod = _methodBuilder->lookupFunction(functionName);
   TR_ASSERT(resolvedMethod, "Could not identify function %s\n", functionName);

   TR::SymbolReference *methodSymRef = symRefTab()->findOrCreateComputedStaticMethodSymbol(JITTED_METHOD_INDEX, -1, resolvedMethod);
   return genCall(methodSymRef, numArgs, argValues, false /*isDirectCall*/);
   }

/*
 * This service takes a MethodBuilder object as the target and will, for
 * now, inline the code for that MethodBuilder into the current builder
 * object. Really, this API does not promise to inline the provided
 * MethodBuilder object, but this current implementation will inline.
 * In future, as inlining support moves deeper into the OMR compiler,
 * this service may or may not inline the provided MethodBuilder.
 * This particular implementation does not handle VirtualMachineState
 * propagation well, but does work for simpler examples (code sample
 * coming in a subsequent commit).
 */
TR::IlValue *
OMR::IlBuilder::Call(TR::MethodBuilder *calleeMB, int32_t numArgs, ...)
   {
   va_list args;
   va_start(args, numArgs);
   TR::IlValue **argValues = processCallArgs(_comp, numArgs, args);
   va_end(args);

   return Call(calleeMB, numArgs, argValues);
   }

/*
 * This service takes a MethodBuilder object as the target and will, for
 * now, inline the code for that MethodBuilder into the current builder
 * object. Really, this API does not promise to inline the provided
 * MethodBuilder object, but this current implementation will inline.
 * In future, as inlining support moves deeper into the OMR compiler,
 * this service may or may not inline the provided MethodBuilder.
 * This particular implementation does not handle VirtualMachineState
 * propagation well, but does work for simpler examples (code sample
 * coming in a subsequent commit).
 */
TR::IlValue *
OMR::IlBuilder::Call(TR::MethodBuilder *calleeMB, int32_t numArgs, TR::IlValue **argValues)
   {
   TraceIL("IlBuilder[ %p ]::Call %s\n", this, calleeMB->GetMethodName());

   // set up callee's inline site index
   calleeMB->setInlineSiteIndex(_methodBuilder->getNextInlineSiteIndex());

   // set up callee's return builder for return control flows
   TR::IlBuilder *returnBuilder = OrphanBuilder();
   calleeMB->setReturnBuilder(returnBuilder);

   // get calleeMB ready to be part of this compilation
   // MUST be the OMR::IlBuilder implementation, not the OMR::MethodBuilder one
   calleeMB->OMR::IlBuilder::setupForBuildIL();

   // store arguments into parameter values
   for (int32_t a=0;a < numArgs;a++)
      {
      calleeMB->Store(calleeMB->getSymbolName(a), argValues[a]);
      }

   // propagate vm state into the callee
   if (vmState() != NULL)
      calleeMB->setVMState(vmState());

   // now flow control into the callee
   AppendBuilder(calleeMB);

   bool rc = calleeMB->buildIL();
   TraceIL("callee's buildIL() returned %d\n", rc);
   if (!rc)
      return NULL;

   // there shouldn't be any fall-through, but if there is it should go to the return block and we need to put it somewhere anyway
   AppendBuilder(returnBuilder);

   setVMState(returnBuilder->vmState());

   // if no return value, then we're done
   const char *returnSymbol = calleeMB->returnSymbol();
   if (!returnSymbol)
      return NULL;

   // otherwise, return callee's return value
   TR::IlValue *returnValue = returnBuilder->Load(returnSymbol);
   TraceIL("IlBuilder[ %p ]::Call callee return value is %d loaded from %s\n", this, returnValue->getID(), returnSymbol);
   return returnValue;
   }

TR::IlValue *
OMR::IlBuilder::Call(const char *functionName, int32_t numArgs, ...)
   {
   TraceIL("IlBuilder[ %p ]::Call %s\n", this, functionName);
   va_list args;
   va_start(args, numArgs);
   TR::IlValue **argValues = processCallArgs(_comp, numArgs, args);
   va_end(args);
   TR::ResolvedMethod *resolvedMethod = _methodBuilder->lookupFunction(functionName);
   if (resolvedMethod == NULL && _methodBuilder->RequestFunction(functionName))
      resolvedMethod = _methodBuilder->lookupFunction(functionName);
   TR_ASSERT(resolvedMethod, "Could not identify function %s\n", functionName);

   TR::SymbolReference *methodSymRef = symRefTab()->findOrCreateStaticMethodSymbol(JITTED_METHOD_INDEX, -1, resolvedMethod);
   return genCall(methodSymRef, numArgs, argValues);
   }

TR::IlValue *
OMR::IlBuilder::Call(const char *functionName, int32_t numArgs, TR::IlValue ** argValues)
   {
   TraceIL("IlBuilder[ %p ]::Call %s\n", this, functionName);
   TR::ResolvedMethod *resolvedMethod = _methodBuilder->lookupFunction(functionName);
   if (resolvedMethod == NULL && _methodBuilder->RequestFunction(functionName))
      resolvedMethod = _methodBuilder->lookupFunction(functionName);
   TR_ASSERT(resolvedMethod, "Could not identify function %s\n", functionName);

   TR::SymbolReference *methodSymRef = symRefTab()->findOrCreateStaticMethodSymbol(JITTED_METHOD_INDEX, -1, resolvedMethod);
   return genCall(methodSymRef, numArgs, argValues);
   }

TR::IlValue *
OMR::IlBuilder::genCall(TR::SymbolReference *methodSymRef, int32_t numArgs, TR::IlValue ** argValues, bool isDirectCall /* true by default*/)
   {
   TR::DataType returnType = methodSymRef->getSymbol()->castToMethodSymbol()->getMethod()->returnType();
   TR::Node *callNode = TR::Node::createWithSymRef(isDirectCall? TR::ILOpCode::getDirectCall(returnType): TR::ILOpCode::getIndirectCall(returnType), numArgs, methodSymRef);

   // TODO: should really verify argument types here
   int32_t childIndex = 0;
   for (int32_t a=0;a < numArgs;a++)
      {
      TR::IlValue *arg = argValues[a];
      if (arg->getDataType() == TR::Int8 || arg->getDataType() == TR::Int16 || (Word == Int64 && arg->getDataType() == TR::Int32))
         arg = ConvertTo(Word, arg);
      callNode->setAndIncChild(childIndex++, loadValue(arg));
      }

   // callNode must be anchored by itself
   genTreeTop(callNode);

   if (returnType != TR::NoType)
      {
      TR::IlValue *returnValue = newValue(callNode->getDataType(), callNode);
      if (returnType != callNode->getDataType())
         returnValue = convertTo(returnType, returnValue, false);

      return returnValue;
      }

   return NULL;
   }


/** \brief
 *     The service is used to atomically increase memory location \p baseAddress by amount of \p value. 
 *
 *  \param value
 *     The amount to increase for the memory location.
 *
 *  \note
 *     This service currently only supports Int32/Int64. 
 *
 *  \return
 *     The old value at the location \p baseAddress.
 */
TR::IlValue *
OMR::IlBuilder::AtomicAdd(TR::IlValue * baseAddress, TR::IlValue * value)
   {
   TR_ASSERT(baseAddress->getDataType() == TR::Address, "baseAddress must be TR::Address");

   //Determine the implementation type and returnType by detecting "value"'s type
   TR::DataType returnType = value->getDataType();
   TR_ASSERT(returnType == TR::Int32 || (returnType == TR::Int64 && TR::Compiler->target.is64Bit()), "AtomicAdd currently only supports Int32/64 values");
   TraceIL("IlBuilder[ %p ]::AtomicAdd(%d, %d)\n", this, baseAddress->getID(), value->getID());

   TR::SymbolReference *methodSymRef = symRefTab()->findOrCreateCodeGenInlinedHelper(TR::SymbolReferenceTable::atomicAddSymbol);
   TR::Node *callNode;
   callNode = TR::Node::createWithSymRef(TR::ILOpCode::getDirectCall(returnType), 2, methodSymRef);
   callNode->setAndIncChild(0, loadValue(baseAddress));
   callNode->setAndIncChild(1, loadValue(value));

   TR::IlValue *returnValue = newValue(callNode->getDataType(), callNode);
   return returnValue; 
   }


/**
 * \brief
 *  The service is for generating a treetop for transaction begin when the user needs to use transactional memory.
 *  At the end of transaction path, service will automatically generate transaction end instruction.
 *
 * \verbatim
 *   
 *    +---------------------------------+
 *    |<block_$tstart>                  |
 *    |==============                   |
 *    | TSTART                          |
 *    |    |                            |
 *    |    |_ <block_$persistentFailure>|
 *    |    |_ <block_$transientFailure> |
 *    |    |_ <block_$transaction>      |+------------------------+----------------------------------+
 *    |                                 |                         |                                  |
 *    +---------------------------------+                         |                                  |
 *                       |                                        |                                  |
 *                       v                                        V                                  V
 *    +-----------------------------------------+   +-------------------------------+    +-------------------------+     
 *    |<block_$transaction>                     |   |<block_$persistentFailure>     |    |<block_$transientFailure>|      
 *    |====================                     |   |===========================    |    |=========================|
 *    |     add (For illustration, we take add  |   |AtomicAdd (For illustration, we|    |   ...                   |
 *    |     ... as an example. User could       |   |...       take atomicAdd as an |    |   ...                   |
 *    |     ... replace it with other non-atomic|   |...       example. User could  |    |   ...                   |
 *    |     ... operations.)                    |   |...       replace it with other|    |   ...                   |
 *    |     ...                                 |   |...       atomic operations.)  |    |   ...                   |
 *    |     TEND                                |   |...                            |    |   ...                   |
 *    |goto --> block_$merge                    |   |goto->block_$merge             |    |goto->block_$merge       |
 *    +----------------------------------------+    +-------------------------------+    +-------------------------+
 *                      |                                          |                                 |
 *                      |                                          |                                 |
 *                      |------------------------------------------+---------------------------------+
 *                      |
 *                      v             
 *              +----------------+     
 *              | block_$merge   |     
 *              | ============== |   
 *              |      ...       |
 *              +----------------+   
 *
 * \endverbatim 
 *
 * \structure & \param value
 *     tstart
 *       |
 *       ----persistentFailure // This is a permanent failure, try atomic way to do it instead
 *       |
 *       ----transientFailure // Temporary failure, user can retry 
 *       |
 *       ----transaction // Success, use general(non-atomic) way to do it
 *                |
 *                ---- non-atomic operations
 *                |
 *                ---- ...
 *                |
 *                ---- tend
 *
 * \note
 *      If user's platform doesn't support TM, go to persistentFailure path directly/
 *      In this case, current IlBuilder walks around transientFailureBuilder and transactionBuilder
 *      and goes to persistentFailureBuilder.
 *      
 *      _currentBuilder
 *          |
 *          ->Goto()
 *              |   transientFailureBuilder 
 *              |   transactionBuilder
 *              |-->persistentFailurie
 */
void
OMR::IlBuilder::Transaction(TR::IlBuilder **persistentFailureBuilder, TR::IlBuilder **transientFailureBuilder, TR::IlBuilder **transactionBuilder)
   {   
   //This assertion is to rule out platforms which don't have tstart evaluator yet. 
   TR_ASSERT(comp()->cg()->hasTMEvaluator(), "this platform doesn't support tstart or tfinish evaluator yet");   
    
   TraceIL("IlBuilder[ %p ]::transactionBegin %p, %p, %p, %p)\n", this, *persistentFailureBuilder, *transientFailureBuilder, *transactionBuilder);

   appendBlock();

   TR::Block *mergeBlock = emptyBlock();
   *persistentFailureBuilder = createBuilderIfNeeded(*persistentFailureBuilder);
   *transientFailureBuilder = createBuilderIfNeeded(*transientFailureBuilder);
   *transactionBuilder = createBuilderIfNeeded(*transactionBuilder);

   if (!comp()->cg()->getSupportsTM())
      {
      //if user's processor doesn't support TM.
      //we will walk around transaction and transientFailure paths

      Goto(persistentFailureBuilder);

      AppendBuilder(*transactionBuilder);
      AppendBuilder(*transientFailureBuilder);

      AppendBuilder(*persistentFailureBuilder);
      appendBlock(mergeBlock);
      return;
      }

   TR::Node *persistentFailureNode = TR::Node::create(TR::branch, 0, (*persistentFailureBuilder)->getEntry()->getEntry());
   TR::Node *transientFailureNode = TR::Node::create(TR::branch, 0, (*transientFailureBuilder)->getEntry()->getEntry());
   TR::Node *transactionNode = TR::Node::create(TR::branch, 0, (*transactionBuilder)->getEntry()->getEntry());

   TR::Node *tStartNode = TR::Node::create(TR::tstart, 3, persistentFailureNode, transientFailureNode, transactionNode);   
   tStartNode->setSymbolReference(comp()->getSymRefTab()->findOrCreateTransactionEntrySymbolRef(comp()->getMethodSymbol()));
   
   genTreeTop(tStartNode);

   //connecting the block having tstart with persistentFailure's and transaction's blocks 
   cfg()->addEdge(_currentBlock, (*persistentFailureBuilder)->getEntry());
   cfg()->addEdge(_currentBlock, (*transientFailureBuilder)->getEntry());
   cfg()->addEdge(_currentBlock, (*transactionBuilder)->getEntry());

   appendNoFallThroughBlock();
   AppendBuilder(*transientFailureBuilder);
   gotoBlock(mergeBlock);

   AppendBuilder(*persistentFailureBuilder);
   gotoBlock(mergeBlock);

   AppendBuilder(*transactionBuilder);
    
   //ending the transaction at the end of transactionBuilder
   appendBlock();
   TR::Node *tEndNode=TR::Node::create(TR::tfinish,0);
   tEndNode->setSymbolReference(comp()->getSymRefTab()->findOrCreateTransactionExitSymbolRef(comp()->getMethodSymbol()));
   genTreeTop(tEndNode);

   //Three IlBuilders above merged here
   appendBlock(mergeBlock);
   }  


/**
 * Generate XABORT instruction to abort transaction
 */
void
OMR::IlBuilder::TransactionAbort()
   {
   TraceIL("IlBuilder[ %p ]::transactionAbort", this);
   TR::Node *tAbortNode = TR::Node::create(TR::tabort, 0);
   tAbortNode->setSymbolReference(comp()->getSymRefTab()->findOrCreateTransactionAbortSymbolRef(comp()->getMethodSymbol()));
   genTreeTop(tAbortNode);
   }

void
OMR::IlBuilder::IfCmpNotEqualZero(TR::IlBuilder **target, TR::IlValue *condition)
   {
   *target = createBuilderIfNeeded(*target);
   IfCmpNotEqualZero(*target, condition);
   }

void
OMR::IlBuilder::IfCmpNotEqualZero(TR::IlBuilder *target, TR::IlValue *condition)
   {
   TR_ASSERT(target != NULL, "This IfCmpNotEqualZero requires a non-NULL builder object");
   TraceIL("IlBuilder[ %p ]::IfCmpNotEqualZero %d? -> [ %p ] B%d\n", this, condition->getID(), target, target->getEntry()->getNumber());
   ifCmpNotEqualZero(condition, target->getEntry());
   }

void
OMR::IlBuilder::IfCmpNotEqual(TR::IlBuilder **target, TR::IlValue *left, TR::IlValue *right)
   {
   *target = createBuilderIfNeeded(*target);
   IfCmpNotEqual(*target, left, right);
   }

void
OMR::IlBuilder::IfCmpNotEqual(TR::IlBuilder *target, TR::IlValue *left, TR::IlValue *right)
   {
   TR_ASSERT(target != NULL, "This IfCmpNotEqual requires a non-NULL builder object");
   TraceIL("IlBuilder[ %p ]::IfCmpNotEqual %d == %d? -> [ %p ] B%d\n", this, left->getID(), right->getID(), target, target->getEntry()->getNumber());
   ifCmpCondition(TR_cmpNE, false, left, right, target->getEntry());
   }

void
OMR::IlBuilder::IfCmpEqualZero(TR::IlBuilder **target, TR::IlValue *condition)
   {
   *target = createBuilderIfNeeded(*target);
   IfCmpEqualZero(*target, condition);
   }

void
OMR::IlBuilder::IfCmpEqualZero(TR::IlBuilder *target, TR::IlValue *condition)
   {
   TR_ASSERT(target != NULL, "This IfCmpEqualZero requires a non-NULL builder object");
   TraceIL("IlBuilder[ %p ]::IfCmpEqualZero %d == 0? -> [ %p ] B%d\n", this, condition->getID(), target, target->getEntry()->getNumber());
   ifCmpEqualZero(condition, target->getEntry());
   }

void
OMR::IlBuilder::IfCmpEqual(TR::IlBuilder **target, TR::IlValue *left, TR::IlValue *right)
   {
   *target = createBuilderIfNeeded(*target);
   IfCmpEqual(*target, left, right);
   }

void
OMR::IlBuilder::IfCmpEqual(TR::IlBuilder *target, TR::IlValue *left, TR::IlValue *right)
   {
   TR_ASSERT(target != NULL, "This IfCmpEqual requires a non-NULL builder object");
   TraceIL("IlBuilder[ %p ]::IfCmpEqual %d == %d? -> [ %p ] B%d\n", this, left->getID(), right->getID(), target, target->getEntry()->getNumber());
   ifCmpCondition(TR_cmpEQ, false, left, right, target->getEntry());
   }

void
OMR::IlBuilder::IfCmpLessThan(TR::IlBuilder **target, TR::IlValue *left, TR::IlValue *right)
   {
   *target = createBuilderIfNeeded(*target);
   IfCmpLessThan(*target, left, right);
   }

void
OMR::IlBuilder::IfCmpLessThan(TR::IlBuilder *target, TR::IlValue *left, TR::IlValue *right)
   {
   TR_ASSERT(target != NULL, "This IfCmpLessThan requires a non-NULL builder object");
   TraceIL("IlBuilder[ %p ]::IfCmpLessThan %d < %d? -> [ %p ] B%d\n", this, left->getID(), right->getID(), target, target->getEntry()->getNumber());
   ifCmpCondition(TR_cmpLT, false, left, right, target->getEntry());
   }

void
OMR::IlBuilder::IfCmpUnsignedLessThan(TR::IlBuilder **target, TR::IlValue *left, TR::IlValue *right)
   {
   *target = createBuilderIfNeeded(*target);
   IfCmpUnsignedLessThan(*target, left, right);
   }

void
OMR::IlBuilder::IfCmpUnsignedLessThan(TR::IlBuilder *target, TR::IlValue *left, TR::IlValue *right)
   {
   TR_ASSERT(target != NULL, "This IfCmpUnsignedLessThan requires a non-NULL builder object");
   TraceIL("IlBuilder[ %p ]::IfCmpUnsignedLessThan %d < %d? -> [ %p ] B%d\n", this, left->getID(), right->getID(), target, target->getEntry()->getNumber());
   ifCmpCondition(TR_cmpLT, true, left, right, target->getEntry());
   }

void
OMR::IlBuilder::IfCmpLessOrEqual(TR::IlBuilder **target, TR::IlValue *left, TR::IlValue *right)
   {
   *target = createBuilderIfNeeded(*target);
   IfCmpLessOrEqual(*target, left, right);
   }

void
OMR::IlBuilder::IfCmpLessOrEqual(TR::IlBuilder *target, TR::IlValue *left, TR::IlValue *right)
   {
   TR_ASSERT(target != NULL, "This IfCmpLessOrEqual requires a non-NULL builder object");
   TraceIL("IlBuilder[ %p ]::IfCmpLessOrEqual %d <= %d? -> [ %p ] B%d\n", this, left->getID(), right->getID(), target, target->getEntry()->getNumber());
   ifCmpCondition(TR_cmpLE, false, left, right, target->getEntry());
   }

void
OMR::IlBuilder::IfCmpUnsignedLessOrEqual(TR::IlBuilder **target, TR::IlValue *left, TR::IlValue *right)
   {
   *target = createBuilderIfNeeded(*target);
   IfCmpUnsignedLessOrEqual(*target, left, right);
   }

void
OMR::IlBuilder::IfCmpUnsignedLessOrEqual(TR::IlBuilder *target, TR::IlValue *left, TR::IlValue *right)
   {
   TR_ASSERT(target != NULL, "This IfCmpUnsignedLessOrEqual requires a non-NULL builder object");
   TraceIL("IlBuilder[ %p ]::IfCmpUnsignedLessOrEqual %d <= %d? -> [ %p ] B%d\n", this, left->getID(), right->getID(), target, target->getEntry()->getNumber());
   ifCmpCondition(TR_cmpLE, true, left, right, target->getEntry());
   }

void
OMR::IlBuilder::IfCmpGreaterThan(TR::IlBuilder **target, TR::IlValue *left, TR::IlValue *right)
   {
   *target = createBuilderIfNeeded(*target);
   IfCmpGreaterThan(*target, left, right);
   }

void
OMR::IlBuilder::IfCmpGreaterThan(TR::IlBuilder *target, TR::IlValue *left, TR::IlValue *right)
   {
   TraceIL("IlBuilder[ %p ]::IfCmpGreaterThan %d > %d? -> [ %p ] B%d\n", this, left->getID(), right->getID(), target, target->getEntry()->getNumber());
   ifCmpCondition(TR_cmpGT, false, left, right, target->getEntry());
   }

void
OMR::IlBuilder::IfCmpUnsignedGreaterThan(TR::IlBuilder **target, TR::IlValue *left, TR::IlValue *right)
   {
   *target = createBuilderIfNeeded(*target);
   IfCmpUnsignedGreaterThan(*target, left, right);
   }

void
OMR::IlBuilder::IfCmpUnsignedGreaterThan(TR::IlBuilder *target, TR::IlValue *left, TR::IlValue *right)
   {
   TraceIL("IlBuilder[ %p ]::IfCmpUnsignedGreaterThan %d > %d? -> [ %p ] B%d\n", this, left->getID(), right->getID(), target, target->getEntry()->getNumber());
   ifCmpCondition(TR_cmpGT, true, left, right, target->getEntry());
   }

void
OMR::IlBuilder::IfCmpGreaterOrEqual(TR::IlBuilder **target, TR::IlValue *left, TR::IlValue *right)
   {
   *target = createBuilderIfNeeded(*target);
   IfCmpGreaterOrEqual(*target, left, right);
   }

void
OMR::IlBuilder::IfCmpGreaterOrEqual(TR::IlBuilder *target, TR::IlValue *left, TR::IlValue *right)
   {
   TraceIL("IlBuilder[ %p ]::IfCmpGreaterOrEqual %d >= %d? -> [ %p ] B%d\n", this, left->getID(), right->getID(), target, target->getEntry()->getNumber());
   ifCmpCondition(TR_cmpGE, false, left, right, target->getEntry());
   }

void
OMR::IlBuilder::IfCmpUnsignedGreaterOrEqual(TR::IlBuilder **target, TR::IlValue *left, TR::IlValue *right)
   {
   *target = createBuilderIfNeeded(*target);
   IfCmpUnsignedGreaterOrEqual(*target, left, right);
   }

void
OMR::IlBuilder::IfCmpUnsignedGreaterOrEqual(TR::IlBuilder *target, TR::IlValue *left, TR::IlValue *right)
   {
   TraceIL("IlBuilder[ %p ]::IfCmpUnsignedGreaterOrEqual %d >= %d? -> [ %p ] B%d\n", this, left->getID(), right->getID(), target, target->getEntry()->getNumber());
   ifCmpCondition(TR_cmpGE, true, left, right, target->getEntry());
   }

void
OMR::IlBuilder::ifCmpCondition(TR_ComparisonTypes ct, bool isUnsignedCmp, TR::IlValue *left, TR::IlValue *right, TR::Block *target)
   {
   integerizeAddresses(&left, &right);
   TR::Node *leftNode = loadValue(left);
   TR::Node *rightNode = loadValue(right);
   TR::ILOpCode cmpOpCode(TR::ILOpCode::compareOpCode(leftNode->getDataType(), ct, isUnsignedCmp));
   ifjump(cmpOpCode.convertCmpToIfCmp(),
          leftNode,
          rightNode,
          target);
   appendBlock();
   }

void
OMR::IlBuilder::ifCmpNotEqualZero(TR::IlValue *condition, TR::Block *target)
   {
   ifCmpCondition(TR_cmpNE, false, condition, zeroForValue(condition), target);
   }

void
OMR::IlBuilder::ifCmpEqualZero(TR::IlValue *condition, TR::Block *target)
   {
   ifCmpCondition(TR_cmpEQ, false, condition, zeroForValue(condition), target);
   }

void
OMR::IlBuilder::appendGoto(TR::Block *destBlock)
   {
   gotoBlock(destBlock);
   appendBlock();
   }

/* Flexible builder for if...then...else structures
 * Basically 3 ways to call it:
 *    1) with then and else paths
 *    2) only a then path
 *    3) only an else path
 * #2 and #3 are similar: the existing path is appended and the proper condition causes branch around to the merge point
 * #1: then path exists "out of line", else path falls through from If and then jumps to merge, followed by then path which
 *     falls through to the merge point
 */
void
OMR::IlBuilder::IfThenElse(TR::IlBuilder **thenPath, TR::IlBuilder **elsePath, TR::IlValue *condition)
   {
   TR_ASSERT(thenPath != NULL || elsePath != NULL, "IfThenElse needs at least one conditional path");

   TR::Block *thenEntry = NULL;
   TR::Block *elseEntry = NULL;

   if (thenPath)
      {
      *thenPath = createBuilderIfNeeded(*thenPath);
      thenEntry = (*thenPath)->getEntry();
      }

   if (elsePath)
      {
      *elsePath = createBuilderIfNeeded(*elsePath);
      elseEntry = (*elsePath)->getEntry();
      }

   TR::Block *mergeBlock = emptyBlock();

   TraceIL("IlBuilder[ %p ]::IfThenElse %d", this, condition->getID());
   if (thenEntry)
      TraceIL(" then B%d", thenEntry->getNumber());
   if (elseEntry)
      TraceIL(" else B%d", elseEntry->getNumber());
   TraceIL(" merge B%d\n", mergeBlock->getNumber());

   if (thenPath == NULL) // case #3
      {
      ifCmpNotEqualZero(condition, mergeBlock);
      if ((*elsePath)->_partOfSequence)
         gotoBlock(elseEntry);
      else
         AppendBuilder(*elsePath);
      }
   else if (elsePath == NULL) // case #2
      {
      ifCmpEqualZero(condition, mergeBlock);
      if ((*thenPath)->_partOfSequence)
         gotoBlock(thenEntry);
      else
         AppendBuilder(*thenPath);
      }
   else // case #1
      {
      ifCmpNotEqualZero(condition, thenEntry);
      if ((*elsePath)->_partOfSequence)
         {
         gotoBlock(elseEntry);
         }
      else
         {
         AppendBuilder(*elsePath);
         appendGoto(mergeBlock);
         }
      if (!(*thenPath)->_partOfSequence)
         AppendBuilder(*thenPath);
      // if then path exists elsewhere already,
      //  then IfCmpNotEqual above already brances to it
      }

   // all paths possibly merge back here
   appendBlock(mergeBlock);
   }

void
OMR::IlBuilder::Switch(const char *selectionVar,
                  TR::IlBuilder **defaultBuilder,
                  uint32_t numCases,
                  JBCase **cases)
   {
   TR::IlValue *selectorValue = Load(selectionVar);
   TR_ASSERT(selectorValue->getDataType() == TR::Int32, "Switch only supports selector having type Int32");
   *defaultBuilder = createBuilderIfNeeded(*defaultBuilder);

   TR::Node *defaultNode = TR::Node::createCase(0, (*defaultBuilder)->getEntry()->getEntry());
   TR::Node *lookupNode = TR::Node::create(TR::lookup, numCases + 2, loadValue(selectorValue), defaultNode);

   generateSwitchCases(lookupNode, defaultNode, defaultBuilder, numCases, cases);
   }

void
OMR::IlBuilder::Switch(const char *selectionVar,
                  TR::IlBuilder **defaultBuilder,
                  uint32_t numCases,
                  ...)
   {
   va_list args;
   va_start(args, numCases);
   JBCase **cases = createCaseArray(numCases, args);
   va_end(args);

   Switch(selectionVar, defaultBuilder, numCases, cases);
   }

void
OMR::IlBuilder::TableSwitch(const char *selectionVar,
                  TR::IlBuilder **defaultBuilder,
                  bool generateBoundsCheck,
                  uint32_t numCases,
                  JBCase **cases)
   {
   TR::IlValue *selectorValue = Load(selectionVar);
   TR_ASSERT(selectorValue->getDataType() == TR::Int32, "TableSwitch only supports selector having type Int32");
   TR_ASSERT(numCases > 0, "TableSwitch requires at least 1 case");
   int32_t low = cases[0]->_value;
   int32_t high = cases[numCases -1]->_value;
   int32_t casesCovered = (high - low) + 1;
   TR_ASSERT(numCases == casesCovered, "TableSwitch only supports dense case sets");
   if (low != 0)
      selectorValue = Sub(selectorValue, ConstInt32(low));

   *defaultBuilder = createBuilderIfNeeded(*defaultBuilder);

   TR::Node *defaultNode = TR::Node::createCase(0, (*defaultBuilder)->getEntry()->getEntry());
   TR::Node *tableNode = TR::Node::create(TR::table, numCases + 2, loadValue(selectorValue), defaultNode);
   if (!generateBoundsCheck)
       tableNode->setIsSafeToSkipTableBoundCheck(true);

   generateSwitchCases(tableNode, defaultNode, defaultBuilder, numCases, cases);
   }

void
OMR::IlBuilder::TableSwitch(const char *selectionVar,
                  TR::IlBuilder **defaultBuilder,
                  bool generateBoundsCheck,
                  uint32_t numCases,
                  ...)
   {
   va_list args;
   va_start(args, numCases);
   JBCase **cases = createCaseArray(numCases, args);
   va_end(args);

   TableSwitch(selectionVar, defaultBuilder, generateBoundsCheck, numCases, cases);
   }

TR::IlBuilder::JBCase *
OMR::IlBuilder::MakeCase(int32_t caseValue, TR::IlBuilder **caseBuilder, int32_t caseFallsThrough)
   {
   TR_ASSERT(caseBuilder != NULL, "MakeCase, needs to have non-null caseBuilder");
   *caseBuilder = createBuilderIfNeeded(*caseBuilder);
   auto * c = new (_comp->trHeapMemory()) JBCase(caseValue, *caseBuilder, caseFallsThrough);
   return c;
   }

TR::IlBuilder::JBCase **
OMR::IlBuilder::createCaseArray(uint32_t numCases, va_list args)
   {
   JBCase **cases = (JBCase **) _comp->trMemory()->allocateHeapMemory(numCases * sizeof(JBCase *));
   TR_ASSERT(NULL != cases, "out of memory");

   for (uint32_t c = 0; c < numCases; ++c)
      {
      cases[c] = va_arg(args, JBCase *);
      }

   return cases;
   }

void
OMR::IlBuilder::generateSwitchCases(TR::Node *switchNode, TR::Node *defaultNode, TR::IlBuilder **defaultBuilder, uint32_t numCases, JBCase **cases)
   {
   // get the lookup tree into this builder, even though we haven't completely filled it in yet
   genTreeTop(switchNode);
   TR::Block *switchBlock = _currentBlock;

   // make sure no fall through edge created from the lookup
   appendNoFallThroughBlock();

   TR::IlBuilder *breakBuilder = OrphanBuilder();
   // each case handler is a sequence of two builder objects: first the one passed in via `cases`,
   //   and second a builder that branches to the breakBuilder (unless this case falls through)
   for (int32_t c = 0; c < numCases; c++)
      {
      int32_t value = cases[c]->_value;
      TR::IlBuilder *handler = NULL;
      TR::IlBuilder *builder = cases[c]->_builder;
      if (!cases[c]->_fallsThrough)
         {
         handler = OrphanBuilder();
         handler->AppendBuilder(builder);

         // handle "break" with a separate builder so user can add whatever they want into caseBuilders[c]
         TR::IlBuilder *branchToBreak = OrphanBuilder();
         branchToBreak->Goto(&breakBuilder);
         handler->AppendBuilder(branchToBreak);
         }
      else
         {
         handler = builder;
         }

      TR::Block *caseBlock = handler->getEntry();
      cfg()->addEdge(switchBlock, caseBlock);
      AppendBuilder(handler);

      TR::Node *caseNode = TR::Node::createCase(0, caseBlock->getEntry(), value);
      switchNode->setAndIncChild(c+2, caseNode);
      }

   cfg()->addEdge(switchBlock, (*defaultBuilder)->getEntry());
   AppendBuilder(*defaultBuilder);

   AppendBuilder(breakBuilder);
   }

void
OMR::IlBuilder::ForLoop(bool countsUp,
                   const char *indVar,
                   TR::IlBuilder **loopCode,
                   TR::IlBuilder **breakBuilder,
                   TR::IlBuilder **continueBuilder,
                   TR::IlValue *initial,
                   TR::IlValue *end,
                   TR::IlValue *increment)
   {
   methodSymbol()->setMayHaveLoops(true);
   TR_ASSERT(loopCode != NULL, "ForLoop needs to have loopCode builder");
   *loopCode = createBuilderIfNeeded(*loopCode);

   TraceIL("IlBuilder[ %p ]::ForLoop ind %s initial %d end %d increment %d loopCode %p countsUp %d\n", this, indVar, initial->getID(), end->getID(), increment->getID(), *loopCode, countsUp);

   Store(indVar, initial);

   TR::IlValue *loopCondition;
   TR::IlBuilder *loopBody = OrphanBuilder();
   loopCondition = countsUp ? LessThan(Load(indVar), end) : GreaterThan(Load(indVar), end);
   IfThen(&loopBody, loopCondition);
   loopBody->AppendBuilder(*loopCode);
   TR::IlBuilder *loopContinue = OrphanBuilder();
   loopBody->AppendBuilder(loopContinue);

   if (breakBuilder)
      {
      TR_ASSERT(*breakBuilder == NULL, "ForLoop returns breakBuilder, cannot provide breakBuilder as input");
      *breakBuilder = OrphanBuilder();
      AppendBuilder(*breakBuilder);
      }

   if (continueBuilder)
      {
      TR_ASSERT(*continueBuilder == NULL, "ForLoop returns continueBuilder, cannot provide continueBuilder as input");
      *continueBuilder = loopContinue;
      }

   if (countsUp)
      {
      loopContinue->Store(indVar,
      loopContinue->   Add(
      loopContinue->      Load(indVar),
                          increment));
      loopContinue->IfCmpLessThan(&loopBody,
      loopContinue->   Load(indVar),
                       end);
      }
   else
      {
      loopContinue->Store(indVar,
      loopContinue->   Sub(
      loopContinue->      Load(indVar),
                          increment));
      loopContinue->IfCmpGreaterThan(&loopBody,
      loopContinue->   Load(indVar),
                       end);
      }

   // make sure any subsequent operations go into their own block *after* the loop
   appendBlock();
   }

void
OMR::IlBuilder::DoWhileLoop(const char *whileCondition, TR::IlBuilder **body, TR::IlBuilder **breakBuilder, TR::IlBuilder **continueBuilder)
   {
   methodSymbol()->setMayHaveLoops(true);
   TR_ASSERT(body != NULL, "doWhileLoop needs to have a body");

   if (!_methodBuilder->symbolDefined(whileCondition))
      _methodBuilder->defineValue(whileCondition, Int32);

   *body = createBuilderIfNeeded(*body);
   TraceIL("IlBuilder[ %p ]::DoWhileLoop do body B%d while %s\n", this, (*body)->getEntry()->getNumber(), whileCondition);

   AppendBuilder(*body);
   TR::IlBuilder *loopContinue = NULL;

   if (continueBuilder)
      {
      TR_ASSERT(*continueBuilder == NULL, "DoWhileLoop returns continueBuilder, cannot provide continueBuilder as input");
      loopContinue = *continueBuilder = OrphanBuilder();
      }
   else
      loopContinue = OrphanBuilder();

   AppendBuilder(loopContinue);
   loopContinue->IfCmpNotEqualZero(body,
   loopContinue->   Load(whileCondition));

   if (breakBuilder)
      {
      TR_ASSERT(*breakBuilder == NULL, "DoWhileLoop returns breakBuilder, cannot provide breakBuilder as input");
      *breakBuilder = OrphanBuilder();
      AppendBuilder(*breakBuilder);
      }

   // make sure any subsequent operations go into their own block *after* the loop
   appendBlock();
   }

void
OMR::IlBuilder::WhileDoLoop(const char *whileCondition, TR::IlBuilder **body, TR::IlBuilder **breakBuilder, TR::IlBuilder **continueBuilder)
   {
   methodSymbol()->setMayHaveLoops(true);
   TR_ASSERT(body != NULL, "WhileDo needs to have a body");
   TraceIL("IlBuilder[ %p ]::WhileDoLoop while %s do body %p\n", this, whileCondition, *body);

   TR::IlBuilder *done = OrphanBuilder();
   if (breakBuilder)
      {
      TR_ASSERT(*breakBuilder == NULL, "WhileDoLoop returns breakBuilder, cannot provide breakBuilder as input");
      *breakBuilder = done;
      }

   TR::IlBuilder *loopContinue = OrphanBuilder();
   if (continueBuilder)
      {
      TR_ASSERT(*continueBuilder == NULL, "WhileDoLoop returns continueBuilder, cannot provide continueBuilder as input");
      *continueBuilder = loopContinue;
      }

   AppendBuilder(loopContinue);
   loopContinue->IfCmpEqualZero(&done,
   loopContinue->   Load(whileCondition));

   *body = createBuilderIfNeeded(*body);
   AppendBuilder(*body);

   Goto(&loopContinue);
   setComesBack(); // this goto is on one particular flow path, doesn't mean every path does a goto

   AppendBuilder(done);
   }

void *
OMR::IlBuilder::client()
   {
   if (_client == NULL && _clientAllocator != NULL)
      _client = _clientAllocator(static_cast<TR::IlBuilder *>(this));
   return _client;
   }

void *
OMR::IlBuilder::JBCase::client()
   {
   if (_client == NULL && _clientAllocator != NULL)
      _client = _clientAllocator(static_cast<TR::IlBuilder::JBCase *>(this));
   return _client;
   }

void *
OMR::IlBuilder::JBCondition::client()
   {
   if (_client == NULL && _clientAllocator != NULL)
      _client = _clientAllocator(static_cast<TR::IlBuilder::JBCondition *>(this));
   return _client;
   }

ClientAllocator OMR::IlBuilder::_clientAllocator = NULL;
ClientAllocator OMR::IlBuilder::_getImpl = NULL;
ClientAllocator OMR::IlBuilder::JBCase::_clientAllocator = NULL;
ClientAllocator OMR::IlBuilder::JBCase::_getImpl = NULL;
ClientAllocator OMR::IlBuilder::JBCondition::_clientAllocator = NULL;
ClientAllocator OMR::IlBuilder::JBCondition::_getImpl = NULL;
