# Find DIA SDK
# Will search the environment variable DIASDK first.

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
	DIA2_H_DIR
	DIAGUIDS_LIBRARY
)

if (DiaSDK_FOUND)
	set(DiaSDK_INCLUDE_DIRS ${DIA2_H_DIR})
	set(DiaSDK_LIBRARIES ${DIAGUIDS_LIBRARY})
endif(DiaSDK_FOUND)
