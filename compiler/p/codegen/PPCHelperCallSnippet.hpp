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

#ifndef PPCHELPERCALLSNIPPET_INCL
#define PPCHELPERCALLSNIPPET_INCL

#include "codegen/Snippet.hpp"

#include <stddef.h>                  // for NULL
#include <stdint.h>                  // for uint32_t, uint8_t, int32_t
#include "codegen/RealRegister.hpp"  // for RealRegister, etc
#include "il/SymbolReference.hpp"    // for SymbolReference

#include "codegen/GCStackAtlas.hpp"

namespace TR { class CodeGenerator; }
namespace TR { class LabelSymbol; }
namespace TR { class Node; }

namespace TR {

class PPCHelperCallSnippet : public TR::Snippet
   {
   TR::SymbolReference      *_destination;
   TR::LabelSymbol          *_restartLabel;

   public:

   PPCHelperCallSnippet(TR::CodeGenerator    *cg,
                        TR::Node             *node,
                        TR::LabelSymbol      *snippetlab,
                        TR::SymbolReference  *helper,
                        TR::LabelSymbol      *restartLabel=NULL)
      : TR::Snippet(cg, node, snippetlab, (restartLabel!=NULL && helper->canCauseGC())),
        _destination(helper),
        _restartLabel(restartLabel)
      {
      }

   virtual Kind getKind() { return IsHelperCall; }

   TR::SymbolReference *getDestination()             {return _destination;}
   TR::SymbolReference *setDestination(TR::SymbolReference *s) {return (_destination = s);}

   TR::LabelSymbol     *getRestartLabel() {return _restartLabel;}
   TR::LabelSymbol     *setRestartLabel(TR::LabelSymbol *l) {return (_restartLabel=l);}

   uint8_t *genHelperCall(uint8_t *cursor);
   uint32_t getHelperCallLength();

   virtual uint8_t *emitSnippetBody();

   virtual uint32_t getLength(int32_t estimatedSnippetStart);
   };

class PPCArrayCopyCallSnippet : public TR::PPCHelperCallSnippet
   {
   TR::RealRegister::RegNum _lengthRegNum;

   public:

   PPCArrayCopyCallSnippet(TR::CodeGenerator    *cg,
                           TR::Node             *node,
                           TR::LabelSymbol      *snippetlab,
                           TR::SymbolReference  *helper,
                           TR::RealRegister::RegNum lengthRegNum,
                           TR::LabelSymbol      *restartLabel=NULL)
      : TR::PPCHelperCallSnippet(cg, node, snippetlab, helper, restartLabel),
        _lengthRegNum(lengthRegNum)
      {
      }
   virtual Kind getKind() { return IsArrayCopyCall; }

   TR::RealRegister::RegNum getLengthRegNum() {return _lengthRegNum;}
   TR::RealRegister::RegNum
      setLengthRegNum(TR::RealRegister::RegNum r) {return (_lengthRegNum = r);}

   virtual uint8_t *emitSnippetBody();

   virtual uint32_t getLength(int32_t estimatedSnippetStart);
   };

}

#endif
