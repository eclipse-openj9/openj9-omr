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

#include "optimizer/PreExistence.hpp"

#include "env/CompilerEnv.hpp"
#include "env/KnownObjectTable.hpp"    // for KnownObjectTable, etc
#include "compile/Compilation.hpp"         // for Compilation
#ifdef J9_PROJECT_SPECIFIC
#include "env/VMAccessCriticalSection.hpp" // for VMAccessCriticalSEction
#endif
#include "optimizer/Inliner.hpp"

PrexKnowledgeLevel
TR_PrexArgument::knowledgeLevel(TR_PrexArgument *pa)
   {
   if (!pa)
      return NONE;
   else if (pa->hasKnownObjectIndex())
      return KNOWN_OBJECT;
   else if (pa->getFixedClass())
      return FIXED_CLASS;
   else if (pa->classIsPreexistent())
      return PREEXISTENT;
   else
      return NONE;
   }

TR_PrexArgument::TR_PrexArgument(
      TR::KnownObjectTable::Index knownObjectIndex,
      TR::Compilation *comp) :
   _classKind(ClassIsUnknown),
   _fixedClass(0),
   _profiledClazz(0),
   _knownObjectIndex(knownObjectIndex)
   {
#ifdef J9_PROJECT_SPECIFIC
   TR::VMAccessCriticalSection prexArgumentCriticalSection(comp,
                                                            TR::VMAccessCriticalSection::tryToAcquireVMAccess);

   if (prexArgumentCriticalSection.hasVMAccess())
      {
      _fixedClass = TR::Compiler->cls.objectClass(comp, comp->getKnownObjectTable()->getPointer(knownObjectIndex));
      _classKind = ClassIsFixed;
      }
#endif
   }
