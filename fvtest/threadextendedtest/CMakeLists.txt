###############################################################################
#
# (c) Copyright IBM Corp. 2017, 2017
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

add_executable(omrthreadextendedtest
	processTimeTest.cpp
	threadCpuTimeTest.cpp
	threadExtendedTestHelpers.cpp
	threadExtendedTestMain.cpp
	timeBaseTest.cpp
)


target_link_libraries(omrthreadextendedtest
	omrtestutil
	omrGtestGlue
	omrutil
	${OMR_PORT_LIB}
	omrutil
	${OMR_THREAD_LIB}
	omrcore
	${OMR_GC_LIB}
)

if(OMR_HOST_OS STREQUAL "zos")
	target_link_libraries(omrthreadextendedtest j9a2e)
endif()

add_test(NAME threadextendedtest COMMAND omrthreadextendedtest)

