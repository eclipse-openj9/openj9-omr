<!--
Copyright (c) 2016, 2018 IBM Corp. and others

This program and the accompanying materials are made available under
the terms of the Eclipse Public License 2.0 which accompanies this
distribution and is available at https://www.eclipse.org/legal/epl-2.0/
or the Apache License, Version 2.0 which accompanies this distribution and
is available at https://www.apache.org/licenses/LICENSE-2.0.

This Source Code may also be made available under the following
Secondary Licenses when the conditions for such availability set
forth in the Eclipse Public License, v. 2.0 are satisfied: GNU
General Public License, version 2 with the GNU Classpath
Exception [1] and GNU General Public License, version 2 with the
OpenJDK Assembly Exception [2].

[1] https://www.gnu.org/software/classpath/license.html
[2] http://openjdk.java.net/legal/assembly-exception.html

SPDX-License-Identifier: EPL-2.0 OR Apache-2.0 OR GPL-2.0 WITH Classpath-exception-2.0 OR LicenseRef-GPL-2.0 WITH Assembly-exception
-->

#  Introduction

The Testarossa compiler technology is shipped as part of a number of compiler
products, such as IBM J9. It consumes a description of a program (in bytecode,
or through the direct generation of intermediate language) into native code,
thereby improving the performance of the application. During the compilation,
the Testarossa based compilers performs analyses and optimizes the program.

As with any complex software system, occasionally the compiler might
malfunction. Typical symptoms include:

  * incorrect results or exceptions are produced by a correct but unusual,
    or complex program;
  * the VM crashes, resulting in a core file and/or crash report.

This document discusses troubleshooting procedures as well as some of the tools
available to you with which you could try to determine whether the compiler is 
faulty, as well as identifying the faulty component.

#  Invocation

The most common use case of Testarossa is in JIT compilation where it is
typically invoked by the VM automatically; the run-time behaviour of the JIT
can only be controlled via options passed through VM command line or, in some
cases, environment variables.

Generally speaking, the JIT can operate in two different modes:

  1. recompilation
  2. fixed optimization level

In many VMs, by default, the JIT is enabled in recompilation mode, i.e. more
frequently executed methods are compiled more often, with increasingly
aggressive optimizations. This mode tends to maximize the performance gain from
JIT compilation while minimizing its cost.

Alternatively, the JIT can operate at a fixed optimization level, which forces
it to compile all methods (or only certain methods selected by the user) with
the same aggresiveness. As you will see in this document, this mode is
extremely useful for diagnosing JIT problems.


# Isolating a Failure

### Completely Disabling the Compiler

The first step in diagnosing a failure is to determine whether the problem is
in fact related to the compiler. When Testarossa is being used as a JIT compiler,
there is almost certainly an interpreter to fall back to, which also means
a bug could be in the interpreter, not the compiler.

Running the program with the compiler disabled leads to one of two outcomes:

  * The failure remains. The problem is, therefore, not related to the compiler. In
    some cases, the program might start failing in a different manner;
    nevertheless, the problem is not in the compiler. You should consult other
    documentation on VM problem determination,
  * The failure disappears. The problem is most likely, although _not
    definitely_, in the compiler.

### Reducing Optimization Levels

If the failure of your program appears to come from a problem within the
compiler, you can try to narrow down the problem further by reducing the amount
of optimizations performed by the compiler.

The compiler optimizes methods at various optimization levels; that is, different
selections of optimizations are applied to different methods, based on their
call counts. Methods that are called more frequently are optimized at higher
levels. By changing compiler parameters, you can control the optimization level at
which methods are optimized, and determine whether the optimizer is at fault
and, if it is, which optimization is problematic.

If it is supported in your VM, the first compiler parameter to try is `count=0`,
which sets the compilation  threshold to zero and effectively causes the
program to be run in purely compiled mode, and tends to simplify the remainder
of the problem determination process. If the failure disappears when `count=0`
is used, try other count values such as 1 and 10.

In systems with concurrency, Changing the compiler threshold could affect the order in
which compilations and VM state changes occur. If the failure does not
occur when any compiler threshold is set, omit the `count=` parameter.

Another useful compiler parameter is `disableInlining`. With this parameter set, the
compiler is prohibited from generating larger and more complex code that is the side
effect of inlining, in an attempt to perform aggressive optimizations. If the
failure disappears when `disableInlining` is used, omit the parameter.

Now try decreasing the compiler optimization levels. The various optimization
levels are, depending on the implementation, typically

  1. `scorching`
  2. `veryHot`
  3. `hot`
  4. `warm`
  5. `cold`
  6. `noOpt`

from the most aggressive to the least expensive.

Run the program with the `optlevel=` parameter:


    count=0,disableInlining,optLevel=scorching


Try each of the optimization levels in turn, and record your observations,
until the `noOpt` level is reached, or until the failure cannot be reproduced.
The last optimization level that still triggers the failure is the level that
contains a potentially faulty optimization. If the program still fails at
`noOpt`, then the problem is most likely in the code generation phase (versus
the optimization phase) of the compiler.

### Locating the Failing Method

When you have arrived at the lowest optimization level at which the compiler must
compile methods to trigger the failure, you can try to find out which part of
the program, when compiled, causes the failure. You can then instruct the
compiler, when running as a JIT, to limit the workaround to a specific method,
class, or package, allowing the compiler to compile the rest of the program as it
normally would. If the failure occurs with `optLevel=noOpt`, you can also
instruct the compiler to not compile the method or methods that are causing the
failure (thus avoiding it).

To locate the method that is causing the failure, follow these steps:

1. Run the program with the compiler options `verbose` and
   `vlog=filename`. With these options, the JIT reports its progress, as it
   compiles methods, in a verbose log file, also called a **limit file**. A
   typical limit file contains lines that correspond to compiled methods, like:


        + (hot) java/lang/Math.max(II)I @ 0x10C11DA4-0x10C11DDD <... snipped ...>


   Lines that do not start with the plus sign are ignored by the JIT in the
   steps below, so you can edit them out of the file.

2. Run the program again with the compiler option `limitFile=(filename,m,n)`,
   where `filename` is the path to the limit file, and `m` and `n` are line
   numbers indicating respectively the first and the last methods in the limit
   file that should be compiled. This parameter causes the JIT to compile only the
   methods listed on lines `m` to `n` in the limit file. Methods not listed in
   the limit file and methods listed on lines outside the range are not compiled.
   Repeat this step with a smaller range if the program still fails.

   The recommended number of lines to select from the limit file in each
   repetition is half of the previous selection, such that this step consists of
   an efficient binary search for the failing method.

3. If the program no longer fails, one or more of the methods that you have
   removed in the last iteration must have been the cause of the failure. Invert
   the selection (select methods not selected in the previous iteration), and
   repeat the previous step to see if the program starts to the fail again.

4. Repeat the last two steps, as many times as necessary, to find the minimum
   number of methods that must be compiled to trigger the failure. Often, you can
   reduce the file to a single line.

When you have located the failing method, you can restrict compiler options to
the failing method, so that the remainder of the application can run normally.
For example, if the method `test.rb:61:pass` causes the program to
fail when compiled with `optLevel=hot`, you can run the program with:



    {test.rb:61:pass}(optLevel=warm,count=0)


which tells the compiler to compile only the troublesome method at the `warm`
optimization level, but recompile all other methods normally. Note, in most shells
the limit syntax uses special characters, and needs to be quoted.

If a method fails when it is compiled at `noOpt`, you can exclude it from
compilation altogether, using the `exclude=method` parameter:

    exclude={test.rb:61:pass}

### Identifying the failing Optimization.

Most transformations that can be elided are guarded in the source code by a special
check called `performTransformation` that can be controlled by the option
`lastOptIndex=N`. This tells the compiler to perform `N` optional transformations,
then proceed to only do required operations. In this sense, `lastOptIndex=-1` is an
even stronger property than `optLevel=noOpt`.

By binary searching `lastOptIndex=` you can find the optimization that failed.


### Identifying Compilation Failures

If the VM crashes, and you can see that the crash has occurred in the compiler
module, it means that either the compiler has failed during an attempt to compile a
method, or one of the JIT run-time routines, which are also contained in the
JIT module, has failed. Usually the VM prints this information on standard
error just before it terminates; the information is also recorded in the corefile
if one is produced.

To see if the compiler is crashing in the middle of a compilation, use the
`verbose` option with the following additional settings:

    verbose={compileStart|compileEnd}


These verbose settings report when the compiler starts to compile a method, and
when it ends. If the compiler fails on a particular method (that is, it starts
compiling, but crashes before it can end), use the `exclude=` parameter to
prevent the compiler from compiling the method.

# Limit

A limit is a mechanism for selecting methods to compile (additive limit), or to exclude
from compilation (subtractive limit). Limits can be specified in a limit file
or in the command line link.

### Limit files

A limit file (also known as a verbose log) contains the signatures of all 
methods in a program that have been JIT-compiled. The limit file produced 
by one run of the program can be edited to select individual methods, and 
subsequently read by the JIT compiler to compile only those selected 
methods. This allows a developer investigating a defect to concentrate on 
only methods that, once compiled, will trigger a given failure. 

Limit files can be generated using the compiler options
`verbose,vlog=limit-file`. the `verbose` option instructs the compiler to
enable verbose log, and the `vlog=limit-file` writes the verbose log to the
specified file. To learn how to single out methods that cause compilation failures,
follow the steps in the [section about locating failing method](#locating-the-failing-method) 
that outlines how you can perform a manual binary search using the limit file.

### Command line limits

An alternative to using limit files is to use the `-Xjit:limit={*method*}` option 
on the command line. This is suitable if you know exactly what method(s) you 
want to limit, and want a quick test run without having to create a text file.
`method` can be specified in three ways:

* The simplest way to use the option is to specify the (full or partial) name 
of the method. For example, `-Xjit:limit={*main*}` will instruct the JIT compiler 
to compile any method whose name contains the word "main".
* You can also spell out the entire signature of the method, e.g. 
`-Xjit:limit={*java/lang/Class.initialize()V*}.` Only the method with a 
signature that matches exactly will be compiled.
* Finally, you can use a regular expression to specify a group of methods. For 
instance, `-Xjit:limit={SimpleLooper.*}` specifies all methods that are members 
of the class SimpleLooper. The syntax of the regular expression used for 
limiting is the same as discussed above. You can combine this with the binary 
search approach to isolate a failure quickly without editing limit files, by 
running the program repeatedly using increasingly restrictive regular 
expressions such as `{[a-m]*}`, `{[a-f]*}`, and so on. Note that the brace 
character is treated specially by some UNIX shells, in which case you will 
need to escape the character with a backslash, or by surrounding the regular 
expression in quotation marks.

### Limit expressions

You can use regular expressions with limit directives to control compilation 
for methods based on whether their names match the patterns you specify. To 
use this feature, instead of a method signature, put a regular expression, 
enclosed in braces, in the limit file. Note that these "regular expressions" 
are not the same as those used by Perl, or by grep, or even by the Sovereign 
JIT compiler.

The following table gives some examples. 

| **Limit expression** | **Meaning** |
| ---------------------- | ------------------------------------------------------ |
| `- {*(I)*}` | Skip all methods with a single integer parameter |
| `+ {*.[^a]*}` | Compile all methods whose names do not begin with "a" |
| `+ {java/lang/[A-M]*}` | Compile all methods of all classes in the `java.lang` 
package whose names start with the letters `A` through `M` |
| `+ {*)[[]*}` | Compile All methods that return any kind of array |

### Limit file and its command line filter

You can use filters to work with a limit file to further specify compilation options for
a method. The table below lists the four kinds of available filters.

| **Limit filter** | **Meaning** |
| ----------------------- | ------------------------------------------------------- |
| `{regular expression}` | Methods with its signature matching `regular expression`. |
| `1-0 set index` | Methods specified with one digit number in the limit file 
right after the + or - sign |
| `[m,n]` | Methods between line `m` and `n` |
| `[n]` | The method in line `n` in the limit file |

An example of using command line filters with a limit file:

```sh
-Xjit:optlevel=noOpt,limitFile=limit.log,[5-10](optlevel=hot) 
```

The example above would instruct the compiler to compile methods between line 5 and 10 inclusive
at `hot` optimization level, and the rest of the methods in the limit file at minimal
optimization (level `noOpt`).

# Compilation Log

The compilation log, or simply log, is a file that records the results of program
analyses and code transformations performed by the compiler during one or more
compilations. The information to output in the log file can controlled through the
use of [compiler options](CompilerOptions.md).

### Generating the log

To create a log file in the JVM, use the command line argument 
`-Xjit:log=filename,traceFull`. This will record trace messages and listings 
for all compiled methods in the specified file. For example:

```sh
java -Xjit:log=compile.log,traceFull HelloWorld
```

The `traceFull` option would enable a number of important trace options. You can be
more specific about the information you want in the log file by using the [list 
of logging and tracing options](CompilerOptions.md#logging-and-trace-parameters).

### Filtering methods

You may also specify what methods or group of methods you would like to restrict the
logging to by using [limits](#limit).

An example of using a limit file to specify methods to trace:
```sh
java -Xjit:limitFile=methods.txt,log=compile.log,traceFull HelloWorld
```

Alternatively, you may also specify the method(s) directly in the command line:
```sh
java -Xjit:limit={*hello*},log=compile.log,traceFull HelloWorld
```

The options used in the command line argument above will result in the JIT-ing of
methods that contain "hello" in its name.

Whether you use a limit file or command line limits to specify methods to log,
it will result in all other methods not being compiled. If you would like to trace the
compilation of specific methods without affecting the compilation of the other methods,
you may do so by using [option sets](##option-sets).

### Deciphering the generated log

Each full method listing (i.e. when `-Xjit:traceFull` is used) consists of multiple sections.

* **Bytecode listing:** A low-level disassembly of the Java bytecode that constitute the
Java method.
* **Initial Trees:** A listing of the trees immediately after IL generation,
which should match the bytecode fairly closely. A trees listing is accompanied by the
control flow graph and the structure graph (once structural analysis had been
performed) for the method.
* **Per-optimization diagnostic information:** This section starts with the 
number and name of the optimization that was performed, e.g. "Performing 0: inlining", 
"Performing 1: compactNullChecks", etc., followed by trace messages produced as the 
optimization proceeded, e.g. "(Doing structural analysis)", "(Building use/def info)", 
"Removing redundant resolve check node [001E6B24]", etc., and finally includes a 
complete listing of the IL after any transformations.
* **Post Optimization Trees, Pre Instruction Selection Trees:** Both are listings of the
IL after all optimizations have been performed, the second differing only in that some
trees are "lowered", i.e. translated into equivalent trees that are better for a
particular architecture. The two listings should be largely similar.
* **Post Instruction Selection Instructions:** A listing of pseudo-machine instructions
that have been selected for the trees. Instruction sequences are annotated with the
individual trees for which they were selected. Registers and memory references, etc.,
remain symbolic in this listing.
* **Post Scheduling Instructions, Post Register Assignment Instructions:** Both are 
listings of the pseudo-machine instructions representing the jitted method, after the
procedures for which they are named have been performed. In the second listing, the
names of assigned physical registers as well as memory references are displayed in
native notation.
* **Post Binary Instructions:** A listing of the same pseudo-machine instructions after
binary encoding has been performed, annotated with the actual code bytes encoded for the
instructions, and the absolute addresses of the instructions in the code cache. This
listing also includes additional instructions like the prologue, the epilogue, and
out-of-line snippet code. In other words, it includes all instructions that consistute 
the jitted method. This listing should look fairly close to a disassembly listing that
one can obtain from a debugger.
**Method Meta-data:** A dump of meta-data about the method, such as GC atlas, exception
ranges and handlers, etc.
**Mixed Mode Disassembly:** The same listing as Post Binary Instructions, but annotated
with Java bytecode call stack information. This makes it easy to see what native
instructions were generated for a particular Java bytecode instruction.

**Trees Listing**  

The IL used in the Testarossa JIT compiler consists of a doubly linked list of trees of
nodes. Nodes are identified by their memory addresses, and their functions are described 
by their opcodes. Every node is either associated with a data type that indicates the 
type of expression represented by the node, or marked as "NoType", i.e. evaluating the 
node's opcode returns no value. Conceptually, each node can have multiple children, and 
either zero or one parent node; parent-less nodes (or root nodes) become children of 
"treetops", which are used to organize individual trees into a list. In reality, 
commoned nodes have more than one parent, and every node has a reference count that is 
used to keep track of how many parents point to itself.

The following snippet contains part of the listing of generated trees for method
`SimpleLooper.main([Ljava/lang/String;)V`:

```
Call Stack Info
CalleeIndex CallerIndex ByteCodeIndex CalleeMethod
       0         -1          b        SimpleLooper.<init>V
       1          0          1        java/lang/Object.<init>V
       2         -1         15        SimpleLooper.createArray(I)V
       3         -1         19        SimpleLooper.walkArray()V
       4         -1         2b        SimpleLooper.counter()I

    ------------ ByteCodeIndex
    |   ------------- CallSiteIndex
    |   |   ------------- Reference Count
    |   |   |     -------------- Visit Count
    |   |   |     |     -------------- Global Index
    |   |   |     |     |     ------------ Side Table Index
    |   |   |     |     |     |   ------------- Use/def Index
    |   |   |     |     |     |   |  ------------- Number of Children
    |   |   |     |     |     |   |  |  ------------- Size
    |   |   |     |     |     |   |  |  |      ------------- Data Type
    |   |   |     |     |     |   |  |  |      |           ------------- Node Address
    |   |   |     |     |     |   |  |  |      |           |      ------------- Instruction
    |   |   |     |     |     |   |  |  |      |           |      |
    V   V   V     V     V     V   V  V  V      V           V      V
[   0, -1,  0, 3234,    5,   -1, -1, 0, 0,    NoType [001E61F0]   BBStart (block 1)
.
.
.
[  1c,  3,  0, 3234,  232,   -1, -1, 1, 0,    NoType [001F267C]   treetop 
[  13,  3,  2, 3234,  217,    2, -1, 1, 4,       Int [001F2440]     iiload #127[001EF32C]  Shadow[SimpleLooper.count I]+12
[  16,  3,  2, 3234,  218,    2, 24, 0, 4,   Address [001F2464]       aload #106[001E624C] Auto[<auto slot 1>] 
[  1f,  3,  0, 3234,  237,   -1, -1, 1, 4,   Address [001F28E4]   ResolveCHK #14[001E6580] Method[throwNullPointerException] 
[  1f,  3,  2, 3234,  235,    2, -1, 2, 4,       Int [001F289C]     iicall #140[001F2884] unresolved Method[countB] 
[  1f,  3,  1, 3234,  236,    1, -1, 1, 4,   Address [001F28C0]       iaload #99[001E6A08] Shadow[<vft-symbol>] 
                                                                        ==>iaload at [001F21D8]
                                                                      ==>iaload at [001F21D8]
[  22,  3,  0, 3234,  436,   -1, -1, 1, 0,    NoType [02222958]   treetop 
[  22,  3,  2, 3234,  238,    2, -1, 2, 4,       Int [001F2918]     iadd    
                                                                      ==>iiload at [001F2440]
                                                                      ==>iicall at [001F289C]
[  23,  3,  0, 3234,  239,   -1, -1, 2, 4,       Int [001F293C]   iistore #127[001EF32C] Shadow[SimpleLooper.count I]+12 
                                                                    ==>aload at [001F2464]
                                                                    ==>iadd at [001F2918]
.
.
.
```

The `Call Stack Info` table gives a list of methods that have been inlined into 
the one being compiled. The numbers in the `CalleeIndex` and the `CallerIndex` columns 
describe the relationship between the inlined methods; the caller index of an 
inlined method is the callee index of its caller. The method being compiled is 
always assigned the callee index -1. In this example, `main()` (index -1) calls the 
constructor of `SimpleLooper` (index 0), which in turn calls the constructor of 
`java/lang/Object` (index 1). `main()` then calls `createArray()`, `walkArray()` and 
`counter()` (indices 2, 3 and 4, respectively). These indices are used in the IL 
listing (in the `CallSiteIndex` column) to indicate which method generated a given 
IL node.

Nodes in a tree are indented according to their position in the tree. Note that 
treetops do not have an opcode, only nodes have opcodes. However, some root nodes 
(e.g. `[001F267C]` and `[02222958]` above) have the opcode `treetop`, which usually 
mean that the nodes are merely placeholders that perform no computation other than 
evaluate their children. You may view the full list of IL opcodes in
 `<omr-root-dir>/compiler/il/OMRILOpCodesEnum.hpp`.

A node of the form `==>iiload at [001F2440]` represents commoning, i.e. the value 
of the expression has already been computed. Searching backwards for the node 
address will reveal the actual first occurrence of the node. The `Reference Count` 
column gives the number of times a node will be reused in subsequent trees. We
first perform an indirect load (`[001F2440]`) of an integer from 
the instance field count of a certain `SimpleLooper` object (`[001F2464]`). Then we 
call the virtual method `countB()` which returns an integer (`[001F289C]`). The two 
integer expressions are used again in an integer addition (`[001F2918]`). The sum is 
finally stored (`[001F293C]`) back into the same instance field count of the same 
`SimpleLooper` object.

The Byte Code Index column indicates which bytecode an instruction originated 
from. For example, the `iadd` node above `[001F2918]` was actually part of the 
method `SimpleLooper.walkArray()V` (index 3), and was at bytecode offset `0x22` in 
that method. 
