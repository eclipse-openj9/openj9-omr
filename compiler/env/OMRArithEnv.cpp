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
 ******************************************************************************/

#include <stddef.h>                   // for NULL
#include <stdint.h>                   // for int32_t, int64_t, uint32_t
#include <math.h>
#include "env/ArithEnv.hpp"
#include "env/CompilerEnv.hpp"
#include "env/jittypes.h"             // for uintptrj_t, intptrj_t
#include "infra/Assert.hpp"           // for TR_ASSERT


TR::ArithEnv *
OMR::ArithEnv::self()
   {
   return static_cast<TR::ArithEnv *>(this);
   }

float
OMR::ArithEnv::floatAddFloat(float a, float b)
   {
   return a+b;
   }

float
OMR::ArithEnv::floatSubtractFloat(float a, float b)
   {
   return a-b;
   }

float
OMR::ArithEnv::floatMultiplyFloat(float a, float b)
   {
   return a * b;
   }

float
OMR::ArithEnv::floatDivideFloat(float a, float b)
   {
   return a / b;
   }

float
OMR::ArithEnv::floatRemainderFloat(float a, float b)
   {

   // C99 IEEE remainder is not available in older MSVC versions

#if defined (_MSC_VER)
   return 0;
#else
   return remainderf(a, b);
#endif

   }

float
OMR::ArithEnv::floatNegate(float a)
   {
   float c;
   c = -a;
   return c;
   }

double
OMR::ArithEnv::doubleAddDouble(double a, double b)
   {
   return a + b;
   }

double
OMR::ArithEnv::doubleSubtractDouble(double a, double b)
   {
   return a - b;
   }

double
OMR::ArithEnv::doubleMultiplyDouble(double a, double b)
   {
   return a * b;
   }

double
OMR::ArithEnv::doubleDivideDouble(double a, double b)
   {
   return a / b;
   }

double
OMR::ArithEnv::doubleRemainderDouble(double a, double b)
   {
#if defined (_MSC_VER)
   return 0;
#else
   return remainder(a, b);
#endif
   }

double
OMR::ArithEnv::doubleNegate(double a)
   {
   double c;
   c = -a;
   return c;
   }

double
OMR::ArithEnv::floatToDouble(float a)
   {
   return (double) a;
   }

float
OMR::ArithEnv::doubleToFloat(double a)
   {
   return (float) a;
   }

int64_t
OMR::ArithEnv::longRemainderLong(int64_t a, int64_t b)
   {
   return a % b;
   }

int64_t
OMR::ArithEnv::longDivideLong(int64_t a, int64_t b)
   {
   return a / b;
   }

int64_t
OMR::ArithEnv::longMultiplyLong(int64_t a, int64_t b)
   {
   return a * b;
   }
