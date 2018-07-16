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
typically invoked by the VM automatically.

The the run-time behaviour can be controlled using options. Please see the
CompilerOptions document to learn more about how options can be used.

Generally speaking, the JIT can operate in two different modes:

  1. recompilation
  2. fixed optimization level

In many VMs, by default, the JIT is enabled in recompilation mode, i.e. more
frequently executed methods are compiled more often, with increasingly
aggressive optimizations. This mode tends to maximize the performance gain from
JIT compilation while minimizing its cost.

Alternatively, the JIT can operate at a fixed optimization level, which forces
it to compile all methods (or only certain methods selected by the user) with
the same aggressiveness. As you will see in this document, this mode is
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
of optimizations performed by the compiler through the use of compiler options.
The option controlling optimization level is `optlevel`, and it is either used
through environment variable:

```sh
TR_Options=`optLevel=noOpt`
```

or through command line options when the compiler is being used as part of a VM:

```sh
-Xjit:optLevel=noOpt
```
For more on compiler option usage, see the [OMR Compiler Options](CompilerOptions.md)
document.

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

An alternative to using limit files is to use the `limit={*method*}` option 
in the command line. This is suitable if you know exactly what method(s) you 
want to limit, and want a quick test run without having to create a text file.
`method` can be specified in three ways:

* The simplest way to use the option is to specify the (full or partial) name 
of the method. For example, `limit={*main*}` will instruct the JIT compiler 
to compile any method whose name contains the word "main".
* You can also spell out the entire signature of the method, e.g. 
`limit={*java/lang/Class.initialize()V*}.` Only the method with a 
signature that matches exactly will be compiled.
* Finally, you can use a regular expression to specify a group of methods. For 
instance, `-limit={SimpleLooper.*}` specifies all methods that are members 
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
optlevel=noOpt,limitFile=limit.log,[5-10](optlevel=hot) 
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

To create a log file, use the options `log=filename,traceFull`. This will record trace messages
and listings for all compiled methods a file. For example:

```sh
log=compile.log,traceFull
```

The `traceFull` option would enable a number of important trace options. There are other options
available that would allow you to be more specific about the information you want in the log file, and the
table below lists a few commonly used ones:

| Option                                                                        | Description |
| ------------------- | --------------------------------------------------------------------- |
| optDetails | Log all optimizer transformations. |
| traceBC | Dump bytecodes at the beginning of the compilation. |
| traceBin | Dump binary instructions at the end of compilation. |
| traceCG | Trace code generation. |
| traceOptTrees | Dump trees after each optimization. |
| traceTrees | Dump trees after each compilation phase. |

To see the complete set of trace options, see the [logging options listed in OMROptions.cpp](https://github.com/eclipse/omr/blob/e2f65411e67d21ef04e2062a8945e604d82cb19e/compiler/control/OMROptions.cpp#L1118).

### Filtering methods

You may also specify what methods or group of methods you would like to restrict the
logging to by using [limits](#limit).

An example of using a limit file to specify methods to trace:
```sh
limitFile=methods.txt,log=compile.log,traceFull
```

Alternatively, you may also specify the method(s) directly in the command line:
```sh
limit={*hello*},log=compile.log,traceFull
```

The options used in the command line argument above will result in the JIT-ing of
methods that contain "hello" in its name.

Whether you use a limit file or command line limits to specify methods to log,
it will result in all other methods not being compiled. If you would like to trace the
compilation of specific methods without affecting the compilation of the other methods,
you may do so by using [option sets](CompilerOptions.md#option-sets).

## Deciphering the generated log

Each full compilation log (i.e. when `TR_Options='traceFull'` is used) consists
of multiple sections. It can be overwhelming at first,  and even more so when
you do not restrict logging to specific methods. This section aims to provide
an explanation on some of the important components of the compilation logs.

For this section, we will mostly use the compilation log generated from a simple
Java *hello world* program through the OpenJ9 JVM.

### Pre ILGenOpt Trees and Initial Trees

The IL used in the Testarossa JIT compiler consists of doubly linked list of trees
of nodes. Nodes can be identified by their global indices or their memory addresses,
and their functions are described by their opcodes. Conceptually, each node can have
multiple children, and either zero or one parent node; parent-less nodes
(or root nodes) become children of "treetops", which are used to organize individual
trees into a list. In reality, commoned nodes have more than one parent, and every
node has a reference count that is used to keep track of how many parents point to itself.

Pre-ILGenOpt Trees are the listing of trees immediately after ILGeneration, and
Initial Trees is the listing of trees after transformations due to ILGen
optimizations. The following snippet contains the Initial Trees
generated from method `HelloWorld.main(Ljava/lang/String;)V`.

```
<trees
	title="Initial Trees"
	method="testfield/HelloWorld.main([Ljava/lang/String;)V"
	hotness="warm">

Initial Trees: for testfield/HelloWorld.main([Ljava/lang/String;)V

------------------------------------------------------------------------------------------------------------------------------------------------------------------------
n1n       BBStart <block_2>                                                                   [0x7f8981868920] bci=[-1,0,6] rc=0 vc=6 vn=- li=- udi=- nc=0
n4n       ResolveCHK [#548]                                                                   [0x7f8981868a10] bci=[-1,0,6] rc=0 vc=6 vn=- li=- udi=- nc=1
n3n         aload  java/lang/System.out Ljava/io/PrintStream;[#611  unresolved notAccessed volatile Static] [flags 0x2307 0x0 ]  [0x7f89818689c0] bci=[-1,0,6] rc=3 vc=6 vn=- li=3 udi=- nc=0
n6n       ResolveCHK [#548]                                                                   [0x7f8981868ab0] bci=[-1,3,6] rc=0 vc=6 vn=- li=- udi=- nc=1
n5n         aload  <string>[#612  unresolved Static +35577232] [flags 0x80000307 0x0 ]        [0x7f8981868a60] bci=[-1,3,6] rc=2 vc=6 vn=- li=2 udi=- nc=0
n9n       ResolveAndNULLCHK on n3n [#30]                                                      [0x7f8981868ba0] bci=[-1,5,6] rc=0 vc=6 vn=- li=- udi=- nc=1
n8n         calli  java/io/PrintStream.println(Ljava/lang/String;)V[#613  unresolved virtual Method] [flags 0x400 0x0 ] ()  [0x7f8981868b50] bci=[-1,5,6] rc=1 vc=6 vn=- li=1 udi=- nc=3 flg=0x20
n7n           aloadi  <vft-symbol>[#543  Shadow] [flags 0x18607 0x0 ]                         [0x7f8981868b00] bci=[-1,5,6] rc=1 vc=6 vn=- li=1 udi=- nc=1
n3n             ==>aload
n3n           ==>aload
n5n           ==>aload
n10n      return                                                                              [0x7f8981868bf0] bci=[-1,8,7] rc=0 vc=6 vn=- li=- udi=- nc=0
n2n       BBEnd </block_2> =====                                                              [0x7f8981868970] bci=[-1,8,7] rc=0 vc=6 vn=- li=- udi=- nc=0

n13n      BBStart <block_3> (freq 0) (catches ...) (OSR handler) (cold)                       [0x7f8981868ce0] bci=[-1,0,6] rc=0 vc=6 vn=- li=- udi=- nc=0
n14n      BBEnd </block_3> (cold) =====                                                       [0x7f8981868d30] bci=[-1,0,6] rc=0 vc=6 vn=- li=- udi=- nc=0

n11n      BBStart <block_4> (freq 0) (cold)                                                   [0x7f8981868c40] bci=[-1,0,6] rc=0 vc=6 vn=- li=- udi=- nc=0
n21n      treetop                                                                             [0x7f8981868f60] bci=[-1,0,6] rc=0 vc=6 vn=- li=- udi=- nc=1
n20n        call  prepareForOSR[#53  helper Method] [flags 0x400 0x0 ] ()                     [0x7f8981868f10] bci=[-1,0,6] rc=1 vc=6 vn=- li=1 udi=- nc=5 flg=0x20
n15n          loadaddr  vmThread[#614  MethodMeta] [flags 0x200 0x0 ]                         [0x7f8981868d80] bci=[-1,0,6] rc=1 vc=6 vn=- li=1 udi=- nc=0
n16n          iconst -1                                                                       [0x7f8981868dd0] bci=[-1,0,6] rc=1 vc=6 vn=- li=1 udi=- nc=0
n17n          aload  args<parm 0 [Ljava/lang/String;>[#610  Parm] [flags 0x40000107 0x0 ]     [0x7f8981868e20] bci=[-1,0,6] rc=1 vc=6 vn=- li=1 udi=- nc=0
n18n          iconst 610                                                                      [0x7f8981868e70] bci=[-1,0,6] rc=1 vc=6 vn=- li=1 udi=- nc=0
n19n          iconst -1                                                                       [0x7f8981868ec0] bci=[-1,0,6] rc=1 vc=6 vn=- li=1 udi=- nc=0
n23n      igoto                                                                               [0x7f8981869000] bci=[-1,0,6] rc=0 vc=6 vn=- li=- udi=- nc=1
n22n        aload  osrReturnAddress[#573  MethodMeta +2288] [flags 0x10207 0x0 ]              [0x7f8981868fb0] bci=[-1,0,6] rc=1 vc=6 vn=- li=1 udi=- nc=0
n12n      BBEnd </block_4> (cold)                                                             [0x7f8981868c90] bci=[-1,0,6] rc=0 vc=6 vn=- li=- udi=- nc=0

index:       node global index
bci=[x,y,z]: byte-code-info [callee-index, bytecode-index, line-number]
rc:          reference count
vc:          visit count
vn:          value number
li:          local index
udi:         use/def index
nc:          number of children
addr:        address size in bytes
flg:         node flags

Number of nodes = 23, symRefCount = 615
</trees>
```

Near the beginning of the trees listing for some methods, you may see a
`Call Stack Info` table, which gives a list of methods that have been inlined
into the one being compiled.

Nodes in a tree are indented according to their position in the tree. Note that
treetops/root nodes (left-most indented nodes) do not have opcodes, only their
children have opcodes. The root nodes with the opcode `treetop` (eg, node `n21n`)
are merely placeholders that do nothing more than evaluate their children.

Nodes with their opcodes prepended with `==>`, such as `n3n` and `n5n` represents
commoning, i.e. the value of the expression has been already computed.

On the right hand side of the trees listing, there's more information about the
nodes such its address in memory, `bci` (indicating which bytecode the instruction
originated from), and `rc` (indicating how many parent nodes point to the node).

You may have noticed the symbol references in some of the nodes (eg. `#548` in
`n4n`). They refer to the entries in the symbol table. The symbol table can be
found at the end of the pre-ILGenOpt trees, which in this case was:

```
.
.
.
Symbol References (incremental):
--------------------------------
#30:   jitThrowNullPointerException[ helper Method] [flags 0x400 0x0 ] [0x7f89818b3370] (NoType)
#53:   prepareForOSR[ helper Method] [flags 0x400 0x0 ] [0x7f89818f3440] (NoType)
#543:  <vft-symbol>[ Shadow] [flags 0x18607 0x0 ] [0x7f89818b3740] (Address)
#548:  <resolve check>[ helper Method] [flags 0x400 0x0 ] [0x7f89818b3370] (NoType)
#573:  osrReturnAddress[ MethodMeta +2288] [flags 0x10207 0x0 ] [0x7f89818f3530] (Address)
#610:  args<parm 0 [Ljava/lang/String;>[ Parm] [flags 0x40000107 0x0 ] [0x7f8981869b00] (Address)
#611:  java/lang/System.out Ljava/io/PrintStream;[ unresolved notAccessed volatile Static] [flags 0x2307 0x0 ] [0x7f89818b32a0] (Address) [volatile]
#612:  <string>[ unresolved Static +35577232] [flags 0x80000307 0x0 ] [0x7f89818b34b0] (Address)
#613:  java/io/PrintStream.println(Ljava/lang/String;)V[ unresolved virtual Method] [flags 0x400 0x0 ] [0x7f89818b3660] (NoType)
#614:  vmThread[ MethodMeta] [flags 0x200 0x0 ] [0x7f89818f33c0] (NoType)

Number of nodes = 23, symRefCount = 615
</trees>
```

### Control Flow Graph

A control flow graph or CFG is printed right after each trees listing. A CFG
is a flowchart-like representation of the different paths of execution through
the instructions constituting a single function, method, or trace. Each vertex
in the graph is a basic block: a sequence of instructions with a single entry
point at the start, and a single exit point at the end. Each edge in the graph
joins a predecessor block to a successor; each block could have multiple 
predecessors and successors. A loop is represented as a cycle in the graph.
Below is the CFG printed after the Initial Trees listing for the
`HelloWorld.main(Ljava/lang/String;)V` method.

```
<cfg>
         0 [0x7f89818841f0] entry
                 in        = []
                 out       = [2(0) ]
                 exception in  = []
                 exception out = []
         1 [0x7f89818840f0] exit
                 in        = [4(0) 2(0) ]
                 out       = []
                 exception in  = []
                 exception out = []
         2 [0x7f89818b3170] BBStart at 0x7f8981868920
                 in        = [0(0) ]
                 out       = [1(0) ]
                 exception in  = []
                 exception out = [3(0) ]
         3 [0x7f89818f31c0] BBStart at 0x7f8981868ce0, frequency = 0
                 in        = []
                 out       = [4(0) ]
                 exception in  = [2(0) ]
                 exception out = []
         4 [0x7f89818f3080] BBStart at 0x7f8981868c40, frequency = 0
                 in        = [3(0) ]
                 out       = [1(0) ]
                 exception in  = []
                 exception out = []

</cfg>
```

The CFG is is printed in the order of increasing basic block numbers. Each of the
entries have the memory address of the basic block, its corresponding address in
the IL (eg. block at index 2 corresponds to node `n1n` in the trees listing in 
the last section), followed by 4 entries:
* **in**: blocks that fall through or branch to the current block
* **out**: blocks to which the current block falls through or branches 
* **exception-in**: blocks that throw exceptions for which the current block is the catcher
* **exception-out**: blocks that catch exceptions thrown by the current block

The CFG will also include the structure graph if structural analysis has
already been performed.

### Per-optimization diagnostic information
This section starts with the number and name of each of the optimizations that
were performed, e.g:

```
<optimize
	method="testfield/HelloWorld.main([Ljava/lang/String;)V"
	hotness="warm">
<optimization id=10 name=coldBlockOutlining method=testfield/HelloWorld.main([Ljava/lang/String;)V>
Performing 10: coldBlockOutlining
         No transformations done by this pass -- omitting listings
</optimization>
```

If an optimization does not result in transformation of the trees, then the
trees are not listed again, as is the case in the example above. Otherwise, the
transformed trees are printed.

### Post Optimization Trees and Pre Instruction Selection Trees
Both are listings of the IL after all optimizations have been performed, the
second differing only in that some trees are "lowered", i.e. translated into
equivalent trees that are better for a particular architecture. The two listings
should be largely similar.

### Post Instruction Selection Instructions:
A listing of pseudo-machine instructions that have been selected for the trees.
Instruction sequences are annotated with the individual trees for which they
were selected. Registers and memory references, etc., remain symbolic in this
listing.

### Post Scheduling Instructions and Post Register Assignment Instructions
Both are listings of the pseudo-machine instructions representing the jitted
method, after the procedures for which they are named have been performed.
In post register assignment instructions listing, the names of assigned physical
registers as well as memory references are displayed in native notation.

### Post Binary Instructions
A listing of the same pseudo-machine instructions after binary encoding has been
performed, annotated with the actual code bytes encoded for the instructions,
and the absolute addresses of the instructions in the code cache. This listing
also includes additional instructions like the prologue, the epilogue, and
out-of-line snippet code. In other words, it includes all instructions that
constitute the jitted method. This listing should look fairly close to a
disassembly listing that one can obtain from a debugger.

### Method Meta-data
A dump of meta-data about the method, such as GC atlas, exception ranges and
handlers, etc.

### Mixed Mode Disassembly
The same listing as Post Binary Instructions, but annotated with Java bytecode call
stack information. This makes it easy to see what native instructions were generated
for a particular Java bytecode instruction.