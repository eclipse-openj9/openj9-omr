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
