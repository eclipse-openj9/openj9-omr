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

#ifndef OMR_X86_REGISTER_DEPENDENCY_STRUCT_INCL
#define OMR_X86_REGISTER_DEPENDENCY_STRUCT_INCL

/*
 * The following #define and typedef must appear before any #includes in this file
 */
#ifndef OMR_REGISTER_DEPENDENCY_STRUCT_CONNECTOR
#define OMR_REGISTER_DEPENDENCY_STRUCT_CONNECTOR
namespace OMR { namespace X86 { struct RegisterDependencyExt; } }
namespace OMR { typedef OMR::X86::RegisterDependencyExt RegisterDependency; }
#else
#error OMR::X86::RegisterDependencyExt expected to be a primary connector, but a OMR connector is already defined
#endif

#include "compiler/codegen/OMRRegisterDependencyStruct.hpp"

#include <stdint.h>                  // for int32_t, uint32_t
#include "codegen/RealRegister.hpp"  // for TR::RealRegister::RegNum

#define GlobalRegisterFPDependency    0x04

class TR_X86RegisterDependencyIndex
   {
   int32_t _index;

   public:

   TR_X86RegisterDependencyIndex(int32_t index):_index(index){}

   operator int32_t() const { return _index; }
   TR_X86RegisterDependencyIndex operator++()    { ++_index; return *this; }
   TR_X86RegisterDependencyIndex operator--()    { --_index; return *this; }
   TR_X86RegisterDependencyIndex operator++(int) { int32_t oldIndex = _index; _index++; return oldIndex; }
   TR_X86RegisterDependencyIndex operator--(int) { int32_t oldIndex = _index; _index--; return oldIndex; }

   };

namespace OMR
{
namespace X86
{

struct RegisterDependencyExt : OMR::RegisterDependencyExt
   {
   TR::RealRegister::RegNum _realRegister;

   TR::RealRegister::RegNum getRealRegister() {return _realRegister;}
   TR::RealRegister::RegNum setRealRegister(TR::RealRegister::RegNum r) { return (_realRegister = r); }

   uint32_t getGlobalFPRegister()   {return _flags & GlobalRegisterFPDependency;}
   uint32_t setGlobalFPRegister()   {return (_flags |= GlobalRegisterFPDependency);}

   };
}
}

#endif
