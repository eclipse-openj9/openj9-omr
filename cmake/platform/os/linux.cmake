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

set(OMR_OS_DEFINITIONS 
	LINUX
	_FILE_OFFSET_BITS=64
)

set(OMR_OS_COMPILE_OPTIONS
	-pthread
)

# Check that we need to pull librt to get clock_gettime/settime family of functions
if(NOT DEFINED OMR_NEED_LIBRT)
	check_symbol_exists(clock_gettime time.h OMR_LIBC_HAS_CLOCK_GETTIME)
	if(OMR_LIBC_HAS_CLOCK_GETTIME)
		set(OMR_NEED_LIBRT FALSE CACHE BOOL "")
	else()
		set(OMR_NEED_LIBRT TRUE CACHE BOOL "")
	endif()
	mark_as_advanced(OMR_NEED_LIBRT)
endif()
