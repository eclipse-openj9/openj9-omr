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

#ifndef ARM_SYSTEMLINKAGE_INCL
#define ARM_SYSTEMLINKAGE_INCL

#include "codegen/Linkage.hpp"

#include "infra/Assert.hpp"

namespace TR { class AutomaticSymbol; }
namespace TR { class CodeGenerator; }
namespace TR { class Instruction; }
namespace TR { class MemoryReference; }
namespace TR { class Node; }
namespace TR { class Register; }
namespace TR { class RegisterDependencyConditions; }
namespace TR { class ResolvedMethodSymbol; }

namespace TR {

class ARMSystemLinkage : public TR::Linkage
   {
   static TR::ARMLinkageProperties properties;

   public:

   ARMSystemLinkage(TR::CodeGenerator *codeGen) : TR::Linkage(codeGen) {}

   virtual uint32_t getRightToLeft();
   virtual void mapStack(TR::ResolvedMethodSymbol *method);
   virtual void mapSingleAutomatic(TR::AutomaticSymbol *p, uint32_t &stackIndex);
   virtual void initARMRealRegisterLinkage();

   virtual TR::MemoryReference *getOutgoingArgumentMemRef(int32_t               totalParmAreaSize,
                                                            int32_t               argOffset,
                                                            TR::Register          *argReg,
                                                            TR_ARMOpCodes         opCode,
                                                            TR::ARMMemoryArgument &memArg);

   virtual TR::ARMLinkageProperties& getProperties();

   virtual void createPrologue(TR::Instruction *cursor);
   virtual void createEpilogue(TR::Instruction *cursor);

   virtual int32_t buildArgs(TR::Node                            *callNode,
                             TR::RegisterDependencyConditions *dependencies,
                             TR::Register*                       &vftReg,
                             bool                                isJNI);

   virtual TR::Register *buildDirectDispatch(TR::Node *callNode);
   virtual TR::Register *buildIndirectDispatch(TR::Node *callNode);
   };

}

#endif
