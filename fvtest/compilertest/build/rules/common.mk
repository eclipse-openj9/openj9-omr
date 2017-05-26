################################################################################
##
## (c) Copyright IBM Corp. 2016, 2017
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


# Add our targets to the global targets
all: jit
clean: jit_cleanobjs jit_cleandeps jit_cleandll
cleanobjs: jit_cleanobjs
cleandeps: jit_cleandeps
cleandll: jit_cleandll

#
# Define our targets. "jit_cleanobjs" "jit_cleandeps" and "jit_cleandll" are double-colon so they can be appended to
# throughout the makefile.
#
.PHONY: jit jit_createdirs jit_cleanobjs jit_cleandeps jit_cleandll
jit:
jit_createdirs::
jit_cleanobjs::
jit_cleandeps::
jit_cleandll::

include $(JIT_MAKE_DIR)/rules/$(TOOLCHAIN)/common.mk
