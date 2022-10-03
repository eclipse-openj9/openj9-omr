<!--
Copyright (c) 2022, 2022 IBM Corp. and others

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

# Notes on IL Opcodes


## `treetop`

The `treetop` opcode represents a top-level operation which simply ignores the result of its child.


### Anchoring

By mentioning a child whose value is then ignored, `treetop` provides a way to control the point of evaluation of the child. A node is evaluated in the tree in which it first appears, so by making a node appear within a `treetop`, or as we say, *anchoring* the node, it is possible to ensure that the node is evaluated no later than a particular tree. For example, consider the following IL, and suppose that `n3n` is the first point of use of `n1n`:
```
      treetop
n1n     iload x
      ...
n2n   istore x
        iconst 0
      ...
n3n   istorei Foo.bar
        aload foo
n1n     ==>iload x
```
This IL sets `Foo.bar` to the value of `x` from before it is overwritten in `n2n`. If `n1n` were not anchored, then it would be evaluated at `n3n`, and the IL would instead set `Foo.bar` to zero.

Why not just put `n1n` directly at top-level? Top-level nodes do not produce a value. They are expected only to perform control flow and/or side-effects, and they have reference count zero. Value-producing nodes, on the other hand, may have multiple occurrences when their results are used multiple times. To ensure that the reference count of a value-producing node is exactly its number of occurrences, it is not allowed at top-level.


### Calls

As value-producing nodes, calls are not allowed at top-level. But calls also perform side-effects and (exception) control flow, so they must be carefully sequenced w.r.t. other effectful operations. To satisfy both of these constraints at once, each call node is required to be anchored directly within `treetop` (or one of a few other opcodes, e.g. `NULLCHK`) at its first occurrence.


### Resultless Children

Because `treetop` ignores the result of its child, it can cope with situations in which the child does not produce a result at all. In particular, this occurs for void-typed calls, which for consistency with other calls are still anchored, and for other side-effecting operations that can be fused with `NULLCHK`. These latter operations are allowed so that `NULLCHK` can be eliminated simply by changing its opcode to `treetop`. For example, consider this tree, which checks that `foo` is not null, and if so, also stores to `Foo.bar`:
```
NULLCHK
  istorei Foo.bar
    aload foo
    iload x
```
If the `NULLCHK` is found to be unnecessary, it will be eliminated:
```
treetop
  istorei Foo.bar
    aload foo
    iload x
```
So an indirect store can appear as the child of `treetop`. In such a case, the `treetop` is unnecessary. It can be (and most likely will be) removed later. However, developers must still be careful to take account of the possibility that there may be a store (or any other node allowed as the child of `NULLCHK`) in this position; when looking for such nodes, it is necessary to check the child of `treetop`.


## `PassThrough`

`PassThrough` is an operation that computes the identity function. That is, the result of a `PassThrough` node is simply the result of its child, unchanged. This result is therefore also the same type as the child's result, and the `PassThrough` opcode itself has no particular result type.


### Use in `NULLCHK`

The original use of `PassThrough` is as the child of `NULLCHK`. Because `NULLCHK` supports fusing with an indirect load or store, it checks for null in the value not of its child, but rather its first grandchild (ignoring calls). It is possible though for a `NULLCHK` not to be fused with any other operation, in which case some node must still stand in as the child of `NULLCHK`, and `PassThrough` is useful there as a kind of nop.

For example, this tree checks to make sure that `foo` is non-null, and if so, it also loads `Foo.bar`:
```
NULLCHK
  iloadi Foo.bar
    aload foo
```
If the value of `Foo.bar` turns out to be unneeded, this can be changed to:
```
NULLCHK
  PassThrough
    aload foo
```
which checks that `foo` is non-null and does nothing else. Because a `NULLCHK` gets eliminated by changing its opcode to `treetop`, it's also possible to see `PassThrough` just below `treetop`:
```
treetop
  PassThrough
    aload foo
```
Here, the `PassThrough` is unnecessary. It can be (and most likely will be) eliminated later. This tree just has the effect of anchoring the load of `foo`.


### Other Uses

A `PassThrough` node can specify a global register number, and `PassThrough` nodes that do so are used as part of the syntax for outgoing [`GlRegDeps`](GlRegDeps.md). This is how the `GlRegDeps` specifies the outgoing register that carries each value (except those from matching incoming registers via `xRegLoad`).

A `PassThrough` node can also be flagged with `copyToNewVirtualRegister`, which is a hint to code generation back-ends. Such nodes are generated by the register dependency copy removal optimization pass.


### Code Generation

It is not necessary to generate any code for this operation. A `PassThrough` node can simply reuse its child's virtual register. However, if the `copyToNewVirtualRegister` flag is set, and if the code generator supports it, then a fresh virtual register will be allocated, and `PassThrough` will be implemented as a register-register move.
