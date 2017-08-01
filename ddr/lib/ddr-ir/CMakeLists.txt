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

add_library(omr_ddr_ir
	ClassType.cpp
	ClassUDT.cpp
	EnumMember.cpp
	EnumUDT.cpp
	Field.cpp
	Macro.cpp
	Members.cpp
	Modifiers.cpp
	NamespaceUDT.cpp
	Symbol_IR.cpp
	Type.cpp
	TypedefUDT.cpp
	UDT.cpp
	UnionUDT.cpp
)

set_property(TARGET omr_ddr_ir PROPERTY CXX_STANDARD 11)

target_link_libraries(omr_ddr_ir
	omr_ddr_base
)
