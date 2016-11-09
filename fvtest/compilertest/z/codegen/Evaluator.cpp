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
#include "env/ConcreteFE.hpp"
#include "z/codegen/S390GenerateInstructions.hpp"
#include "z/codegen/TRSystemLinkage.hpp"
#include "codegen/S390Snippets.hpp"
#include "il/Node.hpp"
#include "il/Node_inlines.hpp"
#include "env/IO.hpp"


#define NOT_IMPLEMENTED { TR_ASSERT_FATAL(0, "This function is not implemented in TestCompiler JIT"); }


uint32_t
TR::S390RestoreGPR7Snippet::getLength(int32_t estimatedSnippetStart)
   {
   NOT_IMPLEMENTED;
   return 0;
   }

uint8_t *
TR::S390RestoreGPR7Snippet::emitSnippetBody()
   {
   NOT_IMPLEMENTED;
   return NULL;
   }

void
TR_Debug::print(TR::FILE *pOutFile, TR::S390RestoreGPR7Snippet *snippet)
   {
   NOT_IMPLEMENTED;
   }

void
TestCompiler::FrontEnd::generateBinaryEncodingPrologue(
      TR_BinaryEncodingData *beData,
      TR::CodeGenerator *cg)
   {
   TR::Compilation* comp = cg->comp();
   TR_S390BinaryEncodingData *data = (TR_S390BinaryEncodingData *)beData;

   data->cursorInstruction = comp->getFirstInstruction();
   data->estimate = 0;
   data->preProcInstruction = data->cursorInstruction;
   data->jitTojitStart = data->cursorInstruction;
   data->cursorInstruction = NULL;

   TR::Instruction * preLoadArgs, * endLoadArgs;
   preLoadArgs = data->preProcInstruction;
   endLoadArgs = preLoadArgs;

   TR::Instruction * oldFirstInstruction = data->cursorInstruction;

   data->cursorInstruction = comp->getFirstInstruction();

   static char *disableAlignJITEP = feGetEnv("TR_DisableAlignJITEP");

   // Padding for JIT Entry Point
   if (!disableAlignJITEP)
      {
      data->estimate += 256;
      }

   while (data->cursorInstruction && data->cursorInstruction->getOpCodeValue() != TR::InstOpCode::PROC)
      {
      data->estimate = data->cursorInstruction->estimateBinaryLength(data->estimate);
      data->cursorInstruction = data->cursorInstruction->getNext();
      }

   cg->getLinkage()->createPrologue(data->cursorInstruction);
   //cg->getLinkage()->analyzePrologue();
   }



