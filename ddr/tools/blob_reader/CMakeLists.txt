add_executable(omr_blob_reader
	blob_reader.cpp
)

set_property(TARGET omr_blob_reader PROPERTY CXX_STANDARD 11)

target_link_libraries(omr_blob_reader
	omr_ddr_base
	omr_ddr_blobgen
	omr_ddr_ir
	omr_ddr_macros
	omr_ddr_scanner
)
