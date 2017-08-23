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

include_directories(${CMAKE_CURRENT_BINARY_DIR})

#TODO there is a bunch of stuff with vpaths, presumably for some extensibility reasons
# need to figure out if its actually needed and implement properly if required
add_library(j9hook_obj OBJECT
	${CMAKE_CURRENT_SOURCE_DIR}/hookable.cpp
	${CMAKE_CURRENT_BINARY_DIR}/ut_j9hook.c
)

target_include_directories(j9hook_obj
	PUBLIC
		.
)

add_library(j9hookstatic STATIC
	$<TARGET_OBJECTS:j9hook_obj>
)
target_include_directories(j9hookstatic
	PUBLIC
	$<TARGET_PROPERTY:j9hook_obj,INTERFACE_INCLUDE_DIRECTORIES>
)
target_link_libraries(j9hookstatic PUBLIC
	j9thrstatic
	j9pool
	omrport
)
add_tracegen(j9hook.tdf)

