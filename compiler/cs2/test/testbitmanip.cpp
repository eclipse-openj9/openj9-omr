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

#include "cs2/bitvectr.h"

template <class ABitVector>
int foo(int seed) {
  ABitVector v1,v2,v3;
  int count;
  srand(seed);

  count =rand()%1000000;
  for (int i=0; i<count; i++)
    v1[rand()%0xFFFFFF]=1;

  count =rand()%1000000;
  for (int i=0; i<count; i++)
    v2[rand()%0xFFFFFF]=(bool)v1[rand()%0xFFFFFF];

  count =rand()%1000000;
  for (int i=0; i<count; i++)
    v3[rand()%0xFFFFFF]=(bool)v2[rand()%0xFFFFFF];

  v2.Or(v3);
  v1.And(v3);

  v1.Or(v2,v3);
  v3.And(v2,v1);

  uint64_t x = 0x0200000000000000ull;

  uint32_t lastWordIndex = v1.LastWord();
  uint64_t lastWord = v1.WordValueAt(0, lastWordIndex);
  printf("x                        = %0*llX\n", CS2::kBitWordSize/4, x);
  printf("trailingZeroes(x)        = %d\n", CS2::BitManipulator::TrailingZeroes(x));
  if (x != lastWord) {
    printf("%0*llX != %0*llX\n", CS2::kBitWordSize/4, x, CS2::kBitWordSize/4, lastWord);
  } else {
    printf("Comparison shows that x is equal to lastWord\n");
  }
  printf("lastWord                 = %0*llX\n", CS2::kBitWordSize/4, lastWord);
  printf("trailingZeroes(lastWord) = %d\n", CS2::BitManipulator::TrailingZeroes(lastWord));
  return 1;
}

typedef CS2::BitVector dense;

int main() {
  int seed = 2;
  foo<dense>(seed);
  return 1;
}
