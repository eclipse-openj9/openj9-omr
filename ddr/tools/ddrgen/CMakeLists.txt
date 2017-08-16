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

add_executable(omr_ddrgen
	ddrgen.cpp
)

set_property(
	TARGET omr_ddrgen
	PROPERTY CXX_STANDARD 11
)

target_link_libraries(omr_ddrgen
	omr_ddr_base
	omr_ddr_blobgen
	omr_ddr_ir
	omr_ddr_macros
	omr_ddr_scanner
	omrport
)
