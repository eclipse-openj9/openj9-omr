.phony: all clean cleandeps cleandll
all:
clean:
cleandeps:
cleandll:

# This is the logic right now for locating Clang and LLVM-config
# There's probably a nicer way to do all of this... it's pretty bad

ifeq ($(PLATFORM),amd64-linux64-clang)
    # Luckily we can just use the default path for Clang :)
endif

ifeq ($(PLATFORM),ppc64-linux64-clang)
    export CC_PATH?=/tr/llvm_checker/ppc-64/sles11/bin/clang
    export CXX_PATH?=/tr/llvm_checker/ppc-64/sles11/bin/clang++
endif

ifeq ($(PLATFORM),s390-zos64-vacpp)
    export CC_PATH?=/usr/lpp/cbclib/xlc/bin/xlc
    export CXX_PATH?=/usr/lpp/cbclib/xlc/bin/xlC
    export A2E_INCLUDE_PATH?=/usr/lpp/cbclib/include
endif

ifeq ($(PLATFORM),arm-linux-gcc-cross)
    export CC_PATH?=arm-bcm2708hardfp-linux-gnueabi-gcc
    export CXX_PATH?=arm-bcm2708hardfp-linux-gnueabi-g++
    export AS_PATH?=arm-bcm2708hardfp-linux-gnueabi-as
    SUBMAKE_EXTRAARGS=PLATFORM=arm-linux-gcc
endif


SUBMAKE_EXTRAARGS+=ASSUMES=1

#
# Also convert between old TRSRC variable and new ones
#
TRSRC?=$(J9SRC)
JIT_SRCBASE?=$(TRSRC)
JIT_OBJBASE?=$(TRSRC)/../build
JIT_DLL_DIR?=$(TRSRC)/omr/jitbuilder/release

#
# Now we need to cleanup these variables on Windows
#
FIXED_SRCBASE=$(subst \,/,$(JIT_SRCBASE))
FIXED_OBJBASE=$(subst \,/,$(JIT_OBJBASE))
FIXED_DLL_DIR=$(subst \,/,$(JIT_DLL_DIR))

#
# Add JitTest to base targets and lint targets
#
all: jitbuilder
clean: jitbuilder_clean
cleandeps: jitbuilder_cleandeps
cleandll: jitbuilder_cleandll

#
# Add JitBuilder targets
#
.phony: jitbuilder jitbuilder_clean jitbuilder_cleandeps jitbuilder_cleandll

jitbuilder:
	$(MAKE) -f jitbuilder.mk JIT_SRCBASE=$(FIXED_SRCBASE) JIT_OBJBASE=$(FIXED_OBJBASE) JIT_DLL_DIR=$(FIXED_DLL_DIR) $(SUBMAKE_EXTRAARGS)

jitbuilder_clean:
	$(MAKE) -f jitbuilder.mk clean JIT_SRCBASE=$(FIXED_SRCBASE) JIT_OBJBASE=$(FIXED_OBJBASE) JIT_DLL_DIR=$(FIXED_DLL_DIR) $(SUBMAKE_EXTRAARGS)

jitbuilder_cleandeps:
	$(MAKE) -f jitbuilder.mk cleandeps JIT_SRCBASE=$(FIXED_SRCBASE) JIT_OBJBASE=$(FIXED_OBJBASE) JIT_DLL_DIR=$(FIXED_DLL_DIR) $(SUBMAKE_EXTRAARGS)

jitbuilder_cleandll:
	$(MAKE) -f jitbuilder.mk cleandll JIT_SRCBASE=$(FIXED_SRCBASE) JIT_OBJBASE=$(FIXED_OBJBASE) JIT_DLL_DIR=$(FIXED_DLL_DIR) $(SUBMAKE_EXTRAARGS)

