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


compiler_library(runtime 
	${CMAKE_CURRENT_SOURCE_DIR}/Runtime.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/Trampoline.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/Alignment.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/CodeCacheTypes.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/OMRCodeCache.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/OMRCodeCacheManager.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/OMRCodeCacheMemorySegment.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/OMRCodeCacheConfig.cpp
)
