###############################################################################
# Copyright (c) 2018, 2018 IBM Corp. and others
#
# This program and the accompanying materials are made available under
# the terms of the Eclipse Public License 2.0 which accompanies this
# distribution and is available at https://www.eclipse.org/legal/epl-2.0/
# or the Apache License, Version 2.0 which accompanies this distribution and
# is available at https://www.apache.org/licenses/LICENSE-2.0.
#
# This Source Code may also be made available under the following
# Secondary Licenses when the conditions for such availability set
# forth in the Eclipse Public License, v. 2.0 are satisfied: GNU
# General Public License, version 2 with the GNU Classpath
# Exception [1] and GNU General Public License, version 2 with the
# OpenJDK Assembly Exception [2].
#
# [1] https://www.gnu.org/software/classpath/license.html
# [2] http://openjdk.java.net/legal/assembly-exception.html
#
# SPDX-License-Identifier: EPL-2.0 OR Apache-2.0 OR GPL-2.0 WITH Classpath-exception-2.0 OR LicenseRef-GPL-2.0 WITH Assembly-exception
###############################################################################

include(OmrCompilerSupport)

# Add the contents of the macro to CORE_COMPILER_OBJECTS in this scope.
# The library name parameter is currently ignored.
macro(compiler_library libraryname)
	list(APPEND CORE_COMPILER_OBJECTS ${ARGN})
endmacro(compiler_library)

# Instead of a real add_subdirectory we just include the file so everything stays
# in the same scope.
macro(add_compiler_subdirectory dir)
	include("${omr_SOURCE_DIR}/compiler/${dir}/CMakeLists.txt")
endmacro(add_compiler_subdirectory)


# create an object list which is populated in the subdirectories
set(CORE_COMPILER_OBJECTS "")

add_compiler_subdirectory(codegen)
add_compiler_subdirectory(compile)
add_compiler_subdirectory(control)
add_compiler_subdirectory(env)
add_compiler_subdirectory(il)
add_compiler_subdirectory(ilgen)
add_compiler_subdirectory(infra)
add_compiler_subdirectory(optimizer)
add_compiler_subdirectory(ras)
add_compiler_subdirectory(runtime)

add_compiler_subdirectory(${TR_TARGET_ARCH})

# Promote the list variable into a global property.
# That way we dont need to worry about scoping issues.
set_property(GLOBAL PROPERTY OMR_CORE_COMPILER_OBJECTS ${CORE_COMPILER_OBJECTS})
