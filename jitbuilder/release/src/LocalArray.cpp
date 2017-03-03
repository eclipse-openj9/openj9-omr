/******************************************************************************
 Copyright IBM Corp. 2016, 2016
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
#include "LocalArray.hpp"

static void printArray(int32_t numLongs, int64_t *array)
   {
   #define PRINTArray_LINE LINETOSTR(__LINE__)
   printf("printArray (%d) :\n", numLongs);
   for (int32_t i=0;i < numLongs;i++)
      printf("\t%lld\n", array[i]);
   }

static void printInt64(int64_t num)
   {
   #define PRINTInt64_LINE LINETOSTR(__LINE__)
   printf("%lld\n", num);
   }


LocalArrayMethod::LocalArrayMethod(TR::TypeDictionary *d)
   : MethodBuilder(d)
   {
   DefineLine(LINETOSTR(__LINE__));
   DefineFile(__FILE__);

   DefineName("testLocalArray");

   pInt64 = d->PointerTo(Int64);

   DefineParameter("notUsed",Int64);
   DefineReturnType(NoType);

   DefineFunction((char *)"printArray", 
                  (char *)__FILE__,
                  (char *)PRINTArray_LINE,
                  (void *)&printArray,
                  NoType,
                  2,
                  Int32,
                  pInt64);
   DefineFunction((char *)"printInt64", 
                  (char *)__FILE__,
                  (char *)PRINTInt64_LINE,
                  (void *)&printInt64,
                  NoType,
                  1,
                  Int64);
   }

bool
LocalArrayMethod::buildIL()
   {
   Store("myArray",
      CreateLocalArray(10, Int64));

   StoreAt(
      IndexAt(pInt64,
         Load("myArray"),
         ConstInt32(0)),
      ConstInt64(100));
   StoreAt(
      IndexAt(pInt64,
         Load("myArray"),
         ConstInt32(1)),
      ConstInt64(101));
   StoreAt(
      IndexAt(pInt64,
         Load("myArray"),
         ConstInt32(2)),
      ConstInt64(102));
   StoreAt(
      IndexAt(pInt64,
         Load("myArray"),
         ConstInt32(3)),
      ConstInt64(103));
   StoreAt(
      IndexAt(pInt64,
         Load("myArray"),
         ConstInt32(4)),
      ConstInt64(104));
   StoreAt(
      IndexAt(pInt64,
         Load("myArray"),
         ConstInt32(5)),
      ConstInt64(105));
   StoreAt(
      IndexAt(pInt64,
         Load("myArray"),
         ConstInt32(6)),
      ConstInt64(106));
   StoreAt(
      IndexAt(pInt64,
         Load("myArray"),
         ConstInt32(7)),
      ConstInt64(107));
   StoreAt(
      IndexAt(pInt64,
         Load("myArray"),
         ConstInt32(8)),
      ConstInt64(108));
   StoreAt(
      IndexAt(pInt64,
         Load("myArray"),
         ConstInt32(9)),
      ConstInt64(109));

   Call("printArray", 2,
      ConstInt32(10),
      Load("myArray"));

   Call("printInt64", 1,
      LoadAt(pInt64,
         IndexAt(pInt64,
            Load("myArray"),
            ConstInt32(9))));

   Call("printInt64", 1,
      LoadAt(pInt64,
         IndexAt(pInt64,
            Load("myArray"),
            ConstInt32(8))));

   Call("printInt64", 1,
      LoadAt(pInt64,
         IndexAt(pInt64,
            Load("myArray"),
            ConstInt32(7))));

   Call("printInt64", 1,
      LoadAt(pInt64,
         IndexAt(pInt64,
            Load("myArray"),
            ConstInt32(6))));

   Call("printInt64", 1,
      LoadAt(pInt64,
         IndexAt(pInt64,
            Load("myArray"),
            ConstInt32(5))));

   Call("printInt64", 1,
      LoadAt(pInt64,
         IndexAt(pInt64,
            Load("myArray"),
            ConstInt32(4))));

   Call("printInt64", 1,
      LoadAt(pInt64,
         IndexAt(pInt64,
            Load("myArray"),
            ConstInt32(3))));

   Call("printInt64", 1,
      LoadAt(pInt64,
         IndexAt(pInt64,
            Load("myArray"),
            ConstInt32(2))));

   Call("printInt64", 1,
      LoadAt(pInt64,
         IndexAt(pInt64,
            Load("myArray"),
            ConstInt32(1))));

   Call("printInt64", 1,
      LoadAt(pInt64,
         IndexAt(pInt64,
            Load("myArray"),
            ConstInt32(0))));

   Return();

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
   LocalArrayMethod method(&types);
   uint8_t *entry;
   int32_t rc = compileMethodBuilder(&method, &entry);
   if (rc != 0)
      {
      fprintf(stderr,"FAIL: compilation error %d\n", rc);
      exit(-2);
      }

   printf("Step 4: invoke compiled code and verify results\n");
   LocalArrayFunctionType *test = (LocalArrayFunctionType *) entry;
   test(0);

   printf ("Step 5: shutdown JIT\n");
   shutdownJit();

   printf("PASS\n");
   }
