/*******************************************************************************
 *
 * (c) Copyright IBM Corp. 2000, 2016
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
 *******************************************************************************/

#include "compile/Compilation.hpp"
#include "compile/Method.hpp"
#include "env/FrontEnd.hpp"
#include "infra/List.hpp"
#include "il/Block.hpp"
#include "ilgen/IlBuilder.hpp"
#include "ilgen/MethodBuilder.hpp"
#include "ilgen/BytecodeBuilder.hpp"
#include "ilgen/TypeDictionary.hpp"

// should really move into IlInjector.hpp
#define TraceEnabled    (comp()->getOption(TR_TraceILGen))
#define TraceIL(m, ...) {if (TraceEnabled) {traceMsg(comp(), m, ##__VA_ARGS__);}}


OMR::BytecodeBuilder::BytecodeBuilder(TR::MethodBuilder *methodBuilder,
                                      int32_t bcIndex,
                                      char *name)
   : TR::IlBuilder(methodBuilder, methodBuilder->typeDictionary()),
   _fallThroughBuilder(0),
   _bcIndex(bcIndex),
   _name(name)
   {
   _successorBuilders = new (PERSISTENT_NEW) List<TR::BytecodeBuilder>(_types->trMemory());
   }

void
TR::BytecodeBuilder::initialize(TR::IlGeneratorMethodDetails * details,
                           TR::ResolvedMethodSymbol     * methodSymbol,
                           TR::FrontEnd                 * fe,
                           TR::SymbolReferenceTable     * symRefTab)
    {
    _details = details;
    _methodSymbol = methodSymbol;
    _fe = fe;
    _symRefTab = symRefTab;
    _comp = TR::comp();
    //addBytecodeBuilderToList relies on _comp and it won't be ready until now
    _methodBuilder->addBytecodeBuilderToList(this);
    }

void
OMR::BytecodeBuilder::appendBlock(TR::Block *block, bool addEdge)
   {
   if (block == NULL)
      {
      block = emptyBlock();
      }

   block->setByteCodeIndex(_bcIndex, _comp);
   return TR::IlBuilder::appendBlock(block, addEdge);
   }

uint32_t
OMR::BytecodeBuilder::countBlocks()
   {
   // only count each block once
   if (_count > -1)
      return _count;

   TraceIL("[ %p ] TR::BytecodeBuilder::countBlocks 0\n", this);

   _count = TR::IlBuilder::countBlocks();

   if (NULL != _fallThroughBuilder)
      _methodBuilder->addToBlockCountingWorklist(_fallThroughBuilder);

   ListIterator<TR::BytecodeBuilder> iter(_successorBuilders);
   for (TR::BytecodeBuilder *builder = iter.getFirst(); !iter.atEnd(); builder = iter.getNext())
      if (builder->_count < 0)
         _methodBuilder->addToBlockCountingWorklist(builder);

   TraceIL("[ %p ] TR::BytecodeBuilder::countBlocks %d\n", this, _count);

   return _count;
   }

bool
OMR::BytecodeBuilder::connectTrees()
   {
   if (!_connectedTrees)
      {
      TraceIL("[ %p ] TR::BytecodeBuilder::connectTrees\n", this);
      bool rc = TR::IlBuilder::connectTrees();
      addAllSuccessorBuildersToWorklist();
      return rc;
      }

   return true;
   }


void
OMR::BytecodeBuilder::addAllSuccessorBuildersToWorklist()
   {
   if (NULL != _fallThroughBuilder)
      _methodBuilder->addToTreeConnectingWorklist(_fallThroughBuilder);

   // iterate over _successorBuilders
   ListIterator<TR::BytecodeBuilder> iter(_successorBuilders);
   for (TR::BytecodeBuilder *builder = iter.getFirst(); !iter.atEnd(); builder = iter.getNext())
      _methodBuilder->addToTreeConnectingWorklist(builder);
   }

void
OMR::BytecodeBuilder::AddFallThroughBuilder(TR::BytecodeBuilder *ftb)
    {
    TR_ASSERT(comesBack(), "builder does not appear to have a fall through path");

    _fallThroughBuilder = ftb;
    TraceIL("IlBuilder[ %p ]:: fallThrough successor [ %p ]\n", this, ftb);

    // add explicit goto the fall-through block
    // requires that AddFallThroughBuilder() be called *after* any code is added to it
    TR::IlBuilder *b = ftb;
    Goto(&b);
    }

void
OMR::BytecodeBuilder::AddSuccessorBuilders(uint32_t numExits, ...)
   {
   va_list exits;
   va_start(exits, numExits);
   for (int32_t e=0;e < numExits;e++)
      {
      TR::BytecodeBuilder *builder = (TR::BytecodeBuilder *) va_arg(exits, TR::BytecodeBuilder *);
      _successorBuilders->add(builder);
      TraceIL("IlBuilder[ %p ]:: successor [ %p ]\n", this, builder);
      }
   va_end(exits);
   }

void
OMR::BytecodeBuilder::setHandlerInfo(uint32_t catchType)
   {
   TR::Block *catchBlock = getEntry();
   catchBlock->setIsCold();
   catchBlock->setHandlerInfo(catchType, comp()->getInlineDepth(), -1, _methodSymbol->getResolvedMethod(), comp());
   }

