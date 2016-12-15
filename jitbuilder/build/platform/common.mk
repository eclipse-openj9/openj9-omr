################################################################################
##
## (c) Copyright IBM Corp. 2016, 2016
##
##  This program and the accompanying materials are made available
##  under the terms of the Eclipse Public License v1.0 and
##  Apache License v2.0 which accompanies this distribution.
##
##      The Eclipse Public License is available at
##      http://www.eclipse.org/legal/epl-v10.html
##
##      The Apache License v2.0 is available at
##      http://www.opensource.org/licenses/apache2.0.php
##
## Contributors:
##    Multiple authors (IBM Corp.) - initial implementation and documentation
################################################################################

#
# Common file for setting platform tokens
# Mostly just guesses the platform and includes the relevant
# HOST and TARGET files
#

#
# Try and guess platform if user didn't give it
#
ifeq (,$(PLATFORM))
    # TODO: move guess-platform.sh into a common directory
    PLATFORM:=$(shell $(SHELL) ../fvtest/compilertest/build/scripts/guess-platform.sh)
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
