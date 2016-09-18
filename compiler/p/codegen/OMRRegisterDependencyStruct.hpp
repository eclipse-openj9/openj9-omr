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

#ifndef OMR_Power_REGISTER_DEPENDENCY_STRUCT_INCL
#define OMR_Power_REGISTER_DEPENDENCY_STRUCT_INCL

/*
 * The following #define and typedef must appear before any #includes in this file
 */
#ifndef OMR_REGISTER_DEPENDENCY_STRUCT_CONNECTOR
#define OMR_REGISTER_DEPENDENCY_STRUCT_CONNECTOR
namespace OMR { namespace Power { struct RegisterDependencyExt; } }
namespace OMR { typedef OMR::Power::RegisterDependencyExt RegisterDependency; }
#else
#error OMR::Power::RegisterDependencyExt expected to be a primary connector, but a OMR connector is already defined
#endif

#include "compiler/codegen/OMRRegisterDependencyStruct.hpp"

#include "codegen/RealRegister.hpp"  // for RealRegister, etc

#define DefinesDependentRegister    0x01
#define ReferencesDependentRegister 0x02
#define UsesDependentRegister       (ReferencesDependentRegister | DefinesDependentRegister)
#define ExcludeGPR0InAssigner       0x80

namespace OMR
{

namespace Power
{

struct RegisterDependencyExt: OMR::RegisterDependencyExt
   {
   TR::RealRegister::RegNum  _realRegister;

   TR::RealRegister::RegNum getRealRegister() {return _realRegister;}
   TR::RealRegister::RegNum setRealRegister(TR::RealRegister::RegNum r) { return (_realRegister = r); }

   uint32_t getExcludeGPR0()    {return _flags & ExcludeGPR0InAssigner;}
   uint32_t setExcludeGPR0()    {return (_flags |= ExcludeGPR0InAssigner);}
   uint32_t resetExcludeGPR0()  {return (_flags &= ~ExcludeGPR0InAssigner);}


   };

}
}

#endif
