/*******************************************************************************
 *
 * (c) Copyright IBM Corp. 2000, 2017
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

#ifndef OMR_VMMETHODENV_INCL
#define OMR_VMMETHODENV_INCL

/*
 * The following #define and typedef must appear before any #includes in this file
 */
#ifndef OMR_VMMETHODENV_CONNECTOR
#define OMR_VMMETHODENV_CONNECTOR
namespace OMR { class VMMethodEnv; }
namespace OMR { typedef OMR::VMMethodEnv VMMethodEnvConnector; }
#endif

#include <stdint.h>        // for int32_t, int64_t, uint32_t
#include "infra/Annotations.hpp"
#include "env/jittypes.h"


namespace OMR
{

class OMR_EXTENSIBLE VMMethodEnv
   {
public:

   /**
    * \brief  Does this method contain any backward branches?
    * \return true if there are backward branches, false if there aren't or unknown
    */
   bool hasBackwardBranches(TR_OpaqueMethodBlock *method) { return false; }

   /**
    * \brief  Is this method compiled?
    * \return true if compiled, false if not or unknown
    */ 
   bool isCompiledMethod(TR_OpaqueMethodBlock *method) { return false; }

   /**
    * \brief  Ask for the start PC of the provided method
    * \return the start PC, or 0 if unknown or not compiled
    */
   uintptr_t startPC(TR_OpaqueMethodBlock *method) { return 0; }
   };

}

#endif
