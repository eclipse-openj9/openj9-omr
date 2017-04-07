###############################################################################
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

add_executable(omrsigtest
	sigTest.cpp
	sigTestHelpers.cpp
	main.cpp
)

#TODO: hack for windows. These libraries should be linked on dependants
if(OMR_HOST_OS STREQUAL "win")
	target_link_libraries(omrsigtest
		ws2_32
		shell32
		Iphlpapi
		psapi
		pdh
	)
endif()

if(OMR_HOST_OS STREQUAL "zos")
	target_link_libraries(omrsigtest j9a2e)
endif()

#TODO pretty sure omrtestutil only used for headers
target_link_libraries(omrsigtest
	omrGtestGlue
	omrtestutil
	j9thrstatic
	omrport
	omrglue
	omrsig
)

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

add_test(NAME sigtest COMMAND omrsigtest)
