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


compiler_library(codegen
	${CMAKE_CURRENT_SOURCE_DIR}/OMRAheadOfTimeCompile.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/Analyser.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/CodeGenPrep.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/CodeGenGC.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/CodeGenRA.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/FrontEnd.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/OMRGCRegisterMap.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/OMRGCStackAtlas.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/OMRLinkage.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/LiveRegister.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/OutOfLineCodeSection.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/OMRRegisterDependency.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/Relocation.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/ScratchRegisterManager.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/StorageInfo.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/OMRTreeEvaluator.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/PreInstructionSelection.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/NodeEvaluation.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/OMRSnippet.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/OMRUnresolvedDataSnippet.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/OMRSnippetGCMap.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/OMRCodeGenerator.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/OMRCodeGenPhase.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/OMRMemoryReference.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/OMRMachine.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/OMRRegister.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/OMRRealRegister.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/OMRRegisterPair.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/OMRInstruction.cpp
)
