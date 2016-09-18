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
#include "NestedLoop.hpp"

NestedLoopMethod::NestedLoopMethod(TR::TypeDictionary *types)
   : MethodBuilder(types)
   {
   DefineLine(LINETOSTR(__LINE__));
   DefineFile(__FILE__);

   DefineName("nested_loop");
   DefineParameter("n", Int32);
   DefineReturnType(Int32);
   }

bool
NestedLoopMethod::buildIL()
   {
   Store("x",
      ConstInt32(0));

   IlBuilder *aLoop=NULL;
   ForLoopUp("a", &aLoop,
             ConstInt32(0),
             Load("n"),
             ConstInt32(1));

   IlBuilder *bLoop=NULL;
   aLoop->ForLoopUp("b", &bLoop,
   aLoop->          ConstInt32(0),
   aLoop->          Load("n"),
   aLoop->          ConstInt32(1));

   IlBuilder *cLoop=NULL;
   bLoop->ForLoopUp("c", &cLoop,
   bLoop->          ConstInt32(0),
   bLoop->          Load("n"),
   bLoop->          ConstInt32(1));

   IlBuilder *dLoop=NULL;
   cLoop->ForLoopUp("d", &dLoop,
   cLoop->          ConstInt32(0),
   cLoop->          Load("n"),
   cLoop->          ConstInt32(1));

   IlBuilder *eLoop=NULL;
   dLoop->ForLoopUp("e", &eLoop,
   dLoop->          ConstInt32(0),
   dLoop->          Load("n"),
   dLoop->          ConstInt32(1));

   IlBuilder *fLoop=NULL;
   eLoop->ForLoopUp("f", &fLoop,
   eLoop->          ConstInt32(0),
   eLoop->          Load("n"),
   eLoop->          ConstInt32(1));

   fLoop->Store("x",
   fLoop->      Add(
   fLoop->         Load("x"),
   fLoop->         ConstInt32(1)));

   Return(
      Load("x"));

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

   printf("Step 2: define relevant types\n");
   TR::TypeDictionary types;

   printf("Step 3: compile method builder\n");
   NestedLoopMethod nestedLoop(&types);
   uint8_t *entry=0;
   int32_t rc = compileMethodBuilder(&nestedLoop, &entry);
   if (rc != 0)
      {
      fprintf(stderr,"FAIL: compilation error %d\n", rc);
      exit(-2);
      }

   printf("Step 4: invoke compiled code\n");
   NestedLoopFunctionType *nested_loop = (NestedLoopFunctionType *)entry;
   for (int32_t n=0;n < 20;n++)
      printf("nested_loop(%2d) = %d\n", n, nested_loop(n));

   printf ("Step 5: shutdown JIT\n");
   shutdownJit();

   printf("PASS\n");
   }
