# Find DIA SDK
# Will search the environment variable DIASDK first.
#
# Will set:
#   DIASDK_FOUND
#   DIASDK_INCLUDE_DIRS
#   DIASDK_LIBRARIES
#   DIASDK_DEFINITIONS

find_path(DIA2_H_DIR "dia2.h"
	HINTS
		"$ENV{DIASDK}\\include"
		"$ENV{VSSDK140Install}..\\DIA SDK\\include"
)

find_library(DIAGUIDS_LIBRARY "diaguids"
	HINTS
		"$ENV{DIASDK}\\lib"
		"$ENV{VSSDK140Install}..\\DIA SDK\\lib"
)

include (FindPackageHandleStandardArgs)

find_package_handle_standard_args(DiaSDK
	DEFAULT_MSG
	DIAGUIDS_LIBRARY
	DIA2_H_DIR
)

if(DIASDK_FOUND)
	set(DIASDK_DEFINITIONS -DHAVE_DIA)
	set(DIASDK_INCLUDE_DIRS ${DIA2_H_DIR})
	set(DIASDK_LIBRARIES ${DIAGUIDS_LIBRARY})
endif(DIASDK_FOUND)

