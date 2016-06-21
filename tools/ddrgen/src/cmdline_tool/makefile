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

ddrgen_topdir = ../..
include $(ddrgen_topdir)/config/ddrgencfg.mk

MODULE_NAME := ddrgen
ARTIFACT_TYPE := cxx_executable

OBJECTS = \
  main$(OBJEXT)

DDRGEN_STATIC_LIBS = \
  scanner \
  intermediate_representation \
  genBlob \
  macroTool

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
  $(ddrgen_srcdir)/blob_generation \
  $(ddrgen_srcdir)/intermediate_representation \
  $(ddrgen_srcdir)/macros \
  $(ddrgen_srcdir)/scanners \
  $(ddrgen_srcdir)

ifeq (dwo,$(DEBUG_FORMAT))
MODULE_INCLUDES += \
  $(ddrgen_srcdir)/scanners/dwarf \
  $(OMR_LIBDWARF_IPATH)
else
MODULE_INCLUDES += \
  $(ddrgen_srcdir)/scanners/pdb \
  $(ddrgen_srcdir)/scanners/pdb/DIA-SDK/include
endif

ifeq (linux,$(OMR_HOST_OS))
  MODULE_SHARED_LIBS += rt pthread
endif
ifeq (aix,$(OMR_HOST_OS))
  MODULE_SHARED_LIBS += iconv perfstat
endif
ifeq (win,$(OMR_HOST_OS))
  MODULE_SHARED_LIBS += ws2_32 shell32 Iphlpapi psapi pdh
endif

include $(top_srcdir)/omrmakefiles/rules.mk
