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


add_library(omr_ddr_scanner
	Scanner.cpp
)

set_property(TARGET omr_ddr_scanner PROPERTY CXX_STANDARD 11)

target_link_libraries(omr_ddr_scanner
	PUBLIC
		omr_ddr_base
		omr_ddr_ir
)

if(OMR_HOST_OS STREQUAL "win")
	find_package(DiaSDK REQUIRED)
	target_sources(omr_ddr_scanner
		PRIVATE
			pdb/PdbScanner.cpp
	)
	target_link_libraries(omr_ddr_scanner
		PUBLIC
			${DIASDK_LIBRARIES}
	)
	target_include_directories(omr_ddr_scanner
		PUBLIC
			${DIASDK_INCLUDE_DIRS}
	)
elseif(OMR_HOST_OS STREQUAL "linux")
	
	find_package(LibDwarf REQUIRED)

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
