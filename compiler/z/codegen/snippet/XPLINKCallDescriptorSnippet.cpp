/*******************************************************************************
 * Copyright (c) 2019, 2019 IBM Corp. and others
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

#include "codegen/CodeGenerator.hpp"
#include "codegen/Instruction.hpp"
#include "codegen/Linkage.hpp"
#include "codegen/Linkage_inlines.hpp"
#include "codegen/Relocation.hpp"
#include "codegen/SystemLinkagezOS.hpp"
#include "codegen/snippet/XPLINKCallDescriptorSnippet.hpp"
#include "env/CompilerEnv.hpp"
#include "il/DataTypes.hpp"
#include "il/Node.hpp"
#include "OMR/Bytes.hpp"

uint32_t TR::XPLINKCallDescriptorSnippet::generateCallDescriptorValue(TR::S390zOSSystemLinkage* linkage, TR::Node* callNode)
   {
   uint32_t result = 0;

   if (TR::Compiler->target.is32Bit())
      {
      uint32_t returnValueAdjust = 0;

      // 5 bit values for Return Value Adjust field of XPLLINK descriptor
      enum ReturnValueAdjust
         {
         XPLINK_RVA_RETURN_VOID_OR_UNUSED  = 0x00,
         XPLINK_RVA_RETURN_INT32_OR_LESS   = 0x01,
         XPLINK_RVA_RETURN_INT64           = 0x02,
         XPLINK_RVA_RETURN_FAR_POINTER     = 0x04,
         XPLINK_RVA_RETURN_FLOAT4          = 0x08,
         XPLINK_RVA_RETURN_FLOAT8          = 0x09,
         XPLINK_RVA_RETURN_FLOAT16         = 0x0A,
         XPLINK_RVA_RETURN_COMPLEX4        = 0x0C,
         XPLINK_RVA_RETURN_COMPLEX8        = 0x0D,
         XPLINK_RVA_RETURN_COMPLEX16       = 0x0E,
         XPLINK_RVA_RETURN_AGGREGATE       = 0x10,
         };

      TR::DataType dataType = callNode->getDataType();

      switch (dataType)
         {
         case TR::NoType:
            returnValueAdjust = XPLINK_RVA_RETURN_VOID_OR_UNUSED;
            break;

         case TR::Int8:
         case TR::Int16:
         case TR::Int32:
         case TR::Address:
            returnValueAdjust = XPLINK_RVA_RETURN_INT32_OR_LESS;
            break;

         case TR::Int64:
            returnValueAdjust = XPLINK_RVA_RETURN_INT64;
            break;

         case TR::Float:
            returnValueAdjust = XPLINK_RVA_RETURN_FLOAT4;
            break;

         case TR::Double:
            returnValueAdjust = XPLINK_RVA_RETURN_FLOAT8;
            break;

         default:
            TR_ASSERT_FATAL(false, "Unknown datatype (%s) for call node (%p)", dataType.toString(), callNode);
            break;
         }

      result |= returnValueAdjust << 24;

      //
      // Float parameter description fields
      // Bits 8-31 inclusive
      //

      uint32_t parmAreaOffset = 0;

#ifdef J9_PROJECT_SPECIFIC
      TR::MethodSymbol* callSymbol = callNode->getSymbol()->castToMethodSymbol();
      if (callSymbol->isJNI() && callNode->isPreparedForDirectJNI())
         {
         TR::ResolvedMethodSymbol * cs = callSymbol->castToResolvedMethodSymbol();
         TR_ResolvedMethod * resolvedMethod = cs->getResolvedMethod();
         // JNI Calls include a JNIEnv* pointer that is not included in list of children nodes.
         // For FastJNI, certain calls do not require us to pass the JNIEnv.
         if (!linkage->cg()->fej9()->jniDoNotPassThread(resolvedMethod))
            parmAreaOffset += sizeof(uintptrj_t);

         // For FastJNI, certain calls do not have to pass in receiver object.
         if (linkage->cg()->fej9()->jniDoNotPassReceiver(resolvedMethod))
            parmAreaOffset -= sizeof(uintptrj_t);
         }
#endif

      uint32_t parmDescriptorFields = 0;


      // WCode only logic follows for float parameter description fields

      TR::Symbol *funcSymbol = callNode->getSymbolReference()->getSymbol();

      uint32_t firstArgumentChild = callNode->getFirstArgumentIndex();
      int32_t to = callNode->getNumChildren() - 1;
      int32_t parmCount = 1;

      int32_t floatParmNum = 0;
      uint32_t gprSize = linkage->cg()->machine()->getGPRSize();

      uint32_t lastFloatParmAreaOffset = 0;

      bool done = false;
      for (int32_t i = firstArgumentChild; (i <= to) && !done; i++, parmCount++)
         {
         TR::Node *child = callNode->getChild(i);
         TR::DataType dataType = child->getDataType();
         TR::SymbolReference *parmSymRef = child->getOpCode().hasSymbolReference() ? child->getSymbolReference() : NULL;
         int32_t argSize = 0;

         if (parmSymRef == NULL)
            argSize = child->getSize();
         else
            argSize = parmSymRef->getSymbol()->getSize();


         // Note: complex type is attempted to be handled although other code needs
         // to change in 390 codegen to support complex
         //
         // PERFORMANCE TODO: it is desirable to use the defined "parameter count" of
         // the function symbol to help determine if we have an unprototyped argument
         // of a call (site) to a vararg function.  Currently we overcompensate for
         // outgoing float parms to vararg functions and always shadow in FPR and
         // and stack/gprs as with an unprotoyped call - see pushArg(). Precise
         // information can help remove such compensation. Changes to fix this would
         // involve: this function, pushArg() and buildArgs().

         int32_t numFPRsNeeded = 0;
         switch (dataType)
            {
            case TR::Float:
            case TR::Double:
#ifdef J9_PROJECT_SPECIFIC
            case TR::DecimalFloat:
            case TR::DecimalDouble:
#endif
               numFPRsNeeded = 1;
               break;
#ifdef J9_PROJECT_SPECIFIC
            case TR::DecimalLongDouble:
               break;
#endif
            }

         if (numFPRsNeeded != 0)
            {
            uint32_t unitSize = argSize / numFPRsNeeded;
            uint32_t wordsToPreviousParm = (parmAreaOffset - lastFloatParmAreaOffset) / gprSize;
            if (wordsToPreviousParm > 0xF)
               { // to big for descriptor. Will pass in stack
               done = true; // done
               }
            uint32_t val = wordsToPreviousParm + ((unitSize == 4) ? 0x10 : 0x20);

            parmDescriptorFields |= val << (6 * (3 - floatParmNum));

            floatParmNum++;

            if (floatParmNum >= linkage->getNumFloatArgumentRegisters())
               {
               done = true;
               }
            }
         parmAreaOffset += argSize < gprSize ? gprSize : argSize;

         if (numFPRsNeeded != 0)
            {
            lastFloatParmAreaOffset = parmAreaOffset;
            }
         }

      result |= parmDescriptorFields;
      }

   return result;
   }

TR::XPLINKCallDescriptorSnippet::XPLINKCallDescriptorSnippet(TR::CodeGenerator* cg, TR::S390zOSSystemLinkage* linkage, uint32_t callDescriptorValue)
   :
   TR::S390ConstantDataSnippet(cg, NULL, NULL, SIZE),
   _linkage(linkage),
   _callDescriptorValue(callDescriptorValue)
   {
   *(reinterpret_cast<uint32_t*>(_value) + 0) = 0;
   *(reinterpret_cast<uint32_t*>(_value) + 1) = callDescriptorValue;
   }

uint8_t*
TR::XPLINKCallDescriptorSnippet::emitSnippetBody()
   {
   uint8_t* cursor = cg()->getBinaryBufferCursor();

   // TODO: We should not have to do this here. This should be done by the caller.
   getSnippetLabel()->setCodeLocation(cursor);

   TR_ASSERT_FATAL((reinterpret_cast<uintptr_t>(cursor) % 8) == 0, "XPLINKCallDescriptorSnippet is not aligned on a doubleword bounary");

   // Signed offset, in bytes, to Entry Point Marker (if it exists)
   if (_linkage->getEntryPointMarkerLabel() != NULL)
      {
      *reinterpret_cast<int32_t*>(cursor) = _linkage->getEntryPointMarkerLabel()->getInstruction()->getBinaryEncoding() - cursor;
      *reinterpret_cast<int32_t*>(_value) = *reinterpret_cast<int32_t*>(cursor);
      }
   else
      {
      *reinterpret_cast<int32_t*>(cursor) = 0x00000000;
      }

   cursor += sizeof(int32_t);

   // Linkage, Return Value Adjust, and Parameter Adjust
   *reinterpret_cast<int32_t*>(cursor) = _callDescriptorValue;
   cursor += sizeof(int32_t);

   return cursor;
   }
