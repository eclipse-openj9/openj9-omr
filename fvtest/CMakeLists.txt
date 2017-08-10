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

include(OmrAssert)

omr_assert(TEST OMR_FVTEST)

add_subdirectory(util)

add_subdirectory(omrGtestGlue)
add_subdirectory(algotest)
add_subdirectory(gctest)
add_subdirectory(porttest)
# add_subdirectory(rastest)
add_subdirectory(sigtest)
add_subdirectory(threadextendedtest)
add_subdirectory(threadtest)
add_subdirectory(utiltest)
add_subdirectory(vmtest)

if (OMR_JITBUILDER)
	add_subdirectory(jitbuildertest)
endif()

if(OMR_TEST_COMPILER)
	add_subdirectory(compilertest)
endif()

if(OMR_JITBUILDER)
	add_subdirectory(compilertriltest)
	add_subdirectory(tril/test)
endif()
