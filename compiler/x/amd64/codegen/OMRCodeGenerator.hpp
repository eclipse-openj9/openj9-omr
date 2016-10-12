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

#ifndef OMR_AMD64_CODEGENERATOR_INCL
#define OMR_AMD64_CODEGENERATOR_INCL

/*
 * The following #define and typedef must appear before any #includes in this file
 */
#ifndef OMR_CODEGENERATOR_CONNECTOR
#define OMR_CODEGENERATOR_CONNECTOR
namespace OMR { namespace X86 { namespace AMD64 { class CodeGenerator; } } }
namespace OMR { typedef OMR::X86::AMD64::CodeGenerator CodeGeneratorConnector; }
#else
#error OMR::X86::AMD64::CodeGenerator expected to be a primary connector, but a OMR connector is already defined
#endif

#include "compiler/x/codegen/OMRCodeGenerator.hpp"

#include "codegen/RealRegister.hpp"       // for TR::RealRegister::NumRegisters
#include "codegen/RegisterConstants.hpp"  // for TR_GlobalRegisterNumber

namespace TR { class ILOpCode; }
namespace TR { class Node; }

namespace OMR
{

namespace X86
{

namespace AMD64
{

class OMR_EXTENSIBLE CodeGenerator : public OMR::X86::CodeGenerator
   {
   public:

   CodeGenerator();

   virtual TR::Register *longClobberEvaluate(TR::Node *node);

   TR_GlobalRegisterNumber getLinkageGlobalRegisterNumber(int8_t linkageRegisterIndex, TR::DataType type);
   TR_BitVector *getGlobalGPRsPreservedAcrossCalls(){ return &_globalGPRsPreservedAcrossCalls; }
   TR_BitVector *getGlobalFPRsPreservedAcrossCalls(){ return &_globalFPRsPreservedAcrossCalls; }

   using OMR::X86::CodeGenerator::getMaximumNumberOfGPRsAllowedAcrossEdge;
   int32_t getMaximumNumberOfGPRsAllowedAcrossEdge(TR::Node *);

   bool opCodeIsNoOpOnThisPlatform(TR::ILOpCode &opCode);

   bool internalPointerSupportImplemented() { return true; }
   virtual bool allowVMThreadRematerialization() { return false; }

   protected:

   TR_BitVector _globalGPRsPreservedAcrossCalls;
   TR_BitVector _globalFPRsPreservedAcrossCalls;

   private:

   TR_GlobalRegisterNumber _gprLinkageGlobalRegisterNumbers[TR::RealRegister::NumRegisters], _fprLinkageGlobalRegisterNumbers[TR::RealRegister::NumRegisters];
   void initLinkageToGlobalRegisterMap();
   };

} // namespace AMD64

} // namespace X86

} // namespace OMR

#endif
