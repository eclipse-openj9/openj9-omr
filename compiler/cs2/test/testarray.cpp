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

#define CS2_ASSERT
#define CS2_ALLOCINFO
#include <cstdio>
#include <iostream>
#include <cs2/arrayof.h>
#include <cs2/allocator.h>
#include <cs2/timer.h>
#include <cs2/sparsrbit.h>
#include <cs2/bitvectr.h>
#include <cs2/hashtab.h>

typedef CS2::trace_allocator<> a;

int main(int argc, char **argv) {
  CS2::ArrayOf<uint32_t, a> A1( a(), 3);
  CS2::ArrayOf<uint32_t, a> A2;

  CS2::PhaseTimingSummary<a> x("noname", a(), argc>1);

  CS2::LexicalBlockTimer<a> t("whatever", x);

  CS2::ABitVector<a> b,b2;
  CS2::ASparseBitVector<a> sb,sb2;

  b[34]=0;
  sb[34]=0;

  b2 = b;
  sb2 = sb;
  sb = b;

  CS2::HashTable<uint32_t, bool, a> h;

  CS2::HashIndex hi;
  bool f = h.Locate(10, hi);
}
