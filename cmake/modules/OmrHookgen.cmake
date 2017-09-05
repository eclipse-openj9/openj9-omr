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

if(OMR_HOOKGEN_)
	return()
endif()
set(OMR_HOOKGEN_ 1)

# Process an input hookgen file to generate output source files.
# Usage: omr_add_hookgen(<input> <output>...)
# TODO: currently output in source tree, should be in build tree
# TODO: Dependecy checking is broken, since it checks for output in build tree rather than src
function(omr_add_hookgen input)
	get_filename_component(input_dir "${input}" DIRECTORY)
	add_custom_command(
		OUTPUT ${ARGN}
		COMMAND hookgen ${CMAKE_CURRENT_SOURCE_DIR}/${input}
		DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/${input}"
		WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/${input_dir}
	)
endfunction(omr_add_hookgen)

macro(add_hookgen)
	omr_add_hookgen(${ARGN})
endmacro(add_hookgen)
