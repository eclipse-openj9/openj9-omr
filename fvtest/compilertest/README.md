Test Compiler
=============

This file contains the Eclipse OMR 'Test compiler', a framework for building
language independent compiler tests for the Testarossa compiler technology. 

Building
--------
Before building the Test compiler you must make sure to configure the project
by following the 'Basic configuration and compile' step as outlined in the 
top-level `omr/README.md` document.

To build the Test compiler, typing `make` in this directory
(`omr/fvtest/compilertest`) should suffice. A binary called 
`testjit` will be produced in the top-level `omr` directory.

BUILD_CONFIG defaults to prod. 

In the case where `guess-platform.sh` is incorrect, you can set the `PLATFORM`
environment variables to one of the supported ones, listed in
`compilertest/build/platforms/host.mk`. 


Running the Test Compiler
-------------------------

The Test compiler has been written using [Google Test][gtest], and as such, 
obeys the Google Test command line arguments, that can be seen by running 
`testjit --help`. 

A very useful subset include: 

* `--gtest_list_tests` Lists the tests
* `--gtest_filter=` Filters what runs based on a simple regex. 

Options can be passed to the Testarossa compiler technology using the
`TR_Options` environment variable.

    $ TR_Options='log=/tmp/testcomplog,tracefull' testjit



[gtest]: https://github.com/google/googletest/ 
