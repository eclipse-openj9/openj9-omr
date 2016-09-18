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
#include <cstdio>
#include "cs2/dgraph.h"

int main() {

  CS2::DirectedGraph<CS2::DirectedGraphNode, CS2::DirectedGraphEdge, CS2::check_allocator<CS2::malloc_allocator> > g;

  std::cout << "G=" << g << "\n";

  g.AddEdge(g.AddNode(), g.AddNode());

  std::cout << "G=" << g << "\n";

}
