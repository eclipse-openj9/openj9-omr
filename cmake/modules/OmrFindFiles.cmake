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

include(OmrAssert)

# omr_find_files(<output variable> PATHS <directory>... [FILES <file>...])
#
# Search PATHS for files.
# This function takes an ordered list of directories to search.
# The output is the full path to each found file.
#
# ie. if you have dir1/a.c and dir2/b.c in your directories, and
#
#     FILES  = a.c; b.c
#     PATH = dir1, dir2
#
# then
#
#     OUTPUT = dir1/a.c, dir2/b.c
#
function(omr_find_files output_variable)
	cmake_parse_arguments(arg "" "" "PATHS;FILES" ${ARGN})
	omr_assert(TEST DEFINED arg_PATHS MESSAGE "omr_find_files called without PATHS")

	set(paths "")

	foreach(path ${arg_PATHS})
		if(NOT IS_ABSOLUTE "${path}")
			set(path "${CMAKE_CURRENT_SOURCE_DIR}/${path}")
		endif()
		omr_assert(WARNING TEST EXISTS "${path}" MESSAGE "Could not find path: ${path}")
		list(APPEND paths ${path})
	endforeach()

	set(result "")

	foreach(file ${arg_FILES})
		set(file_found FALSE)
		foreach(path ${paths})
			if(EXISTS "${path}/${file}")
				list(APPEND result "${path}/${file}")
				set(file_found TRUE)
				break()
			endif()
		endforeach()
		omr_assert(FATAL_ERROR TEST file_found MESSAGE "Could not find file: ${file}")
	endforeach()

	set(${output_variable} "${result}" PARENT_SCOPE)
endfunction()
