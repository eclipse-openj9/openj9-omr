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


#ifndef CALL_INCL
#define CALL_INCL

#include "ilgen/MethodBuilder.hpp"

typedef int32_t (CallFunctionType)(int32_t);

class CallMethod : public TR::MethodBuilder
   {
   public:
   CallMethod(TR::TypeDictionary *types);
   virtual bool buildIL();
   };

class ComputedCallMethod : public TR::MethodBuilder
   {
   public:
   ComputedCallMethod(TR::TypeDictionary *types);
   virtual bool buildIL();
   };

#endif // !defined(CALL_INCL)
