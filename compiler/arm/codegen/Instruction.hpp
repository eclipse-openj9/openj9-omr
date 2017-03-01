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

#ifndef TR_INSTRUCTION_INCL
#define TR_INSTRUCTION_INCL

#include "codegen/ARMConditionCode.hpp"
#include "codegen/OMRInstruction.hpp"

#include "codegen/RegisterDependency.hpp" // @@@@

namespace TR
{
class Instruction;

class OMR_EXTENSIBLE Instruction : public OMR::InstructionConnector
   {

   public:

   Instruction(TR::CodeGenerator *cg, TR::InstOpCode::Mnemonic op, TR::Node *node = 0)
      : OMR::InstructionConnector(cg, op, node)
      {}
   Instruction(TR::CodeGenerator *cg, TR::Instruction *precedingInstruction, TR::InstOpCode::Mnemonic op, TR::Node *node = 0)
      : OMR::InstructionConnector(cg, precedingInstruction, op, node)
      {}

   Instruction(TR::Node *node, TR::CodeGenerator *cg)
      : OMR::InstructionConnector(node, cg)
      {}

   Instruction(TR_ARMOpCodes op, TR::Node *node, TR::CodeGenerator *cg)
      : OMR::InstructionConnector(op, node, cg)
      {}

   Instruction(TR::Instruction   *precedingInstruction,
               TR_ARMOpCodes     op,
               TR::Node          *node,
               TR::CodeGenerator *cg)
      : OMR::InstructionConnector(precedingInstruction, op, node, cg)
      {}

   Instruction(TR_ARMOpCodes                       op,
               TR::Node                            *node,
               TR::RegisterDependencyConditions    *cond,
               TR::CodeGenerator                   *cg)
      : OMR::InstructionConnector(op, node, cond, cg)
      {}

   Instruction(TR::Instruction                     *precedingInstruction,
               TR_ARMOpCodes                       op,
               TR::Node                            *node,
               TR::RegisterDependencyConditions    *cond,
               TR::CodeGenerator                   *cg)
      : OMR::InstructionConnector(precedingInstruction, op, node, cond, cg)
      {}

   };

}


//TODO: these downcasts everywhere need to be removed
inline uint32_t        * toARMCursor(uint8_t *i) { return (uint32_t *)i; }

#endif
