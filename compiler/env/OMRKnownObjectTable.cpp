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

#include "env/KnownObjectTable.hpp"
#include "codegen/FrontEnd.hpp"
#ifdef J9_PROJECT_SPECIFIC
#include "env/VMAccessCriticalSection.hpp"
#endif
#include "compile/Compilation.hpp"
#include "env/CompilerEnv.hpp"


#define notImplemented(A) TR_ASSERT(0, "TR_FrontEnd::%s is undefined", (A) )


OMR::KnownObjectTable::KnownObjectTable(TR::Compilation *comp) :
      _comp(comp),
      _fe(comp->fe())
   {}


TR::KnownObjectTable *
OMR::KnownObjectTable::self()
   {
   return static_cast<TR::KnownObjectTable *>(this);
   }

TR::KnownObjectTable::Index
OMR::KnownObjectTable::getEndIndex()
   {
   notImplemented("OMR::KnownObjectTable::getEndIndex");
   return -1;
   }

TR::KnownObjectTable::Index
OMR::KnownObjectTable::getIndex(uintptrj_t objectPointer)
   {
   notImplemented("OMR::KnownObjectTable::getIndex");
   return -1;
   }

uintptrj_t *
OMR::KnownObjectTable::getPointerLocation(Index index)
   {
   notImplemented("OMR::KnownObjectTable::getPointerLocation");
   return NULL;
   }

bool
OMR::KnownObjectTable::isNull(Index index)
   {
   notImplemented("OMR::KnownObjectTable::isNull");
   return false;
   }

void
OMR::KnownObjectTable::dumpTo(TR::FILE *file, TR::Compilation *comp)
   {
   notImplemented("OMR::KnownObjectTable::dumpTo");
   }

TR::KnownObjectTable::Index
OMR::KnownObjectTable::getIndexAt(uintptrj_t *objectReferenceLocation)
   {
#ifdef J9_PROJECT_SPECIFIC
   TR::VMAccessCriticalSection getIndexCriticalSection(self()->comp());
#endif
   uintptrj_t objectPointer = *objectReferenceLocation; // Note: object references held as uintptrj_t must never be compressed refs
   Index result = self()->getIndex(objectPointer);
   return result;
   }

TR::KnownObjectTable::Index
OMR::KnownObjectTable::getExistingIndexAt(uintptrj_t *objectReferenceLocation)
   {
   Index result = UNKNOWN;
#ifdef J9_PROJECT_SPECIFIC
   TR::VMAccessCriticalSection getIndexCriticalSection(self()->comp());
#endif
   uintptrj_t objectPointer = *objectReferenceLocation;
   for (Index i = 0; i < self()->getEndIndex() && (result == UNKNOWN); i++)
      {
      if (self()->getPointer(i) == objectPointer)
         result = i;
      }
   return result;
   }

uintptrj_t
OMR::KnownObjectTable::getPointer(Index index)
   {
   TR_ASSERT(TR::Compiler->vm.hasAccess(self()->comp()), "Getting a heap object pointer requires VM access");
   if (self()->isNull(index))
      return 0; // Assumes host and target representations of null match each other
   else
      return *self()->getPointerLocation(index);
   }

uintptrj_t *
OMR::KnownObjectTable::getfPointerLocationAt(uintptrj_t *objectReferenceLocation)
   {
   return self()->getPointerLocation(self()->getIndexAt(objectReferenceLocation));
   }
