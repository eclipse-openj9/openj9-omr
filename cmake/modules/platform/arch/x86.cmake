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

if(OMR_ENV_DATA64)
	list(APPEND OMR_PLATFORM_DEFINITIONS
		-DJ9HAMMER
	)
else()
	list(APPEND OMR_PLATFORM_DEFINITIONS
		-DJ9X86
	)
endif()

# Testarossa build variables. Longer term the distinction between TR and the rest 
# of the OMR code should be heavily reduced. In the mean time, we keep
# the distinction
list(APPEND TR_COMPILE_DEFINITIONS -DTR_HOST_X86 -DTR_TARGET_X86)

set(TR_HOST_ARCH    x)

if(OMR_ENV_DATA64)
	list(APPEND TR_COMPILE_DEFINITIONS -DTR_HOST_64BIT -DTR_TARGET_64BIT)


	set(TR_HOST_SUBARCH amd64)
	set(TR_HOST_BITS    64)
	set(CMAKE_ASM-ATT_FLAGS "--64 --defsym TR_HOST_X86=1 --defsym TR_HOST_64BIT=1 --defsym BITVECTOR_64BIT=1 --defsym LINUX=1 --defsym TR_TARGET_X86=1 --defsym TR_TARGET_64BIT=1")
else()
		message(FATAL_ERROR "JIT isn't ready to build with CMake on this platform: ")
endif()
