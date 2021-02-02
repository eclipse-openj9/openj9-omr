###############################################################################
# Copyright (c) 2021, 2021 IBM Corp. and others
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
###############################################################################

if(OMR_TEST_)
	return()
endif()
set(OMR_TEST_ 1)

if(NOT OMR_TEST_LAUNCHER)
	if(OMR_EXE_LAUNCHER)
		set(OMR_TEST_LAUNCHER "${OMR_EXE_LAUNCHER}")
	endif()
endif()

function(omr_add_test)
	cmake_parse_arguments(opt "" "NAME;COMMAND" "" ${ARGN})
	if(NOT opt_COMMAND)
		message(FATAL_ERROR "omr_add_test used without specifying COMMAND")
	endif()
	if(NOT opt_NAME)
		message(FATAL_ERROR "omr_add_test used without specifying NAME")
	endif()

	add_test(NAME "${opt_NAME}" COMMAND ${OMR_TEST_LAUNCHER} ${opt_COMMAND} ${opt_UNPARSED_ARGUMENTS})
endfunction()
