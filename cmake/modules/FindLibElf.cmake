find_path(LIBELF_INCLUDE_DIRS elf.h)

find_library(LIBELF_LIBRARIES elf)

include(FindPackageHandleStandardArgs)

find_package_handle_standard_args(LibElf
	DEFAULT_MSG
	LIBELF_LIBRARIES
	LIBELF_INCLUDE_DIRS
)
