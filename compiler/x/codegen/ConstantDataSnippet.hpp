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

#ifndef IA32CONSTANTDATASNIPPET_INCL
#define IA32CONSTANTDATASNIPPET_INCL

#include "x/codegen/DataSnippet.hpp"

#include <stdint.h>                   // for uint8_t
#include "codegen/Snippet.hpp"        // for TR::X86Snippet::Kind, etc

namespace TR { class CodeGenerator; }
namespace TR { class Node; }

namespace TR {

class IA32ConstantDataSnippet : public TR::IA32DataSnippet
   {
   public:

   IA32ConstantDataSnippet(TR::CodeGenerator *cg, TR::Node *, void *c, uint8_t size);

   virtual Kind getKind() { return IsConstantData; }
   uint8_t getConstantSize()  { return getDataSize(); }
   };

}

#endif
