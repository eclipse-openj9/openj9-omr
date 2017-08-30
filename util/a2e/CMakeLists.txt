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

add_library(j9a2e SHARED
	sysTranslate.c
	atoe.c
	atoe_utils.c
)

target_link_libraries(j9a2e PRIVATE omr_shared)

target_include_directories(j9a2e
	PUBLIC
		.
)

add_custom_command(TARGET j9a2e
	POST_BUILD
	COMMAND mv j9a2e.x ${CMAKE_BINARY_DIR}/j9a2e.x
	WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
)

