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

#ifndef OMR_REGISTER_DEPENDENCY_INCL
#define OMR_REGISTER_DEPENDENCY_INCL

/*
 * The following #define and typedef must appear before any #includes in this file
 */
#ifndef OMR_REGISTER_DEPENDENCY_CONNECTOR
#define OMR_REGISTER_DEPENDENCY_CONNECTOR
   namespace OMR { class RegisterDependencyConditions; }
   namespace OMR { typedef OMR::RegisterDependencyConditions RegisterDependencyConditionsConnector; }
#endif

#include "env/TRMemory.hpp"  // for TR_Memory, etc
#include "codegen/RegisterDependencyStruct.hpp"

namespace OMR
{

class RegisterDependencyConditions
   {
   protected:
   RegisterDependencyConditions() {};

   public:
   TR_ALLOC(TR_Memory::RegisterDependencyConditions)

   };

}

#endif
