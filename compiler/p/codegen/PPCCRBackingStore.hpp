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

#ifndef PPCCRBACKINGSTORE_INCL
#define PPCCRBACKINGSTORE_INCL

#include <stdint.h>                  // for uint8_t
#include "codegen/BackingStore.hpp"  // for TR_BackingStore
#include "compile/Compilation.hpp"   // for Compilation
#include "il/SymbolReference.hpp"    // for SymbolReference

class TR_PPCCRBackingStore;

// Pseudo-safe downcast function, used exclusively for cr spill/restore
//
inline TR_PPCCRBackingStore *toPPCCRBackingStore(TR_BackingStore *r)
   {
   return (TR_PPCCRBackingStore *)r;
   }

class TR_PPCCRBackingStore: public TR_BackingStore
   {
   private:

   TR_BackingStore *original;
   uint8_t ccrFieldIndex;

   public:

   TR_PPCCRBackingStore();

   TR_PPCCRBackingStore(TR::Compilation * comp, TR_BackingStore *orig)
     : TR_BackingStore(comp->getSymRefTab(), orig->getSymbolReference()->getSymbol(), orig->getSymbolReference()->getOffset()), original(orig)
      {}

   uint8_t getCcrFieldIndex() {return ccrFieldIndex;}
   uint8_t setCcrFieldIndex(uint8_t cfi) {return (ccrFieldIndex=cfi);}
   TR_BackingStore *getOriginal() {return original;}
   TR_BackingStore *setOriginal(TR_BackingStore *o) {return (original = o);}
   };

#endif
