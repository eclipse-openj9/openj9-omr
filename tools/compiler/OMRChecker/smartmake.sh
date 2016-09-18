#! /usr/bin/sh
#
# This script builds OMRChecker using the default c++ compiler.
# It then runs some tests on the build. If one of these fails,
# a new build is attempted  with g++ explicitly set as the c++
# compiler. The tests are then run once again. If a failure is
# again detected, a non-zero value is returned.
#
# This script was created to help avoid problems when
# running OMRChecker as part of an automated build system.
#
# On certain platforms, when OMRChecker is compiled with an old
# version of clang (e.g. clang-3.4), it fails to catch errors.
# The current fix is to compile OMRChecker with gcc instead.
# This script can be used to avoid accidently running
# a malfunctioning build of OMRChecker whil.
#

echo "Attempting to build OMRChecker normally."

make cleanall
make OMRCHECKER_DIR=$PWD
python test.py --checker $PWD/OMRChecker.so

if [ $? -eq 0 ]; then
   exit 0
fi

echo "Build failed. Attempting to rebuild OMRChecker with g++."

make cleanall
make OMRCHECKER_DIR=$PWD CXX=g++
python test.py --checker $PWD/OMRChecker.so

exit $?
