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


compiler_library(ras 
	${CMAKE_CURRENT_SOURCE_DIR}/CallStack.cpp 
	${CMAKE_CURRENT_SOURCE_DIR}/CFGChecker.cpp 
	${CMAKE_CURRENT_SOURCE_DIR}/Debug.cpp 
	${CMAKE_CURRENT_SOURCE_DIR}/DebugCounter.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/ILValidator.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/IgnoreLocale.cpp 
	${CMAKE_CURRENT_SOURCE_DIR}/LimitFile.cpp 
	${CMAKE_CURRENT_SOURCE_DIR}/LogTracer.cpp 
	${CMAKE_CURRENT_SOURCE_DIR}/OptionsDebug.cpp 
	${CMAKE_CURRENT_SOURCE_DIR}/PPCOpNames.cpp 
	${CMAKE_CURRENT_SOURCE_DIR}/Tree.cpp 
)
