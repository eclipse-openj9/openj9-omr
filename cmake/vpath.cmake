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


# VPathResolve takes a list of source files, a list of directories to search in,
# and produces a list of source files, prepended with the directory in which the
# file was found. This is inspired by the VPATH system of Make.
#
# ie. if you have a/a.c and b/b.c in your directories, and
#
#     inlist  = [a.c, b.c]
#     dirlist = [a, b]
#
# then
#
#     outlist = [a/a.c, b/b.c]
#
function(VPathResolve inlist dirlist outlist)
    set(output "")
  foreach(currentFile ${${inlist}})
    set(fileFound False)
    foreach(currentDir ${${dirlist}})
        if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/${currentDir}/${currentFile}")
            list(APPEND output "${currentDir}/${currentFile}")
            set(fileFound True)
            break()
        endif()
    endforeach()
    if(NOT fileFound)
        message(FATAL_ERROR "Could not find file ${currentFile} in ${${dirlist}}")
    endif()
  endforeach()
  set(${outlist} ${output} PARENT_SCOPE)
endfunction()

