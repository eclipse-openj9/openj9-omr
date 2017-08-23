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

# Include once
if(OMR_UTILITY_)
	return()
endif()
set(OMR_UTILITY_ 1)

include(OmrAssert)

# omr_add_prefix(<output_variable> <prefix> <element>...)
# Given a prefix, and a list of arguments, prefix the list of arguments and
# assign to out: ie, add_prefix(out "-I" "a;b;c") should set out to
# "-Ia;-Ib;-Ic".
function(omr_add_prefix out prefix)
   set(ret "")
   foreach(var IN ITEMS ${ARGN})
      list(APPEND ret "${prefix}${var}")
   endforeach()
   set(${out} ${ret} PARENT_SCOPE)
endfunction(omr_add_prefix)

# omr_trim_whitespace(<output_variable> <string>)
# Trim repeated whitespace from <string> and assign to <output_variable>.
# Does not remove leading/trailing whitespace. For that, use string(STRIP ...)
function(omr_trim_whitespace variable string)
	string(REGEX REPLACE " +" " " result "${string}")
	set(${variable} "${result}" PARENT_SCOPE)
endfunction(omr_trim_whitespace)

# omr_remove_flags(<output_variable> [<flag>...])
# Remove flags from a string.
function(omr_remove_flags variable)
	set(result "${${variable}}")
	foreach(option IN ITEMS ${ARGN})
		string(REGEX REPLACE "(^| )${option}( |$)" " " result "${result}")
	endforeach()
	omr_trim_whitespace(result "${result}")
	set(${variable} "${result}" PARENT_SCOPE)
endfunction(omr_remove_flags)

# omr_stringify(<output_variable> <item>...)
# Convert items to a string of space-separated items.
function(omr_stringify output)
	string(REPLACE ";" " " result "${ARGN}")
	set(${output} ${result} PARENT_SCOPE)
endfunction(omr_stringify)

# omr_append_flags(<output_variable> [<flag>...])
# Append flags to variable. Flags are space separated.
# <variable> is a string, <flag> is a list.
function(omr_append_flags variable)
	omr_stringify(flags ${ARGN})
	set(${variable} "${${variable}} ${flags}" PARENT_SCOPE)
endfunction(omr_append_flags)
