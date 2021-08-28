# Loop Canonicalizer, Loop Versioner, and Loop Specializer

## 1. Loop Canonicalizer

Loop Canonicalizer converts while-do loops into do-while loops and creates loop preheader blocks. It runs ahead of other
optimizations such as Loop Versioner, Loop Unroller, and Loop Strider, etc. as an enabler for other optimizations. Loop
canonicalization does not improve the performance on its own, but rather converts the trees representing the loop into
a standardized form which other loop optimizations will expect. This makes implementing other loop optimizations easier
as they only have to consider loops in the canonical (standard) form.

### Loop Preheader Block

Loop preheader block is a basic block that is outside of the loop and dominates all blocks in the loop. It is a block
that has cleared all the other conditions that have to be cleared to enter the loop. It must run before the loop is
entered. Preferably, the loop preheader block is the last thing that runs before a loop is entered.

Other optimizations can add computations to the preheader block when they change the content of a loop, for example
Loop Unroller can use the preheader block to put down initialization code before the loop is entered. Similarly,
Loop Strider can add stores to new variables in the preheader block.

Consider the following loop:
```
while (i< 15) {
   z = x + y + i; // x+y is loop invariant
   ++i;
}
```
There is an expression `x+y` inside the loop. `x+y` does not change inside the loop. Neither `x` nor `y` is affected
by anything inside the loop. The expression `x+y` can be hoisted out of the loop and put in a local register or a local
variable: `t=x+y`. This expression should be computed at some point where it will not add overhead if the loop is not
executed. At the same time, this expression should also be at a place where it is guaranteed to run if the loop is
executed. These are the two necessary conditions for an expression to be hoisted out of a loop, and the natural location
to hoist the expressions to is the preheader block.

The above loop can be transformed into a more performant loop with equivalent semantics:
```
t = x + y;  // put x+y into the preheader
while (i<15) {
   z = t + i;
   ++i;
}
```

### while-do Loop and do-while Loop

Loops are organized in two different ways: (1) while-do loop (2) do-while loop.

A do-while loop executes the body of the loop before it checks the condition. A while-do loop executes the conditions
first and then executes the body. The difference between them is that one iteration of the loop is guaranteed to run in a
do-while loop. For a while-do loop, it is possible that no single iteration will get to run if the condition check fails.

These two types of loops are canonicalized differently. A do-while loop is in a better state to be optimized than a
while-do loop.

For do-while loops, because the condition is checked at the end of the loop, it is guaranteed that the loop body gets
to be executed at least once. If a computation from the loop body is hoisted out into the preheader block, likely it is
not going to be any worse than doing the computation inside the loop. In the worst case, the loop will still get to be
executed at least once. Executing those computations in the preheader is no worse than executing them once inside the
loop when the loop runs.

Therefore, for a do-while loop, a preheader block can be simply added ahead of the do-while loop body.

For while-do loops, if a computation is added in the preheader from inside a loop, it could become redundant and make
things worse if the loop is not entered at all if the condition check fails on the first iteration.

```
while (i< 15) {
   z = x + y + i;
   ++i;
}
// Convert a while-do loop into a do-while loop
if (i< 15) {
  t = x + y;
  do {
     z = t + i;
     ++i;
  }
  while (i<15)
}
```
If it is a while-do loop, we hoist the first “if” test that controls whether we enter the loop outside the loop. If it is
true, we fall into the loop preheader block and then execute the body of the loop. We will [clone that condition](https://github.com/eclipse/omr/blob/ef9fafc24998865bc4041ad56cc17b2b4c1f210c/compiler/optimizer/LoopCanonicalizer.cpp#L817-L845)
so that we check it at the end of the loop as well. This is how a while-do loop is converted into a do-while loop by
checking the condition outside the loop.

## 2. Loop Versioner

Loop Versioner optimizes away either exception checks or tests from inside the loop so that they will not be executed
every time inside the loop. Instead, they will execute less frequently outside the loop. Compared to Loop Canonicalizer,
Loop Versioner does something that will improve performance.

There are several different checks, tests, and conditions that could be optimized by Loop Versioner. After a loop is
transformed by Loop Versioner, there are two versions of a loop: One is a good (fast or hot) version that has all the checks
(that meet the necessary conditions/criteria) removed. Another is a bad (slow or cold) version that has all the checks in it.

### Versioning with Loop Invariant Bound Check
```
for (i=0; i<5; i++) {
   c[i] = i + a[n]; // a[n] is loop invariant
   }
```
For example, `a[n]` is accessed in a loop and `n` does not change inside the loop. There is a bound check on `n` that happens
inside the loop. The bound check compares `n` with `a.length`. If `n` is less than `0`, and greater or equals to `a.length`,
it throws an array index out of bound exception (`ArrayIndexOutOfBoundsException`).

We cannot move the bound check on `n` to the loop preheader because it will change the program order with respect to other
operations. An exception has side effects. Anytime a node has a side effect, you must respect the program order. Users rely
on these operations that have side effects to happen at the order that it is being written. If we change the order of the
exceptions that may be raised, we may have changed the semantics of the original program, which violates the behaviour of
the original program.

Instead, we compare the value `n` in `if` tests: (1) If `n` is less than `0`, (2) If `n` is greater or equals to `a.length`,
outside the loop before the preheader block. These are `if` tests and they are not check nodes. We will get the same answer
as we do it inside the loop. We can remove the bound check inside the loop because we have done the testing outside the loop
because `n` is loop invariant.

What do we do if we are supposed to raise an exception? If the test passes outside the loop, we just fall into the loop and
the body is executed without the bound check. This is the fast path and the common case.

We also create a cold version of the loop on the slow path. We clone the entire loop which has all the exception checks and
everything intact from the original program. If the test outside the loop fails, this cold version of the original loop is
executed, and it will throw the exception where it is supposed to be thrown.

This way the behaviour is preserved from the original program while the common case is still optimized. Exceptions are not
supposed to happen in a well-behaved program, which is the common case. That is the assumption we take advantage of while
doing these optimizations.

### Versioning with Multiple Loop Invariant Checks

What if there are two bound checks: `a[n]` and `b[m]`? Both are invariant. Now we have four tests: `n < 0`, `n >= a.length`,
`m < 0`, `m >= b.length`. If we create a version of the loop for every test, it will explode the code size. What happens in
this case is that there is only one good version and there is only one bad version when Loop Versioner versions a particular
loop. The good version is executed if all the conditions are satisfied for executing the good version of the loop, meaning
none of the exception checks will fail. The bad loop runs if any one of the conditions fail. The bad version has all the
checks left intact from the original loop. The assumption is that none of the checks are meant to fail. The likelihood of
hitting the good loop is pretty good. This is how multiple checks are versioned with respect to the same loop.

### Versioning with Well-behaved Loops with Variant Checks
```
for (i=0; i<n; i++) {
   a[i] = i; // doing a bound check every time because “i" changes
   }
```
This is a well-behaved loop: `n` and array `a` are invariant inside the loop; The index starts out with a value and increments
by `1` every time. The range of `i` is known. As long as `i` is less than `n` to begin with, you will increment `i` until it
becomes `n`, at which point, the loop exits.

Since the loop is going to run `n` times, instead of checking `i < 0` and `i >= a.length` inside the loop in each iteration,
you can check `i < 0` and `i+n >= a.length` in the loop preheader. The two edges/ends of the range for values of `i` are tested:
the enter value `i` and the final value `i+n`, with respect to the two conditions. If the upper bound and the lower bound are
guaranteed to be fine, the bound does not need to be checked inside the loop every time.

If any of the conditions fail, you go to the slow loop. Somewhere down the line in the slow loop in some iteration, the bound
check exception will be raised. That is good enough to send it to the slow loop and run all iterations in the slow loop.

Therefore, with a well-behaved loop, you could use the starting value to do the less-than test and the ending/final value to
do the greater-or-equal-to test. This allows you to eliminate the bound check even if the value changes within the loop.

This trick cannot be used if `i` is not changed in a predictable way. For example, `i=foo()` and the bound check on `a[i]`
inside the loop cannot be eliminated. The base array pointer/object cannot be changing within the loop either. If array `a`
points to an array of `100` elements on the first iteration and points to an array of `1` element on the second iteration,
the bound check cannot be eliminated inside the loop.  Another example is that if the index to the array is a field: such as
`a[o.f]` and there is a call within the loop. That call could change the field `o.f`. In this case, the bound check on `a[o.f]`
cannot be eliminated.

### Versioning with Different Checks and Guards

We do not always loop version `NULL` checks. `NULL` check is free on X86 via the resumable trap mechanism. We execute `NULL`
check not via an explicit test on `NULL`. There will be no generated code. We optimized away the `NULL` check in the code
generator. The first few pages of the address space are protected. If anyone tries to access a value based off `NULL`, say
a field with offset `0x25`, they will be accessing address `0x0+0x25` which falls on the first page in memory which is
protected.  A trap gets raised. VM handles the trap and checks where this trap comes from. It looks up the JIT meta data
for which instruction is trapped. Then it raises `NULL` pointer exception with the right line number.

However, we version bound checks and `NULL` checks at a higher optimization level because exception checks act as barriers to
the code motion. It is always a good thing to have fewer checks in the code when the optimizer is moving the code around. If an
optimization wants to move an expression up and there is a check standing in the way, the optimization might think it is not
allowed to move up the expression beyond the check.

We version `NULL` checks, bound checks, divided by zero checks, checkcast, array store checks, with respect to the right
conditions when we try to put them outside the loop.

We also version with respect to guards. Guards are extremely unlikely to fail. If we have a guard within a loop and the value
does not change within the loop, we can pull out the guard and do it outside the loop. If it passes, we run the loop without
the guard. If it fails, we run the loop with the guard.

If the guard is put outside the loop, the slow path can be eliminated from the good version of the loop. The call only exists in
the bad version of the loop. This allows you to optimize the fast version of the loop better because having a call inhibits other
optimizations. If there is a call inside a loop, you cannot consider any fields, static variables, or array accesses that happen
in the loop as invariant.

## 3. Loop Transfer in Loop Versioner

Loop Transfer optimization occurs in Loop Versioner, and it is related to versioning of guards.

Versioning must guarantee the expressions do not change within the loop. The versioning of guards requires the receiver for the
call to be invariant, which is checked before guard is put outside the loop.

Loop Transfer optimizes guards inside a loop where the underlying receiver object is not invariant.
```

                       Fast/Hot Loop (A)                                       Slow/Cold Loop (A')
                               |                                                      |
                               |                                                      |
                               v                                                      v
                            Guard (G)                                              Guard (G')
                            /     \                                                 /     \
                           /       \                                               /       \
                          v         v                                             v         v
 Inlined code (fast path) (I)      call (slow path) (C)  Inlined code (fast path) (I')     call (slow path) (C')
```
For example, there is a loop with a non-loop invariant virtual guard and an invariant bound check inside the loop. After the loop
is versioned with the bound check, there are two versions of the loop. One version is the fast/hot loop that does not have the
bound check (`A`). Another one is the slow/cold loop that is a clone of the fast loop but with all the exception checks (`A’`).
Both have the guards: `G` inside the fast loop, `G’` insides the slow loop.

Loop Transfer takes the slow path (call path) out of the guard and redirects it to the slow loop. In the fast loop, if the virtual
guard succeeds, it falls into the inlined code block (`I`). If it fails, it will not go into the call block (`C`) and it will go
to `C’` in the slow loop.

```
                    Fast/Hot Loop (A)                                       Slow/Cold Loop (A')
                               |                                                       |
                               |                                                       |
                               v                                                       v
                            Guard (G)                                               Guard (G')
                            /      |                                                /     \
                           /       |                                               /       \
                          v        |                                              v         v
 Inlined code (fast path) (I)      |                     Inlined code (fast path) (I')     call (slow path) (C')
                                   |                                                                ^
                                   |                                                                |
                                   +----------------------------------------------------------------+

```
The point of this optimization is to essentially remove the call from the fast loop by sending the slow path out of that guard in
the fast/hot loop into the slow loop. After the transformation, the fast loop only has the fast path. It has the guard and its
fast path, but if the guard fails, it goes into the slow loop and take the slow path (`C’`) in the slow loop. The slow path (`C`)
is eliminated from the fast loop. When the call (`C`) is gone, it may open up opportunities for other optimizations in the fast
loop.

The bad thing about Loop Transfer is that it branches into the slow loop. It does not enter the slow loop in a well-behaved way.
A loop should be entered from the entry block of the loop. That is the main property of a natural loop structure. A loop should
not be entered at any place. It should be entered in a nice and structured way at the top of the loop.

By changing the guard `G` to branch into `C’` in the middle of the slow loop without going through the entry block, it is no longer
a well-behaved loop. By doing so, an improper region structure is created. An improper region indicates something is going on with
the loop and it is no longer a well-structured control flow. Other optimizations become conservative around improper regions. If
an improper region is created, the loop is not versioned, unrolled, loop stride on, and even VP forgets all the constraints it
knows about at that time.

### Avoid Creation of Improper Regions in Loop Transfer ([#13243](https://github.com/eclipse-openj9/openj9/issues/13243))

Instead of taking `G` and redirecting to `C’` in the slow loop, the optimization redirects it to the header of the slow loop. It
enters the slow loop at where everybody enters. All the different places from the fast loop could branch into the entry block of
the slow loop. We change the header of the slow loop to figure out first where they come from and then branch accordingly inside
the slow loop to the appropriate point.
```

                                                +--------------------------------------+
                                                |                                      |
                                                |                                      v
                                                |                                    Switch ------------+
                       Fast/Hot Loop (A)        |                              Slow/Cold Loop (A')      |
                               |                |                                      |                |
                               |                |                                      |                |
                               v                |                                      v                |
                            Guard (G)           |                                   Guard (G')          |
                            /    |              |                                   /     \             |
                           /     |              |                                  /       <------------+
                          v      +--------------+                                 v         v
 Inlined code (fast path) (I)                            Inlined code (fast path) (I')     call (slow path) (C')
```
A switch statement is inserted into the header or the actual entry of the slow loop to set up the control flow. If there are
two guards. We will have `G1`, `I1`, `C1`, `G2`, `I2`, `C2` in the fast loop and copies of them in the slow loop: `G1’`,
`I1’`, `C1’`, `G2’`, `I2’`, `C2’`. `G1` goes to `C1’` and `G2` goes to `C2’`.  Two different places from the fast loop would
branch into the slow loop. Instead, we send both `G1` and `G2` to the header of the slow loop. The header of the slow loop
decides where they are from and then send `G1` to `C1’` and send `G2` to`C2’`.

When there is a nested loop, there is an outer loop and there is an inner loop. The slow loop of the inner loop is inside
the slow loop of the outer loop. There are guards in the inner loop. If the guard is sent to the slow loop of the inner loop
directly, not only you branch into the middle of the slow loop of the inner loop, you also branch into the middle of the slow
loop of the outer loop.

The guard from the fast loop should be sent to the header of the slow version of the outer loop. Let the slow version of
the outer loop decide where to send it. It will send it to the header of the slow version of the inner loop. Now you have
a switch table at the header of the outer loop and a switch table at the header of the inner loop. There are cascades of
switch tables until the desired inner loop is reached. The overhead of these switch tables is less likely executed because
the slow loop is likely not executed based on the assumption that the original guard in the fast loop will not fail.

## 4. Loop Specializer

[Loop Specializer](https://github.com/eclipse/omr/blob/e3a15a993c8aba80582aa1d6f3071e122acbd4c4/compiler/optimizer/LoopVersioner.hpp#L1061-L1071)
subclasses Loop Versioner and shares about 90% of the code with Loop Versioner. The reason that Loop Specializer shares
so much code with Loop Versioner is that the transformation it performs is quite like what Loop Versioner does.

Profiler profiles various things at the run time, for example block frequency, or the values of a specific
expression, etc. Loop Specializer is based on the value profiling information. We keep track of loop invariant values
or expressions. If they are always a certain value, the loop can be specialized with respect to that value.

In the following example, `n` is loop invariant. It would be great if we know what the value `n` is. If we know `n` is
a small value like `10` or `4`, rather than testing if the value is between `0` and `n`, we could test if the value is
between `0` and `10`, or whatever the value is profiled as. The loop could be unrolled completely and would be eliminated.
Even if the loop cannot be unrolled completely, knowing the precise bounds of the constant allows subsequent optimizations
such as [Value Propagation](https://github.com/eclipse/omr/blob/6cc32df405cd1dd688470e0b5b13fcc5938e2921/doc/compiler/optimizer/ValuePropagation.md)
to further constrain expressions within the loop, which may enable further optimization.

```
for (int i=0; i < n; ++) {
   … // n is loop invariant and the profiled value is 10 or a small value
}
```
The intent of Loop Specializer is to introduce constants into loops as much as possible. It can only be done to the
expressions that do not change within the loop. `n` is profiled because it is loop invariant. If `n` takes the value
`10` 100% times when it is profiled, when it comes to the next compilation, we look at the profiling information for
`n`, and we could check outside the loop as below:

```
if (n == 10) {
    for (int i = 0; i < 10; ++i) {
    ... // n == 10 can be constant propagated inside the loop
    }
} else {
    // original loop
}
```

### Loop Versioner & Loop Specializer

Loop Versioner runs before Loop Specializer. As mentioned in section 2, Loop Versioner creates tests corresponding to
exception checks. There is a fast version of the loop and there is a slow version of the loop. The fast version has all
the checks that could be removed have been removed. The slow version has all the checks intact. The Loop Specializer
does not do anything to the slow version of the loop. Loop Specializer specializes the fast version of the loop.

After both Loop Versioner and Loop Specializer have run, there are three versions of the loop. (1) One version is marked
as cold with all the checks, and nothing is changed to constants. (2) Another version of the loop has all the checks that
could be removed have been removed, and all the expressions that could be transformed into constants have been transformed.
(3) The third version of the loop is somewhere in between. This intermediate loop is versioned but not specialized. It has
all the checks that could be removed have been removed, but the constants are not specialized or substituted in the loop.

### Reasons That Loop Specializer Is Not Combined With Loop Versioner

Profiling information is not as strong or guaranteed as exception checks are. The exception checks should not fail in well
behaved programs. However, values or expressions could change in well behaved programs.  In Loop Versioner or Loop Specializer,
the tests are all or nothing. Even if one condition fails in the loop pre-header, it goes to the slow version of the loop
or the less optimized version.

For example, if a version that has `50` exception checks and one profiling condition and the profiled value changes, that
one condition failing sends the control to the slow version of the loop where there are `50` exception checks, and that
one value is not a constant. It is a too high of price to pay. Therefore, it is not preferred to combine the extremely
high probability tests with nearly high probability tests. They should be separated. The extremely high probability tests
should remain in one version. Nearly high probability tests should have their version of the loop. That is why there are
three versions of the loop.

### Similarities to Loop Versioner

For example, there is an expression `x.f.g` that is profiled, and it is loop invariant. If there is a check `x.f.g == 10`
in the loop, the expression can be hoisted to the loop pre-header. If `x.f.g` equals to `10`, the execution goes to the
fast version of the loop. Otherwise, it goes to the slow version of the loop.

We must ask if it is safe to check `x.f.g == 10` outside the loop. What if `x` is `NULL`? We do not want to crash or raise
an exception in this comparison. We need to lay down a set of tests before the test `x.f.g == 10`. We must test if
`x != NULL && x.f != NULL` before testing `x.f.g == 10`. In this case, two more tests must be laid down first to ensure
we do not crash or throw an exception when accessing this expression outside the loop. The last one is the actual expression
test based on the value that is profiled. This involves multiple tests and any of the tests fail will send the control
flow to the slow version of the loop. Some of these tests are exception check related tests, which are similar to how
Loop Versioner would have checked to ensure that we are allowed to access what we want to access.

Multiple values can be invariant in a loop. They could be profiled into different values. Multiple tests must be laid down
for all of them. We do not create a separate loop for each of the expressions that are profiled. There are only three
versions of the loop after both Loop Versioner and Loop Specializer run. Loop Versioner will version with respect to all
the checks. Loop Specializer will specialize with respect to all the profiled values. The specialized version will have
all the expressions that we want to specialize are converted into constants. The unspecialized version will have none of
those expressions converted into constants. Like Loop versioner, all checks are gone in one version, and all checks are
intact in another.
