##############################################################################
#
# (c) Copyright IBM Corp. 2017,2017
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

list(APPEND OMR_PLATFORM_DEFINITIONS
	-DJ9VM_TIERED_CODE_CACHE
)

# Testarossa build variables. Longer term the distinction between TR and the rest 
# of the OMR code should be heavily reduced. In the mean time, we keep
# the distinction
set(TR_HOST_ARCH    z)
set(TR_HOST_BITS    64)
list(APPEND TR_COMPILE_DEFINITIONS TR_HOST_S390 TR_TARGET_S390)

if(OMR_ENV_DATA64)
	list(APPEND TR_COMPILE_DEFINITIONS TR_HOST_64BIT TR_TARGET_64BIT)
endif()
set(CMAKE_ASM-ATT_FLAGS "-noexecstack -march=z9-109")
