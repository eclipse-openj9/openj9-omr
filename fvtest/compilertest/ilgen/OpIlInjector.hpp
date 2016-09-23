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

#ifndef TEST_OPILINJECTOR_INCL
#define TEST_OPILINJECTOR_INCL

#include "ilgen/IlInjector.hpp"

#include <stdint.h>
#include "env/jittypes.h"

namespace TR { class Node; }
namespace TR { class TypeDictionary; }

namespace TestCompiler
{
typedef enum ParmType { ParmInteger, ParmLong, ParmShort, ParmByte, ParmAddress, ParmDouble, ParmFloat } ParmType;

typedef union ParmValue
{
   int32_t parmInt;
   int64_t parmLong;
   int16_t parmShort;
   int8_t parmByte;
   uintptrj_t parmAddress;
   double parmDouble;
   float parmFloat;
} ParmValue;

typedef struct ParmNode
{
   ParmType type;
   ParmValue value;

   ParmNode(ParmType t) : type(t)  {}
} ParmNode;

class OpIlInjector : public TR::IlInjector
   {
   public:
   OpIlInjector(TR::TypeDictionary *types, TestDriver *test, TR::ILOpCodes opCode)
      : TR::IlInjector(types, test),
        _opCode(opCode),
        _numOptArgs(0),
        _conditionalDataType(TR::NoType)
      {
      setDataType();
      }
   TR_ALLOC(TR_Memory::IlGenerator)

   virtual bool injectIL() = 0;

   virtual void iconstParm(uint32_t slot, int32_t value)
      {
      TR_ASSERT(slot > 0 && slot <= _numOptArgs, "iconstParm: slot %d should be greater than 0 and less than _numOptArgs %d\n", slot, _numOptArgs);
      ParmNode *pnode = new ParmNode(ParmInteger);
      pnode->value.parmInt = value;
      _optArgs[slot-1] = pnode;
      }
   virtual void lconstParm(uint32_t slot, int64_t value)
      {
      TR_ASSERT(slot > 0 && slot <= _numOptArgs, "lconstParm: slot %d should be greater than 0 and less than _numOptArgs %d\n", slot, _numOptArgs);
      ParmNode *pnode = new ParmNode(ParmLong);
      pnode->value.parmLong = value;
      _optArgs[slot-1] = pnode;
      }
   virtual void bconstParm(uint32_t slot, int8_t value)
      {
      TR_ASSERT(slot > 0 && slot <= _numOptArgs, "bconstParm: slot %d should be greater than 0 and less than _numOptArgs %d\n", slot, _numOptArgs);
      ParmNode *pnode = new ParmNode(ParmByte);
      pnode->value.parmByte = value;
      _optArgs[slot-1] = pnode;
      }
   virtual void sconstParm(uint32_t slot, int16_t value)
      {
      TR_ASSERT(slot > 0 && slot <= _numOptArgs, "sconstParm: slot %d should be greater than 0 and less than _numOptArgs %d\n", slot, _numOptArgs);
      ParmNode *pnode = new ParmNode(ParmShort);
      pnode->value.parmShort = value;
      _optArgs[slot-1] = pnode;
      }
   virtual void aconstParm(uint32_t slot, uintptrj_t value)
      {
      TR_ASSERT(slot > 0 && slot <= _numOptArgs, "aconstParm: slot %d should be greater than 0 and less than _numOptArgs %d\n", slot, _numOptArgs);
      ParmNode *pnode = new ParmNode(ParmAddress);
      pnode->value.parmAddress = value;
      _optArgs[slot-1] = pnode;
      }
   virtual void dconstParm(uint32_t slot, double value)
      {
      TR_ASSERT(slot > 0 && slot <= _numOptArgs, "dconstParm: slot %d should be greater than 0 and less than _numOptArgs %d\n", slot, _numOptArgs);
      ParmNode *pnode = new ParmNode(ParmDouble);
      pnode->value.parmDouble = value;
      _optArgs[slot-1] = pnode;
      }
   virtual void fconstParm(uint32_t slot, float value)
      {
      TR_ASSERT(slot > 0 && slot <= _numOptArgs, "fconstParm: slot %d should be greater than 0 and less than _numOptArgs %d\n", slot, _numOptArgs);
      ParmNode *pnode = new ParmNode(ParmFloat);
      pnode->value.parmFloat = value;
      _optArgs[slot-1] = pnode;
      }

   protected:
   bool isOpCodeSupported();
   TR::Node *parm(uint32_t slot);
   void initOptArgs(uint32_t numOptArgs)
      {
      if (numOptArgs == 0) return;
      _numOptArgs = numOptArgs;
      _optArgs = new ParmNode *[_numOptArgs];
      for (int32_t i = 0; i < _numOptArgs; ++i)
         _optArgs[i] = NULL;
      }

   TR::ILOpCodes _opCode;
   TR::DataTypes _dataType; // datatype of OpCode child/children
   TR::DataTypes _conditionalDataType; // return type of Ternary opcodes

   ParmNode **_optArgs;  // holds args that are not on stack params
   uint32_t _numOptArgs;

   private:
   void setDataType();
   };

} // namespace TestCompiler

#endif // !defined(TEST_OPILINJECTOR_INCL)
