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

#ifndef OMR_ENVIRONMENT_INCL
#define OMR_ENVIRONMENT_INCL

/*
 * The following #define and typedef must appear before any #includes in this file
 */
#ifndef OMR_ENVIRONMENT_CONNECTOR
#define OMR_ENVIRONMENT_CONNECTOR
namespace OMR { class Environment; }
namespace OMR { typedef OMR::Environment EnvironmentConnector; }
#endif

#include "env/CPU.hpp"
#include <stdint.h>


namespace TR
{

// Address bitness of the process in this environment
//
enum Bitness
   {
   bits_32,
   bits_64,
   bits_unknown
   };


// Major operating system
//
enum MajorOperatingSystem
   {
   os_linux,
   os_aix,
   os_unix,
   os_windows,
   os_zos,
   os_osx,
   os_unknown
   };

}




namespace OMR
{

class Environment
   {

public:

   Environment() :
         _majorOS(TR::os_unknown),
         _bitness(TR::bits_unknown),
         _isSMP(false),
         _numberOfProcessors(1),
         cpu()
      {}

   Environment(TR::MajorOperatingSystem o, TR::Bitness b) :
         _majorOS(o),
         _bitness(b),
         _isSMP(false),
         _numberOfProcessors(1),
         cpu()
      {}

   TR::CPU cpu;

   TR::MajorOperatingSystem majorOS() { return _majorOS; }
   void setMajorOS(TR::MajorOperatingSystem os) { _majorOS = os; }
   bool isWindows() { return _majorOS == TR::os_windows; }
   bool isLinux() { return _majorOS == TR::os_linux; }
   bool isAIX() { return _majorOS == TR::os_aix; }
   bool isUnix() { return _majorOS == TR::os_unix; }
   bool isZOS() { return _majorOS == TR::os_zos; }
   bool isOSX() { return _majorOS == TR::os_osx; }

   TR::Bitness bitness() { return _bitness; }
   void setBitness(TR::Bitness b) { _bitness = b; }
   bool is32Bit() { return _bitness == TR::bits_32; }
   bool is64Bit() { return _bitness == TR::bits_64; }

   bool isSMP() { return _isSMP; }
   void setSMP(bool s) { _isSMP = s; }

   uint32_t numberOfProcessors() { return _numberOfProcessors; }
   void setNumberOfProcessors(uint32_t p) { _numberOfProcessors = p; }

private:

   TR::MajorOperatingSystem _majorOS;

   TR::Bitness _bitness;

   bool _isSMP;

   uint32_t _numberOfProcessors;
   };

}

#endif
