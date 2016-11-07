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

#ifndef IA32DATASNIPPET_INCL
#define IA32DATASNIPPET_INCL

#include "codegen/Snippet.hpp"

#include <stdint.h>                  // for uint8_t, int32_t, int64_t, etc

namespace TR { class CodeGenerator; }
namespace TR { class Node; }

namespace TR {

class IA32DataSnippet : public TR::Snippet
   {
   uint8_t _length;
   bool    _isClassAddress;
   protected:
   uint8_t _value[16];
   public:

   IA32DataSnippet(TR::CodeGenerator *cg, TR::Node *, void *c, uint8_t size);

   virtual Kind getKind() { return IsData; }
   uint8_t* getValue()  { return _value; }
   virtual uint8_t *emitSnippetBody();
   virtual uint8_t getDataSize() { return _length; }
   virtual uint32_t getLength(int32_t estimatedSnippetStart);
   virtual bool setClassAddress(bool isClassAddress) { return _isClassAddress = isClassAddress;}

   void addMetaDataForCodeAddress(uint8_t *cursor);

   int32_t getDataAs2Bytes() { return *((int16_t *) &_value); }
   int32_t getDataAs4Bytes() { return *((int32_t *) &_value); }
   int64_t getDataAs8Bytes() { return *((int64_t *) &_value); }
   };

}

#endif
