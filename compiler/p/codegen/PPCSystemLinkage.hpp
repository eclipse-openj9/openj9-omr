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

#ifndef OMR_POWER_SYSTEMLINKAGE_INCL
#define OMR_POWER_SYSTEMLINKAGE_INCL

#include "codegen/Linkage.hpp"

#include <stdint.h>                         // for uint32_t, uintptr_t, etc
#include "codegen/Register.hpp"             // for Register

namespace TR { class AutomaticSymbol; }
namespace TR { class CodeGenerator; }
namespace TR { class Instruction; }
namespace TR { class Node; }
namespace TR { class ParameterSymbol; }
namespace TR { class RegisterDependencyConditions; }
namespace TR { class ResolvedMethodSymbol; }
namespace TR { class SymbolReference; }
template <class T> class List;

class TR_PPCSystemLinkage : public TR::Linkage
   {
   protected:

   TR_PPCLinkageProperties _properties;

   public:

   TR_PPCSystemLinkage(TR::CodeGenerator *cg);

   virtual const TR_PPCLinkageProperties& getProperties();
   virtual uintptr_t calculateActualParameterOffset(uintptr_t, TR::ParameterSymbol&);
   virtual uintptr_t calculateParameterRegisterOffset(uintptr_t, TR::ParameterSymbol&);

   virtual uint32_t getRightToLeft();
   virtual bool hasToBeOnStack(TR::ParameterSymbol *parm);
   virtual void mapStack(TR::ResolvedMethodSymbol *method);
   virtual void mapSingleAutomatic(TR::AutomaticSymbol *p, uint32_t &stackIndex);
   virtual void initPPCRealRegisterLinkage();

   virtual void createPrologue(TR::Instruction *cursor);
   virtual void createPrologue(TR::Instruction *cursor, List<TR::ParameterSymbol> &parm);

   virtual void createEpilogue(TR::Instruction *cursor);

   virtual int32_t buildArgs(
         TR::Node *callNode,
         TR::RegisterDependencyConditions *dependencies);

   virtual void buildVirtualDispatch(
         TR::Node *callNode,
         TR::RegisterDependencyConditions *dependencies,
         uint32_t sizeOfArguments);

   void buildDirectCall(
         TR::Node *callNode,
         TR::SymbolReference *callSymRef,
         TR::RegisterDependencyConditions *dependencies,
         const TR_PPCLinkageProperties &pp,
         int32_t argSize);

   virtual TR::Register *buildDirectDispatch(TR::Node *callNode);

   virtual TR::Register *buildIndirectDispatch(TR::Node *callNode);

   virtual void setParameterLinkageRegisterIndex(TR::ResolvedMethodSymbol *method);
   virtual void setParameterLinkageRegisterIndex(TR::ResolvedMethodSymbol *method, List<TR::ParameterSymbol> &parmList);
   virtual void mapParameters(TR::ResolvedMethodSymbol *method, List<TR::ParameterSymbol> &parmList);
   };

#endif
