# Find libelf
# Will set:
#  LibElf_FOUND
#  LibElf_INCLUDE_DIRS
#  LibElf_LIBRARIES
#  LibElf_DEFINES

find_path(ELF_H_INCLUDE_DIR elf.h)

find_path(LIBELF_H_INCLUDE_DIR libelf.h)

set(LibElf_INCLUDE_DIRS
	${ELF_H_INCLUDE_DIR}
	${LIBELF_H_INCLUDE_DIR}
)

find_library(LIBELF_LIBRARY elf)

set(LibElf_LIBRARIES
	${LIBELF_LIBRARY}
)
set (LibElf_DEFINES "")

include(FindPackageHandleStandardArgs)

find_package_handle_standard_args(LibElf
	DEFAULT_MSG
	LIBELF_LIBRARY
	ELF_H_INCLUDE_DIR
	LIBELF_H_INCLUDE_DIR
)
