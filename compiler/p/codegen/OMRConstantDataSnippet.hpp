/*******************************************************************************
 * Copyright (c) 2000, 2020 IBM Corp. and others
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

#ifndef OMR_PPCCONSTANTDATASNIPPET_INCL
#define OMR_PPCCONSTANTDATASNIPPET_INCL

/*
 * The following #define and typedef must appear before any #includes in this file
 */
#ifndef OMR_PPCCONSTANTDATASNIPPET_CONNECTOR
#define OMR_PPCCONSTANTDATASNIPPET_CONNECTOR
namespace OMR { class ConstantDataSnippet; }
namespace OMR { typedef OMR::ConstantDataSnippet ConstantDataSnippetConnector; }
#endif

#include <stddef.h>
#include <stdint.h>
#include "codegen/CodeGenerator.hpp"
#include "codegen/Instruction.hpp"
#include "codegen/Relocation.hpp"
#include "env/TRMemory.hpp"
#include "env/jittypes.h"
#include "il/DataTypes.hpp"
#include "infra/Array.hpp"
#include "infra/List.hpp"

namespace TR { class Node; }

namespace OMR
{

template <class T> class PPCConstant
   {
   TR_Array<TR::Instruction *>   _instructionPairs;
   int32_t                         _tocOffset;
   T                               _value;
   TR::Node                        *_node;
   bool                            _isUnloadablePicSite;

   public:

   TR_ALLOC(TR_Memory::PPCConstant)

   PPCConstant(TR::CodeGenerator * cg, T v, TR::Node *n=NULL, bool ps=false) : _instructionPairs(cg->trMemory()), _value(v), _node(n), _isUnloadablePicSite(ps) {};

   T getConstantValue() {return _value;}
   bool isUnloadablePicSite() {return _isUnloadablePicSite;}

   TR_Array<TR::Instruction *> &getRequestors() {return _instructionPairs;}

   void addValueRequest(TR::Instruction *n0, TR::Instruction *n1, TR::Instruction *n2, TR::Instruction *n3)
      {
      if (n0 != NULL)
         {
         _instructionPairs.add(n0);
         _instructionPairs.add(n1);
         n0->setWillBePatched();
         n1->setWillBePatched();
         }

      if (n2 != NULL)
         {
         _instructionPairs.add(n2);
         _instructionPairs.add(n3);
         n2->setWillBePatched();
         n3->setWillBePatched();
         }
      }

   void patchRequestors(TR::CodeGenerator *cg, intptr_t addr)
      {
      if (TR::Compiler->target.is64Bit())
         {
         TR_ASSERT_FATAL(_instructionPairs.size() % 4 == 0, "Expected groups of 4 requestors");

         intptr_t addrHi = cg->hiValue(addr);
         intptr_t addrLo = LO_VALUE(addr);

         for (int32_t i = 0; i < _instructionPairs.size(); i += 4)
            {
            TR::Instruction *instr1 = _instructionPairs[i];
            TR::Instruction *instr2 = _instructionPairs[i + 1];
            TR::Instruction *instr3 = _instructionPairs[i + 2];
            TR::Instruction *instr4 = _instructionPairs[i + 3];

            TR_ASSERT_FATAL_WITH_INSTRUCTION(instr2, instr2->getBinaryEncoding() == instr1->getBinaryEncoding() + 8, "Unexpected ConstantDataSnippet load sequence");
            TR_ASSERT_FATAL_WITH_INSTRUCTION(instr3, instr3->getBinaryEncoding() == instr1->getBinaryEncoding() + 4, "Unexpected ConstantDataSnippet load sequence");
            TR_ASSERT_FATAL_WITH_INSTRUCTION(instr4, instr4->getBinaryEncoding() == instr1->getBinaryEncoding() + 16, "Unexpected ConstantDataSnippet load sequence");

            if (cg->canEmitDataForExternallyRelocatableInstructions())
               {
               *reinterpret_cast<uint32_t*>(instr1->getBinaryEncoding()) |= (addrHi >> 32) & 0xffff;
               *reinterpret_cast<uint32_t*>(instr2->getBinaryEncoding()) |= (addrHi >> 16) & 0xffff;
               *reinterpret_cast<uint32_t*>(instr3->getBinaryEncoding()) |= addrHi & 0xffff;
               *reinterpret_cast<uint32_t*>(instr4->getBinaryEncoding()) |= addrLo & 0xffff;
               }
            else
               {
               cg->addExternalRelocation(
                  new (cg->trHeapMemory()) TR::BeforeBinaryEncodingExternalRelocation(
                     instr1,
                     (uint8_t *)(addr),
                     (uint8_t *)fixedSequence4,
                     TR_FixedSequenceAddress2,
                     cg
                  ),
                  __FILE__,
                  __LINE__,
                  instr1->getNode()
               );
               }
            }
         }
      else
         {
         TR_ASSERT_FATAL(_instructionPairs.size() % 2 == 0, "Expected groups of 2 requestors");

         intptr_t addrHi = cg->hiValue(addr);
         intptr_t addrLo = LO_VALUE(addr);

         for (int32_t i = 0; i < _instructionPairs.size(); i += 2)
            {
            TR::Instruction *instr1 = _instructionPairs[i];
            TR::Instruction *instr2 = _instructionPairs[i + 1];

            *reinterpret_cast<uint32_t*>(instr1->getBinaryEncoding()) |= addrHi & 0xffff;
            *reinterpret_cast<uint32_t*>(instr2->getBinaryEncoding()) |= addrLo & 0xffff;

            TR_RelocationRecordInformation *recordInfo = (TR_RelocationRecordInformation *)cg->trMemory()->allocateMemory(sizeof(TR_RelocationRecordInformation), heapAlloc);
            recordInfo->data3 = orderedPairSequence1;
            cg->addExternalRelocation(
               new (cg->trHeapMemory()) TR::ExternalOrderedPair32BitRelocation(
                  instr1->getBinaryEncoding(),
                  instr2->getBinaryEncoding(),
                  (uint8_t *)recordInfo,
                  TR_AbsoluteMethodAddressOrderedPair,
                  cg
               ),
               __FILE__,
               __LINE__,
               instr1->getNode()
            );
            }
         }
      }

   int32_t getTOCOffset() {return _tocOffset;}
   void setTOCOffset(int32_t o) {_tocOffset = o;}
   TR::Node *getNode() { return _node; }
   };


class ConstantDataSnippet
   {
   List< PPCConstant<double> > _doubleConstants;
   List< PPCConstant<float> > _floatConstants;
   List< PPCConstant<intptr_t> > _addressConstants;
   uint8_t *_snippetBinaryStart;
   TR::CodeGenerator *_cg;

   public:

   TR_ALLOC(TR_Memory::PPCConstantDataSnippet)

   ConstantDataSnippet(TR::CodeGenerator *cg) : _cg(cg), _doubleConstants(cg->trMemory()),
        _floatConstants(cg->trMemory()), _addressConstants(cg->trMemory())
      {
      };

   uint8_t *getSnippetBinaryStart() {return _snippetBinaryStart;}
   uint8_t *setSnippetBinaryStart(uint8_t *p) {return _snippetBinaryStart=p;}

   int32_t addConstantRequest(void              *v,
                           TR::DataType       type,
                           TR::Instruction *nibble0,
                           TR::Instruction *nibble1,
                           TR::Instruction *nibble2,
                           TR::Instruction *nibble3,
                           TR::Node *node,
                           bool isUnloadablePicSite);

   virtual void emitAddressConstant(PPCConstant<intptr_t> *acursor, uint8_t *codeCursor);

   bool getRequestorsFromNibble(TR::Instruction *nibble, TR::Instruction **q, bool remove);

   virtual uint8_t *emitSnippetBody();
   virtual uint32_t getLength();

   TR::CodeGenerator *cg() {return _cg;}

#ifdef DEBUG
   virtual void print(TR::FILE *outFile);
#endif

   };

}

#endif
