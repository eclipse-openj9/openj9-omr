# Find libdwarf.
# Requires:
#   LibElf
# Will set:
#   LibDwarf_FOUND
#   LibDwarf_INCLUDE_DIRS
#   LibDwarf_LIBRARIES
#   LibDwarf_DEFINES

find_package(LibElf)

set(LibDwarf_DEFINITIONS "")

find_path(DWARF_H_INCLUDE_DIR "dwarf.h")
if(DWARF_H_INCLUDE_DIR)
	list(APPEND LibDwarf_DEFINITIONS -DHAVE_DWARF_H=1)
endif()

find_path(LIBDWARF_H_INCLUDE_DIR "libdwarf/dwarf.h")
if(LIBDWARF_H_INCLUDE_DIR)
	list(APPEND LibDwarf_DEFINITIONS -DHAVE_LIBDWARF_LIBDWARF_H=1)
else()
	find_path(LIBDWARF_H_INCLUDE_DIR "libdwarf.h")
	if(LIBDWARF_H_INCLUDE_DIR)
		list(APPEND LibDwarf_DEFINITIONS -DHAVE_LIBDWARF_H=1)
	endif()
endif()


find_library(LIBDWARF_LIBRARY dwarf)

include (FindPackageHandleStandardArgs)

set(LibDwarf_INCLUDE_DIRS
	${DWARF_H_INCLUDE_DIR}
	${LIBDWARF_H_INCLUDE_DIR}
	${LibElf_INCLUDE_DIRS}
)

set(LibDwarf_LIBRARIES
	${LIBDWARF_LIBRARY}
	${LibElf_LIBRARIES}
)

find_package_handle_standard_args(LibDwarf
	DEFAULT_MSG
	LibElf_FOUND
	DWARF_H_INCLUDE_DIR
	LIBDWARF_LIBRARY
	LIBDWARF_H_INCLUDE_DIR
)
