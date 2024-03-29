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

omr_assert(
	TEST OMR_EXAMPLE
	MESSAGE "The fv gctest component relies on the example glue"
)

omr_add_executable(omrgctest
	GCConfigObjectTable.cpp
	GCConfigTest.cpp
	gcTestHelpers.cpp
	main.cpp
	StartupManagerTestExample.cpp
)

if (OMR_GC_VLHGC)
if (OMR_GC_VLHGC_CONCURRENT_COPY_FORWARD)
	target_sources(omrgctest
		PRIVATE
		TestHeapRegionStateTable.cpp
	)
endif()
endif()

#TODO this is a real gross, tangled mess
target_link_libraries(omrgctest
	omrGtestGlue
	pugixml
	omrtestutil
	omrcore
	omrvmstartup
	${OMR_GC_LIB}
	${OMR_PORT_LIB}
)

set_property(TARGET omrgctest PROPERTY FOLDER fvtest)

omr_add_test(NAME gctest
	COMMAND $<TARGET_FILE:omrgctest> "--gtest_filter=gcFunctionalTest*" "--gtest_output=xml:${CMAKE_CURRENT_BINARY_DIR}/omrgctest-results.xml"
	WORKING_DIRECTORY "${omr_SOURCE_DIR}"
)
