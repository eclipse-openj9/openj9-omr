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

#include <iostream>
#include "cs2/llistof.h"

struct pair {
  pair(int _a, int _b): a(_a), b(_b) {}
  int a,b;
};

std::ostream &operator<< (std::ostream &out, struct pair &p) {
  return out << "(" << p.a << ":" << p.b <<")";
}

int main() {
  CS2::check_allocator<> ca;
  typedef CS2::LinkedListOf<pair, CS2::check_allocator<> > LIST;
  LIST lp(ca);

  bool isAddedToEnd = false;
  for (int i=0; i<10; i++) {
    lp.Add(pair(i,i*i), isAddedToEnd);
    isAddedToEnd = !isAddedToEnd;
  }

  LIST::Cursor c(lp);

  for (c.SetToFirst(); c.Valid(); c.SetToNext())
    std::cout << "[" << c->a << "," << c->b << "] ";
  std::cout << "\n";

  // test stack-based methods
  int i=0;
  while (!lp.IsEmpty())
    {
    pair &p = *lp.Head();
    int a = p.a;
    std::cout << "[" << p.a << "," << p.b << "] ";
    lp.Pop();

    if (a > 0)
      lp.Push(pair(a-1, a));
    else
      std::cout << "\n";
    i++;
    }

  for (int i=0; i<10; i++) {
    lp.Add(pair(i,i*i), true);
  }

  LIST l2(lp, lp);
  l2.Add(pair(3,5), true);

  std::cout << l2 << "\n";

  l2 = lp;
  std::cout << l2 << "\n";
  std::cout << "\n";
}
