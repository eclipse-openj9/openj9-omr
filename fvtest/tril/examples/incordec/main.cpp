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

#include "method_handler.hpp"
#include "Jit.hpp"

typedef int32_t (TreeMethodFunction)(int32_t*);

int main(int argc, char const * const * const argv) {
   initializeJit();

   for (unsigned int i = 1; i < argc; ++i) {
      FILE* inputFile = fopen(argv[i], "r");
      ASTNode* trees = parseFile(inputFile);
      printf("parsed trees:\n");
      printTrees(trees, 0);

      MethodHandler incordec{trees};
      auto rc = incordec.compile();
      auto treeMethod = incordec.getEntryPoint<TreeMethodFunction*>();

      if (rc == 0) {
         int32_t value = 1;
         printf("%d -> %d\n", value, treeMethod(&value));
         value = 2;
         printf("%d -> %d\n", value, treeMethod(&value));
         value = -1;
         printf("%d -> %d\n", value, treeMethod(&value));
         value = -2;
         printf("%d -> %d\n", value, treeMethod(&value));
      }

      fclose(inputFile);
   }

   shutdownJit();
   return 0;
}
