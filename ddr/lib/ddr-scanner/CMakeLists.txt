find_package(LibDwarf)

add_library(omr_ddr_scanner
	Scanner.cpp
)

set_property(TARGET omr_ddr_scanner PROPERTY CXX_STANDARD 11)

target_link_libraries(omr_ddr_scanner
	PUBLIC
		omr_ddr_base
		omr_ddr_ir
)

if(LIBDWARF_FOUND)
	target_link_libraries(omr_ddr_scanner
		PUBLIC
			${LIBDWARF_LIBRARIES}
	)
	target_include_directories(omr_ddr_scanner
		PUBLIC
			${LIBDWARF_INCLUDE_DIRS}
	)
	target_compile_definitions(omr_ddr_scanner
		PUBLIC
			${LIBDWARF_DEFINITIONS}
	)
endif(LIBDWARF_FOUND)

if(OMR_HOST_OS STREQUAL "win")
	find_package(DiaSDK REQUIRED)
	target_sources(omr_ddr_scanner
		PRIVATE
			pdb/PdbScanner.cpp
	)
	target_link_libraries(omr_ddr_scanner
		PUBLIC
			${DiaSDK_LIBRARIES}
	)
	target_include_directories(omr_ddr_scanner
		PUBLIC
			${DiaSDK_INCLUDE_DIRS}
	)
elseif(OMR_HOST_OS STREQUAL "linux")
	target_sources(omr_ddr_scanner
		PRIVATE
			dwarf/DwarfFunctions.cpp
			dwarf/DwarfScanner.cpp
	)

elseif(OMR_HOST_OS STREQUAL "osx")
	target_sources(omr_ddr_scanner
		PRIVATE
			dwarf/DwarfFunctions.cpp
			dwarf/DwarfParser.cpp
			dwarf/DwarfScanner.cpp
	)
elseif(OMR_HOST_OS STREQUAL "aix")
	target_sources(omr_ddr_scanner
		PRIVATE
			dwarf/AixSymbolTableParser.cpp
			dwarf/DwarfFunctions.cpp
			dwarf/DwarfScanner.cpp
	)
	target_link_libraries(omr_ddr_scanner
		PUBLIC
			${LIBDWARF_LIBRARIES}
	)
endif()
