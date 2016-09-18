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

#ifndef OMR_ARITHENV_INCL
#define OMR_ARITHENV_INCL

/*
 * The following #define and typedef must appear before any #includes in this file
 */
#ifndef OMR_ARITHENV_CONNECTOR
#define OMR_ARITHENV_CONNECTOR
namespace OMR { class ArithEnv; }
namespace OMR { typedef OMR::ArithEnv ArithEnvConnector; }
#endif

#include <stdint.h>        // for int32_t, int64_t, uint32_t
#include "infra/Annotations.hpp"
#include "env/jittypes.h"

namespace TR { class ArithEnv; }

struct OMR_VMThread;
namespace TR { class Compilation; }

namespace OMR
{

class OMR_EXTENSIBLE ArithEnv
   {
public:

   TR::ArithEnv * self();

   float floatAddFloat(float a, float b);
   float floatSubtractFloat(float a, float b);
   float floatMultiplyFloat(float a, float b);
   float floatDivideFloat(float a, float b);
   float floatRemainderFloat(float a, float b);
   float floatNegate(float a);
   double doubleAddDouble(double a, double b);
   double doubleSubtractDouble(double a, double b);
   double doubleMultiplyDouble(double a, double b);
   double doubleDivideDouble(double a, double b);
   double doubleRemainderDouble(double a, double b);
   double doubleNegate(double a);
   double floatToDouble(float a);
   float doubleToFloat(double a);
   int64_t longRemainderLong(int64_t a, int64_t b);
   int64_t longDivideLong(int64_t a, int64_t b);
   int64_t longMultiplyLong(int64_t a, int64_t b);
   };

}

#endif
