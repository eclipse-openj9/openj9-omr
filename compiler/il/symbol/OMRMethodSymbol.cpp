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

#include "il/symbol/OMRMethodSymbol.hpp"

#include "compile/Method.hpp"                  // for TR_Method
#include "env/TRMemory.hpp"
#include "il/symbol/MethodSymbol.hpp"          // for MethodSymbol, etc
#include "infra/Flags.hpp"                     // for flags32_t

OMR::MethodSymbol::MethodSymbol(TR_LinkageConventions lc, TR_Method * m) :
   TR::Symbol(),
   _methodAddress(NULL),
   _method(m),
   _linkageConvention(lc)
   {
   _flags.setValue(KindMask, IsMethod);
   }

bool
OMR::MethodSymbol::firstArgumentIsReceiver()
   {
   if (isSpecial() || isVirtual() || isInterface() || isComputedVirtual())
      return true;

   return false;
   }


TR::MethodSymbol *
OMR::MethodSymbol::self()
   {
   return static_cast<TR::MethodSymbol *>(this);
   }


/**
 * Method Symbol Factory.
 */
template <typename AllocatorType>
TR::MethodSymbol * OMR::MethodSymbol::create(AllocatorType t, TR_LinkageConventions lc, TR_Method * m)
   {
   return new (t) TR::MethodSymbol(lc, m);
   }

//Explicit instantiations
template TR::MethodSymbol * OMR::MethodSymbol::create(TR_HeapMemory t,          TR_LinkageConventions lc, TR_Method * m);
template TR::MethodSymbol * OMR::MethodSymbol::create(TR_StackMemory t,         TR_LinkageConventions lc, TR_Method * m);
template TR::MethodSymbol * OMR::MethodSymbol::create(PERSISTENT_NEW_DECLARE t, TR_LinkageConventions lc, TR_Method * m);
