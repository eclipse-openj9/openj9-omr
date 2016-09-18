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

#include <algorithm>                // for std::max
#include <stddef.h>                 // for NULL
#include <stdint.h>                 // for int32_t
#include "env/TRMemory.hpp"         // for TR_AllocationKind, etc
#include "infra/Assert.hpp"         // for TR_ASSERT
#include "infra/Cfg.hpp"            // for MAX_BLOCK_COUNT, etc
#include "optimizer/CallInfo.hpp"   // for TR_CallTarget (ptr only), etc
#include "optimizer/Inliner.hpp"    // for TR_InlinerBase, etc

class TR_OpaqueClassBlock;
class TR_PrexArgInfo;
class TR_ResolvedMethod;
namespace TR { class ResolvedMethodSymbol; }
namespace TR { class SymbolReference; }
namespace TR { class Compilation; }
namespace TR { class Node; }
namespace TR { class TreeTop; }

TR_CallSite* TR_CallSite::create(TR::TreeTop* callNodeTreeTop,
                           TR::Node *parent,
                           TR::Node* callNode,
                           TR_OpaqueClassBlock *receiverClass,
                           TR::SymbolReference *symRef,
                           TR_ResolvedMethod *resolvedMethod,
                           TR::Compilation* comp,
                           TR_Memory* trMemory,
                           TR_AllocationKind kind,
                           TR_ResolvedMethod* caller,
                           int32_t depth,
                           bool allConsts)

   {
   TR_ASSERT(0, "static ctor TR_CallSite::create must be implemented");
   return NULL;
   }

bool TR_InlinerBase::tryToGenerateILForMethod (TR::ResolvedMethodSymbol* calleeSymbol, TR::ResolvedMethodSymbol* callerSymbol, TR_CallTarget* calltarget)
   {
   return false;
   }

bool TR_InlinerBase::inlineCallTarget(TR_CallStack *callStack, TR_CallTarget *calltarget, bool inlinefromgraph, TR_PrexArgInfo *argInfo, TR::TreeTop** cursorTreeTop)
   {
   TR_ASSERT(0, "TR_InlinerBase::inlineCallTarget must be implemented");
   return false;
   }

void TR_InlinerBase::getBorderFrequencies(int32_t &hotBorderFrequency, int32_t &coldBorderFrequency, TR_ResolvedMethod * calleeResolvedMethod, TR::Node *callNode)
   {
   hotBorderFrequency = 2500;
   coldBorderFrequency = 0;
   return;
   }

int32_t TR_InlinerBase::scaleSizeBasedOnBlockFrequency(int32_t bytecodeSize, int32_t frequency, int32_t borderFrequency, TR_ResolvedMethod * calleeResolvedMethod, TR::Node *callNode, int32_t coldBorderFrequency)
   {
   int32_t maxFrequency = MAX_BLOCK_COUNT + MAX_COLD_BLOCK_COUNT;
   bytecodeSize = (int)((float)bytecodeSize * (float)(maxFrequency-borderFrequency)/(float)maxFrequency);
              if (bytecodeSize < 10) bytecodeSize = 10;

   return bytecodeSize;
   }

int TR_InlinerBase::checkInlineableWithoutInitialCalleeSymbol (TR_CallSite* callsite, TR::Compilation* comp)
   {
   return Unknown_Reason;
   }
