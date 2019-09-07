###############################################################################
# Copyright (c) 2019, 2019 IBM Corp. and others
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

# Find libz
# Will set:
#  LIBZ_FOUND
#  LIBZ_INCLUDE_DIRS
#  LIBZ_LIBRARIES
#  LIBZ_DEFINITIONS

find_path(ZLIB_H_INCLUDE_DIR zlib.h)

find_library(LIBZ_LIBRARY z)

include(FindPackageHandleStandardArgs)

find_package_handle_standard_args(LibZ
	DEFAULT_MSG
	LIBZ_LIBRARY
	ZLIB_H_INCLUDE_DIR
)

if(LIBZ_FOUND)
	set(LIBZ_INCLUDE_DIRS
		${ZLIB_H_INCLUDE_DIR}
	)
	set(LIBZ_LIBRARIES
		${LIBZ_LIBRARY}
	)
	set (LIBZ_DEFINITIONS "")
endif(LIBZ_FOUND)
