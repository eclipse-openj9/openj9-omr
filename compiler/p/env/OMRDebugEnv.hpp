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

#ifndef OMR_POWER_DEBUG_ENV_INCL
#define OMR_POWER_DEBUG_ENV_INCL

/*
 * The following #define and typedef must appear before any #includes in this file
 */
#ifndef OMR_DEBUG_ENV_CONNECTOR
#define OMR_DEBUG_ENV_CONNECTOR
namespace OMR { namespace Power { class DebugEnv; } }
namespace OMR { typedef OMR::Power::DebugEnv DebugEnvConnector; }
#else
#error OMR::Power::DebugEnv expected to be a primary connector, but an OMR connector is already defined
#endif

#include "compiler/env/OMRDebugEnv.hpp"
#include "infra/Annotations.hpp"

namespace OMR
{

namespace Power
{

class OMR_EXTENSIBLE DebugEnv : public OMR::DebugEnv
   {
public:

   DebugEnv();

   };

}

}

#endif
