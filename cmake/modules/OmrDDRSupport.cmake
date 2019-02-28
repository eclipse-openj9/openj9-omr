###############################################################################
# Copyright (c) 2018, 2019 IBM Corp. and others
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

if(_OMR_DDR_SUPPORT)
	return()
endif()
set(_OMR_DDR_SUPPORT 1)

include(OmrAssert)
include(OmrUtility)
include(ExternalProject)

set(OMR_MODULES_DIR ${CMAKE_CURRENT_LIST_DIR})

function(make_ddr_set set_name)
	# if DDR is not enabled, just skip
	# Also skip if we are on windows since it is unsupported at the moment
	if((OMR_HOST_OS STREQUAL "win") OR (NOT OMR_DDR))
		return()
	endif()
	set(DDR_TARGET_NAME "${set_name}_ddr")
	set(DDR_BIN_DIR "${CMAKE_CURRENT_BINARY_DIR}/${DDR_TARGET_NAME}")
	set(DDR_TARGETS_LIST "${DDR_BIN_DIR}/targets.list")
	set(DDR_MACRO_INPUTS_FILE "${DDR_BIN_DIR}/macros.list")
	set(DDR_TOOLS_EXPORT "${omr_BINARY_DIR}/ddr/tools/DDRTools.cmake")

	add_custom_command(
		OUTPUT "${DDR_BIN_DIR}/config.stamp"
		COMMAND "${CMAKE_COMMAND}" -G "${CMAKE_GENERATOR}" .
		WORKING_DIRECTORY "${DDR_BIN_DIR}"
	)
	add_custom_target(${DDR_TARGET_NAME} DEPENDS "${DDR_BIN_DIR}/config.stamp")
	set_property(TARGET "${DDR_TARGET_NAME}" PROPERTY DDR_BIN_DIR "${DDR_BIN_DIR}")

	file(READ ${OMR_MODULES_DIR}/ddr/DDRSetStub.cmake.in cmakelist_template)
	string(CONFIGURE "${cmakelist_template}" cmakelist_template @ONLY)
	file(GENERATE OUTPUT ${DDR_BIN_DIR}/CMakeLists.txt CONTENT "${cmakelist_template}")
endfunction(make_ddr_set)


function(target_enable_ddr tgt ddr_set)
	if((OMR_HOST_OS STREQUAL "win") OR (NOT OMR_DDR))
		return()
	endif()

	set(DDR_SET_TARGET "${ddr_set}_ddr")
	omr_assert(FATAL_ERROR TEST TARGET ${tgt} MESSAGE "target_enable_ddr called on non-existant target ${tgt}")
	omr_assert(FATAL_ERROR TEST TARGET "${DDR_SET_TARGET}" MESSAGE "target_enable_ddr called on non-existant ddr_set ${ddr_set}")

	get_target_property(target_type "${tgt}" TYPE)
	if(target_type MATCHES "INTERFACE_LIBRARY")
		message(FATAL_ERROR "Cannot call enable_ddr on interface libraries")
	endif()

	get_property(DDR_BIN_DIR TARGET "${DDR_SET_TARGET}" PROPERTY DDR_BIN_DIR)

	omr_join("\n" MAGIC_TEMPLATE
		"SOURCE_DIR"
		"$<TARGET_PROPERTY:${tgt},SOURCE_DIR>"
		"INCLUDE_PATH"
		"$<JOIN:$<TARGET_PROPERTY:${tgt},INCLUDE_DIRECTORIES>,\n>"
		"DEFINES"
		"$<JOIN:$<TARGET_PROPERTY:${tgt},COMPILE_DEFINITIONS>,\n>"
		"SOURCES"
		"$<JOIN:$<TARGET_PROPERTY:${tgt},SOURCES>,\n>"
		"HEADERS"
		"$<JOIN:$<TARGET_PROPERTY:${tgt},DDR_HEADERS>,\n>"
		"PREINCLUDES"
		"$<JOIN:$<TARGET_PROPERTY:${tgt},DDR_PREINCLUDES>,\n>"
	)
	if(target_type MATCHES "EXECUTABLE|SHARED_LIBRARY")
		set(MAGIC_TEMPLATE "OUTPUT_FILE\n$<TARGET_FILE:${tgt}>\n${MAGIC_TEMPLATE}")
	endif()

	file(GENERATE OUTPUT "${DDR_BIN_DIR}/${tgt}.txt" CONTENT "${MAGIC_TEMPLATE}\n")
	set_property(TARGET ${DDR_SET_TARGET} APPEND PROPERTY INPUT_TARGETS ${tgt})
endfunction(target_enable_ddr)
