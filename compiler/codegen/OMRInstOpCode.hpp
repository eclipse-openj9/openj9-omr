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

#ifndef OMR_INSTOPCODE_INCL
#define OMR_INSTOPCODE_INCL

/*
 * The following #define and typedef must appear before any #includes in this file
 */
#ifndef OMR_INSTOPCODE_CONNECTOR
#define OMR_INSTOPCODE_CONNECTOR
namespace OMR { class InstOpCode; }
namespace OMR { typedef OMR::InstOpCode InstOpCodeConnector; }
#endif

#include <stdint.h>          // for uint8_t, int32_t
#include "env/TRMemory.hpp"  // for TR_Memory, etc

namespace OMR
{

class InstOpCode
   {
   public:

   TR_ALLOC(TR_Memory::Instruction)

   enum Mnemonic
      {
      #include "codegen/InstOpCodeEnum.hpp"
      NumOpCodes
      };

   protected:

   InstOpCode(Mnemonic m) :
      _mnemonic(m) {}

   public:

   Mnemonic getMnemonic() { return _mnemonic; }
   void setMnemonic(Mnemonic op) { _mnemonic = op; }

   static int32_t getNumOpCodes() { return NumOpCodes; }

   /*
    * Length of the encoded binary representation of an opcode.
    */
   static uint8_t length(Mnemonic m);

   /*
    * Copies the encoded binary representation of a given mnemonic to the
    * provided buffer.  This will copy at most length() bytes.
    *
    * Returns the cursor position after the bytes have been copied.
    */
   static uint8_t *copyBinaryToBuffer(Mnemonic m, uint8_t *cursor);
   static const char *getOpCodeName(Mnemonic m);
   static const char *getMnemonicName(Mnemonic m);

   const char *getOpCodeName()    { return getOpCodeName(_mnemonic);  }
   const char *getMnemonicName()  { return getMnemonicName(_mnemonic);  }

   /*
    * 0-terminated, printable description of the given mnemonic
    */
   //static char *description(Mnemonic m);

   protected:

   /*
    * Pointer to the encoded binary representation of an instruction.
    */
   static uint8_t *binaryEncoding(Mnemonic m);

   Mnemonic _mnemonic;

   private:

   InstOpCode() {}
   };

}

#endif
