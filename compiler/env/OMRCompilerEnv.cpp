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

#include "env/CompilerEnv.hpp"
#include "env/Environment.hpp"
#include "env/CPU.hpp"
#include "env/defines.h"


OMR::CompilerEnv::CompilerEnv(
   TR::RawAllocator raw,
   const TR::PersistentAllocatorKit &persistentAllocatorKit
   ) :
      rawAllocator(raw),
      _initialized(false),
      _persistentAllocator(persistentAllocatorKit),
      regionAllocator(_persistentAllocator)
   {
   }


TR::CompilerEnv *OMR::CompilerEnv::self()
   {
   return static_cast<TR::CompilerEnv *>(this);
   }


void
OMR::CompilerEnv::initialize()
   {
   self()->initializeHostEnvironment();

   self()->initializeTargetEnvironment();

   om.initialize();

   _initialized = true;
   }


void
OMR::CompilerEnv::initializeHostEnvironment()
   {

   // Host processor bitness
   //
#ifdef TR_HOST_64BIT
   host.setBitness(TR::bits_64);
#elif TR_HOST_32BIT
   host.setBitness(TR::bits_32);
#else
   host.setBitness(TR::bits_unknown);
#endif

   // Initialize the host CPU by querying the host processor
   //
   host.cpu.initializeByHostQuery();

   // Host major operating system
   //
#if HOST_OS == OMR_LINUX
   host.setMajorOS(TR::os_linux);
#elif HOST_OS == OMR_AIX
   host.setMajorOS(TR::os_aix);
#elif HOST_OS == OMR_WINDOWS
   host.setMajorOS(TR::os_windows);
#elif HOST_OS == OMR_ZOS
   host.setMajorOS(TR::os_zos);
#elif HOST_OS == OMR_OSX
   host.setMajorOS(TR::os_osx);
#else
   host.setMajorOS(TR::os_unknown);
#endif

   }


// Projects are encouraged to over-ride this function in their project
// extension.
//
// By default, the target will be initialized to the same environment
// as the host.
//
void
OMR::CompilerEnv::initializeTargetEnvironment()
   {

   // Target processor bitness
   //
#ifdef TR_TARGET_64BIT
   target.setBitness(TR::bits_64);
#elif TR_TARGET_32BIT
   target.setBitness(TR::bits_32);
#else
   target.setBitness(TR::bits_unknown);
#endif

   // Initialize the target CPU by querying the host processor
   //
   target.cpu.initializeByHostQuery();

   // Target major operating system
   //
#if HOST_OS == OMR_LINUX
   target.setMajorOS(TR::os_linux);
#elif HOST_OS == OMR_AIX
   target.setMajorOS(TR::os_aix);
#elif HOST_OS == OMR_WINDOWS
   target.setMajorOS(TR::os_windows);
#elif HOST_OS == OMR_ZOS
   target.setMajorOS(TR::os_zos);
#elif HOST_OS == OMR_OSX
   target.setMajorOS(TR::os_osx);
#else
   target.setMajorOS(TR::os_unknown);
#endif

   }
