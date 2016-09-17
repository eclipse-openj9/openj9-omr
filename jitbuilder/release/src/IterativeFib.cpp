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


#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <dlfcn.h>
#include <errno.h>

#include "Jit.hpp"
#include "ilgen/TypeDictionary.hpp"
#include "ilgen/MethodBuilder.hpp"
#include "IterativeFib.hpp"

IterativeFibonnaciMethod::IterativeFibonnaciMethod(TR::TypeDictionary *types)
   : MethodBuilder(types)
   {
   DefineLine(LINETOSTR(__LINE__));
   DefineFile(__FILE__);

   DefineName("fib_iter"); // defines _method
   DefineParameter("n", Int32);
   DefineReturnType(Int32);
   }

bool
IterativeFibonnaciMethod::buildIL()
   {
   TR::IlBuilder *returnN = NULL;
   IfThen(&returnN,
      LessThan(
         Load("n"),
         ConstInt32(2)));

   returnN->Return(
   returnN->   Load("n"));

   Store("LastSum",
      ConstInt32(0));

   Store("Sum",
      ConstInt32(1));

   TR::IlBuilder *iloop = NULL;
   ForLoopUp("i", &iloop,
           ConstInt32(1),
           Load("n"),
           ConstInt32(1));

   iloop->Store("tempSum",
   iloop->   Add(
   iloop->      Load("Sum"),
   iloop->      Load("LastSum")));
   iloop->Store("LastSum",
   iloop->   Load("Sum"));
   iloop->Store("Sum",
   iloop->   Load("tempSum"));

   Return(
      Load("Sum"));

   return true;
   }


int
main(int argc, char *argv[])
   {
   printf("Step 1: initialize JIT\n");
   bool initialized = initializeJit();
   if (!initialized)
      {
      fprintf(stderr, "FAIL: could not initialize JIT\n");
      exit(-1);
      }

   printf("Step 2: define type dictionary\n");
   TR::TypeDictionary types;

   printf("Step 3: compile method builder\n");
   IterativeFibonnaciMethod iterFibMethodBuilder(&types);
   uint8_t *entry=0;
   int32_t rc = compileMethodBuilder(&iterFibMethodBuilder, &entry);
   if (rc != 0)
      {
      fprintf(stderr,"FAIL: compilation error %d\n", rc);
      exit(-2);
      }

   printf("Step 4: invoke compiled code\n");
   IterativeFibFunctionType *iter_fib=(IterativeFibFunctionType *)entry;
   for (int32_t n=0;n < 20;n++)
      printf("fib(%2d) = %d\n", n, iter_fib(n));

   printf ("Step 5: shutdown JIT\n");
   shutdownJit();

   printf("PASS\n");
   }
