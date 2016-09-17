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

#ifndef OMR_SNIPPETGCMAP_INCL
#define OMR_SNIPPETGCMAP_INCL

/*
 * The following #define and typedef must appear before any #includes in this file
 */
#ifndef OMR_SNIPPETGCMAP_CONNECTOR
#define OMR_SNIPPETGCMAP_CONNECTOR
namespace OMR { class SnippetGCMap; }
namespace OMR { typedef OMR::SnippetGCMap SnippetGCMapConnector; }
#endif

#include "infra/Flags.hpp"

namespace TR { class Instruction; }
namespace TR { class CodeGenerator; }
class TR_GCStackMap;

namespace OMR
{

class SnippetGCMap
   {
   public:

   SnippetGCMap() :
      _flags(0),
      _GCRegisterMask(0),
      _stackMap(0)
      {
      }

   TR_GCStackMap *getStackMap() { return _stackMap; }
   void setStackMap(TR_GCStackMap *m) { _stackMap = m; }

   uint32_t getGCRegisterMask() { return _GCRegisterMask; }
   void setGCRegisterMask(uint32_t regMask) { _GCRegisterMask = regMask; }

   bool isGCSafePoint() { return _flags.testAll(TO_MASK8(GCSafePoint)); }
   void setGCSafePoint() { _flags.set(TO_MASK8(GCSafePoint)); }
   void resetGCSafePoint() { _flags.reset(TO_MASK8(GCSafePoint)); }

   void registerStackMap(TR::Instruction *instruction, TR::CodeGenerator *cg);
   void registerStackMap(uint8_t *callSiteAddress, TR::CodeGenerator *cg);

   enum
      {
      GCSafePoint = 0
      };

   protected:

   flags8_t _flags;
   uint32_t _GCRegisterMask;
   TR_GCStackMap *_stackMap;
   };

}

#endif
