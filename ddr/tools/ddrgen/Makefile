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

top_srcdir = ../../..
include $(top_srcdir)/omrmakefiles/configure.mk

MODULE_NAME := ddrgen
ARTIFACT_TYPE := cxx_executable

DDRGEN_STATIC_LIBS = \
  ddr-scanner \
  ddr-ir \
  ddr-blobgen \
  ddr-macros

DEPENDENCIES = $(call buildStaticLibFilename,$(DDRGEN_STATIC_LIBS))

MODULE_STATIC_LIBS = $(DDRGEN_STATIC_LIBS)
MODULE_STATIC_LIBS += omrstatic
ifeq (dwo,$(DEBUG_FORMAT))
MODULE_STATIC_LIBS += $(OMR_LIBDWARF_STATIC_LIBS)
MODULE_SHARED_LIBS += $(OMR_LIBDWARF_SHARED_LIBS)
endif


# TODO libpath hack because ddrgen libs don't go into the same directory as omr libs (yet)
MODULE_LIBPATH += $(top_srcdir)/lib

MODULE_INCLUDES += \
  $(top_srcdir)/ddr/include \
  $(top_srcdir)/include_core

ifeq (dwo,$(DEBUG_FORMAT))
MODULE_INCLUDES += \
  $(OMR_LIBDWARF_IPATH)
else
MODULE_INCLUDES += \
  $(ddrgen_srcdir)/scanners/pdb/DIA-SDK/include
endif

ifeq (linux,$(OMR_HOST_OS))
  MODULE_SHARED_LIBS += rt pthread
  ifeq (ppc,$(OMR_HOST_ARCH))
    MODULE_SHARED_LIBS += elf
  endif
  ifeq (s390,$(OMR_HOST_ARCH))
    MODULE_SHARED_LIBS += z
  endif
endif
ifeq (aix,$(OMR_HOST_OS))
  MODULE_SHARED_LIBS += iconv perfstat
endif
ifeq (win,$(OMR_HOST_OS))
  MODULE_SHARED_LIBS += ws2_32 shell32 Iphlpapi psapi pdh
endif
ifeq (osx,$(OMR_HOST_OS))
  MODULE_SHARED_LIBS += iconv
endif

ifeq (gcc,$(OMR_TOOLCHAIN))
  MODULE_CXXFLAGS += -frtti -D__STDC_LIMIT_MACROS -std=c++0x
endif

ifeq (msvc,$(OMR_TOOLCHAIN))
  MODULE_CXXFLAGS += /EHsc
endif

OBJECTS = \
  ddrgen$(OBJEXT)

include $(top_srcdir)/omrmakefiles/rules.mk
