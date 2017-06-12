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

typedef int32_t (TreeMethodFunction)(int32_t*);

int main(int argc, char const * const * const argv) {
   initializeJit();

   TR::TypeDictionary types;

   for (unsigned int i = 1; i < argc; ++i) {
      FILE* inputFile = fopen(argv[i], "r");
      ASTNode* trees = genTrees(inputFile);
      printf("parsed trees:\n");
      printTrees(trees, 0);

      TRLangBuilder builder{trees, &types};
      auto Int32 = types.PrimitiveType(TR::Int32);
      auto argTypes = new TR::IlType*[1];
      argTypes[0] = types.PointerTo(Int32);

      TR::ResolvedMethod compilee(__FILE__, LINETOSTR(__LINE__), "incordec", 1, argTypes, Int32, 0, &builder);
      TR::IlGeneratorMethodDetails methodDetails(&compilee);

      int32_t rc = 0;
      auto entry = compileMethodFromDetails(NULL, methodDetails, warm, rc);
      auto treeMethod = (TreeMethodFunction*)entry;

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
