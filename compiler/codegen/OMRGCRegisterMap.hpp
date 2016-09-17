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

#ifndef OMR_GCREGISTERMAP_INCL
#define OMR_GCREGISTERMAP_INCL

/*
 * The following #define and typedef must appear before any #includes in this file
 */
#ifndef OMR_GCREGISTERMAP_CONNECTOR
#define OMR_GCREGISTERMAP_CONNECTOR
namespace OMR { class GCRegisterMap; }
namespace OMR { typedef OMR::GCRegisterMap GCRegisterMapConnector; }
#endif

#include <stdint.h>          // for uint32_t
#include "env/TRMemory.hpp"  // for TR_Memory, etc

namespace TR { class GCRegisterMap; }

namespace OMR
{

class GCRegisterMap
   {
   public:

   TR_ALLOC(TR_Memory::GCRegisterMap)

   GCRegisterMap() : _map(0), _registerSaveDescription(0), _hprmap(0) {}

   TR::GCRegisterMap * self();

   uint32_t getMap() {return _map;}
   void setRegisterBits(uint32_t bits) {_map |= bits;}
   void resetRegisterBits(uint32_t bits) { _map &= ~bits; }
   void empty() {_map = 0;}
   void maskRegisters(uint32_t mask) {_map &= mask;}
   void maskRegistersWithInfoBits(uint32_t mask, uint32_t info) {_map = (mask & (_map | info));}
   void setInfoBits(uint32_t info) {_map |= info;}

   uint32_t getHPRMap() {return _hprmap;}
   void setHighWordRegisterBits(uint32_t bits) {_hprmap |= bits;}
   void resetHighWordRegisterBits(uint32_t bits) { _hprmap &= ~bits; }
   void emptyHPR() {_hprmap = 0;}

   uint32_t getRegisterSaveDescription() {return _registerSaveDescription;}
   void setRegisterSaveDescription(uint32_t bits) {_registerSaveDescription = bits;}

   private:

   uint32_t _map;
   uint32_t _registerSaveDescription;
   uint32_t _hprmap;

   };

}

#endif
