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

omr_add_executable(omrsigtest
	sigTest.cpp
	sigTestHelpers.cpp
	main.cpp
)

#TODO: hack for windows. These libraries should be linked on dependants
if(OMR_OS_WINDOWS)
	target_link_libraries(omrsigtest
		ws2_32
		shell32
		Iphlpapi
		psapi
		pdh
	)
endif()

target_include_directories(omrsigtest
	PRIVATE
		.
		../util/
)

target_link_libraries(omrsigtest
	omrGtestGlue
	j9hashtable
	omrsig
	omrvmstartup
	${OMR_PORT_LIB}
)

if(OMR_HOST_OS MATCHES "linux|osx")
	target_link_libraries(omrsigtest dl)
endif()

#TODO Unported makefile section:
#MODULE_INCLUDES += ../util
#MODULE_INCLUDES += $(OMR_GTEST_INCLUDES)
#MODULE_CXXFLAGS += $(OMR_GTEST_CXXFLAGS)

#vpath argmain.cpp $(top_srcdir)/fvtest/omrGtestGlue

#MODULE_STATIC_LIBS += \
	#omrGtest \
	#testutil \
	#omrstatic

#MODULE_SHARED_LIBS += omrsig

#ifeq (linux,$(OMR_HOST_OS))
	#MODULE_SHARED_LIBS += rt pthread
#endif
#ifeq (osx,$(OMR_HOST_OS))
	#MODULE_SHARED_LIBS += iconv pthread
#endif
#ifeq (aix,$(OMR_HOST_OS))
	#MODULE_SHARED_LIBS += iconv perfstat
#endif
#ifeq (win,$(OMR_HOST_OS))
	#MODULE_SHARED_LIBS += ws2_32 shell32 Iphlpapi psapi pdh
#endif

set_property(TARGET omrsigtest PROPERTY FOLDER fvtest)

omr_add_test(NAME sigtest COMMAND $<TARGET_FILE:omrsigtest> --gtest_output=xml:${CMAKE_CURRENT_BINARY_DIR}/omrsigtest-results.xml)
