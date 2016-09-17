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

#include "codegen/CodeGenerator.hpp"
#include "codegen/Linkage.hpp"
#include "p/codegen/PPCSystemLinkage.hpp"
#include "p/codegen/SystemLinkage.hpp"
#include "codegen/Instruction.hpp"
#include "env/OMRMemory.hpp"

// Get or create the TR::Linkage object that corresponds to the given linkage
// convention.
// Even though this method is common, its implementation is machine-specific.
//

namespace Test
{
namespace Power
{

TR::Linkage *
CodeGenerator::createLinkage(TR_LinkageConventions lc)
   {
   // *this    swipeable for debugging purposes
   TR::Linkage *linkage = NULL;

   switch (lc)
      {
      case TR_System:
         linkage = new (self()->trHeapMemory()) Test::Power::SystemLinkage(self());
         break;

      default:
         linkage = new (self()->trHeapMemory()) TR_PPCSystemLinkage(self());
      }

   self()->setLinkage(lc, linkage);
   return linkage;
   }

void
CodeGenerator::generateBinaryEncodingPrologue(TR_PPCBinaryEncodingData *data)
   {
   TR::Compilation *comp = self()->comp();
   data->recomp = NULL;
   data->cursorInstruction = comp->getFirstInstruction();
   data->preProcInstruction = data->cursorInstruction;

   data->jitTojitStart = data->cursorInstruction;
   data->cursorInstruction = NULL;

   self()->getLinkage()->loadUpArguments(data->cursorInstruction);

   data->cursorInstruction = comp->getFirstInstruction();

   while (data->cursorInstruction && data->cursorInstruction->getOpCodeValue() != TR::InstOpCode::proc)
      {
      data->estimate          = data->cursorInstruction->estimateBinaryLength(data->estimate);
      data->cursorInstruction = data->cursorInstruction->getNext();
      }

   int32_t boundary = comp->getOptions()->getJitMethodEntryAlignmentBoundary(self());
   if (boundary && (boundary > 4) && ((boundary & (boundary - 1)) == 0))
      {
      comp->getOptions()->setJitMethodEntryAlignmentBoundary(boundary);
      self()->setPreJitMethodEntrySize(data->estimate);
      data->estimate += (boundary - 4);
      }

   self()->getLinkage()->createPrologue(data->cursorInstruction);

   }

} // namespace Power
} // namespace Test
