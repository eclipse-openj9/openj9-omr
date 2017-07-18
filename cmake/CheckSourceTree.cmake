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

# Ensure the source tree hasn't been used for building before.
message(STATUS "SRC = ${CMAKE_SOURCE_DIR}")
if(EXISTS "${omr_SOURCE_DIR}/omrcfg.h")
	message(FATAL_ERROR "An existing omrcfg.h has been detected in the source tree. This causes unexpected errors when compiling. Please build from a clean source tree.")
endif()


