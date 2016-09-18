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


#ifndef LOCALARRAY_INCL
#define LOCALARRAY_INCL

#include "ilgen/MethodBuilder.hpp"

namespace TR { class TypeDictionary; }

typedef void (LocalArrayFunctionType)(int64_t);

class LocalArrayMethod : public TR::MethodBuilder
   {
   private:

   TR::IlType *pInt64;

   public:
   LocalArrayMethod(TR::TypeDictionary *);
   virtual bool buildIL();
   };

#endif // !defined(LOCALARRAY_INCL)
