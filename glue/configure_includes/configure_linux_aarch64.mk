###############################################################################
# Copyright (c) 2018, 2018 IBM Corp. and others
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
###############################################################################

include $(CONFIG_INCL_DIR)/configure_common.mk

ifeq (linux_aarch64, $(SPEC))
	CONFIGURE_ARGS += \
    --host=aarch64-linux-gnu \
    --target=aarch64-linux-gnu \
    --build=x86_64-pc-linux-gnu \
    --enable-OMRTHREAD_LIB_UNIX \
    --enable-OMR_ARCH_AARCH64 \
    --enable-OMR_ENV_LITTLE_ENDIAN \
    --enable-OMR_GC_TLH_PREFETCH_FTA \
    --enable-OMR_PORT_CAN_RESERVE_SPECIFIC_ADDRESS \
    --enable-OMR_THR_FORK_SUPPORT \
    --enable-OMR_GC_ARRAYLETS \
    --enable-fvtest \
    --enable-OMR_ENV_DATA64
endif

CONFIGURE_ARGS += \
	libprefix=lib exeext= solibext=.so arlibext=.a objext=.o

#	Customize this include path for your build environment
CONFIGURE_ARGS += \
	'GLOBAL_INCLUDES=/tools/aarch64-linux-gnu/include' \
	'AS=aarch64-linux-gnu-as' \
	'CC=aarch64-linux-gnu-gcc' \
	'CXX=aarch64-linux-gnu-g++' \
#	'CPP=aarch64-linux-gnu-cpp -E -P' \
	'AR=aarch64-linux-gnu-ar' \
	'OBJCOPY=aarch64-linux-gnu-objcopy' \
	'CCLINKEXE=$$(CC)' \
	'CCLINKSHARED=$$(CC)' \
	'CXXLINKEXE=$$(CXX)' \
	'CXXLINKSHARED=$$(CC)' \
	'OMR_HOST_OS=linux' \
	'OMR_HOST_ARCH=aarch64' \
	'OMR_TARGET_DATASIZE=64' \
	'OMR_TOOLCHAIN=gcc'
