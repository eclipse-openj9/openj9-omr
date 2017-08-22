###############################################################################
#
# (c) Copyright IBM Corp. 2017, 2017
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

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(jitbuildertest
	main.cpp
	selftest.cpp
	UnionTest.cpp
	FieldAddressTest.cpp
	AnonymousTest.cpp
	ControlFlowTest.cpp
	SystemLinkageTest.cpp
	WorklistTest.cpp
)

target_link_libraries(jitbuildertest
	jitbuilder
	omrGtest
	${CMAKE_DL_LIBS}
)

add_test(NAME JitBuilderTest COMMAND jitbuildertest)
