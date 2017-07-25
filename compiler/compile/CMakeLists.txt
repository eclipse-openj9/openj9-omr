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


compiler_library(compile 

	${CMAKE_CURRENT_SOURCE_DIR}/OSRData.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/Method.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/VirtualGuard.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/OMRSymbolReferenceTable.cpp 
	${CMAKE_CURRENT_SOURCE_DIR}/OMRAliasBuilder.cpp 
	${CMAKE_CURRENT_SOURCE_DIR}/OMRCompilation.cpp 
	${CMAKE_CURRENT_SOURCE_DIR}/TLSCompilationManager.cpp 

)
