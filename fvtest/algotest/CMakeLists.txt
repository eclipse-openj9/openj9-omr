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

add_hookgen(hooksample.hdf
	${CMAKE_CURRENT_SOURCE_DIR}/hooksample.h
	${CMAKE_CURRENT_SOURCE_DIR}/hooksample_internal.h
)

add_executable(omralgotest
	algoTest.cpp
	algorithm_test_internal.h
	avltest.c
	avltest.lst
	hashtabletest.c
	hooksample.h
	hooksample_internal.h
	#hooksample.hdf
	#hooksample.sentinel
	#hooksample_internal.h
	hooktest.c
	main.cpp
	pooltest.c
)

target_link_libraries(omralgotest
	j9avl
	j9hashtable
	j9pool
	omrGtestGlue
	#omrGtest
	omrcore
	omrport
	omrtestutil
)

#TODO hack to get building
if(OMR_HOST_OS STREQUAL "linux")
	target_link_libraries(omralgotest
		rt
	)
elseif(OMR_HOST_OS STREQUAL "win")
	target_link_libraries(omralgotest
		ws2_32
		shell32
		Iphlpapi
		psapi
		pdh
		omrglue
	)
endif()

target_link_libraries(omralgotest
	j9hookstatic
)

if(OMR_HOST_OS STREQUAL "zos")
	target_link_libraries(omralgotest j9a2e)
endif()

add_test(NAME algotest COMMAND omralgotest -avltest:${CMAKE_CURRENT_SOURCE_DIR}/avltest.lst)
