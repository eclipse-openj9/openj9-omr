###############################################################################
# Copyright IBM Corp. and others 2019
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

# Inputs:
# LIBRARY_FILE_NAME - name of the generated dll file (no path)
# ARCHIVE_DIR - Archive directory for the target
# RUNTIME_DIR - Runtime directory for the target
# BINARY_DIR - CMake Binary Dir for the library target

# xlc creates the output file name by taking the name of the generated library
# and change the file exentsion to .x. (or appending if no extension exists)
# e.g.
# - foo.so => foo.x
# - foo.bar.baz => foo.bar.x
# - foo => foo.x

# if ARCHIVE_DIR not set, assume it is the same as RUNTIME_DIR
if(NOT ARCHIVE_DIR)
	set(ARCHIVE_DIR ${RUNTIME_DIR})
endif()

if(NOT EXISTS "${ARCHIVE_DIR}")
	file(MAKE_DIRECTORY "${ARCHIVE_DIR}")
endif()
string(FIND "${LIBRARY_FILE_NAME}" "." dot_pos REVERSE)
string(SUBSTRING "${LIBRARY_FILE_NAME}" 0 ${dot_pos} base_name)

set(SRC_FILE "${CMAKE_BINARY_DIR}/${base_name}.x")
set(DEST_FILE "${ARCHIVE_DIR}/${base_name}.x")

if(EXISTS ${SRC_FILE} AND NOT "${SRC_FILE}" STREQUAL "${DEST_FILE}")
	file(RENAME "${SRC_FILE}" "${DEST_FILE}")
endif()

# Work around a bug in CMake where it looks for .x files in the runime dir rather than the archive dir.
if(EXISTS ${DEST_FILE} AND NOT "${ARCHIVE_DIR}" STREQUAL "${RUNTIME_DIR}")
	file(COPY "${DEST_FILE}" DESTINATION "${RUNTIME_DIR}")
endif()
