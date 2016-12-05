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
 ******************************************************************************/

#include "il/symbol/OMRLabelSymbol.hpp"

#include <stddef.h>                            // for NULL, size_t
#include <stdint.h>                            // for intptr_t
#include <stdio.h>                             // for sprintf
#include <stdlib.h>                            // for calloc
#include "codegen/CodeGenPhase.hpp"            // for CodeGenPhase
#include "codegen/CodeGenerator.hpp"           // for CodeGenerator
#include "compile/Compilation.hpp"             // for Compilation, comp
#include "env/TRMemory.hpp"                    // for PERSISTENT_NEW_DECLARE, etc
#include "il/Block.hpp"                        // for Block
#include "il/DataTypes.hpp"                    // for TR_YesNoMaybe, etc
#include "il/Symbol.hpp"                       // for Symbol, etc
#include "il/symbol/LabelSymbol.hpp"           // for LabelSymbol
#include "il/symbol/ResolvedMethodSymbol.hpp"  // for ResolvedMethodSymbol
#include "il/symbol/StaticSymbol.hpp"          // for StaticSymbol
#include "infra/Assert.hpp"                    // for TR_ASSERT
#include "ras/Debug.hpp"                       // for TR_DebugBase

namespace TR { class ParameterSymbol; }
namespace TR { class Snippet; }
template <class T> class List;

TR::LabelSymbol *
OMR::LabelSymbol::self()
   {
   return static_cast<TR::LabelSymbol*>(this);
   }

template <typename AllocatorType>
TR::LabelSymbol * OMR::LabelSymbol::create(AllocatorType t)
   {
   return new (t) TR::LabelSymbol();
   }

template <typename AllocatorType>
TR::LabelSymbol * OMR::LabelSymbol::create(AllocatorType t, TR::CodeGenerator* c)
   {
   return new (t) TR::LabelSymbol(c);
   }

template <typename AllocatorType>
TR::LabelSymbol * OMR::LabelSymbol::create(AllocatorType t, TR::CodeGenerator* c, TR::Block* b)
   {
   return new (t) TR::LabelSymbol(c,b);
   }

OMR::LabelSymbol::LabelSymbol() :
   TR::Symbol(),
   _instruction(NULL),
   _codeLocation(NULL),
   _estimatedCodeLocation(0),
   _snippet(NULL),
   _vmThreadRestoringLabel(NULL),
   _directlyTargeted(false)
   {
   self()->setIsLabel();

   TR::Compilation *comp = TR::comp();
   if (comp && comp->getDebug())
      comp->getDebug()->newLabelSymbol(self());
   }

OMR::LabelSymbol::LabelSymbol(TR::CodeGenerator *codeGen) :
   TR::Symbol(),
   _instruction(NULL),
   _codeLocation(NULL),
   _estimatedCodeLocation(0),
   _snippet(NULL),
   _vmThreadRestoringLabel(NULL)
   {
   self()->setIsLabel();

   TR::Compilation *comp = TR::comp();
   if (comp && comp->getDebug())
      comp->getDebug()->newLabelSymbol(self());
   }

OMR::LabelSymbol::LabelSymbol(TR::CodeGenerator *codeGen, TR::Block *labb) :
   TR::Symbol(),
   _instruction(NULL),
   _codeLocation(NULL),
   _estimatedCodeLocation(0),
   _snippet(NULL),
   _vmThreadRestoringLabel(NULL)
   {
   self()->setIsLabel();

   TR::Compilation *comp = TR::comp();
   if (comp && comp->getDebug())
      comp->getDebug()->newLabelSymbol(self());
   }

TR_YesNoMaybe
OMR::LabelSymbol::isTargeted()
   {
   if (TR::comp()->cg() && TR::comp()->cg()->getCodeGeneratorPhase() <= TR::CodeGenPhase::InstructionSelectionPhase)
      return TR_maybe;
   return _directlyTargeted ? TR_yes : TR_no;
   }

const char *
OMR::LabelSymbol::getName(TR_Debug *debug)
   {
   if (debug)
      return debug->getName(self());
   else
      return "<unknown labelsym>";
   }

void
OMR::LabelSymbol::makeRelativeLabelSymbol(intptr_t offset)
   {
   // Is this assert here purely to ensure that the label size doesn't blow the buffer?
   TR_ASSERT(offset*2 > -9999999 && offset*2 < +9999999, "assertion failure");

   self()->setRelativeLabel();
   _offset = offset;
   char * name = (char*)calloc(10,sizeof(char));  // FIXME: Leaked.
   sprintf(name, "%d", (int)(offset*2));
   self()->setName(name);
   }

intptr_t
OMR::LabelSymbol::getDistance()
   {
   TR_ASSERT(self()->isRelativeLabel(), "Must be a relative label to have a valid offset!");
   return _offset;
   }

TR::LabelSymbol *
generateLabelSymbol(TR::CodeGenerator *cg)
   {
   return TR::LabelSymbol::create(cg->trHeapMemory(), cg);
   }

template <typename AllocatorType>
TR::LabelSymbol *
OMR::LabelSymbol::createRelativeLabel(AllocatorType m, TR::CodeGenerator * cg, intptr_t offset)
   {
   TR::LabelSymbol * rel = new (m) TR::LabelSymbol();
   rel->makeRelativeLabelSymbol(offset);
   return rel;
   }

// Explicit instantiation
template TR::LabelSymbol * OMR::LabelSymbol::create(TR_HeapMemory t);
template TR::LabelSymbol * OMR::LabelSymbol::create(TR_HeapMemory t, TR::CodeGenerator* c);
template TR::LabelSymbol * OMR::LabelSymbol::create(TR_HeapMemory t, TR::CodeGenerator* c, TR::Block* b);
template TR::LabelSymbol * OMR::LabelSymbol::createRelativeLabel(TR_HeapMemory m, TR::CodeGenerator * cg, intptr_t offset);

template TR::LabelSymbol * OMR::LabelSymbol::create(TR_StackMemory t);
template TR::LabelSymbol * OMR::LabelSymbol::create(TR_StackMemory t, TR::CodeGenerator* c);
template TR::LabelSymbol * OMR::LabelSymbol::create(TR_StackMemory t, TR::CodeGenerator* c, TR::Block* b);
template TR::LabelSymbol * OMR::LabelSymbol::createRelativeLabel(TR_StackMemory m, TR::CodeGenerator * cg, intptr_t offset);

template TR::LabelSymbol * OMR::LabelSymbol::create(PERSISTENT_NEW_DECLARE t);
template TR::LabelSymbol * OMR::LabelSymbol::create(PERSISTENT_NEW_DECLARE t, TR::CodeGenerator* c);
template TR::LabelSymbol * OMR::LabelSymbol::create(PERSISTENT_NEW_DECLARE t, TR::CodeGenerator* c, TR::Block* b);
template TR::LabelSymbol * OMR::LabelSymbol::createRelativeLabel(PERSISTENT_NEW_DECLARE m, TR::CodeGenerator * cg, intptr_t offset);

template TR::LabelSymbol * OMR::LabelSymbol::create(TR_ArenaAllocator* t, TR::CodeGenerator* c);
