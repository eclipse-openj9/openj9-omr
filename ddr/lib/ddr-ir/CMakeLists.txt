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
