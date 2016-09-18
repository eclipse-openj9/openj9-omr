J9_VERSION?=29

OMR_DIR?=$(J9SRC)/omr
top_srcdir=$(OMR_DIR)
include $(OMR_DIR)/omrmakefiles/configure.mk

GTEST_PATH=$(OMR_GTEST_DIR)

PRODUCT_INCLUDES=\
    $(OMR_DIR)/include_core \
    $(FIXED_SRCBASE)/$(JIT_PRODUCT_DIR)/$(TARGET_ARCH)/$(TARGET_SUBARCH) \
    $(FIXED_SRCBASE)/$(JIT_PRODUCT_DIR)/$(TARGET_ARCH) \
    $(FIXED_SRCBASE)/$(JIT_PRODUCT_DIR) \
    $(FIXED_SRCBASE)/omr/fvtest \
    $(FIXED_SRCBASE)/$(JIT_OMR_DIRTY_DIR)/$(TARGET_ARCH)/$(TARGET_SUBARCH) \
    $(FIXED_SRCBASE)/$(JIT_OMR_DIRTY_DIR)/$(TARGET_ARCH) \
    $(FIXED_SRCBASE)/$(JIT_OMR_DIRTY_DIR) \
    $(FIXED_SRCBASE)/omr \
    $(FIXED_SRCBASE) \
    $(GTEST_PATH) \
    $(GTEST_PATH)/include \
    $(J9SRC)/oti \
    $(J9SRC)/include

PRODUCT_DEFINES+=\
    BITVECTOR_BIT_NUMBERING_MSB \
    UT_DIRECT_TRACE_REGISTRATION \
    JITTEST \
    TEST_PROJECT_SPECIFIC

ifdef ASSUMES
    PRODUCT_DEFINES+=PROD_WITH_ASSUMES
endif

PRODUCT_RELEASE?=tr.open.test

PRODUCT_NAME?=testjit

PRODUCT_LIBPATH=
PRODUCT_SLINK=

#
# Now we include the host and target tool config
# These don't really do much generally... They set a few defines but there really
# isn't a lot of stuff that's host/target dependent that isn't also dependent
# on what tools you're using
#
include $(JIT_MAKE_DIR)/toolcfg/host/$(HOST_ARCH).mk
include $(JIT_MAKE_DIR)/toolcfg/host/$(HOST_BITS).mk
include $(JIT_MAKE_DIR)/toolcfg/host/$(OS).mk
include $(JIT_MAKE_DIR)/toolcfg/target/$(TARGET_ARCH).mk
include $(JIT_MAKE_DIR)/toolcfg/target/$(TARGET_BITS).mk

#
# Now this is the big tool config file. This is where all the includes and defines
# get turned into flags, and where all the flags get setup for the different
# tools and file types
#
include $(JIT_MAKE_DIR)/toolcfg/$(TOOLCHAIN)/common.mk
