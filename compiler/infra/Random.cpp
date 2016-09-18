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

#include "infra/Random.hpp"

#include <limits.h>                 // for MIN_INT, MAX_INT
#include <stdint.h>                 // for int32_t, uint32_t
#include "compile/Compilation.hpp"  // for Compilation

TR_HasRandomGenerator::TR_HasRandomGenerator(TR::Compilation *comp)
  : _randomGenerator(comp->primaryRandom()) {}

uint32_t TR_RandomGenerator::getRandom()
   {
   // Linear congruential parameters used by GCC; see http://en.wikipedia.org/wiki/Linear_congruential_generator
   // ...except we keep the sign bit (bit 31) while apparently they do not.
   //
   _bits = 1103515245 * _bits + 12345;

   // _bits alternates between odd and even, which is terrible for randomBoolean.
   // Let's mix it up a bit.
   //
   return (uint32_t)(_bits ^ (_bits >> 16));
   }

int32_t TR_RandomGenerator::getRandom(int32_t min, int32_t max)
   {
   uint32_t bits = getRandom();
   uint32_t range = max-min+1;
   if (range == 0) // corner case when range is INT_MIN..INT_MAX
      return (int32_t)bits;
   uint32_t rand = bits % range; // unsigned remainder
   return min + rand;
   }

void TR_RandomGenerator::setSeed(uint32_t newSeed)
   {
   _bits = newSeed;

   // Similar seeds can produce sequences that start off similar, so eat a few.
   //
   for (int i = 0; i < 5; i++)
      getRandom();
   }

class RandomExercizer: public TR_HasRandomGenerator
   {
   TR::Compilation *_comp;

   public:

   RandomExercizer(TR::Compilation *comp): TR_HasRandomGenerator(comp), _comp(comp){}

   TR::Compilation *comp(){ return _comp; }
   };


void TR_RandomGenerator::exercise(int32_t period, TR::Compilation *comp)
   {
   // Don't use the actual random generator because we don't want to perturb it
   RandomExercizer ex(comp);
   traceMsg(comp, "  %12s %12s %12s %12s %12s %12s\n",
      "Int", "Int(-5,5)", "Int(1,1)", "Int(MIN,MAX)", "Boolean", "Boolean(5)");
   for (int32_t i = 0; i < period; i++)
      {
      traceMsg(comp, "  %12d %12d %12d %12d %12d %12d\n",
         ex.randomInt(), ex.randomInt(-5,5), ex.randomInt(1,1), ex.randomInt(INT_MIN, INT_MAX), ex.randomBoolean(), ex.randomBoolean(5));
      }
   }
