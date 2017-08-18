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


compiler_library(p
# Host files
	${CMAKE_CURRENT_SOURCE_DIR}/runtime/OMRCodeCacheConfig.cpp
# Target files
	${CMAKE_CURRENT_SOURCE_DIR}/codegen/BinaryEvaluator.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/codegen/ControlFlowEvaluator.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/codegen/FPTreeEvaluator.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/codegen/GenerateInstructions.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/codegen/OMRMemoryReference.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/codegen/OpBinary.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/codegen/OpProperties.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/codegen/PPCAOTRelocation.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/codegen/PPCBinaryEncoding.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/codegen/OMRCodeGenerator.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/codegen/OMRInstruction.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/codegen/PPCDebug.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/codegen/PPCHelperCallSnippet.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/codegen/PPCInstruction.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/codegen/OMRLinkage.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/codegen/PPCSystemLinkage.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/codegen/OMRMachine.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/codegen/PPCOutOfLineCodeSection.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/codegen/OMRRealRegister.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/codegen/OMRRegisterDependency.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/codegen/OMRSnippet.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/codegen/PPCTableOfConstants.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/codegen/OMRTreeEvaluator.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/codegen/TreeEvaluatorVMX.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/codegen/UnaryEvaluator.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/codegen/OMRConstantDataSnippet.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/codegen/OMRRegisterIterator.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/env/OMRCPU.cpp
)
