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

#include "x/codegen/ConstantDataSnippet.hpp"

#include <stddef.h>                           // for NULL
#include <stdint.h>                           // for uint8_t, int16_t
#include "DataSnippet.hpp"                    // for TR::IA32DataSnippet
#include "codegen/Snippet.hpp"                // for commentString, etc
#include "compile/Compilation.hpp"            // for Compilation
#include "il/symbol/LabelSymbol.hpp"          // for LabelSymbol
#include "ras/Debug.hpp"                      // for TR_Debug
#include "env/IO.hpp"

namespace TR { class CodeGenerator; }
namespace TR { class Node; }

TR::IA32ConstantDataSnippet::IA32ConstantDataSnippet(TR::CodeGenerator *cg, TR::Node * n, void *c, uint8_t size)
   : TR::IA32DataSnippet(cg, n, c, size)
   {
   }

void
TR_Debug::print(TR::FILE *pOutFile, TR::IA32ConstantDataSnippet * snippet)
   {
   // *this   swipeable for debugger
   if (pOutFile == NULL)
      return;

   uint8_t *bufferPos = snippet->getSnippetLabel()->getCodeLocation();

   printSnippetLabel(pOutFile, snippet->getSnippetLabel(), bufferPos, getName(snippet));
   printPrefix(pOutFile, NULL, bufferPos, snippet->getConstantSize());

   trfprintf(pOutFile, "%s \t%s", snippet->getConstantSize() == 8 ?
                 dqString() :
                 (snippet->getConstantSize() == 4 ?
                  ddString() :
                  dwString()),
                 hexPrefixString());

   for (int i=snippet->getConstantSize()-1; i >= 0; i--)
     {
       trfprintf(pOutFile, "%02x", bufferPos[i]);
     }

   trfprintf(pOutFile,"%s", hexSuffixString());

   if (snippet->getConstantSize() == 8)
      {
      double *d = (double *)bufferPos;
      trfprintf(pOutFile, "\t%s %gD",
                    commentString(),
                    *d);
      }
   else if (snippet->getConstantSize() == 4)
      {
      float *f = (float *)bufferPos;
      trfprintf(pOutFile, "\t\t%s %gF",
                    commentString(),
                    *f);
      }
   else if (snippet->getConstantSize() == 2)
      {
      int16_t *s = (int16_t *)bufferPos;
      trfprintf(pOutFile, "\t\t\t%s 0x%04x",
                    commentString(),
                    *s);
      }
   }
