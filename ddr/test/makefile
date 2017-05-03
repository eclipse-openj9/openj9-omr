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

top_srcdir = ../..
include $(top_srcdir)/omrmakefiles/configure.mk

MODULE_NAME := ddrgentest
ARTIFACT_TYPE := cxx_executable

OBJECTS += $(patsubst %.cpp,%$(OBJEXT),$(wildcard *.cpp))
OBJECTS += $(patsubst %.c,%$(OBJEXT),$(wildcard *.c))
OBJECTS += $(patsubst %.cpp,%$(OBJEXT),$(wildcard test_subdir/*.cpp))
OBJECTS += $(patsubst %.c,%$(OBJEXT),$(wildcard test_subdir/*.c))
OBJECTS += $(patsubst %.cpp,%$(OBJEXT),$(wildcard map_to_type/*.cpp))
OBJECTS += $(patsubst %.c,%$(OBJEXT),$(wildcard map_to_type/*.c))

ifeq (gcc,$(OMR_TOOLCHAIN))
  MODULE_CXXFLAGS += -g3 -Wno-error
else ifeq (msvc,$(OMR_TOOLCHAIN))
  MODULE_CXXFLAGS += /Zi
else
  MODULE_CXXFLAGS += -qdbgfmt=stabstring
endif

include $(top_srcdir)/omrmakefiles/rules.mk
