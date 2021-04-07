<!--
Copyright (c) 2021, 2021 IBM Corp. and others

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

# Global Register Dependency

`GlRegDeps` is an IL level construct that models code generator register dependencies. It associates virtual registers with real registers, or pairs virtual registers with real registers.

Register dependencies enforce constraints. They represent that a particular value (IL node) has to be in a particular real register at a particular point in the program. These particular points in the program are usually entry or exit corresponding to basic blocks or extended basic blocks where there is control flow. Fundamentally we can think of global register dependencies as a way of conveying information within real (hardware) registers across block boundaries.

Most common control flow opcodes are: `if`, `goto`, `return`, `athrow`, `switch`. If present, they must be the last tree before the `BBEnd`. They should have their own `GlRegDeps` for the branch target if required. The `GlRegDeps` under `BBEnd` is for the fall through case. It is important to make sure the right value is placed in the right real register at the point where the `GlRegDeps` is evaluated. The `GlRegDeps` with `xRegLoads` are created for `BBStart` that must be consistent with the GlRegDeps stated on all the predecessor blocks.

If a value that is expected to be in a register is not in the register, or the register is not preserved if it is a call, it leads to register shuffling. The local register allocator has to shuffle it to a different register to make sure it has the right value in `GlRegDeps`. Register shuffling does come at a cost. It should be avoided when possible.

Reference counts are a general concept that is used within the codegen for nodes and their associated virtual registers. Global Register Allocation (`GRA`) related opcodes such as `GlRegDeps` use reference counts to determine when a register is no longer needed. It must be set up correctly, otherwise it could result in registers being overwritten with some other values prematurely, or the value being kept in a register longer than necessary. In the following example, `n190n` is referenced `5` times but its reference count is set up incorrectly as `6` (`rc=6`).
```
n541n     treetop                                                                             [0x7f66870b98d0] bci=[1,29,-] rc=0 vc=0 vn=- li=- udi=- nc=1
n190n       iand (X>=0 cannotOverflow )                                                       [0x7f66870b2b20] bci=[1,29,-] rc=6 vc=1095 vn=- li=- udi=- nc=2 flg=0x1100
n185n         iadd (X>=0 cannotOverflow )                                                     [0x7f66870b2990] bci=[1,21,-] rc=1 vc=1095 vn=- li=- udi=- nc=2 flg=0x1100
n183n           iload  n<auto slot 5>[#370  Auto] [flags 0x3 0x0 ] (X>=0 cannotOverflow )     [0x7f66870b28f0] bci=[1,18,-] rc=1 vc=1095 vn=- li=- udi=34 nc=0 flg=0x1100
n184n           iconst -1 (X!=0 X<=0 )                                                        [0x7f66870b2940] bci=[1,20,-] rc=1 vc=1095 vn=- li=- udi=- nc=0 flg=0x204
n505n         ==>iRegLoad
n542n     treetop                                                                             [0x7f66870b9920] bci=[1,17,-] rc=0 vc=0 vn=- li=- udi=- nc=1
n506n       ==>aRegLoad
n561n     aRegStore ecx                                                                       [0x7f66870b9f10] bci=[-1,0,-] rc=0 vc=0 vn=- li=- udi=- nc=1
n560n       aconst NULL                                                                       [0x7f66870b9ec0] bci=[-1,0,-] rc=1 vc=0 vn=- li=- udi=- nc=0
n568n     iRegStore edx                                                                       [0x7f66870ba140] bci=[1,29,-] rc=0 vc=0 vn=- li=- udi=- nc=1
n190n       ==>iand
n584n     ificmpne --> block_57 BBStart at n563n ()                                           [0x7f66870ba640] bci=[1,30,-] rc=0 vc=0 vn=- li=- udi=- nc=3 flg=0x20
n582n       iand                                                                              [0x7f66870ba5a0] bci=[1,30,-] rc=1 vc=0 vn=- li=- udi=- nc=2
n580n         iloadi  <isClassFlags>[#287  Shadow +36] [flags 0x603 0x0 ]                     [0x7f66870ba500] bci=[1,30,-] rc=1 vc=0 vn=- li=- udi=- nc=1
n579n           aloadi  <componentClass>[#282  Shadow +88] [flags 0x10607 0x0 ]               [0x7f66870ba4b0] bci=[1,30,-] rc=1 vc=0 vn=- li=- udi=- nc=1
n578n             aloadi  <vft-symbol>[#288  Shadow] [flags 0x18607 0x0 ]                     [0x7f66870ba460] bci=[1,30,-] rc=1 vc=0 vn=- li=- udi=- nc=1
n506n               ==>aRegLoad
n581n         iconst 1024                                                                     [0x7f66870ba550] bci=[1,30,-] rc=1 vc=0 vn=- li=- udi=- nc=0
n583n       iconst 0                                                                          [0x7f66870ba5f0] bci=[1,30,-] rc=1 vc=0 vn=- li=- udi=- nc=0
n585n       GlRegDeps ()                                                                      [0x7f66870ba690] bci=[1,30,-] rc=1 vc=0 vn=- li=- udi=- nc=3 flg=0x20
n506n         ==>aRegLoad
n505n         ==>iRegLoad
n586n         PassThrough rdx                                                                 [0x7f66870ba6e0] bci=[1,29,-] rc=1 vc=0 vn=- li=- udi=- nc=1
n190n           ==>iand
n574n     BBEnd </block_33>                                                                   [0x7f66870ba320] bci=[1,30,-] rc=0 vc=0 vn=- li=- udi=- nc=0
n573n     BBStart <block_58> (freq 2226) (extension of previous block)                        [0x7f66870ba2d0] bci=[1,30,-] rc=0 vc=0 vn=- li=- udi=- nc=0
n576n     NULLCHK on n506n [#32]                                                              [0x7f66870ba3c0] bci=[1,17,-] rc=0 vc=0 vn=- li=- udi=- nc=1
n575n       arraylength (stride 8)                                                            [0x7f66870ba370] bci=[1,17,-] rc=2 vc=0 vn=- li=- udi=- nc=1
n506n         ==>aRegLoad
n577n     BNDCHK [#1]                                                                         [0x7f66870ba410] bci=[1,17,-] rc=0 vc=0 vn=- li=- udi=- nc=2
n575n       ==>arraylength
n190n       ==>iand
n550n     treetop                                                                             [0x7f66870b9ba0] bci=[1,30,-] rc=0 vc=0 vn=- li=- udi=- nc=1
n549n       aloadi  <array-shadow>[#229  Shadow] [flags 0x80000607 0x0 ]                      [0x7f66870b9b50] bci=[1,30,-] rc=3 vc=0 vn=- li=- udi=- nc=1
n548n         aladd                                                                           [0x7f66870b9b00] bci=[1,17,-] rc=1 vc=0 vn=- li=- udi=- nc=2
n506n           ==>aRegLoad
n547n           ladd (X>=0 )                                                                  [0x7f66870b9ab0] bci=[1,29,-] rc=1 vc=0 vn=- li=- udi=- nc=2 flg=0x100
n545n             lshl                                                                        [0x7f66870b9a10] bci=[1,29,-] rc=1 vc=0 vn=- li=- udi=- nc=2
n543n               i2l                                                                       [0x7f66870b9970] bci=[1,29,-] rc=1 vc=0 vn=- li=- udi=- nc=1
n190n                 ==>iand
n544n               iconst 3                                                                  [0x7f66870b99c0] bci=[-1,0,-] rc=1 vc=0 vn=- li=- udi=- nc=0
n546n             lconst 24                                                                   [0x7f66870b9a60] bci=[-1,0,-] rc=1 vc=0 vn=- li=- udi=- nc=0
n572n     aRegStore ecx                                                                       [0x7f66870ba280] bci=[1,30,-] rc=0 vc=0 vn=- li=- udi=- nc=1
n549n       ==>aloadi
n564n     BBEnd </block_58> ===== ()                                                          [0x7f66870ba000] bci=[1,30,-] rc=0 vc=0 vn=- li=- udi=- nc=1 flg=0x20
n562n       GlRegDeps ()                                                                      [0x7f66870b9f60] bci=[1,30,-] rc=1 vc=0 vn=- li=- udi=- nc=3 flg=0x20
n506n         ==>aRegLoad
n505n         ==>iRegLoad
n587n         PassThrough rcx                                                                 [0x7f66870ba730] bci=[1,30,-] rc=1 vc=0 vn=- li=- udi=- nc=1
n549n           ==>aloadi

```

The children of the `GlRegDeps` node that represent new values and are assigned to new global registers in the extended block must have been evaluated before `GlRegDeps`. The job of `GlRegDeps` is to take values that are already in virtual registers and simply map them to real registers. `GlRegDeps` is only about generating the register dependency. It should not be about generating any other instructions.

The only other instruction that a `GlRegDeps` evaluation can result in is a register-register move when the same value (same IL node) goes out in two (or more) different global registers. In such a case the same virtual register cannot be mapped to two (or more) real registers in the codegen register dependency since this is a condition that cannot be satisfied. By adding a new virtual register and inserting a register-register copy instruction, the codegen register dependency can use different virtual/real registers. Note that a register-register copy instruction does not result in any arithmetic or setting of control registers. Therefore, it is a safe instruction to execute between the compare and the branch for an `if` opcode where the `GlRegDeps` is present.

The following example of `GlRegDeps` will cause issues. It does the `aload` operations (`n111n` and `n106n`) and the `add` operation (`n110n`) in the middle of laying down register dependencies. These actual tree evaluations that arise due to the register dependencies could end up interfering with branching.

```
n803n     ificmpne --> block_44 BBStart at n781n ()                                           [0x7f9d0f08eab0] bci=[-1,119,600] rc=0 vc=1295 vn=- li=-1 udi=- nc=3 flg=0x20
n801n       iand                                                                              [0x7f9d0f08ea10] bci=[-1,119,600] rc=1 vc=1295 vn=- li=- udi=- nc=2
n799n         iloadi  <isClassFlags>[#287  Shadow +36] [flags 0x603 0x0 ]                     [0x7f9d0f08e970] bci=[-1,119,600] rc=1 vc=1295 vn=- li=- udi=- nc=1
n798n           aloadi  <componentClass>[#282  Shadow +88] [flags 0x10607 0x0 ]               [0x7f9d0f08e920] bci=[-1,119,600] rc=1 vc=1295 vn=- li=- udi=- nc=1
n797n             aloadi  <vft-symbol>[#288  Shadow] [flags 0x18607 0x0 ]                     [0x7f9d0f08e8d0] bci=[-1,119,600] rc=1 vc=1295 vn=- li=- udi=- nc=1
n106n               ==>aload
n800n         iconst 1024                                                                     [0x7f9d0f08e9c0] bci=[-1,119,600] rc=1 vc=1295 vn=- li=- udi=- nc=0
n802n       iconst 0                                                                          [0x7f9d0f08ea60] bci=[-1,119,600] rc=1 vc=1295 vn=- li=- udi=- nc=0
n804n       GlRegDeps ()                                                                      [0x7f9d0f08eb00] bci=[-1,111,600] rc=1 vc=1295 vn=- li=- udi=- nc=3 flg=0x20
n805n         PassThrough rcx                                                                 [0x7f9d0f08eb50] bci=[-1,117,600] rc=1 vc=1295 vn=- li=- udi=- nc=1
n111n           aload  tmp<auto slot 4>[#358  Auto] [flags 0x7 0x0 ]                          [0x7f9d0f081270] bci=[-1,117,600] rc=6 vc=1295 vn=- li=1 udi=70 nc=0
n806n         PassThrough r8                                                                  [0x7f9d0f08eba0] bci=[-1,116,600] rc=1 vc=1295 vn=- li=- udi=- nc=1
n110n           iadd (cannotOverflow )                                                        [0x7f9d0f081220] bci=[-1,116,600] rc=5 vc=1295 vn=- li=1 udi=- nc=2 flg=0x1000
n107n             ==>arraylength
n109n             iconst -1 (X!=0 X<=0 )                                                      [0x7f9d0f0811d0] bci=[-1,115,600] rc=1 vc=1295 vn=- li=- udi=- nc=0 flg=0x204
n807n         PassThrough rdi                                                                 [0x7f9d0f08ebf0] bci=[-1,111,600] rc=1 vc=1295 vn=- li=- udi=- nc=1
n106n           ==>aload
n796n     BBEnd </block_13>                                                                   [0x7f9d0f08e880] bci=[-1,119,600] rc=0 vc=1295 vn=- li=-1 udi=- nc=0
```

These nodes must be anchored under `xRegStore` earlier in the block. By anchoring these nodes earlier in the block, it ensures that they get evaluated earlier than `GlRegDeps` so that it does not interfere with the control flow. For example, `n106n` is now anchored under `aRegStore edi`. `n111n` is anchored under `aRegStore ecx`. `n110n` is anchored under `iRegStore r8d`.
```
n835n     aRegStore edi                                                                       [0x7fe8a708f4b0] bci=[-1,111,600] rc=0 vc=1295 vn=- li=-1 udi=- nc=1
n106n       ==>aload
n838n     iRegStore r8d                                                                       [0x7fe8a708f5a0] bci=[-1,116,600] rc=0 vc=1295 vn=- li=-1 udi=- nc=1
n110n       iadd (cannotOverflow )                                                            [0x7fe8a7081220] bci=[-1,116,600] rc=4 vc=1295 vn=- li=1 udi=- nc=2 flg=0x1000
n107n         ==>arraylength
n109n         iconst -1 (X!=0 X<=0 )                                                          [0x7fe8a70811d0] bci=[-1,115,600] rc=1 vc=1295 vn=- li=- udi=- nc=0 flg=0x204
n841n     aRegStore ecx                                                                       [0x7fe8a708f690] bci=[-1,117,600] rc=0 vc=1295 vn=- li=-1 udi=- nc=1
n111n       aload  tmp<auto slot 4>[#358  Auto] [flags 0x7 0x0 ]                              [0x7fe8a7081270] bci=[-1,117,600] rc=3 vc=1295 vn=- li=1 udi=70 nc=0
n866n     ificmpne --> block_48 BBStart at n832n ()                                           [0x7fe8a708fe60] bci=[-1,119,600] rc=0 vc=1295 vn=- li=-1 udi=- nc=3 flg=0x20
n864n       iand                                                                              [0x7fe8a708fdc0] bci=[-1,119,600] rc=1 vc=1295 vn=- li=- udi=- nc=2
n862n         iloadi  <isClassFlags>[#287  Shadow +36] [flags 0x603 0x0 ]                     [0x7fe8a708fd20] bci=[-1,119,600] rc=1 vc=1295 vn=- li=- udi=- nc=1
n861n           aloadi  <componentClass>[#282  Shadow +88] [flags 0x10607 0x0 ]               [0x7fe8a708fcd0] bci=[-1,119,600] rc=1 vc=1295 vn=- li=- udi=- nc=1
n860n             aloadi  <vft-symbol>[#288  Shadow] [flags 0x18607 0x0 ]                     [0x7fe8a708fc80] bci=[-1,119,600] rc=1 vc=1295 vn=- li=- udi=- nc=1
n106n               ==>aload
n863n         iconst 1024                                                                     [0x7fe8a708fd70] bci=[-1,119,600] rc=1 vc=1295 vn=- li=- udi=- nc=0
n865n       iconst 0                                                                          [0x7fe8a708fe10] bci=[-1,119,600] rc=1 vc=1295 vn=- li=- udi=- nc=0
n867n       GlRegDeps ()                                                                      [0x7fe8a708feb0] bci=[-1,119,600] rc=1 vc=1295 vn=- li=- udi=- nc=3 flg=0x20
n868n         PassThrough rcx                                                                 [0x7fe8a708ff00] bci=[-1,117,600] rc=1 vc=1295 vn=- li=- udi=- nc=1
n111n           ==>aload
n869n         PassThrough r8                                                                  [0x7fe8a708ff50] bci=[-1,116,600] rc=1 vc=1295 vn=- li=- udi=- nc=1
n110n           ==>iadd
n870n         PassThrough rdi                                                                 [0x7fe8a708ffa0] bci=[-1,111,600] rc=1 vc=1295 vn=- li=- udi=- nc=1
n106n           ==>aload
n854n     BBEnd </block_13>
```

The `xRegStore` serves the purpose of anchoring those children that are honored and ensuring that they get evaluated before the `GlRegDeps` gets encountered.

However, the `xRegStore` does not mandate the value has to be in the right real register at the point of `xRegStore`. It is not responsible for putting the value into the register. It is not actually going to do anything at that point other than evaluates the child and associates it with a virtual register.

The actual move of that value into the right real register is dictated by `GlRegDeps` at the point of transitioning from one block to another. The local register allocator in the codegen happens backwards. It walks backwards through the instructions. It will see `GlRegDeps` first, or the result of `GlRegDeps` first. It associates the virtual register with the real register the first time.


# Check IL Rules
The IL rules can be checked by enabling `TR_EnableParanoidOptCheck`, which does IL validation, tree verification, block and CFG verification at the end of the optimization to catch structural problems at the compile time.
