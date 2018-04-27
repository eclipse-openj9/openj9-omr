###############################################################################
# Copyright (c) 2017, 2017 IBM Corp. and others
#
# This program and the accompanying materials are made available under
# the terms of the Eclipse Public License 2.0 which accompanies this
# distribution and is available at http://eclipse.org/legal/epl-2.0
# or the Apache License, Version 2.0 which accompanies this distribution
# and is available at https://www.apache.org/licenses/LICENSE-2.0.
#
# This Source Code may also be made available under the following Secondary
# Licenses when the conditions for such availability set forth in the
# Eclipse Public License, v. 2.0 are satisfied: GNU General Public License,
# version 2 with the GNU Classpath Exception [1] and GNU General Public
# License, version 2 with the OpenJDK Assembly Exception [2].
#
# [1] https://www.gnu.org/software/classpath/license.html
# [2] http://openjdk.java.net/legal/assembly-exception.html
#
# SPDX-License-Identifier: EPL-2.0 OR Apache-2.0 OR GPL-2.0 WITH Classpath-exception-2.0 OR LicenseRef-GPL-2.0 WITH Assembly-exception
#############################################################################

set(OMR_VERSION_MAJOR 0)
set(OMR_VERSION_MINOR 0)
set(OMR_VERSION_PATCH 1)
set(OMR_VERSION ${OMR_VERSION_MAJOR}.${OMR_VERSION_MINOR}.${OMR_VERSION_PATCH})

set(VERSION_STRING "<Unknown>")

find_package(Git)
if(Git_FOUND)
	execute_process(
		COMMAND "${GIT_EXECUTABLE}" rev-parse --short HEAD
		WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
		RESULT_VARIABLE rc
		OUTPUT_VARIABLE OMR_SHA
		OUTPUT_STRIP_TRAILING_WHITESPACE
	)

	if(NOT "${rc}" STREQUAL "0")
		message(AUTHOR_WARNING "Failed to get OMR git SHA")
	else()
		set(VERSION_STRING "${OMR_SHA}")
	endif()
endif()


set(OMR_VERSION_STRING "${VERSION_STRING}" CACHE INTERNAL "")
set(OMR_JIT_VERSION_STRING "${VERSION_STRING}" CACHE INTERNAL "")
