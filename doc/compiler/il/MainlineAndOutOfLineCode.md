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

# Mainline and Out of Line Code
The operations that are believed to run commonly are arranged on the mainline path. The less often executed operations
are chosen as the branch target on the out of line (`OOL`) path. Arranging the code that executes more often in the
mainline takes advantage of branch prediction and instruction cache resources. The out of line path for infrequently
executed operations is usually in a different cache line. By introducing locality to the most frequently executed parts
of code and outlining the least frequently executed fallback paths, we increase the probability of reducing the number
of cache lines needed to execute a particular code sequence. This feature also works more optimally with hardware
branch prediction, as 1) predictions are generally optimized for the fall-through path by many hardware platforms,
and 2) predicted fall-through instructions are generally already loaded in the instruction cache (i-cache), thereby
avoiding potential branch prediction delays due to i-cache misses for predicted taken paths. Also note that
occasionally, there would be some minor branching within the code on the mainline path to implement some basic logics.

Executing the common code on the mainline is not only adopted in IL but also in the codegen, such as the non-trivial
logics: write barrier, instanceof, etc. One example in the codegen is the bound check. It does the comparison between
arraylength and index. If it fails, an exception is going to be raised and it branches to an `OOL`, or a snippet that
jumps into the VM to throw the exception that needs to the thrown. Because the path that jumps to throw the exception
is expected to be very rarely called, the snippet that calls into the VM to throw the appropriate exception is put at
the end of the code section, or `OOL`, at the time of generating code for the bound check, so that it does not
interfere with the code that we believe will actually run. It is possible that subsequent bound checks (or other checks
/operations) that get evaluated also append more code to the end of the code section. The snippet for a given bound
check would not be exactly at the very end but instead, near the end of the code section.

Another example is block ordering and extension of basic blocks in IL. Both uses the same logic to arrange the blocks.
For example one possible scenario with a block ending with `if` is that one path might be marked as cold, and another
path is not marked as cold. Block ordering takes the cold path out of the mainline, puts it at the end of the method,
and keeps the non-cold path as the fall through. This way it avoids consuming branch prediction and instruction cache
resources.

# Out of Line Code

There are two categories regarding to out of line code:
1. Global cold blocks
2. Local `OOL` code sections

###  Global Cold Blocks
There are two optimizations related to global cold blocks: `coldBlockMarker` and `coldBlockOutlining`.

`coldBlockMarker` optimization as part of ILGen strategy identifies cold blocks and propagates cold info in CFG.
A block is considered as cold such as if a block throws an exception and exceptions are considered as rare, or if
it has not run, or if it is an interpreted call node, etc. In propagating code info, the block is marked as cold
if all predecessors or all successors are cold.

`coldBlockOutlining` optimization performs what `coldBlockMarker` does: mark cold blocks and propagates cold info
in CFG, after which it also moves the cold blocks to the end of tree list to ensure the more often executed,
or `hot`, blocks are kept close.


### OOL

The `OOL` code section is used by codegen to emit slow path instructions so that the register allocator could
allocate the registers first for the fast path to minimize the impact of register shuffling or register spilling
on the fast path. More details can be found in [omr/compiler/codegen/OutOfLineCodeSection.hpp](https://github.com/eclipse/omr/blob/2ccbf5e8ce2cc1f7b0888e562ca9ee11e712f5d9/compiler/codegen/OutOfLineCodeSection.hpp#L36-L106)

