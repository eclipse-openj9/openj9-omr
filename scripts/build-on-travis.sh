#!/bin/bash
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

set -evx

if test "x$BUILD_WITH_CMAKE" = "xyes"; then
  mkdir build
  cd build
  cmake -Wdev -C../cmake/caches/Travis.cmake ..
  if test "x$RUN_BUILD" != "xno"; then
    cmake --build .
    if test "x$RUN_TESTS" != "xno"; then
      ctest -V
    fi
  fi
else
  make -f run_configure.mk OMRGLUE=./example/glue SPEC="$SPEC" PLATFORM="$PLATFORM"
  if test "x$RUN_BUILD" != "xno"; then
    # Normal build system
    make -j4
    if test "x$RUN_TESTS" != "xno"; then
      make test
    fi
  fi
  if test "x$RUN_LINT" = "xyes"; then
    llvm-config --version
    clang++ --version
    make lint
  fi
fi
