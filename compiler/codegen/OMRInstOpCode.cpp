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

#include "compiler/codegen/OMRInstOpCode.hpp"

uint8_t
OMR::InstOpCode::length(Mnemonic m)
   {
   return 0;
   }

uint8_t *
OMR::InstOpCode::copyBinaryToBuffer(
      Mnemonic m,
      uint8_t *cursor)
   {
   return cursor;
   }

const char *
OMR::InstOpCode::getOpCodeName(Mnemonic m)
   {
   if (IS_OMR_MNEMONIC(m))
      {
      return "OMROpCode";
      }
   else
      {
      return "Unknown";
      }
   }

const char *
OMR::InstOpCode::getMnemonicName(Mnemonic m)
   {
   if (IS_OMR_MNEMONIC(m))
      {
      return "OMROpCode";
      }
   else
      {
      return "Unknown";
      }
   }
