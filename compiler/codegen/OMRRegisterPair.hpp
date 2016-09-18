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

#ifndef OMR_REGISTER_PAIR_INCL
#define OMR_REGISTER_PAIR_INCL

/*
 * The following #define and typedef must appear before any #includes in this file
 */
#ifndef OMR_REGISTER_PAIR_CONNECTOR
#define OMR_REGISTER_PAIR_CONNECTOR
namespace OMR { class RegisterPair; }
namespace OMR { typedef OMR::RegisterPair RegisterPairConnector; }
#endif

#include <stddef.h>                         // for NULL
#include <stdint.h>                         // for int32_t
#include "codegen/Register.hpp"             // for Register
#include "codegen/RegisterConstants.hpp"    // for TR_RegisterKinds

namespace TR { class CodeGenerator; }
namespace TR { class RegisterPair; }

#define REGPAIR_THIS static_cast<TR::RegisterPair*>(this)
template<typename QueueKind> class TR_Queue;

namespace OMR
{

class OMR_EXTENSIBLE RegisterPair : public TR::Register
   {
   protected:

   RegisterPair() : _lowOrder(NULL), _highOrder(NULL) {}
   RegisterPair(TR_RegisterKinds rk) : TR::Register(rk), _lowOrder(NULL), _highOrder(NULL) {}
   RegisterPair(TR::Register *lo, TR::Register *ho) : _lowOrder(lo), _highOrder(ho) {}

   public:

   virtual void block();
   virtual void unblock();
   virtual bool usesRegister(TR::Register* reg);

   virtual TR::Register     *getLowOrder();
   virtual TR::Register     *getHighOrder();

   TR::Register     *setLowOrder(TR::Register *lo, TR::CodeGenerator *codeGen);
   TR::Register     *setHighOrder(TR::Register *ho, TR::CodeGenerator *codeGen);

   virtual TR::Register     *getRegister();
   virtual TR::RegisterPair *getRegisterPair();
   virtual int32_t            FlattenRegisterPairs(TR_Queue<TR::Register> * Pairs);


   private:
   TR::Register *_lowOrder;
   TR::Register *_highOrder;
   };

}

#endif
