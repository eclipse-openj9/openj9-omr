/*******************************************************************************
 * Copyright (c) 2000, 2018 IBM Corp. and others
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

#ifndef IA32DATASNIPPET_INCL
#define IA32DATASNIPPET_INCL

#include "codegen/Snippet.hpp"
#include "infra/vector.hpp"    // for TR::vector
#include <stdint.h>            // for uint8_t, int32_t, int64_t, etc

namespace TR { class CodeGenerator; }
namespace TR { class Node; }

namespace TR {

class IA32DataSnippet : public TR::Snippet
   {
   public:

   IA32DataSnippet(TR::CodeGenerator *cg, TR::Node *, void *c, uint8_t size);

   virtual Kind getKind() { return IsData; }
   uint8_t* getRawData()  { return _data.data(); }
   virtual uint8_t *emitSnippetBody();
   virtual uint8_t getDataSize() const { return _data.size(); }
   virtual void print(TR::FILE* pOutFile, TR_Debug* debug);
   virtual void printValue(TR::FILE* pOutFile, TR_Debug* debug);
   virtual uint32_t getLength(int32_t estimatedSnippetStart) { return _data.size(); }
   virtual bool setClassAddress(bool isClassAddress) { return _isClassAddress = isClassAddress;}

   void addMetaDataForCodeAddress(uint8_t *cursor);

   template <typename T> inline T getData() { return *((T*)getRawData()); }

   private:
   bool    _isClassAddress;
   TR::vector<uint8_t> _data;
   };

}

#endif
