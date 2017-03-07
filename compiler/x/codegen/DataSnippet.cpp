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

#include "x/codegen/DataSnippet.hpp"

#include <stdint.h>                   // for uint8_t, int16_t, uint32_t, etc
#include <string.h>                   // for NULL, memcpy
#include "codegen/CodeGenerator.hpp"  // for CodeGenerator
#include "compile/Compilation.hpp"    // for Compilation
#include "env/CompilerEnv.hpp"
#include "env/IO.hpp"
#include "env/TRMemory.hpp"           // for TR_HeapMemory
#include "env/jittypes.h"             // for intptrj_t
#include "il/symbol/LabelSymbol.hpp"  // for LabelSymbol
#include "ras/Debug.hpp"              // for TR_Debug

namespace TR { class Node; }

TR::IA32DataSnippet::IA32DataSnippet(TR::CodeGenerator *cg, TR::Node * n, void *c, uint8_t size)
   : TR::Snippet(cg, n, TR::LabelSymbol::create(cg->trHeapMemory(),cg), false)
   {

   memcpy(_value, c, size);
   _length = size;
   _isClassAddress = false;
   }


void
TR::IA32DataSnippet::addMetaDataForCodeAddress(uint8_t *cursor)
   {
   // add dummy class unload/redefinition assumption.
   if (_isClassAddress)
      {
      if (TR::Compiler->target.is64Bit())
         {
         cg()->jitAddPicToPatchOnClassUnload((void*)-1, (void *) cursor);
         if (cg()->wantToPatchClassPointer(NULL, cursor)) // unresolved
            {
            cg()->jitAddPicToPatchOnClassRedefinition(((void *) -1), (void *) cursor, true);
            }
         }
      else
         {
         cg()->jitAdd32BitPicToPatchOnClassUnload((void*)-1, (void *) cursor);
         if (cg()->wantToPatchClassPointer(NULL, cursor)) // unresolved
            {
            cg()->jitAdd32BitPicToPatchOnClassRedefinition(((void *) -1), (void *) cursor, true);
            }
         }
      }
   }


uint8_t *TR::IA32DataSnippet::emitSnippetBody()
   {

   uint8_t *cursor = cg()->getBinaryBufferCursor();

   // align to 16 bytes
   if (_length == 16)
      {
      cursor = (uint8_t*)(((intptrj_t)(cursor + 15)) & ((-1)<<4));
      }

   getSnippetLabel()->setCodeLocation(cursor);

   memcpy(cursor, &_value, _length);

   addMetaDataForCodeAddress(cursor);


   cursor += _length;

   return cursor;
   }

void
TR_Debug::print(TR::FILE *pOutFile, TR::IA32DataSnippet * snippet)
   {
   // *this   swipeable for debugger
   if (pOutFile == NULL)
      return;

   uint8_t *bufferPos = snippet->getSnippetLabel()->getCodeLocation();

   printSnippetLabel(pOutFile, snippet->getSnippetLabel(), bufferPos, getName(snippet));
   printPrefix(pOutFile, NULL, bufferPos, snippet->getDataSize());

   trfprintf(pOutFile, "%s \t%s", snippet->getDataSize() == 8 ?
                 dqString() :
                 (snippet->getDataSize() == 4 ?
                  ddString() :
                  dwString()),
                 hexPrefixString());

   for (int i=snippet->getDataSize()-1; i >= 0; i--)
     {
       trfprintf(pOutFile, "%02x", bufferPos[i]);
     }

   trfprintf(pOutFile,"%s", hexSuffixString());

   if (snippet->getDataSize() == 8)
      {
      double *d = (double *)bufferPos;
      trfprintf(pOutFile, "\t%s %gD",
                    commentString(),
                    *d);
      }
   else if (snippet->getDataSize() == 4)
      {
      float *f = (float *)bufferPos;
      trfprintf(pOutFile, "\t\t%s %gF",
                    commentString(),
                    *f);
      }
   else if (snippet->getDataSize() == 2)
      {
      int16_t *s = (int16_t *)bufferPos;
      trfprintf(pOutFile, "\t\t\t%s 0x%04x",
                    commentString(),
                    *s);
      }
   }

uint32_t TR::IA32DataSnippet::getLength(int32_t estimatedSnippetStart)
   {
   return _length;
   }

