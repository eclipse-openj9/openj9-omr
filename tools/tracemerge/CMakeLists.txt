###############################################################################
# Copyright IBM Corp. and others 2017
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
# [2] https://openjdk.org/legal/assembly-exception.html
#
# SPDX-License-Identifier: EPL-2.0 OR Apache-2.0 OR GPL-2.0-only WITH Classpath-exception-2.0 OR GPL-2.0-only WITH OpenJDK-assembly-exception-1.0
###############################################################################

omr_add_executable(tracemerge
	DATMerge.cpp
	main.cpp
)

target_include_directories(tracemerge
	PRIVATE
		../tracegen/
)

target_link_libraries(tracemerge
	PRIVATE
		trace # static
)

if(OMR_OS_ZOS)
	if(OMR_TOOLS_USE_NATIVE_ENCODING)
		target_link_libraries(tracemerge PUBLIC omr_ebcdic)
	else()
		target_link_libraries(tracemerge PUBLIC omr_ascii)
	endif()
endif()

set_property(TARGET tracemerge PROPERTY FOLDER util)

install(TARGETS tracemerge
	RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
	COMPONENT tooling
)
