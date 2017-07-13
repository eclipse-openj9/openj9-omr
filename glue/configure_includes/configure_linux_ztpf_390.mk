###############################################################################
#
#  Copyright IBM Corp. 2015, 2017
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
#    James Johnston (IBM Corp.) - initial z/TPF Port Updates
###############################################################################

# Detect 64-bit vs. 31-bit
ifneq (,$(findstring -64,$(SPEC)))
	TEMP_TARGET_DATASIZE:=64
else
	TEMP_TARGET_DATASIZE:=31
endif

ifeq (linux_ztpf_390-64_cmprssptrs_codecov, $(SPEC))
  	CONFIGURE_ARGS += \
		--enable-OMR_RTTI\
		--enable-OMRTHREAD_LIB_UNIX \
		--enable-OMR_ARCH_S390 \
		--enable-OMR_ENV_DATA64 \
		--enable-OMR_GC_COMPRESSED_POINTERS \
		--enable-OMR_INTERP_COMPRESSED_OBJECT_HEADER \
		--enable-OMR_INTERP_SMALL_MONITOR_SLOT \
		--enable-OMR_PORT_RUNTIME_INSTRUMENTATION \
		--enable-OMR_THR_FORK_SUPPORT \
		--enable-OMR_THR_THREE_TIER_LOCKING \
		--enable-OMR_THR_YIELD_ALG \
		--enable-OMR_GC_ARRAYLETS
#    --enable-OMR_PORT_CAN_RESERVE_SPECIFIC_ADDRESS
endif

ifeq (linux_ztpf_390-64_cmprssptrs, $(SPEC))
	CONFIGURE_ARGS += \
		--enable-OMR_RTTI\
		--enable-OMRTHREAD_LIB_UNIX \
		--enable-OMR_ARCH_S390 \
		--enable-OMR_ENV_DATA64 \
		--enable-OMR_GC_COMPRESSED_POINTERS \
		--enable-OMR_INTERP_COMPRESSED_OBJECT_HEADER \
		--enable-OMR_INTERP_SMALL_MONITOR_SLOT \
		--enable-OMR_PORT_RUNTIME_INSTRUMENTATION \
		--enable-OMR_THR_FORK_SUPPORT \
		--enable-OMR_THR_THREE_TIER_LOCKING \
		--enable-OMR_THR_YIELD_ALG \
		--enable-OMR_GC_ARRAYLETS
#    --enable-OMR_PORT_CAN_RESERVE_SPECIFIC_ADDRESS
endif

ifeq (linux_ztpf_390-64_codecov, $(SPEC))
	CONFIGURE_ARGS += \
		--enable-OMR_RTTI\
		--enable-OMRTHREAD_LIB_UNIX \
		--enable-OMR_ARCH_S390 \
		--enable-OMR_ENV_DATA64 \
		--enable-OMR_PORT_RUNTIME_INSTRUMENTATION \
		--enable-OMR_THR_FORK_SUPPORT \
		--enable-OMR_THR_THREE_TIER_LOCKING \
		--enable-OMR_THR_YIELD_ALG \
		--enable-OMR_GC_ARRAYLETS
#    --enable-OMR_PORT_CAN_RESERVE_SPECIFIC_ADDRESS
endif

ifeq (linux_ztpf_390-64, $(SPEC))
	CONFIGURE_ARGS += \
		--enable-OMR_RTTI\
		--enable-OMRTHREAD_LIB_UNIX \
		--enable-OMR_ARCH_S390 \
		--enable-OMR_ENV_DATA64 \
		--enable-OMR_PORT_RUNTIME_INSTRUMENTATION \
		--enable-OMR_THR_FORK_SUPPORT \
		--enable-OMR_THR_THREE_TIER_LOCKING \
		--enable-OMR_THR_YIELD_ALG \
		--enable-OMR_GC_ARRAYLETS
#    --enable-OMR_PORT_CAN_RESERVE_SPECIFIC_ADDRESS
endif

ifeq (linux_ztpf_390, $(SPEC))
	CONFIGURE_ARGS += \
		--enable-OMR_RTTI\
		--enable-OMRTHREAD_LIB_UNIX \
		--enable-OMR_ARCH_S390 \
		--enable-OMR_PORT_RUNTIME_INSTRUMENTATION \
		--enable-OMR_THR_FORK_SUPPORT \
		--enable-OMR_THR_THREE_TIER_LOCKING \
		--enable-OMR_THR_YIELD_ALG \
		--enable-OMR_GC_ARRAYLETS
#    --enable-OMR_PORT_CAN_RESERVE_SPECIFIC_ADDRESS
endif

CONFIGURE_ARGS += libprefix=lib exeext= solibext=.so arlibext=.a objext=.o

CONFIGURE_ARGS += --host=s390x-ibm-tpf
CONFIGURE_ARGS += --build=s390x-ibm-linux-gnu
CONFIGURE_ARGS += 'AS=as'
CONFIGURE_ARGS += 'CC=tpf-gcc'
CONFIGURE_ARGS += 'CXX=tpf-g++'
#CONFIGURE_ARGS += 'CPP=cpp -E -P'
CONFIGURE_ARGS += 'CCLINKEXE=tpf-gcc'
CONFIGURE_ARGS += 'CCLINKSHARED=tpf-gcc'
CONFIGURE_ARGS += 'CXXLINKEXE=tpf-gcc'
CONFIGURE_ARGS += 'CXXLINKSHARED=tpf-gcc'
CONFIGURE_ARGS += 'AR=ar'

CONFIGURE_ARGS += 'CFLAGS=-D_TPF_SOURCE'

CONFIGURE_ARGS += 'OMR_HOST_OS=linux_ztpf'
CONFIGURE_ARGS += 'OMR_HOST_ARCH=s390x'
CONFIGURE_ARGS += 'OMR_TARGET_DATASIZE=$(TEMP_TARGET_DATASIZE)'
CONFIGURE_ARGS += 'OMR_TOOLCHAIN=gcc'
