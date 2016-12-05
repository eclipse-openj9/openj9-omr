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


#ifndef ATOMICOPS_INCL
#define ATOMICOPS_INCL

#include "ilgen/MethodBuilder.hpp"

/**
 * I created two methodBuilders for testing
 * Int32 / Int64 atomicadd() respectively
 */

class AtomicInt32Add : public TR::MethodBuilder
   {
   private:
   TR::IlType *pInt32;

   public:
   AtomicInt32Add(TR::TypeDictionary *);
   virtual bool buildIL();
   };

class AtomicInt64Add : public TR::MethodBuilder
   {   
   private:
   TR::IlType *pInt64;

   public:
   AtomicInt64Add(TR::TypeDictionary *); 
   virtual bool buildIL();
   };

#endif // !defined(ATOMICOPS_INCL)
