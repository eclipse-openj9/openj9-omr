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

#ifndef OMR_REGISTER_USAGE_INCL
#define OMR_REGISTER_USAGE_INCL

#include "env/TRMemory.hpp"  // for TR_Memory, etc
#include "il/Node.hpp"       // for ncount_t

namespace TR { class Register; }

namespace OMR
{

class RegisterUsage
   {
   public:
   TR_ALLOC(TR_Memory::Register)

   RegisterUsage(TR::Register *v, ncount_t u): virtReg(v), useCount(u), mergeFuc(0) {}

   TR::Register *   virtReg;
   ncount_t     useCount;
   ncount_t     mergeFuc;  // cache a virtual's original future use
   };

}

#endif
