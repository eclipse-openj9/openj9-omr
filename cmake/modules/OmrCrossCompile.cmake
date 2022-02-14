###############################################################################
# Copyright (c) 2021, 2022 IBM Corp. and others
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

if(OMR_CROSS_COMPILE_)
	return()
endif()
set(OMR_CROSS_COMPILE_ 1)


if(CMAKE_CROSSCOMPILING)
	# CMake considers cygwin/mingw/msys to be different from Windows, however we don't.
	# For our purposes we consider it all Windows, and not a cross compile.
	if(OMR_OS_WINDOWS AND "${CMAKE_HOST_SYSTEM_NAME}" MATCHES "CYGWIN|MINGW|MSYS")
		set(OMR_CROSSCOMPILING OFF CACHE INTERNAL "")
	else()
		set(OMR_CROSSCOMPILING ON CACHE INTERNAL "")
	endif()
else()
	set(OMR_CROSSCOMPILING OFF CACHE INTERNAL "")
endif()

# If the user hasn't provided a way to launch executables, try to find one.
if(OMR_CROSSCOMPILING AND NOT (OMR_EXE_LAUNCHER OR CMAKE_CROSSCOMPILING_EMULATOR))
	# If we are targeting linux try looking
	if(OMR_OS_LINUX)
		# Determine the processor name used by qemu
		if(OMR_ARCH_AARCH64)
			set(qemu_system "aarch64")
		elseif(OMR_ARCH_ARM)
			set(qemu_system "arm")
		elseif(OMR_ARCH_POWER)
			if(OMR_ENV_DATA32)
				set(qemu_system "power")
			else()
				if(OMR_ENV_LITTLE_ENDIAN)
					set(qemu_system "ppc64le")
				else()
					set(qemu_system "ppc64")
				endif()
			endif()
		elseif(OMR_ARCH_RISCV)
			if(OMR_ENV_DATA32)
				set(qemu_system "riscv32")
			else()
				set(qemu_system "riscv64")
			endif()
		elseif(OMR_ARCH_S390)
			set(qemu_system "s390x")
		elseif(OMR_ARCH_X86)
			if(OMR_ENV_DATA32)
				set(qemu_system "i386")
			else()
				set(qemu_system "x86_64")
			endif()
		endif()

		find_program(OMR_QEMU_EXE NAMES "qemu-${qemu_system}" "qemu-${qemu_system}-static")

		if(OMR_QEMU_EXE)
			# If we have a sysroot, use that for the library search path (eg to find ld-linux.so)
			if(CMAKE_SYSROOT)
				list(APPEND OMR_QEMU_EXE "-L" "${CMAKE_SYSROOT}")
			endif()
			set(OMR_EXE_LAUNCHER "${OMR_QEMU_EXE}" CACHE STRING "")
		endif()
	endif()

	# If we couldn't find anything issue a warning
	if(NOT (OMR_EXE_LAUNCHER OR CMAKE_CROSSCOMPILING_EMULATOR))
		message(WARNING "Cross compilation detected, but no exe launcher found in OMR_EXE_LAUNCHER or CMAKE_CROSSCOMPILING_EMULATOR")
		message(WARNING "You may experience issues when running build tools (hookgen/tracegen/tracemerge).")
	endif()
endif()
