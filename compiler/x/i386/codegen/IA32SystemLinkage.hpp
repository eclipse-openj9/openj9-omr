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

#ifndef IA32_SYSTEMLINKAGE_INCL
#define IA32_SYSTEMLINKAGE_INCL

#include "x/codegen/X86SystemLinkage.hpp"

#include <stdint.h>                        // for int32_t, uint16_t, etc
#include "codegen/Register.hpp"            // for Register
#include "il/DataTypes.hpp"                // for DataTypes

namespace TR { class CodeGenerator; }
namespace TR { class Node; }
namespace TR { class ParameterSymbol; }
namespace TR { class RegisterDependencyConditions; }

class TR_IA32SystemLinkage : public TR_X86SystemLinkage
   {
   public:

   TR_IA32SystemLinkage(TR::CodeGenerator *cg);
   void setUpStackSizeForCallNode(TR::Node*);
   protected:
   virtual int32_t buildArgs(TR::Node *callNode, TR::RegisterDependencyConditions *deps);
   virtual TR::Register *buildIndirectDispatch(TR::Node *callNode);
   virtual TR::Register *buildDirectDispatch(TR::Node *callNode, bool spillFPRegs);
   int32_t layoutParm(TR::Node*, int32_t&, uint16_t&, uint16_t&, parmLayoutResult&);
   int32_t layoutParm(TR::ParameterSymbol*, int32_t&, uint16_t&, uint16_t&, parmLayoutResult&);
   virtual TR::Register *buildVolatileAndReturnDependencies(TR::Node *callNode, TR::RegisterDependencyConditions *deps);
   private:
   virtual uint32_t getAlignment(TR::DataTypes);
   };

#if 0
class TR_IA32SystemLinkage : public TR_IA32PrivateLinkage
   {
   public:

   TR_IA32SystemLinkage(TR::CodeGenerator *cg);

   virtual TR::Register *buildDirectDispatch(TR::Node *callNode, bool spillFPRegs);
   virtual TR::Register *buildIndirectDispatch(TR::Node *callNode);
   virtual TR::Register *buildAlloca(TR::Node *callNode);
   virtual TR::Register *pushStructArg(TR::Node* child);
   virtual void notifyHasalloca(){}

   protected:

   TR::Register *buildDispatch(TR::Node *callNode);

   };
#endif

#endif
