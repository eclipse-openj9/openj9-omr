# Introduction To JIT Compilation Logs

1. How To Generate A Log File
2. Bytecodes And Trees
3. Optimization: globalValuePropagation
4. Optimization: localCSE
5. Optimization: basicBlockExtension
6. Optimization: tacticalGlobalRegisterAllocator
7. Code Generation

## 1. How To Generate A Log File

To collect a compilation log file, specify the `-Xjit` option with `{methodSignature}(traceFull,log=logFileName)`.
The log file is suffixed with a process ID to avoid the previous log files being overwritten by the new ones, for
example `JITlog.16996.30451.20210824.114051.16996`. The log file name can also include a full path, and if omitted the
trace log will be generated in the present working directory.

```
-Xjit:{java/util/HashMap.putVal(ILjava/lang/Object;Ljava/lang/Object;ZZ)Ljava/lang/Object;}(traceFull,log=JITlog)
```

Wildcard expressions can be used to log multiple methods. For example, `{java/util/hashMap.*}` will log every single
method that is compiled in `java/util/hashMap`.

`traceFull` turns on all important trace types (i.e. `traceBC`, `traceTrees`, `traceCG`, `traceOptTrees`, and `optDetails`).
Tracing options are defined in [OMROptions.cpp](https://github.com/eclipse/omr/blob/1d8fb435675f022855948c08f08f6db66cbe38d8/compiler/control/OMROptions.cpp#L1136).

## 2. Bytecodes And Trees

```
<!--
MULTIPLE LOG FILES MAY EXIST
Please check for ADDITIONAL log files named:  /root/bumblebench/JITlog.1  /root/bumblebench/JITlog.2  /root/bumblebench/JITlog.3  /root/bumblebench/JITlog.4  /root/bumblebench/JITlog.5  /root/bumblebench/JITlog.6
-->
```

The above message at the beginning of a log file shows the possible logs that could be generated. The example names in
the log file are not suffixed with process IDs. A method could be compiled multiple times. There are multiple compilation
threads. Each compilation could be done by different compilation threads. There could be multiple log files generated
for one method. Each compilation thread writes to its own log file. The max number of logs files is the max number of
compilation threads.

```
-->
<compile
	method="java/util/HashMap.putVal(ILjava/lang/Object;Ljava/lang/Object;ZZ)Ljava/lang/Object;"
	hotness="cold"
	isProfilingCompile=0>
</compile>


=======>java/util/HashMap.putVal(ILjava/lang/Object;Ljava/lang/Object;ZZ)Ljava/lang/Object;

...

This method is cold
...
```

There could be multiple methods in one log file. Each compilation starts with `=======>`. `=======>` can be used as an
eye catcher to search other compilations in the same log file. Or `<compile` XML tag can be used as an eye catcher.
Another eye catcher is `“This method is”` that indicates the hotness level.

```
        +------------- Byte Code Index
        |  +-------------------- OpCode
        |  |                        +------------- First Field
        |  |                        |     +------------- Branch Target
        |  |                        |     |      +------- Const Pool Index
        |  |                        |     |      |    +------------- Constant
        |  |                        |     |      |    |
        V  V                        V     V      V    V

        0, JBaload0getfield
        1, JBgetfield                           38
        4, JBdup
        5, JBastore                 6
        7, JBifnull                12,   19,
       10, JBaload                  6
       12, JBarraylength
       13, JBdup
       14, JBistore                 8
       16, JBifne                  13,   29,
```

JIT takes bytecodes as an input and generates [IL trees](https://github.com/eclipse/omr/blob/dd1373f8fbe46759cc02c3c6386dbee9ce30ce5e/doc/compiler/il/IntroToTrees.md). The first section is the bytecodes that are received from VM.

- Bytecode `#1` `getfield` shows the constant pool index `38` of the field.
- Bytecode `#5` `astore` stores the address to the stack slot number `6`.
- Bytecode `#7` `ifnull` tests if the value is `NULL`. If it is, it branches to bytecode `#19`.


The IL generator generated the following IL which showed up in the section `"Pre IlGenOpt Trees"`.
The IL consists of a doubly linked list of trees of nodes ([TR::Nodes](https://github.com/eclipse/omr/blob/master/compiler/il/OMRNode.hpp)).

```
n284n     BBStart <block_32>                                                                  [0x7eff0a557880] bci=[-1,0,628] rc=0 vc=0 vn=- li=- udi=- nc=0
n288n     ificmpne --> block_2 BBStart at n29n ()                                             [0x7eff0a5579c0] bci=[-1,0,628] rc=0 vc=0 vn=- li=- udi=- nc=2 flg=0x20
n286n       iload  <count-for-recompile>[#320  Static] [flags 0x2000303 0x40 ]                [0x7eff0a557920] bci=[-1,0,628] rc=1 vc=0 vn=- li=- udi=- nc=0
n287n       iconst 1                                                                          [0x7eff0a557970] bci=[-1,0,628] rc=1 vc=0 vn=- li=- udi=- nc=0
n285n     BBEnd </block_32> =====                                                             [0x7eff0a5578d0] bci=[-1,0,628] rc=0 vc=0 vn=- li=- udi=- nc=0

n289n     BBStart <block_33> (freq 0) (cold)                                                  [0x7eff0a557a10] bci=[-1,0,628] rc=0 vc=0 vn=- li=- udi=- nc=0
...
```

 - `n284n`: nodeID, or [the GlobalIndex of an TR::Node](https://github.com/eclipse/omr/blob/e4c209ac149ef8db3cc9834a962b689a2e3c0fd7/compiler/il/OMRNode.hpp#L754)
that uniquely identifies a node. The node's global index can sometimes be used as an index into a bit vector or an array
to get some piece of information associated with that specific node. i.e. it serves a purpose that cannot be served using
just the node's address.
- `0x7eff0a557880`: The address of the C++ TR::Node object.
- `BBStart`, `ificmpne`, `BBEnd`, and other treeTops in `block_32` are linked in order.
- `n284n BBStart` has no children in this example.
 `n288n ificmpne` has two children. It compares `n286n` with `n287n`. If they are not equal, it branches to `block_2`,
otherwise it will fall through to `block_33`.
- We can find the [IL opcode properties](https://github.com/eclipse/omr/blob/1d0a329e26b096dacba6f31fef33236ed419428a/compiler/il/OMROpcodes.enum#L4502-L4517)
in [OMROpcodes.enum](https://github.com/eclipse/omr/blob/1d0a329e26b096dacba6f31fef33236ed419428a/compiler/il/OMROpcodes.enum)
to figure out the number of children an IL opcode has and the types of the children.

```
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

eg: bci=[-1,0,628] rc=0 vc=0 vn=- li=- udi=- nc=1
```
At the end of `"Pre IlGenOpt Trees"`, there is a description on the abbreviation format. `bci` shows which method the
node belongs to, what the bytecode it is, and which line number it is in the original program. For the method being compiled,
the callee index is always `-1`. For inlining methods, the callee-index is a non-negative number.

### 2.1 JIT Instrumented Trees At The Beginning Of A Method

```
n284n     BBStart <block_32>                                                                  [0x7eff0a557880] bci=[-1,0,628] rc=0 vc=0 vn=- li=- udi=- nc=0
n288n     ificmpne --> block_2 BBStart at n29n ()                                             [0x7eff0a5579c0] bci=[-1,0,628] rc=0 vc=0 vn=- li=- udi=- nc=2 flg=0x20
n286n       iload  <count-for-recompile>[#320  Static] [flags 0x2000303 0x40 ]                [0x7eff0a557920] bci=[-1,0,628] rc=1 vc=0 vn=- li=- udi=- nc=0
n287n       iconst 1                                                                          [0x7eff0a557970] bci=[-1,0,628] rc=1 vc=0 vn=- li=- udi=- nc=0
n285n     BBEnd </block_32> =====                                                             [0x7eff0a5578d0] bci=[-1,0,628] rc=0 vc=0 vn=- li=- udi=- nc=0

n289n     BBStart <block_33> (freq 0) (cold)                                                  [0x7eff0a557a10] bci=[-1,0,628] rc=0 vc=0 vn=- li=- udi=- nc=0
n294n     istore  <recompilation-counter>[#304  Static] [flags 0x1000303 0x40 ]               [0x7eff0a557ba0] bci=[-1,0,628] rc=0 vc=0 vn=- li=- udi=- nc=1
n293n       iadd                                                                              [0x7eff0a557b50] bci=[-1,0,628] rc=2 vc=0 vn=- li=- udi=- nc=2
n292n         iload  <recompilation-counter>[#304  Static] [flags 0x1000303 0x40 ]            [0x7eff0a557b00] bci=[-1,0,628] rc=1 vc=0 vn=- li=- udi=- nc=0
n291n         iconst -1                                                                       [0x7eff0a557ab0] bci=[-1,0,628] rc=1 vc=0 vn=- li=- udi=- nc=0
n296n     ificmpgt --> block_2 BBStart at n29n ()                                             [0x7eff0a557c40] bci=[-1,0,628] rc=0 vc=0 vn=- li=- udi=- nc=2 flg=0x20
n293n       ==>iadd
n295n       iconst 0                                                                          [0x7eff0a557bf0] bci=[-1,0,628] rc=1 vc=0 vn=- li=- udi=- nc=0
n290n     BBEnd </block_33> (cold) =====                                                      [0x7eff0a557a60] bci=[-1,0,628] rc=0 vc=0 vn=- li=- udi=- nc=0

n297n     BBStart <block_34> (freq 0) (cold)                                                  [0x7eff0a557c90] bci=[-1,0,628] rc=0 vc=0 vn=- li=- udi=- nc=0
n300n     istore  <recompilation-counter>[#304  Static] [flags 0x1000303 0x40 ]               [0x7eff0a557d80] bci=[-1,0,628] rc=0 vc=0 vn=- li=- udi=- nc=1
n299n       iconst 0x7fffffff                                                                 [0x7eff0a557d30] bci=[-1,0,628] rc=1 vc=0 vn=- li=- udi=- nc=0
n302n     bstore  <gcr-patch-point>[#321  Static] [flags 0x400301 0x40 ]                      [0x7eff0a557e20] bci=[-1,0,628] rc=0 vc=0 vn=- li=- udi=- nc=1
n301n       bconst   2                                                                        [0x7eff0a557dd0] bci=[-1,0,628] rc=1 vc=0 vn=- li=- udi=- nc=0
n307n     treetop                                                                             [0x7eff0a557fb0] bci=[-1,0,628] rc=0 vc=0 vn=- li=- udi=- nc=1
n303n       icall  jitRetranslateCallerWithPreparation[#71  helper Method] [flags 0x400 0x0 ] ()  [0x7eff0a557e70] bci=[-1,0,628] rc=1 vc=0 vn=- li=- udi=- nc=3 flg=0x20
n304n         iconst 0x80000                                                                  [0x7eff0a557ec0] bci=[-1,0,628] rc=1 vc=0 vn=- li=- udi=- nc=0
n305n         loadaddr  <start-PC>[#323  Static] [flags 0x4000303 0x40 ]                      [0x7eff0a557f10] bci=[-1,0,628] rc=1 vc=0 vn=- li=- udi=- nc=0
n306n         loadaddr  <J9Method>[#324  Static] [flags 0x8000307 0x40 ]                      [0x7eff0a557f60] bci=[-1,0,628] rc=1 vc=0 vn=- li=- udi=- nc=0
n298n     BBEnd </block_34> (cold) =====                                                      [0x7eff0a557ce0] bci=[-1,0,628] rc=0 vc=0 vn=- li=- udi=- nc=0

n29n      BBStart <block_2>                                                                   [0x7eff0a4c7740] bci=[-1,0,628] rc=0 vc=0 vn=- li=- udi=- nc=0
n34n      compressedRefs                                                                      [0x7eff0a4c78d0] bci=[-1,1,628] rc=0 vc=0 vn=- li=- udi=- nc=2
n32n        aloadi  java/util/HashMap.table [Ljava/util/HashMap$Node;[#372  Shadow +16] [flags 0x607 0x0 ]  [0x7eff0a4c7830] bci=[-1,1,628] rc=3 vc=0 vn=- li=- udi=- nc=1
n31n          aload  <'this' parm Ljava/util/HashMap;>[#366  Parm] [flags 0x40000107 0x0 ] (X!=0 sharedMemory )  [0x7eff0a4c77e0] bci=[-1,0,628] rc=1 vc=0 vn=- li=- udi=- nc=0 flg=0x4
n33n        lconst 0                                                                          [0x7eff0a4c7880] bci=[-1,1,628] rc=1 vc=0 vn=- li=- udi=- nc=0
n35n      astore  <auto slot 6>[#373  Auto] [flags 0x7 0x0 ]                                  [0x7eff0a4c7920] bci=[-1,5,628] rc=0 vc=0 vn=- li=- udi=- nc=1
n32n        ==>aloadi
n39n      ifacmpeq --> block_4 BBStart at n1n ()                                              [0x7eff0a4c7a60] bci=[-1,7,628] rc=0 vc=0 vn=- li=- udi=- nc=2 flg=0x20
n32n        ==>aloadi
n36n        aconst NULL                                                                       [0x7eff0a4c7970] bci=[-1,7,628] rc=1 vc=0 vn=- li=- udi=- nc=0
n30n      BBEnd </block_2> =====
```

The above example is at the beginning of `"Pre IlGenOpt Trees"` in a cold compilation. It shows the trees JIT
instrumented to facilitate the recompilation. Trees in `block_32`, `block_33`, and `block_34` are prefixed by the
Control infrastructure to the method to allow us to recompile the method once JVM startup is done. Because they are
fabricated, we put bytecode index `0` on these nodes.

The static `count-for-recompile` variable is created by JIT at cold for each compiled method body. It keeps track
of how many times we are executing the method. After the method is executed multiple times after the startup time
is done, we will decide whether or not to recompile it.

`block_32` checks if the `count-for-recompile` is not equal to `1`. If it is not equal to `1`, it branches to `block_2`.
`block 2` is where the normal bytecode for the method starts. Otherwise, if it equals to `1`, it decrements `recompilation-counter`
in `block_33`. If the `recompilation-counter` is greater than `0`, it goes to execute the normal bytecode in `block_2`.
If it is `0`, it goes to call the helper call `jitRetranslateCallerWithPreparation` to recompile the method.

### 2.2 Bytecode `getfield` And Its Trees

```
        +------------- Byte Code Index
        |  +-------------------- OpCode
        |  |                        +------------- First Field
        |  |                        |     +------------- Branch Target
        |  |                        |     |      +------- Const Pool Index
        |  |                        |     |      |    +------------- Constant
        |  |                        |     |      |    |
        V  V                        V     V      V    V

        1, JBgetfield                           38

...
...
n34n      compressedRefs                                                                      [0x7eff0a4c78d0] bci=[-1,1,628] rc=0 vc=0 vn=- li=- udi=- nc=2
n32n        aloadi  java/util/HashMap.table [Ljava/util/HashMap$Node;[#372  Shadow +16] [flags 0x607 0x0 ]  [0x7eff0a4c7830] bci=[-1,1,628] rc=3 vc=0 vn=- li=- udi=- nc=1
n31n          aload  <'this' parm Ljava/util/HashMap;>[#366  Parm] [flags 0x40000107 0x0 ] (X!=0 sharedMemory )  [0x7eff0a4c77e0] bci=[-1,0,628] rc=1 vc=0 vn=- li=- udi=- nc=0 flg=0x4
n33n        lconst 0                                                                          [0x7eff0a4c7880] bci=[-1,1,628] rc=1 vc=0 vn=- li=- udi=- nc=0

```

Bytecode `#1` `getfield` is translated into IL `n32n aloadi` to load an address. `"i"` is a suffix meaning it is an
indirect load. Indirect load means we don’t have an address to map to the field directly. We need another object to
load off from. The address load loads something pointing to an object or a reference field.

`n32n aloadi` is an indirect load of `java/util/HashMap.table`. Its type is an array: `[Ljava/util/HashMap$Node;`. The
second child of `n32n aloadi` is the base object (`n31n aload`). It indirectly loads field `java/util/HashMap.table`
off an object of `Ljava/util/HashMap;`.

The symbol reference in `n32n aloadi` tell us what we are loading. The symbol reference number is `#372`. `Shadow` is
an indirect access. It is not static, or local variable. It indicates that another pointer is required to complete the
memory access. The field offset is `16` within Ljava/util/HashMap object.

The child node (`n31n aload`) is evaluated first, and it is put into a register. Then offset `16` is added to it to get
the location of the field to do a memory access at the address.

`n34n compressedRefs` node indicates a field access or an array element access here requires compression/decompression
to be done. The second child (`n33n lconst 0`) of the `compressedRefs` is the start of the heap. It supports the case
where the heap base is non-zero value. Field access is stored in a compressed 4 bytes form. For a load, it requires
decompression. For a store, it requires compression before storing into the field.

Nodes in a tree are indented according to their depth in the tree. Indentation level indicates the parent-child relationship.
`n34n compressedRefs` has two children. The first child `n32n aloadi` has its own child `n31n aload` which is indented again.
`n33n lconst` is the second child of `n34n compressedRefs`.

### 2.3 Bytecode `astore`, `ifnull` And Their Trees

```
        +------------- Byte Code Index
        |  +-------------------- OpCode
        |  |                        +------------- First Field
        |  |                        |     +------------- Branch Target
        |  |                        |     |      +------- Const Pool Index
        |  |                        |     |      |    +------------- Constant
        |  |                        |     |      |    |
        V  V                        V     V      V    V

        5, JBastore                 6
        7, JBifnull                12,   19,
...
...

n35n      astore  <auto slot 6>[#373  Auto] [flags 0x7 0x0 ]                                  [0x7eff0a4c7920] bci=[-1,5,628] rc=0 vc=0 vn=- li=- udi=- nc=1
n32n        ==>aloadi
n39n      ifacmpeq --> block_4 BBStart at n1n ()                                              [0x7eff0a4c7a60] bci=[-1,7,628] rc=0 vc=0 vn=- li=- udi=- nc=2 flg=0x20
n32n        ==>aloadi
n36n        aconst NULL
```

`n35n astore` stores the value into `auto` slot `6` corresponding to the symbol table reference `#373`.

Some symbol reference numbers are reserved for known methods such as various helpers: `#71 jitRetranslateCallerWithPreparation`,
`#323 start-PC`, etc. For internal known symbols, we generate these symbol references in advance.

`auto` can be substitute as local variable. They are variables allocated on the stack most of the time. Sometimes they
correspond to local variables that exist in the program, or they could be temporary variables that JIT introduces.

`flags` are properties of a node, for example `nodeIsNonNull`.

`n32n ==>aloadi` is the child of `n35n astore`. It is the rhs value that is being stored. `==>` represents commoning,
i.e. the value of the expression has already been computed. It is a back reference to a note whose value has already
been computed at an earlier point in the (extended) basic block. The node global index (`n32n` in this case) can be
used to search for the first reference of this node, which is `n32n aloadi java/util/HashMap.table`.

This store consumes the value that is evaluated before. We are not doing load again
here for `n35n astore`. We already did the load before. We are going to reuse the register value from the previous
computation. `n32n` is also reused by `n39n ifacmpeq` for `NULL` comparison. If it is equal to `NULL`, it branches to
`block_4`. If it is not equal, it falls through to the next block.

### 2.4 Bytecode `aload`, `ifne` And Their Trees

```
        +------------- Byte Code Index
        |  +-------------------- OpCode
        |  |                        +------------- First Field
        |  |                        |     +------------- Branch Target
        |  |                        |     |      +------- Const Pool Index
        |  |                        |     |      |    +------------- Constant
        |  |                        |     |      |    |
        V  V                        V     V      V    V

       10, JBaload                  6
       12, JBarraylength
       13, JBdup
       14, JBistore                 8
       16, JBifne                  13,   29,
...
...

n37n      BBStart <block_3>                                                                   [0x7eff0a4c79c0] bci=[-1,10,628] rc=0 vc=0 vn=- li=- udi=- nc=0
n42n      NULLCHK on n40n [#32]                                                               [0x7eff0a4c7b50] bci=[-1,12,628] rc=0 vc=0 vn=- li=- udi=- nc=1
n41n        arraylength                                                                       [0x7eff0a4c7b00] bci=[-1,12,628] rc=3 vc=0 vn=- li=- udi=- nc=1
n40n          aload  <auto slot 6>[#373  Auto] [flags 0x7 0x0 ]                               [0x7eff0a4c7ab0] bci=[-1,10,628] rc=1 vc=0 vn=- li=- udi=- nc=0
n43n      istore  <auto slot 8>[#374  Auto] [flags 0x3 0x0 ]                                  [0x7eff0a4c7ba0] bci=[-1,14,628] rc=0 vc=0 vn=- li=- udi=- nc=1
n41n        ==>arraylength
n45n      ificmpne --> block_5 BBStart at n3n ()                                              [0x7eff0a4c7c40] bci=[-1,16,628] rc=0 vc=0 vn=- li=- udi=- nc=2 flg=0x20
n41n        ==>arraylength
n44n        iconst 0                                                                          [0x7eff0a4c7bf0] bci=[-1,16,628] rc=1 vc=0 vn=- li=- udi=- nc=0
n38n      BBEnd </block_3> =====
```

`n42n NULLCHK`: There is no `NULL` check in bytecodes, but we must raise a `NullPointerException` if the `n40n aload`
is `NULL`. When accessing `n31n aload` in the previous example, we don’t need to do a `NULL` check because we assume
this pointer is non-NULL. If this pointer is `NULL`, this method will not even have been entered.

`n43n istore`: Stores the `arrayLength` value to stack `auto` slot `8`.

`n45n ificmpne`: Tests if `arrayLength` equals to `0`. If yes, it branches to `block_5`. If no, it falls through to the
next block.

The reason that we have separate trees or treetops is because we want to order [side effects](https://github.com/eclipse/omr/blob/master/doc/compiler/il/IntroToTrees.md#side-effects) in a program.
The inexplicit order of treetop indicates the order of side effects. They happen one after another.

Side effects mean that the result of an operation will have an effect even after the trees are executed. Throwing an
exception, changing memory, and branching are known side effect operations. If a `NULL` check fails, it goes to a
completely different block. Doing a store is going to change a memory location. The memory is changed when it gets to
evaluate the next treetop. When branching happens, the effect of that is felt by whatever comes next.

Only one side effect exits in each treetop. We do not want one tree to have a `NULL` check, and a store, and branching.


## 3. Optimization: globalValuePropagation

[Global Value Propagation](https://github.com/eclipse/omr/blob/master/doc/compiler/optimizer/ValuePropagation.md) is important because it is one place where it has the most knowledge of what the semantics
are for different opcodes.

```
<optimization id=42 name=globalValuePropagation method=net/adoptopenjdk/bumblebench/collections/ArrayListForEachLambdaBench.doBatch(J)J>
Performing 42: globalValuePropagation
            (Building use/def info)
         PREPARTITION VN   (Building value number info)
[  4830] O^O VALUE PROPAGATION: Constant folding lload [00007F00A30046D0]          to lconst 0
[  4831] O^O VALUE PROPAGATION: Add known-object constraint to 00007F00A3004AE0 based on known object obj1 of class Ljava/lang/invoke/ConstructorHandle;
Abort transformIndirectLoadChain - cannot dereference at compile time!
Abort transformIndirectLoadChain - cannot dereference at compile time!
[  4832] O^O VALUE PROPAGATION:  Removing redundant call to jitCheckIfFinalize [00007F00A30A35C0]
[  4833] O^O VALUE PROPAGATION: Replace PassThrough node [00007F00A30A35C0] with its child in its parent [00007F00A30A3570]
[  4834] O^O VALUE PROPAGATION: Changing wrtbar to store because the rhs is a new object [00007F00A30A3070]
[  4835] O^O VALUE PROPAGATION: Removing redundant null check node [00007F00A30AC0D0]
```

`globalValuePropagation` performs constant propagation and type propagation. Global optimization looks at all the blocks
including the inlined code. It runs several times in `scorching` and `hot` compilations. It does not run for `cold`
compilation. It can be expensive for `cold` compilations.

### 3.1 Fold A Constant

```
[  4830] O^O VALUE PROPAGATION: Constant folding lload [00007F00A30046D0]          to lconst 0

Before VP:

n13n      iflcmpge --> block_68 BBStart at n755n ()                                           [0x7f00a3004810] bci=[-1,5,25] rc=0 vc=351 vn=- li=-1 udi=- nc=2 flg=0x20
n9n         lload  i<auto slot 3>[#352  Auto] [flags 0x4 0x0 ] (cannotOverflow )              [0x7f00a30046d0] bci=[-1,2,25] rc=1 vc=351 vn=- li=- udi=- nc=0 flg=0x1000
n10n        lload  numIterations<parm 1 J>[#351  Parm] [flags 0x40000104 0x0 ] (cannotOverflow )  [0x7f00a3004720] bci=[-1,3,25] rc=1 vc=351 vn=- li=- udi=- nc=0 flg=0x1000
n4n       BBEnd </block_3> =====

After VP:

n13n      iflcmpge --> block_68 BBStart at n755n ()                                           [0x7f00a3004810] bci=[-1,5,25] rc=0 vc=380 vn=- li=- udi=- nc=2 flg=0x20
n9n         lconst 0 (X==0 X>=0 X<=0 cannotOverflow )                                         [0x7f00a30046d0] bci=[-1,2,25] rc=1 vc=380 vn=- li=12 udi=- nc=0 flg=0x1302
n10n        lload  numIterations<parm 1 J>[#351  Parm] [flags 0x40000104 0x0 ] (cannotOverflow )  [0x7f00a3004720] bci=[-1,3,25] rc=1 vc=380 vn=- li=- udi=39 nc=0 flg=0x1000
n4n       BBEnd </block_3> =====
```

If there is a value and it is a constant, the value is folded with the constant.

We can search the node address `0x7f00a30046d0` to trace its transformations. Before VP, node `0x7f00a30046d0` (`n9n`)
is `lload i<auto slot 3>`. It loads from stack auto slot `3`. The variable name is `i` in the source code.
After VP, node `0x7f00a30046d0` (`n9n`) is transformed into `lconst 0`. It is still the same node with the same address
but it has a different opcode now.

```
[  4830] O^O VALUE PROPAGATION: Constant folding lload [00007F00A30046D0]          to lconst 0
```
[performTransformation](https://github.com/eclipse/omr/blob/6eec759cd2d446f74d2b8a7ee3348d98ce6bfa79/compiler/ras/Debug.cpp#L489) prints out the message on the above and guards whether or not the transformation performs.
`[  4830]` indicates the number of transformations that have occurred so far. To [narrow down the transformation](https://github.com/eclipse/omr/blob/master/doc/compiler/ProblemDetermination.md#identifying-the-failing-optimization) that
causes a problem, we can do a binary search on these numbers by using `lastOptIndex`. If `lastOptIndex=4830`, none of
the transformations after `4830` will take place.

### 3.2 Fold `checkcast`

```
[  5584] O^O VALUE PROPAGATION: Removing redundant checkcast node [00007FEF2577CB70]

Before VP:

n191n     checkcast [#86]                                                                     [0x7fef2577cb70] bci=[1,7,-] rc=0 vc=312 vn=- li=-1 udi=- nc=2
n189n       aload  <auto slot 2>[#391  Auto] [flags 0x7 0x0 ]                                 [0x7fef2577cad0] bci=[1,6,-] rc=1 vc=312 vn=- li=- udi=- nc=0
n190n       loadaddr  java/lang/invoke/MemberName[#392  Static] [flags 0x18307 0x0 ]          [0x7fef2577cb20] bci=[1,7,-] rc=1 vc=312 vn=- li=- udi=- nc=0

After VP:

n191n     treetop                                                                             [0x7fef2577cb70] bci=[1,7,-] rc=0 vc=328 vn=- li=- udi=- nc=1
n189n       aload  <auto slot 2>[#391  Auto] [flags 0x7 0x0 ] (X!=0 sharedMemory )            [0x7fef2577cad0] bci=[1,6,-] rc=1 vc=328 vn=- li=- udi=39 nc=0 flg=0x4
```
Before VP, `0x7fef2577cb70` was loading from auto slot `2` and `checkast` against `java/lang/invoke/MemberName`.
After VP, `0x7fef2577cb70` is converted into a `treetop` from a `checkcast` node.

### 3.3 Fold `NULLCHK`

```
[  5585] O^O VALUE PROPAGATION: Removing redundant null check node [00007FEF2577FE10]

Before VP:

n353n     NULLCHK on n355n [#32]                                                              [0x7fef2577fe10] bci=[3,0,-] rc=0 vc=312 vn=- li=-1 udi=- nc=1
n352n       PassThrough                                                                       [0x7fef2577fdc0] bci=[3,0,-] rc=1 vc=312 vn=- li=- udi=- nc=1
n355n         aload  <temp slot 3>[#413  Auto] [flags 0x20000007 0x0 ]                        [0x7fef2577feb0] bci=[3,5,-] rc=1 vc=312 vn=- li=- udi=- nc=0

After VP:

n353n     treetop                                                                             [0x7fef2577fe10] bci=[3,0,-] rc=0 vc=328 vn=- li=- udi=- nc=1
n355n       aload  <temp slot 3>[#413  Auto] [flags 0x20000007 0x0 ] (X!=0 sharedMemory )     [0x7fef2577feb0] bci=[3,5,-] rc=1 vc=328 vn=- li=- udi=40 nc=0 flg=0x4
```
Originally, we had a `NULLCHK` on temp slot `3` before VP. After VP, `NULLCHK` node is converted a noop `treetop` node
to hang on the `n355n aload`.

### 3.4 Fold `ificmpne`

```
[  5588] O^O VALUE PROPAGATION: Removing conditional branch [00007FEF2577EF60] ificmpne

Before VP:

n331n     astore  <temp slot 3>[#413  Auto] [flags 0x20000007 0x0 ] (X!=0 privatizedInlinerArg sharedMemory )  [0x7fef2577f730] bci=[3,0,-] rc=0 vc=312 vn=- li=-1 udi=- nc=1 flg=0x2004
n238n       ==>new
…
n353n     NULLCHK on n355n [#32]                                                              [0x7fef2577fe10] bci=[3,0,-] rc=0 vc=312 vn=- li=-1 udi=- nc=1
n352n       PassThrough                                                                       [0x7fef2577fdc0] bci=[3,0,-] rc=1 vc=312 vn=- li=- udi=- nc=1
n355n         aload  <temp slot 3>[#413  Auto] [flags 0x20000007 0x0 ]                        [0x7fef2577feb0] bci=[3,5,-] rc=1 vc=312 vn=- li=- udi=- nc=0
n306n     ificmpne --> block_21 BBStart at n312n ()                                           [0x7fef2577ef60] bci=[5,0,39] rc=0 vc=312 vn=- li=-1 udi=- nc=2 flg=0x20
n605n       iand (X>=0 cannotOverflow )                                                       [0x7fef25784cd0] bci=[5,0,39] rc=1 vc=312 vn=- li=- udi=- nc=2 flg=0x1100
n604n         l2i                                                                             [0x7fef25784c80] bci=[5,0,39] rc=1 vc=312 vn=- li=- udi=- nc=1
n302n           lloadi  <isClassAndDepthFlags>[#279  Shadow +24] [flags 0x603 0x0 ] (cannotOverflow )  [0x7fef2577ee20] bci=[5,0,39] rc=1 vc=312 vn=- li=- udi=- nc=1 flg=0x1000
n301n             aloadi  <vft-symbol>[#282  Shadow] [flags 0x18607 0x0 ] (X!=0 X>=0 sharedMemory )  [0x7fef2577edd0] bci=[5,0,39] rc=1 vc=312 vn=- li=- udi=- nc=1 flg=0x104
n297n               aload  <temp slot 3>[#413  Auto] [flags 0x20000007 0x0 ] (X!=0 X>=0 sharedMemory )  [0x7fef2577ec90] bci=[5,0,39] rc=1 vc=312 vn=- li=- udi=- nc=0 flg=0x104
n602n         iconst 0x40200000 (X!=0 X>=0 )                                                  [0x7fef25784be0] bci=[5,0,39] rc=1 vc=312 vn=- li=- udi=- nc=0 flg=0x104
n603n       iconst 0 (X==0 X>=0 X<=0 )                                                        [0x7fef25784c30] bci=[5,0,39] rc=1 vc=312 vn=- li=- udi=- nc=0 flg=0x302
n311n     BBEnd </block_4> =====                                                              [0x7fef2577f0f0] bci=[5,0,39] rc=0 vc=294 vn=- li=-1 udi=- nc=0

After VP:

n331n     astore  <temp slot 3>[#413  Auto] [flags 0x20000007 0x0 ] (X!=0 privatizedInlinerArg sharedMemory )  [0x7fef2577f730] bci=[3,0,-] rc=0 vc=312 vn=- li=-1 udi=- nc=1 flg=0x2004
n238n       ==>new
…
n353n     treetop                                                                             [0x7fef2577fe10] bci=[3,0,-] rc=0 vc=328 vn=- li=- udi=- nc=1
n355n       aload  <temp slot 3>[#413  Auto] [flags 0x20000007 0x0 ] (X!=0 sharedMemory )     [0x7fef2577feb0] bci=[3,5,-] rc=1 vc=328 vn=- li=- udi=40 nc=0 flg=0x4
n311n     BBEnd </block_4> =====                                                              [0x7fef2577f0f0] bci=[5,0,39] rc=0 vc=328 vn=- li=- udi=- nc=0
```

This is an example of branch folding. `n306n ificmpne` checks the bits in `classAndDepthFlags` of `J9Class` from the
object on temp slot `3`. We know temp slot `3` stores a newly created object (`n331n  astore`). We might know the type
of the object and what its `classAndDepthFlags` are during the compilation time. Based on the value of `classAndDepthFlags`,
it knows it will fall through to the next block. The `ificmpne` is completed removed.

### 3.5 Fold `awrtbari`

```
[  5589] O^O VALUE PROPAGATION: Changing wrtbar to store because the rhs is a new object [00007FEF2577E600]

Before VP:

n276n       awrtbari  com/ibm/bumblebench/collections/ArrayListForEachLambdaBench$$Lambda$44/0x0000000000000000.arg$1 Lcom/ibm/bumblebench/collections/ArrayListForEachLambdaBench;[#410  final Shadow +8] [flags 0xa0607 0x0 ] (sharedMemory )  [0x7fef2577e600] bci=[4,6,-] rc=1 vc=312 vn=- li=- udi=- nc=3 flg=0x20

After VP:

n276n       awrtbari  com/ibm/bumblebench/collections/ArrayListForEachLambdaBench$$Lambda$44/0x0000000000000000.arg$1 Lcom/ibm/bumblebench/collections/ArrayListForEachLambdaBench;[#410  final Shadow +8] [flags 0xa0607 0x0 ] (X!=0 skipWrtBar sharedMemory )  [0x7fef2577e600] bci=[4,6,-] rc=1 vc=328 vn=- li=- udi=- nc=3 flg=0x824
```

The rhs of `n276n awrtbari` node is a new object. We don’t need to do the bookkeeping. We put `skipWrtBar` on the node.
When it gets to the codegen, the write barrier sequence will not be generated, instead only the store will be generated.

### 3.6 Fold `calli`

```
[  5591] O^O VALUE PROPAGATION: Changing an indirect call com/ibm/bumblebench/collections/ArrayListForEachLambdaBench$$Lambda$44/0x0000000000000000.accept(Ljava/lang/Object;)V (calli) to a direct call [00007FEF25782200]

Before VP:

n468n       calli  java/util/function/Consumer.accept(Ljava/lang/Object;)V[#436  unresolved interface Method] (Interface class) [flags 0x400 0x0 ] ()  [0x7fef25782200] bci=[6,48,1541] rc=1 vc=312 vn=- li=- udi=- nc=3 flg=0x20

After VP:

n468n       call  com/ibm/bumblebench/collections/ArrayListForEachLambdaBench$$Lambda$44/0x0000000000000000.accept(Ljava/lang/Object;)V[#445  final virtual Method -64] [flags 0x20500 0x0 ] (virtualCallNodeForAGuardedInlinedCall )  [0x7fef25782200] bci=[6,48,1541] rc=1 vc=328 vn=- li=- udi=- nc=2 flg=0x820
```
With type information, the target of the call `n468n calli` is known at the compilation time. The indirect call `calli`
is changed into a direct call `call`, de-virtualized. When we de-virtualize a call in VP, we also try to inline it.


## 4. Optimization: localCSE

localCSE (Local Common Subexpression Elimination) operates in a single extended basic block. It commons nodes to avoid
repeating common computations, and it increases the reference count on the nodes. Commoning allows us to reuse the same
register as much as we can. localCSE also does local copy propagation: Checking the store nodes and propagating the
value to other nodes that use the stored value.

### 4.1 Common `aload`
```
<optimization id=109 name=localCSE method=com/ibm/bumblebench/collections/ArrayListForEachLambdaBench.doBatch(J)J>

[  5833] O^O LOCAL COMMON SUBEXPRESSION ELIMINATION:    Local Common Subexpression Elimination commoning node : 00007FEF25A9F140 by available node : 00007FEF25A9F000

Before localCSE:

n623n     BBStart <block_55> (freq 6) (loop pre-header)                                       [0x7fef25785270] bci=[-1,1,12] rc=0 vc=1318 vn=- li=-1 udi=- nc=0
n1128n    astore  <temp slot 8>[#465  Auto] [flags 0x7 0x0 ]                                  [0x7fef25a9f050] bci=[-1,18,14] rc=0 vc=1318 vn=- li=- udi=- nc=1
n1127n      aload  <callSite entry @0     0x3455e0>[#359  Static] (obj1) [flags 0x307 0x8 ]   [0x7fef25a9f000] bci=[-1,18,14] rc=1 vc=1318 vn=- li=5 udi=- nc=0
n1135n    compressedRefs                                                                      [0x7fef25a9f280] bci=[-1,18,14] rc=0 vc=1318 vn=- li=- udi=- nc=2
n1129n      aloadi  unknown field[#360  Shadow] (obj2) [flags 0x80000607 0x0 ]                [0x7fef25a9f0a0] bci=[-1,18,14] rc=2 vc=1318 vn=- li=6 udi=- nc=1
n1130n        aladd (internalPtr sharedMemory )                                               [0x7fef25a9f0f0] bci=[-1,18,14] rc=1 vc=1318 vn=- li=-1 udi=- nc=2 flg=0x8000
n1131n          aload  <callSite entry @0     0x3455e0>[#359  Static] (obj1) [flags 0x307 0x8 ]  [0x7fef25a9f140] bci=[-1,18,14] rc=1 vc=1318 vn=- li=5 udi=- nc=0
n1132n          lconst 20                                                                     [0x7fef25a9f190] bci=[-1,18,14] rc=1 vc=1318 vn=- li=-1 udi=- nc=0
n1134n      lconst 0                                                                          [0x7fef25a9f230] bci=[-1,18,14] rc=1 vc=1318 vn=- li=- udi=- nc=0
n1133n    astore  <temp slot 9>[#466  Auto] [flags 0x7 0x0 ]                                  [0x7fef25a9f1e0] bci=[-1,18,14] rc=0 vc=1318 vn=- li=- udi=- nc=1
n1129n      ==>aloadi
n624n     BBEnd </block_55> =====

After localCSE:

n623n     BBStart <block_55> (freq 6) (loop pre-header)                                       [0x7fef25785270] bci=[-1,1,12] rc=0 vc=1318 vn=- li=-1 udi=- nc=0
n1128n    astore  <temp slot 8>[#465  Auto] [flags 0x7 0x0 ]                                  [0x7fef25a9f050] bci=[-1,18,14] rc=0 vc=1409 vn=- li=- udi=- nc=1
n1127n      aload  <callSite entry @0     0x3455e0>[#359  Static] (obj1) [flags 0x307 0x8 ]   [0x7fef25a9f000] bci=[-1,18,14] rc=2 vc=1409 vn=- li=- udi=- nc=0
n1135n    compressedRefs                                                                      [0x7fef25a9f280] bci=[-1,18,14] rc=0 vc=1409 vn=- li=- udi=- nc=2
n1129n      aloadi  unknown field[#360  Shadow] (obj2) [flags 0x80000607 0x0 ]                [0x7fef25a9f0a0] bci=[-1,18,14] rc=2 vc=1409 vn=- li=- udi=- nc=1
n1130n        aladd (internalPtr sharedMemory )                                               [0x7fef25a9f0f0] bci=[-1,18,14] rc=1 vc=1409 vn=- li=- udi=- nc=2 flg=0x8000
n1127n          ==>aload
n1132n          lconst 20                                                                     [0x7fef25a9f190] bci=[-1,18,14] rc=1 vc=1409 vn=- li=- udi=- nc=0
n1134n      lconst 0                                                                          [0x7fef25a9f230] bci=[-1,18,14] rc=1 vc=1409 vn=- li=- udi=- nc=0
n1133n    astore  <temp slot 9>[#466  Auto] [flags 0x7 0x0 ]                                  [0x7fef25a9f1e0] bci=[-1,18,14] rc=0 vc=1409 vn=- li=- udi=- nc=1
n1129n      ==>aloadi
n624n     BBEnd </block_55> =====
```

Both `n1127n aload` and `n1131n aload` load the same call site with symRef `#359`. After localCSE, The child of
`n1130n aladd` back references the earlier `n1127n aload`. Before localCSE, the same computation repeats twice.
After the commoning transformation is done, when it gets to the codegen, `n1127n aload` is evaluated first and the
value is put into a register. The next time, it will not be evaluated. We will just pick up the register value
for `n1127n` when it is referenced under `n1130n`.

### 4.2 Local Copy Propagation

```
[  5837] O^O LOCAL COMMON SUBEXPRESSION ELIMINATION:    Local Common Subexpression Elimination propagating local #415 in node : 00007FEF2577E560 PARENT : 00007FEF2577E600 from node 00007FEF2577FF50

Before localCSE:

357n     astore  <temp slot 2>[#415  Auto] [flags 0x7 0x0 ] (X!=0 sharedMemory )             [0x7fef2577ff50] bci=[3,0,-] rc=0 vc=1320 vn=- li=-1 udi=5 nc=1 flg=0xc
n238n       ==>new
n278n     compressedRefs                                                                      [0x7fef2577e6a0] bci=[4,6,-] rc=0 vc=1320 vn=- li=-1 udi=- nc=2
n276n       awrtbari  com/ibm/bumblebench/collections/ArrayListForEachLambdaBench$$Lambda$44/0x0000000000000000.arg$1 Lcom/ibm/bumblebench/collections/ArrayListForEachLambdaBench;[#410  final Shadow +8] [flags 0xa0607 0x0 ] (X!=0 skipWrtBar sharedMemory )  [0x7fef2577e600] bci=[4,6,-] rc=1 vc=1320 vn=- li=9 udi=- nc=3 flg=0x824
n274n         aload  <temp slot 2>[#415  Auto] [flags 0x7 0x0 ] (X!=0 X>=0 sharedMemory )     [0x7fef2577e560] bci=[4,4,-] rc=2 vc=1320 vn=- li=8 udi=56 nc=0 flg=0x104
n275n         aload  this<'this' parm Lcom/ibm/bumblebench/collections/ArrayListForEachLambdaBench;>[#351  Parm] [flags 0x40000107 0x0 ] (X!=0 sharedMemory )  [0x7fef2577e5b0] bci=[4,5,-] rc=1 vc=1320 vn=- li=2 udi=57 nc=0 flg=0x4
n274n         ==>aload
n277n       lconst 0 (highWordZero X==0 X>=0 X<=0 )

After localCSE:

n357n     astore  <temp slot 2>[#415  Auto] [flags 0x7 0x0 ] (X!=0 sharedMemory )             [0x7fef2577ff50] bci=[3,0,-] rc=0 vc=1413 vn=- li=- udi=5 nc=1 flg=0xc
n238n       ==>new
n278n     compressedRefs                                                                      [0x7fef2577e6a0] bci=[4,6,-] rc=0 vc=1413 vn=- li=- udi=- nc=2
n276n       awrtbari  com/ibm/bumblebench/collections/ArrayListForEachLambdaBench$$Lambda$44/0x0000000000000000.arg$1 Lcom/ibm/bumblebench/collections/ArrayListForEachLambdaBench;[#410  final Shadow +8] [flags 0xa0607 0x0 ] (X!=0 skipWrtBar sharedMemory )  [0x7fef2577e600] bci=[4,6,-] rc=1 vc=1413 vn=- li=- udi=- nc=3 flg=0x824
n238n         ==>new
n14n          ==>aload
n238n         ==>new
n15n        ==>lconst 0
```

`n274n aload` loads an object with symRef `#415` from the temp slot `2`. Prior to it, `n357n astore` stores a new object
(`n238n`) with the same symRef `#415` to the temp slot `2`. The new object (`n238n`) would already be in a register when
it gets to evaluate `n274n`. We could just pick up the value directly rather than doing a store first and then loading
from the memory (`n274n`). After the localCSE transformation, the first child (`n274n aload`) of `n276n awrtbari` is
converted into a direct or back reference to `n238n new`, which is the first child of `n357n astore`.

`n357n astore` is still there after localCSE. It is not the job of localCSE to clean it up. It will be the job of
`deadTreeElimination` to do it later if no load is driven by this `n357n astore`.

## 5. Optimization: basicBlockExtension

### 5.1 Extend Basic Blocks
```
Before basicBlockExtension:

n1279n    BBStart <block_126> (freq 6)                                                        [0x7fef25951f80] bci=[16,6,15] rc=0 vc=6377 vn=- li=- udi=- nc=0
n1296n    NULLCHK on n1298n [#32]                                                             [0x7fef259524d0] bci=[15,1,-] rc=0 vc=6377 vn=- li=- udi=- nc=1
n1237n      lloadi  com/ibm/bumblebench/collections/ArrayListForEachLambdaBench._sum J[#354  Shadow +64] [flags 0x80604 0x0 ] (cannotOverflow )  [0x7fef25951260] bci=[16,2,15] rc=2 vc=6377 vn=- li=- udi=- nc=1 flg=0x1000
n1298n        aload  <temp slot 6>[#511  Auto] [flags 0x20000007 0x0 ] (X>=0 sharedMemory )   [0x7fef25952570] bci=[15,8,-] rc=1 vc=6377 vn=- li=- udi=235 nc=0 flg=0x100
n1271n    lstore  <temp slot 5>[#510  Auto] [flags 0x4 0x0 ]                                  [0x7fef25951d00] bci=[16,2,15] rc=0 vc=6377 vn=- li=- udi=67 nc=1
n1237n      ==>lloadi
n4264n    BBEnd </block_126> =====

After basicBlockExtension:

n1279n    BBStart <block_126> (freq 6) (extension of previous block) (in loop 82)             [0x7fef25951f80] bci=[16,6,15] rc=0 vc=6377 vn=- li=- udi=- nc=0
n1296n    NULLCHK on n1298n [#32]                                                             [0x7fef259524d0] bci=[15,1,-] rc=0 vc=6377 vn=- li=- udi=- nc=1
n1237n      lloadi  com/ibm/bumblebench/collections/ArrayListForEachLambdaBench._sum J[#354  Shadow +64] [flags 0x80604 0x0 ] (cannotOverflow )  [0x7fef25951260] bci=[16,2,15] rc=2 vc=6377 vn=- li=- udi=- nc=1 flg=0x1000
n1298n        aload  <temp slot 6>[#511  Auto] [flags 0x20000007 0x0 ] (X>=0 sharedMemory )   [0x7fef25952570] bci=[15,8,-] rc=1 vc=6377 vn=- li=- udi=235 nc=0 flg=0x100
n1271n    lstore  <temp slot 5>[#510  Auto] [flags 0x4 0x0 ]                                  [0x7fef25951d00] bci=[16,2,15] rc=0 vc=6377 vn=- li=- udi=67 nc=1
n1237n      ==>lloadi
n4264n    BBEnd </block_126>

```

A basic block has a single entry and multiple exits. Up until the path of `basicBlockExtension`, the blocks are not extended.
There is only one way to enter to `block_126`, which is the predecessor falling through to it. After `basicBlockExtension`,
`block_126` becomes an extension of previous block.

Marking blocks as extensions of previous blocks doesn’t result in any better code. Once they are marked as extensions
of previous block, optimizations like localCSE could run again. It can common up values in `block_126` with the values
in its predecessors from the same extended block. It allows the expansion of other local optimizations beyond just one
block. Other local optimizations can now operate on the extended blocks.

### 5.2 Enable Other Optimizations To Run Again

```
n1274n    astore  <temp slot 6>[#511  Auto] [flags 0x20000007 0x0 ] (privatizedInlinerArg sharedMemory )  [0x7fef25951df0] bci=[15,1,-] rc=0 vc=6377 vn=- li=- udi=66 nc=1 flg=0x2000
n1225n      ==>aloadi
n4262n    BBEnd </block_131>                                                                  [0x7fef2641c3f0] bci=[16,0,15] rc=0 vc=0 vn=- li=- udi=- nc=0
n1279n    BBStart <block_126> (freq 6) (extension of previous block) (in loop 82)             [0x7fef25951f80] bci=[16,6,15] rc=0 vc=6377 vn=- li=- udi=- nc=0
n1296n    NULLCHK on n1298n [#32]                                                             [0x7fef259524d0] bci=[15,1,-] rc=0 vc=6377 vn=- li=- udi=- nc=1
n1237n      lloadi  com/ibm/bumblebench/collections/ArrayListForEachLambdaBench._sum J[#354  Shadow +64] [flags 0x80604 0x0 ] (cannotOverflow )  [0x7fef25951260] bci=[16,2,15] rc=2 vc=6377 vn=- li=- udi=- nc=1 flg=0x1000
n1298n        aload  <temp slot 6>[#511  Auto] [flags 0x20000007 0x0 ] (X>=0 sharedMemory )   [0x7fef25952570] bci=[15,8,-] rc=1 vc=6377 vn=- li=- udi=235 nc=0 flg=0x100

localCSE runs again after basicBlockExtension

n1296n    NULLCHK on n1225n [#32]                                                             [0x7fef259524d0] bci=[15,1,-] rc=0 vc=6450 vn=- li=- udi=- nc=1
n1237n      lloadi  com/ibm/bumblebench/collections/ArrayListForEachLambdaBench._sum J[#354  Shadow +64] [flags 0x80604 0x0 ] (cannotOverflow )  [0x7fef25951260] bci=[16,2,15] rc=3 vc=6450 vn=- li=- udi=- nc=1 flg=0x1000
n1225n        ==>aloadi
```

`n1298n aload` loads symbol `#511` from temp slot `6` in `block_126`. `#511` is loaded by `n1225n aloadi` and stored to
temp slot `6` by `n1274n astore` in `block_131`. After `basicBlockExtension` and `localCSE` runs again which does copy
propagation. The first child of `n1237n lloadi` is no longer a load of temp `n1298n aload` but now it is commoned to
the rhs (`n1225n aloadi`) of the `n1274n astore`.

## 6. Optimization: tacticalGlobalRegisterAllocator

`tacticalGlobalRegisterAllocator` (GRA) extends the register setup between multiple different extended blocks. Memory
loads/stores become register loads/stores. Instead of referencing stack slots, nodes access registers for values.

```
Before GRA:

n638n     BBStart <block_53> (freq 509)                                                       [0x7fef25795720] bci=[-1,0,12] rc=0 vc=55 vn=- li=- udi=- nc=0
n642n     ificmpeq --> block_2 BBStart at n5n (profilingCode )                                [0x7fef25795860] bci=[-1,0,12] rc=0 vc=55 vn=- li=- udi=- nc=2 flg=0xa0
n640n       iload  0x7fef247330cc[#479  Static] [flags 0x10303 0x8040 ] (cannotOverflow )     [0x7fef257957c0] bci=[-1,0,12] rc=1 vc=55 vn=- li=- udi=- nc=0 flg=0x1000
n641n       iconst -1 (X!=0 X<=0 )                                                            [0x7fef25795810] bci=[-1,0,12] rc=1 vc=55 vn=- li=- udi=- nc=0 flg=0x204
n639n     BBEnd </block_53>
…
n1097n    BBStart <block_109> (freq 509) (extension of previous block)                        [0x7fef2594e6a0] bci=[-1,1,12] rc=0 vc=55 vn=- li=- udi=- nc=0
n1102n    iflcmple --> block_5 BBStart at n1n ()                                              [0x7fef2594e830] bci=[-1,5,12] rc=0 vc=55 vn=- li=- udi=- nc=2 flg=0x20020
n1104n      lload  numIterations<parm 1 J>[#352  Parm] [flags 0x40000104 0x0 ] (cannotOverflow )  [0x7fef2594e8d0] bci=[-1,3,12] rc=1 vc=55 vn=- li=- udi=65 nc=0 flg=0x1000
n1099n      lconst 0 (highWordZero X==0 X>=0 X<=0 )                                           [0x7fef2594e740] bci=[-1,0,12] rc=2 vc=55 vn=- li=- udi=- nc=0 flg=0x4302
n1105n    BBEnd </block_109>
…
n5n       BBStart <block_2> (freq 6)                                                          [0x7fef256de520] bci=[-1,1,12] rc=0 vc=2 vn=- li=-2 udi=- nc=0
n13n      iflcmple --> block_5 BBStart at n1n ()                                              [0x7fef256de7a0] bci=[-1,5,12] rc=0 vc=2 vn=- li=-2 udi=- nc=2 flg=0x20020
n10n        lload  numIterations<parm 1 J>[#352  Parm] [flags 0x40000104 0x0 ] (cannotOverflow )  [0x7fef256de6b0] bci=[-1,3,12] rc=1 vc=2 vn=- li=-1 udi=85 nc=0 flg=0x1000
n7n         lconst 0 (highWordZero X==0 X>=0 X<=0 )                                           [0x7fef256de5c0] bci=[-1,0,12] rc=2 vc=2 vn=- li=-2 udi=- nc=0 flg=0x4302
n4n       BBEnd </block_2>


After GRA:

n638n     BBStart <block_53> (freq 509)                                                       [0x7fef25795720] bci=[-1,0,12] rc=0 vc=2718 vn=- li=- udi=- nc=1
n4810n      GlRegDeps                                                                         [0x7fef26426f30] bci=[-1,0,12] rc=1 vc=2718 vn=- li=- udi=- nc=2
n4811n        lRegLoad esi   numIterations<parm 1 J>[#352  Parm] [flags 0x40000104 0x0 ] (cannotOverflow SeenRealReference )  [0x7fef26426f80] bci=[-1,3,12] rc=6 vc=2718 vn=- li=- udi=- nc=0 flg=0x9000
n4812n        aRegLoad eax   this<'this' parm Lcom/ibm/bumblebench/collections/ArrayListForEachLambdaBench;>[#351  Parm] [flags 0x40000107 0x0 ]  [0x7fef26426fd0] bci=[-1,0,12] rc=5 vc=2718 vn=- li=- udi=- nc=0
n642n     ificmpeq --> block_2 BBStart at n5n (profilingCode )                                [0x7fef25795860] bci=[-1,0,12] rc=0 vc=2718 vn=- li=- udi=- nc=3 flg=0xa0
n640n       iload  0x7fef247330cc[#479  Static] [flags 0x10303 0x8040 ] (cannotOverflow )     [0x7fef257957c0] bci=[-1,0,12] rc=1 vc=2718 vn=- li=- udi=- nc=0 flg=0x1000
n641n       iconst -1 (X!=0 X<=0 )                                                            [0x7fef25795810] bci=[-1,0,12] rc=1 vc=2718 vn=- li=- udi=- nc=0 flg=0x204
n4813n      GlRegDeps                                                                         [0x7fef26427020] bci=[-1,0,12] rc=1 vc=2718 vn=- li=- udi=- nc=2
n4811n        ==>lRegLoad
n4812n        ==>aRegLoad
n639n     BBEnd </block_53>
…
n1097n    BBStart <block_109> (freq 509) (extension of previous block)                        [0x7fef2594e6a0] bci=[-1,1,12] rc=0 vc=2718 vn=- li=- udi=- nc=0
n1102n    iflcmple --> block_495 BBStart at n4815n ()                                         [0x7fef2594e830] bci=[-1,5,12] rc=0 vc=2718 vn=- li=- udi=- nc=3 flg=0x20020
n4811n      ==>lRegLoad
n1099n      lconst 0 (highWordZero X==0 X>=0 X<=0 )                                           [0x7fef2594e740] bci=[-1,0,12] rc=3 vc=2718 vn=- li=- udi=- nc=0 flg=0x4302
n4818n      GlRegDeps                                                                         [0x7fef264271b0] bci=[-1,5,12] rc=1 vc=2718 vn=- li=- udi=- nc=2
n4811n        ==>lRegLoad
n4812n        ==>aRegLoad
n1105n    BBEnd </block_109>
…
n5n       BBStart <block_2> (freq 6)                                                          [0x7fef256de520] bci=[-1,1,12] rc=0 vc=2718 vn=- li=- udi=- nc=1
n5265n      GlRegDeps                                                                         [0x7fef12707d70] bci=[-1,1,12] rc=1 vc=2718 vn=- li=- udi=- nc=2
n5266n        lRegLoad esi   numIterations<parm 1 J>[#352  Parm] [flags 0x40000104 0x0 ] (cannotOverflow SeenRealReference )  [0x7fef12707dc0] bci=[-1,3,12] rc=4 vc=2718 vn=- li=- udi=- nc=0 flg=0x9000
n5267n        aRegLoad eax   this<'this' parm Lcom/ibm/bumblebench/collections/ArrayListForEachLambdaBench;>[#351  Parm] [flags 0x40000107 0x0 ]  [0x7fef12707e10] bci=[-1,1,12] rc=3 vc=2718 vn=- li=- udi=- nc=0
n13n      iflcmple --> block_504 BBStart at n5268n ()                                         [0x7fef256de7a0] bci=[-1,5,12] rc=0 vc=2718 vn=- li=- udi=- nc=3 flg=0x20020
n5266n      ==>lRegLoad
n7n         lconst 0 (highWordZero X==0 X>=0 X<=0 )                                           [0x7fef256de5c0] bci=[-1,0,12] rc=3 vc=2718 vn=- li=- udi=- nc=0 flg=0x4302
n5271n      GlRegDeps                                                                         [0x7fef12707f50] bci=[-1,5,12] rc=1 vc=2718 vn=- li=- udi=- nc=2
n5266n        ==>lRegLoad
n5267n        ==>aRegLoad
n4n       BBEnd </block_2>
```
`n1104n lload` in `block_109` is replaced with `n4811n ==>lRegLoad`.  `lRegLoad` is long register load. The value is
now picked up from the register that is associated with `n4811n lRegLoad`. Looking at the earlier references of `n4811n`
in `block_53`, `n4811n` takes the value `parm 1` and associates it with the real register `esi`. `n4811n` is also the
first child of [GlRegDeps](https://github.com/eclipse/omr/blob/6eec759cd2d446f74d2b8a7ee3348d98ce6bfa79/doc/compiler/il/GlRegDeps.md) which is the first child of `BBStart` in `block_53`. `GlRegDeps` is a request to the code
generator that the value must be in the register `esi` when coming into the `block_53`.

The third child of `n642n ificmpeq` is `n4813n GlRegDeps`. When `ificmpeq` branches to `block_2`, the values that are
associated with `n4811n lRegLoad` and `n4812n aRegLoad` have to be in the register `esi` and `eax`. `GlRegDeps` for the
`BBStart` in `block_2` loads from `esi` and `eax`.  `block_2` used to load `parm 1` and compares it with `0`. After GRA,
it picks up the value from register `esi` that is set up in `block_53`.

```
Before GRA:

n4314n    BBStart <block_448> (freq 5050) (extension of previous block)                       [0x7fef2641d430] bci=[-1,1,12] rc=0 vc=55 vn=- li=- udi=- nc=0
n4316n    lstore  i<auto slot 3>[#353  Auto] [flags 0x4 0x0 ]                                 [0x7fef2641d4d0] bci=[-1,1,12] rc=0 vc=55 vn=- li=- udi=1 nc=1
n1099n      ==>lconst 0
n4315n    BBEnd </block_448> =====

After GRA:

n4314n    BBStart <block_448> (freq 5050) (extension of previous block)                       [0x7fef2641d430] bci=[-1,1,12] rc=0 vc=2718 vn=- li=- udi=- nc=0
n4316n    lRegStore edi                                                                       [0x7fef2641d4d0] bci=[-1,1,12] rc=0 vc=2718 vn=- li=- udi=1 nc=1
n1099n      ==>lconst 0
n4315n    BBEnd </block_448> =====                                                            [0x7fef2641d480] bci=[-1,1,12] rc=0 vc=2718 vn=- li=- udi=- nc=1
n4821n      GlRegDeps ()                                                                      [0x7fef264272a0] bci=[-1,1,12] rc=1 vc=2718 vn=- li=- udi=- nc=3 flg=0x20
n4820n        PassThrough rdi                                                                 [0x7fef26427250] bci=[-1,0,12] rc=1 vc=2718 vn=- li=- udi=- nc=1
n1099n          ==>lconst 0
n4811n        ==>lRegLoad
n4812n        ==>aRegLoad

n4576n    BBStart <block_485> (freq 15050) (loop pre-header)                                  [0x7fef26422610] bci=[-1,1,12] rc=0 vc=2718 vn=- li=- udi=- nc=1
n4822n      GlRegDeps ()                                                                      [0x7fef264272f0] bci=[-1,1,12] rc=1 vc=2718 vn=- li=- udi=- nc=3 flg=0x20
n4823n        lRegLoad edi   i<auto slot 3>[#353  Auto] [flags 0x4 0x0 ] (SeenRealReference )  [0x7fef26427340] bci=[-1,29,12] rc=4 vc=2718 vn=- li=- udi=- nc=0 flg=0x8000
n4824n        lRegLoad esi   numIterations<parm 1 J>[#352  Parm] [flags 0x40000104 0x0 ] (SeenRealReference )  [0x7fef26427390] bci=[-1,3,12] rc=4 vc=2718 vn=- li=- udi=- nc=0 flg=0x8000
n4825n        aRegLoad eax   this<'this' parm Lcom/ibm/bumblebench/collections/ArrayListForEachLambdaBench;>[#351  Parm] [flags 0x40000107 0x0 ] (SeenRealReference sharedMemory )  [0x7fef264273e0] bci=[-1,8,13] rc=3 vc=2718 vn=- li=- udi=- nc=0 flg=0x8000

```

`n4316n lstore` in `block_448` used to take `0` and store `i` to stack slot `3`. After GRA, it takes `0` and store it
into the register `edi`. The value will be sent out of the block in `rdi` (`n4820n`). The next block that the `block_485`
falls into have three values in the registers. One of them associates value `i` with `edi` (`n4823n`).


## 7. Code Generation

The IL trees after optimization phase are listed under `title="Post Optimization Trees"` section. A Lower Trees pass,
detailed below, may transform certain IL tree patterns to facilitate more efficient instruction selection.
The final set of IL trees evaluated by Instruction Selection are listed under `title="Pre Instruction Selection Trees"` section.

### 7.1 Lowering Trees

```
[   496] O^O CODE GENERATION: Pwr of 2 mult opt node 00007FEF257618A0

Before Lower Trees:

n440n           lsub (highWordZero X>=0 cannotOverflow )                                      [0x7fef25761940] bci=[8,2,448] rc=1 vc=1394 vn=- li=-1 udi=- nc=2 flg=0x5100
n438n             lmul (X>=0 cannotOverflow )                                                 [0x7fef257618a0] bci=[8,2,448] rc=1 vc=1394 vn=- li=-1 udi=- nc=2 flg=0x1100
n437n               i2l (highWordZero X>=0 )                                                  [0x7fef25761850] bci=[8,2,448] rc=1 vc=1394 vn=- li=-1 udi=- nc=1 flg=0x4100
n1204n                ==>iRegLoad
n436n               lconst 4 (highWordZero X!=0 X>=0 )                                        [0x7fef25761800] bci=[8,2,448] rc=1 vc=1394 vn=- li=-1 udi=- nc=0 flg=0x4104
n439n             lconst -16 (X!=0 X<=0 )                                                     [0x7fef257618f0] bci=[8,2,448] rc=1 vc=1394 vn=- li=-1 udi=- nc=0 flg=0x204

After Lower Trees:

n438n                 lshl (X>=0 cannotOverflow )                                             [0x7fef257618a0] bci=[8,2,448] rc=1 vc=1397 vn=- li=40 udi=- nc=2 flg=0x1100
n437n                   i2l (highWordZero X>=0 )                                              [0x7fef25761850] bci=[8,2,448] rc=1 vc=1397 vn=- li=40 udi=- nc=1 flg=0x4100
n1204n                    ==>iRegLoad
n436n                   iconst 2 (Unsigned X!=0 X>=0 )                                        [0x7fef25761800] bci=[8,2,448] rc=1 vc=1397 vn=- li=40 udi=- nc=0 flg=0x4104
n439n                 lconst -16 (X!=0 X<=0 )                                                 [0x7fef257618f0] bci=[8,2,448] rc=1 vc=1397 vn=- li=40 udi=- nc=0 flg=0x204

```

The above is one example of what lowering trees does. There are all kinds of other tree lowering transformations
that make the task of generating code easier. In the above example, a constant multiplication represented by `n438n lmul`
is strength reduced to a `n438n lshl`, as shifts generally execute in fewer cycles than multiply instructions by most CPUs.

Every shift can be expressed by a multiply, but not every multiply can be expressed into a shift. In Optimization,
we canonicalize all the shifts to multiplies to increase the chance of commoning nodes. If there are a shift by `2`
and a multiply by `4` in the original program, we can convert the shift to multiply to share the common expression.
In Lowering Trees, multiply expressions are transformed into shifts as shown in the above example.

### 7.2 Instruction Selection

The first step of generating code is `“Performing Instruction Selection”`. Architecture-specific instructions are
generated to evaluate the logic represented by the corresponding IL Tree.

There are three things essential to generate code for a specific platform.
1.  Instructions for the opcode
2.  Registers
3.  Encoding (the way the instructions encoded in the generated code cache)

```
============================================================
; Live regs: GPR=2 FPR=0 VRF=0 {&GPR_0033, GPR_0032}
------------------------------
 n642n    (  0)  ificmpeq --> block_2 BBStart at n5n ()                                               [0x7fef25795860] bci=[-1,0,12] rc=0 vc=4181 vn=- li=53 udi=- nc=3 flg=0x20
 n640n    (  1)    iload  0x7fef247330cc[#479  Static] [flags 0x10303 0x8040 ] (cannotOverflow )      [0x7fef257957c0] bci=[-1,0,12] rc=1 vc=4181 vn=- li=53 udi=- nc=0 flg=0x1000
 n641n    (  1)    iconst -1 (X!=0 X<=0 )                                                             [0x7fef25795810] bci=[-1,0,12] rc=1 vc=4181 vn=- li=53 udi=- nc=0 flg=0x204
 n4813n   (  1)    GlRegDeps                                                                          [0x7fef26427020] bci=[-1,0,12] rc=1 vc=4181 vn=- li=53 udi=- nc=2
 n4811n   (  5)      ==>lRegLoad (in GPR_0032) (cannotOverflow SeenRealReference )
 n4812n   (  3)      ==>aRegLoad (in &GPR_0033)
------------------------------
------------------------------
 n642n    (  0)  ificmpeq --> block_2 BBStart at n5n ()                                               [0x7fef25795860] bci=[-1,0,12] rc=0 vc=4181 vn=- li=53 udi=- nc=3 flg=0x20
 n640n    (  0)    iload  0x7fef247330cc[#479  Static] [flags 0x10303 0x8040 ] (cannotOverflow )      [0x7fef257957c0] bci=[-1,0,12] rc=0 vc=4181 vn=- li=53 udi=- nc=0 flg=0x1000
 n641n    (  0)    iconst -1 (X!=0 X<=0 )                                                             [0x7fef25795810] bci=[-1,0,12] rc=0 vc=4181 vn=- li=53 udi=- nc=0 flg=0x204
 n4813n   (  1)    GlRegDeps                                                                          [0x7fef26427020] bci=[-1,0,12] rc=1 vc=4181 vn=- li=53 udi=- nc=2
 n4811n   (  4)      ==>lRegLoad (in GPR_0032) (cannotOverflow SeenRealReference )
 n4812n   (  2)      ==>aRegLoad (in &GPR_0033)
------------------------------

 [0x7fef12ad29f0]	cmp	dword ptr [$0x00007fef247330cc], 0xffffffffffffffff	# CMP4MemImms, SymRef  0x7fef247330cc[#627  Static] [flags 0x10303 0x8040 ]
 [0x7fef12ad2db0]	assocreg                        # assocreg
	POST: [&GPR_0033 : eax] [GPR_0032 : esi]
 [0x7fef12ad2b40]	je	Label L0016			# JE4
	 PRE: [&GPR_0033 : eax] [GPR_0032 : esi]
	POST: [&GPR_0033 : eax] [GPR_0032 : esi]
```
In the above example, the first IL trees are pre-evaluation, while the second IL trees are post-evaluation.
In the latter set, the IL nodes will be tagged with the corresponding virtual registers, if any.
The reference counts (`rc`) of the various IL nodes will also be decremented as appropriate in the second IL trees.

The generated instruction at `0x7fef12ad29f0` puts a constant value into a memory reference and compares it with `-1`.
`$0x00007fef247330cc` is the address as encoded in the memory. Depending on the comparison answer, it will jump to
`Label L0016` if it is equal. `L0016` is associated with `block_2`.

```
n635n    (  0)      iadd (in GPR_0049)                                                               [0x7fef25795630] bci=[-1,0,12] rc=0 vc=4181 vn=- li=54 udi=13952 nc=2
 n633n    (  0)        iload  0x7fef247330f4[#476  Static] [flags 0x10303 0x4040 ] (in GPR_0049) (cannotOverflow )  [0x7fef25795590] bci=[-1,0,12] rc=0 vc=4181 vn=- li=54 udi=13952 nc=0 flg=0x1000
 n634n    (  0)        iload  0x7fef24733180[#477  Static] [flags 0x10303 0x4040 ] (cannotOverflow )  [0x7fef257955e0] bci=[-1,0,12] rc=0 vc=4181 vn=- li=54 udi=1 nc=0 flg=0x1000

[0x7fef12ad3700]	mov	GPR_0049, dword ptr [$0x00007fef247330f4]		# L4RegMem, SymRef  0x7fef247330f4[#628  Static] [flags 0x10303 0x4040 ]
[0x7fef12ad38e0]	add	GPR_0049, dword ptr [$0x00007fef24733180]		# ADD4RegMem, SymRef  0x7fef24733180[#629  Static] [flags 0x10303 0x4040 ]
```

The instruction at `0x7fef12ad3700` loads `4` byte memory (`L4RegMem`) into the virtual register `GPR_0049`. The instruction
at `0x7fef12ad38e0` loads from another address `0x00007fef24733180` and adds it to `GPR_0049`.

`GPR_0049` is a virtual register created by the compiler. The step of `"Pre Instruction Selection Trees"` associates
nodes with virtual registers, which allows commoning to work. It pretends there are infinite number of virtual registers.
When a node is evaluated, it is checked where or not it is in a virtual register already. If it is, the value is picked
up from the virtual register. If it is not, the value is put into a virtual register.

```
============================================================
; Live regs: GPR=2 FPR=0 VRF=0 {&GPR_0033, GPR_0032}
------------------------------
 n4316n   (  0)  lRegStore edi                                                                        [0x7fef2641d4d0] bci=[-1,1,12] rc=0 vc=4181 vn=- li=448 udi=- nc=1
 n1099n   (  2)    ==>lconst 0 (highWordZero X==0 X>=0 X<=0 )
------------------------------
------------------------------
 n4316n   (  0)  lRegStore edi                                                                        [0x7fef2641d4d0] bci=[-1,1,12] rc=0 vc=4181 vn=- li=448 udi=- nc=1
 n1099n   (  1)    ==>lconst 0 (in GPR_0064) (highWordZero X==0 X>=0 X<=0 )
------------------------------

 [0x7fef12ad5140]	xor	GPR_0064, GPR_0064		# XOR4RegReg
```
`n1099n  (  2)` has two references. It is evaluated and put in `GPR_0064`. We do `XOR` to produce a zero in `GPR_0064`.

```
============================================================
; Live regs: GPR=6 FPR=0 VRF=0 {&GPR_0561, &GPR_0560, GPR_0545, &GPR_0544, GPR_0528, GPR_0512}
------------------------------
 n6563n   (  0)  compressedRefs                                                                       [0x7fef12731320] bci=[9,1,-] rc=0 vc=4181 vn=- li=479 udi=- nc=2
 n1858n   (  4)    l2a                                                                                [0x7fef25c7d480] bci=[9,1,-] rc=4 vc=4181 vn=- li=479 udi=- nc=1
 n6603n   (  1)      iu2l                                                                             [0x7fef12731fa0] bci=[9,1,-] rc=1 vc=4181 vn=- li=479 udi=- nc=1
 n6602n   (  1)        iloadi  com/ibm/bumblebench/collections/ArrayListForEachLambdaBench$$Lambda$44/0x0000000000000000.arg$1 Lcom/ibm/bumblebench/collections/ArrayListForEachLambdaBench;[#410  final Shadow +8] [flags 0xa0607 0x0 ]  [0x7fef12731f50] bci=[9,1,-] rc=1 vc=4181 vn=- li=479 udi=- nc=1
 n4933n   (  3)          ==>aRegLoad (in &GPR_0560) (X!=0 X>=0 SeenRealReference sharedMemory )
 n6562n   (  1)    lconst 0                                                                           [0x7fef127312d0] bci=[9,1,-] rc=1 vc=4181 vn=- li=479 udi=- nc=0
------------------------------
------------------------------
 n6563n   (  0)  compressedRefs                                                                       [0x7fef12731320] bci=[9,1,-] rc=0 vc=4181 vn=- li=479 udi=- nc=2
 n1858n   (  3)    l2a (in &GPR_0562)                                                                 [0x7fef25c7d480] bci=[9,1,-] rc=3 vc=4181 vn=- li=479 udi=42816 nc=1
 n6603n   (  0)      iu2l (in &GPR_0562)                                                              [0x7fef12731fa0] bci=[9,1,-] rc=0 vc=4181 vn=- li=479 udi=42816 nc=1
 n6602n   (  0)        iloadi  com/ibm/bumblebench/collections/ArrayListForEachLambdaBench$$Lambda$44/0x0000000000000000.arg$1 Lcom/ibm/bumblebench/collections/ArrayListForEachLambdaBench;[#410  final Shadow +8] [flags 0xa0607 0x0 ] (in &GPR_0562)  [0x7fef12731f50] bci=[9,1,-] rc=0 vc=4181 vn=- li=479 udi=42816 nc=1
 n4933n   (  2)          ==>aRegLoad (in &GPR_0560) (X!=0 X>=0 SeenRealReference sharedMemory )
 n6562n   (  0)    lconst 0                                                                           [0x7fef127312d0] bci=[9,1,-] rc=0 vc=4181 vn=- li=479 udi=- nc=0
------------------------------

 [0x7fef12b0a7c0]	mov	&GPR_0562, dword ptr [&GPR_0560+0x8]		# L4RegMem, SymRef  com/ibm/bumblebench/collections/ArrayListForEachLambdaBench$$Lambda$44/0x0000000000000000.arg$1 Lcom/ibm/bumblebench/collections/ArrayListForEachLambdaBench;[#676  final Shadow +8] [flags 0xa0607 0x0 ]

============================================================
; Live regs: GPR=7 FPR=0 VRF=0 {&GPR_0562, &GPR_0561, &GPR_0560, GPR_0545, &GPR_0544, GPR_0528, GPR_0512}
------------------------------
 n1862n   (  0)  ifacmpeq --> block_193 BBStart at n1836n ()                                          [0x7fef25c7d5c0] bci=[9,1,-] rc=0 vc=4181 vn=- li=479 udi=- nc=3 flg=0x20
 n1858n   (  3)    ==>l2a (in &GPR_0562)
 n1861n   (  1)    aconst NULL (X==0 sharedMemory )                                                   [0x7fef25c7d570] bci=[9,1,-] rc=1 vc=4181 vn=- li=479 udi=- nc=0 flg=0x2
 n4935n   (  1)    GlRegDeps ()                                                                       [0x7fef26429640] bci=[9,1,-] rc=1 vc=4181 vn=- li=479 udi=- nc=6 flg=0x20
 n4929n   (  2)      ==>iRegLoad (in GPR_0512) (SeenRealReference )
 n4930n   (  4)      ==>iRegLoad (in GPR_0528) (cannotOverflow SeenRealReference )
 n4931n   (  2)      ==>aRegLoad (in &GPR_0544) (SeenRealReference sharedMemory )
 n4932n   (  2)      ==>iRegLoad (in GPR_0545) (SeenRealReference )
 n4933n   (  2)      ==>aRegLoad (in &GPR_0560) (X!=0 X>=0 SeenRealReference sharedMemory )
 n4934n   (  1)      ==>aRegLoad (in &GPR_0561)
------------------------------
------------------------------
 n1862n   (  0)  ifacmpeq --> block_193 BBStart at n1836n ()                                          [0x7fef25c7d5c0] bci=[9,1,-] rc=0 vc=4181 vn=- li=479 udi=- nc=3 flg=0x20
 n1858n   (  2)    ==>l2a (in &GPR_0562) (X!=0 sharedMemory )
 n1861n   (  0)    aconst NULL (X==0 sharedMemory )                                                   [0x7fef25c7d570] bci=[9,1,-] rc=0 vc=4181 vn=- li=479 udi=- nc=0 flg=0x2
 n4935n   (  1)    GlRegDeps ()                                                                       [0x7fef26429640] bci=[9,1,-] rc=1 vc=4181 vn=- li=479 udi=- nc=6 flg=0x20
 n4929n   (  1)      ==>iRegLoad (in GPR_0512) (SeenRealReference )
 n4930n   (  3)      ==>iRegLoad (in GPR_0528) (cannotOverflow SeenRealReference )
 n4931n   (  1)      ==>aRegLoad (in &GPR_0544) (SeenRealReference sharedMemory )
 n4932n   (  1)      ==>iRegLoad (in GPR_0545) (SeenRealReference )
 n4933n   (  1)      ==>aRegLoad (in &GPR_0560) (X!=0 X>=0 SeenRealReference sharedMemory )
 n4934n   (  0)      ==>aRegLoad (in &GPR_0561)
------------------------------

 [0x7fef12b0abc0]	test	&GPR_0562, &GPR_0562		# TEST8RegReg
 [0x7fef12b0b080]	assocreg                        # assocreg
	POST: [&GPR_0544 : ebx] [GPR_0512 : edi] [GPR_0528 : r8d] [GPR_0545 : r9d] [&GPR_0560 : r12d] [&GPR_0561 : r13d]
 [0x7fef12b0ae10]	je	Label L0025			# JE4
	 PRE: [&GPR_0561 : r13d] [&GPR_0560 : r12d] [GPR_0545 : r9d] [&GPR_0544 : ebx] [GPR_0528 : r8d] [GPR_0512 : edi]
	POST: [&GPR_0561 : r13d] [&GPR_0560 : r12d] [GPR_0545 : r9d] [&GPR_0544 : ebx] [GPR_0528 : r8d] [GPR_0512 : edi]

…
============================================================
; Live regs: GPR=5 FPR=0 VRF=0 {&GPR_0562, GPR_0545, &GPR_0544, GPR_0528, GPR_0512}
------------------------------
 n4736n   (  0)  astore  <temp slot 8>[#522  Auto] [flags 0x4007 0x0 ]                                [0x7fef26425810] bci=[9,1,-] rc=0 vc=4181 vn=- li=356 udi=- nc=1
 n1858n   (  2)    ==>l2a (in &GPR_0562) (X!=0 sharedMemory )
------------------------------
------------------------------
 n4736n   (  0)  astore  <temp slot 8>[#522  Auto] [flags 0x4007 0x0 ]                                [0x7fef26425810] bci=[9,1,-] rc=0 vc=4181 vn=- li=356 udi=- nc=1
 n1858n   (  1)    ==>l2a (in &GPR_0562) (X!=0 sharedMemory )
------------------------------

 [0x7fef12b0bdd0]	mov	qword ptr [vfp], &GPR_0562		# S8MemReg, SymRef  <temp slot 8>[#678  Auto] [flags 0x4007 0x0 ]
…
```

In the above example,
- `n1858n l2a` is evaluated and the value is stored in `&GPR_0562` in the instruction at `0x7fef12b0a7c0`.
- `GPR_0562` is used directly for the `NULL` test in `n1862n ifacmpeq` in the instruction at `0x7fef12b0abc0`.
- `GPR_0562` is used again in `n4736n astore` to store the value to stack temp slot `8` in the instruction at
`0x7fef12b0bdd0`. `vfp` stands for Virtual Frame Pointer. It is used because we have not mapped stack slots.
We don’t have offset associated with each auto slot yet.

`&` means the virtual register is a collected reference. A reference typed node in the IL must be marked as
"collected" if it could result in a value that is on the Java heap. It implies that the GC needs to be notified
about it if the value is live across a GC point. Note that collected reference may be held live within a single
BB and yet be live across a GC point (since a single block can contain any number of GC points).

When the "Collected References" in the IL are associated with a virtual register or stack slot in the codegen,
they are in turn considered collected. This information eventually gets used to emit GC stack and register maps at the
GC points notifying the GC about live objects as well as reference slots/registers needing to be updated if the object is moved.


### 7.3 Examples Of Sections After Instruction Selection

The following are some examples of transformations that happen after Instruction Selection.
For example for the instruction at `0x7fef12b0bdd0` from the above example:

```
[0x7fef12b0bdd0]	mov	qword ptr [vfp], rcx		# S8MemReg, SymRef  <temp slot 8>[#678  Auto] [flags 0x4007 0x0 ]
```
`"Post Register Assignment Instructions"`: `GPR_0562` is assigned with the real register `rcx` in
the instruction at `0x7fef12b0bdd0`.

```
[0x7fef12b0bdd0]	mov	qword ptr [vfp-0x80], rcx		# S8MemReg, SymRef  <temp slot 8>[#678  Auto] [flags 0x4007 0x0 ]
```
`"Post Stack Map"`: Stack slot temp `8` is mapped to offset `-0x80` in the instruction at `0x7fef12b0bdd0`.

```
[0x7fef12b0bdd0]	mov	qword ptr [rsp+0xd8], rcx		# S8MemReg, SymRef  <temp slot 8>[#678  Auto +216] [flags 0x4007 0x0 ]
```
`"VFP Substitution"`: The instruction at `0x7fef12b0bdd0` populates the stack pointer `rsp`. This is the instruction
that looks more or less what is going into the code cache.

```
0x7fef13a74929 00000229 [0x7fef12b0bdd0] 48 89 8c 24 d8 00 00 00            mov	qword ptr [rsp+0xd8], rcx		# S8MemReg, SymRef  <temp slot 8>[#678  Auto +216] [flags 0x4007 0x0 ]
```
`"Post Binary Instructions"`: The final binary encoding at `0x7fef13a74929`.

### 7.4 Precondition and Postondition

```
; Live regs: GPR=3 FPR=0 VRF=0 {&GPR_1441, &GPR_1440, GPR_1424}
------------------------------
 n430n    (  0)  treetop ()                                                                           [0x7fef25791620] bci=[6,43,1541] rc=0 vc=4181 vn=- li=34 udi=- nc=1 flg=0x8
 n349n    ( 12)    acall  java/util/ArrayList.elementAt([Ljava/lang/Object;I)Ljava/lang/Object;[#430  static Method] [flags 0x500 0x0 ] (X>=0 virtualCallNodeForAGuardedInlinedCall sharedMemory )  [0x7fef2578fcd0] bci=[6,43,1541] rc=12 vc=4181 vn=- li=34 udi=- nc=2 flg=0x908
 n5121n   (  8)      ==>aRegLoad (in &GPR_1441) (X!=0 SeenRealReference sharedMemory )
 n5119n   (  8)      ==>iRegLoad (in GPR_1424) (cannotOverflow SeenRealReference )
------------------------------
------------------------------
 n430n    (  0)  treetop ()                                                                           [0x7fef25791620] bci=[6,43,1541] rc=0 vc=4181 vn=- li=34 udi=- nc=1 flg=0x8
 n349n    ( 11)    acall  java/util/ArrayList.elementAt([Ljava/lang/Object;I)Ljava/lang/Object;[#430  static Method] [flags 0x500 0x0 ] (in &GPR_1469) (X>=0 virtualCallNodeForAGuardedInlinedCall sharedMemory )  [0x7fef2578fcd0] bci=[6,43,1541] rc=11 vc=4181 vn=- li=34 udi=64144 nc=2 flg=0x908
 n5121n   (  7)      ==>aRegLoad (in &GPR_1441) (X!=0 SeenRealReference sharedMemory )
 n5119n   (  7)      ==>iRegLoad (in GPR_1424) (cannotOverflow SeenRealReference )
------------------------------

 [0x7fef12b4f170]	mov	&GPR_1456, &GPR_1441		# MOV8RegReg
 [0x7fef12b4f280]	mov	GPR_1457, GPR_1424		# MOV4RegReg
 [0x7fef12b4fb10]	Label L0768:			# label	# (Start of internal control flow)
	 PRE: [&GPR_1456 : eax] [GPR_1457 : esi]
 [0x7fef12b4f470]	nop			# Align patchable code @32 [0x0:5]
 [0x7fef12b4f3d0]	call	java/util/ArrayList.elementAt([Ljava/lang/Object;I)Ljava/lang/Object;# CALLImm4 (00007FEF13A6CF89)# CALLImm4
 [0x7fef12b4fe20]	assocreg                        # assocreg
	POST: [GPR_1424 : r8d] [&GPR_1440 : r12d] [&GPR_1441 : r13d]
 [0x7fef12b4fbb0]	Label L0769:			# label	# (End of internal control flow)
	 PRE: [&GPR_1456 : eax] [GPR_1457 : esi]
	POST: [D_GPR_1458 : ecx] [D_GPR_1459 : edx] [D_GPR_1460 : edi] [D_GPR_1461 : esi] [D_GPR_1462 : r8d] [D_GPR_1463 : r10d] [D_GPR_1464 : r11d] [D_GPR_1465 : r12d] [D_GPR_1466 : r13d] [D_GPR_1467 : r14d] [D_GPR_1468 : r15d] [GPR_0000 : ebp] [&GPR_1469 : eax]
```

The two parameters for call `java/util/ArrayList.elementAt([Ljava/lang/Object;I)Ljava/lang/Object;` are passed from
`GPR_1441` to `GPR_1456`, and `GPR_1424` to `GPR_1457`.

```
PRE: [&GPR_1456 : eax] [GPR_1457 : esi]
```
Precondition: `GPR_1456` is associated with real register `eax`; `GPR_1457` is associated with real register `esi`
because of the linkage convention. The linkage convention on the platform requires the first argument should be in `eax`
and the second argument should be `esi`. It tells register allocator to make sure the values are in the right real registers.

```
POST: [GPR_1424 : r8d] [&GPR_1440 : r12d] [&GPR_1441 : r13d]
```
Postcondition: Associates the virtual registers with the right real registers for return values or other dependencies
that need to be obeyed.

### 7.5 An Example On Generated Code For `checkcast`

```
============================================================
; Live regs: GPR=9 FPR=0 VRF=0 {&GPR_5073, &GPR_5072, GPR_5056, GPR_5043, &GPR_5042, &GPR_5041, GPR_5040, GPR_5025, &GPR_5024}
------------------------------
 n2032n   (  0)  checkcast [#86]                                                                      [0x7fef25c80ae0] bci=[15,5,-] rc=0 vc=4181 vn=- li=217 udi=- nc=2
 n5851n   (  2)    ==>aRegLoad (in &GPR_5024) (SeenRealReference sharedMemory )
 n2034n   (  1)    loadaddr  java/lang/Integer[#501  Static] [flags 0x18307 0x0 ]                     [0x7fef25c80b80] bci=[15,5,-] rc=1 vc=4181 vn=- li=217 udi=- nc=0
------------------------------
------------------------------
 n2032n   (  0)  checkcast [#86]                                                                      [0x7fef25c80ae0] bci=[15,5,-] rc=0 vc=4181 vn=- li=217 udi=- nc=2
 n5851n   (  1)    ==>aRegLoad (in &GPR_5024) (SeenRealReference sharedMemory )
 n2034n   (  0)    loadaddr  java/lang/Integer[#501  Static] [flags 0x18307 0x0 ]                     [0x7fef25c80b80] bci=[15,5,-] rc=0 vc=4181 vn=- li=217 udi=- nc=0
------------------------------

 [0x7fef12c4e440]	mov	GPR_5088, &GPR_5024		# MOV8RegReg
 [0x7fef12c4e4d0]	Label L2064:			# label	# (Start of internal control flow)
 [0x7fef12c4e570]	test	GPR_5088, GPR_5088		# TEST8RegReg
 [0x7fef12c4e600]	je	Label L2065			# JE4
 [0x7fef12c4e730]	mov	GPR_5088, dword ptr [GPR_5088]		# L4RegMem
 [0x7fef12c4e7c0]	and	GPR_5088, 0xffffffffffffff00	# AND4RegImm4
 [0x7fef12c4e850]	cmp	GPR_5088, 0x000a4100	# CMP4RegImm4
 [0x7fef12c4e8e0]	jne	Outlined Label L2066			# JNE4
 [0x7fef12c4efc0]	assocreg                        # assocreg
	POST: [&GPR_5041 : eax] [&GPR_5042 : ebx] [&GPR_5024 : edi] [GPR_5040 : esi] [GPR_5025 : r8d] [GPR_5043 : r9d] [GPR_5056 : r10d] [&GPR_5072 : r11d] [&GPR_5073 : r13d]
 [0x7fef12c4ed50]	Label L2065:			# label	# (End of internal control flow)
	 PRE: [GPR_5089 : NoReg] [GPR_5088 : NoReg]
	POST: [GPR_5089 : NoReg] [GPR_5088 : NoReg]
```

The above is an example of the generated code for `n2032n checkcast` node. JIT optimizes the common case when the
checkcast exception will not be thrown.

Instructions at `0x7fef12c4e570` and `0x7fef12c4e600` check if the value is `NULL`. If it is not `NULL`, it will get the
class pointer and mask out the lower `8` bits and does a comparison to the class (`0x000a4100`) that is casting to. If
it is equal, the checkcast succeeds. If it is not equal, it fails and throw the exception. Throwing the exception is
not handled here. We jump out of [mainline to the out-of-line](https://github.com/eclipse/omr/blob/6eec759cd2d446f74d2b8a7ee3348d98ce6bfa79/doc/compiler/il/MainlineAndOutOfLineCode.md) `Label L2066`. We do not pollute the code in the
mainline which is executed more often.

```
------------ start out-of-line instructions

 [0x7fef12c4e9f0]	Outlined Label L2066:			# label
 [0x7fef12c4ea90]	push	GPR_5088			# PUSHReg
 [0x7fef12c4eb20]	push	0x000a4100		# PUSHImm4
 [0x7fef12c4ebb0]	call	jitThrowClassCastException# CALLImm4 (00007FEF27D989D0)# CALLImm4
 [0x7fef12c4ecb0]	Label L2067:			# label
------------ end out-of-line instructions
```
The out-of-line code is at the bottom of the code. It pushes the arguments that are required for the VM helper call
`jitThrowClassCastException`. The VM helper call takes care of throwing the exception.

### 7.6 An Example On Register Spilling

```
[0x7fef12bbd320]	Label L0052:			# label
 [0x7fef12ced790]	mov	qword ptr [vfp], rsi		# S8MemReg, #SPILL8, SymRef  <#SPILL8_1133 0x7fef12cac3f0>[#1133  Auto] [flags 0x80000000 0x0 ]
 [0x7fef12ced630]	mov	qword ptr [vfp], rax		# S8MemReg, #SPILL8, SymRef  <#SPILL8_1132 0x7fef12cabf70>[#1132  Auto] [flags 0x80000000 0x0 ]
 [0x7fef12ced4d0]	mov	qword ptr [vfp], r10		# S8MemReg, #SPILL8, SymRef  <#SPILL8_1131 0x7fef12cac870>[#1131  Auto] [flags 0x80000000 0x0 ]
 [0x7fef12ced370]	mov	qword ptr [vfp], r12		# S8MemReg, #SPILL8, SymRef  <#SPILL8_1130 0x7fef12ca8980>[#1130  Auto] [flags 0x80000000 0x0 ]
========================================
 [0x7fef12bbd610]	fence Relative [ 00007FEF25C302C0 ]	# fence BBStart <block_166> (frequency 1) (cold) (in loop 176)
 [0x7fef12bbe060]	inc	dword ptr [$0x00007fef24733168]		# INC4Mem, SymRef  0x7fef24733168[#866  Static] [flags 0x10303 0x4040 ]
 [0x7fef12bbeaf0]	mov	rax, r13		# MOV8RegReg
 [0x7fef12ced1d0]	mov	qword ptr [vfp], r13		# S8MemReg, #SPILL8, SymRef  <#SPILL8_1129 0x7fef12cac630>[#1129  Auto] [flags 0x80000000 0x0 ]
 [0x7fef12bbec00]	mov	esi, r8d		# MOV4RegReg
 [0x7fef12ced070]	mov	qword ptr [vfp], r8		# S8MemReg, #SPILL8, SymRef  <#SPILL8_1128 0x7fef12cac1b0>[#1128  Auto] [flags 0x80000000 0x0 ]
 [0x7fef12bbf490]	Label L1440:			# label	# (Start of internal control flow)
 [0x7fef12bbedf0]	nop			# Align patchable code @32 [0x0:5]
 [0x7fef12bbed50]	call	java/util/ArrayList.elementAt([Ljava/lang/Object;I)Ljava/lang/Object;# CALLImm4 (00007FEF13A6CF89)# CALLImm4
 [0x7fef12bbf7a0]	assocreg                        # assocreg
 [0x7fef12bbf530]	Label L1441:			# label	# (End of internal control flow)
 [0x7fef12cece70]	mov	r13, qword ptr [vfp]		# L8RegMem, #SPILL8, SymRef  <#SPILL8_1127 0x7fef12cac630>[#1127  Auto] [flags 0x80000000 0x0 ]#, spilled for acall
 [0x7fef12cecd10]	mov	r12, qword ptr [vfp]		# L8RegMem, #SPILL8, SymRef  <#SPILL8_1126 0x7fef12ca8980>[#1126  Auto] [flags 0x80000000 0x0 ]#, spilled for acall
 [0x7fef12cecbb0]	mov	r10, qword ptr [vfp]		# L8RegMem, #SPILL8, SymRef  <#SPILL8_1125 0x7fef12cac870>[#1125  Auto] [flags 0x80000000 0x0 ]#, spilled for acall
 [0x7fef12ceca50]	mov	r8, qword ptr [vfp]		# L8RegMem, #SPILL8, SymRef  <#SPILL8_1124 0x7fef12cac1b0>[#1124  Auto] [flags 0x80000000 0x0 ]#, spilled for acall
 [0x7fef12cec8f0]	mov	rsi, qword ptr [vfp]		# L8RegMem, #SPILL8, SymRef  <#SPILL8_1123 0x7fef12cac3f0>[#1123  Auto] [flags 0x80000000 0x0 ]#, spilled for acall
 [0x7fef12cec7d0]	mov	rdi, rax		# MOV8RegReg
 [0x7fef12cec700]	mov	rax, qword ptr [vfp]		# L8RegMem, #SPILL8, SymRef  <#SPILL8_1122 0x7fef12cabf70>[#1122  Auto] [flags 0x80000000 0x0 ]#, spilled for acall
```
Register spill around a call at `0x7fef12bbed50`. The spills are added by the register assigner. Sometimes it cannot
satisfy all the values that need to be kept alive at a particular program point given the constraints of an architecture.
Sometimes there are more virtual registers than the number of real registers that are available. It will spill some of
the values into the memory stack slots. Once in a better state, after the call is done, the values are restored from
the memory into the register. Certain registers are set aside as being preserved before the call.

One of the things on debugging register allocator problem is to look at if there is a lot of spilling. If there is,
exam closely and see if there is an issue of save and restore registers. One possibility could be that the restore is
not done correctly on one of the paths, for example missing a restore, or restoring a wrong register, or a wrong value
is used.

### 7.7 Post Binary Instructions

```
                 +--------------------------------------- instruction address
                 |        +----------------------------------------- instruction offset from start of method
                 |        |                   +------------------------------------------ corresponding TR::Instruction instance
                 |        |                   |  +-------------------------------------------------- code bytes
                 |        |                   |  |                          +-------------------------------------- opcode and operands
                 |        |                   |  |                          |				+----------- additional information
                 |        |                   |  |                          |				|
                 V        V                   V  V                          V				V
0x7fef13a746d0 ffffffd0 [0x7fef12ad0bf0]                                    Label L0084:			# label
0x7fef13a746d0 ffffffd0 [0x7fef12ad0d80] 48 bf 78 2c 34 00 00 00 00 00      mov	rdi, 0x0000000000342c78	# MOV8RegImm64

0x7fef13a74700 00000000 [0x7fef12d881e0] 48 8b 44 24 18                     mov	rax, qword ptr [rsp+0x18]		# L8RegMem, SymRef [#1164 +24]
```

`"Post Binary Instructions"` is the last thing in the JIT compilation.

`0x7fef13a74700`: the code cache address or instruction address where the bytecode instruction is.

`00000000`: instruction offset from start of method.

`0x7fef12ad1d50`: TR instruction address.

`48 8b 44 24 18`: bytes that represents the instruction.

The codegen also generates prologue and epilogue. Linkage conventions may require managements of incoming parameters
and return values, adjustments of stack pointer to allocate/deallocate a call frame, saving/restoring preserved
register values, checks for stack overflow, etc.

### 7.8 GC Maps

We keep track of every point that could result in GC. Every local variable is searchable. All local variables have GC
map index. The ones that are collected references (collected autos, collected register, and collected parms) have
positive or non-negative GC map index.

```
Internal stack atlas:
  numberOfMaps=92
  numberOfSlotsMapped=23
  numberOfParmSlots=1
  parmBaseOffset=24
  localBaseOffset=-176

  Locals information :
  Local [0x7fef256df590] (GC map index :   0, Offset :  24, Size : 8) is an uninitialized collected parm
  Local [0x7fef256df7c0] (GC map index :  -1, Offset :   8, Size : 8) is an uninitialized uncollected parm
  Local [0x7fef12cac870] (GC map index :  17, Offset : -48, Size : 8) is an uninitialized uncollected auto
  Local [0x7fef12cac630] (GC map index :  16, Offset : -56, Size : 8) is an uninitialized uncollected auto
  Local [0x7fef12cac3f0] (GC map index :  20, Offset : -24, Size : 8) is an uninitialized uncollected auto
…
  Local [0x7fef262476b0] (GC map index :  -1, Offset : -288, Size : 4) is an uninitialized uncollected auto
```
GC maps describe all live autos/parms/registers that contain potential heap object references at the given point in
the program. GC maps are associated with instructions, or more correctly, ranges of instructions. When a GC occurs,
the address of the current instruction (i.e. the address in the program counter register) is used to search for the
appropriate map in the GC atlas of the method. The addresses of the heap object references are fixed up one by one
by calling a GC routine that computes the displacement to apply for each address.

```
  Map number : 2
  Code offset range starts at [00001885]
  GC stack map information :
    number of stack slots mapped = 21
    live stack slots containing addresses --> {0,5}
  GC register map information :
slot pushes: 0    registers: {eax }
```
Different GC maps are generated for different GC points. In the above example, for GC `Map 2`, there are `21` collected autos.
Out of the `21` autos, stack slot index `0` and `5` are live at this point. Register `eax` holds collected references.
At the runtime, the collected references are referred as O-Slots in Stack Walker output. During a stack walk, the stack slot
indices are used to map to the specific stack slots that will be checked for collected references.
