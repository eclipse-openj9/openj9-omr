JitBuilder

A library for building JIT compilers, and an advance release of the
Eclipse OMR JIT compiler component
(see [Eclipse OMR Project](https://github.com/eclipse/omr)).

The library can be found in libjitbuilder.a and can be linked
against statically using the API in include/Jit.hpp as well
as using "Builders" : MethodBuilders, IlBuilders, and
BytecodeBuilders.

Some simple examples can be found in the src/ directory, and the
top level directory contains a Makefile that shows you how to
build them.

Just type 'make' and you'll build a number of examples that
demonstrate how you can start building native code dynamically!

More details to come!

This library has been used as the basis of a JIT compiler for
the SOM++ Simple Object Machine runtime.
