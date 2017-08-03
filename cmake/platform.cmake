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

include(OmrAssert)
include(OmrDetectSystemInformation)

###
### Platform flags
### TODO: arch flags. Defaulting to x86-64

message(STATUS "CMAKE_SYSTEM_NAME=${CMAKE_SYSTEM}")
message(STATUS "CMAKE_SYSTEM_VERSION=${CMAKE_SYSTEM_VERSION}")
message(STATUS "CMAKE_SYSTEM_PROCESSOR=${CMAKE_SYSTEM_PROCESSOR}")
message(STATUS "CMAKE_CROSSCOMPILING=${CMAKE_CROSSCOMPILING}")

omr_detect_system_information()

# Pickup OS info 
include(cmake/platform/os/${OMR_HOST_OS}.cmake OPTIONAL)

# Pickup Arch Info
include(cmake/platform/arch/${OMR_HOST_ARCH}.cmake OPTIONAL) 

# Pickup toolconfig based on platform. 
include(cmake/platform/toolcfg/${OMR_TOOLCONFIG}.cmake OPTIONAL)

# Verify toolconfig!
include(cmake/platform/toolcfg/verify.cmake)

include(cmake/AddPrefix.cmake)

# Remove a specified option from a variable
macro(omr_remove_option var opt)
	string( REGEX REPLACE
		"(^| )${opt}($| )"
		""
		${var}
		"${${var}}"
	)
endmacro(omr_remove_option)


omr_add_prefix(OMR_OS_DEFINITIONS_PREFIXED   ${OMR_C_DEFINITION_PREFIX} ${OMR_OS_DEFINITIONS})
omr_add_prefix(OMR_ARCH_DEFINITIONS_PREFIXED ${OMR_C_DEFINITION_PREFIX} ${OMR_ARCH_DEFINITIONS})


add_definitions(
	${OMR_OS_COMPILE_OPTIONS}
	${OMR_OS_DEFINITIONS_PREFIXED}
	${OMR_ARCH_DEFINITIONS_PREFIXED}
)

if(OMR_HOST_OS STREQUAL "linux")
	if(OMR_WARNINGS_AS_ERRORS)
		set(CMAKE_C_FLAGS   "${CMAKE_C_FLAGS} ${OMR_WARNING_AS_ERROR_FLAG}")
		set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OMR_WARNING_AS_ERROR_FLAG}")
	endif()
endif()


# interface library for exporting symbols from dynamic library
# currently does nothing except on zos
add_library(omr_shared INTERFACE)

# If the OS requires global setup, do it here. 
omr_os_global_setup()

# And now the toolconfig setup
omr_toolconfig_global_setup()


###
### Flags we aren't using
###

# TODO: SPEC

# TODO: OMR_HOST_ARCH
# TODO: OMR_TARGET_DATASIZE
# TODO: OMR_TOOLCHAIN
# TODO: OMR_CROSS_CONFIGURE
# TODO: OMR_RTTI

