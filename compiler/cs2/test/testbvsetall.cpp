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

#include <stdlib.h>
#include <iostream>

#include "cs2/bitvectr.h"
#include "cs2/sparsrbit.h"

template <class ABitVector, class Allocator>
int foo(int seed, Allocator &a) {
  ABitVector v1(a);

  uint32_t n = seed;

  v1.SetAll(n);

  int i = 0;
  auto iv = v1.begin();
  for (; (iv != v1.end()) && i < n ; ++iv, ++i) {
    if (iv != i) {
      if (i > iv)
        printf("%d: seed = %d ERROR: iterator index %d should not exist in vector.\n", __LINE__, seed, (uint32_t) iv);
      else
        printf("%d: seed = %d ERROR: iterator index %d should exist in vector.\n", __LINE__, seed, i);
      return 1;
    }
    // printf("iv    = %d\n", (uint32_t) iv);
  }
  if ((iv == v1.end()) != (i == n)) {
    printf("i     = %d\n", i);
    printf("n     = %d\n", n);
    printf("iv    = %d\n", (uint32_t) iv);
    printf("begin = %d\n", (uint32_t) v1.begin());
    printf("end   = %d\n", (uint32_t) v1.end());
    printf("%d: seed = %d ERROR: iterator has %sterminated, when there should %sindices left.\n",
        __LINE__, seed, (iv == v1.end()) ? "" : "not ", (i < n) ? "still be " : "be no ");
    return 1;
  }

  return 0;
}

typedef CS2::shared_allocator<CS2::heap_allocator<8192,10> > allocator;
typedef CS2::ABitVector<allocator> dense;

int main() {
  CS2::heap_allocator<8192,10> h;
  allocator a(h);

  for(int seed = 0; seed < 1000; seed++)
    if (foo<dense,allocator>(seed,a) == 1)
      return 1;
  return 0;
}
