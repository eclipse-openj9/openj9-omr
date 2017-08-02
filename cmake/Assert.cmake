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
