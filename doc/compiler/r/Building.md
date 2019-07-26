# Building Compiler on RISC-V

## Native build

*Note:* Following steps have been tested on Debian system creates using
[debian-riscv][1]. The image built using these scripts should contain everything
needed to build OMR on RISC-V. It should be possible to build OMR on RISC-V
using Fedora images but this has not been tried.

*Note:* You will need `riscv.h` and `riscv-opc.h` headers from GNU binutils. You
can get them using (the image created using [debian-riscv][1] contains them):

    sudo wget "-O/usr/local/include/riscv.h" 'https://sourceware.org/git/gitweb.cgi?p=binutils-gdb.git;a=blob_plain;f=include/opcode/riscv.h;hb=HEAD'
    sudo wget "-O/usr/local/include/riscv-opc.h" 'https://sourceware.org/git/gitweb.cgi?p=binutils-gdb.git;a=blob_plain;f=include/opcode/riscv-opc.h;hb=HEAD'

### Building

```
git clone https://github.com/shingarov/omr -b riscv
cd omr
mkdir build
cd build
cmake .. \
      -Wdev -DCMAKE_BUILD_TYPE=Debug -DCMAKE_EXPORT_COMPILE_COMMANDS=ON \
      -DOMR_COMPILER=ON -DOMR_TEST_COMPILER=ON \
      -DOMR_JITBUILDER=ON -DOMR_JITBUILDER_TEST=ON
make -j4
```

Notes:

 * You *must* return `cmake ...` when you add/remove/rename files (generally, when you change any `CMakeLists.txt`).
 * `-DCMAKE_BUILD_TYPE=Debug` is necessary to produce debug symbols
 * `-DCMAKE_EXPORT_COMPILE_COMMANDS=ON` is to produce a `compile_commands.json` used by Clang-based tools. Should not harm.

### Compiler tests

To see what tests are in minimal testsuite run (assume you're in `build` directory)

    cd fvtest/compilertest
    ./compilertest --gtest_list_tests --gtest_filter=MinimalTest*

 To run specific test, say `MeaningOfLife`:

    ./compilertest --gtest_filter=MinimalTest.MeaningOfLife

 To run all tests in minimal test suite:

    ./compilertest --gtest_filter=MinimalTest.*

### TRIL tests

To run selected TRIL tests:

    cd fvtest/compilertriltest
    ./comptest --gtest_filter=*Arithmetic*/Int32*:LogicalTest/Int32*Unary*

## Cross compiling

Not yet tested.

[1]: https://github.com/janvrany/riscv-debian
