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

include(OmrAssert)

omr_assert(TEST OMR_TOOLS)
omr_assert(TEST NOT OMR_TOOLS_IMPORTFILE)

if(CMAKE_CROSSCOMPILING)
	message(STATUS "OMR: Using cross compiled tools.")
	message(STATUS "OMR: You may need to import tools from a native project.")
	message(STATUS "OMR: See OMR_TOOLS_IMPORTFILE.")
endif(CMAKE_CROSSCOMPILING)

add_subdirectory(hookgen)
add_subdirectory(tracemerge)
add_subdirectory(tracegen)

export(TARGETS hookgen tracemerge tracegen FILE "ImportTools.cmake")
