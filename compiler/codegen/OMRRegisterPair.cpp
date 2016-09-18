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

#include "codegen/OMRRegisterPair.hpp"  // for RegisterPair, etc

#include <stddef.h>                              // for NULL
#include <stdint.h>                              // for int32_t
#include "codegen/CodeGenerator.hpp"             // for CodeGenerator
#include "codegen/LiveRegister.hpp"              // for TR_LiveRegisters
#include "codegen/Register.hpp"                  // for Register
#include "codegen/RegisterPair.hpp"              // for RegisterPair
#include "infra/List.hpp"                        // for TR_Queue

void OMR::RegisterPair::block()
   {
   _lowOrder->block();
   _highOrder->block();
   }

void OMR::RegisterPair::unblock()
   {
   _lowOrder->unblock();
   _highOrder->unblock();
   }

bool OMR::RegisterPair::usesRegister(TR::Register* reg)
   {
   if (REGPAIR_THIS == reg || _lowOrder == reg || _highOrder == reg )
      {
      return true;
      }
   else
      {
      return false;
      }
   }

TR::Register *
OMR::RegisterPair::getLowOrder()
   {
   return _lowOrder;
   }

TR::Register *
OMR::RegisterPair::setLowOrder(TR::Register *lo, TR::CodeGenerator *codeGen)
   {
   if (!lo->isLive() && codeGen->getLiveRegisters(lo->getKind())!=NULL)
      codeGen->getLiveRegisters(lo->getKind())->addRegister(lo);

   return (_lowOrder = lo);
   }

TR::Register *
OMR::RegisterPair::getHighOrder()
   {
   return _highOrder;
   }

TR::Register *
OMR::RegisterPair::setHighOrder(TR::Register *ho, TR::CodeGenerator *codeGen)
   {
   if (!ho->isLive() && codeGen->getLiveRegisters(ho->getKind())!=NULL)
      codeGen->getLiveRegisters(ho->getKind())->addRegister(ho);

   return (_highOrder = ho);
   }

TR::Register *
OMR::RegisterPair::getRegister()
   {
   return NULL;
   }

TR::RegisterPair *
OMR::RegisterPair::getRegisterPair()
   {
   return REGPAIR_THIS;
   }

int32_t
OMR::RegisterPair::FlattenRegisterPairs(TR_Queue<TR::Register> * Pairs)
   {
    //Empty Queue
     while(Pairs->dequeue());

    int32_t regCount = 0;
    TR::Register * firstPair = REGPAIR_THIS;
    Pairs->enqueue(firstPair);
    regCount++;

      while(Pairs->peek()->getRegisterPair() != NULL){
         TR::Register * pair  = Pairs->dequeue();
         regCount--;
         Pairs->enqueue(pair->getHighOrder());
         regCount++;
         Pairs->enqueue(pair->getLowOrder());
         regCount++;
      }

    return regCount;
   }


