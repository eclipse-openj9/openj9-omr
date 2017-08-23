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


list(APPEND OMR_PLATFORM_DEFINITIONS
	-DWIN32
	-D_CRT_SECURE_NO_WARNINGS
	-DCRTAPI1=_cdecl
	-DCRTAPI2=_cdecl
	-D_WIN_95
	-D_WIN32_WINDOWS=0x0500
	-D_WIN32_DCOM
	-D_MT
	-D_WINSOCKAPI_
	-D_WIN32_WINVER=${OMR_WINVER}
	-D_WIN32_WINNT=${OMR_WINVER}
	-D_DLL
	-D_HAS_EXCEPTIONS=0
)

if(OMR_ENV_DATA64)
	list(APPEND OMR_PLATFORM_DEFINITIONS
		-DWIN64
		-D_AMD64_=1
	)
else()
	list(APPEND OMR_PLATFORM_DEFINITIONS
		-D_X86_
	)
endif()
