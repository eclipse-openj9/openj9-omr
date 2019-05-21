###############################################################################
# Copyright (c) 2015, 2018 IBM Corp. and others
#
# This program and the accompanying materials are made available under
# the terms of the Eclipse Public License 2.0 which accompanies this
# distribution and is available at https://www.eclipse.org/legal/epl-2.0/
# or the Apache License, Version 2.0 which accompanies this distribution and
# is available at https://www.apache.org/licenses/LICENSE-2.0.
#
# This Source Code may also be made available under the following
# Secondary Licenses when the conditions for such availability set
# forth in the Eclipse Public License, v. 2.0 are satisfied: GNU
# General Public License, version 2 with the GNU Classpath
# Exception [1] and GNU General Public License, version 2 with the
# OpenJDK Assembly Exception [2].
#
# [1] https://www.gnu.org/software/classpath/license.html
# [2] http://openjdk.java.net/legal/assembly-exception.html
#
# SPDX-License-Identifier: EPL-2.0 OR Apache-2.0 OR GPL-2.0 WITH Classpath-exception-2.0 OR LicenseRef-GPL-2.0 WITH Assembly-exception
###############################################################################


TEMP_TARGET_DATASIZE:=64

ifeq (linux_ztpf_390-64, $(SPEC))
	CONFIGURE_ARGS += \
		--enable-OMRTHREAD_LIB_UNIX \
		--enable-OMR_ARCH_S390 \
		--enable-OMR_ENV_DATA64 \
		--enable-OMR_PORT_RUNTIME_INSTRUMENTATION \
		--enable-OMR_THR_FORK_SUPPORT \
		--enable-OMR_THR_THREE_TIER_LOCKING \
		--enable-OMR_THR_YIELD_ALG \
		--enable-OMR_GC_ARRAYLETS
endif

ifeq (linux_ztpf_390, $(SPEC))
	CONFIGURE_ARGS += \
		--enable-OMRTHREAD_LIB_UNIX \
		--enable-OMR_ARCH_S390 \
		--enable-OMR_PORT_RUNTIME_INSTRUMENTATION \
		--enable-OMR_THR_FORK_SUPPORT \
		--enable-OMR_THR_THREE_TIER_LOCKING \
		--enable-OMR_THR_YIELD_ALG \
		--enable-OMR_GC_ARRAYLETS
endif

CONFIGURE_ARGS += libprefix=lib exeext= solibext=.so arlibext=.a objext=.o

CONFIGURE_ARGS += --host=s390x-ibm-tpf
CONFIGURE_ARGS += --build=s390x-ibm-linux-gnu
CONFIGURE_ARGS += 'AS=as'
CONFIGURE_ARGS += 'CC=tpf-gcc'
CONFIGURE_ARGS += 'CXX=tpf-g++'
CONFIGURE_ARGS += 'CCLINKEXE=tpf-gcc'
CONFIGURE_ARGS += 'CCLINKSHARED=tpf-gcc'
CONFIGURE_ARGS += 'CXXLINKEXE=tpf-gcc'
CONFIGURE_ARGS += 'CXXLINKSHARED=tpf-gcc'
CONFIGURE_ARGS += 'AR=ar'
CONFIGURE_ARGS += 'OMR_HOST_OS=linux_ztpf'
CONFIGURE_ARGS += 'OMR_HOST_ARCH=s390'
CONFIGURE_ARGS += 'OMR_TARGET_DATASIZE=$(TEMP_TARGET_DATASIZE)'
CONFIGURE_ARGS += 'OMR_BUILD_DATASIZE=$(TEMP_TARGET_DATASIZE)'
CONFIGURE_ARGS += 'OMR_ENV_DATA64=1'
CONFIGURE_ARGS += 'OMR_TOOLCHAIN=gcc'
CONFIGURE_ARGS += 'OMR_BUILD_TOOLCHAIN=gcc'
CONFIGURE_ARGS += 'OMR_TOOLS_CC=gcc'
CONFIGURE_ARGS += 'OMR_TOOLS_CXX=g++'
