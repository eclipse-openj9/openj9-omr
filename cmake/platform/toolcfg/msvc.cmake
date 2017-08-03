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

set(OMR_C_DEFINITION_PREFIX /D)

set(OMR_WARNING_AS_ERROR_FLAG /WX)

macro(omr_toolconfig_global_setup)
	set(opt_flags "/GS-")
	# we want to disable C4091, and C4577
	# C4577 is a bogus warning about specifying noexcept when exceptions are disabled
	# C4091 is caused by broken windows sdk (https://connect.microsoft.com/VisualStudio/feedback/details/1302025/warning-c4091-in-sdk-7-1a-shlobj-h-1051-dbghelp-h-1054-3056)
	set(common_flags "-MD -Zm400 /wd4577 /wd4091")

	if(OMR_WARNINGS_AS_ERRORS)
           set(common_flags "${common_flags} ${OMR_WARNING_AS_ERROR_FLAG}")
		# TODO we also want to be setting warning as error on linker flags
	endif()

	set(linker_common "-subsystem:console -machine:${TARGET_MACHINE}")
	if(NOT OMR_ENV_DATA64)
		set(linker_common "${linker_common} /SAFESEH")
	endif()

	set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${linker_common} /LARGEADDRESSAWARE wsetargv.obj")
	if(OMR_ENV_DATA64)
		#TODO: makefile has this but it seems to break linker
		#set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /NODEFAULTLIB:MSVCRTD")
	endif()


	# Make sure we are building without incremental linking
	omr_remove_option(CMAKE_EXE_LINKER_FLAGS "/INCREMENTAL")
	omr_remove_option(CMAKE_SHARED_LINKER_FLAGS "/INCREMENTAL")
	set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /INCREMENTAL:NO /NOLOGO")
	set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} /INCREMENTAL:NO /NOLOGO")
	foreach(build_type IN LISTS CMAKE_CONFIGURATION_TYPES)
		string(TOUPPER "${build_type}" build_type)
		omr_remove_option("CMAKE_EXE_LINKER_FLAGS_${build_type}" "/INCREMENTAL")
		omr_remove_option("CMAKE_SHARED_LINKER_FLAGS_${build_type}" "/INCREMENTAL")
	endforeach()

	if(OMR_ENV_DATA64)
		set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -entry:_DllMainCRTStartup")
	else()
		set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -entry:_DllMainCRTStartup@12")
	endif()

	#strip out exception handling flags (added by default by cmake)
	omr_remove_option(CMAKE_CXX_FLAGS "/EHsc")
	omr_remove_option(CMAKE_CXX_FLAGS "/GR")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${common_flags}")
	set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${common_flags}")

	message(STATUS "CFLAGS = ${CMAKE_C_FLAGS}")
	message(STATUS "CXXFLAGS = ${CMAKE_CXX_FLAGS}")

	#Hack up output dir to fix dll dependency issues on windows
	set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}")
endmacro(omr_toolchain_global_setup)
