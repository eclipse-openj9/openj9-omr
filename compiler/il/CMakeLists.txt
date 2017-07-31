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


compiler_library(il 
	${CMAKE_CURRENT_SOURCE_DIR}/symbol/OMRSymbol.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/symbol/OMRAutomaticSymbol.cpp 
	${CMAKE_CURRENT_SOURCE_DIR}/symbol/OMRLabelSymbol.cpp 
	${CMAKE_CURRENT_SOURCE_DIR}/symbol/OMRMethodSymbol.cpp 
	${CMAKE_CURRENT_SOURCE_DIR}/symbol/OMRParameterSymbol.cpp 
	${CMAKE_CURRENT_SOURCE_DIR}/symbol/OMRRegisterMappedSymbol.cpp 
	${CMAKE_CURRENT_SOURCE_DIR}/symbol/OMRResolvedMethodSymbol.cpp 
	${CMAKE_CURRENT_SOURCE_DIR}/symbol/OMRStaticSymbol.cpp 

	${CMAKE_CURRENT_SOURCE_DIR}/Aliases.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/OMRBlock.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/OMRDataTypes.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/OMRIL.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/OMRILOps.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/OMRNode.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/OMRSymbolReference.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/OMRTreeTop.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/NodePool.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/NodeUtils.cpp
)
