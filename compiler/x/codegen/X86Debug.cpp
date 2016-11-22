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

#include <stdint.h>                              // for int32_t, uint8_t, etc
#include <stdio.h>                               // for NULL, sprintf
#include <string.h>                              // for memset
#include "codegen/CodeGenerator.hpp"             // for CodeGenerator, etc
#include "codegen/GCRegisterMap.hpp"             // for GCRegisterMap
#include "codegen/Instruction.hpp"               // for Instruction, etc
#include "codegen/Linkage.hpp"
#include "codegen/Machine.hpp"                   // for MachineBase, etc
#include "codegen/MemoryReference.hpp"           // for MemoryReference
#include "codegen/RealRegister.hpp"              // for RealRegister, etc
#include "codegen/RegisterConstants.hpp"
#include "codegen/RegisterDependency.hpp"
#include "codegen/RegisterDependencyStruct.hpp"
#include "codegen/Snippet.hpp"                   // for commentString, etc
#include "compile/Compilation.hpp"               // for Compilation
#include "control/Options.hpp"
#include "control/Options_inlines.hpp"
#include "env/IO.hpp"                            // for IO
#include "env/jittypes.h"                        // for intptrj_t
#include "il/Block.hpp"                          // for Block
#include "il/DataTypes.hpp"                      // for DataTypes::Double, etc
#include "il/ILOpCodes.hpp"                      // for ILOpCodes::BBStart, etc
#include "il/Node.hpp"                           // for Node
#include "il/Node_inlines.hpp"                   // for Node::getDataType, etc
#include "il/Symbol.hpp"                         // for Symbol
#include "il/SymbolReference.hpp"                // for SymbolReference
#include "il/symbol/LabelSymbol.hpp"             // for LabelSymbol
#include "il/symbol/MethodSymbol.hpp"            // for MethodSymbol
#include "infra/Assert.hpp"                      // for TR_ASSERT
#include "infra/List.hpp"                        // for ListIterator, List
#include "ras/Debug.hpp"                         // for TR_Debug, etc
#include "x/codegen/DataSnippet.hpp"             // for TR::IA32DataSnippet
#include "x/codegen/OutlinedInstructions.hpp"
#include "x/codegen/X86Instruction.hpp"
#include "x/codegen/X86Ops.hpp"                  // for TR_X86OpCode, etc
#include "x/codegen/X86Register.hpp"
#include "env/CompilerEnv.hpp"

#ifdef J9_PROJECT_SPECIFIC
#include "x/codegen/CallSnippet.hpp"             // for TR::X86CallSnippet
#include "x/codegen/WriteBarrierSnippet.hpp"
#endif

namespace TR { class Register; }

void
TR_Debug::printx(TR::FILE *pOutFile, TR::Instruction  * instr)
   {
   //print all the X86 instructions here.
   if (pOutFile == NULL)
      return;
   // Do not print pseudo-instructions in post-binary encoding dumps.
   if (instr->getBinaryEncoding() && (instr->getOpCode().isPseudoOp() != 0))
      return;
   switch (instr->getKind())
      {
      case TR::Instruction::IsLabel:
         print(pOutFile, (TR::X86LabelInstruction  *)instr);
         break;
      case TR::Instruction::IsPadding:
         print(pOutFile, (TR::X86PaddingInstruction  *)instr);
         break;
      case TR::Instruction::IsAlignment:
         print(pOutFile, (TR::X86AlignmentInstruction  *)instr);
         break;
      case TR::Instruction::IsBoundaryAvoidance:
         print(pOutFile, (TR::X86BoundaryAvoidanceInstruction  *)instr);
         break;
      case TR::Instruction::IsPatchableCodeAlignment:
         print(pOutFile, (TR::X86PatchableCodeAlignmentInstruction  *)instr);
         break;
#ifdef J9_PROJECT_SPECIFIC
      case TR::Instruction::IsVirtualGuardNOP:
         print(pOutFile, (TR::X86VirtualGuardNOPInstruction  *)instr);
         break;
#endif
      case TR::Instruction::IsFence:
         print(pOutFile, (TR::X86FenceInstruction  *)instr);
         break;
      case TR::Instruction::IsRestoreVMThread:
         print(pOutFile, (TR::X86RestoreVMThreadInstruction  *)instr);
         break;
      case TR::Instruction::IsImm:
         print(pOutFile, (TR::X86ImmInstruction  *)instr);
         break;
      case TR::Instruction::IsImm64:
         print(pOutFile, (TR::AMD64Imm64Instruction *)instr);
         break;
      case TR::Instruction::IsImm64Sym:
         print(pOutFile, (TR::AMD64Imm64SymInstruction *)instr);
      case TR::Instruction::IsImmSnippet:
         print(pOutFile, (TR::X86ImmSnippetInstruction  *)instr);
         break;
      case TR::Instruction::IsImmSym:
         print(pOutFile, (TR::X86ImmSymInstruction  *)instr);
         break;
      case TR::Instruction::IsReg:
         print(pOutFile, (TR::X86RegInstruction  *)instr);
         break;
      case TR::Instruction::IsRegReg:
         print(pOutFile, (TR::X86RegRegInstruction  *)instr);
         break;
      case TR::Instruction::IsRegRegImm:
         print(pOutFile, (TR::X86RegRegImmInstruction  *)instr);
         break;
      case TR::Instruction::IsFPRegReg:
      case TR::Instruction::IsFPST0ST1RegReg:
      case TR::Instruction::IsFPST0STiRegReg:
      case TR::Instruction::IsFPSTiST0RegReg:
      case TR::Instruction::IsFPArithmeticRegReg:
      case TR::Instruction::IsFPCompareRegReg:
      case TR::Instruction::IsFPRemainderRegReg:
         print(pOutFile, (TR::X86FPRegRegInstruction  *)instr);
         break;
#ifdef TR_TARGET_64BIT
      case TR::Instruction::IsRegImm64:
      case TR::Instruction::IsRegImm64Sym:
         print(pOutFile, (TR::AMD64RegImm64Instruction *)instr);
         break;
#endif
      case TR::Instruction::IsRegImm:
      case TR::Instruction::IsRegImmSym:
         print(pOutFile, (TR::X86RegImmInstruction  *)instr);
         break;
      case TR::Instruction::IsRegMem:
         print(pOutFile, (TR::X86RegMemInstruction  *)instr);
         break;
      case TR::Instruction::IsRegMemImm:
         print(pOutFile, (TR::X86RegMemImmInstruction  *)instr);
         break;
      case TR::Instruction::IsFPRegMem:
         print(pOutFile, (TR::X86FPRegMemInstruction  *)instr);
         break;
      case TR::Instruction::IsFPReg:
         print(pOutFile, (TR::X86FPRegInstruction  *)instr);
         break;
      case TR::Instruction::IsMem:
      case TR::Instruction::IsMemTable:
      case TR::Instruction::IsCallMem:
         print(pOutFile, (TR::X86MemInstruction  *)instr);
         break;
      case TR::Instruction::IsMemImm:
      case TR::Instruction::IsMemImmSym:
      case TR::Instruction::IsMemImmSnippet:
         print(pOutFile, (TR::X86MemImmInstruction  *)instr);
         break;
      case TR::Instruction::IsMemReg:
         print(pOutFile, (TR::X86MemRegInstruction  *)instr);
         break;
      case TR::Instruction::IsMemRegImm:
         print(pOutFile, (TR::X86MemRegRegInstruction  *)instr);
         break;
      case TR::Instruction::IsMemRegReg:
         print(pOutFile, (TR::X86MemRegRegInstruction  *)instr);
         break;
      case TR::Instruction::IsFPMemReg:
         print(pOutFile, (TR::X86FPMemRegInstruction  *)instr);
         break;
      case TR::Instruction::IsVFPSave:
         print(pOutFile, (TR::X86VFPSaveInstruction  *)instr);
         break;
      case TR::Instruction::IsVFPRestore:
         print(pOutFile, (TR::X86VFPRestoreInstruction  *)instr);
         break;
      case TR::Instruction::IsVFPDedicate:
         print(pOutFile, (TR::X86VFPDedicateInstruction  *)instr);
         break;
      case TR::Instruction::IsVFPRelease:
         print(pOutFile, (TR::X86VFPReleaseInstruction  *)instr);
         break;
      case TR::Instruction::IsVFPCallCleanup:
         print(pOutFile, (TR::X86VFPCallCleanupInstruction  *)instr);
         break;
      default:
         TR_ASSERT( 0, "Unknown instruction kind");
         // fall thru
      case TR::Instruction::IsFPCompareEval:
      case TR::Instruction::IsNotExtended:
         {
         printPrefix(pOutFile, instr);
         trfprintf(pOutFile, "%-32s", getMnemonicName(&instr->getOpCode()));
         printInstructionComment(pOutFile, 0, instr);
         dumpDependencies(pOutFile, instr);
         trfflush(pOutFile);
         }
         break;
      }
   }



void
TR_Debug::printPrefix(TR::FILE *pOutFile, TR::Instruction  *instr)
   {
   if (pOutFile == NULL)
      return;

   printPrefix(pOutFile, instr, instr->getBinaryEncoding(), instr->getBinaryLength());
   }

void
TR_Debug::printDependencyConditions(
   TR_X86RegisterDependencyGroup *conditions,
   uint8_t                         numConditions,
   char                           *prefix,
   TR::FILE *pOutFile)
   {
   char buf[32];
   char *cursor;
   int len,i;

   if (pOutFile == NULL)
      return;

   for (i=0; i<numConditions; i++)
      {
      cursor = buf;
      memset(buf, ' ', 23);
      len = sprintf(cursor, "    %s[%d]", prefix, i);
      *(cursor+len) = ' ';
      cursor += 12;

      *(cursor++) = '(';
      TR::RealRegister::RegNum r = (conditions->getRegisterDependency(i))->getRealRegister();
      if  (r == TR::RealRegister::AllFPRegisters)
         {
         len = sprintf(cursor, "AllFP");
         }
      else if (r == TR::RealRegister::NoReg)
         {
         len = sprintf(cursor, "NoReg");
         }
      else if (r == TR::RealRegister::ByteReg)
         {
         len = sprintf(cursor, "ByteReg");
         }
      else if (r == TR::RealRegister::BestFreeReg)
         {
         len = sprintf(cursor, "BestFreeReg");
         }
      else if (r == TR::RealRegister::SpilledReg)
         {
         len = sprintf(cursor, "SpilledReg");
         }
      else
         {
         len = sprintf(cursor, "%s", getName(_cg->machine()->getX86RealRegister(r)));
        }

      *(cursor+len)=')';
      *(cursor+9) = 0x00;

      trfprintf(pOutFile, "%s", buf);

      if (conditions->getRegisterDependency(i)->getRegister())
         {
         printFullRegInfo(pOutFile, conditions->getRegisterDependency(i)->getRegister());
         }
      else
         {
         trfprintf(pOutFile, "[ None        ]\n");
         }
      }
   }

void
TR_Debug::printFullRegisterDependencyInfo(TR::FILE *pOutFile, TR::RegisterDependencyConditions  * conditions)
   {
   if (pOutFile == NULL)
      return;

   if (conditions->getNumPreConditions() > 0)
      {
      printDependencyConditions(conditions->getPreConditions(), conditions->getNumPreConditions(), "Pre", pOutFile);
      }

   if (conditions->getNumPostConditions() > 0)
      {
      printDependencyConditions(conditions->getPostConditions(), conditions->getNumPostConditions(), "Post", pOutFile);
      }
   }

void
TR_Debug::dumpDependencyGroup(TR::FILE *                         pOutFile,
                              TR_X86RegisterDependencyGroup *group,
                              int32_t                         numConditions,
                              char                           *prefix,
                              bool                            omitNullDependencies)
   {
   TR::RealRegister::RegNum r;
   TR::Register *virtReg;
   int32_t      i;
   bool         foundDep = false;

   trfprintf(pOutFile, "\n\t%s:", prefix);
   for (i = 0; i < numConditions; ++i)
      {
      virtReg = group->getRegisterDependency(i)->getRegister();
      r = (group->getRegisterDependency(i))->getRealRegister();

      if (omitNullDependencies)
	 if (!virtReg && r != TR::RealRegister::AllFPRegisters)
	    continue;

      if  (r == TR::RealRegister::AllFPRegisters)
	 {
	 trfprintf(pOutFile, " [All FPRs]");
	 }
      else
	 {
	 trfprintf(pOutFile, " [%s : ", getName(virtReg));
	 if (r == TR::RealRegister::NoReg)
	    trfprintf(pOutFile, "NoReg]");
	 else if (r == TR::RealRegister::ByteReg)
	    trfprintf(pOutFile, "ByteReg]");
	 else if (r == TR::RealRegister::BestFreeReg)
	    trfprintf(pOutFile, "BestFreeReg]");
	 else if (r == TR::RealRegister::SpilledReg)
	    trfprintf(pOutFile, "SpilledReg]");
	 else
	    trfprintf(pOutFile, "%s]", getName(_cg->machine()->getX86RealRegister(r)));
	 }

      foundDep = true;
      }

   if (!foundDep)
      trfprintf(pOutFile, " None");
   }

void
TR_Debug::dumpDependencies(TR::FILE *pOutFile, TR::Instruction  * instr)
   {
   // *this    swipeable for debugging purposes

   // If we are in instruction selection or register assignment and
   // dependency information is requested, dump it.
   //
   if (pOutFile == NULL || // no dump file
       (_cg->getStackAtlas() // not in instruction selection
        && !(_registerAssignmentTraceFlags & TRACERA_IN_PROGRESS && // not in register assignment...
          _comp->getOptions()->getRegisterAssignmentTraceOption(TR_TraceRADependencies)) // or dependencies are not traced
       ))
      return;

   TR::RegisterDependencyConditions  *deps = instr->getDependencyConditions();
   if (!deps)
      return;  // Nothing to dump

   if (deps->getNumPreConditions() > 0)
      dumpDependencyGroup(pOutFile, deps->getPreConditions(), deps->getNumPreConditions(), " PRE", true);

   if (deps->getNumPostConditions() > 0)
      dumpDependencyGroup(pOutFile, deps->getPostConditions(), deps->getNumPostConditions(), "POST", true);

   trfflush(pOutFile);
   }

void
TR_Debug::printRegisterInfoHeader(TR::FILE *pOutFile, TR::Instruction  * instr)
   {
   if (pOutFile == NULL)
      return;

   trfprintf(pOutFile, "\n\n  FP stack height: %d", _cg->machine()->getFPTopOfStack() + 1);
   trfprintf(pOutFile,   "\n  Referenced Regs:        Register         State        Assigned      Total Future Flags\n");
   trfflush(pOutFile);
   }

void
TR_Debug::printReferencedRegisterInfo(TR::FILE *pOutFile, TR::Instruction  * instr)
   {
   if (pOutFile == NULL)
      return;

   if (instr->getDependencyConditions())
      {
      printRegisterInfoHeader(pOutFile, instr);
      printFullRegisterDependencyInfo(pOutFile, instr->getDependencyConditions());
      }
   }

void
TR_Debug::print(TR::FILE *pOutFile, TR::X86PaddingInstruction  * instr)
   {
   // *this    swipeable for debugging purposes
   if (pOutFile == NULL)
      return;

   if (instr->getOpCode().cannotBeAssembled())
      return;

   printPrefix(pOutFile, instr);
   if (instr->getBinaryEncoding())
      trfprintf(pOutFile, "nop (%d byte%s)\t\t%s Padding (%d byte%s)",
         instr->getBinaryLength(), (instr->getBinaryLength() == 1)?"":"s",
         commentString(),
         instr->getLength(),       (instr->getLength()       == 1)?"":"s"
         );
   else
      trfprintf(pOutFile, "nop\t\t\t%s Padding (%d byte%s)",
         commentString(),
         instr->getLength(),       (instr->getLength()       == 1)?"":"s"
         );

   dumpDependencies(pOutFile, instr);
   trfflush(pOutFile);
   }

void
TR_Debug::print(TR::FILE *pOutFile, TR::X86AlignmentInstruction  * instr)
   {
   // *this    swipeable for debugging purposes
   if (pOutFile == NULL)
      return;

   uint8_t length = instr->getBinaryLength();
   uint8_t margin = instr->getMargin();

   if (instr->getOpCode().cannotBeAssembled())
      return;

   printPrefix(pOutFile, instr);
   if (instr->getBinaryEncoding())
      trfprintf(pOutFile, "nop (%d byte%s)\t\t%s ",
                    instr->getBinaryLength(),
                    (instr->getBinaryLength() == 1)?"":"s",
                    commentString());
   else
      trfprintf(pOutFile, "nop\t\t\t%s ", commentString());

   if (margin)
      trfprintf(pOutFile, "Alignment (boundary=%d, margin=%d)", instr->getBoundary(), instr->getMargin());
   else
      trfprintf(pOutFile, "Alignment (boundary=%d)", instr->getBoundary());

   dumpDependencies(pOutFile, instr);
   trfflush(pOutFile);
   }

void
TR_Debug::print(TR::FILE *pOutFile, TR::X86RestoreVMThreadInstruction  * instr)
   {
   // *this    swipeable for debugging purposes
   if (pOutFile == NULL)
      return;

   if (instr->getOpCode().cannotBeAssembled())
      return;

   printPrefix(pOutFile, instr);
   if (instr->getBinaryEncoding())
      {
      trfprintf(pOutFile, "mov ebp, dword ptr fs:[0]\t\t;%sRestoreVMThread ",
                    commentString());
      }

   trfflush(pOutFile);
   }

void
TR_Debug::printBoundaryAvoidanceInfo(TR::FILE *pOutFile, TR::X86BoundaryAvoidanceInstruction  * instr)
   {
   trfprintf(pOutFile, " @%d", instr->getBoundarySpacing());
   if (instr->getMaxPadding() < instr->getBoundarySpacing()-1)
      trfprintf(pOutFile, " max %d", instr->getMaxPadding());
   if (0) // We rarely care about the target instruction
      {
      if (instr->getTargetCode())
         {
         trfprintf(pOutFile, " [%s]", getName(instr->getTargetCode()));
         }
      else
         {
         trfprintf(pOutFile, " (anything)");
         }
      if (instr->getTargetCode() != instr->getNext())
         {
         trfprintf(pOutFile, " (actually %s)", getName(instr->getNext()));
         }
      }

   trfprintf(pOutFile, " [");

   const char *sep = "";
   for (const TR_AtomicRegion *region = instr->getAtomicRegions(); region->getLength(); region++)
      {
      trfprintf(pOutFile, "%s0x%x:%d", sep, region->getStart(), region->getLength());
      sep = ",";
      }
   trfprintf(pOutFile, "]");
   }

void
TR_Debug::print(TR::FILE *pOutFile, TR::X86BoundaryAvoidanceInstruction  * instr)
   {
   // *this    swipeable for debugging purposes
   if (pOutFile == NULL)
      return;

   if (instr->getOpCode().cannotBeAssembled())
      return;

   printPrefix(pOutFile, instr);
   if (instr->getBinaryEncoding())
      trfprintf(pOutFile, "nop (%d byte%s)\t\t%s ",
                    instr->getBinaryLength(), (instr->getBinaryLength() == 1)?"":"s",
                    commentString());
   else
      trfprintf(pOutFile, "nop\t\t\t%s ", commentString());

   trfprintf(pOutFile, "Avoid boundary");
   printBoundaryAvoidanceInfo(pOutFile, instr);
   dumpDependencies(pOutFile, instr);
   trfflush(pOutFile);
   }

void
TR_Debug::print(TR::FILE *pOutFile, TR::X86PatchableCodeAlignmentInstruction  * instr)
   {
   // *this    swipeable for debugging purposes
   if (pOutFile == NULL)
      return;

   if (instr->getOpCode().cannotBeAssembled())
      return;

   printPrefix(pOutFile, instr);
   if (instr->getBinaryEncoding())
      trfprintf(pOutFile, "nop (%d byte%s)\t\t%s ",
                    instr->getBinaryLength(), (instr->getBinaryLength() == 1)?"":"s",
                    commentString());
   else
      trfprintf(pOutFile, "nop\t\t\t%s ", commentString());

   trfprintf(pOutFile, "Align patchable code");
   printBoundaryAvoidanceInfo(pOutFile, instr);
   dumpDependencies(pOutFile, instr);
   trfflush(pOutFile);
   }

void
TR_Debug::print(TR::FILE *pOutFile, TR::X86LabelInstruction  * instr)
   {
   // *this    swipeable for debugging purposes
   if (pOutFile == NULL)
      return;

   if (instr->getOpCode().cannotBeAssembled())
      return;

   printPrefix(pOutFile, instr);
   TR::LabelSymbol *label   = instr->getLabelSymbol();
   TR::Snippet *snippet = label ? label->getSnippet() : NULL;
   if (instr->getOpCodeValue() == LABEL)
      {
      print(pOutFile, label);

      trfprintf(pOutFile, ":");
      printInstructionComment(pOutFile, snippet ? 2 : 3, instr);

      if (label->isStartInternalControlFlow())
         trfprintf(pOutFile, "\t%s (Start of internal control flow)", commentString());
      else if (label->isEndInternalControlFlow())
         trfprintf(pOutFile, "\t%s (End of internal control flow)", commentString());
      }
   else
      {
      trfprintf(pOutFile, "%s\t", getMnemonicName(&instr->getOpCode()));

      if (label)
         {
         print(pOutFile, label);
         printInstructionComment(pOutFile, snippet ? 2 : 3, instr);
         }
      else
         {
         trfprintf(pOutFile, "Label L<null>");
         printInstructionComment(pOutFile, 2, instr);
         }

      if (snippet)
         trfprintf(pOutFile, "\t%s (%s)", commentString(), getName(snippet));
      }

   dumpDependencies(pOutFile, instr);
   trfflush(pOutFile);
   }

void
TR_Debug::print(TR::FILE *pOutFile, TR::X86FenceInstruction  * instr)
   {
   // *this    swipeable for debugging purposes
   if (pOutFile == NULL)
      return;

   // Omit fences from post-binary dumps unless they mark basic block boundaries.
   if (instr->getBinaryEncoding() &&
       instr->getNode()->getOpCodeValue() != TR::BBStart &&
       instr->getNode()->getOpCodeValue() != TR::BBEnd)
      return;

   if (instr->getOpCode().cannotBeAssembled())
      return;

   TR::Node *node = instr->getNode();
   if (node && node->getOpCodeValue() == TR::BBStart)
      {
      TR::Block *block = node->getBlock();
      if (block->isExtensionOfPreviousBlock())
         {
         trfprintf(pOutFile, "\n........................................");
         }
      else
         {
         trfprintf(pOutFile, "\n========================================");
         }
      }

   printPrefix(pOutFile, instr);
   trfprintf(pOutFile, "%s", getMnemonicName(&instr->getOpCode()));
   if (instr->getFenceNode()->getNumRelocations() > 0)
      {
      if (instr->getFenceNode()->getRelocationType() == TR_AbsoluteAddress)
         trfprintf(pOutFile, " Absolute [");
      else if (instr->getFenceNode()->getRelocationType() == TR_ExternalAbsoluteAddress)
         trfprintf(pOutFile, " External Absolute [");
      else
         trfprintf(pOutFile, " Relative [");

      if (!_comp->getOption(TR_MaskAddresses))
         {
         for (int32_t i = 0; i < instr->getFenceNode()->getNumRelocations(); ++i)
            trfprintf(pOutFile," " POINTER_PRINTF_FORMAT, instr->getFenceNode()->getRelocationDestination(i));
         }
      trfprintf(pOutFile, " ]");
      }

   printInstructionComment(pOutFile, (instr->getFenceNode()->getNumRelocations() > 0) ? 1 : 3, instr);

//   TR::Node *node = instr->getNode();
   printBlockInfo(pOutFile, node);
   dumpDependencies(pOutFile, instr);
   trfflush(pOutFile);
   }

#ifdef J9_PROJECT_SPECIFIC
void
TR_Debug::print(TR::FILE *pOutFile, TR::X86VirtualGuardNOPInstruction  * instr)
   {
   // *this    swipeable for degubbing purposes
   if (pOutFile == NULL)
      return;

   if (instr->getOpCode().cannotBeAssembled())
      return;

   printPrefix(pOutFile, instr);
   trfprintf(pOutFile, "%s Site:" POINTER_PRINTF_FORMAT ", ", getMnemonicName(&instr->getOpCode()), instr->getSite());
   print(pOutFile, instr->getLabelSymbol());
   printInstructionComment(pOutFile, 1, instr);
   dumpDependencies(pOutFile, instr);
   trfflush(pOutFile);
   }
#endif

void
TR_Debug::print(TR::FILE *pOutFile, TR::X86ImmInstruction  * instr)
   {
   // *this    swipeable for debugging purposes
   if (pOutFile == NULL)
      return;

   if (instr->getOpCode().cannotBeAssembled())
      return;

   printPrefix(pOutFile, instr);
   trfprintf(pOutFile, "%s\t", getMnemonicName(&instr->getOpCode()));

   if (instr->getOpCode().isCallImmOp() && instr->getNode()->getSymbolReference())
      {
      TR::SymbolReference *symRef  = instr->getNode()->getSymbolReference();
      const char         *symName = getName(symRef);

      trfprintf(pOutFile, "%-24s", symName);
      printInstructionComment(pOutFile, 0, instr);
      if (symRef->isUnresolved())
         trfprintf(pOutFile, " (unresolved method)");
      else
         trfprintf(pOutFile, " (" POINTER_PRINTF_FORMAT ")",  instr->getSourceImmediate()); // TODO:AMD64: Target address gets truncated
      }
   else
      {
      printIntConstant(pOutFile, instr->getSourceImmediate(), 16, getImmediateSizeFromInstruction(instr), true);
      printInstructionComment(pOutFile, 2, instr);
      }

   dumpDependencies(pOutFile, instr);
   trfflush(pOutFile);
   }

void
TR_Debug::print(TR::FILE *pOutFile, TR::AMD64Imm64Instruction * instr)
   {
   // *this    swipeable for debugging purposes
   if (pOutFile == NULL)
      return;

   if (instr->getOpCode().cannotBeAssembled())
      return;

   printPrefix(pOutFile, instr);
   trfprintf(pOutFile, "%s\t", getMnemonicName(&instr->getOpCode()));

   if (instr->getOpCode().isCallImmOp() && instr->getNode()->getSymbolReference())
      {
      TR::SymbolReference *symRef  = instr->getNode()->getSymbolReference();
      const char         *symName = getName(symRef);

      trfprintf(pOutFile, "%-24s", symName);
      printInstructionComment(pOutFile, 0, instr);
      if (symRef->isUnresolved())
         trfprintf(pOutFile, " (unresolved method)");
      else
         trfprintf(pOutFile, " (" POINTER_PRINTF_FORMAT ")",  instr->getSourceImmediate());
      }
   else
      {
      printIntConstant(pOutFile, instr->getSourceImmediate(), 16, getImmediateSizeFromInstruction(instr), true);
      printInstructionComment(pOutFile, 2, instr);
      }

   dumpDependencies(pOutFile, instr);
   trfflush(pOutFile);
   }

void
TR_Debug::print(TR::FILE *pOutFile, TR::AMD64Imm64SymInstruction * instr)
   {
   // *this    swipeable for debugging purposes
   if (pOutFile == NULL)
      return;

   if (instr->getOpCode().cannotBeAssembled())
      return;

   printPrefix(pOutFile, instr);
   TR::Symbol       *sym   = instr->getSymbolReference()->getSymbol();
   const char     *name  = getName(instr->getSymbolReference());

   trfprintf(pOutFile, "%s\t", getMnemonicName(&instr->getOpCode()));
   if (sym->getMethodSymbol() && name)
      {
      trfprintf(pOutFile, "%-24s%s %s (" POINTER_PRINTF_FORMAT ")",
                    name,
                    commentString(),
                    getOpCodeName(&instr->getOpCode()),
                    instr->getSourceImmediate());
      }
   else if (sym->getLabelSymbol() && name)
      {
      if (sym->getLabelSymbol()->getSnippet())
         trfprintf(pOutFile, "%-24s%s %s (%s)",
                       name,
                       commentString(),
                       getOpCodeName(&instr->getOpCode()),
                       getName(sym->getLabelSymbol()->getSnippet()));
      else
         trfprintf(pOutFile, "%-24s%s %s (" POINTER_PRINTF_FORMAT ")",
                       name,
                       commentString(),
                       getOpCodeName(&instr->getOpCode()),
                       instr->getSourceImmediate());
      }
   else
      {
      printIntConstant(pOutFile, instr->getSourceImmediate(), 16, getImmediateSizeFromInstruction(instr), true);
      printInstructionComment(pOutFile, 2, instr);
      }

   dumpDependencies(pOutFile, instr);
   trfflush(pOutFile);
   }

void
TR_Debug::print(TR::FILE *pOutFile, TR::AMD64RegImm64Instruction * instr)
   {
   // *this    swipeable for debugging purposes
   if (pOutFile == NULL)
      return;

   if (instr->getOpCode().cannotBeAssembled())
      return;

   printPrefix(pOutFile, instr);
   trfprintf(pOutFile, "%s\t", getMnemonicName(&instr->getOpCode()));
   if (!(instr->getOpCode().targetRegIsImplicit() != 0))
      {
      print(pOutFile, instr->getTargetRegister(), TR_DoubleWordReg);
      trfprintf(pOutFile, ", ");
      }
   printIntConstant(pOutFile, instr->getSourceImmediate(), 16, getImmediateSizeFromInstruction(instr), true);
   printInstructionComment(pOutFile, 1, instr);

   dumpDependencies(pOutFile, instr);
   trfflush(pOutFile);
   }

void TR_Debug::print(TR::FILE *pOutFile, TR::X86VFPSaveInstruction  *instr)
   {
   if (pOutFile == NULL)
      return;

   // vfpSave is a compile-time accounting pseudo instruction
   if (instr->getOpCode().cannotBeAssembled())
      return;

   printPrefix(pOutFile, instr);
   trfprintf(pOutFile, "vfpSave", getMnemonicName(&instr->getOpCode()));

   printInstructionComment(pOutFile, 3, instr);

   dumpDependencies(pOutFile, instr);
   trfflush(pOutFile);
   }

void TR_Debug::print(TR::FILE *pOutFile, TR::X86VFPRestoreInstruction  *instr)
   {
   if (pOutFile == NULL)
      return;

   // vfpRestore is a compile-time accounting pseudo instruction
   if (instr->getOpCode().cannotBeAssembled())
      return;

   printPrefix(pOutFile, instr);
   trfprintf(pOutFile, "vfpRestore [%s]", getName(instr->getSaveInstruction()));

   printInstructionComment(pOutFile, 3, instr);

   dumpDependencies(pOutFile, instr);
   trfflush(pOutFile);
   }

void TR_Debug::print(TR::FILE *pOutFile, TR::X86VFPDedicateInstruction  *instr)
   {
   if (pOutFile == NULL)
      return;

   print(pOutFile, (TR::X86RegMemInstruction*)instr);

   trfprintf(pOutFile, "%s vfpDedicate %s",
                 commentString(),
                 getName(instr->getTargetRegister()));
   trfflush(pOutFile);
   }

void TR_Debug::print(TR::FILE *pOutFile, TR::X86VFPReleaseInstruction  *instr)
   {
   if (pOutFile == NULL)
      return;

   if (instr->getOpCode().cannotBeAssembled())
      return;

   printPrefix(pOutFile, instr);
   trfprintf(pOutFile, "vfpRelease [%s]", getName(instr->getDedicateInstruction()));

   printInstructionComment(pOutFile, 3, instr);

   dumpDependencies(pOutFile, instr);
   trfflush(pOutFile);
   }

void TR_Debug::print(TR::FILE *pOutFile, TR::X86VFPCallCleanupInstruction  *instr)
   {
   if (pOutFile == NULL)
      return;

   // vfpCallCleanup is a compile-time accounting pseudo instruction
   if (instr->getOpCode().cannotBeAssembled())
      return;

   printPrefix(pOutFile, instr);
   trfprintf(pOutFile, "vfpCallCleanup (%d bytes)", instr->getStackPointerAdjustment());

   printInstructionComment(pOutFile, 3, instr);

   dumpDependencies(pOutFile, instr);
   trfflush(pOutFile);
   }


void
TR_Debug::print(TR::FILE *pOutFile, TR::X86ImmSnippetInstruction  * instr)
   {
   // *this    swipeable for debugging purposes
   if (pOutFile == NULL)
      return;

   if (instr->getOpCode().cannotBeAssembled())
      return;

   printPrefix(pOutFile, instr);
   trfprintf(pOutFile, "%s\t", getMnemonicName(&instr->getOpCode()));
   printIntConstant(pOutFile, instr->getSourceImmediate(), 16, getImmediateSizeFromInstruction(instr), true);
   printInstructionComment(pOutFile, 2, instr);
   dumpDependencies(pOutFile, instr);
   trfflush(pOutFile);
   }


void
TR_Debug::print(TR::FILE *pOutFile, TR::X86ImmSymInstruction  * instr)
   {
   // *this    swipeable for debugging purposes
   if (pOutFile == NULL)
      return;

   if (instr->getOpCode().cannotBeAssembled())
      return;

   TR::Symbol       *sym   = instr->getSymbolReference()->getSymbol();
   const char     *name  = getName(instr->getSymbolReference());

   printPrefix(pOutFile, instr);
   trfprintf(pOutFile, "%s\t", getMnemonicName(&instr->getOpCode()));
   intptr_t targetAddress = 0;

   //  64 bit always gets the targetAddress from the symRef
   if(TR::Compiler->target.is64Bit())
      // new code patching might have a call to a snippet label, which is not a method
      if(!sym->isLabel())
         targetAddress = (intptr_t)instr->getSymbolReference()->getMethodAddress();
   else
      targetAddress = instr->getSourceImmediate();

   if (name)
      {
      trfprintf(pOutFile, "%-24s", name);
      }
   else
      {
      trfprintf(pOutFile, POINTER_PRINTF_FORMAT, targetAddress);
      }

   if (sym->getMethodSymbol() && name)
      {
      trfprintf(pOutFile, "%s %s (" POINTER_PRINTF_FORMAT ")",
                    commentString(),
                    getOpCodeName(&instr->getOpCode()),
                    targetAddress);
      }
   else if (sym->getLabelSymbol() && name)
     {
     if (sym->getLabelSymbol()->getSnippet())
        trfprintf(pOutFile, "%s %s (%s)",
                      commentString(),
                      getOpCodeName(&instr->getOpCode()),
                      getName(sym->getLabelSymbol()->getSnippet()));
     else
        trfprintf(pOutFile, "%s %s (" POINTER_PRINTF_FORMAT ")",
                      commentString(),
                      getOpCodeName(&instr->getOpCode()),
                      targetAddress);
     }
   else
     {
     trfprintf(pOutFile, " \t\t%s %s",
                   commentString(),
                   getOpCodeName(&instr->getOpCode()));
     }

   printInstructionComment(pOutFile, 0, instr);
   dumpDependencies(pOutFile, instr);
   trfflush(pOutFile);
   }

void
TR_Debug::print(TR::FILE *pOutFile, TR::X86RegInstruction  * instr)
   {
   // *this    swipeable for debugging purposes
   if (pOutFile == NULL)
      return;

   if (instr->getOpCode().cannotBeAssembled())
      return;

   printPrefix(pOutFile, instr);
   trfprintf(pOutFile, "%s\t", getMnemonicName(&instr->getOpCode()));
   if (!(instr->getOpCode().targetRegIsImplicit() != 0))
      print(pOutFile, instr->getTargetRegister(), getTargetSizeFromInstruction(instr));
   printInstructionComment(pOutFile, 3, instr);
   dumpDependencies(pOutFile, instr);
   trfflush(pOutFile);
   }

void
TR_Debug::printReferencedRegisterInfo(TR::FILE *pOutFile, TR::X86RegInstruction  * instr)
   {
   if (pOutFile == NULL)
      return;

   printRegisterInfoHeader(pOutFile, instr);
   trfprintf(pOutFile,"    Target            ");
   printFullRegInfo(pOutFile, instr->getTargetRegister());

   if (instr->getDependencyConditions())
      {
      printFullRegisterDependencyInfo(pOutFile, instr->getDependencyConditions());
      }

   trfflush(pOutFile);
   }

void
TR_Debug::print(TR::FILE *pOutFile, TR::X86RegRegInstruction  * instr)
   {
   // *this    swipeable for debugging purposes
   if (pOutFile == NULL)
      return;

   if (instr->getOpCode().cannotBeAssembled())
      return;

   printPrefix(pOutFile, instr);
   trfprintf(pOutFile, "%s\t", getMnemonicName(&instr->getOpCode()));
   if (!(instr->getOpCode().targetRegIsImplicit() != 0))
      print(pOutFile, instr->getTargetRegister(), getTargetSizeFromInstruction(instr));
   if (!(instr->getOpCode().targetRegIsImplicit() != 0) && !(instr->getOpCode().sourceRegIsImplicit() !=0 ))
      trfprintf(pOutFile, ", ");
   if (!(instr->getOpCode().sourceRegIsImplicit() !=0 ))
      print(pOutFile, instr->getSourceRegister(), getSourceSizeFromInstruction(instr));
   printInstructionComment(pOutFile, 2, instr);
   dumpDependencies(pOutFile, instr);
   trfflush(pOutFile);
   }

void
TR_Debug::printReferencedRegisterInfo(TR::FILE *pOutFile, TR::X86RegRegInstruction  * instr)
   {
   if (pOutFile == NULL)
      return;

   printRegisterInfoHeader(pOutFile, instr);
   trfprintf(pOutFile,"    Target            ");
   printFullRegInfo(pOutFile, instr->getTargetRegister());
   trfprintf(pOutFile,"    Source            ");
   printFullRegInfo(pOutFile, instr->getSourceRegister());

   if (instr->getDependencyConditions())
      {
      printFullRegisterDependencyInfo(pOutFile, instr->getDependencyConditions());
      }

   trfflush(pOutFile);
   }

void
TR_Debug::print(TR::FILE *pOutFile, TR::X86RegImmInstruction  * instr)
   {
   // *this    swipeable for debugging purposes
   if (pOutFile == NULL)
      return;

   if (instr->getOpCode().cannotBeAssembled())
      return;

   printPrefix(pOutFile, instr);
   trfprintf(pOutFile, "%s\t", getMnemonicName(&instr->getOpCode()));
   if (!(instr->getOpCode().targetRegIsImplicit() != 0))
      {
      print(pOutFile, instr->getTargetRegister(), getTargetSizeFromInstruction(instr));
      trfprintf(pOutFile, ", ");
      }
   printIntConstant(pOutFile, instr->getSourceImmediate(), 16, getImmediateSizeFromInstruction(instr), true);
   printInstructionComment(pOutFile, 1, instr);
   dumpDependencies(pOutFile, instr);
   trfflush(pOutFile);
   }

void
TR_Debug::print(TR::FILE *pOutFile, TR::X86RegRegImmInstruction  * instr)
   {
   // *this    swipeable for debugging purposes
   if (pOutFile == NULL)
      return;

   if (instr->getOpCode().cannotBeAssembled())
      return;

   printPrefix(pOutFile, instr);
   trfprintf(pOutFile, "%s\t", getMnemonicName(&instr->getOpCode()));
   if (!(instr->getOpCode().targetRegIsImplicit() != 0))
      {
      print(pOutFile, instr->getTargetRegister(), getTargetSizeFromInstruction(instr));
      trfprintf(pOutFile, ", ");
      }
   if (!(instr->getOpCode().sourceRegIsImplicit() !=0 ))
      {
      print(pOutFile, instr->getSourceRegister(), getSourceSizeFromInstruction(instr));
      trfprintf(pOutFile, ", ");
      }
   printIntConstant(pOutFile, instr->getSourceImmediate(), 16, getImmediateSizeFromInstruction(instr), true);
   trfprintf(pOutFile, " \t%s %s",
                 commentString(),
                 getOpCodeName(&instr->getOpCode()));
   dumpDependencies(pOutFile, instr);
   trfflush(pOutFile);
   }

void
TR_Debug::print(TR::FILE *pOutFile, TR::X86RegRegRegInstruction  * instr)
   {
   // *this    swipeable for debugging purposes
   if (pOutFile == NULL)
      return;

   if (instr->getOpCode().cannotBeAssembled())
      return;

   printPrefix(pOutFile, instr);
   trfprintf(pOutFile, "%s\t", getMnemonicName(&instr->getOpCode()));
   if (!(instr->getOpCode().targetRegIsImplicit() != 0))
      {
      print(pOutFile, instr->getTargetRegister(), getTargetSizeFromInstruction(instr));
      trfprintf(pOutFile, ", ");
      }
   TR_RegisterSizes sourceSize = getSourceSizeFromInstruction(instr);
   if (!(instr->getOpCode().sourceRegIsImplicit() != 0))
      {
      print(pOutFile, instr->getSourceRegister(), sourceSize);
      trfprintf(pOutFile, ", ");
      }

   if (instr->getOpCodeValue() == SHLD4RegRegCL || instr->getOpCodeValue() <= SHRD4RegRegCL)
      trfprintf(pOutFile, "cl");
   else
      print(pOutFile, instr->getSourceRightRegister(), sourceSize);

   printInstructionComment(pOutFile, 2, instr);
   dumpDependencies(pOutFile, instr);
   trfflush(pOutFile);
   }

void
TR_Debug::printReferencedRegisterInfo(TR::FILE *pOutFile, TR::X86RegRegRegInstruction  * instr)
   {
   if (pOutFile == NULL)
      return;

   printRegisterInfoHeader(pOutFile, instr);
   trfprintf(pOutFile,"    SourceRight       ");
   printFullRegInfo(pOutFile, instr->getSourceRightRegister());
   trfprintf(pOutFile,"    Source            ");
   printFullRegInfo(pOutFile, instr->getSourceRegister());
   trfprintf(pOutFile,"    Target            ");
   printFullRegInfo(pOutFile, instr->getTargetRegister());

   if (instr->getDependencyConditions())
      {
      printFullRegisterDependencyInfo(pOutFile, instr->getDependencyConditions());
      }

   trfflush(pOutFile);
   }


int32_t
TR_Debug::printPrefixAndMnemonicWithoutBarrier(
   TR::FILE *pOutFile,
   TR::Instruction  *instr,
   int32_t             barrier)
   {
   int32_t barrierLength = ::estimateMemoryBarrierBinaryLength(barrier, _comp->cg());
   int32_t nonBarrierLength = instr->getBinaryLength()-barrierLength;

   printPrefix(pOutFile, instr, instr->getBinaryEncoding(), nonBarrierLength);
   trfprintf(pOutFile, "%s%s\t",  (barrier & LockPrefix) ? "lock " : "", getMnemonicName(&instr->getOpCode()));

   return nonBarrierLength;
   }


void
TR_Debug::printPrefixAndMemoryBarrier(
   TR::FILE *pOutFile,
   TR::Instruction  *instr,
   int32_t             barrier,
   int32_t             barrierOffset)
   {
   int32_t barrierLength = ::estimateMemoryBarrierBinaryLength(barrier, _comp->cg());
   uint8_t *barrierStart = instr->getBinaryEncoding() ? (instr->getBinaryEncoding()+barrierOffset) : NULL;

   printPrefix(pOutFile, instr, barrierStart, barrierLength);
   }


void
TR_Debug::print(TR::FILE *pOutFile, TR::X86MemInstruction  * instr)
   {
   // *this    swipeable for debugging purposes
   if (pOutFile == NULL)
      return;

   int32_t barrier = memoryBarrierRequired(instr->getOpCode(), instr->getMemoryReference(), _cg, false);
   int32_t barrierOffset = printPrefixAndMnemonicWithoutBarrier(pOutFile, instr, barrier);

   print(pOutFile, instr->getMemoryReference(), getTargetSizeFromInstruction(instr));
   printInstructionComment(pOutFile, 2, instr);
   printMemoryReferenceComment(pOutFile, instr->getMemoryReference());

   if (barrier & NeedsExplicitBarrier)
      printPrefixAndMemoryBarrier(pOutFile, instr, barrier, barrierOffset);

   dumpDependencies(pOutFile, instr);
   trfflush(pOutFile);
   }

void
TR_Debug::printReferencedRegisterInfo(TR::FILE *pOutFile, TR::X86MemInstruction  * instr)
   {
   if (pOutFile == NULL)
      return;

   printRegisterInfoHeader(pOutFile, instr);
   printReferencedRegisterInfo(pOutFile, instr->getMemoryReference());

   if (instr->getDependencyConditions())
      {
      printFullRegisterDependencyInfo(pOutFile, instr->getDependencyConditions());
      }

   trfflush(pOutFile);
   }

void
TR_Debug::print(TR::FILE *pOutFile, TR::X86MemImmInstruction  * instr)
   {
   // *this    swipeable for debugging purposes
   if (pOutFile == NULL)
      return;

   int32_t barrier = memoryBarrierRequired(instr->getOpCode(), instr->getMemoryReference(), _cg, false);
   int32_t barrierOffset = printPrefixAndMnemonicWithoutBarrier(pOutFile, instr, barrier);

   print(pOutFile, instr->getMemoryReference(), getTargetSizeFromInstruction(instr));
   trfprintf(pOutFile, ", ");
   printIntConstant(pOutFile, instr->getSourceImmediate(), 16, getImmediateSizeFromInstruction(instr), true);
   printInstructionComment(pOutFile, 1, instr);
   printMemoryReferenceComment(pOutFile, instr->getMemoryReference());

   if (barrier & NeedsExplicitBarrier)
      printPrefixAndMemoryBarrier(pOutFile, instr, barrier, barrierOffset);

   dumpDependencies(pOutFile, instr);
   trfflush(pOutFile);
   }

void
TR_Debug::print(TR::FILE *pOutFile, TR::X86MemRegInstruction  * instr)
   {
   // *this    swipeable for debugging purposes
   if (pOutFile == NULL)
      return;

   int32_t barrier = memoryBarrierRequired(instr->getOpCode(), instr->getMemoryReference(), _cg, false);
   int32_t barrierOffset = printPrefixAndMnemonicWithoutBarrier(pOutFile, instr, barrier);

   print(pOutFile, instr->getMemoryReference(), getTargetSizeFromInstruction(instr));
   if (!(instr->getOpCode().sourceRegIsImplicit() != 0))
      {
      trfprintf(pOutFile, ", ");
      print(pOutFile, instr->getSourceRegister(), getSourceSizeFromInstruction(instr));
      }
   printInstructionComment(pOutFile, 2, instr);
   printMemoryReferenceComment(pOutFile, instr->getMemoryReference());

   if (barrier & NeedsExplicitBarrier)
      printPrefixAndMemoryBarrier(pOutFile, instr, barrier, barrierOffset);

   dumpDependencies(pOutFile, instr);
   trfflush(pOutFile);
   }

void
TR_Debug::printReferencedRegisterInfo(TR::FILE *pOutFile, TR::X86MemRegInstruction  * instr)
   {
   if (pOutFile == NULL)
      return;

   printRegisterInfoHeader(pOutFile, instr);
   trfprintf(pOutFile,"    Source            ");
   printFullRegInfo(pOutFile, instr->getSourceRegister());
   printReferencedRegisterInfo(pOutFile, instr->getMemoryReference());

   if (instr->getDependencyConditions())
      {
      printFullRegisterDependencyInfo(pOutFile, instr->getDependencyConditions());
      }

   trfflush(pOutFile);
   }

void
TR_Debug::print(TR::FILE *pOutFile, TR::X86MemRegImmInstruction  * instr)
   {
   // *this    swipeable for debugging purposes
   if (pOutFile == NULL)
      return;

   int32_t barrier = memoryBarrierRequired(instr->getOpCode(), instr->getMemoryReference(), _cg, false);
   int32_t barrierOffset = printPrefixAndMnemonicWithoutBarrier(pOutFile, instr, barrier);

   print(pOutFile, instr->getMemoryReference(), getTargetSizeFromInstruction(instr));
   trfprintf(pOutFile, ", ");
   if (!(instr->getOpCode().sourceRegIsImplicit() !=0 ))
      {
      print(pOutFile, instr->getSourceRegister(), getSourceSizeFromInstruction(instr));
      trfprintf(pOutFile, ", ");
      }
   printIntConstant(pOutFile, instr->getSourceImmediate(), 16, getImmediateSizeFromInstruction(instr), true);
   printInstructionComment(pOutFile, 1, instr);
   printMemoryReferenceComment(pOutFile, instr->getMemoryReference());

   if (barrier & NeedsExplicitBarrier)
      printPrefixAndMemoryBarrier(pOutFile, instr, barrier, barrierOffset);

   dumpDependencies(pOutFile, instr);
   trfflush(pOutFile);
   }

void
TR_Debug::print(TR::FILE *pOutFile, TR::X86MemRegRegInstruction  * instr)
   {
   // *this    swipeable for debugging purposes
   if (pOutFile == NULL)
      return;

   int32_t barrier = memoryBarrierRequired(instr->getOpCode(), instr->getMemoryReference(), _cg, false);
   int32_t barrierOffset = printPrefixAndMnemonicWithoutBarrier(pOutFile, instr, barrier);

   print(pOutFile, instr->getMemoryReference(), getTargetSizeFromInstruction(instr));
   trfprintf(pOutFile, ", ");
   TR_RegisterSizes sourceSize = getSourceSizeFromInstruction(instr);
   if (!(instr->getOpCode().sourceRegIsImplicit() != 0))
      {
      print(pOutFile, instr->getSourceRegister(), sourceSize);
      trfprintf(pOutFile, ", ");
      }

   if (instr->getOpCodeValue() == SHLD4MemRegCL || instr->getOpCodeValue() == SHRD4MemRegCL)
      trfprintf(pOutFile, "cl");
   else
      print(pOutFile, instr->getSourceRightRegister(), sourceSize);

   printInstructionComment(pOutFile, 1, instr);
   printMemoryReferenceComment(pOutFile, instr->getMemoryReference());

   if (barrier & NeedsExplicitBarrier)
      printPrefixAndMemoryBarrier(pOutFile, instr, barrier, barrierOffset);

   dumpDependencies(pOutFile, instr);
   trfflush(pOutFile);
   }

void
TR_Debug::printReferencedRegisterInfo(TR::FILE *pOutFile, TR::X86MemRegRegInstruction  * instr)
   {
   if (pOutFile == NULL)
      return;

   printRegisterInfoHeader(pOutFile, instr);
   trfprintf(pOutFile,"    SourceRight       ");
   printFullRegInfo(pOutFile, instr->getSourceRightRegister());
   trfprintf(pOutFile,"    Source            ");
   printFullRegInfo(pOutFile, instr->getSourceRegister());

   printReferencedRegisterInfo(pOutFile, instr->getMemoryReference());

   if (instr->getDependencyConditions())
      {
      printFullRegisterDependencyInfo(pOutFile, instr->getDependencyConditions());
      }

   trfflush(pOutFile);
   }

void
TR_Debug::print(TR::FILE *pOutFile, TR::X86RegMemInstruction  * instr)
   {
   // *this    swipeable for debugging purposes
   if (pOutFile == NULL)
      return;

   int32_t barrier = memoryBarrierRequired(instr->getOpCode(), instr->getMemoryReference(), _cg, false);
   int32_t barrierOffset = printPrefixAndMnemonicWithoutBarrier(pOutFile, instr, barrier);

   if (!(instr->getOpCode().targetRegIsImplicit() != 0))
      {
      print(pOutFile, instr->getTargetRegister(), getTargetSizeFromInstruction(instr));
      trfprintf(pOutFile, ", ");
      }
   print(pOutFile, instr->getMemoryReference(), getSourceSizeFromInstruction(instr));
   printInstructionComment(pOutFile, 2, instr);
   printMemoryReferenceComment(pOutFile, instr->getMemoryReference());
   TR::Symbol *symbol = instr->getMemoryReference()->getSymbolReference().getSymbol();
   if (symbol && symbol->isSpillTempAuto())
      {
      trfprintf(pOutFile, "%s, spilled for %s",
                    commentString(),
                    getName(instr->getNode()->getOpCode()));
      }

   if (barrier & NeedsExplicitBarrier)
      printPrefixAndMemoryBarrier(pOutFile, instr, barrier, barrierOffset);

   dumpDependencies(pOutFile, instr);
   trfflush(pOutFile);
   }

void
TR_Debug::printReferencedRegisterInfo(TR::FILE *pOutFile, TR::X86RegMemInstruction  * instr)
   {
   if (pOutFile == NULL)
      return;

   printRegisterInfoHeader(pOutFile, instr);
   trfprintf(pOutFile,"    Target            ");
   printFullRegInfo(pOutFile, instr->getTargetRegister());

   printReferencedRegisterInfo(pOutFile, instr->getMemoryReference());

   if (instr->getDependencyConditions())
      {
      printFullRegisterDependencyInfo(pOutFile, instr->getDependencyConditions());
      }

   trfflush(pOutFile);
   }

void
TR_Debug::print(TR::FILE *pOutFile, TR::X86RegMemImmInstruction  * instr)
   {
   // *this    swipeable for debugging purposes
   if (pOutFile == NULL)
      return;

   int32_t barrier = memoryBarrierRequired(instr->getOpCode(), instr->getMemoryReference(), _cg, false);
   int32_t barrierOffset = printPrefixAndMnemonicWithoutBarrier(pOutFile, instr, barrier);

   if (!(instr->getOpCode().targetRegIsImplicit() != 0))
      {
      print(pOutFile, instr->getTargetRegister(), getTargetSizeFromInstruction(instr));
      trfprintf(pOutFile, ", ");
      }

   print(pOutFile, instr->getMemoryReference(), getSourceSizeFromInstruction(instr));
   trfprintf(pOutFile, ", ");
   printIntConstant(pOutFile, instr->getSourceImmediate(), 16, getImmediateSizeFromInstruction(instr), true);
   printInstructionComment(pOutFile, 1, instr);
   printMemoryReferenceComment(pOutFile, instr->getMemoryReference());

   if (barrier & NeedsExplicitBarrier)
      printPrefixAndMemoryBarrier(pOutFile, instr, barrier, barrierOffset);

   dumpDependencies(pOutFile, instr);
   trfflush(pOutFile);
   }

void
TR_Debug::print(TR::FILE *pOutFile, TR::X86FPRegInstruction  * instr)
   {
   // *this    swipeable for debugging purposes
   if (pOutFile == NULL)
      return;
   printPrefix(pOutFile, instr);
   trfprintf(pOutFile, "%s\t", getMnemonicName(&instr->getOpCode()));
   if (!(instr->getOpCode().targetRegIsImplicit() != 0))
      print(pOutFile, instr->getTargetRegister());
   printInstructionComment(pOutFile, 3, instr);
   printFPRegisterComment(pOutFile, instr->getTargetRegister(), NULL);
   dumpDependencies(pOutFile, instr);
   trfflush(pOutFile);
   }

void
TR_Debug::print(TR::FILE *pOutFile, TR::X86FPRegRegInstruction  * instr)
   {
   // *this    swipeable for debugging purposes

   if (pOutFile == NULL)
      return;
   printPrefix(pOutFile, instr);
   trfprintf(pOutFile, "%s\t", getMnemonicName(&instr->getOpCode()));
   if (!(instr->getOpCode().targetRegIsImplicit() != 0))
      print(pOutFile, instr->getTargetRegister());
   if (!(instr->getOpCode().targetRegIsImplicit() != 0) && !(instr->getOpCode().sourceRegIsImplicit()!=0))
      trfprintf(pOutFile, ", ");
   if (!(instr->getOpCode().sourceRegIsImplicit()!=0))
      print(pOutFile, instr->getSourceRegister());
   printInstructionComment(pOutFile, 2, instr);
   printFPRegisterComment(pOutFile, instr->getTargetRegister(), instr->getSourceRegister());
   dumpDependencies(pOutFile, instr);
   trfflush(pOutFile);
   }

void
TR_Debug::print(TR::FILE *pOutFile, TR::X86FPMemRegInstruction  * instr)
   {
   // *this    swipeable for debugging purposes
   if (pOutFile == NULL)
      return;
   printPrefix(pOutFile, instr);
   trfprintf(pOutFile, "%s\t", getMnemonicName(&instr->getOpCode()));
   print(pOutFile, instr->getMemoryReference(), getTargetSizeFromInstruction(instr));
   if (!(instr->getOpCode().sourceRegIsImplicit()!=0))
      {
      trfprintf(pOutFile, ", ");
      print(pOutFile, instr->getSourceRegister());
      }
   printInstructionComment(pOutFile, 1, instr);
   printFPRegisterComment(pOutFile, NULL, instr->getSourceRegister());
   printMemoryReferenceComment(pOutFile, instr->getMemoryReference());
   dumpDependencies(pOutFile, instr);
   trfflush(pOutFile);
   }

void
TR_Debug::print(TR::FILE *pOutFile, TR::X86FPRegMemInstruction  * instr)
   {
   // *this    swipeable for debugging purposes
   if (pOutFile == NULL)
      return;

   int32_t barrier = memoryBarrierRequired(instr->getOpCode(), instr->getMemoryReference(), _cg, false);
   int32_t barrierOffset = printPrefixAndMnemonicWithoutBarrier(pOutFile, instr, barrier);

   if (!(instr->getOpCode().targetRegIsImplicit() != 0))
      {
      print(pOutFile, instr->getTargetRegister());
      trfprintf(pOutFile, ", ");
      }
   print(pOutFile, instr->getMemoryReference(), getSourceSizeFromInstruction(instr));
   printInstructionComment(pOutFile, 1, instr);
   printFPRegisterComment(pOutFile, instr->getTargetRegister(), NULL);
   printMemoryReferenceComment(pOutFile, instr->getMemoryReference());

   if (barrier & NeedsExplicitBarrier)
      printPrefixAndMemoryBarrier(pOutFile, instr, barrier, barrierOffset);

   dumpDependencies(pOutFile, instr);
   trfflush(pOutFile);
   }

void
TR_Debug::print(TR::FILE *pOutFile, TR::MemoryReference  * mr, TR_RegisterSizes operandSize)
   {
   // *this    swipeable for debugging purposes
   if (pOutFile == NULL)
      return;

   const char *typeSpecifier[7] = {
      "byte",     // TR_ByteReg
      "word",     // TR_HalfWordReg
      "dword",    // TR_WordReg
      "qword",    // TR_DoubleWordReg
      "oword",    // TR_QuadWordReg
      "dword",    // TR_FloatReg
      "qword" };  // TR_DoubleReg

   TR_RegisterSizes addressSize = (TR::Compiler->target.cpu.isAMD64() ? TR_DoubleWordReg : TR_WordReg);
   bool hasTerm = false;
   bool hasPrecedingTerm = false;
   trfprintf(pOutFile, "%s ptr [", typeSpecifier[operandSize]);
   if (mr->getBaseRegister())
      {
      print(pOutFile, mr->getBaseRegister(), addressSize);
      hasPrecedingTerm = true;
      hasTerm = true;
      }

   if (mr->getIndexRegister())
      {
      if (hasPrecedingTerm)
         trfprintf(pOutFile, "+");
      else
         hasPrecedingTerm = true;
      trfprintf(pOutFile, "%d*", mr->getStrideMultiplier());
      print(pOutFile, mr->getIndexRegister(), addressSize);
      hasTerm = true;
      }

   TR::Symbol *sym = mr->getSymbolReference().getSymbol();

      if (sym != NULL || mr->getSymbolReference().getOffset() != 0)
      {
      intptrj_t disp32 = mr->getDisplacement();

      if (!hasPrecedingTerm)
         {
         // Treat this as an absolute reference and display in base16.
         //
         printIntConstant(pOutFile, disp32, 16, addressSize, true);
         }
      else
         {
         // Treat this as a relative offset and display in base10, or in base16 if the displacement was
         // explicitly forced wider.
         //
         if ((disp32 != 0) || mr->isForceWideDisplacement())
            {
            if (disp32 > 0)
               {
               trfprintf(pOutFile, "+");
               }
            else
               {
               trfprintf(pOutFile, "-");
               disp32 = -disp32;
               }
            }

         if (mr->isForceWideDisplacement())
            printIntConstant(pOutFile, disp32, 16, TR_WordReg);
         else if (disp32 != 0)
            printIntConstant(pOutFile, disp32, 16);
         }

      hasTerm = true;
      }

   if (!hasTerm)
      {
      // This must be an absolute memory reference (a constant data snippet or a label)
      //
      TR::IA32DataSnippet *cds = mr->getDataSnippet();
      TR::LabelSymbol *label = NULL;
      if (cds)
         label = cds->getSnippetLabel();
      else
         label = mr->getLabel();
      TR_ASSERT(label, "expecting a constant data snippet or a label");

      int64_t disp = (int64_t)(label->getCodeLocation());

      if (mr->getLabel())
         {
         print(pOutFile, label);
         if (disp)
            {
            trfprintf(pOutFile, " : ");
            printHexConstant(pOutFile, disp, TR::Compiler->target.is64Bit() ? 16 : 8, false);
            }
         }
      else if (disp)
         {
         printIntConstant(pOutFile, (int32_t)disp, 16, TR_WordReg, true);
         }
      else
         {
         trfprintf(pOutFile, "FPRCONSTANT");
         //         printFPConstant(pOutFile, void *value, int8_t numBits);
         }
      }

   trfprintf(pOutFile, "]");
   }

int32_t
TR_Debug::printIntConstant(TR::FILE *pOutFile, int64_t value, int8_t radix, TR_RegisterSizes size, bool padWithZeros)
   {
   if (pOutFile == NULL)
      return 0;

   const int8_t registerSizeToWidth[7] = {
      2,     // TR_ByteReg
      4,     // TR_HalfWordReg
      8,     // TR_WordReg
      16,    // TR_DoubleWordReg
      32,    // TR_QuadWordReg
      8,     // TR_FloatReg
      16 };  // TR_DoubleReg

   int8_t width = registerSizeToWidth[size];

   switch (radix)
      {
      case 10:
         return printDecimalConstant(pOutFile, value, width, padWithZeros);

      case 16:
         return printHexConstant(pOutFile, value, width, padWithZeros);

      default:
         TR_ASSERT(0, "Can't print in unimplemented radix %d\n", radix);
         break;
      }

   return 0;
   }

int32_t
TR_Debug::printDecimalConstant(TR::FILE *pOutFile, int64_t value, int8_t width, bool padWithZeros)
   {
   trfprintf(pOutFile, "%lld", value);
   return 0;
   }

int32_t
TR_Debug::printHexConstant(TR::FILE *pOutFile, int64_t value, int8_t width, bool padWithZeros)
   {
   // we probably need to revisit generateMasmListingSyntax
   const char *prefix = TR::Compiler->target.isLinux() ? "0x" : (_cg->generateMasmListingSyntax() ? "0" : "0x");
   const char *suffix = TR::Compiler->target.isLinux() ? "" : (_cg->generateMasmListingSyntax() ? "h" : "");

   if (padWithZeros)
      trfprintf(pOutFile, "%s%0*llx%s", prefix, width, value, suffix);
   else
      trfprintf(pOutFile, "%s%llx%s", prefix, value, suffix);

   return 0;
   }

void
TR_Debug::printFPRegisterComment(TR::FILE *pOutFile, TR::Register *source, TR::Register *target)
   {
   trfprintf(pOutFile, " using ");
   if (target)
      print(pOutFile, target);
   if (source && target)
      trfprintf(pOutFile, " & ");
   if (source)
      print(pOutFile, source);
   }

void
TR_Debug::printInstructionComment(TR::FILE *pOutFile, int32_t tabStops, TR::Instruction  *instr)
   {
   while (tabStops-- > 0)
      trfprintf(pOutFile, "\t");

   trfprintf(pOutFile, "%s %s",
                 commentString(),
                 getOpCodeName(&instr->getOpCode()));
   dumpInstructionComments(pOutFile, instr);
   }

void
TR_Debug::printMemoryReferenceComment(TR::FILE *pOutFile, TR::MemoryReference  *mr)
   {
   if (pOutFile == NULL)
      return;

   TR::Symbol *symbol = mr->getSymbolReference().getSymbol();

   if (symbol == NULL && mr->getSymbolReference().getOffset() == 0)
      return;

   if (symbol && symbol->isSpillTempAuto())
      {
      const char *prefix = (symbol->getDataType() == TR::Float ||
                            symbol->getDataType() == TR::Double) ? "#FP" : "#";
      trfprintf(pOutFile, ", %sSPILL%d", prefix, symbol->getSize());
      }

   trfprintf(pOutFile, ", SymRef");

   print(pOutFile, &mr->getSymbolReference());
   }

TR_RegisterSizes
TR_Debug::getTargetSizeFromInstruction(TR::Instruction  *instr)
   {
   TR_RegisterSizes targetSize;

   if (instr->getOpCode().hasXMMTarget() != 0)
      targetSize = TR_QuadWordReg;
   else if (instr->getOpCode().hasIntTarget() != 0)
      targetSize = TR_WordReg;
   else if (instr->getOpCode().hasShortTarget() != 0)
      targetSize = TR_HalfWordReg;
   else if (instr->getOpCode().hasByteTarget() != 0)
      targetSize = TR_ByteReg;
   else if ((instr->getOpCode().hasLongTarget() != 0) || (instr->getOpCode().doubleFPOp() != 0))
      targetSize = TR_DoubleWordReg;
   else
      targetSize = TR_WordReg;

   return targetSize;
   }

TR_RegisterSizes
TR_Debug::getSourceSizeFromInstruction(TR::Instruction  *instr)
   {
   TR_RegisterSizes sourceSize;

   if (instr->getOpCode().hasXMMSource() != 0)
      sourceSize = TR_QuadWordReg;
   else if (instr->getOpCode().hasIntSource()!= 0)
      sourceSize = TR_WordReg;
   else if (instr->getOpCode().hasShortSource() != 0)
      sourceSize = TR_HalfWordReg;
   else if ((&instr->getOpCode())->hasByteSource())
      sourceSize = TR_ByteReg;
   else if ((instr->getOpCode().hasLongSource() != 0) || (instr->getOpCode().doubleFPOp() != 0))
      sourceSize = TR_DoubleWordReg;
   else
      sourceSize = TR_WordReg;

   return sourceSize;
   }

TR_RegisterSizes
TR_Debug::getImmediateSizeFromInstruction(TR::Instruction  *instr)
   {
   TR_RegisterSizes immedSize;

   if (instr->getOpCode().hasShortImmediate() != 0)
      immedSize = TR_HalfWordReg;
   else if (instr->getOpCode().hasByteImmediate() != 0)
      immedSize = TR_ByteReg;
   else if (instr->getOpCode().hasLongImmediate() != 0)
      immedSize = TR_DoubleWordReg;
   else
      // IntImmediate or SignExtendImmediate
      //
      immedSize = TR_WordReg;

   return immedSize;
   }

int32_t
TR_Debug::printFPConstant(TR::FILE *pOutFile, void *value, int8_t numBits, bool printAsBytes)
   {
   return 0;
   }

void
TR_Debug::printReferencedRegisterInfo(TR::FILE *pOutFile, TR::MemoryReference  * mr)
   {
   if (pOutFile == NULL)
      return;

   if (mr->getBaseRegister())
      {
      trfprintf(pOutFile,"    Base Reg          ");
      printFullRegInfo(pOutFile, mr->getBaseRegister());
      }

   if (mr->getIndexRegister())
      {
      trfprintf(pOutFile,"    Index Reg         ");
      printFullRegInfo(pOutFile, mr->getIndexRegister());
      }

   trfflush(pOutFile);
   }

void
TR_Debug::printX86GCRegisterMap(TR::FILE *pOutFile, TR::GCRegisterMap * map)
   {
   TR::Machine *machine = _cg->machine();

   trfprintf(pOutFile,"    slot pushes: %d", ((map->getMap() & _cg->getRegisterMapInfoBitsMask()) >> 16));

   trfprintf(pOutFile,"    registers: {");
   for (int i = TR::RealRegister::FirstGPR; i <= TR::RealRegister::LastAssignableGPR; ++i)
      {
      if (map->getMap() & (1 << (i-1))) // TODO:AMD64: Use the proper mask value
         trfprintf(pOutFile,"%s ", getName(machine->getX86RealRegister((TR::RealRegister::RegNum)i)));
      }

   trfprintf(pOutFile,"}\n");
   }

void
TR_Debug::print(TR::FILE *pOutFile, TR::RealRegister * reg, TR_RegisterSizes size)
   {
   if (pOutFile == NULL)
      return;

   switch (size)
      {
      case TR_WordReg:
      case TR_FloatReg:
      case TR_DoubleReg:
         trfprintf(pOutFile, "%s", getName(reg));
         break;
      case TR_QuadWordReg:
      case TR_DoubleWordReg:
      case TR_HalfWordReg:
      case TR_ByteReg:
         trfprintf(pOutFile, "%s", getName(reg, size));
         break;
      default:
      	break;
      }
   }

void
TR_Debug::printFullRegInfo(TR::FILE *pOutFile, TR::RealRegister * reg)
   {
   if (pOutFile == NULL)
      return;

   trfprintf(pOutFile, "[ ");

   trfprintf(pOutFile, "%-12s ][ ", getName(reg));

   static const char * stateNames[5] = { "Free", "Unlatched", "Assigned", "Blocked", "Locked" };

   trfprintf(pOutFile, "%-10s ][ ", stateNames[reg->getState()]);

   trfprintf(pOutFile, "%-12s ]\n", reg->getAssignedRegister() ? getName(reg->getAssignedRegister()) : " ");

   trfflush(pOutFile);
   }

static const char *
unknownRegisterName(const char kind=0)
   {
   // This function allows us to put a breakpoint to trap unknown registers
   switch(kind)
      {
      case 'f':
         return "fp?";
      case 's':
         return "st(?)";
      case 'm':
         return "mm?";
      case 'r':
         return "r?";
      case 'x':
         return "xmm?";
      case 'v':
         return "vfp?";
      default:
         return "???";
      }
   }


const char *
TR_Debug::getName(uint32_t realRegisterIndex, TR_RegisterSizes size)
   {
   switch (realRegisterIndex)
      {
      case TR::RealRegister::NoReg:
         return "noReg";
      case TR::RealRegister::ByteReg:
         return "byteReg";
      case TR::RealRegister::BestFreeReg:
         return "bestFreeReg";
      case TR::RealRegister::SpilledReg:
         return "spilledReg";
      case TR::RealRegister::eax:
         switch (size) { case 0: return "al";   case 1:  return "ax";  case 2: case -1: return "eax"; case 3: return "rax"; default: return unknownRegisterName('r'); }
      case TR::RealRegister::ebx:
         switch (size) { case 0: return "bl";   case 1:  return "bx";  case 2: case -1: return "ebx"; case 3: return "rbx"; default: return unknownRegisterName('r'); }
      case TR::RealRegister::ecx:
         switch (size) { case 0: return "cl";   case 1:  return "cx";  case 2: case -1: return "ecx"; case 3: return "rcx"; default: return unknownRegisterName('r'); }
      case TR::RealRegister::edx:
         switch (size) { case 0: return "dl";   case 1:  return "dx";  case 2: case -1: return "edx"; case 3: return "rdx"; default: return unknownRegisterName('r'); }
      case TR::RealRegister::edi:
         switch (size) { case 0: return "dil";  case 1: return "di";   case 2: case -1: return "edi"; case 3: return "rdi"; default: return unknownRegisterName('r'); }
      case TR::RealRegister::esi:
         switch (size) { case 0: return "sil";  case 1: return "si";   case 2: case -1: return "esi"; case 3: return "rsi"; default: return unknownRegisterName('r'); }
      case TR::RealRegister::ebp:
         switch (size) { case 0: return "bpl";  case 1: return "bp";   case 2: case -1: return "ebp"; case 3: return "rbp"; default: return unknownRegisterName('r'); }
      case TR::RealRegister::esp:
         switch (size) { case 0: return "spl";  case 1: return "sp";   case 2: case -1: return "esp"; case 3: return "rsp"; default: return unknownRegisterName('r'); }
#ifdef TR_TARGET_64BIT
      case TR::RealRegister::r8:
         switch (size) { case 0: return "r8b";  case 1: return "r8w";  case 2: return "r8d"; case 3: case -1: return "r8"; default: return unknownRegisterName('r'); }
      case TR::RealRegister::r9:
         switch (size) { case 0: return "r9b";  case 1: return "r9w";  case 2: return "r9d"; case 3: case -1: return "r9"; default: return unknownRegisterName('r'); }
      case TR::RealRegister::r10:
         switch (size) { case 0: return "r10b"; case 1: return "r10w"; case 2: return "r10d"; case 3: case -1: return "r10"; default: return unknownRegisterName('r'); }
      case TR::RealRegister::r11:
         switch (size) { case 0: return "r11b"; case 1: return "r11w"; case 2: return "r11d"; case 3: case -1: return "r11"; default: return unknownRegisterName('r'); }
      case TR::RealRegister::r12:
         switch (size) { case 0: return "r12b"; case 1: return "r12w"; case 2: return "r12d"; case 3: case -1: return "r12"; default: return unknownRegisterName('r'); }
      case TR::RealRegister::r13:
         switch (size) { case 0: return "r13b"; case 1: return "r13w"; case 2: return "r13d"; case 3: case -1: return "r13"; default: return unknownRegisterName('r'); }
      case TR::RealRegister::r14:
         switch (size) { case 0: return "r14b"; case 1: return "r14w"; case 2: return "r14d"; case 3: case -1: return "r14"; default: return unknownRegisterName('r'); }
      case TR::RealRegister::r15:
         switch (size) { case 0: return "r15b"; case 1: return "r15w"; case 2: return "r15d"; case 3: case -1: return "r15"; default: return unknownRegisterName('r'); }
#endif
      case TR::RealRegister::vfp:
         switch (size) { case 2: case 3: case -1: return "vfp";   default: return unknownRegisterName('v'); } // 3 is for AMD64
      case TR::RealRegister::st0:
         switch (size) { case 2: case -1: return "st(0)";   default: return unknownRegisterName('s'); }
      case TR::RealRegister::st1:
         switch (size) { case 2: case -1: return "st(1)";   default: return unknownRegisterName('s'); }
      case TR::RealRegister::st2:
         switch (size) { case 2: case -1: return "st(2)";   default: return unknownRegisterName('s'); }
      case TR::RealRegister::st3:
         switch (size) { case 2: case -1: return "st(3)";   default: return unknownRegisterName('s'); }
      case TR::RealRegister::st4:
         switch (size) { case 2: case -1: return "st(4)";   default: return unknownRegisterName('s'); }
      case TR::RealRegister::st5:
         switch (size) { case 2: case -1: return "st(5)";   default: return unknownRegisterName('s'); }
      case TR::RealRegister::st6:
         switch (size) { case 2: case -1: return "st(6)";   default: return unknownRegisterName('s'); }
      case TR::RealRegister::st7:
         switch (size) { case 2: case -1: return "st(7)";   default: return unknownRegisterName('s'); }
      case TR::RealRegister::mm0:
         switch (size) { case 3: case -1: return "mm0";   default: return unknownRegisterName('m'); }
      case TR::RealRegister::mm1:
         switch (size) { case 3: case -1: return "mm1";   default: return unknownRegisterName('m'); }
      case TR::RealRegister::mm2:
         switch (size) { case 3: case -1: return "mm2";   default: return unknownRegisterName('m'); }
      case TR::RealRegister::mm3:
         switch (size) { case 3: case -1: return "mm3";   default: return unknownRegisterName('m'); }
      case TR::RealRegister::mm4:
         switch (size) { case 3: case -1: return "mm4";   default: return unknownRegisterName('m'); }
      case TR::RealRegister::mm5:
         switch (size) { case 3: case -1: return "mm5";   default: return unknownRegisterName('m'); }
      case TR::RealRegister::mm6:
         switch (size) { case 3: case -1: return "mm6";   default: return unknownRegisterName('m'); }
      case TR::RealRegister::mm7:
         switch (size) { case 3: case -1: return "mm7";   default: return unknownRegisterName('m'); }
      case TR::RealRegister::xmm0:
         switch (size) { case 4: case -1: return "xmm0";  default: return unknownRegisterName('x'); }
      case TR::RealRegister::xmm1:
         switch (size) { case 4: case -1: return "xmm1";  default: return unknownRegisterName('x'); }
      case TR::RealRegister::xmm2:
         switch (size) { case 4: case -1: return "xmm2";  default: return unknownRegisterName('x'); }
      case TR::RealRegister::xmm3:
         switch (size) { case 4: case -1: return "xmm3";  default: return unknownRegisterName('x'); }
      case TR::RealRegister::xmm4:
         switch (size) { case 4: case -1: return "xmm4";  default: return unknownRegisterName('x'); }
      case TR::RealRegister::xmm5:
         switch (size) { case 4: case -1: return "xmm5";  default: return unknownRegisterName('x'); }
      case TR::RealRegister::xmm6:
         switch (size) { case 4: case -1: return "xmm6";  default: return unknownRegisterName('x'); }
      case TR::RealRegister::xmm7:
         switch (size) { case 4: case -1: return "xmm7";  default: return unknownRegisterName('x'); }
#ifdef TR_TARGET_64BIT
      case TR::RealRegister::xmm8:
         switch (size) { case 4: case -1: return "xmm8";  default: return unknownRegisterName('x'); }
      case TR::RealRegister::xmm9:
         switch (size) { case 4: case -1: return "xmm9";  default: return unknownRegisterName('x'); }
      case TR::RealRegister::xmm10:
         switch (size) { case 4: case -1: return "xmm10"; default: return unknownRegisterName('x'); }
      case TR::RealRegister::xmm11:
         switch (size) { case 4: case -1: return "xmm11"; default: return unknownRegisterName('x'); }
      case TR::RealRegister::xmm12:
         switch (size) { case 4: case -1: return "xmm12"; default: return unknownRegisterName('x'); }
      case TR::RealRegister::xmm13:
         switch (size) { case 4: case -1: return "xmm13"; default: return unknownRegisterName('x'); }
      case TR::RealRegister::xmm14:
         switch (size) { case 4: case -1: return "xmm14"; default: return unknownRegisterName('x'); }
      case TR::RealRegister::xmm15:
         switch (size) { case 4: case -1: return "xmm15"; default: return unknownRegisterName('x'); }
#endif
      default: TR_ASSERT( 0, "unexpected register number"); return unknownRegisterName();
      }
   }

const char *
TR_Debug::getName(TR::RealRegister * reg, TR_RegisterSizes size)
   {
   if (reg->getKind() == TR_X87)
      {
      switch (reg->getRegisterNumber())
         {
         case TR::RealRegister::st0: return "st(0)";
         case TR::RealRegister::st1: return "st(1)";
         case TR::RealRegister::st2: return "st(2)";
         case TR::RealRegister::st3: return "st(3)";
         case TR::RealRegister::st4: return "st(4)";
         case TR::RealRegister::st5: return "st(5)";
         case TR::RealRegister::st6: return "st(6)";
         case TR::RealRegister::st7: return "st(7)";
         case TR::RealRegister::NoReg:
            switch (toX86FPStackRegister(reg)->getFPStackRegisterNumber())
               {
               case TR_X86FPStackRegister::fp0: return "fp0";
               case TR_X86FPStackRegister::fp1: return "fp1";
               case TR_X86FPStackRegister::fp2: return "fp2";
               case TR_X86FPStackRegister::fp3: return "fp3";
               case TR_X86FPStackRegister::fp4: return "fp4";
               case TR_X86FPStackRegister::fp5: return "fp5";
               case TR_X86FPStackRegister::fp6: return "fp6";
               case TR_X86FPStackRegister::fp7: return "fp7";
               default: TR_ASSERT( 0, "unexpected FPR number"); return unknownRegisterName('f');
               }
         default: TR_ASSERT( 0, "unexpected FPR number"); return unknownRegisterName('s');
         }
      }

   if (reg->getKind() == TR_FPR)
      size = TR_QuadWordReg;

   return getName(reg->getRegisterNumber(), size);
   }

void TR_Debug::dumpInstructionWithVFPState(TR::Instruction *instr, const TR_VFPState *prevState)
   {
   if (_file != NULL)
      {
      const TR_VFPState &vfpState = _cg->vfpState();
      print(_file, instr);

      // Print the VFP state if something changed
      //
      // Note: IA32.cpp version 1.158 has code to compare the VFP state with
      // that calculated using the old VFP scheme to make sure it's correct.
      // TODO: Remove this note when we trust the new VFP scheme.
      //
      if (prevState && (vfpState != *prevState))
         {
         trfprintf(_file, "\n\t%s VFP=%s+%d",
                       commentString(),
                       getName(vfpState._register), vfpState._displacement);
         }
      trfflush(_file);
      }
   }

//
// IA32 Instructions
//

void
TR_Debug::printRegRegInstruction(TR::FILE *pOutFile, const char *opCode, TR::RealRegister *reg1, TR::RealRegister *reg2)
   {
   trfprintf(pOutFile, "%s\t", opCode);
   print(pOutFile, reg1);
   if (reg2)
      {
      trfprintf(pOutFile, ", ");
      print(pOutFile, reg2);
      }
   }

void
TR_Debug::printRegMemInstruction(TR::FILE *pOutFile, const char *opCode, TR::RealRegister *reg, TR::RealRegister *base, int32_t offset)
   {
   trfprintf(pOutFile, "%s\t", opCode);
   print(pOutFile, reg);
   if (base)
      {
      trfprintf(pOutFile, ", [");
      print(pOutFile, base);
      trfprintf(pOutFile, " +%d]", offset);
      }
   }

void
TR_Debug::printRegImmInstruction(TR::FILE *pOutFile, const char *opCode, TR::RealRegister *reg, int32_t imm)
   {
   trfprintf(pOutFile, "%s\t", opCode);
   print(pOutFile, reg);
   if (imm <= 1024)
      trfprintf(pOutFile, ", %d", imm);
   else
      trfprintf(pOutFile, ", " POINTER_PRINTF_FORMAT, imm);
   }

void
TR_Debug::printMemRegInstruction(TR::FILE *pOutFile, const char *opCode, TR::RealRegister *base, int32_t offset, TR::RealRegister *reg)
   {
   trfprintf(pOutFile, "%s\t", opCode);
   trfprintf(pOutFile, "[");
   print(pOutFile, base);
   trfprintf(pOutFile, " +%d]", offset);
   if (reg)
      {
      trfprintf(pOutFile, ", ");
      print(pOutFile, reg);
      }
   }

void
TR_Debug::printMemImmInstruction(TR::FILE *pOutFile, const char *opCode, TR::RealRegister *base, int32_t offset, int32_t imm)
   {
   trfprintf(pOutFile, "%s\t", opCode);
   trfprintf(pOutFile, "[");
   print(pOutFile, base);
   trfprintf(pOutFile, " +%d]", offset);
   if (imm <= 1024)
      trfprintf(pOutFile, ", %d", imm);
   else
      trfprintf(pOutFile, ", " POINTER_PRINTF_FORMAT, imm);
   }

void
TR_Debug::printLabelInstruction(TR::FILE *pOutFile, const char *opCode, TR::LabelSymbol *label)
   {
   trfprintf(pOutFile, "%s\t", opCode);
   print(pOutFile, label);
   }


const char callRegName64[][5] =
   {
      "rax","rsi","rdx","rcx"
   };

const char callRegName32[][5] =
   {
      "eax","esi","edx","ecx"
   };

static const char*
getInterpretedMethodNameHelper(TR::MethodSymbol *methodSymbol,
                               TR::DataType     type){
   if (methodSymbol->isVMInternalNative() || methodSymbol->isJITInternalNative())
      return "icallVMprJavaSendNativeStatic";

   switch(type)
      {
      case TR::NoType:
         return "interpreterVoidStaticGlue";
      case TR::Int8:
      case TR::Int16:
      case TR::Int32:
         return "interpreterIntStaticGlue";
      case TR::Address:
         return "interpreterLongStaticGlue";
      case TR::Int64:
         return "interpreterLongStaticGlue";
      case TR::Float:
         return "interpreterFloatStaticGlue";
      case TR::Double:
         return "interpreterDoubleStaticGlue";
      default:
         TR_ASSERT(0, "Unsupported data type: %d", type);
         return "UNKNOWN interpreted method type";
      }
}


//
// IA32 Snippets
//

#ifdef J9_PROJECT_SPECIFIC
void
TR_Debug::print(TR::FILE *pOutFile, TR::X86CallSnippet  * snippet)
   {
   // *this   swipeable for debugger
   if (pOutFile == NULL)
      return;

   uint8_t            *bufferPos = snippet->getSnippetLabel()->getCodeLocation();
   TR::Node            *callNode = snippet->getNode();
   TR::SymbolReference *methodSymRef = callNode->getSymbolReference();
   TR::MethodSymbol    *methodSymbol = methodSymRef->getSymbol()->castToMethodSymbol();

   printSnippetLabel(pOutFile, snippet->getSnippetLabel(), bufferPos, getName(snippet));

   if (TR::Compiler->target.is64Bit())
      {
      int32_t   count;
      int32_t   size = 0;
      int32_t   offset = 8 * callNode->getNumChildren();

      for (count = 0; count < callNode->getNumChildren(); count++)
         {
            TR::Node       *child   = callNode->getChild(count);
            TR::DataType    type    = child->getType();


            switch (type)
               {
               case TR::Float:
               case TR::Int8:
               case TR::Int16:
               case TR::Int32:
                  printPrefix(pOutFile, NULL, bufferPos, 4);
                  trfprintf(pOutFile, "mov \tdword ptr[rsp+%d], %s\t\t#save registers for interpreter call snippet",
                                offset, callRegName32[count] );
                  offset-=8;
                  bufferPos+=4;
                  break;
               case TR::Double:
               case TR::Address:
               case TR::Int64:
                  printPrefix(pOutFile, NULL, bufferPos, 5);
                  trfprintf(pOutFile, "mov \tqword ptr[rsp+%d], %s\t\t#save registers for interpreter call snippet",
                                offset, callRegName64[count] );
                  offset-=8;
                  bufferPos+=5;
                  break;
               default:
                  TR_ASSERT(0, "unknown data type: %d", type);
                  break;
               }

          }
         intptrj_t ramMethod = (intptr_t)methodSymbol->getMethodAddress();

         printPrefix(pOutFile, NULL, bufferPos, 10);
         trfprintf(pOutFile, "mov \trdi, 0x%x\t\t# MOV8RegImm64",ramMethod);
         bufferPos+=10;

         printPrefix(pOutFile, NULL, bufferPos, 5);
         trfprintf(pOutFile, "jmp \t%s\t\t# jump out of snippet code",
                       getInterpretedMethodNameHelper (methodSymbol, callNode->getDataType()));
         bufferPos+=5;
      }

   else
      {
         intptrj_t ramMethod = (intptr_t)methodSymbol->getMethodAddress();

         printPrefix(pOutFile, NULL, bufferPos, 5);
         trfprintf(pOutFile, "mov \tedi, 0x%x\t\t# MOV8RegImm32",ramMethod);
         bufferPos+=5;

         printPrefix(pOutFile, NULL, bufferPos, 5);
         trfprintf(pOutFile, "jmp \t%s\t\t# jump out of snippet code",
                       getInterpretedMethodNameHelper (methodSymbol, callNode->getDataType()));
         bufferPos+=5;

      }

   return;

   }

#endif

void TR_Debug::printX86OOLSequences(TR::FILE *pOutFile)
   {

   auto oiIterator = _cg->getOutlinedInstructionsList().begin();
   while (oiIterator != _cg->getOutlinedInstructionsList().end())
      {
      trfprintf(pOutFile, "\n------------ start out-of-line instructions\n");

      TR::Instruction *instr = (*oiIterator)->getFirstInstruction();

      do {
         print(pOutFile, instr);
         instr = instr->getNext();
      } while (instr != (*oiIterator)->getAppendInstruction());

      if ((*oiIterator)->getAppendInstruction())
         {
         print(pOutFile, (*oiIterator)->getAppendInstruction());
         }
      trfprintf(pOutFile, "\n------------ end out-of-line instructions\n");

      ++oiIterator;
      }

   }



// } RTSJ Support ends

//
// AMD64 Snippets
//

#ifdef TR_TARGET_64BIT
uint8_t *
TR_Debug::printArgumentFlush(TR::FILE *              pOutFile,
                             TR::Node             *callNode,
                             bool                 isFlushToStack, // flush to stack or flush to regs
                             uint8_t             *bufferPos)
   {
   TR::MethodSymbol *methodSymbol = callNode->getSymbol()->castToMethodSymbol();
   TR::Linkage *linkage = _cg->getLinkage();

   const TR_X86LinkageProperties &linkageProperties = linkage->getProperties();

   int32_t   numGPArgs = 0;
   int32_t   numFPArgs = 0;

   for (int i = callNode->getFirstArgumentIndex(); i < callNode->getNumChildren(); i++)
      {
      TR::Node *child = callNode->getChild(i);
      int8_t modrmOffset=0;
      const char *opCodeName=NULL, *regName=NULL;

      // Compute differences based on datatype
      //
      switch (child->getDataType())
         {
         case TR::Int8:
         case TR::Int16:
         case TR::Int32:
            if (numGPArgs < linkageProperties.getNumIntegerArgumentRegisters())
               {
               opCodeName = "mov";
               modrmOffset = 1;
               TR::RealRegister::RegNum reg = linkageProperties.getIntegerArgumentRegister(numGPArgs);
               regName = getName(_cg->machine()->getX86RealRegister(reg));
               }
            numGPArgs++;
            break;
         case TR::Address:
         case TR::Int64:
            if (numGPArgs < linkageProperties.getNumIntegerArgumentRegisters())
               {
               opCodeName = "mov";
               modrmOffset = 2;
               TR::RealRegister::RegNum reg = linkageProperties.getIntegerArgumentRegister(numGPArgs);
               regName = getName(_cg->machine()->getX86RealRegister(reg), TR_DoubleWordReg);
               }
            numGPArgs++;
            break;
         case TR::Float:
            if (numFPArgs < linkageProperties.getNumFloatArgumentRegisters())
               {
               opCodeName = "movss";
               modrmOffset = 3;
               TR::RealRegister::RegNum reg = linkageProperties.getFloatArgumentRegister(numFPArgs);
               regName = getName(_cg->machine()->getX86RealRegister(reg), TR_QuadWordReg);
               }
            numFPArgs++;
            break;
         case TR::Double:
            if (numFPArgs < linkageProperties.getNumFloatArgumentRegisters())
               {
               opCodeName = "movsd";
               modrmOffset = 3;
               TR::RealRegister::RegNum reg = linkageProperties.getFloatArgumentRegister(numFPArgs);
               regName = getName(_cg->machine()->getX86RealRegister(reg), TR_QuadWordReg);
               }
            numFPArgs++;
            break;
         default:
         	break;
         }

      if (opCodeName)
         {
         // Compute differences based on addressing mode
         //
         uint32_t instrLength, displacement;
         switch (bufferPos[modrmOffset] & 0xc0)
            {
            case 0x40:
               displacement = *(uint8_t*)(bufferPos + modrmOffset + 2);
               instrLength = modrmOffset + 3;
               break;
            default:
               TR_ASSERT(0, "Unexpected Mod/RM value 0x%02x", bufferPos[modrmOffset]);
               // fall through
            case 0x80:
               displacement = *(uint32_t*)(bufferPos + modrmOffset + 2);
               instrLength = modrmOffset + 6;
               break;
            }

         // Print the instruction
         //
         printPrefix(pOutFile, NULL, bufferPos, instrLength);
         if (isFlushToStack)
            trfprintf(pOutFile, "%s\t[rsp +%d], %s", opCodeName, displacement, regName);
         else // Flush from stack to argument registers.
            trfprintf(pOutFile, "%s\t%s, [rsp +%d]", opCodeName, regName, displacement);
         bufferPos += instrLength;
         }
      else
         {
         // This argument is not in a register; skip it
         }
      }
   return bufferPos;
   }

#ifdef J9_PROJECT_SPECIFIC
uint8_t*
TR_Debug::printArgs(TR::FILE *pOutFile,
                    TR::AMD64WriteBarrierSnippet * snippet,
                    bool restoreRegs,
                    uint8_t *bufferPos)
   {
   TR::CodeGenerator *cg = snippet->cg();
   TR::Linkage *linkage = cg->getLinkage();
   TR::RegisterDependencyConditions *deps = snippet->getDependencies();

   TR::RealRegister *r1Real = toRealRegister(snippet->getDestOwningObjectRegister());
   TR::RealRegister::RegNum r1 = r1Real->getRegisterNumber();
   TR::RealRegister *r2Real = NULL;
   TR::RealRegister::RegNum r2 = TR::RealRegister::NoReg;

   if (deps->getNumPostConditions() > 1)
      {
      r2Real = toRealRegister(snippet->getSourceRegister());
      r2 = r2Real->getRegisterNumber();
      }

   // Fast path the single argument case.  Presently, only a batch arraycopy write barrier applies.
   //
   if (deps->getNumPostConditions() == 1)
      {
      if (r1 != TR::RealRegister::eax)
         {
         // Register is not in RAX so exchange it.  This applies to both building and restoring args.
         //
         printPrefix(pOutFile, NULL, bufferPos, 2);
         trfprintf(pOutFile, "xchg\t\trax, %s", getName(r1, TR_DoubleWordReg));
         bufferPos+=2;
         }
      return bufferPos;
      }

   int8_t action = 0;

   action = (restoreRegs ? 32 : 0) |
            (r1 == TR::RealRegister::eax ? 16 : 0) |
            (r1 == TR::RealRegister::esi ? 8 : 0) |
            (r2 == TR::RealRegister::esi ? 4 : 0) |
            (r2 == TR::RealRegister::eax ? 2 : 0) |
            (r1 == r2 ? 1 : 0);

   switch (action)
      {
      case 0:
      case 32:
         printPrefix(pOutFile, NULL, bufferPos, 2);
         trfprintf(pOutFile, "xchg\t\trax, %s",  getName(r1, TR_DoubleWordReg));
         bufferPos+=2;

         printPrefix(pOutFile, NULL, bufferPos, 3);
         trfprintf(pOutFile, "xchg\t\trsi, %s", getName(r2, TR_DoubleWordReg));
         bufferPos+=3;
         break;

      case 1:
         printPrefix(pOutFile, NULL, bufferPos, 2);
         trfprintf(pOutFile, "push\t\trsi");
         bufferPos+=2;

         printPrefix(pOutFile, NULL, bufferPos, 2);
         trfprintf(pOutFile, "xchg\t\trax, %s", getName(r1, TR_DoubleWordReg));
         bufferPos+=2;

         printPrefix(pOutFile, NULL, bufferPos, 3);
         trfprintf(pOutFile, "mov\t\trsi, rax");
         bufferPos+=3;
         break;

      case 2:

         printPrefix(pOutFile, NULL, bufferPos, 2);
         trfprintf(pOutFile, "xchg\t\trax, rsi");
         bufferPos+=2;

         printPrefix(pOutFile, NULL, bufferPos, 2);
         trfprintf(pOutFile, "xchg\t\trax, %s", getName(r1, TR_DoubleWordReg));
         bufferPos+=2;
         break;

      case 4:
      case 36:
         printPrefix(pOutFile, NULL, bufferPos, 2);
         trfprintf(pOutFile, "xchg\t\trax, %s", getName(r1, TR_DoubleWordReg));
         bufferPos+=2;
         break;

      case 8:
         printPrefix(pOutFile, NULL, bufferPos, 2);
         trfprintf(pOutFile, "xchg\t\trsi, rax");
         bufferPos+=2;

         printPrefix(pOutFile, NULL, bufferPos, 3);
         trfprintf(pOutFile, "xchg\t\trsi, %s", getName(r2, TR_DoubleWordReg));
         bufferPos+=3;
         break;

      case 10:
      case 42:
         printPrefix(pOutFile, NULL, bufferPos, 2);
         trfprintf(pOutFile, "xchg\t\trsi, rax");
         bufferPos+=2;
         break;

      case 13:
         printPrefix(pOutFile, NULL, bufferPos, 1);
         trfprintf(pOutFile, "push\t\trax");
         bufferPos+=1;

         printPrefix(pOutFile, NULL, bufferPos, 2);
         trfprintf(pOutFile, "mov\t\trax, rsi");
         bufferPos+=2;
         break;

      case 16:
      case 48:
         printPrefix(pOutFile, NULL, bufferPos, 3);
         trfprintf(pOutFile, "xchg\t\trsi, %s", getName(r2, TR_DoubleWordReg));
         bufferPos+=3;
         break;

      case 19:
         printPrefix(pOutFile, NULL, bufferPos, 1);
         trfprintf(pOutFile, "push\t\trsi");
         bufferPos+=1;

         printPrefix(pOutFile, NULL, bufferPos, 2);
         trfprintf(pOutFile, "mov\t\trsi, rax");
         bufferPos+=2;
         break;

      case 33:
         printPrefix(pOutFile, NULL, bufferPos, 2);
         trfprintf(pOutFile, "xchg\t\trax, %s", getName(r1, TR_DoubleWordReg));
         bufferPos+=2;

         printPrefix(pOutFile, NULL, bufferPos, 1);
         trfprintf(pOutFile, "pop rsi");
         bufferPos+=1;
         break;

      case 34:
         printPrefix(pOutFile, NULL, bufferPos, 2);
         trfprintf(pOutFile, "xchg\t\trax, %s", getName(r1, TR_DoubleWordReg));
         bufferPos+=2;

         printPrefix(pOutFile, NULL, bufferPos, 2);
         trfprintf(pOutFile, "xchg\t\trsi, rax");
         bufferPos+=2;
         break;

      case 40:
         printPrefix(pOutFile, NULL, bufferPos, 3);
         trfprintf(pOutFile, "xchg\t\trsi, %s", getName(r2, TR_DoubleWordReg));
         bufferPos+=3;

         printPrefix(pOutFile, NULL, bufferPos, 2);
         bufferPos+=2;
         trfprintf(pOutFile, "xchg\t\trsi, rax");
         break;

      case 45:
         printPrefix(pOutFile, NULL, bufferPos, 1);
         trfprintf(pOutFile, "pop\t\trax");
         bufferPos+=1;
         break;

      case 51:
         printPrefix(pOutFile, NULL, bufferPos, 1);
         trfprintf(pOutFile, "pop\t\trsi");
         bufferPos+=1;
         break;
      }

   return bufferPos;

   }

static uint8_t *
estimateJumpSize(TR::AMD64WriteBarrierSnippet * snippet,
                 uint8_t * bufferPos)
   {
   TR::LabelSymbol *restartLabel = snippet->getRestartLabel();
   uint8_t        *destination = restartLabel->getCodeLocation();
   intptrj_t  distance    = destination - (bufferPos + 2);

   if (snippet->getForceLongRestartJump())
      {
         return bufferPos += 5;
      }
   else
      {
         if (distance >= -128 && distance <= 127)
            {
            return bufferPos += 2;
            }
         else
            {
            return bufferPos += 5;
            }

      }
   }

void
TR_Debug::print(TR::FILE *pOutFile, TR::AMD64WriteBarrierSnippet * snippet)
   {
   if (pOutFile == NULL)
      return;

   uint8_t *bufferPos = snippet->getSnippetLabel()->getCodeLocation();
   printSnippetLabel(pOutFile, snippet->getSnippetLabel(), bufferPos, getName(snippet));

   bufferPos = printArgs(pOutFile, snippet, false, bufferPos);
   printPrefix(pOutFile, NULL, bufferPos, 5);
   trfprintf(pOutFile, "call\t\tjitWriteBarrierStoreGenerational");
   bufferPos += 5;
   bufferPos = printArgs(pOutFile, snippet, true, bufferPos);

   printRestartJump(pOutFile, snippet, bufferPos);
   bufferPos = estimateJumpSize(snippet, bufferPos);

   bufferPos = printArgs(pOutFile, snippet, false, bufferPos);
   printPrefix(pOutFile, NULL, bufferPos, 5);
   trfprintf(pOutFile, "call\t\tjitWriteBarrierStoreGenerationalAndConcurrentMark");
   bufferPos += 5;
   bufferPos = printArgs(pOutFile, snippet, true, bufferPos);

   printRestartJump(pOutFile, snippet, bufferPos);
   bufferPos = estimateJumpSize(snippet, bufferPos);

   }
#endif

#endif

static const char * opCodeToNameMap[IA32NumOpCodes] =
   {
   "BADIA32Op",
   "ADC1AccImm1",
   "ADC2AccImm2",
   "ADC4AccImm4",
   "ADC8AccImm4", // (AMD64)
   "ADC1RegImm1",
   "ADC2RegImm2",
   "ADC2RegImms",
   "ADC4RegImm4",
   "ADC8RegImm4", // (AMD64)
   "ADC4RegImms",
   "ADC8RegImms", // (AMD64)
   "ADC1MemImm1",
   "ADC2MemImm2",
   "ADC2MemImms",
   "ADC4MemImm4",
   "ADC8MemImm4", // (AMD64)
   "ADC4MemImms",
   "ADC8MemImms", // (AMD64)
   "ADC1RegReg",
   "ADC2RegReg",
   "ADC4RegReg",
   "ADC8RegReg", // (AMD64)
   "ADC1RegMem",
   "ADC2RegMem",
   "ADC4RegMem",
   "ADC8RegMem", // (AMD64)
   "ADC1MemReg",
   "ADC2MemReg",
   "ADC4MemReg",
   "ADC8MemReg", // (AMD64)
   "ADD1AccImm1",
   "ADD2AccImm2",
   "ADD4AccImm4",
   "ADD8AccImm4", // (AMD64)
   "ADD1RegImm1",
   "ADD2RegImm2",
   "ADD2RegImms",
   "ADD4RegImm4",
   "ADD8RegImm4", // (AMD64)
   "ADD4RegImms",
   "ADD8RegImms", // (AMD64)
   "ADD1MemImm1",
   "ADD2MemImm2",
   "ADD2MemImms",
   "ADD4MemImm4",
   "ADD8MemImm4", // (AMD64)
   "ADD4MemImms",
   "ADD8MemImms", // (AMD64)
   "ADD1RegReg",
   "ADD2RegReg",
   "ADD4RegReg",
   "ADD8RegReg", // (AMD64)
   "ADD1RegMem",
   "ADD2RegMem",
   "ADD4RegMem",
   "ADD8RegMem", // (AMD64)
   "ADD1MemReg",
   "ADD2MemReg",
   "ADD4MemReg",
   "ADD8MemReg", // (AMD64)
   "ADDSSRegReg",
   "ADDSSRegMem",
   "ADDPSRegReg",
   "ADDPSRegMem",
   "ADDSDRegReg",
   "ADDSDRegMem",
   "ADDPDRegReg",
   "ADDPDRegMem",
   "LADD1MemReg",
   "LADD2MemReg",
   "LADD4MemReg",
   "LADD8MemReg", // (AMD64)
   "LXADD1MemReg",
   "LXADD2MemReg",
   "LXADD4MemReg",
   "LXADD8MemReg", // (AMD64)
   "AND1AccImm1",
   "AND2AccImm2",
   "AND4AccImm4",
   "AND8AccImm4", // (AMD64)
   "AND1RegImm1",
   "AND2RegImm2",
   "AND2RegImms",
   "AND4RegImm4",
   "AND8RegImm4", // (AMD64)
   "AND4RegImms",
   "AND8RegImms", // (AMD64)
   "AND1MemImm1",
   "AND2MemImm2",
   "AND2MemImms",
   "AND4MemImm4",
   "AND8MemImm4", // (AMD64)
   "AND4MemImms",
   "AND8MemImms", // (AMD64)
   "AND1RegReg",
   "AND2RegReg",
   "AND4RegReg",
   "AND8RegReg", // (AMD64)
   "AND1RegMem",
   "AND2RegMem",
   "AND4RegMem",
   "AND8RegMem", // (AMD64)
   "AND1MemReg",
   "AND2MemReg",
   "AND4MemReg",
   "AND8MemReg", // (AMD64)
   "BSF2RegReg",
   "BSF4RegReg",
   "BSF8RegReg",
   "BSR4RegReg",
   "BSR8RegReg",
   "BSWAP4Reg",
   "BSWAP8Reg", // (AMD64)
   "BTS4RegReg",
   "BTS4MemReg",
   "CALLImm4",
   "CALLREXImm4", // (AMD64)
   "CALLReg",
   "CALLREXReg", // (AMD64)
   "CALLMem",
   "CALLREXMem", // (AMD64)
   "CBWAcc",
   "CBWEAcc",
   "CMOVA4RegMem",
   "CMOVB4RegMem",
   "CMOVE4RegMem",
   "CMOVE8RegMem", // (64-bit)
   "CMOVG4RegMem",
   "CMOVL4RegMem",
   "CMOVNE4RegMem",
   "CMOVNE8RegMem", // (64-bit)
   "CMOVNO4RegMem",
   "CMOVNS4RegMem",
   "CMOVO4RegMem",
   "CMOVS4RegMem",
   "CMP1AccImm1",
   "CMP2AccImm2",
   "CMP4AccImm4",
   "CMP8AccImm4", // (AMD64)
   "CMP1RegImm1",
   "CMP2RegImm2",
   "CMP2RegImms",
   "CMP4RegImm4",
   "CMP8RegImm4", // (AMD64)
   "CMP4RegImms",
   "CMP8RegImms", // (AMD64)
   "CMP1MemImm1",
   "CMP2MemImm2",
   "CMP2MemImms",
   "CMP4MemImm4",
   "CMP8MemImm4", // (AMD64)
   "CMP4MemImms",
   "CMP8MemImms", // (AMD64)
   "CMP1RegReg",
   "CMP2RegReg",
   "CMP4RegReg",
   "CMP8RegReg", // (AMD64)
   "CMP1RegMem",
   "CMP2RegMem",
   "CMP4RegMem",
   "CMP8RegMem", // (AMD64)
   "CMP1MemReg",
   "CMP2MemReg",
   "CMP4MemReg",
   "CMP8MemReg", // (AMD64)
   "CMPXCHG1MemReg",
   "CMPXCHG2MemReg",
   "CMPXCHG4MemReg",
   "CMPXCHG8MemReg", // (AMD64)
   "CMPXCHG8BMem", // (IA32 only)
   "CMPXCHG16BMem", // (AMD64)
   "LCMPXCHG1MemReg",
   "LCMPXCHG2MemReg",
   "LCMPXCHG4MemReg",
   "LCMPXCHG8MemReg", // (AMD64)
   "LCMPXCHG8BMem", // (IA32 only)
   "LCMPXCHG16BMem", // (AMD64)
   "XALCMPXCHG8MemReg",
   "XACMPXCHG8MemReg",
   "XALCMPXCHG4MemReg",
   "XACMPXCHG4MemReg",
   "CVTSI2SSRegReg4",
   "CVTSI2SSRegReg8", // (AMD64)
   "CVTSI2SSRegMem",
   "CVTSI2SSRegMem8", // (AMD64)
   "CVTSI2SDRegReg4",
   "CVTSI2SDRegReg8", // (AMD64)
   "CVTSI2SDRegMem",
   "CVTSI2SDRegMem8", // (AMD64)
   "CVTTSS2SIReg4Reg",
   "CVTTSS2SIReg8Reg", // (AMD64)
   "CVTTSS2SIReg4Mem",
   "CVTTSS2SIReg8Mem", // (AMD64)
   "CVTTSD2SIReg4Reg",
   "CVTTSD2SIReg8Reg", // (AMD64)
   "CVTTSD2SIReg4Mem",
   "CVTTSD2SIReg8Mem", // (AMD64)
   "CVTSS2SDRegReg",
   "CVTSS2SDRegMem",
   "CVTSD2SSRegReg",
   "CVTSD2SSRegMem",
   "CWDAcc",
   "CDQAcc",
   "CQOAcc", // (AMD64)
   "DEC1Reg",
   "DEC2Reg",
   "DEC4Reg",
   "DEC8Reg", // (AMD64)
   "DEC1Mem",
   "DEC2Mem",
   "DEC4Mem",
   "DEC8Mem", // (AMD64)
   "FABSReg",
   "DABSReg",
   "FSQRTReg",
   "DSQRTReg",
   "FADDRegReg",
   "DADDRegReg",
   "FADDPReg",
   "FADDRegMem",
   "DADDRegMem",
   "FIADDRegMem",
   "DIADDRegMem",
   "FSADDRegMem",
   "DSADDRegMem",
   "FCHSReg",
   "DCHSReg",
   "FDIVRegReg",
   "DDIVRegReg",
   "FDIVRegMem",
   "DDIVRegMem",
   "FDIVPReg",
   "FIDIVRegMem",
   "DIDIVRegMem",
   "FSDIVRegMem",
   "DSDIVRegMem",
   "FDIVRRegReg",
   "DDIVRRegReg",
   "FDIVRRegMem",
   "DDIVRRegMem",
   "FDIVRPReg",
   "FIDIVRRegMem",
   "DIDIVRRegMem",
   "FSDIVRRegMem",
   "DSDIVRRegMem",
   "FILDRegMem",
   "DILDRegMem",
   "FLLDRegMem",
   "DLLDRegMem",
   "FSLDRegMem",
   "DSLDRegMem",
   "FISTMemReg",
   "DISTMemReg",
   "FISTPMem",
   "DISTPMem",
   "FLSTPMem",
   "DLSTPMem",
   "FSSTMemReg",
   "DSSTMemReg",
   "FSSTPMem",
   "DSSTPMem",
   "FLDLN2",
   "FLDRegReg",
   "DLDRegReg",
   "FLDRegMem",
   "DLDRegMem",
   "FLD0Reg",
   "DLD0Reg",
   "FLD1Reg",
   "DLD1Reg",
   "FLDMem",
   "DLDMem",
   "LDCWMem",
   "FMULRegReg",
   "DMULRegReg",
   "FMULPReg",
   "FMULRegMem",
   "DMULRegMem",
   "FIMULRegMem",
   "DIMULRegMem",
   "FSMULRegMem",
   "DSMULRegMem",
   "FNCLEX",
   "FPREMRegReg",
   "FSCALERegReg",
   "FSTMemReg",
   "DSTMemReg",
   "FSTRegReg",
   "DSTRegReg",
   "FSTPMemReg",
   "DSTPMemReg",
   "FSTPReg",
   "DSTPReg",
   "STCWMem",
   "STSWMem",
   "STSWAcc",
   "FSUBRegReg",
   "DSUBRegReg",
   "FSUBRegMem",
   "DSUBRegMem",
   "FSUBPReg",
   "FISUBRegMem",
   "DISUBRegMem",
   "FSSUBRegMem",
   "DSSUBRegMem",
   "FSUBRRegReg",
   "DSUBRRegReg",
   "FSUBRRegMem",
   "DSUBRRegMem",
   "FSUBRPReg",
   "FISUBRRegMem",
   "DISUBRRegMem",
   "FSSUBRRegMem",
   "DSSUBRRegMem",
   "FTSTReg",
   "FCOMRegReg",
   "DCOMRegReg",
   "FCOMRegMem",
   "DCOMRegMem",
   "FCOMPReg",
   "FCOMPMem",
   "DCOMPMem",
   "FCOMPP",
   "FCOMIRegReg",
   "DCOMIRegReg",
   "FCOMIPReg",
   "FYL2X",
   "UCOMISSRegReg",
   "UCOMISSRegMem",
   "UCOMISDRegReg",
   "UCOMISDRegMem",
   "FXCHReg",
   "IDIV1AccReg",
   "IDIV2AccReg",
   "IDIV4AccReg",
   "IDIV8AccReg", // (AMD64)
   "DIV4AccReg",
   "DIV8AccReg",  // (AMD64)
   "IDIV1AccMem",
   "IDIV2AccMem",
   "IDIV4AccMem",
   "IDIV8AccMem", // (AMD64)
   "DIV4AccMem",
   "DIV8AccMem",  // (AMD64)
   "DIVSSRegReg",
   "DIVSSRegMem",
   "DIVSDRegReg",
   "DIVSDRegMem",
   "IMUL1AccReg",
   "IMUL2AccReg",
   "IMUL4AccReg",
   "IMUL8AccReg", // (AMD64)
   "IMUL1AccMem",
   "IMUL2AccMem",
   "IMUL4AccMem",
   "IMUL8AccMem", // (AMD64)
   "IMUL2RegReg",
   "IMUL4RegReg",
   "IMUL8RegReg", // (AMD64)
   "IMUL2RegMem",
   "IMUL4RegMem",
   "IMUL8RegMem", // (AMD64)
   "IMUL2RegRegImm2",
   "IMUL2RegRegImms",
   "IMUL4RegRegImm4",
   "IMUL8RegRegImm4", // (AMD64)
   "IMUL4RegRegImms",
   "IMUL8RegRegImms", // (AMD64)
   "IMUL2RegMemImm2",
   "IMUL2RegMemImms",
   "IMUL4RegMemImm4",
   "IMUL8RegMemImm4", // (AMD64)
   "IMUL4RegMemImms",
   "IMUL8RegMemImms", // (AMD64)
   "MUL1AccReg",
   "MUL2AccReg",
   "MUL4AccReg",
   "MUL8AccReg", // (AMD64)
   "MUL1AccMem",
   "MUL2AccMem",
   "MUL4AccMem",
   "MUL8AccMem", // (AMD64)
   "MULSSRegReg",
   "MULSSRegMem",
   "MULPSRegReg",
   "MULPSRegMem",
   "MULSDRegReg",
   "MULSDRegMem",
   "MULPDRegReg",
   "MULPDRegMem",
   "INC1Reg",
   "INC2Reg",
   "INC4Reg",
   "INC8Reg", // (AMD64)
   "INC1Mem",
   "INC2Mem",
   "INC4Mem",
   "INC8Mem", // (AMD64)
   "JA1",
   "JAE1",
   "JB1",
   "JBE1",
   "JE1",
   "JNE1",
   "JG1",
   "JGE1",
   "JL1",
   "JLE1",
   "JO1",
   "JNO1",
   "JS1",
   "JNS1",
   "JPO1",
   "JPE1",
   "JMP1",
   "JA4",
   "JAE4",
   "JB4",
   "JBE4",
   "JE4",
   "JNE4",
   "JG4",
   "JGE4",
   "JL4",
   "JLE4",
   "JO4",
   "JNO4",
   "JS4",
   "JNS4",
   "JPO4",
   "JPE4",
   "JMP4",
   "JMPReg",
   "JMPMem",
   "JRCXZ1",
   "LOOP1",
   "LAHF",
   "LDDQU",
   "LEA2RegMem",
   "LEA4RegMem",
   "LEA8RegMem", // (AMD64)
   "S1MemReg",
   "S2MemReg",
   "S4MemReg",
   "S8MemReg", // (AMD64)
   "S1MemImm1",
   "S2MemImm2",
   "S4MemImm4",
   "XRS4MemImm4",
   "S8MemImm4", // (AMD64)
   "XRS8MemImm4", // (AMD64)
   "L1RegMem",
   "L2RegMem",
   "L4RegMem",
   "L8RegMem", // (AMD64)
   "MOVAPSRegReg",
   "MOVAPSRegMem",
   "MOVAPSMemReg",
   "MOVAPDRegReg",
   "MOVAPDRegMem",
   "MOVAPDMemReg",
   "MOVUPSRegReg",
   "MOVUPSRegMem",
   "MOVUPSMemReg",
   "MOVUPDRegReg",
   "MOVUPDRegMem",
   "MOVUPDMemReg",
   "MOVSSRegReg",
   "MOVSSRegMem",
   "MOVSSMemReg",
   "MOVSDRegReg",
   "MOVSDRegMem",
   "MOVSDMemReg",
   "SQRTSFRegReg",
   "SQRTSDRegReg",
   "MOVDRegReg4",
   "MOVQRegReg8", // (AMD64)
   "MOVDReg4Reg",
   "MOVQReg8Reg", // (AMD64)
   "MOVDQURegReg",
   "MOVDQURegMem",
   "MOVDQUMemReg",
   "MOV1RegReg",
   "MOV2RegReg",
   "MOV4RegReg",
   "MOV8RegReg", // (AMD64)
   "CMOVG4RegReg",
   "CMOVG8RegReg", // (AMD64)
   "CMOVL4RegReg",
   "CMOVL8RegReg", // (AMD64)
   "CMOVE4RegReg",
   "CMOVE8RegReg", // (AMD64)
   "CMOVNE4RegReg",
   "CMOVNE8RegReg", // (AMD64)
   "MOV1RegImm1",
   "MOV2RegImm2",
   "MOV4RegImm4",
   "MOV8RegImm4", // (AMD64)
   "MOV8RegImm64", // (AMD64)
   "MOVLPDRegMem",
   "MOVLPDMemReg",
   "MOVQRegMem",
   "MOVQMemReg",
   "MOVSB",
   "MOVSW",
   "MOVSD",
   "MOVSQ", // (AMD64)
   "MOVSXReg2Reg1",
   "MOVSXReg4Reg1",
   "MOVSXReg8Reg1", // (AMD64)
   "MOVSXReg4Reg2",
   "MOVSXReg8Reg2", // (AMD64)
   "MOVSXReg8Reg4", // (AMD64)
   "MOVSXReg2Mem1",
   "MOVSXReg4Mem1",
   "MOVSXReg8Mem1", // (AMD64)
   "MOVSXReg4Mem2",
   "MOVSXReg8Mem2", // (AMD64)
   "MOVSXReg8Mem4", // (AMD64)
   "MOVZXReg2Reg1",
   "MOVZXReg4Reg1",
   "MOVZXReg8Reg1", // (AMD64)
   "MOVZXReg4Reg2",
   "MOVZXReg8Reg2", // (AMD64)
   "MOVZXReg8Reg4", // (AMD64)
   "MOVZXReg2Mem1",
   "MOVZXReg4Mem1",
   "MOVZXReg8Mem1", // (AMD64)
   "MOVZXReg4Mem2",
   "MOVZXReg8Mem2", // (AMD64)
   "NEG1Reg",
   "NEG2Reg",
   "NEG4Reg",
   "NEG8Reg", // (AMD64)
   "NEG1Mem",
   "NEG2Mem",
   "NEG4Mem",
   "NEG8Mem", // (AMD64)
   "NOT1Reg",
   "NOT2Reg",
   "NOT4Reg",
   "NOT8Reg", // (AMD64)
   "NOT1Mem",
   "NOT2Mem",
   "NOT4Mem",
   "NOT8Mem", // (AMD64)
   "OR1AccImm1",
   "OR2AccImm2",
   "OR4AccImm4",
   "OR8AccImm4", // (AMD64)
   "OR1RegImm1",
   "OR2RegImm2",
   "OR2RegImms",
   "OR4RegImm4",
   "OR8RegImm4", // (AMD64)
   "OR4RegImms",
   "OR8RegImms", // (AMD64)
   "OR1MemImm1",
   "OR2MemImm2",
   "OR2MemImms",
   "OR4MemImm4",
   "OR8MemImm4", // (AMD64)
   "OR4MemImms",
   "LOR4MemImms",
   "LOR4MemReg",
   "LOR8MemReg", // (AMD64)
   "OR8MemImms", // (AMD64)
   "OR1RegReg",
   "OR2RegReg",
   "OR4RegReg",
   "OR8RegReg", // (AMD64)
   "OR1RegMem",
   "OR2RegMem",
   "OR4RegMem",
   "OR8RegMem", // (AMD64)
   "OR1MemReg",
   "OR2MemReg",
   "OR4MemReg",
   "OR8MemReg", // (AMD64)
   "PAUSE",
   "PCMPEQBRegReg",
   "PMOVMSKB4RegReg",
   "PMOVZXWD",
   "PMULLD",
   "PADDD",
   "PSHUFDRegRegImm1",
   "PSHUFDRegMemImm1",
   "PSRLDQRegImm1",
   "PMOVZXxmm18Reg",
   "POPCNT4RegReg",
   "POPCNT8RegReg", // (AMD64)
   "POPReg",
   "POPMem",
   "PUSHImms",
   "PUSHImm4",
   "PUSHReg",
   "PUSHRegLong",
   "PUSHMem",
   "RCL1RegImm1",
   "RCL4RegImm1",
   "RCR1RegImm1",
   "RCR4RegImm1",
   "REPMOVSB",
   "REPMOVSW",
   "REPMOVSD",
   "REPMOVSQ",
   "REPSTOSB",
   "REPSTOSW",
   "REPSTOSD",
   "REPSTOSQ", // (AMD64)
   "RET",
   "RETImm2",
   "ROL1RegImm1",
   "ROL2RegImm1",
   "ROL4RegImm1",
   "ROL8RegImm1", // (AMD64)
   "ROL4RegCL",
   "ROL8RegCL",   // (AMD64)
   "ROR1RegImm1",
   "ROR2RegImm1",
   "ROR4RegImm1",
   "ROR8RegImm1", // (AMD64)
   "SAHF",
   "SHL1RegImm1",
   "SHL1RegCL",
   "SHL2RegImm1",
   "SHL2RegCL",
   "SHL4RegImm1",
   "SHL8RegImm1", // (AMD64)
   "SHL4RegCL",
   "SHL8RegCL", // (AMD64)
   "SHL1MemImm1",
   "SHL1MemCL",
   "SHL2MemImm1",
   "SHL2MemCL",
   "SHL4MemImm1",
   "SHL8MemImm1", // (AMD64)
   "SHL4MemCL",
   "SHL8MemCL", // (AMD64)
   "SHR1RegImm1",
   "SHR1RegCL",
   "SHR2RegImm1",
   "SHR2RegCL",
   "SHR4RegImm1",
   "SHR8RegImm1", // (AMD64)
   "SHR4RegCL",
   "SHR8RegCL", // (AMD64)
   "SHR1MemImm1",
   "SHR1MemCL",
   "SHR2MemImm1",
   "SHR2MemCL",
   "SHR4MemImm1",
   "SHR8MemImm1", // (AMD64)
   "SHR4MemCL",
   "SHR8MemCL", // (AMD64)
   "SAR1RegImm1",
   "SAR1RegCL",
   "SAR2RegImm1",
   "SAR2RegCL",
   "SAR4RegImm1",
   "SAR8RegImm1", // (AMD64)
   "SAR4RegCL",
   "SAR8RegCL", // (AMD64)
   "SAR1MemImm1",
   "SAR1MemCL",
   "SAR2MemImm1",
   "SAR2MemCL",
   "SAR4MemImm1",
   "SAR8MemImm1", // (AMD64)
   "SAR4MemCL",
   "SAR8MemCL", // (AMD64)
   "SBB1AccImm1",
   "SBB2AccImm2",
   "SBB4AccImm4",
   "SBB8AccImm4", // (AMD64)
   "SBB1RegImm1",
   "SBB2RegImm2",
   "SBB2RegImms",
   "SBB4RegImm4",
   "SBB8RegImm4", // (AMD64)
   "SBB4RegImms",
   "SBB8RegImms", // (AMD64)
   "SBB1MemImm1",
   "SBB2MemImm2",
   "SBB2MemImms",
   "SBB4MemImm4",
   "SBB8MemImm4", // (AMD64)
   "SBB4MemImms",
   "SBB8MemImms", // (AMD64)
   "SBB1RegReg",
   "SBB2RegReg",
   "SBB4RegReg",
   "SBB8RegReg", // (AMD64)
   "SBB1RegMem",
   "SBB2RegMem",
   "SBB4RegMem",
   "SBB8RegMem", // (AMD64)
   "SBB1MemReg",
   "SBB2MemReg",
   "SBB4MemReg",
   "SBB8MemReg", // (AMD64)
   "SETA1Reg",
   "SETAE1Reg",
   "SETB1Reg",
   "SETBE1Reg",
   "SETE1Reg",
   "SETNE1Reg",
   "SETG1Reg",
   "SETGE1Reg",
   "SETL1Reg",
   "SETLE1Reg",
   "SETS1Reg",
   "SETNS1Reg",
   "SETPO1Reg",
   "SETPE1Reg",
   "SETA1Mem",
   "SETAE1Mem",
   "SETB1Mem",
   "SETBE1Mem",
   "SETE1Mem",
   "SETNE1Mem",
   "SETG1Mem",
   "SETGE1Mem",
   "SETL1Mem",
   "SETLE1Mem",
   "SETS1Mem",
   "SETNS1Mem",
   "SHLD4RegRegImm1",
   "SHLD4RegRegCL",
   "SHLD4MemRegImm1",
   "SHLD4MemRegCL",
   "SHRD4RegRegImm1",
   "SHRD4RegRegCL",
   "SHRD4MemRegImm1",
   "SHRD4MemRegCL",
   "STOSB",
   "STOSW",
   "STOSD",
   "STOSQ", // (AMD64)
   "SUB1AccImm1",
   "SUB2AccImm2",
   "SUB4AccImm4",
   "SUB8AccImm4", // (AMD64)
   "SUB1RegImm1",
   "SUB2RegImm2",
   "SUB2RegImms",
   "SUB4RegImm4",
   "SUB8RegImm4", // (AMD64)
   "SUB4RegImms",
   "SUB8RegImms", // (AMD64)
   "SUB1MemImm1",
   "SUB2MemImm2",
   "SUB2MemImms",
   "SUB4MemImm4",
   "SUB8MemImm4", // (AMD64)
   "SUB4MemImms",
   "SUB8MemImms", // (AMD64)
   "SUB1RegReg",
   "SUB2RegReg",
   "SUB4RegReg",
   "SUB8RegReg", // (AMD64)
   "SUB1RegMem",
   "SUB2RegMem",
   "SUB4RegMem",
   "SUB8RegMem", // (AMD64)
   "SUB1MemReg",
   "SUB2MemReg",
   "SUB4MemReg",
   "SUB8MemReg", // (AMD64)
   "SUBSSRegReg",
   "SUBSSRegMem",
   "SUBSDRegReg",
   "SUBSDRegMem",
   "TEST1AccImm1",
   "TEST1AccHImm1",
   "TEST2AccImm2",
   "TEST4AccImm4",
   "TEST8AccImm4", // (AMD64)
   "TEST1RegImm1",
   "TEST2RegImm2",
   "TEST4RegImm4",
   "TEST8RegImm4", // (AMD64)
   "TEST1MemImm1",
   "TEST2MemImm2",
   "TEST4MemImm4",
   "TEST8MemImm4", // (AMD64)
   "TEST1RegReg",
   "TEST2RegReg",
   "TEST4RegReg",
   "TEST8RegReg", // (AMD64)
   "TEST1MemReg",
   "TEST2MemReg",
   "TEST4MemReg",
   "TEST8MemReg", // (AMD64)
   "XABORT",
   "XBEGIN2",
   "XBEGIN4",
   "XCHG2AccReg",
   "XCHG4AccReg",
   "XCHG8AccReg", // (AMD64)
   "XCHG1RegReg",
   "XCHG2RegReg",
   "XCHG4RegReg",
   "XCHG8RegReg", // (AMD64)
   "XCHG1RegMem",
   "XCHG2RegMem",
   "XCHG4RegMem",
   "XCHG8RegMem", // (AMD64)
   "XCHG1MemReg",
   "XCHG2MemReg",
   "XCHG4MemReg",
   "XCHG8MemReg", // (AMD64)
   "XEND",
   "XOR1AccImm1",
   "XOR2AccImm2",
   "XOR4AccImm4",
   "XOR8AccImm4", // (AMD64)
   "XOR1RegImm1",
   "XOR2RegImm2",
   "XOR2RegImms",
   "XOR4RegImm4",
   "XOR8RegImm4", // (AMD64)
   "XOR4RegImms",
   "XOR8RegImms", // (AMD64)
   "XOR1MemImm1",
   "XOR2MemImm2",
   "XOR2MemImms",
   "XOR4MemImm4",
   "XOR8MemImm4", // (AMD64)
   "XOR4MemImms",
   "XOR8MemImms", // (AMD64)
   "XOR1RegReg",
   "XOR2RegReg",
   "XOR4RegReg",
   "XOR8RegReg", // (AMD64)
   "XOR1RegMem",
   "XOR2RegMem",
   "XOR4RegMem",
   "XOR8RegMem", // (AMD64)
   "XOR1MemReg",
   "XOR2MemReg",
   "XOR4MemReg",
   "XOR8MemReg", // (AMD64)
   "XORPSRegReg",
   "XORPSRegMem",
   "XORPDRegReg",
   "MFENCE",
   "LFENCE",
   "SFENCE",
   "PCMPESTRI",
   "PREFETCHNTA",
   "PREFETCHT0",
   "PREFETCHT1",
   "PREFETCHT2",
   "ANDNPSRegReg",
   "ANDNPDRegReg",
   "PSLLQRegImm1",
   "PSRLQRegImm1",
   "FENCE",
   "VGFENCE",
   "PROCENTRY",
   "DQImm64", // (AMD64)
   "DDImm4",
   "DWImm2",
   "DBImm1",
   "WRTBAR",
   "ASSOCREGS",
   "FPREGSPILL",
   "VirtualGuardNOP",
   "LABEL",
   "FCMPEVAL",
   "RestoreVMThread",
   "PPS_OPCount",
   "PPS_OPField",
   "AdjustFramePtr",
   "ReturnMarker"
   };

const char *
TR_Debug::getOpCodeName(TR_X86OpCode  * opCode)
   {
   return opCodeToNameMap[opCode->getOpCodeValue()];
   }

static const char * opCodeToMnemonicMap[IA32NumOpCodes] =
   {
   "int3",           // BADIA32Op
   "adc",            // ADC1AccImm1
   "adc",            // ADC2AccImm2
   "adc",            // ADC4AccImm4
   "adc",            // ADC8AccImm4 (AMD64)
   "adc",            // ADC1RegImm1
   "adc",            // ADC2RegImm2
   "adc",            // ADC2RegImms
   "adc",            // ADC4RegImm4
   "adc",            // ADC8RegImm4 (AMD64)
   "adc",            // ADC4RegImms
   "adc",            // ADC8RegImms (AMD64)
   "adc",            // ADC1MemImm1
   "adc",            // ADC2MemImm2
   "adc",            // ADC2MemImms
   "adc",            // ADC4MemImm4
   "adc",            // ADC8MemImm4 (AMD64)
   "adc",            // ADC4MemImms
   "adc",            // ADC8MemImms (AMD64)
   "adc",            // ADC1RegReg
   "adc",            // ADC2RegReg
   "adc",            // ADC4RegReg
   "adc",            // ADC8RegReg (AMD64)
   "adc",            // ADC1RegMem
   "adc",            // ADC2RegMem
   "adc",            // ADC4RegMem
   "adc",            // ADC8RegMem (AMD64)
   "adc",            // ADC1MemReg
   "adc",            // ADC2MemReg
   "adc",            // ADC4MemReg
   "adc",            // ADC8MemReg (AMD64)
   "add",            // ADD1AccImm1
   "add",            // ADD2AccImm2
   "add",            // ADD4AccImm4
   "add",            // ADD8AccImm4 (AMD64)
   "add",            // ADD1RegImm1
   "add",            // ADD2RegImm2
   "add",            // ADD2RegImms
   "add",            // ADD4RegImm4
   "add",            // ADD8RegImm4 (AMD64)
   "add",            // ADD4RegImms
   "add",            // ADD8RegImms (AMD64)
   "add",            // ADD1MemImm1
   "add",            // ADD2MemImm2
   "add",            // ADD2MemImms
   "add",            // ADD4MemImm4
   "add",            // ADD8MemImm4 (AMD64)
   "add",            // ADD4MemImms
   "add",            // ADD8MemImms (AMD64)
   "add",            // ADD1RegReg
   "add",            // ADD2RegReg
   "add",            // ADD4RegReg
   "add",            // ADD8RegReg (AMD64)
   "add",            // ADD1RegMem
   "add",            // ADD2RegMem
   "add",            // ADD4RegMem
   "add",            // ADD8RegMem (AMD64)
   "add",            // ADD1MemReg
   "add",            // ADD2MemReg
   "add",            // ADD4MemReg
   "add",            // ADD8MemReg (AMD64)
   "addss",          // ADDSSRegReg
   "addss",          // ADDSSRegMem
   "addps",          // ADDPSRegReg
   "addps",          // ADDPSRegMem
   "addsd",          // ADDSDRegReg
   "addsd",          // ADDSDRegMem
   "addpd",          // ADDPDRegReg
   "addpd",          // ADDPDRegMem
   "lock add",       // LADD1MemReg
   "lock add",       // LADD2MemReg
   "lock add",       // LADD4MemReg
   "lock add",       // LADD8MemReg (AMD64)
   "lock xadd",      // LXADD1MemReg
   "lock xadd",      // LXADD2MemReg
   "lock xadd",      // LXADD4MemReg
   "lock xadd",      // LXADD8MemReg (AMD64)
   "and",            // AND1AccImm1
   "and",            // AND2AccImm2
   "and",            // AND4AccImm4
   "and",            // AND8AccImm4 (AMD64)
   "and",            // AND1RegImm1
   "and",            // AND2RegImm2
   "and",            // AND2RegImms
   "and",            // AND4RegImm4
   "and",            // AND8RegImm4 (AMD64)
   "and",            // AND4RegImms
   "and",            // AND8RegImms (AMD64)
   "and",            // AND1MemImm1
   "and",            // AND2MemImm2
   "and",            // AND2MemImms
   "and",            // AND4MemImm4
   "and",            // AND8MemImm4 (AMD64)
   "and",            // AND4MemImms
   "and",            // AND8MemImms (AMD64)
   "and",            // AND1RegReg
   "and",            // AND2RegReg
   "and",            // AND4RegReg
   "and",            // AND8RegReg (AMD64)
   "and",            // AND1RegMem
   "and",            // AND2RegMem
   "and",            // AND4RegMem
   "and",            // AND8RegMem (AMD64)
   "and",            // AND1MemReg
   "and",            // AND2MemReg
   "and",            // AND4MemReg
   "and",            // AND8MemReg (AMD64)
   "bsf",            // BSF2RegReg
   "bsf",            // BSF4RegReg
   "bsf",            // BSF8RegReg
   "bsr",            // BSR4RegReg
   "bsr",            // BSR8RegReg
   "bswap",          // BSWAP4Reg
   "bswap",          // BSWAP8Reg (AMD64)
   "bts",            // BTS4RegReg
   "bts",            // BTS4MemReg
   "call",           // CALLImm4
   "call",           // CALLREXImm4 (AMD64)
   "call",           // CALLReg
   "call",           // CALLREXReg (AMD64)
   "call",           // CALLMem
   "call",           // CALLREXMem (AMD64)
   "cbw",            // CBWAcc
   "cbwe",           // CBWEAcc
   "cmova",          // CMOVA4RegMem
   "cmovb",          // CMOVB4RegMem
   "cmove",          // CMOVE4RegMem
   "cmove",          // CMOVE8RegMem (64-bit)
   "cmovg",          // CMOVG4RegMem
   "cmovl",          // CMOVL4RegMem
   "cmovne",         // CMOVNE4RegMem
   "cmovne",         // CMOVNE8RegMem (64-bit)
   "cmovno",         // CMOVNO4RegMem
   "cmovns",         // CMOVNS4RegMem
   "cmovo",          // CMOVO4RegMem
   "cmovs",          // CMOVS4RegMem
   "cmp",            // CMP1AccImm1
   "cmp",            // CMP2AccImm2
   "cmp",            // CMP4AccImm4
   "cmp",            // CMP8AccImm4 (AMD64)
   "cmp",            // CMP1RegImm1
   "cmp",            // CMP2RegImm2
   "cmp",            // CMP2RegImms
   "cmp",            // CMP4RegImm4
   "cmp",            // CMP8RegImm4 (AMD64)
   "cmp",            // CMP4RegImms
   "cmp",            // CMP8RegImms (AMD64)
   "cmp",            // CMP1MemImm1
   "cmp",            // CMP2MemImm2
   "cmp",            // CMP2MemImms
   "cmp",            // CMP4MemImm4
   "cmp",            // CMP8MemImm4 (AMD64)
   "cmp",            // CMP4MemImms
   "cmp",            // CMP8MemImms (AMD64)
   "cmp",            // CMP1RegReg
   "cmp",            // CMP2RegReg
   "cmp",            // CMP4RegReg
   "cmp",            // CMP8RegReg (AMD64)
   "cmp",            // CMP1RegMem
   "cmp",            // CMP2RegMem
   "cmp",            // CMP4RegMem
   "cmp",            // CMP8RegMem (AMD64)
   "cmp",            // CMP1MemReg
   "cmp",            // CMP2MemReg
   "cmp",            // CMP4MemReg
   "cmp",            // CMP8MemReg (AMD64)
   "cmpxchg",        // CMPXCHG1MemReg
   "cmpxchg",        // CMPXCHG2MemReg
   "cmpxchg",        // CMPXCHG4MemReg
   "cmpxchg",        // CMPXCHG8MemReg (AMD64)
   "cmpxchg8b",      // CMPXCHG8MemReg (IA32 only)
   "cmpxchg16b",      // CMPXCHG8MemReg (IA32 only)
   "lock cmpxchg",   // LCMPXCHG1MemReg
   "lock cmpxchg",   // LCMPXCHG2MemReg
   "lock cmpxchg",   // LCMPXCHG4MemReg
   "lock cmpxchg",   // LCMPXCHG8MemReg (AMD64)
   "lock cmpxchg8b", // LCMPXCHG8BMem (IA32 only)
   "lock cmpxchg16b", // LCMPXCHG16MemReg (IA32 only)
   "xacquire lock cmpxchg",   // XALCMPXCHG8MemReg
   "xacquire lock cmpxchg",// XACMPXCHG8MemReg
   "xacquire lock cmpxchg",// XALCMPXCHG4MemReg
   "xacquire lock cmpxchg",// XACMPXCHG4MemReg
   "cvtsi2ss",       // CVTSI2SSRegReg4
   "cvtsi2ss",       // CVTSI2SSRegReg8 (AMD64)
   "cvtsi2ss",       // CVTSI2SSRegMem
   "cvtsi2ss",       // CVTSI2SSRegMem8 (AMD64)
   "cvtsi2sd",       // CVTSI2SDRegReg4
   "cvtsi2sd",       // CVTSI2SDRegReg8 (AMD64)
   "cvtsi2sd",       // CVTSI2SDRegMem
   "cvtsi2sd",       // CVTSI2SDRegMem8 (AMD64)
   "cvttss2si",      // CVTTSS2SIReg4Reg
   "cvttss2si",      // CVTTSS2SIReg8Reg (AMD64)
   "cvttss2si",      // CVTTSS2SIReg4Mem
   "cvttss2si",      // CVTTSS2SIReg8Mem (AMD64)
   "cvttsd2si",      // CVTTSD2SIReg4Reg
   "cvttsd2si",      // CVTTSD2SIReg8Reg (AMD64)
   "cvttsd2si",      // CVTTSD2SIReg4Mem
   "cvttsd2si",      // CVTTSD2SIReg8Mem (AMD64)
   "cvtss2sd",       // CVTSS2SDRegReg
   "cvtss2sd",       // CVTSS2SDRegMem
   "cvtsd2ss",       // CVTSD2SSRegReg
   "cvtsd2ss",       // CVTSD2SSRegMem
   "cwd",            // CWDAcc
   "cdq",            // CDQAcc
   "cqo",            // CQOAcc (AMD64)
   "dec",            // DEC1Reg
   "dec",            // DEC2Reg
   "dec",            // DEC4Reg
   "dec",            // DEC8Reg (AMD64)
   "dec",            // DEC1Mem
   "dec",            // DEC2Mem
   "dec",            // DEC4Mem
   "dec",            // DEC8Mem (AMD64)
   "fabs",           // FABSReg
   "fabs",           // DABSReg
   "fsqrt",          // FSQRTReg
   "fsqrt",          // DSQRTReg
   "fadd",           // FADDRegReg
   "fadd",           // DADDRegReg
   "faddp",          // FADDPReg
   "fadd",           // FADDRegMem
   "fadd",           // DADDRegMem
   "fiadd",          // FIADDRegMem
   "fiadd",          // DIADDRegMem
   "fiadd",          // FSADDRegMem
   "fiadd",          // DSADDRegMem
   "fchs",           // FCHSReg
   "fchs",           // DCHSReg
   "fdiv",           // FDIVRegReg
   "fdiv",           // DDIVRegReg
   "fdiv",           // FDIVRegMem
   "fdiv",           // DDIVRegMem
   "fdivp",          // FDIVPReg
   "fidiv",          // FIDIVRegMem
   "fidiv",          // DIDIVRegMem
   "fidiv",          // FSDIVRegMem
   "fidiv",          // DSDIVRegMem
   "fdivr",          // FDIVRRegReg
   "fdivr",          // DDIVRRegReg
   "fdivr",          // FDIVRRegMem
   "fdivr",          // DDIVRRegMem
   "fdivrp",         // FDIVRPReg
   "fidivr",         // FIDIVRRegMem
   "fidivr",         // DIDIVRRegMem
   "fidivr",         // FSDIVRRegMem
   "fidivr",         // DSDIVRRegMem
   "fild",           // FILDRegMem
   "fild",           // DILDRegMem
   "fild",           // FLLDRegMem
   "fild",           // DLLDRegMem
   "fild",           // FSLDRegMem
   "fild",           // DSLDRegMem
   "fist",           // FISTMemReg
   "fist",           // DISTMemReg
   "fistp",          // FISTPMem
   "fistp",          // DISTPMem
   "flstp",          // FLSTPMem
   "flstp",          // DLSTPMem
   "fist",           // FSSTMemReg
   "fist",           // DSSTMemReg
   "fistp",          // FSSTPMem
   "fistp",          // DSSTPMem
   "fldln2",         // FLDLN2
   "fld",            // FLDRegReg
   "fld",            // DLDRegReg
   "fld",            // FLDRegMem
   "fld",            // DLDRegMem
   "fldz",           // FLD0Reg
   "fldz",           // DLD0Reg
   "fld1",           // FLD1Reg
   "fld1",           // DLD1Reg
   "fld",            // FLDMem
   "fld",            // DLDMem
   "fldcw",          // LDCWMem
   "fmul",           // FMULRegReg
   "fmul",           // DMULRegReg
   "fmulp",          // FMULPReg
   "fmul",           // FMULRegMem
   "fmul",           // DMULRegMem
   "fimul",          // FIMULRegMem
   "fimul",          // DIMULRegMem
   "fimul",          // FSMULRegMem
   "fimul",          // DSMULRegMem
   "fnclex",         // FNCLEX
   "fprem",          // FPREMRegReg
   "fscale",         // FSCALERegReg
   "fst",            // FSTMemReg
   "fst",            // DSTMemReg
   "fst",            // FSTRegReg
   "fst",            // DSTRegReg
   "fstp",           // FSTPMemReg
   "fstp",           // DSTPMemReg
   "fstp",           // FSTPReg
   "fstp",           // DSTPReg
   "fnstcw",         // STCWMem
   "fnstsw",         // STSWMem
   "fnstsw",         // STSWAcc
   "fsub",           // FSUBRegReg
   "fsub",           // DSUBRegReg
   "fsub",           // FSUBRegMem
   "fsub",           // DSUBRegMem
   "fsubp",          // FSUBPReg
   "fisub",          // FISUBRegMem
   "fisub",          // DISUBRegMem
   "fisub",          // FSSUBRegMem
   "fisub",          // DSSUBRegMem
   "fsubr",          // FSUBRRegReg
   "fsubr",          // DSUBRRegReg
   "fsubr",          // FSUBRRegMem
   "fsubr",          // DSUBRRegMem
   "fsubrp",         // FSUBRPReg
   "fisubr",         // FISUBRRegMem
   "fisubr",         // DISUBRRegMem
   "fisubr",         // FSSUBRRegMem
   "fisubr",         // DSSUBRRegMem
   "ftst",           // FTSTReg
   "fcom",           // FCOMRegReg
   "fcom",           // DCOMRegReg
   "fcom",           // FCOMRegMem
   "fcom",           // DCOMRegMem
   "fcomp",          // FCOMPReg
   "fcomp",          // FCOMPMem
   "fcomp",          // DCOMPMem
   "fcompp",         // FCOMPP
   "fcomi",          // FCOMIRegReg
   "fcomi",          // DCOMIRegReg
   "fcomip",         // FCOMIPReg
   "fyl2x",          // FYL2X
   "ucomiss",        // UCOMISSRegReg
   "ucomiss",        // UCOMISSRegMem
   "ucomisd",        // UCOMISDRegReg
   "ucomisd",        // UCOMISDRegMem
   "fxch",           // FXCHReg
   "idiv",           // IDIV1AccReg
   "idiv",           // IDIV2AccReg
   "idiv",           // IDIV4AccReg
   "idiv",           // IDIV8AccReg (AMD64)
   "div",            // DIV4AccMem
   "div",            // DIV8AccMem (AMD64)
   "idiv",           // IDIV1AccMem
   "idiv",           // IDIV2AccMem
   "idiv",           // IDIV4AccMem
   "idiv",           // IDIV8AccMem (AMD64)
   "div",            // DIV4AccMem
   "div",            // DIV8AccMem (AMD64)
   "divss",          // DIVSSRegReg
   "divss",          // DIVSSRegMem
   "divsd",          // DIVSDRegReg
   "divsd",          // DIVSDRegMem
   "imul",           // IMUL1AccReg
   "imul",           // IMUL2AccReg
   "imul",           // IMUL4AccReg
   "imul",           // IMUL8AccReg (AMD64)
   "imul",           // IMUL1AccMem
   "imul",           // IMUL2AccMem
   "imul",           // IMUL4AccMem
   "imul",           // IMUL8AccMem (AMD64)
   "imul",           // IMUL2RegReg
   "imul",           // IMUL4RegReg
   "imul",           // IMUL8RegReg (AMD64)
   "imul",           // IMUL2RegMem
   "imul",           // IMUL4RegMem
   "imul",           // IMUL8RegMem (AMD64)
   "imul",           // IMUL2RegRegImm2
   "imul",           // IMUL2RegRegImms
   "imul",           // IMUL4RegRegImm4
   "imul",           // IMUL8RegRegImm4 (AMD64)
   "imul",           // IMUL4RegRegImms
   "imul",           // IMUL8RegRegImms (AMD64)
   "imul",           // IMUL2RegMemImm2
   "imul",           // IMUL2RegMemImms
   "imul",           // IMUL4RegMemImm4
   "imul",           // IMUL8RegMemImm4 (AMD64)
   "imul",           // IMUL4RegMemImms
   "imul",           // IMUL8RegMemImms (AMD64)
   "mul",            // MUL1AccReg
   "mul",            // MUL2AccReg
   "mul",            // MUL4AccReg
   "mul",            // MUL8AccReg (AMD64)
   "mul",            // MUL1AccMem
   "mul",            // MUL2AccMem
   "mul",            // MUL4AccMem
   "mul",            // MUL8AccMem (AMD64)
   "mulss",          // MULSSRegReg
   "mulss",          // MULSSRegMem
   "mulps",          // MULPSRegReg
   "mulps",          // MULPSRegMem
   "mulsd",          // MULSDRegReg
   "mulsd",          // MULSDRegMem
   "mulpd",          // MULPDRegReg
   "mulpd",          // MULPDRegMem
   "inc",            // INC1Reg
   "inc",            // INC2Reg
   "inc",            // INC4Reg
   "inc",            // INC8Reg (AMD64)
   "inc",            // INC1Mem
   "inc",            // INC2Mem
   "inc",            // INC4Mem
   "inc",            // INC8Mem (AMD64)
   "ja",             // JA1
   "jae",            // JAE1
   "jb",             // JB1
   "jbe",            // JBE1
   "je",             // JE1
   "jne",            // JNE1
   "jg",             // JG1
   "jge",            // JGE1
   "jl",             // JL1
   "jle",            // JLE1
   "jo",             // JO1
   "jno",            // JNO1
   "js",             // JS1
   "jns",            // JNS1
   "jpo",            // JPO1
   "jpe",            // JPE1
   "jmp",            // JMP1
   "ja",             // JA4
   "jae",            // JAE4
   "jb",             // JB4
   "jbe",            // JBE4
   "je",             // JE4
   "jne",            // JNE4
   "jg",             // JG4
   "jge",            // JGE4
   "jl",             // JL4
   "jle",            // JLE4
   "jo",             // JO4
   "jno",            // JNO4
   "js",             // JS4
   "jns",            // JNS4
   "jpo",            // JPO4
   "jpe",            // JPE4
   "jmp",            // JMP4
   "jmp",            // JMPReg
   "jmp",            // JMPMem
   "jrcxz",          // JRCXZ1
   "loop",           // LOOP1
   "lahf",           // LAHF
   "lddqu",          // LDDQU
   "lea",            // LEA2RegMem
   "lea",            // LEA4RegMem
   "lea",            // LEA8RegMem (AMD64)
   "mov",            // S1MemReg
   "mov",            // S2MemReg
   "mov",            // S4MemReg
   "mov",            // S8MemReg (AMD64)
   "mov",            // S1MemImm1
   "mov",            // S2MemImm2
   "mov",            // S4MemImm4
   "xrelease mov",   // XRS4MemImm4
   "xrelease mov",   // S8MemImm4 (AMD64)
   "mov",            // XRS8MemImm4 (AMD64)
   "mov",            // L1RegMem
   "mov",            // L2RegMem
   "mov",            // L4RegMem
   "mov",            // L8RegMem (AMD64)
   "movaps",         // MOVAPSRegReg
   "movaps",         // MOVAPSRegMem
   "movaps",         // MOVAPSMemReg
   "movapd",         // MOVAPDRegReg
   "movapd",         // MOVAPDRegMem
   "movapd",         // MOVAPDMemReg
   "movups",         // MOVUPSRegReg
   "movups",         // MOVUPSRegMem
   "movups",         // MOVUPSMemReg
   "movupd",         // MOVUPDRegReg
   "movupd",         // MOVUPDRegMem
   "movupd",         // MOVUPDMemReg
   "movss",          // MOVSSRegReg
   "movss",          // MOVSSRegMem
   "movss",          // MOVSSMemReg
   "movsd",          // MOVSDRegReg
   "movsd",          // MOVSDRegMem
   "movsd",          // MOVSDMemReg
   "sqrtsf",         // SQRTSFRegReg
   "sqrtsd",         // SQRTSDRegReg
   "movd",           // MOVDRegReg4
   "movq",           // MOVQRegReg8 (AMD64)
   "movd",           // MOVDReg4Reg
   "movq",           // MOVQReg8Reg (AMD64)
   "movdqu",         // MOVDQURegReg
   "movdqu",         // MOVDQURegMem
   "movdqu",         // MOVDQUMemReg
   "mov",            // MOV1RegReg
   "mov",            // MOV2RegReg
   "mov",            // MOV4RegReg
   "mov",            // MOV8RegReg (AMD64)
   "cmovg",          // CMOVG4RegReg
   "cmovg",          // CMOVG8RegReg (AMD64)
   "cmovl",          // CMOVG4RegReg
   "cmovl",          // CMOVG8RegReg (AMD64)
   "cmove",          // CMOVE4RegReg
   "cmove",          // CMOVE8RegReg (AMD64)
   "cmovne",         // CMOVNE4RegReg
   "cmovne",         // CMOVNE8RegReg (AMD64)
   "mov",            // MOV1RegImm1
   "mov",            // MOV2RegImm2
   "mov",            // MOV4RegImm4
   "mov",            // MOV8RegImm4 (AMD64)
   "mov",            // MOV8RegImm64 (AMD64)
   "movlpd",         // MOVLPDRegMem
   "movlpd",         // MOVLPDMemReg
   "movq",           // MOVQRegMem
   "movq",           // MOVQMemReg
   "movsb",          // MOVSB
   "movsw",          // MOVSW
   "movsd",          // MOVSD
   "movsd",          // MOVSQ (AMD64)
   "movsx",          // MOVSXReg2Reg1
   "movsx",          // MOVSXReg4Reg1
   "movsx",          // MOVSXReg8Reg1 (AMD64)
   "movsx",          // MOVSXReg4Reg2
   "movsx",          // MOVSXReg8Reg2 (AMD64)
   "movsxd",         // MOVSXReg8Reg4 (AMD64)
   "movsx",          // MOVSXReg2Mem1
   "movsx",          // MOVSXReg4Mem1
   "movsx",          // MOVSXReg8Mem1 (AMD64)
   "movsx",          // MOVSXReg4Mem2
   "movsx",          // MOVSXReg8Mem2 (AMD64)
   "movsxd",         // MOVSXReg8Mem4 (AMD64)
   "movzx",          // MOVZXReg2Reg1
   "movzx",          // MOVZXReg4Reg1
   "movzx",          // MOVZXReg8Reg1 (AMD64)
   "movzx",          // MOVZXReg4Reg2
   "movzx",          // MOVZXReg8Reg2 (AMD64)
   "mov",            // MOVZXReg8Reg4 (AMD64)
   "movzx",          // MOVZXReg2Mem1
   "movzx",          // MOVZXReg4Mem1
   "movzx",          // MOVZXReg8Mem1 (AMD64)
   "movzx",          // MOVZXReg4Mem2
   "movzx",          // MOVZXReg8Mem2 (AMD64)
   "neg",            // NEG1Reg
   "neg",            // NEG2Reg
   "neg",            // NEG4Reg
   "neg",            // NEG8Reg (AMD64)
   "neg",            // NEG1Mem
   "neg",            // NEG2Mem
   "neg",            // NEG4Mem
   "neg",            // NEG8Mem (AMD64)
   "not",            // NOT1Reg
   "not",            // NOT2Reg
   "not",            // NOT4Reg
   "not",            // NOT8Reg (AMD64)
   "not",            // NOT1Mem
   "not",            // NOT2Mem
   "not",            // NOT4Mem
   "not",            // NOT8Mem (AMD64)
   "or",             // OR1AccImm1
   "or",             // OR2AccImm2
   "or",             // OR4AccImm4
   "or",             // OR8AccImm4 (AMD64)
   "or",             // OR1RegImm1
   "or",             // OR2RegImm2
   "or",             // OR2RegImms
   "or",             // OR4RegImm4
   "or",             // OR8RegImm4 (AMD64)
   "or",             // OR4RegImms
   "or",             // OR8RegImms (AMD64)
   "or",             // OR1MemImm1
   "or",             // OR2MemImm2
   "or",             // OR2MemImms
   "or",             // OR4MemImm4
   "or",             // OR8MemImm4 (AMD64)
   "or",             // OR4MemImms
   "lock or",        // LOR4MemImms
   "lock or",        // LOR4MemReg
   "lock or",        // LOR8MemReg (AMD64)
   "or",             // OR8MemImms (AMD64)
   "or",             // OR1RegReg
   "or",             // OR2RegReg
   "or",             // OR4RegReg
   "or",             // OR8RegReg (AMD64)
   "or",             // OR1RegMem
   "or",             // OR2RegMem
   "or",             // OR4RegMem
   "or",             // OR8RegMem (AMD64)
   "or",             // OR1MemReg
   "or",             // OR2MemReg
   "or",             // OR4MemReg
   "or",             // OR8MemReg (AMD64)
   "pause",          // PAUSE
   "pcmpeqb",        // PCMPEQBRegReg
   "pmovmskb",       // PMOVMSKB4RegReg
   "pmovzxwd",       // PMOVZXWD
   "pmulld",         // PMULLD
   "paddd",          // PADDD
   "pshufd",         // PSHUFDRegRegImm1
   "pshufd",         // PSHUFDRegMemImm1
   "psrldq",         // PSRLDQRegImm1
   "pmovzx",         // PMOVZXxmm18Reg
   "popcnt",         // POPCNT4RegReg
   "popcnt",         // POPCNT8RegReg
   "pop",            // POPReg
   "pop",            // POPMem
   "push",           // PUSHImms
   "push",           // PUSHImm4
   "push",           // PUSHReg
   "push",           // PUSHRegLong
   "push",           // PUSHMem
   "rcl",            // RCL1RegImm1
   "rcl",            // RCL4RegImm1
   "rcr",            // RCR1RegImm1
   "rcr",            // RCR4RegImm1
   "rep movsb",      // REPMOVSB
   "rep movsw",      // REPMOVSW
   "rep movsd",      // REPMOVSD
   "rep movsq",      // REPMOVSQ
   "rep stosb",      // REPSTOSB
   "rep stosw",      // REPSTOSW
   "rep stosd",      // REPSTOSD
   "rep stosq",      // REPSTOSQ (AMD64)
   "ret",            // RET
   "ret",            // RETImm2
   "rol",            // ROL1RegImm1
   "rol",            // ROL2RegImm1
   "rol",            // ROL4RegImm1
   "rol",            // ROL8RegImm1 (AMD64)
   "rol",            // ROL4RegCL
   "rol",            // ROL8RegCL (AMD64)
   "ror",            // ROR1RegImm1
   "ror",            // ROR2RegImm1
   "ror",            // ROR4RegImm1
   "ror",            // ROR8RegImm1 (AMD64)
   "sahf",           // SAHF
   "shl",            // SHL1RegImm1
   "shl",            // SHL1RegCL
   "shl",            // SHL2RegImm1
   "shl",            // SHL2RegCL
   "shl",            // SHL4RegImm1
   "shl",            // SHL8RegImm1 (AMD64)
   "shl",            // SHL4RegCL
   "shl",            // SHL8RegCL (AMD64)
   "shl",            // SHL1MemImm1
   "shl",            // SHL1MemCL
   "shl",            // SHL2MemImm1
   "shl",            // SHL2MemCL
   "shl",            // SHL4MemImm1
   "shl",            // SHL8MemImm1 (AMD64)
   "shl",            // SHL4MemCL
   "shl",            // SHL8MemCL (AMD64)
   "shr",            // SHR1RegImm1
   "shr",            // SHR1RegCL
   "shr",            // SHR2RegImm1
   "shr",            // SHR2RegCL
   "shr",            // SHR4RegImm1
   "shr",            // SHR8RegImm1 (AMD64)
   "shr",            // SHR4RegCL
   "shr",            // SHR8RegCL (AMD64)
   "shr",            // SHR1MemImm1
   "shr",            // SHR1MemCL
   "shr",            // SHR2MemImm1
   "shr",            // SHR2MemCL
   "shr",            // SHR4MemImm1
   "shr",            // SHR8MemImm1 (AMD64)
   "shr",            // SHR4MemCL
   "shr",            // SHR8MemCL (AMD64)
   "sar",            // SAR1RegImm1
   "sar",            // SAR1RegCL
   "sar",            // SAR2RegImm1
   "sar",            // SAR2RegCL
   "sar",            // SAR4RegImm1
   "sar",            // SAR8RegImm1 (AMD64)
   "sar",            // SAR4RegCL
   "sar",            // SAR8RegCL (AMD64)
   "sar",            // SAR1MemImm1
   "sar",            // SAR1MemCL
   "sar",            // SAR2MemImm1
   "sar",            // SAR2MemCL
   "sar",            // SAR4MemImm1
   "sar",            // SAR8MemImm1 (AMD64)
   "sar",            // SAR4MemCL
   "sar",            // SAR8MemCL (AMD64)
   "sbb",            // SBB1AccImm1
   "sbb",            // SBB2AccImm2
   "sbb",            // SBB4AccImm4
   "sbb",            // SBB8AccImm4 (AMD64)
   "sbb",            // SBB1RegImm1
   "sbb",            // SBB2RegImm2
   "sbb",            // SBB2RegImms
   "sbb",            // SBB4RegImm4
   "sbb",            // SBB8RegImm4 (AMD64)
   "sbb",            // SBB4RegImms
   "sbb",            // SBB8RegImms (AMD64)
   "sbb",            // SBB1MemImm1
   "sbb",            // SBB2MemImm2
   "sbb",            // SBB2MemImms
   "sbb",            // SBB4MemImm4
   "sbb",            // SBB8MemImm4 (AMD64)
   "sbb",            // SBB4MemImms
   "sbb",            // SBB8MemImms (AMD64)
   "sbb",            // SBB1RegReg
   "sbb",            // SBB2RegReg
   "sbb",            // SBB4RegReg
   "sbb",            // SBB8RegReg (AMD64)
   "sbb",            // SBB1RegMem
   "sbb",            // SBB2RegMem
   "sbb",            // SBB4RegMem
   "sbb",            // SBB8RegMem (AMD64)
   "sbb",            // SBB1MemReg
   "sbb",            // SBB2MemReg
   "sbb",            // SBB4MemReg
   "sbb",            // SBB8MemReg (AMD64)
   "seta",           // SETA1Reg
   "setae",          // SETAE1Reg
   "setb",           // SETB1Reg
   "setbe",          // SETBE1Reg
   "sete",           // SETE1Reg
   "setne",          // SETNE1Reg
   "setg",           // SETG1Reg
   "setge",          // SETGE1Reg
   "setl",           // SETL1Reg
   "setle",          // SETLE1Reg
   "sets",           // SETS1Reg
   "setns",          // SETNS1Reg
   "setpo",          // SETPO1Reg
   "setpe",          // SETPE1Reg
   "seta",           // SETA1Mem
   "setae",          // SETAE1Mem
   "setb",           // SETB1Mem
   "setbe",          // SETBE1Mem
   "sete",           // SETE1Mem
   "setne",          // SETNE1Mem
   "setg",           // SETG1Mem
   "setge",          // SETGE1Mem
   "setl",           // SETL1Mem
   "setle",          // SETLE1Mem
   "sets",           // SETS1Mem
   "setns",          // SETNS1Mem
   "shld",           // SHLD4RegRegImm1
   "shld",           // SHLD4RegRegCL
   "shld",           // SHLD4MemRegImm1
   "shld",           // SHLD4MemRegCL
   "shrd",           // SHRD4RegRegImm1
   "shrd",           // SHRD4RegRegCL
   "shrd",           // SHRD4MemRegImm1
   "shrd",           // SHRD4MemRegCL
   "stosb",          // STOSB
   "stosw",          // STOSW
   "stosd",          // STOSD
   "stosd",          // STOSQ (AMD64)
   "sub",            // SUB1AccImm1
   "sub",            // SUB2AccImm2
   "sub",            // SUB4AccImm4
   "sub",            // SUB8AccImm4 (AMD64)
   "sub",            // SUB1RegImm1
   "sub",            // SUB2RegImm2
   "sub",            // SUB2RegImms
   "sub",            // SUB4RegImm4
   "sub",            // SUB8RegImm4 (AMD64)
   "sub",            // SUB4RegImms
   "sub",            // SUB8RegImms (AMD64)
   "sub",            // SUB1MemImm1
   "sub",            // SUB2MemImm2
   "sub",            // SUB2MemImms
   "sub",            // SUB4MemImm4
   "sub",            // SUB8MemImm4 (AMD64)
   "sub",            // SUB4MemImms
   "sub",            // SUB8MemImms (AMD64)
   "sub",            // SUB1RegReg
   "sub",            // SUB2RegReg
   "sub",            // SUB4RegReg
   "sub",            // SUB8RegReg (AMD64)
   "sub",            // SUB1RegMem
   "sub",            // SUB2RegMem
   "sub",            // SUB4RegMem
   "sub",            // SUB8RegMem (AMD64)
   "sub",            // SUB1MemReg
   "sub",            // SUB2MemReg
   "sub",            // SUB4MemReg
   "sub",            // SUB8MemReg (AMD64)
   "subss",          // SUBSSRegReg
   "subss",          // SUBSSRegMem
   "subsd",          // SUBSDRegReg
   "subsd",          // SUBSDRegMem
   "test",           // TEST1AccImm1
   "test",           // TEST1AccHImm1
   "test",           // TEST2AccImm2
   "test",           // TEST4AccImm4
   "test",           // TEST8AccImm4 (AMD64)
   "test",           // TEST1RegImm1
   "test",           // TEST2RegImm2
   "test",           // TEST4RegImm4
   "test",           // TEST8RegImm4 (AMD64)
   "test",           // TEST1MemImm1
   "test",           // TEST2MemImm2
   "test",           // TEST4MemImm4
   "test",           // TEST8MemImm4 (AMD64)
   "test",           // TEST1RegReg
   "test",           // TEST2RegReg
   "test",           // TEST4RegReg
   "test",           // TEST8RegReg (AMD64)
   "test",           // TEST1MemReg
   "test",           // TEST2MemReg
   "test",           // TEST4MemReg
   "test",           // TEST8MemReg (AMD64)
   "xabort",         // XABORT
   "xbegin",         // XBEGIN2
   "xbegin",         // XBEGIN4
   "xchg",           // XCHG2AccReg
   "xchg",           // XCHG4AccReg
   "xchg",           // XCHG8AccReg (AMD64)
   "xchg",           // XCHG1RegReg
   "xchg",           // XCHG2RegReg
   "xchg",           // XCHG4RegReg
   "xchg",           // XCHG8RegReg (AMD64)
   "xchg",           // XCHG1RegMem
   "xchg",           // XCHG2RegMem
   "xchg",           // XCHG4RegMem
   "xchg",           // XCHG8RegMem (AMD64)
   "xchg",           // XCHG1MemReg
   "xchg",           // XCHG2MemReg
   "xchg",           // XCHG4MemReg
   "xchg",           // XCHG8MemReg (AMD64)
   "xend",           // XEND
   "xor",            // XOR1AccImm1
   "xor",            // XOR2AccImm2
   "xor",            // XOR4AccImm4
   "xor",            // XOR8AccImm4 (AMD64)
   "xor",            // XOR1RegImm1
   "xor",            // XOR2RegImm2
   "xor",            // XOR2RegImms
   "xor",            // XOR4RegImm4
   "xor",            // XOR8RegImm4 (AMD64)
   "xor",            // XOR4RegImms
   "xor",            // XOR8RegImms (AMD64)
   "xor",            // XOR1MemImm1
   "xor",            // XOR2MemImm2
   "xor",            // XOR2MemImms
   "xor",            // XOR4MemImm4
   "xor",            // XOR8MemImm4 (AMD64)
   "xor",            // XOR4MemImms
   "xor",            // XOR8MemImms (AMD64)
   "xor",            // XOR1RegReg
   "xor",            // XOR2RegReg
   "xor",            // XOR4RegReg
   "xor",            // XOR8RegReg (AMD64)
   "xor",            // XOR1RegMem
   "xor",            // XOR2RegMem
   "xor",            // XOR4RegMem
   "xor",            // XOR8RegMem (AMD64)
   "xor",            // XOR1MemReg
   "xor",            // XOR2MemReg
   "xor",            // XOR4MemReg
   "xor",            // XOR8MemReg (AMD64)
   "xorps",          // XORPSRegReg
   "xorps",          // XORPSRegMem
   "xorpd",          // XORPDRegReg
   "mfence",         // MFENCE
   "lfence",         // LFENCE
   "sfence",         // SFENCE
   "pcmpestri",      // PCMPESTRI
   "prefetchnta",    // PREFETCHNTA
   "prefetcht0",     // PREFETCHT0
   "prefetcht1",     // PREFETCHT1
   "prefetcht2",     // PREFETCHT2
   "andnps",         // ANDNPSRegReg
   "andnpd",         // ANDNPDRegReg
   "psllq",          // PSLLQRegImm1
   "psrlq",          // PSRLQRegImm1
   "Fence",          // FENCE
   "VGFence",        // VGFENCE
   "ProcEntry",      // PROCENTRY
   "dq",             // DQImm64 (AMD64)
   "dd",             // DDImm4
   "dw",             // DWImm2
   "db",             // DBImm1
   "wrtbar",         // WRTBAR
   "assocRegs",      // ASSOCREGS
   "fpRegSpill",     // FPREGSPILL
   "vgnop",          // VirtualGuardNOP
   "Label",          // LABEL
   "fcmpEval",       // FCMPEVAL
   "RestoreVMThread",// RestoreVMThread
   "PPS_OPCount",    // PPS_OPCOUNT
   "PPS_OPField",    // PPS_OPFIELD
   "AdjustFramePtr", // AdjustFramePtr
   "ReturnMarker"    // ReturnMarker
   };

const char *
TR_Debug::getMnemonicName(TR_X86OpCode  * opCode)
   {
   if (TR::Compiler->target.isLinux())
      {
      int32_t o = opCode->getOpCodeValue();
      if (o == (int32_t) DQImm64) return dqString();
      if (o == (int32_t) DDImm4) return ddString();
      if (o == (int32_t) DWImm2) return dwString();
      if (o == (int32_t) DBImm1) return dbString();
      }
   return opCodeToMnemonicMap[opCode->getOpCodeValue()];
   }
