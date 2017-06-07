/*******************************************************************************
 *
 * (c) Copyright IBM Corp. 2017, 2017
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

#include "ilgen.hpp"
#include "Jit.hpp"

int main(int argc, char const * const * const argv) {
   initializeJit();

   TR::TypeDictionary types;

   for (int i = 1; i < argc; ++i) {
      FILE* inputFile = fopen(argv[i], "r");
      ASTNode* trees = genTrees(inputFile);
      printf("parsed trees:\n");
      printTrees(trees, 0);
      TRLangBuilder builder{trees, &types};
      uint8_t* entry;
      auto rc = compileMethodBuilder(&builder, &entry);
      auto treeMethod = (TreeMethodFunction*)entry;
      if (rc == 0) {
         auto a = 1;
         printf("treeMethod(%d) = %d\n", a, treeMethod(&a));
         a = 2;
         printf("treeMethod(%d) = %d\n", a, treeMethod(&a));
         a = -1;
         printf("treeMethod(%d) = %d\n", a, treeMethod(&a));
         a = -2;
         printf("treeMethod(%d) = %d\n", a, treeMethod(&a));
      }
   }

   shutdownJit();
   return 0;
}
