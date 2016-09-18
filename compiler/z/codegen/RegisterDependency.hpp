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

#ifndef TR_REGISTER_DEPENDENCY_INCL
#define TR_REGISTER_DEPENDENCY_INCL

#include "codegen/OMRRegisterDependency.hpp"

namespace TR
{


class RegisterDependencyConditions : public OMR::RegisterDependencyConditionsConnector
   {
   public:

   RegisterDependencyConditions() : OMR::RegisterDependencyConditionsConnector() {}

   RegisterDependencyConditions(TR::CodeGenerator *cg,
        TR::Node *node,
        uint32_t extranum,
        TR::Instruction **cursorPtr) :
        OMR::RegisterDependencyConditionsConnector(cg, node,extranum, cursorPtr) {}

   RegisterDependencyConditions(RegisterDependencyConditions* iConds, uint16_t numNewPreConds, uint16_t numNewPostConds, TR::CodeGenerator *cg) :
      OMR::RegisterDependencyConditionsConnector(iConds, numNewPreConds, numNewPostConds, cg) {}

   RegisterDependencyConditions(RegisterDependencyConditions * conds_1, RegisterDependencyConditions * conds_2, TR::CodeGenerator *cg) :
      OMR::RegisterDependencyConditionsConnector(conds_1, conds_2, cg) {}

   RegisterDependencyConditions(TR_S390RegisterDependencyGroup *_preConditions,
         TR_S390RegisterDependencyGroup *_postConditions,
            uint16_t numPreConds, uint16_t numPostConds,
            TR::CodeGenerator *cg) :
            OMR::RegisterDependencyConditionsConnector(_preConditions, _postConditions, numPreConds, numPostConds, cg) {}

   RegisterDependencyConditions(uint16_t numPreConds, uint16_t numPostConds, TR::CodeGenerator *cg) :
      OMR::RegisterDependencyConditionsConnector(numPreConds, numPostConds, cg) {}

   };
}

inline TR::RegisterDependencyConditions *
generateRegisterDependencyConditions(uint16_t numPreConds, uint16_t numPostConds, TR::CodeGenerator *cg)
   {
   return new (cg->trHeapMemory()) TR::RegisterDependencyConditions(numPreConds, numPostConds, cg);
   }

inline TR::RegisterDependencyConditions *
generateRegisterDependencyConditions(TR::CodeGenerator *cg, TR::Node *node, uint32_t extranum, TR::Instruction **cursorPtr=NULL)
   {
   return new (cg->trHeapMemory()) TR::RegisterDependencyConditions(cg, node, extranum, cursorPtr);
   }

#endif
