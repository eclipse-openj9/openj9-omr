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

#ifndef STACK_INCL
#define STACK_INCL

#include <stdint.h>          // for int32_t, uint32_t
#include "env/TRMemory.hpp"  // for TR_AllocationKind, etc
#include "infra/Array.hpp"   // for TR_Array

// TR_Stack use TR_Array as a stack
//
template<class T> class TR_Stack : public TR_Array<T>
   {
public:
   TR_Stack(TR_Memory * m, uint32_t initialSize = 8, bool zeroInit = false, TR_AllocationKind allocKind = heapAlloc)
      : TR_Array<T>(m, initialSize, zeroInit, allocKind) { }
   TR_Stack(const TR_Stack<T>& other) : TR_Array<T>(other) { }
   TR_Stack<T> & operator=(const TR_Stack<T>& other) { (TR_Array<T>&)*this = (TR_Array<T>&)other; return *this; }

   int32_t  topIndex() { return TR_Array<T>::lastIndex(); }
   T &      top()      { return TR_Array<T>::element(topIndex()); }
   void     push(T t)  { TR_Array<T>::add(t); }
   T        pop()      { T t = top(); TR_Array<T>::remove(topIndex()); return t; }
   void     swap()     { T t = top(); top() = TR_Array<T>::element(topIndex() - 1); TR_Array<T>::element(topIndex() - 1) = t; }
   bool     isEmpty()  { return TR_Array<T>::size() == 0; }
   void     clear()    { TR_Array<T>::setSize(0); }

   // FIFO behaviour
   T &      bottom()   { return TR_Array<T>::element(0); }
   T        drop()     { T t = bottom(); TR_Array<T>::remove(0); return t; }

   };

#endif
