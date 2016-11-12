/*******************************************************************************
 *
 * (c) Copyright IBM Corp. 2016, 2016
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


/**
 * This file containse JitBuilder methods that are only useful as tests and
 * not as examples of how to use JitBuilder
 */

#include <iostream>
#include <stdlib.h>
#include <stdint.h>
#include <dlfcn.h>
#include <errno.h>

#include "Jit.hpp"
#include "ilgen/TypeDictionary.hpp"
#include "ilgen/MethodBuilder.hpp"
#include "ilgen/BytecodeBuilder.hpp"
#include "ControlFlowTests.hpp"

using std::cout;
using std::cerr;


DoubleReturnBytecodeMethod::DoubleReturnBytecodeMethod(TR::TypeDictionary* d)
   : MethodBuilder(d)
   {
   DefineLine(LINETOSTR(__LINE__));
   DefineFile(__FILE__);

   DefineName("doubleReturn");
   DefineReturnType(NoType);
   }

bool
DoubleReturnBytecodeMethod::buildIL()
   {
   auto firstBuilder = OrphanBytecodeBuilder(0, (char *)"return");
   AppendBuilder(firstBuilder);
   firstBuilder->Return();

   auto secondBuilder = OrphanBytecodeBuilder(1, (char *)"return"); // should be ignored and cleanedup
   secondBuilder->Return();

   return true;
   }


int
main(int argc, char *argv[])
   {
   cout << "Step 1: initialize JIT\n";
   bool initialized = initializeJit();
   if (!initialized)
      {
      cerr << "FAIL: could not initialize JIT\n";
      exit(-1);
      }

   cout << "Step 2: define type dictionary\n";
   TR::TypeDictionary types;

   cout << "Step 3: compile method builder\n";
   DoubleReturnBytecodeMethod method(&types);
   uint8_t *entry = 0;
   int32_t rc = compileMethodBuilder(&method, &entry);
   if (rc != 0)
      {
      cerr << "FAIL: compilation error " << rc << "\n";
      exit(-2);
      }

   cout << "Step 4: invoke compiled code\n";
   DoubleReturnBytecodeMethodFunction *doubleReturn = (DoubleReturnBytecodeMethodFunction *) entry;
   doubleReturn();

   cout << "Step 5: shutdown JIT\n";
   shutdownJit();
   }
