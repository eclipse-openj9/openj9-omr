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

#ifndef TR_COMPILER_ENV_INCL
#define TR_COMPILER_ENV_INCL

#include "env/OMRCompilerEnv.hpp"
#include "env/RawAllocator.hpp"
#include "infra/Annotations.hpp"  // for OMR_EXTENSIBLE

namespace TR
{

class OMR_EXTENSIBLE CompilerEnv : public OMR::CompilerEnvConnector
   {
public:
   CompilerEnv(TR::RawAllocator raw, const TR::PersistentAllocatorKit &persistentAllocatorKit) :
         OMR::CompilerEnvConnector(raw, persistentAllocatorKit)
      {}
   };

extern CompilerEnv *Compiler;

}

#endif
