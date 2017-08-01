###############################################################################
#
# (c) Copyright IBM Corp. 2017
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

add_library(omr_example_gc_glue INTERFACE)
target_sources(omr_example_gc_glue INTERFACE
	${CMAKE_CURRENT_SOURCE_DIR}/CollectorLanguageInterfaceImpl.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/CompactSchemeFixupObject.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/EnvironmentDelegate.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/FrequentObjectsStats.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/GlobalCollectorDelegate.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/MarkingDelegate.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/ObjectIterator.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/ObjectModelDelegate.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/Profiling.c
	${CMAKE_CURRENT_SOURCE_DIR}/StartupManagerImpl.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/VerboseManagerImpl.cpp
)

target_include_directories(omr_example_gc_glue INTERFACE
	${CMAKE_CURRENT_SOURCE_DIR}
	${CMAKE_CURRENT_SOURCE_DIR}/..
)

add_library(omr_example_util_glue INTERFACE)
target_sources(omr_example_util_glue INTERFACE
	${CMAKE_CURRENT_SOURCE_DIR}/UtilGlue.c
)

target_include_directories(omr_example_util_glue INTERFACE
	${CMAKE_CURRENT_SOURCE_DIR}
	${CMAKE_CURRENT_SOURCE_DIR}/..
)

add_library(omr_example_core_glue INTERFACE)
target_sources(omr_example_core_glue INTERFACE
	${CMAKE_CURRENT_SOURCE_DIR}/LanguageVMGlue.c
	${CMAKE_CURRENT_SOURCE_DIR}/omrExampleVM.cpp
)

target_include_directories(omr_example_core_glue INTERFACE
	${CMAKE_CURRENT_SOURCE_DIR}
	${CMAKE_CURRENT_SOURCE_DIR}/..
)

add_library(omr_example_ras_glue INTERFACE)
target_sources(omr_example_ras_glue INTERFACE
	${CMAKE_CURRENT_SOURCE_DIR}/Profiling.c
)

target_include_directories(omr_example_ras_glue INTERFACE
	${CMAKE_CURRENT_SOURCE_DIR}
	${CMAKE_CURRENT_SOURCE_DIR}/..
)
