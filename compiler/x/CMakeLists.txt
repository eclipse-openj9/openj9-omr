###############################################################################
#
# (c) Copyright IBM Corp. 2017, 2017
#
#  This program and the accompanying materials are made available
#  under the terms of the Eclipse Public License v1.0 and
#  Apache License v2.0 which accompanies this distribution.
#
#      The Eclipse Public License is available at
#      http://www.eclipse.org/legal/epl-v10.html
#
#      The Apache License v2.0 is available at
#      http://www.opensource.org/licenses/apache2.0.php
#
# Contributors:
#    Multiple authors (IBM Corp.) - initial implementation and documentation
###############################################################################


compiler_library(x 
	${CMAKE_CURRENT_SOURCE_DIR}/codegen/BinaryCommutativeAnalyser.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/codegen/BinaryEvaluator.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/codegen/CompareAnalyser.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/codegen/ConstantDataSnippet.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/codegen/ControlFlowEvaluator.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/codegen/DataSnippet.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/codegen/DivideCheckSnippet.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/codegen/FPBinaryArithmeticAnalyser.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/codegen/FPCompareAnalyser.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/codegen/FPTreeEvaluator.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/codegen/SIMDTreeEvaluator.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/codegen/HelperCallSnippet.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/codegen/IA32LinkageUtils.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/codegen/IntegerMultiplyDecomposer.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/codegen/OMRMemoryReference.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/codegen/OpBinary.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/codegen/OpNames.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/codegen/OutlinedInstructions.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/codegen/RegisterRematerialization.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/codegen/SubtractAnalyser.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/codegen/Trampoline.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/codegen/OMRTreeEvaluator.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/codegen/UnaryEvaluator.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/codegen/X86BinaryEncoding.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/codegen/X86Debug.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/codegen/X86FPConversionSnippet.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/codegen/OMRInstruction.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/codegen/OMRX86Instruction.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/codegen/OMRMachine.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/codegen/OMRLinkage.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/codegen/OMRRegister.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/codegen/OMRRealRegister.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/codegen/OMRRegisterDependency.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/codegen/OMRSnippet.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/codegen/X86SystemLinkage.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/codegen/XMMBinaryArithmeticAnalyser.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/codegen/OMRCodeGenerator.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/codegen/OMRRegisterIterator.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/env/OMRCPU.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/env/OMRDebugEnv.cpp
	$<$<EQUAL:${OMR_TARGET_DATASIZE},64>: ${CMAKE_CURRENT_SOURCE_DIR}/amd64/codegen/OMRCodeGenerator.cpp> 
	$<$<EQUAL:${OMR_TARGET_DATASIZE},64>: ${CMAKE_CURRENT_SOURCE_DIR}/amd64/codegen/OMRMachine.cpp>
	$<$<EQUAL:${OMR_TARGET_DATASIZE},64>: ${CMAKE_CURRENT_SOURCE_DIR}/amd64/codegen/OMRTreeEvaluator.cpp> 
	$<$<EQUAL:${OMR_TARGET_DATASIZE},64>: ${CMAKE_CURRENT_SOURCE_DIR}/amd64/codegen/OMRMemoryReference.cpp>
	$<$<EQUAL:${OMR_TARGET_DATASIZE},64>: ${CMAKE_CURRENT_SOURCE_DIR}/amd64/codegen/OMRRealRegister.cpp>
	$<$<EQUAL:${OMR_TARGET_DATASIZE},64>: ${CMAKE_CURRENT_SOURCE_DIR}/amd64/codegen/AMD64SystemLinkage.cpp>
)

