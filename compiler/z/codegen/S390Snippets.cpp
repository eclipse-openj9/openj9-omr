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

#include "codegen/Snippet.hpp"
#include "codegen/S390Snippets.hpp"
#include "codegen/CodeGenerator.hpp"
#include "env/IO.hpp"

uint32_t
TR::S390WarmToColdTrampolineSnippet::getLength(int32_t estimatedSnippetStart)
   {
   // BRCL - 6 bytes
   return 6;
   }


uint8_t *
TR::S390WarmToColdTrampolineSnippet::emitSnippetBody()
   {
   uint8_t * cursor = cg()->getBinaryBufferCursor();
   getSnippetLabel()->setCodeLocation(cursor);

   if (getIsUsed())
      {
      cg()->addRelocation(new (cg()->trHeapMemory()) TR_32BitLabelRelativeRelocation(cursor, getTargetLabel()));

      // BRCL 0xf, <target>
      *(int16_t *) cursor = 0xC0F4;
      cursor += sizeof(int16_t);

      // BRCL - Add Relocation the data constants to this LARL.
      *(int32_t *) cursor = 0xDEADBEEF;
      cursor += sizeof(int32_t);
      }
   return cursor;
   }


void
TR_Debug::print(TR::FILE *pOutFile, TR::S390WarmToColdTrampolineSnippet *snippet)
   {
   uint8_t * buffer = snippet->getSnippetLabel()->getCodeLocation();

   printSnippetLabel(pOutFile, snippet->getSnippetLabel(), buffer, "Warm To Cold Trampoline");


   printPrefix(pOutFile, NULL, buffer, 6);
   trfprintf(pOutFile, "BRCL 0xF, targetLabel(%p)", snippet->getTargetLabel()->getCodeLocation());
   buffer += 6;
   }
