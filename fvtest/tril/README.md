# Tril

Tril is a Domain Specific Language (DSL) for generating the Testarossa
Intermediate Language (IL). It's goal is to simplify testing Testarossa by
providing very fine control over the IL that is fed into the compiler.

## trilc

trilc is an experimental compiler that reads a Tril file, compiles the contained
code, and executes it.

Although it is still in an experimental stage, trilc is already capable of
representing fairly complex programs (see `mandelbrot.tril` example).

## Building and running

1) Make sure you have cmake, flex, and yacc installed on your machine
2) Clone the Eclipse OMR repo: `git clone https://github.com/eclipse/omr.git`
3) execute the following commands in a terminal:

```sh
cd omr
./configure SPEC=x86-64 --enable-OMR_JITBUILDER # set SPEC for your platfrom
cd jitbuilder
make -j4
cd ../fvtest/tril
mkdir build
cd build
cmake -GNinja ..
cmake --build .
TR_Options=traceIlGen,traceFull,log=trace.log ./trilc ../mandelbrot.tril
```

4) Enjoy the view!
