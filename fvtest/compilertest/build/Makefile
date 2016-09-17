################################################################################
##
## (c) Copyright IBM Corp. 2000, 2016
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

# omr/fvtest/compilertest/build Makefile

ifeq (,$(TRSRC))
    $(error Must set TRSRC)
endif

LIBJIT_TARBALL=$(TRSRC)/test/libjit/libjit.tgz
JITBUILDER_TARBALL=$(TRSRC)/test/jitbuilder/jitbuilder.tgz

.PHONY: all localclean jitbuilder prod debug clean
all: debug jitbuilder
localclean: clean

prod: jitbuilder ; $(MAKE) -C .. BUILD_CONFIG=prod JIT_DLL_DIR=$(TRSRC)/test/tests CX_FLAGS_EXTRA+='$(USERCFLAGS)'
debug: jitbuilder ; $(MAKE) -C .. BUILD_CONFIG=debug JIT_DLL_DIR=$(TRSRC)/test/tests CX_FLAGS_EXTRA+='$(USERCFLAGS)'
clean: ; $(MAKE) -C .. clean JIT_DLL_DIR=$(TRSRC)/test/tests CX_FLAGS_EXTRA+='$(USERCFLAGS)'

jitbuilder: $(JITBUILDER_TARBALL) $(LIBJIT_TARBALL)

$(JITBUILDER_TARBALL):
	mkdir -p $(dir $@)
	touch $@

$(LIBJIT_TARBALL):
	mkdir -p $(dir $@)
	touch $@
