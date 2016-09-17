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

#ifndef OMR_CPU_INCL
#define OMR_CPU_INCL

/*
 * The following #define and typedef must appear before any #includes in this file
 */
#ifndef OMR_CPU_CONNECTOR
#define OMR_CPU_CONNECTOR
namespace OMR { class CPU; }
namespace OMR { typedef OMR::CPU CPUConnector; }
#endif

#include "env/Processors.hpp"  // for TR_Processor, etc

namespace TR { class CPU; }

namespace TR
{
// Processor endianness
//
enum Endianness
   {
   endian_big,
   endian_little,
   endian_unknown
   };


// Major CPU architecture
//
enum MajorArchitecture
   {
   arch_x86,
   arch_z,
   arch_power,
   arch_arm,
   arch_unknown
   };


// Minor CPU architecture
//
enum MinorArchitecture
   {
   m_arch_none,
   m_arch_i386,
   m_arch_amd64
   };

}


namespace OMR
{

class CPU
   {
protected:

   CPU() :
         _processor(TR_NullProcessor),
         _endianness(TR::endian_unknown),
         _majorArch(TR::arch_unknown),
         _minorArch(TR::m_arch_none)
      {}

public:

   TR::CPU *self();

   // Initialize CPU info by querying the host processor at compile-time
   //
   void initializeByHostQuery();

   TR_Processor setProcessor(TR_Processor p) { return(_processor = p); }

   // Processor identity and generation comparisons
   //
   TR_Processor id() { return _processor; }
   bool is(TR_Processor p) { return _processor == p; }
   bool isNot(TR_Processor p) { return _processor != p; }
   bool isAtLeast(TR_Processor p) { return _processor >= p; }
   bool isLaterThan(TR_Processor p) { return _processor > p; }
   bool isEarlierThan(TR_Processor p) { return _processor < p; }
   bool isAtMost(TR_Processor p) { return _processor <= p; }

   bool getSupportsHardwareSQRT() { return false; }
   bool getSupportsHardwareRound() { return false; }
   bool getSupportsHardwareCopySign() { return false; }
   bool hasPopulationCountInstruction() { return false; }
   bool supportsDecimalFloatingPoint() { return false; }
   bool hasFPU() { return true; }

   TR::Endianness endianness() { return _endianness; }
   void setEndianness(TR::Endianness e) { _endianness = e; }
   bool isBigEndian() { return _endianness == TR::endian_big; }
   bool isLittleEndian() { return _endianness == TR::endian_little; }

   TR::MajorArchitecture majorArch() { return _majorArch; }
   void setMajorArch(TR::MajorArchitecture a) { _majorArch = a; }
   bool isX86() { return _majorArch == TR::arch_x86; }
   bool isZ() { return _majorArch == TR::arch_z; }
   bool isPower() { return _majorArch == TR::arch_power; }
   bool isARM() { return _majorArch == TR::arch_arm; }

   TR::MinorArchitecture minorArch() { return _minorArch; }
   void setMinorArch(TR::MinorArchitecture a) { _minorArch = a; }
   bool isI386() { return _minorArch == TR::m_arch_i386; }
   bool isAMD64() { return _minorArch == TR::m_arch_amd64; }

private:
   TR_Processor _processor;

   TR::Endianness _endianness;
   TR::MajorArchitecture _majorArch;
   TR::MinorArchitecture _minorArch;
   };
}

#endif
