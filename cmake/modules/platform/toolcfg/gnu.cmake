###############################################################################
# Copyright IBM Corp. and others 2017
#
# This program and the accompanying materials are made available under
# the terms of the Eclipse Public License 2.0 which accompanies this
# distribution and is available at https://www.eclipse.org/legal/epl-2.0/
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
# [2] https://openjdk.org/legal/assembly-exception.html
#
# SPDX-License-Identifier: EPL-2.0 OR Apache-2.0 OR GPL-2.0-only WITH Classpath-exception-2.0 OR GPL-2.0-only WITH OpenJDK-assembly-exception-1.0
#############################################################################

# disable warnings as errors for OpenXL
# (see https://github.com/eclipse-omr/omr/issues/7583)
if(NOT CMAKE_C_COMPILER_IS_OPENXL)
	set(OMR_C_WARNINGS_AS_ERROR_FLAG -Werror)
	set(OMR_CXX_WARNINGS_AS_ERROR_FLAG -Werror)
	set(OMR_NASM_WARNINGS_AS_ERROR_FLAG -Werror)
endif()

set(OMR_C_ENHANCED_WARNINGS_FLAG -Wall)
set(OMR_CXX_ENHANCED_WARNINGS_FLAG -Wall)
set(OMR_NASM_ENHANCED_WARNINGS_FLAG -Wall)

# disable builtin strncpy buffer length check for components that use variable length
# array fields at the end of structs
set(OMR_STRNCPY_FORTIFY_OPTIONS -U_FORTIFY_SOURCE -D_FORTIFY_SOURCE=1)

list(APPEND OMR_PLATFORM_COMPILE_OPTIONS -pthread -fno-strict-aliasing)

list(APPEND OMR_PLATFORM_CXX_COMPILE_OPTIONS -std=c++0x)

if(CODE_COVERAGE)
	list(APPEND OMR_PLATFORM_COMPILE_OPTIONS -fprofile-arcs -ftest-coverage)
endif()

if(OMR_ARCH_X86)
	if(OMR_ENV_DATA64)
		list(APPEND OMR_PLATFORM_COMPILE_OPTIONS
			-m64
		)
		list(APPEND OMR_PLATFORM_EXE_LINKER_OPTIONS
			-m64
		)
		list(APPEND OMR_PLATFORM_SHARED_LINKER_OPTIONS
			-m64
		)
	else()
		set(CMAKE_ASM_NASM_OBJECT_FORMAT elf32)
		list(APPEND OMR_PLATFORM_COMPILE_OPTIONS
			-m32
			-msse2
		)
		list(APPEND OMR_PLATFORM_EXE_LINKER_OPTIONS
			-m32
		)
		list(APPEND OMR_PLATFORM_SHARED_LINKER_OPTIONS
			-m32
		)
	endif()
endif()

if(OMR_HOST_ARCH STREQUAL "s390")
	list(APPEND OMR_PLATFORM_COMPILE_OPTIONS -march=z9-109)
	if(OMR_ENV_DATA32)
		list(APPEND OMR_PLATFORM_COMPILE_OPTIONS
			-m31
			-mzarch
		)
		list(APPEND OMR_PLATFORM_EXE_LINKER_OPTIONS
			-m31
			-mzarch
		)
		list(APPEND OMR_PLATFORM_SHARED_LINKER_OPTIONS
			-m31
			-mzarch
		)
	endif()
endif()

if(OMR_OS_AIX AND CMAKE_C_COMPILER_IS_OPENXL)
	omr_append_flags(CMAKE_C_FLAGS "-m64")
	omr_append_flags(CMAKE_CXX_FLAGS "-m64")
	omr_append_flags(CMAKE_ASM_FLAGS "-m64")
	omr_append_flags(CMAKE_SHARED_LINKER_FLAGS "-m64")

	if(OMR_ENV_DATA64)
		set(CMAKE_CXX_ARCHIVE_CREATE "<CMAKE_AR> -X64 cr <TARGET> <LINK_FLAGS> <OBJECTS>")
		set(CMAKE_C_ARCHIVE_CREATE "<CMAKE_AR> -X64 cr <TARGET> <LINK_FLAGS> <OBJECTS>")
		set(CMAKE_C_ARCHIVE_FINISH "<CMAKE_RANLIB> -X64 <TARGET>")
	endif()
endif()

# Testarossa build variables. Longer term the distinction between TR and the rest
# of the OMR code should be heavily reduced. In the meantime, we keep the distinction.

# TR_COMPILE_OPTIONS are variables appended to CMAKE_{C,CXX}_FLAGS, and so
# apply to both C and C++ compilations.
list(APPEND TR_COMPILE_OPTIONS
	-Wno-write-strings #This warning swamps almost all other output
)

set(PASM_CMD ${CMAKE_C_COMPILER})
set(PASM_FLAGS -x assembler-with-cpp -E -P)

set(SPP_CMD ${CMAKE_C_COMPILER})
set(SPP_FLAGS -x assembler-with-cpp -E -P)

# Configure the platform dependent library for multithreading.
# We don't actually have a clang config and use gnu on non-Windows,
# so we have to detect Apple clang here.
# see ../../OmrDetectSystemInformation.cmake
if(CMAKE_C_COMPILER_ID MATCHES "^(Apple)?Clang$")
	set(OMR_PLATFORM_THREAD_LIBRARY pthread)
else()
	set(OMR_PLATFORM_THREAD_LIBRARY -pthread)
endif()

function(_omr_toolchain_separate_debug_symbols tgt)
	set(exe_file "$<TARGET_FILE:${tgt}>")
	if(OMR_OS_OSX)
		set(dbg_file "${exe_file}${OMR_DEBUG_INFO_OUTPUT_EXTENSION}")
		if(OMR_FLATTEN_DEBUG_INFO)
			set(flatten_arg "-f")
		else()
			set(flatten_arg)
		endif()
		add_custom_command(
			TARGET "${tgt}"
			POST_BUILD
			COMMAND dsymutil ${flatten_arg} -o "${dbg_file}" "${exe_file}"
		)
	else()
		omr_get_target_output_genex(${tgt} output_name)
		set(dbg_file "${output_name}${OMR_DEBUG_INFO_OUTPUT_EXTENSION}")
		if(OMR_OS_AIX AND CMAKE_C_COMPILER_IS_OPENXL)
			add_custom_command(
				TARGET "${tgt}"
				POST_BUILD
				COMMAND "${CMAKE_COMMAND}" -E copy ${exe_file} ${dbg_file}
				COMMAND "${CMAKE_STRIP}" -X32_64 ${exe_file}
			)
		else()
			add_custom_command(
				TARGET "${tgt}"
				POST_BUILD
				COMMAND "${CMAKE_OBJCOPY}" --only-keep-debug "${exe_file}" "${dbg_file}"
				COMMAND "${CMAKE_OBJCOPY}" --strip-debug "${exe_file}"
				COMMAND "${CMAKE_OBJCOPY}" --add-gnu-debuglink="${dbg_file}" "${exe_file}"
			)
		endif()
	endif()
	set_target_properties(${tgt} PROPERTIES OMR_DEBUG_FILE "${dbg_file}")
endfunction()

function(_omr_toolchain_process_exports TARGET_NAME)
	# we only need to do something if we are dealing with a shared library
	get_target_property(target_type ${TARGET_NAME} TYPE)
	if(NOT target_type STREQUAL "SHARED_LIBRARY")
		return()
	endif()

	# This does not work on OSX.
	if(OMR_OS_OSX)
		return()
	endif()

	set(exp_file "$<TARGET_PROPERTY:${TARGET_NAME},BINARY_DIR>/${TARGET_NAME}.exp")

	omr_process_template(
		"${omr_SOURCE_DIR}/cmake/modules/platform/toolcfg/gnu_exports.exp.in"
		"${exp_file}"
	)

if(NOT CMAKE_C_COMPILER_IS_OPENXL)
	target_link_libraries(${TARGET_NAME}
		PRIVATE
			"-Wl,--version-script,${exp_file}")
endif()
endfunction()
