add_library(omr_ddr_blobgen
	java/genBinaryBlob.cpp
	java/genBlobJava.cpp
	java/genSuperset.cpp
)

set_property(TARGET omr_ddr_blobgen PROPERTY CXX_STANDARD 11)

target_link_libraries(omr_ddr_blobgen
	omr_ddr_base
	omr_ddr_ir
	omr_ddr_macros
	omr_ddr_scanner
)
