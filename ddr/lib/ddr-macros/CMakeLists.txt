add_library(omr_ddr_macros
	MacroInfo.cpp
	MacroTool.cpp
)

set_property(TARGET omr_ddr_macros PROPERTY CXX_STANDARD 11)

target_link_libraries(omr_ddr_macros
	omr_ddr_base
	omr_ddr_ir
)
