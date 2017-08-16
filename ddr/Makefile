###############################################################################
#
# (c) Copyright IBM Corp. 2015, 2017
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

