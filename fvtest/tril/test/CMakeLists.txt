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

cmake_minimum_required(VERSION 3.2 FATAL_ERROR)

project(triltest LANGUAGES CXX)

# Setup testing
enable_testing()
find_package(GTest REQUIRED)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_INCLUDE_CURRENT_DIR TRUE)

add_executable(triltest
   ASTTest.cpp
   ParserTest.cpp
   MethodInfoTest.cpp
   IlGenTest.cpp
   CompileTest.cpp
)

add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/../tril/ ${CMAKE_CURRENT_BINARY_DIR}/tril)
target_link_libraries(triltest
   ${GTEST_BOTH_LIBRARIES}
   tril
)

add_test(
    NAME triltest
    COMMAND triltest --gtest_color=yes
)

