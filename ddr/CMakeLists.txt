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

if(NOT OMR_DDR)
	message(FATAL_ERROR ddr disabled)
endif(NOT OMR_DDR)

add_library(omr_ddr_base INTERFACE)

target_include_directories(omr_ddr_base
	INTERFACE
		include/
)

add_subdirectory(lib)
add_subdirectory(tools)
