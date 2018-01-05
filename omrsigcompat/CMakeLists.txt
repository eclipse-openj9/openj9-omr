###############################################################################
# Copyright (c) 2017, 2017 IBM Corp. and others
#
# This program and the accompanying materials are made available under
# the terms of the Eclipse Public License 2.0 which accompanies this
# distribution and is available at https://www.eclipse.org/legal/epl-2.0/
# or the Apache License, Version 2.0 which accompanies this distribution and
# is available at https://www.apache.org/licenses/LICENSE-2.0.
#
# This Source Code may also be made available under the following
# Secondary Licenses when the conditions for such availability set
# forth in the Eclipse Public License, v. 2.0 are satisfied: GNU
# General Public License, version 2 with the GNU Classpath
# Exception [1] and GNU General Public License, version 2 with the
# OpenJDK Assembly Exception [2].
#
# [1] https://www.gnu.org/software/classpath/license.html
# [2] http://openjdk.java.net/legal/assembly-exception.html
#
# SPDX-License-Identifier: EPL-2.0 OR Apache-2.0 OR GPL-2.0 WITH Classpath-exception-2.0 OR LicenseRef-GPL-2.0 WITH Assembly-exception
###############################################################################

include(OmrMsvcRuntime)

add_library(omrsig SHARED
	omrsig.cpp
)

if(OMR_WARNINGS_AS_ERRORS)
	target_compile_options(omrsig PRIVATE ${OMR_WARNING_AS_ERROR_FLAG})
endif()

if(OMR_ENHANCED_WARNINGS)
	target_compile_options(omrsig PRIVATE ${OMR_ENHANCED_WARNING_FLAG})
endif()

#TODO need more comprehensive symbols
set(exportedSyms
	omrsig_primary_signal
	omrsig_handler
	signal
)

#for now only export symbols on windows
if(OMR_HOST_OS STREQUAL "win")

	file(WRITE "${CMAKE_CURRENT_BINARY_DIR}/omrsig.exportlist" "EXPORTS\n" )
	foreach(exportSymbol ${exportedSyms})
		file(APPEND "${CMAKE_CURRENT_BINARY_DIR}/omrsig.exportlist" "${exportSymbol}\n" )
	endforeach()

	set_target_properties(omrsig
		PROPERTIES LINK_FLAGS "-def:${CMAKE_CURRENT_BINARY_DIR}/omrsig.exportlist")

	target_compile_definitions(omrsig PRIVATE -DMSVC_RUNTIME_DLL="${OMR_MSVC_CRT}")
endif()

#Symbols exported from source makefile:
#EXPORT_FUNCTIONS_FILE := omrsig.exportlist
#$(EXPORT_FUNCTIONS_FILE):
	#@# all
	#@echo omrsig_primary_signal >>$@
	#@echo omrsig_handler >>$@
	#@echo signal >>$@
#ifeq (,$(findstring win,$(OMR_HOST_OS)))
	#@# !windows
	#@echo omrsig_primary_sigaction >>$@
	#@echo sigaction >>$@
	#@echo sigset >>$@
	#@echo sigignore >>$@
	#@echo bsd_signal >>$@
	#@echo sysv_signal >>$@
#endif
#ifneq (,$(findstring linux,$(OMR_HOST_OS)))
	#@# linux
	#@echo __sysv_signal >>$@
	#@echo ssignal >>$@
#endif
#ifneq (,$(findstring osx,$(OMR_HOST_OS)))
	#@# osx
	#@echo __sysv_signal >>$@
	#@echo ssignal >>$@
#endif
#ifneq (,$(findstring zos,$(OMR_HOST_OS)))
	#@# z/os
	#@echo __sigactionset >>$@
#endif
#ifeq (,$(findstring win,$(OMR_HOST_OS)))
#ifeq (,$(findstring zos,$(OMR_HOST_OS)))
	#@# !win && !zos
	#@echo sigvec >>$@
#endif
#endif

target_link_libraries(omrsig
	PRIVATE
		omrutil
)

target_compile_options(omrsig
	PRIVATE
		${OMR_PLATFORM_SHARED_COMPILE_OPTIONS}
)

if(OMR_HOST_OS MATCHES "linux|osx")
	target_link_libraries(omrsig PRIVATE dl)
endif()
if(OMR_HOST_OS STREQUAL "zos")
	target_link_libraries(omrsig PRIVATE j9a2e)
endif()
#TODO ensure we are linking in the same way as source makefile:
#MODULE_STATIC_LIBS += omrutil
#ifneq (,$(findstring linux,$(OMR_HOST_OS)))
	#MODULE_SHARED_LIBS += pthread
#endif
#ifneq (,$(findstring osx,$(OMR_HOST_OS)))
	#MODULE_SHARED_LIBS += pthread
#endif
install(TARGETS omrsig
	LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
	RUNTIME DESTINATION ${CMAKE_INSTALL_LIBDIR}
)
