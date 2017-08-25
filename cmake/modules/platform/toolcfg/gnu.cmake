###############################################################################
#
# (c) Copyright IBM Corp. 2017, 2017
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

set(OMR_WARNING_AS_ERROR_FLAG -Werror)

if(OMR_ENV_DATA64)
	list(APPEND OMR_PLATFORM_COMPILE_OPTIONS
		-m64
	)
else()
	list(APPEND OMR_PLATFORM_COMPILE_OPTIONS
		-m32
		-msse2
	)
endif()

# Testarossa build variables. Longer term the distinction between TR and the rest 
# of the OMR code should be heavily reduced. In the mean time, we keep
# the distinction

list(APPEND TR_COMPILE_OPTIONS 
	-std=c++0x
	-Wno-write-strings #This warning swamps almost all other output
) 

set(PASM_CMD ${CMAKE_C_COMPILER}) 
set(PASM_FLAGS -x assembler-with-cpp -E -P) 

set(SPP_CMD ${CMAKE_C_COMPILER}) 
set(SPP_FLAGS -x assembler-with-cpp -E -P) 
