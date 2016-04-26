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
### Build DDR artifacts
###
### Must be invoked from the current directory.
###

top_srcdir = .
include $(top_srcdir)/omrmakefiles/configure.mk

# Ensure the source tree path is canonically-formed and ends with /
abs_srcroot = $(abspath $(TOP_SRCDIR))/

all: check-vars
	./tools/ddrgen/src/macros/getMacros.sh $(abs_srcroot)
	$(exe_output_dir)/ddrgen --filelist $(DBG_FILE_LIST) --macrolist $(abs_srcroot)macroList 

check-vars:
ifndef TOP_SRCDIR
	$(error TOP_SRCDIR is undefined)
endif
ifndef DBG_FILE_LIST
	$(error DBG_FILE_LIST is undefined)
endif

help:
	@echo "The following variables must be set:"
	@echo "TOP_SRCDIR    Path of the root of the source tree to be scanned."
	@echo "DBG_FILE_LIST Text file containing a list of files that contain"
	@echo "              debug info (e.g. DWARF info) to be scanned."
	@echo ""
	@echo "All paths must be relative to the directory containing this makefile."

.PHONY: all help check-vars
