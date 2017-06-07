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

cmake_minimum_required(VERSION 3.2 FATAL_ERROR)

project(tril LANGUAGES C CXX)

find_package(BISON)
find_package(FLEX)

set(OMR_PATH ${CMAKE_CURRENT_SOURCE_DIR}/../../../omr)
set(JITBUILDER_PATH ${OMR_PATH}/jitbuilder/release)

set(CMAKE_POSITION_INDEPENDENT_CODE True)

add_definitions(-g -fdiagnostics-color=always -std=c++0x -fno-rtti -fno-threadsafe-statics -Wno-deprecated -Wno-enum-compare -Wno-invalid-offsetof -Wno-write-strings -O3 -pthread -fomit-frame-pointer -fasynchronous-unwind-tables -Wreturn-type -fno-dollars-in-identifiers -m64 -fPIC -fno-strict-aliasing   -DBITVECTOR_BIT_NUMBERING_MSB -DUT_DIRECT_TRACE_REGISTRATION -DJITTEST -DJITBUILDER_SPECIFIC -DPROD_WITH_ASSUMES -DTR_HOST_X86 -DTR_HOST_64BIT -DBITVECTOR_64BIT -DLINUX -DTR_TARGET_X86 -DTR_TARGET_64BIT -DSUPPORTS_THREAD_LOCAL -D_LONG_LONG -DJ9HAMMER)

BISON_TARGET(tril_parser tril.y ${CMAKE_CURRENT_BINARY_DIR}/tril.parser.c)
FLEX_TARGET(tril_scanner tril.l ${CMAKE_CURRENT_BINARY_DIR}/tril.scanner.c
            COMPILE_FLAGS "--yylineno"
            DEFINES_FILE ${CMAKE_CURRENT_BINARY_DIR}/tril.scanner.h)
ADD_FLEX_BISON_DEPENDENCY(tril_scanner tril_parser)

add_executable(trilc
    ${BISON_tril_parser_OUTPUTS}
    ${FLEX_tril_scanner_OUTPUTS}
    ast.c
    ilgen.cpp
    main.cpp
)

target_include_directories(trilc PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_BINARY_DIR}
    ${JITBUILDER_PATH}/include
    ${JITBUILDER_PATH}/include/compiler
    ${OMR_PATH}/compiler/x/amd64
    ${OMR_PATH}/compiler/x
    ${OMR_PATH}/compiler
    ${OMR_PATH}
    ${OMR_PATH}/include_core
)
target_link_libraries(trilc PUBLIC
    ${FLEX_LIBRARIES}
    ${BISON_LIBRARIES}
    ${JITBUILDER_PATH}/libjitbuilder.a
    dl
)
