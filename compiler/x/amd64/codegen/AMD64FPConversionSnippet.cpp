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

#include <stdint.h>                              // for uint8_t, uint32_t, etc
#include <string.h>                              // for NULL, memcpy
#include "codegen/CodeGenerator.hpp"             // for CodeGenerator
#include "codegen/Linkage.hpp"                   // for Linkage
#include "codegen/Machine.hpp"                   // for Machine
#include "codegen/RealRegister.hpp"              // for RealRegister, etc
#include "codegen/RegisterConstants.hpp"         // for TR_RegisterSizes, etc
#include "codegen/Snippet.hpp"                   // for commentString
#include "compile/Compilation.hpp"               // for Compilation
#include "il/ILOpCodes.hpp"                      // for ILOpCodes
#include "il/Node.hpp"                           // for Node
#include "il/symbol/LabelSymbol.hpp"             // for LabelSymbol
#include "infra/Assert.hpp"                      // for TR_ASSERT
#include "ras/Debug.hpp"                         // for TR_Debug
#include "x/codegen/X86FPConversionSnippet.hpp"
#include "codegen/X86Instruction.hpp"
#include "env/IO.hpp"
#include "env/CompilerEnv.hpp"

static const uint8_t pushBinary[] =
   {
   // SUB rsp, 8
   0x48, 0x83, 0xec, 0x08,
   // MOVSD [rsp], xmm0
   0xf2, 0x0f, 0x11, 0x04, 0x24,
   };

static const uint8_t popBinary[] =
   {
   // MOVSD xmm0, [rsp]
   0xf2, 0x0f, 0x10, 0x04, 0x24,
   // ADD rsp, 8
   0x48, 0x83, 0xc4, 0x08
   };

uint8_t *TR::AMD64FPConversionSnippet::genFPConversion(uint8_t *buffer)
   {
   // This didn't end up as clean as I thought.  TODO:AMD64: Separate out the 64-bit code into another class.

   TR::ILOpCodes              opCode          = _convertInstruction->getNode()->getOpCodeValue();
   TR::RealRegister          *targetRegister  = toRealRegister(_convertInstruction->getTargetRegister());
   TR::RealRegister::RegNum   targetReg       = targetRegister->getRegisterNumber();
   TR::Machine               *machine         = cg()->machine();
   const TR::X86LinkageProperties &properties = cg()->getProperties();

   uint8_t *originalBuffer = buffer;

   TR_ASSERT(properties.getIntegerReturnRegister() == TR::RealRegister::eax, "Only support integer return in eax");

   if (targetReg != TR::RealRegister::eax)
      {
      // MOV R, rax
      //
      *buffer++ = TR::RealRegister::REX | TR::RealRegister::REX_W | targetRegister->rexBits(TR::RealRegister::REX_R, false);
      *buffer++ = 0x8b;
      *buffer = 0xc0;
      targetRegister->setRegisterFieldInModRM(buffer);
      buffer++;
      }

   TR_ASSERT(properties.getFloatArgumentRegister(0) == TR::RealRegister::xmm0, "Only support 1st FP arg in xmm0");
   TR::X86RegRegInstruction *instr = _convertInstruction->getIA32RegRegInstruction();
   TR_ASSERT(instr != NULL, "conversion instruction must be CVTTSS2SIRegReg\n");

   TR::RealRegister         *sourceRegister = toRealRegister(instr->getSourceRegister());
   TR::RealRegister::RegNum  sourceReg      = sourceRegister->getRegisterNumber();

   // Save xmm0 if necessary
   //
   if (sourceReg != TR::RealRegister::xmm0)
      {
      TR_ASSERT(TR::Compiler->target.is64Bit(), "This push sequence only works on AMD64");
      memcpy(buffer, pushBinary, sizeof(pushBinary));
      buffer += sizeof(pushBinary);

      // MOVSD xmm0, source
      //
      *buffer++ = 0xf2;
      if( (*buffer = sourceRegister->rexBits(TR::RealRegister::REX_B, false)) )
         buffer++;
      *buffer++ = 0x0f;
      *buffer++ = 0x10;
      *buffer   = 0xc0;
      sourceRegister->setRegisterFieldInOpcode(buffer);
      buffer++;
      }

   // Call the helper
   //
   buffer = emitCallToConversionHelper(buffer);

   // Restore xmm0 if necessary
   //
   if (sourceReg != TR::RealRegister::xmm0)
      {
      TR_ASSERT(TR::Compiler->target.is64Bit(), "This pop sequence only works on AMD64");
      memcpy(buffer, popBinary, sizeof(popBinary));
      buffer += sizeof(popBinary);
      }

   if (targetReg != TR::RealRegister::eax)
      {
      // XCHG R, rax
      //
      *buffer++ = TR::RealRegister::REX | TR::RealRegister::REX_W | targetRegister->rexBits(TR::RealRegister::REX_B, false);
      *buffer = 0x90;
      targetRegister->setRegisterFieldInOpcode(buffer);
      buffer++;
      }

   return buffer;
   }

// Cheat: should compute if Rex is required, but instead we just check if it's there
#define IS_REX(byte) (((byte)&0xf0) == TR::RealRegister::REX )

void
TR_Debug::print(TR::FILE *pOutFile, TR::AMD64FPConversionSnippet * snippet)
   {

   if (pOutFile == NULL)
      return;

   uint8_t *bufferPos = snippet->getSnippetLabel()->getCodeLocation();

   printSnippetLabel(pOutFile, snippet->getSnippetLabel(), bufferPos, getName(snippet));

   TR::Machine *machine = _cg->machine();
   TR::RealRegister *sourceRegister = toRealRegister(snippet->getConvertInstruction()->getSourceRegister());
   TR::RealRegister *targetRegister = toRealRegister(snippet->getConvertInstruction()->getTargetRegister());
   uint8_t             sreg           = sourceRegister->getRegisterNumber();
   uint8_t             treg           = targetRegister->getRegisterNumber();
   TR::ILOpCodes        opCode         = snippet->getConvertInstruction()->getNode()->getOpCodeValue();
   TR_RegisterSizes    size           = TR_DoubleWordReg;

   if (treg != TR::RealRegister::eax)
      {
      int instrSize = IS_REX(*bufferPos)? 3 : 2;
      printPrefix(pOutFile, NULL, bufferPos, instrSize);
      trfprintf(pOutFile, "mov \t");
      print(pOutFile, targetRegister, size);
      trfprintf(pOutFile, ", ");
      print(pOutFile, machine->getX86RealRegister(TR::RealRegister::eax), size);
      trfprintf(pOutFile, "\t%s preserve helper return reg",
                    commentString());
      bufferPos += instrSize;
      }

   if (sreg != TR::RealRegister::xmm0)
      {
      printPrefix(pOutFile, NULL, bufferPos, 4);
      trfprintf(pOutFile, "sub \trsp, 8");
      printPrefix(pOutFile, NULL, bufferPos, 5);
      trfprintf(pOutFile, "movsd\t[rsp], xmm0\t%s save xmm0",
                    commentString());
      bufferPos += 9;
      int instrSize = IS_REX(*bufferPos)? 5 : 4;
      printPrefix(pOutFile, NULL, bufferPos, instrSize);
      trfprintf(pOutFile, "movsd\txmm0, ");
      print(pOutFile, sourceRegister, TR_QuadWordReg);
      trfprintf(pOutFile, "\t%s load parameter",
                    commentString());
      bufferPos += instrSize;
      }

   printPrefix(pOutFile, NULL, bufferPos, 5);
   trfprintf(pOutFile, "call\t%s", getName(snippet->getHelperSymRef()));
   bufferPos += 5;

   if (sreg != TR::RealRegister::xmm0)
      {
      printPrefix(pOutFile, NULL, bufferPos, 5);
      trfprintf(pOutFile, "movsd\txmm0, [rsp]\t%s restore xmm0",
                    commentString());
      printPrefix(pOutFile, NULL, bufferPos, 4);
      trfprintf(pOutFile, "add \trsp, 8");
      bufferPos += 9;
      }

   if (treg != TR::RealRegister::eax)
      {
      int instrSize = IS_REX(*bufferPos)? 2 : 1;
      printPrefix(pOutFile, NULL, bufferPos, instrSize);
      trfprintf(pOutFile, "xchg\t");
      print(pOutFile, targetRegister, size);
      trfprintf(pOutFile, ", ");
      print(pOutFile, machine->getX86RealRegister(TR::RealRegister::eax), size);
      trfprintf(pOutFile, "\t%s restore result reg & put result in target reg",
                    commentString());
      bufferPos += instrSize;
      }

   printRestartJump(pOutFile, snippet, bufferPos);
   }


uint32_t TR::AMD64FPConversionSnippet::getLength(int32_t estimatedSnippetStart)
   {
   uint32_t length = 11;
   TR::Machine *machine = cg()->machine();


   if (toRealRegister(_convertInstruction->getTargetRegister())->getRegisterNumber() != TR::RealRegister::eax)
      {
      // MOV   R, rax
      // XCHG  R, rax
      //
      // 3 instruction/modRM bytes + 2 REX prefixes
      //
      length += (3 + 2);
      }

   TR::X86RegRegInstruction *instr = _convertInstruction->getIA32RegRegInstruction();
   TR_ASSERT(instr != NULL, "f2i conversion instruction must be either L4RegMem or CVTTSS2SIRegReg\n");
   TR::RealRegister *sourceRegister = toRealRegister(instr->getSourceRegister());
   if (sourceRegister->getRegisterNumber() != TR::RealRegister::xmm0)
      {
      length +=
           sizeof(pushBinary) + sizeof(popBinary)             // push and pop
         + 4 + (sourceRegister->rexBits(TR::RealRegister::REX_B, false)? 1 : 0) // MOVSD xmm0, source
         ;
      }

   return length + estimateRestartJumpLength(estimatedSnippetStart + length);
   }
