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

#include <cstdio>
#include <stdlib.h>

#include "cs2/bitvectr.h"
#include "cs2/sparsrbit.h"



template <class ABitVector, class Allocator>
int foo(int seed, Allocator &a) {
  ABitVector v1(a),v2(a),v3(a);
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

  if (v1.IsZero()) return 0;
  return v1.FirstOne()+v1.LastOne()+v1.PopulationCount();
}

typedef CS2::shared_allocator<CS2::heap_allocator<8192,10> > allocator;
typedef CS2::ASparseBitVector<allocator> sparse;
typedef CS2::ABitVector<allocator> dense;

int main() {
  CS2::heap_allocator<8192,10> h;
  allocator a(h);

  for (int seed = 0; seed < 1000; seed++)
    if (foo<sparse,allocator>(seed,a) != foo<dense,allocator>(seed,a)) {
      printf("%d: ERROR %d!=%d\n", __LINE__, foo<sparse,allocator>(seed,a), foo<dense,allocator>(seed,a));
      return 1;
    }
  return 0;
}
