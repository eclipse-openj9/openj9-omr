<!--
Copyright IBM Corp. and others 2016

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
[2] https://openjdk.org/legal/assembly-exception.html

SPDX-License-Identifier: EPL-2.0 OR Apache-2.0 OR GPL-2.0-only WITH Classpath-exception-2.0 OR GPL-2.0-only WITH OpenJDK-assembly-exception-1.0
-->

# OMR Compiler Options

The OMR Compiler Technology provides a number of options that allow you to take
some control over its run-time behavior. This document contains the following:
* [How compiler options can be set](#setting-options)
* [Option Sets](#option-sets)
* [List of some options](#options-list)

# Setting options

OMR Compiler options are either set through command line options or environment
variables. If the OMR Compiler is being used as part of a language VM then the
options can only be set through the VM command line arguments using the `-Xjit`
argument followed by the options, or through environment variables in some cases,
such as `TR_dumpGraphs`.

Compiler options may be set using the `TR_Options` environment variable, for example:

```
TR_Options='traceFull,log=log' ./testjit
```

Language environments may also accept options on their command-line, but the
implementation is environment-specific. For example, the OpenJ9 Java VM accepts
JIT command-line options as:

```sh
java -Xjit:count=0,traceFull,log=log HelloWorld
```


# Option Sets

It is possible to apply a set of compiler options ("option sets") to a specific
method or group of methods. Furthermore, it is possible to apply different
option sets to different (groups of) methods. An option set can be defined on
the command line by enclosing the desired set of parameters in parentheses
and preceding it with a method filter expression in braces. For example, to
apply the option set `count=0,optlevel=hot` only to the method that matches the
signature `HelloWorld.main([Ljava/lang/String;)V` add the following to the
`TR_Options` environment variable or `-Xjit` command-line:

```sh
{HelloWorld.main([Ljava/lang/String;)V}(count=0,optlevel=hot)
```

The option example below makes the compiler compile all methods at the warm
optimization level, and traces the compilation of all method signatures that
start with `Frobnicate` and `Brobnicate`  in two different log files.

```sh
optlevel=warm,count=0,{Frobnicate*}(log=frob.log,traceFull),{Brobnicate*}(log=brob.log,traceFull)
```

Option sets can also be defined in a limit file, if one is used, by adding a
non-zero integer immediately after the initial plus sign on the line with the
selected method(s). The integer corresponds to the option set number which
can be set on the command line in lieu of a method filter expression.

For example, to apply `count=0,optLevel=hot` to method
`HelloWorld.main([Ljava/lang/String;)V`, you could assign the number 1 to the option
set in the command line:

```sh
1(count=0,optLevel=hot)
```

and have the line corresponding to the method in the limit file beginning with `+1`:

```sh
+1 (hot) HelloWorld.main([Ljava/lang/String;)V @ 0x10C11DA4-0x10C11DDD
```

Note that some parameters are always applied globally and are illegal within
option sets. The compiler will fail to start up if an option set contains such
parameters.

# Options list

This section contains a subset of options available for controlling the runtime
behavior of the compiler. This section does not exhaustively list all the options
or their categories. To see all of the available options to control the compiler,
please see the options listed in
[`<omr-root-dir>/compiler/control/OMROptions.cpp`](https://github.com/eclipse/omr/blob/e2f65411e67d21ef04e2062a8945e604d82cb19e/compiler/control/OMROptions.cpp#L98)
and 
[`<omr-root-dir>/compiler/env/FEBase.cpp`](https://github.com/eclipse/omr/blob/e2f65411e67d21ef04e2062a8945e604d82cb19e/compiler/env/FEBase.cpp#L235)

The purpose of listing the options in this document is to give you an insight
into the level of control you have over the compiler through the options rather
than acting as a reference. The options are categorized in subsections to help you
understand the general pattern (eg. optimizer-specific options, trace-specific
options, etc).

### Code Generation Options

| Option                                     | Description                                     |
| ------------------------------------------ | ----------------------------------------------- |
| bcLimit=<em>nnn</em>                       | bytecode size limit                             |
| code=<em>nnn</em>                          | code cache size, in KB                          |
| codetotal=<em>nnn</em>                     | total code memory limit, in KB                  |
| noExceptions                               | fail compilation for methods with exceptions    |
| noregmap                                   | generate GC maps without register maps          |

### Optimization Options

| Option                                           | Description                                                                     |
| ------------------------------------------------ | ------------------------------------------------------------------------------- |
| acceptHugeMethods                                | allow processing of really large methods                                        |
| count=<em>nnn</em>                               | number of invocations before compiling methods without loops                    |
| disableCFGSimplification                         | disable Control Flow Graph simplification                                       |
| disableDeadTreeElimination                       | disable dead tree elimination                                                   |
| disableGlobalDSE                                 | disable global dead store elimination                                           |
| disableGLU                                       | disable general loop unroller                                                   |
| disableGRA                                       | disable IL based global register allocator                                      |
| disableInlining                                  | disable IL inlining                                                             |
| disableLiveRegisterAnalysis                      | disable live register analysis                                                  |
| disableOpts={<em>regex</em>}                     | list of optimizations to disable                                                |
| disableOptTransformations={<em>regex</em>}       | list of optimizer transformations to disable                                    |
| disableTreeCleansing                             | disable tree cleansing                                                          |
| disableVirtualInlining                           | disable inlining of virtual methods                                             |
| dontInline={<em>regex</em>}                      | list of methods to not inline                                                   |
| firstOptIndex=<em>nnn</em>                       | index of the first optimization to perform                                      |
| firstOptTransformationIndex=<em>nnn</em>         | index of the first optimization transformation to perform                       |
| ignoreIEEE                                       | allow non-IEEE compliant optimizations                                          |
| insertDebuggingCounters=<em>nnn</em>             | Insert instrumentation for debugging counters                                   |
| lastOptIndex=<em>nnn</em>                        | index of the last optimization to perform                                       |
| lastOptTransformationIndex=<em>nnn</em>          | index of the last optimization transformation to perform                        |
| numRestrictedGPRs=<em>nnn</em>                   | number of restricted GPRS (0-5).  Currently z only                              |
| onlyInline={<em>regex</em>}                      | list of methods that can be inlined                                             |
| optLevel=<em>level</em>                          | compile all methods at specified level (cold, warm, hot, veryHot, scorching)    |
| paranoidOptCheck                                 | check the trees and cfgs after every optimization phase                         |

### Logging and Trace Options
| Option                                                                        | Description                                                                                            |
| ----------------------------------------------------------------------------- | ------------------------------------------------------------------------------------------------------ |
| debugCounters=                                                                | Activate dynamic Debug counters                                                                        |
| firstVlogLine=<em>nnn</em>                                                    | first vlog line to be written                                                                          |
| lastVlogLine=<em>nnn</em>                                                     | last vlog line to be written                                                                           |
| log=<em>filename</em>                                                         | write log output to <em>filename</em>                                                                  |
| optDetails                                                                    | log all optimizer transformations                                                                      |
| stats                                                                         | dump statistics at end of run                                                                          |
| traceBC                                                                       | dump bytecodes                                                                                         |
| traceBin                                                                      | dump binary instructions                                                                               |
| traceBlockSplitter                                                            | trace block splitter                                                                                   |
| traceCG                                                                       | dump output of code generation passes                                                                  |
| traceFull                                                                     | turn on all trace options                                                                              |
| traceGlobalDSE                                                                | trace global dead store elimination                                                                    |
| traceInlining                                                                 | trace IL inlining                                                                                      |
| traceOpts={<em>regex</em>}                                                    | list of optimizations to trace                                                                         |
| traceOptTrees                                                                 | dump trees after each optimization                                                                     |
| tracePRE                                                                      | trace partial redudndancy elimination                                                                  |
| traceRedundantMonitorElimination                                              | trace redundant monitor elimination                                                                    |
| traceRegisterPressureDetails                                                  | include extra register pressure annotations in register pressure simulation and tree evaluation traces |
| traceTrees                                                                    | dump trees after each compilation phase                                                                |
| verbose                                                                       | write compiled method names to vlog file or stdout in limitfile format                                 |
| verbose={<em>regex</em>}                                                      | list of verbose output to write to vlog or stdout                                                      |
| version                                                                       | display the jit build version                                                                          |
| vlog=<em>filename</em>                                                        | write verbose output to <em>filename</em>                                                              |

### Debugging Options

| Option                                     | Description                                                                                                                               |
| ------------------------------------------ | ----------------------------------------------------------------------------------------------------------------------------------------- |
| breakAfterCompile                          | raise trap when method compilation ends                                                                                                   |
| breakBeforeCompile                         | raise trap when method compilation begins                                                                                                 |
| breakOnBBStart                             | raise trap on BBStarts of method                                                                                                          |
| breakOnCreate={<em>regex</em>}             | raise trap when creating an item whose name (given by <em>enumerate</em>) matches <em>regex</em>                                          |
| breakOnEntry                               | insert entry breakpoint instruction in generated code                                                                                     |
| breakOnLoad                                | break after the options have been processed                                                                                               |
| breakOnOpts={<em>regex</em>}               | raise trap when performing opts with matching <em>regex</em>                                                                              |
| debugBeforeCompile                         | invoke the debugger when method compilation begins                                                                                        |
| debugOnEntry                               | invoke the debugger at the entry of a method                                                                                              |
| exclude=<em>xxx</em>                       | do not compile methods beginning with <em>xxx</em>                                                                                        |
| limit=<em>xxx</em>                         | only compile methods beginning with <em>xxx</em>                                                                                          |
| maskAddresses                              | remove addresses from trace file                                                                                                          |
| noRecompile                                | do not recompile even when counts allow it                                                                                                |
| stopOnFailure                              | stop compilation if exceed memory threshold                                                                                               |
| tlhPrefetch                                | enable software prefetch on allocation for X86                                                                                            |
| tossCode                                   | throw code and data away after compiling                                                                                                  |
