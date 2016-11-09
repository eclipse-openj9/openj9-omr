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

#ifndef S390HELPERCALLSNIPPET_INCL
#define S390HELPERCALLSNIPPET_INCL

#include "codegen/Snippet.hpp"

#include <stddef.h>                // for NULL
#include <stdint.h>                // for int32_t, uint32_t, uint8_t
#include "il/SymbolReference.hpp"  // for SymbolReference
#include "infra/Assert.hpp"        // for TR_ASSERT

namespace TR { class CodeGenerator; }
namespace TR { class LabelSymbol; }
namespace TR { class Node; }

namespace TR {

class S390HelperCallSnippet : public TR::Snippet
   {
   TR::LabelSymbol           *_reStartLabel;         ///< Label of Return Address in Main Line Code.
   TR::SymbolReference      *_helperSymRef;         ///< Helper Symbol Reference.
   int32_t  sizeOfArguments;

   public:

   S390HelperCallSnippet(TR::CodeGenerator        *cg,
                         TR::Node                 *node,
                         TR::LabelSymbol           *snippetlab,
                         TR::SymbolReference      *helper,
                         TR::LabelSymbol           *restartlab = NULL,
                         int32_t                  s = 0)
      : TR::Snippet(cg, node, snippetlab, (restartlab == NULL)),
        _reStartLabel(restartlab),
        _helperSymRef(helper),
	    sizeOfArguments(s)
      {
      // If we don't have a restart label, then we must not be returning to the mainline code -
      // hence, always except.
      TR_ASSERT(restartlab || (!restartlab && helper->canCauseGC()),
                 "An exception snippet is marked as cannot cause GC");

      // Set up appropriate GC Map
      if (!restartlab)
        gcMap().setGCRegisterMask((uint32_t)0x00000000);  // everything gets clobbered if we're taking an exception.
      }

   virtual Kind getKind() { return IsHelperCall; }

   int32_t getSizeOfArguments()          {return sizeOfArguments;}
   int32_t setSizeOfArguments(int32_t s) {return sizeOfArguments = s;}

   TR::SymbolReference *getHelperSymRef()                      {return _helperSymRef;}
   TR::SymbolReference *setHelperSymRef(TR::SymbolReference *s) {return _helperSymRef = s;}

   TR::LabelSymbol *getReStartLabel()                  {return _reStartLabel;}
   TR::LabelSymbol *setReStartLabel(TR::LabelSymbol *l) {return _reStartLabel = l;}

   bool alwaysExcept()               {return _reStartLabel == NULL;}

   virtual uint8_t *emitSnippetBody();

   virtual uint32_t getLength(int32_t);
   };

}

#endif
