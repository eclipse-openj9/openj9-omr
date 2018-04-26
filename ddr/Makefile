###############################################################################
# Copyright (c) 2015, 2018 IBM Corp. and others
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

top_srcdir := ..
include $(top_srcdir)/omrmakefiles/configure.mk

.DEFAULT_GOAL := all

OMR_TOOLCHAIN ?= gcc

ifneq (,$(filter gcc xlc,$(OMR_TOOLCHAIN)))
  DEBUG_FORMAT = dwo
else ifeq (msvc,$(OMR_TOOLCHAIN))
  DEBUG_FORMAT = pdb
else
  $(error DEBUG_FORMAT is undefined)
endif

# Make DEBUG_FORMAT visible to src/cmdline_tool/makefile
export DEBUG_FORMAT

ifeq (win,$(OMR_HOST_OS))
  ifneq (,$(DIASDK_HOME))
    # DIASDK_HOME almost certainly has embedded spaces, so we can't just append
    # to MODULE_INCLUDES; instead, adjust the INCLUDE environment variable.
    export INCLUDE := $(if $(INCLUDE),$(INCLUDE);)$(DIASDK_HOME)/include
  endif
endif

targets = \
  lib \
  test \
  tools

all: $(targets)

$(targets):
	$(MAKE) -C $@ all
.PHONY: $(targets)

tools: lib
test: tools lib

targets_clean := $(addsuffix _clean,$(targets))
clean: $(targets_clean)
$(targets_clean)::
	$(MAKE) -C $(patsubst %_clean,%,$@) clean
.PHONY: $(targets_clean)
