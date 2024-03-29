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
###############################################################################

set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/../../../cmake/modules"
	${CMAKE_MODULE_PATH})

include(OmrUtility)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)
set(CMAKE_CXX_EXTENSIONS OFF)

set(TRIL_BACKEND_LIB      testcompiler)

omr_add_library(tril NOWARNINGS STATIC
	ast.cpp
	parser.cpp
	ilgen.cpp
	CallConverter.cpp
	GenericNodeConverter.cpp
	simple_compiler.cpp
)

# This is mildly peculiar. Because tril relies
# on jitbuilder as a 'backend' of sorts, we use
# its includes. Tril exports this as interface
make_compiler_target(tril PUBLIC COMPILER ${TRIL_BACKEND_LIB})

target_include_directories(tril PUBLIC
	${CMAKE_CURRENT_BINARY_DIR}
	${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(tril PUBLIC
	${TRIL_BACKEND_LIB}
	${CMAKE_DL_LIBS}
)

# TODO system thread library should be linked in a more generic way.
if(NOT (OMR_OS_WINDOWS OR OMR_OS_ZOS))
	target_link_libraries(tril PUBLIC pthread)
endif()

omr_add_executable(tril_dumper NOWARNINGS compiler.cpp)
target_link_libraries(tril_dumper tril)

omr_add_executable(tril_compiler NOWARNINGS compiler.cpp)
target_link_libraries(tril_compiler tril)

# The platform specific ${TR_CXX_COMPILE_OPTIONS} and ${TR_C_COMPILE_OPTIONS} compile options
# should been added, on Windows, for example, these options enable exception handling
# what is required for Clang.
target_compile_options(tril
	PUBLIC
		${TR_COMPILE_OPTIONS}
		$<$<COMPILE_LANGUAGE:CXX>:${TR_CXX_COMPILE_OPTIONS}>
		$<$<COMPILE_LANGUAGE:C>:${TR_C_COMPILE_OPTIONS}>
)

#export(TARGETS tril jitbuilder FILE tril-config.cmake)
#install(FILES ast.h ilgen.hpp DESTINATION include)
#install(TARGETS tril EXPORT tril-targets ARCHIVE DESTINATION lib)
#install(EXPORT tril-targets FILE tril-config.cmake DESTINATION lib/cmake/tril)

set_target_properties(tril tril_dumper tril_compiler PROPERTIES FOLDER fvtest/tril)
