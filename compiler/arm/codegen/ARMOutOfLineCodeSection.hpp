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

#ifndef ARMOUTOFLINECODESECTION_INCL
#define ARMOUTOFLINECODESECTION_INCL

#include "codegen/ARMOps.hpp"
#include "codegen/OutOfLineCodeSection.hpp"
#include "env/TRMemory.hpp"

namespace TR { class CodeGenerator; }
namespace TR { class LabelSymbol; }
class TR_ARMRegisterDependencyConditions;

class TR_ARMOutOfLineCodeSection : public TR_OutOfLineCodeSection
   {

public:
   TR_ARMOutOfLineCodeSection(TR::LabelSymbol * entryLabel, TR::LabelSymbol * restartLabel,
                               TR::CodeGenerator *cg) : TR_OutOfLineCodeSection(entryLabel, restartLabel, cg)
                              {}

   TR_ARMOutOfLineCodeSection(TR::LabelSymbol * entryLabel,
                               TR::CodeGenerator *cg) : TR_OutOfLineCodeSection(entryLabel, cg)
                              {}
   // For calls
   //
   TR_ARMOutOfLineCodeSection(TR::Node *callNode, TR::ILOpCodes callOp, TR::Register *targetReg, TR::LabelSymbol *entryLabel, TR::LabelSymbol *restartLabel, TR::CodeGenerator *cg);

   TR_ARMOutOfLineCodeSection(TR::Node *callNode, TR::ILOpCodes callOp, TR::Register *targetReg, TR::LabelSymbol *entryLabel, TR::LabelSymbol *restartLabel, TR_ARMOpCodes targetRegMovOpcode, TR::CodeGenerator *cg);

public:
   void assignRegisters(TR_RegisterKinds kindsToBeAssigned);
   void generateARMOutOfLineCodeSectionDispatch();
   };
#endif
