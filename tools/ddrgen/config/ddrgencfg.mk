###############################################################################
#
# (c) Copyright IBM Corp. 2016
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
###
### Makefile configuration for ddrgen
### ddrgen modules should include this makefile instead of configure.mk
###

# omr_srcdir = path to top of OMR source tree
omr_srcdir ?= $(ddrgen_topdir)/../..

# ddrgen_topdir = path to top of DDR source tree
ddrgen_srcdir = $(ddrgen_topdir)/src

# set top_srcdir as required by omr makefiles
top_srcdir = $(abspath $(omr_srcdir))
include $(top_srcdir)/omrmakefiles/configure.mk

ifeq (gcc,$(OMR_TOOLCHAIN))
# ddrgen uses rtti
MODULE_CXXFLAGS += -frtti -D__STDC_LIMIT_MACROS -std=c++0x
endif
ifeq (msvc,$(OMR_TOOLCHAIN))
MODULE_CXXFLAGS += /EHsc
endif
