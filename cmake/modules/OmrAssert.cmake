###############################################################################
#
# (c) Copyright IBM Corp. 2017
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

# Include once
if(OMR_ASSERT_)
	return()
endif()
set(OMR_ASSERT_ 1)

# omr_assert([<mode>] TEST <condition>... [MESSAGE <message>])
# <mode> is one of FATAL_ERROR (default), WARNING, STATUS
function(omr_assert)
	cmake_parse_arguments(omr_assert "STATUS;WARNING;FATAL_ERROR" "MESSAGE" "TEST" ${ARGN})

	if(omr_assert_FATAL_ERROR)
		set(mode FATAL_ERROR)
	elseif(omr_assert_WARNING)
		set(mode WARNING)
	elseif(omr_assert_STATUS)
		set(mode STATUS)
	else()
		set(mode FATAL_ERROR)
	endif()

	if(DEFINED omr_assert_UNPARSED_ARGUMENTS)
		message(FATAL_ERROR "omr_assert called with extra arguments")
	endif()

	if(NOT DEFINED omr_assert_TEST)
		message(FATAL_ERROR "omr_assert called without test condition")
	endif()

	if(NOT DEFINED omr_assert_MESSAGE)
		string(REPLACE ";" " " omr_assert_MESSAGE "${omr_assert_TEST}")
	endif()

	if(NOT(${omr_assert_TEST}))
		message(${mode} "Assertion failed: ${omr_assert_MESSAGE}")
	endif()
endfunction()
