################################################################################
##
## (c) Copyright IBM Corp. 2017, 2017
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
    $(JIT_OMR_DIRTY_DIR)/arm/codegen/ARMBinaryEncoding.cpp \
    $(JIT_OMR_DIRTY_DIR)/arm/codegen/OMRCodeGenerator.cpp \
    $(JIT_OMR_DIRTY_DIR)/arm/codegen/ARMDebug.cpp \
    $(JIT_OMR_DIRTY_DIR)/arm/codegen/ARMDisassem.cpp \
    $(JIT_OMR_DIRTY_DIR)/arm/codegen/ARMGenerateInstructions.cpp \
    $(JIT_OMR_DIRTY_DIR)/arm/codegen/OMRInstruction.cpp \
    $(JIT_OMR_DIRTY_DIR)/arm/codegen/OMRMachine.cpp \
    $(JIT_OMR_DIRTY_DIR)/arm/codegen/OMRLinkage.cpp \
    $(JIT_OMR_DIRTY_DIR)/arm/codegen/ARMOperand2.cpp \
    $(JIT_OMR_DIRTY_DIR)/arm/codegen/ARMOutOfLineCodeSection.cpp \
    $(JIT_OMR_DIRTY_DIR)/arm/codegen/OMRRealRegister.cpp \
    $(JIT_OMR_DIRTY_DIR)/arm/codegen/OMRSnippet.cpp \
    $(JIT_OMR_DIRTY_DIR)/arm/codegen/ARMSystemLinkage.cpp \
    $(JIT_OMR_DIRTY_DIR)/arm/codegen/BinaryCommutativeAnalyser.cpp \
    $(JIT_OMR_DIRTY_DIR)/arm/codegen/BinaryEvaluator.cpp \
    $(JIT_OMR_DIRTY_DIR)/arm/codegen/ConstantDataSnippet.cpp \
    $(JIT_OMR_DIRTY_DIR)/arm/codegen/ControlFlowEvaluator.cpp \
    $(JIT_OMR_DIRTY_DIR)/arm/codegen/FPTreeEvaluator.cpp \
    $(JIT_OMR_DIRTY_DIR)/arm/codegen/OMRMemoryReference.cpp \
    $(JIT_OMR_DIRTY_DIR)/arm/codegen/OpBinary.cpp \
    $(JIT_OMR_DIRTY_DIR)/arm/codegen/OpProperties.cpp \
    $(JIT_OMR_DIRTY_DIR)/arm/codegen/OMRRegisterDependency.cpp \
    $(JIT_OMR_DIRTY_DIR)/arm/codegen/StackCheckFailureSnippet.cpp \
    $(JIT_OMR_DIRTY_DIR)/arm/codegen/SubtractAnalyser.cpp \
    $(JIT_OMR_DIRTY_DIR)/arm/codegen/OMRTreeEvaluator.cpp \
    $(JIT_OMR_DIRTY_DIR)/arm/codegen/UnaryEvaluator.cpp \
    $(JIT_OMR_DIRTY_DIR)/arm/codegen/OMRRegisterIterator.cpp \
    $(JIT_OMR_DIRTY_DIR)/arm/env/OMRCompilerEnv.cpp

