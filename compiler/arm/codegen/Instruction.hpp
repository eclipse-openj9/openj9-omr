/*******************************************************************************
 *
 * (c) Copyright IBM Corp. 2017, 2017
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

#ifndef OMR_INSTRUCTION_INCL
#define OMR_INSTRUCTION_INCL

#include "codegen/ARMConditionCode.hpp"
#include "codegen/OMRInstruction.hpp"

#include "codegen/RegisterDependency.hpp" // @@@@

namespace TR
{
class Instruction;

class OMR_EXTENSIBLE Instruction : public OMR::InstructionConnector
   {

   public:

   // TODO: need to fix the InstOpCode initialization
   Instruction(TR::Node *node, TR::CodeGenerator *cg)
      : OMR::InstructionConnector(cg, InstOpCode::BAD, node)
      {
      self()->setOpCodeValue(ARMOp_bad);
      self()->setConditionCode(ARMConditionCodeAL);
      self()->setDependencyConditions(NULL);
      }

   Instruction(TR_ARMOpCodes op, TR::Node *node, TR::CodeGenerator *cg)
      : OMR::InstructionConnector(cg, InstOpCode::BAD, node)
      {
      self()->setOpCodeValue(op);
      self()->setConditionCode(ARMConditionCodeAL);
      self()->setDependencyConditions(NULL);
      }

   Instruction(TR::Instruction   *precedingInstruction,
               TR_ARMOpCodes     op,
               TR::Node          *node,
               TR::CodeGenerator *cg)
      : OMR::InstructionConnector(cg, precedingInstruction, InstOpCode::BAD, node)
      {
      self()->setOpCodeValue(op);
      self()->setConditionCode(ARMConditionCodeAL);
      self()->setDependencyConditions(NULL);
      }

   Instruction(TR_ARMOpCodes                       op,
               TR::Node                            *node,
               TR::RegisterDependencyConditions    *cond,
               TR::CodeGenerator                   *cg)
      : OMR::InstructionConnector(cg, InstOpCode::BAD, node)
      {
      self()->setOpCodeValue(op);
      self()->setConditionCode(ARMConditionCodeAL);
      self()->setDependencyConditions(cond);
      if (cond)
         cond->incRegisterTotalUseCounts(cg);
      }

   Instruction(TR::Instruction                     *precedingInstruction,
               TR_ARMOpCodes                       op,
               TR::Node                            *node,
               TR::RegisterDependencyConditions    *cond,
               TR::CodeGenerator                   *cg)
      : OMR::InstructionConnector(cg, precedingInstruction, InstOpCode::BAD, node)
      {
      self()->setOpCodeValue(op);
      self()->setConditionCode(ARMConditionCodeAL);
      self()->setDependencyConditions(cond);
      if (cond)
         cond->incRegisterTotalUseCounts(cg);
      }

   };

}


//TODO: these downcasts everywhere need to be removed
inline uint32_t        * toARMCursor(uint8_t *i) { return (uint32_t *)i; }

#endif
