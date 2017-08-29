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

if(OMR_TRACEGEN_)
	return()
endif()
set(OMR_TRACEGEN 1)

#TODO: pehaps should detect output by searching for "executable=" line
#takes extra optional argument name to override output filename
function(omr_add_tracegen input)
	get_filename_component(input_dir "${input}" DIRECTORY)

	if(ARGV1)
		set(base_name "${ARGV1}")
	else()
		get_filename_component(base_name "${input}" NAME_WE)
	endif()
	file(TO_CMAKE_PATH "${CMAKE_CURRENT_BINARY_DIR}/ut_${base_name}" generated_filename)

	add_custom_command(
		OUTPUT "${generated_filename}.c" "${generated_filename}.h" "${generated_filename}.pdat"
		COMMAND tracegen -w2cd -treatWarningAsError -generatecfiles -threshold 1 -file ${CMAKE_CURRENT_SOURCE_DIR}/${input}
		DEPENDS ${input}
		WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
	)
endfunction(omr_add_tracegen)

macro(add_tracegen)
	omr_add_tracegen(${ARGN})
endmacro(add_tracegen)
