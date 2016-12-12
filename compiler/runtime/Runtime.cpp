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
 *******************************************************************************/

#include "runtime/Runtime.hpp"

TR_RuntimeHelperTable runtimeHelpers;

#if (defined(__IBMCPP__) || defined(__IBMC__) && !defined(MVS)) && !defined(LINUXPPC64)
 #if defined(AIXPPC)
  #define JIT_HELPER(x) extern "C" void *x
 #else
  #define JIT_HELPER(x) extern "C" __cdecl x()
 #endif
#else
 #if defined(LINUXPPC64)
  #define JIT_HELPER(x) extern "C" void *x
 #elif defined(LINUX)
  #define JIT_HELPER(x) extern "C" void x()
 #else
  #define JIT_HELPER(x) extern "C" x()
 #endif
#endif

extern "C" void *PyTuple_GetItem(void*,int);

void*
TR_RuntimeHelperTable::translateAddress(void * a)
   {
   return a;
   }

void
initializeJitRuntimeHelperTable(char isSMP)
   {
   //runtimeHelpers.setAddress(PyHelper_TupleGetItem,                    (void*)PyTuple_GetItem);
   }

