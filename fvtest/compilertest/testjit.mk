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


#
# "all" should be the first target to appear so it's the default
#
.PHONY: all clean cleanobjs cleandeps cleandll
all: ; @echo SUCCESS - All files are up-to-date
clean: ; @echo SUCCESS - All files are cleaned
cleanobjs: ; @echo SUCCESS - All objects are cleaned
cleandeps: ; @echo SUCCESS - All dependencies are cleaned
cleandll: ; @echo SUCCESS - All shared libraries are cleaned

#
# First setup some important paths
# Personally, I feel it's best to default to out-of-tree build but who knows, there may be
# differing opinions on that.
#
JIT_SRCBASE?=../..
JIT_OBJBASE?=../../objs/compilertest_$(BUILD_CONFIG)
JIT_DLL_DIR?=../..

#
# Windows users will likely use backslashes, but Make tends to not like that so much
#
FIXED_SRCBASE=$(subst \,/,$(JIT_SRCBASE))
FIXED_OBJBASE=$(subst \,/,$(JIT_OBJBASE))
FIXED_DLL_DIR=$(subst \,/,$(JIT_DLL_DIR))

# TODO - "debug" as default?
BUILD_CONFIG?=prod

#
# This is where we setup our component dirs
# Note these are all relative to JIT_SRCBASE and JIT_OBJBASE
# It just makes sense since source and build dirs may be in different places 
# in the filesystem :)
#
JIT_OMR_DIRTY_DIR?=compiler
JIT_PRODUCT_DIR?=fvtest/compilertest

#
# Dirs used internally by the makefiles
#
JIT_MAKE_DIR?=$(FIXED_SRCBASE)/fvtest/compilertest/build
JIT_SCRIPT_DIR?=$(FIXED_SRCBASE)/fvtest/compilertest/build/scripts

#
# First we set a bunch of tokens about the platform that the rest of the
# makefile will use as conditionals
#
include $(JIT_MAKE_DIR)/platform/common.mk

#
# Now we include the names of all the files that will go into building the JIT
# Will automatically include files needed from HOST and TARGET platform
#
include $(JIT_MAKE_DIR)/files/common.mk

#
# Now we configure all the tooling we will use to build the files
#
# There is quite a bit of shared stuff, but the overwhelming majority of this
# is toolchain-dependent.
#
# That makes sense - You can't expect XLC and GCC to take the same arguments
#
include $(JIT_MAKE_DIR)/toolcfg/common.mk

#
# Here's where everything has been setup and we lay down the actual targets and
# recipes that Make will use to build them
#
include $(JIT_MAKE_DIR)/rules/common.mk
