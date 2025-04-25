###############################################################################
# Copyright IBM Corp. and others 2017
#
# This program and the accompanying materials are made available under
# the terms of the Eclipse Public License 2.0 which accompanies this
# distribution and is available at https://www.eclipse.org/legal/epl-2.0/
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
# [2] https://openjdk.org/legal/assembly-exception.html
#
# SPDX-License-Identifier: EPL-2.0 OR Apache-2.0 OR GPL-2.0-only WITH Classpath-exception-2.0 OR GPL-2.0-only WITH OpenJDK-assembly-exception-1.0
###############################################################################

# Testarossa build variables. Longer term the distinction between TR and the rest
# of the OMR code should be heavily reduced. In the mean time, we keep
# the distinction
set(TR_HOST_ARCH z)
list(APPEND TR_COMPILE_DEFINITIONS TR_HOST_S390 TR_TARGET_S390)

if(OMR_ENV_DATA64)
	list(APPEND TR_COMPILE_DEFINITIONS TR_HOST_64BIT TR_TARGET_64BIT BITVECTOR_64BIT)
	set(TR_HOST_BITS 64)
else()
	list(APPEND TR_COMPILE_DEFINITIONS TR_HOST_32BIT TR_TARGET_32BIT)
	set(TR_HOST_BITS 32)
endif()
set(CMAKE_ASM-ATT_FLAGS "-noexecstack -march=z9-109")

if(NOT OMR_OS_ZOS)
	# TODO: this seems wrong. Maybe we mean -qlonglong ?
	list(APPEND OMR_PLATFORM_DEFINITIONS -D_LONG_LONG)
endif()
