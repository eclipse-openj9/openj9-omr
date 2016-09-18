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

#ifndef RANDOM_INCL
#define RANDOM_INCL

#include <stdint.h>          // for int32_t, uint32_t, uint64_t
#include "env/TRMemory.hpp"  // for TR_Memory, etc
namespace TR { class Compilation; }

class TR_RandomGenerator
   {
   public:
   //Cannot new/delete; allocate as a value
   TR_ALLOC(TR_Memory::RandomGenerator)

   uint32_t getRandom();
   int32_t getRandom(int32_t min, int32_t max);
   void setSeed(uint32_t newSeed);

   TR_RandomGenerator(TR_RandomGenerator *parent)
      {
      setSeed(parent->getRandom());
      }

   TR_RandomGenerator(int32_t seedValue)
      {
      setSeed(seedValue);
      }

   TR_RandomGenerator(uint32_t seedValue)
      {
      setSeed(seedValue);
      }

   static void exercise(int32_t period, TR::Compilation *comp); // For testing

   private:
   uint64_t _bits; // Unsigned, so that right-shifts don't wipe the sign bit all over the place
   };

class TR_HasRandomGenerator
   {
   TR_RandomGenerator          _randomGenerator;

   public:
   bool                      randomBoolean(int32_t rarity=2)     { return _randomGenerator.getRandom(1, rarity) == rarity; } // Probability of returning true is 1/rarity
   int32_t                   randomInt()                         { return _randomGenerator.getRandom(); }
   int32_t                   randomInt(int32_t max)              { return _randomGenerator.getRandom(0, max); }
   int32_t                   randomInt(int32_t min, int32_t max) { return _randomGenerator.getRandom(min, max); }
   TR_RandomGenerator       *randomGenerator()                   { return &_randomGenerator; }

   TR_HasRandomGenerator(TR::Compilation *comp);
   };

#endif
