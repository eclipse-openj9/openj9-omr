################################################################################
##
## (c) Copyright IBM Corp. 2016, 2016
##
##  This program and the accompanying materials are made available
##  under the terms of the Eclipse Public License v1.0 and
##  Apache License v2.0 which accompanies this distribution.
##
##      The Eclipse Public License is available at
##      http://www.eclipse.org/legal/epl-v10.html
##
##      The Apache License v2.0 is available at
##      http://www.opensource.org/licenses/apache2.0.php
##
## Contributors:
##    Multiple authors (IBM Corp.) - initial implementation and documentation
################################################################################

JIT_PRODUCT_BACKEND_SOURCES+=\
    $(JIT_OMR_DIRTY_DIR)/z/codegen/BinaryAnalyser.cpp \
    $(JIT_OMR_DIRTY_DIR)/z/codegen/BinaryCommutativeAnalyser.cpp \
    $(JIT_OMR_DIRTY_DIR)/z/codegen/BinaryEvaluator.cpp \
    $(JIT_OMR_DIRTY_DIR)/z/codegen/CallSnippet.cpp \
    $(JIT_OMR_DIRTY_DIR)/z/codegen/CompareAnalyser.cpp \
    $(JIT_OMR_DIRTY_DIR)/z/codegen/ConstantDataSnippet.cpp \
    $(JIT_OMR_DIRTY_DIR)/z/codegen/ControlFlowEvaluator.cpp \
    $(JIT_OMR_DIRTY_DIR)/z/codegen/FPTreeEvaluator.cpp \
    $(JIT_OMR_DIRTY_DIR)/z/codegen/OMRMemoryReference.cpp \
    $(JIT_OMR_DIRTY_DIR)/z/codegen/OpMemToMem.cpp \
    $(JIT_OMR_DIRTY_DIR)/z/codegen/OMRMachine.cpp \
    $(JIT_OMR_DIRTY_DIR)/z/codegen/S390BranchCondNames.cpp \
    $(JIT_OMR_DIRTY_DIR)/z/codegen/S390Debug.cpp \
    $(JIT_OMR_DIRTY_DIR)/z/codegen/S390GenerateInstructions.cpp \
    $(JIT_OMR_DIRTY_DIR)/z/codegen/S390HelperCallSnippet.cpp \
    $(JIT_OMR_DIRTY_DIR)/z/codegen/S390Instruction.cpp \
    $(JIT_OMR_DIRTY_DIR)/z/codegen/OMRLinkage.cpp \
    $(JIT_OMR_DIRTY_DIR)/z/codegen/SystemLinkage.cpp \
    $(JIT_OMR_DIRTY_DIR)/z/codegen/S390OutOfLineCodeSection.cpp \
    $(JIT_OMR_DIRTY_DIR)/z/codegen/OMRRegisterDependency.cpp \
    $(JIT_OMR_DIRTY_DIR)/z/codegen/OMRSnippet.cpp \
    $(JIT_OMR_DIRTY_DIR)/z/codegen/S390Snippets.cpp \
    $(JIT_OMR_DIRTY_DIR)/z/codegen/TranslateEvaluator.cpp \
    $(JIT_OMR_DIRTY_DIR)/z/codegen/OMRTreeEvaluator.cpp \
    $(JIT_OMR_DIRTY_DIR)/z/codegen/UnaryEvaluator.cpp \
    $(JIT_OMR_DIRTY_DIR)/z/codegen/OMRInstruction.cpp \
    $(JIT_OMR_DIRTY_DIR)/z/codegen/InstOpCode.cpp \
    $(JIT_OMR_DIRTY_DIR)/z/codegen/InstOpCodeTables.cpp \
    $(JIT_OMR_DIRTY_DIR)/z/codegen/OMRRegister.cpp \
    $(JIT_OMR_DIRTY_DIR)/z/codegen/OMRRealRegister.cpp \
    $(JIT_OMR_DIRTY_DIR)/z/codegen/OMRRegisterIterator.cpp \
    $(JIT_OMR_DIRTY_DIR)/z/codegen/OMRRegisterPair.cpp \
    $(JIT_OMR_DIRTY_DIR)/z/codegen/OMRCodeGenerator.cpp  \
    $(JIT_OMR_DIRTY_DIR)/z/codegen/OMRCodeGenPhase.cpp \
    $(JIT_OMR_DIRTY_DIR)/z/env/OMRDebugEnv.cpp \
    $(JIT_PRODUCT_DIR)/z/codegen/JBCodeGenerator.cpp \
    $(JIT_PRODUCT_DIR)/z/codegen/Evaluator.cpp
