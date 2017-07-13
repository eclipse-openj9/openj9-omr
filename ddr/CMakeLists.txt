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
