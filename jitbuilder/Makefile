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
	$(MAKE) -f jitbuilder.mk $(SUBMAKE_EXTRAARGS)

jitbuilder_clean:
	$(MAKE) -f jitbuilder.mk clean $(SUBMAKE_EXTRAARGS)

jitbuilder_cleandeps:
	$(MAKE) -f jitbuilder.mk cleandeps $(SUBMAKE_EXTRAARGS)

jitbuilder_cleandll:
	$(MAKE) -f jitbuilder.mk cleandll $(SUBMAKE_EXTRAARGS)

