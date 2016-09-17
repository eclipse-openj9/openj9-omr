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

#ifndef OMR_KNOWN_OBJECT_TABLE_INCL
#define OMR_KNOWN_OBJECT_TABLE_INCL

/*
 * The following #define and typedef must appear before any #includes in this file
 */
#ifndef OMR_KNOWN_OBJECT_TABLE_CONNECTOR
#define OMR_KNOWN_OBJECT_TABLE_CONNECTOR
namespace OMR { class KnownObjectTable; }
namespace OMR { typedef OMR::KnownObjectTable KnownObjectTableConnector; }
#endif


#include <stdint.h>                 // for int32_t
#include "env/FilePointerDecl.hpp"  // for FILE
#include "env/jittypes.h"           // for uintptrj_t
#include "infra/Annotations.hpp"    // for OMR_EXTENSIBLE

class TR_FrontEnd;
namespace TR { class Compilation; }
namespace TR { class KnownObjectTable; }

namespace OMR
{

 /**
  * Table of known objects.
  *
  * These are cases where language properties allows the compiler to know
  * statically not just the type of the object, but the identity of the object.
  */
class OMR_EXTENSIBLE KnownObjectTable
   {
   TR_FrontEnd *_fe;

protected:

   KnownObjectTable(TR::Compilation *comp);
   TR::Compilation *_comp;

public:

   TR::KnownObjectTable * self();

   typedef int32_t Index;
   static const Index UNKNOWN = -1;

   TR_FrontEnd *fe() { return _fe; }
   void setFe(TR_FrontEnd *fe) { _fe = fe; }

   TR::Compilation *comp() { return _comp; }
   void setComp(TR::Compilation *comp) { _comp = comp; }

   virtual Index getEndIndex();                      // Highest index assigned so far + 1
   virtual Index getIndex(uintptrj_t objectPointer); // Must hold vm access for this
   virtual uintptrj_t *getPointerLocation(Index index);
   virtual bool isNull(Index index);

   virtual void dumpTo(TR::FILE *file, TR::Compilation *comp);

   // Handy wrappers

   Index getIndexAt(uintptrj_t *objectReferenceLocation);
   Index getExistingIndexAt(uintptrj_t *objectReferenceLocation);

   uintptrj_t getPointer(Index index);
   uintptrj_t *getfPointerLocationAt(uintptrj_t *objectReferenceLocation);

   };
}

#endif
