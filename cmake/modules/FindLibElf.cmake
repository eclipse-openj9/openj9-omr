# Find libelf
# Will set:
#  LIBELF_FOUND
#  LIBELF_INCLUDE_DIRS
#  LIBELF_LIBRARIES
#  LIBELF_DEFINES

find_path(ELF_H_INCLUDE_DIR elf.h)

find_path(LIBELF_H_INCLUDE_DIR libelf.h)

set(LIBELF_INCLUDE_DIRS
	${ELF_H_INCLUDE_DIR}
	${LIBELF_H_INCLUDE_DIR}
)

find_library(LIBELF_LIBRARY elf)

set(LIBELF_LIBRARIES
	${LIBELF_LIBRARY}
)
set (LIBELF_DEFINES "")

include(FindPackageHandleStandardArgs)

find_package_handle_standard_args(LibElf
	DEFAULT_MSG
	LIBELF_LIBRARY
	ELF_H_INCLUDE_DIR
	LIBELF_H_INCLUDE_DIR
)
