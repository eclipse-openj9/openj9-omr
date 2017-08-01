##############################################################################
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

if(OMR_PLATFORM_)
	return()
endif()
set(OMR_PLATFORM_ 1)

include(OmrAssert)
include(OmrDetectSystemInformation)
include(OmrUtility)

###
### Platform flags
### TODO: arch flags. Defaulting to x86-64

omr_detect_system_information()

if(NOT OMR_HOST_OS STREQUAL "zos")
	enable_language(ASM)
endif()

# Pickup OS info
include(platform/os/${OMR_HOST_OS})

# Pickup Arch Info
include(platform/arch/${OMR_HOST_ARCH})

# Pickup toolconfig based on platform
include(platform/toolcfg/${OMR_TOOLCONFIG})

# Verify toolconfig!
include(platform/toolcfg/verify)


# interface library for exporting symbols from dynamic library
# currently does nothing except on zos
add_library(omr_shared INTERFACE)

macro(omr_platform_global_setup)

	omr_assert(WARNING TEST NOT OMR_PLATFORM_GLOBALLY_INITIALIZED
		MESSAGE "omr_platform_global_setup called twice."
	)

	set(OMR_PLATFORM_GLOBALLY_INITIALIZED 1)

	omr_append_flags(CMAKE_C_FLAGS
		${OMR_PLATFORM_COMPILE_OPTIONS}
		${OMR_PLATFORM_C_COMPILE_OPTIONS}
	)

	omr_append_flags(CMAKE_CXX_FLAGS
		${OMR_PLATFORM_COMPILE_OPTIONS}
		${OMR_PLATFORM_CXX_COMPILE_OPTIONS}
	)

	omr_append_flags(CMAKE_EXE_LINKER_FLAGS
		${OMR_PLATFORM_LINKER_OPTIONS}
		${OMR_PLATFORM_EXE_LINKER_OPTIONS}
	)

	omr_append_flags(CMAKE_SHARED_LINKER_FLAGS
		${OMR_PLATFORM_LINKER_OPTIONS}
		${OMR_PLATFORM_SHARED_LINKER_OPTIONS}
	)

	omr_append_flags(CMAKE_STATIC_LINKER_FLAGS
		${OMR_PLATFORM_LINKER_OPTIONS}
		${OMR_PLATFORM_STATIC_LINKER_OPTIONS}
	)

	include_directories(
		${OMR_PLATFORM_INCLUDE_DIRECTORIES}
	)

	add_definitions(
		${OMR_PLATFORM_DEFINITIONS}
	)

	link_libraries(
		${OMR_PLATFORM_LIBRARIES}
	)

	# TODO: Move this somewhere else, or just find a better home.
	if(OMR_WARNINGS_AS_ERRORS)
		omr_append_flags(CMAKE_C_FLAGS   ${OMR_WARNING_AS_ERROR_FLAG})
		omr_append_flags(CMAKE_CXX_FLAGS ${OMR_WARNING_AS_ERROR_FLAG})
	endif()

	# If the OS requires global setup, do it here.
	if(COMMAND omr_arch_global_setup)
		omr_arch_global_setup()
	endif()

	# If the OS requires global setup, do it here.
	if(COMMAND omr_os_global_setup)
		omr_os_global_setup()
	endif()

	# And now the toolconfig setup
	if(COMMAND omr_toolconfig_global_setup)
		omr_toolconfig_global_setup()
	endif()
endmacro()

###
### Flags we aren't using
###

# TODO: SPEC

# TODO: OMR_HOST_ARCH
# TODO: OMR_TARGET_DATASIZE
# TODO: OMR_TOOLCHAIN
# TODO: OMR_CROSS_CONFIGURE
# TODO: OMR_RTTI

