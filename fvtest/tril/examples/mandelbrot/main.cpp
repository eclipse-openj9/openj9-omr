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

#include "ilgen/TypeDictionary.hpp"
#include "env/jittypes.h"
#include "il/DataTypes.hpp"
#include "il/ILOpCodes.hpp"
#include "compile/Compilation.hpp"
#include "compile/CompilationTypes.hpp"
#include "compile/Method.hpp"
#include "control/CompileMethod.hpp"
#include "env/jittypes.h"
#include "gtest/gtest.h"
#include "il/DataTypes.hpp"
#include "ilgen.hpp"
#include "Jit.hpp"
#include "ilgen/IlGeneratorMethodDetails_inlines.hpp"

using TreeMethodFunction = void(int32_t, int32_t, int32_t*);

int main(int argc, char const * const * const argv) {
   initializeJit();

   TR::TypeDictionary types;

   for (int i = 1; i < argc; ++i) {
      FILE* inputFile = fopen(argv[i], "r");
      ASTNode* trees = genTrees(inputFile);
      printf("parsed trees:\n");
      printTrees(trees, 0);

      TRLangBuilder builder{trees, &types};
      auto Int32 = types.PrimitiveType(TR::Int32);
      auto argTypes = new TR::IlType*[3];
      argTypes[0] = Int32;
      argTypes[1] = Int32;
      argTypes[2] = types.PointerTo(Int32);

      TR::ResolvedMethod compilee(__FILE__, LINETOSTR(__LINE__), "mandelbrot", 3, argTypes, Int32, 0, &builder);
      TR::IlGeneratorMethodDetails methodDetails(&compilee);

      int32_t rc = 0;
      auto entry = compileMethodFromDetails(NULL, methodDetails, warm, rc);
      auto treeMethod = (TreeMethodFunction*)entry;


      if (rc == 0) {
         constexpr auto size = 80;
         int32_t table[size][size] = {{0}};
         treeMethod(1000, size, table[0]);
         for (auto row = 0; row < size; ++row) {
            for (auto i = 0; i < size; ++i) {
#if defined(FULL_COLOUR)
               auto c = table[row][i] < 1000 ? '#' : ' ';
               int colors[] = {1, 1, 5, 4, 6, 2, 3, 3, 3, 3};
               printf(" \e[0;3%dm%c\e[0m ", colors[table[row][i] % 10], c);
#elif defined(SIMPLE_COLOUR)
               auto c = table[row][i] >= 1000 ? '#' : ' ';
               int colors[] = {0, 1, 3, 2, 6, 4, 5, 5, 5};
               printf(" \e[0;3%dm%c\e[0m ", colors[i / 10], c);
#else
               auto c = table[row][i] >= 1000 ? '#' : ' ';
               printf(" %c ", c );
#endif
            }
            printf("\n");
         }
      }
   }

   shutdownJit();
   return 0;
}
