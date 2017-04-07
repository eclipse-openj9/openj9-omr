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

set(CMAKE_ASM-ZOS_SOURCE_FILE_EXTENSIONS s)


set(CMAKE_ASM-ZOS_COMPILE_OBJECT "<CMAKE_ASM-ZOS_COMPILER> <INCLUDES> <FLAGS> <DEFINES> -o <OBJECT> <SOURCE>")

# Load the generic ASMInformation file:
set(ASM_DIALECT "-ZOS")
include(CMakeASMInformation)
set(ASM_DIALECT)
