#
# Common file for setting platform tokens
# Mostly just guesses the platform and includes the relevant
# HOST and TARGET files
#

#
# Try and guess platform if user didn't give it
#
ifeq (,$(PLATFORM))
    PLATFORM:=$(shell $(SHELL) $(JIT_SCRIPT_DIR)/guess-platform.sh)
    $(warning PLATFORM not set. Guessing '$(PLATFORM)')
endif

#
# If we still can't figure it out, bomb out with an error
#
ifeq (,$(PLATFORM))
    $(error PLATFORM not set and unable to guess)
endif

# Make sure platform file exists
$(if $(wildcard $(JIT_MAKE_DIR)/platform/host/$(PLATFORM).mk),,$(error Error: $(PLATFORM) not implemented))

#
# This is where we set variables based on the "host triplet"
# These variables will be tested all throughout the rest of the makefile
# to make decisions based on our host architecture, OS, and toolchain
#
include $(JIT_MAKE_DIR)/platform/host/$(PLATFORM).mk

#
# This will set up the target variables
# We will use these later to make decisions related to target arch
#
# Right now, we really don't have anything special like a
# "target spec", but this leaves it open in the future
#
include $(JIT_MAKE_DIR)/platform/target/all.mk