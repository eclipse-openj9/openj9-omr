###############################################################################
#
# (c) Copyright IBM Corp. 2015, 2016
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
ddrgen_topdir = ../../..
include $(ddrgen_topdir)/config/ddrgencfg.mk

MODULE_NAME := scanner
ARTIFACT_TYPE := archive

OBJECTS = \
  DwarfScanner$(OBJEXT) \
  DwarfFunctions$(OBJEXT)

ifeq (osx,$(OMR_HOST_OS))
  OBJECTS += DwarfParser$(OBJEXT)
endif

ifeq (aix,$(OMR_HOST_OS))
  OBJECTS += AixSymbolTableParser$(OBJEXT)
endif

MODULE_INCLUDES += \
  $(ddrgen_srcdir) \
  $(ddrgen_srcdir)/scanners \
  $(ddrgen_srcdir)/intermediate_representation \
  $(ddrgen_srcdir)/blob_generation
MODULE_INCLUDES += $(OMR_LIBDWARF_IPATH)

include $(top_srcdir)/omrmakefiles/rules.mk
