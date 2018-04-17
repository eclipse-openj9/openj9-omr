###############################################################################
# Copyright (c) 2017, 2018 IBM Corp. and others
#
# This program and the accompanying materials are made available under
# the terms of the Eclipse Public License 2.0 which accompanies this
# distribution and is available at http://eclipse.org/legal/epl-2.0
# or the Apache License, Version 2.0 which accompanies this distribution
# and is available at https://www.apache.org/licenses/LICENSE-2.0.
#
# This Source Code may also be made available under the following Secondary
# Licenses when the conditions for such availability set forth in the
# Eclipse Public License, v. 2.0 are satisfied: GNU General Public License,
# version 2 with the GNU Classpath Exception [1] and GNU General Public
# License, version 2 with the OpenJDK Assembly Exception [2].
#
# [1] https://www.gnu.org/software/classpath/license.html
# [2] http://openjdk.java.net/legal/assembly-exception.html
#
# SPDX-License-Identifier: EPL-2.0 OR Apache-2.0 OR GPL-2.0 WITH Classpath-exception-2.0 OR LicenseRef-GPL-2.0 WITH Assembly-exception
#############################################################################

list(APPEND OMR_PLATFORM_DEFINITIONS
	-DOMR_OS_WINDOWS
	-D_CRT_SECURE_NO_WARNINGS
	-DCRTAPI1=_cdecl
	-DCRTAPI2=_cdecl
	-D_MT
	-D_WINSOCKAPI_
	-D_DLL
	-D_HAS_EXCEPTIONS=0
	-D_VARIADIC_MAX=10
	-D_WIN32_WINNT=0x0501
	-DWINVER=0x0501
)
