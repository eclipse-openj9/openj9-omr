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

#ifndef OMR_REGISTER_DEPENDENCY_STRUCT_INCL
#define OMR_REGISTER_DEPENDENCY_STRUCT_INCL

/*
 * The following #define and typedef must appear before any #includes in this file
 */
#ifndef OMR_REGISTER_DEPENDENCY_STRUCT_CONNECTOR
#define OMR_REGISTER_DEPENDENCY_STRUCT_CONNECTOR
namespace OMR { struct RegisterDependencyExt; }
namespace OMR { typedef OMR::RegisterDependencyExt RegisterDependency; }
#endif

#include <stdint.h>  // for uint32_t, uint8_t
#include "codegen/Register.hpp"

#define DefinesDependentRegister    0x01
#define ReferencesDependentRegister 0x02
#define UsesDependentRegister      (ReferencesDependentRegister | DefinesDependentRegister)

namespace OMR
{

struct RegisterDependencyExt
   {
   uint8_t                                 _flags;
   TR::Register                            *_virtualRegister;

   uint32_t getFlags()             {return _flags;}
   uint32_t assignFlags(uint8_t f) {return _flags = f;}
   uint32_t setFlags(uint8_t f)    {return _flags |= f;}
   uint32_t resetFlags(uint8_t f)  {return _flags &= ~f;}

   uint32_t getDefsRegister()   {return _flags & DefinesDependentRegister;}
   uint32_t setDefsRegister()   {return _flags |= DefinesDependentRegister;}
   uint32_t resetDefsRegister() {return _flags &= ~DefinesDependentRegister;}

   uint32_t getRefsRegister()   {return _flags & ReferencesDependentRegister;}
   uint32_t setRefsRegister()   {return _flags |= ReferencesDependentRegister;}
   uint32_t resetRefsRegister() {return _flags &= ~ReferencesDependentRegister;}

   uint32_t getUsesRegister()   {return _flags & UsesDependentRegister;}

   TR::Register *getRegister()               {return _virtualRegister;}
   TR::Register *setRegister(TR::Register *r) {return (_virtualRegister = r);}

   };
}

#endif
