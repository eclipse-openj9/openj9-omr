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

if (NOT CMAKE_CROSSCOMPILING)

	add_subdirectory(hookgen)
	add_subdirectory(tracemerge)
	add_subdirectory(tracegen)

	export(TARGETS hookgen tracemerge tracegen FILE "ImportTools.cmake")

else(NOT CMAKE_CROSSCOMPILING)

	set(OMR_TOOLS_IMPORTFILE "IMPORTFILE-NOTFOUND" CACHE FILEPATH
		"Point it to the ImportTools.cmake file of a native build")

	#Logically we should be importing the file here, however,
	#burried in the cmake docs, it says imported targets are directory scoped
	#thus they would be useless if imported here
	#include(${OMR_TOOLS_IMPORTFILE})

endif(NOT CMAKE_CROSSCOMPILING)


#TODO: currently output in source tree, should be in build tree
#TODO: Dependecy checking is broken, since it checks for output in build tree rather than src
function(add_hookgen input)
	get_filename_component(input_dir "${input}" DIRECTORY)
	#message("Hookgen, Output = ${ARGN}")
	#message("Hookgen, depends = ${CMAKE_CURRENT_SOURCE_DIR}/${input}")
	add_custom_command(
		OUTPUT ${ARGN}
		COMMAND hookgen ${CMAKE_CURRENT_SOURCE_DIR}/${input}
		DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/${input}"
		WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/${input_dir}
	)
endfunction()


#TODO: pehaps should detect output by searching for "executable=" line
#takes extra optional argument name to override output filename
function(add_tracegen input)
	get_filename_component(input_dir "${input}" DIRECTORY)

	if(ARGV1)
		set(base_name "${ARGV1}")
	else()
		get_filename_component(base_name "${input}" NAME_WE)
	endif()
	#file(TO_CMAKE_PATH "${CMAKE_CURRENT_BINARY_DIR}/${input_dir}/ut_${base_name}" generated_filename)
	file(TO_CMAKE_PATH "${CMAKE_CURRENT_BINARY_DIR}/ut_${base_name}" generated_filename)

	add_custom_command(
		OUTPUT "${generated_filename}.c" "${generated_filename}.h" "${generated_filename}.pdat"
		COMMAND $<TARGET_FILE:tracegen> -w2cd -treatWarningAsError -generatecfiles -threshold 1 -file ${CMAKE_CURRENT_SOURCE_DIR}/${input}
		DEPENDS ${input}
		WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
	)
endfunction()

#TODO we still dont do tracemerge properly
