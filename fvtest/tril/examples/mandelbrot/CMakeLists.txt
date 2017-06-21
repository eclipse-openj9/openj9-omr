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

project(tril_mandelbrot LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)
set(CXX_EXTENSIONS OFF)

if(NOT DEFINED tril_DIR)
    set(tril_DIR ${CMAKE_CURRENT_LIST_DIR}/../../tril/build)
endif()

find_package(tril REQUIRED)

if(NOT tril_FOUND)
    message(FATAL "404: Tril not found.")
endif()

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -fdiagnostics-color=always -fno-rtti -fno-threadsafe-statics -Wno-deprecated -Wno-enum-compare -Wno-invalid-offsetof -Wno-write-strings -O3 -pthread -fomit-frame-pointer -fasynchronous-unwind-tables -Wreturn-type -fno-dollars-in-identifiers -m64 -fno-strict-aliasing")

add_executable(mandelbrot
   main.cpp
)

target_link_libraries(mandelbrot
   ${tril_LIBRAREIS}
   tril
)
