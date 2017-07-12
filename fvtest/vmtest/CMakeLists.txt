###############################################################################
#
# (c) Copyright IBM Corp. 2017,2017
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

add_executable(omrvmtest
	main.cpp
)
if(OMR_THR_FORK_SUPPORT)
	target_sources(omrvmtest PRIVATE vmForkTest.cpp)
endif()

target_link_libraries(omrvmtest
	omrGtestGlue
	omrtestutil
	omrcore
	omrtrace
	omrgc
	j9hookstatic
	omrglue
	j9thrstatic
	omrutil
	omrport
	j9hashtable
	j9avl
	j9pool
	omrgc
	omrglue
	omrcore

)

add_test(NAME vmtest COMMAND omrvmtest)


