/*******************************************************************************
 * Copyright (c) 2000, 2022 IBM Corp. and others
 *
 * This program and the accompanying materials are made available under
 * the terms of the Eclipse Public License 2.0 which accompanies this
 * distribution and is available at http://eclipse.org/legal/epl-2.0
 * or the Apache License, Version 2.0 which accompanies this distribution
 * and is available at https://www.apache.org/licenses/LICENSE-2.0.
 *
 * This Source Code may also be made available under the following Secondary
 * Licenses when the conditions for such availability set forth in the
 * Eclipse Public License, v. 2.0 are satisfied: GNU General Public License,
 * version 2 with the GNU Classpath Exception [1] and GNU General Public
 * License, version 2 with the OpenJDK Assembly Exception [2].
 *
 * [1] https://www.gnu.org/software/classpath/license.html
 * [2] http://openjdk.java.net/legal/assembly-exception.html
 *
 * SPDX-License-Identifier: EPL-2.0 OR Apache-2.0 OR GPL-2.0 WITH Classpath-exception-2.0 OR LicenseRef-GPL-2.0 WITH Assembly-exception
 *******************************************************************************/

#include "il/DataTypes.hpp"

#include <ctype.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "env/jittypes.h"
#include "env/TRMemory.hpp"
#include "il/DataTypes.hpp"
#include "infra/Assert.hpp"
#include "infra/String.hpp"

// When adding new types also update pDataTypeNames[] in ras/Tree.cpp

namespace OMR {

#define TR_Bad TR::BadILOp

static TR::ILOpCodes conversionMap[TR::NumOMRTypes][TR::NumOMRTypes] =
//                       No      Int8     Int16    Int32    Int64    Float    Double   Addr     Aggregate
   {
   /* NoType */        { TR_Bad, TR_Bad,  TR_Bad,  TR_Bad,  TR_Bad,  TR_Bad,  TR_Bad,  TR_Bad,  TR_Bad},  // NoType
   /* Int8 */          { TR_Bad, TR_Bad,  TR::b2s, TR::b2i, TR::b2l, TR::b2f, TR::b2d, TR::b2a, TR_Bad},  // Int8
   /* Int16 */         { TR_Bad, TR::s2b, TR_Bad,  TR::s2i, TR::s2l, TR::s2f, TR::s2d, TR::s2a, TR_Bad},  // Int16
   /* Int32 */         { TR_Bad, TR::i2b, TR::i2s, TR_Bad,  TR::i2l, TR::i2f, TR::i2d, TR::i2a, TR_Bad},  // Int32
   /* Int64 */         { TR_Bad, TR::l2b, TR::l2s, TR::l2i, TR_Bad,  TR::l2f, TR::l2d, TR::l2a, TR_Bad},  // Int64
   /* Float */         { TR_Bad, TR::f2b, TR::f2s, TR::f2i, TR::f2l, TR_Bad,  TR::f2d, TR_Bad,  TR_Bad},  // Float
   /* Double */        { TR_Bad, TR::d2b, TR::d2s, TR::d2i, TR::d2l, TR::d2f, TR_Bad,  TR_Bad,  TR_Bad},  // Double
   /* Address */       { TR_Bad, TR::a2b, TR::a2s, TR::a2i, TR::a2l, TR_Bad,  TR_Bad,  TR_Bad,  TR_Bad},  // Address
   /* Aggregate */     { TR_Bad, TR_Bad,  TR_Bad,  TR_Bad,  TR_Bad,  TR_Bad,  TR_Bad,  TR_Bad,  TR_Bad},  // Aggregate
   };

#undef TR_Bad

} // namespace OMR

bool
OMR::DataType::canGetMaxPrecisionFromType()
   {
   switch (self()->getDataType())
      {
      case TR::Int8:
      case TR::Int16:
      case TR::Int32:
      case TR::Int64:
         return true;
      default:
         return false;
      }
   }

int32_t
OMR::DataType::getMaxPrecisionFromType()
   {
   switch (self()->getDataType())
      {
      case TR::Int8: return TR::getMaxSignedPrecision<TR::Int8>();
      case TR::Int16: return TR::getMaxSignedPrecision<TR::Int16>();
      case TR::Int32: return TR::getMaxSignedPrecision<TR::Int32>();
      case TR::Int64: return TR::getMaxSignedPrecision<TR::Int64>();
      default:
         TR_ASSERT(false, "Unsupported data type in getMaxPrecisionFromType\n");
         return 0;
      }
   }

TR::DataType
OMR::DataType::getVectorIntegralType()
   {
   if (!self()->isVector()) return TR::NoType;

   switch(self()->getVectorElementType())
      {
      case TR::Int8:
      case TR::Int16:
      case TR::Int32:
      case TR::Int64: return self()->getDataType();
      case TR::Float: return TR::DataType::createVectorType(TR::Int32, self()->getVectorLength());
      case TR::Double: return TR::DataType::createVectorType(TR::Int64, self()->getVectorLength());
      default:
         return TR::NoType;
         break;
      }
   }

TR::DataType
OMR::DataType::vectorToScalar()
   {
   return self()->getVectorElementType();
   }

TR::DataType
OMR::DataType::scalarToVector(TR::VectorLength length)
   {
   TR::DataTypes type = self()->getDataType();

   if (type > TR::NoType && type <= TR::NumVectorElementTypes)
      return createVectorType(type, length);
   else
      return TR::NoType;
   }

TR::DataType
OMR::DataType::getIntegralTypeFromPrecision(int32_t precision)
   {
   if (precision < 1 || precision >= TR::getMaxSignedPrecision<TR::Int64>())
      return TR::NoType;
   else if (precision < TR::getMaxSignedPrecision<TR::Int8>())
      return TR::Int8;
   else if (precision < TR::getMaxSignedPrecision<TR::Int16>())
      return TR::Int16;
   else if (precision < TR::getMaxSignedPrecision<TR::Int32>())
      return TR::Int32;
   else
      return TR::Int64;
   }

TR::DataType
OMR::DataType::getFloatTypeFromSize(int32_t size)
   {
   TR::DataType type = TR::NoType;
   switch (size)
      {
      case 4:  type = TR::Float; break;
      case 8:  type = TR::Double; break;
      default: TR_ASSERT(false,"unexpected size %d for float type\n",size);
      }
   return type;
   }

TR::ILOpCodes
OMR::DataType::getDataTypeConversion(TR::DataType t1, TR::DataType t2)
   {
   if (t1.isVector() && t2.isVector())
      return TR::v2v;

   if (t1.isVector() || t2.isVector())
      return TR::BadILOp;


   TR_ASSERT(t1 < TR::NumOMRTypes, "conversion opcode from unexpected datatype %s requested", t1.toString());
   TR_ASSERT(t2 < TR::NumOMRTypes, "conversion opcode to unexpected datatype %s requested", t2.toString());
   return OMR::conversionMap[t1][t2];
   }

TR::ILOpCodes
OMR::DataType::getDataTypeBitConversion(TR::DataType t1, TR::DataType t2)
   {
   if (t1.isVector() || t2.isVector())
      return TR::BadILOp;

   TR_ASSERT(t1 < TR::NumOMRTypes, "conversion opcode from unexpected datatype %s requested", t1.toString());
   TR_ASSERT(t2 < TR::NumOMRTypes, "conversion opcode to unexpected datatype %s requested", t2.toString());
   if (t1 == TR::Int32 && t2 == TR::Float)
      return TR::ibits2f;
   else if (t1 == TR::Float && t2 == TR::Int32)
      return TR::fbits2i;
   else if (t1 == TR::Int64 && t2 == TR::Double)
      return TR::lbits2d;
   else if (t1 == TR::Double && t2 == TR::Int64)
      return TR::dbits2l;
   else
      return TR::BadILOp;
   }

static int32_t OMRDataTypeSizes[] =
   {
   0,                 // TR::NoType
   1,                 // TR::Int8
   2,                 // TR::Int16
   4,                 // TR::Int32
   8,                 // TR::Int64
   4,                 // TR::Float
   8,                 // TR::Double
   sizeof(intptr_t), // TR::Address
   0,                 // TR::Aggregate
   };

static_assert(TR::NumOMRTypes == (sizeof(OMRDataTypeSizes) / sizeof(OMRDataTypeSizes[0])), "OMRDataTypeSizes is not the correct size");

int32_t
OMR::DataType::getSize(TR::DataType dt)
   {
   if (dt.isVector())
      {
      switch (dt.getVectorLength())
         {
         case TR::VectorLength64:  return 8;
         case TR::VectorLength128: return 16;
         case TR::VectorLength256: return 32;
         case TR::VectorLength512: return 64;
         default:
            TR_ASSERT_FATAL(false, "Incorrect Vector Length\n");
         }
      }

   TR_ASSERT(dt < TR::NumOMRTypes, "dataTypeSizeMap called on unrecognized data type");
   return OMRDataTypeSizes[dt];
   }

void
OMR::DataType::setSize(TR::DataType dt, int32_t newSize)
   {
   if (dt.isVector()) return;

   TR_ASSERT(dt < TR::NumOMRTypes, "setDataTypeSizeInMap called on unrecognized data type");
   OMRDataTypeSizes[dt] = newSize;
   }


static const char * OMRDataTypeNames[TR::NumAllTypes] =
   {
   "NoType",
   "Int8",
   "Int16",
   "Int32",
   "Int64",
   "Float",
   "Double",
   "Address",
   "Aggregate",
   // vector names will be created at runtime
   };

#define MAX_TYPE_NAME_LENGTH 20

bool
OMR::DataType::initVectorNames()
   {
   int32_t numVectorTypes = TR::NumAllTypes - TR::NumScalarTypes;
   char *names = (char*)TR_Memory::jitPersistentAlloc(MAX_TYPE_NAME_LENGTH*numVectorTypes*sizeof(char));
   char *name = names;

   for (int32_t i = TR::NumScalarTypes; i < TR::NumAllTypes; i++)
      {
      TR::DataType dt((TR::DataTypes)i);
      TR_ASSERT_FATAL(dt.isVector(), "Should be a vector type");
      TR::snprintfNoTrunc(name, MAX_TYPE_NAME_LENGTH, "Vector%d%s", getSize(dt)*8, getName(dt.getVectorElementType()));
      OMRDataTypeNames[dt] = name;
      name += MAX_TYPE_NAME_LENGTH;
      }
   return true;
   }

const char *
OMR::DataType::getName(TR::DataType dt)
   {
   if (dt.isVector())
      {
      // to avoid any race conditions, initialize all vector names once,
      // as soon as first one is requested
       static bool staticallyInitialized = initVectorNames();
      TR_ASSERT_FATAL(staticallyInitialized && (OMRDataTypeNames[dt] != NULL), "Vector names should've been initialized");
      }

   TR_ASSERT(dt.isOMRDataType(), "Name requested for a non-OMR datatype\n");
   return OMRDataTypeNames[dt];
   }

TR::DataType
OMR::DataType::getTypeFromName(const char *name)
   {
   for (int32_t i = 1 ; i < TR::NumAllTypes; i++)
      {
      TR::DataType dt = (TR::DataTypes)i;

      if (!dt.isOMRDataType()) continue;

      if (strcmp(name, getName(dt)) == 0)
         return  dt;
      }

   return TR::NoType;
   }

const char *
OMR::DataType::toString() const
   {
   return TR::DataType::getName(self()->getDataType());
   }

void FloatingPointLimits::setMaxFloat()
   {
   int32_t f = FLOAT_POS_INFINITY;
   _maxFloat = 0.0f;
   memcpy(&_maxFloat, &f, sizeof(f));
   }

void  FloatingPointLimits::setMinFloat()
   {
   int32_t f = FLOAT_NEG_INFINITY;
   _minFloat = 0.0f;
   memcpy(&_minFloat, &f, sizeof(f));
   }
void  FloatingPointLimits::setMaxDouble()
   {
   uint64_t d = DOUBLE_POS_INFINITY;
   _maxDouble = 0.0;
   memcpy(&_maxDouble, &d, sizeof(d));
   }

void  FloatingPointLimits::setMinDouble()
   {
   int64_t d = DOUBLE_NEG_INFINITY;
   _minDouble = 0.0;
   memcpy(&_minDouble, &d, sizeof(d));
   }

namespace TR{
   static FloatingPointLimits fpLimits;

   float getMaxFloat() { return fpLimits.getMaxFloat(); }
   float getMinFloat() { return fpLimits.getMinFloat(); }
   double getMaxDouble() { return fpLimits.getMaxDouble(); }
   double getMinDouble() { return fpLimits.getMinDouble(); }
}


const TR::DataTypes OMR::DataType::Vector128Int8   = OMR::DataType::createVectorType(TR::Int8, TR::VectorLength128);
const TR::DataTypes OMR::DataType::Vector128Int16  = OMR::DataType::createVectorType(TR::Int16, TR::VectorLength128);
const TR::DataTypes OMR::DataType::Vector128Int32  = OMR::DataType::createVectorType(TR::Int32, TR::VectorLength128);
const TR::DataTypes OMR::DataType::Vector128Int64  = OMR::DataType::createVectorType(TR::Int64, TR::VectorLength128);
const TR::DataTypes OMR::DataType::Vector128Float  = OMR::DataType::createVectorType(TR::Float, TR::VectorLength128);
const TR::DataTypes OMR::DataType::Vector128Double = OMR::DataType::createVectorType(TR::Double, TR::VectorLength128);
