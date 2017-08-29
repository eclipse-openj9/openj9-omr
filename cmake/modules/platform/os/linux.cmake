##############################################################################
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

include(CheckSymbolExists)

list(APPEND OMR_PLATFORM_DEFINITIONS
	-DLINUX
	-D_FILE_OFFSET_BITS=64
)

if(OMR_HOST_ARCH STREQUAL "ppc")
	if(OMR_ENV_DATA64)
		list(APPEND OMR_PLATFORM_DEFINITIONS
			-DLINUXPPC64
		)
	endif()
	list(APPEND OMR_PLATFORM_DEFINITIONS
		-DLINUXPPC
	)
elseif(OMR_HOST_ARCH STREQUAL "s390")
	if(OMR_ENV_DATA64)
		list(APPEND OMR_PLATFORM_DEFINITIONS
			-DS39064
		)
	endif()

	list(APPEND OMR_PLATFORM_DEFINITIONS
		-DS390
		-D_LONG_LONG
	)
endif()

# Check that we need to pull librt to get clock_gettime/settime family of functions
if(NOT DEFINED OMR_NEED_LIBRT)
	check_symbol_exists(clock_gettime time.h OMR_LIBC_HAS_CLOCK_GETTIME)
	if(OMR_LIBC_HAS_CLOCK_GETTIME)
		set(OMR_NEED_LIBRT FALSE CACHE BOOL "")
	else()
		set(OMR_NEED_LIBRT TRUE CACHE BOOL "")
		set(CMAKE_CXX_STANDARD_LIBRARIES "${CMAKE_CXX_STANDARD_LIBRARIES} -lrt")
	endif()
	mark_as_advanced(OMR_NEED_LIBRT)
endif()

# Testarossa build variables. Longer term the distinction between TR and the rest 
# of the OMR code should be heavily reduced. In the mean time, we keep
# the distinction
list(APPEND TR_COMPILE_DEFINITIONS -DSUPPORTS_THREAD_LOCAL -DLINUX)

