# BenefitInliner

BenefitInliner introduces a novel way of inlining. Compared to the traditional way of inlining relies on hard-coded heuristics, BenefitInliner provides a systematic way of evaluating inlining candidates that combines the dynamic and static benefits of inlining. 

## Overview of main phases of BenefitInliner
1. Build the search space for candidate methods to be inlined.
    1. **TR::IDTBuilder** constructs an inlining dependency tree (**TR::IDT**).
    2. **TR::AbstractInterpreter** generates an **TR::InliningMethodSummary** stashing potential optimization opportunities and specifying the constraints for parameters to make optimizations happen.
    3. **TR::AbstractInterpreter** computes abstract state values for each program point. 
    4. At each call site, we calculate the benefit of inlining this method with its call frequency (direct benefit) and optimizations unlocked (indirect benefit) according to **TR::InliningMethodSummary**.
2. Run the knapsack algorithm for choosing the best inlining plan.  
3. Commit the inlining plan and inline the methods.
    
## Higher-Level Structure of the parts of BenfitInliner

### 1. BenefitInliner.cpp

Class hierarchy of the adapter class:
* **TR::Optimization**
    * **TR::BenefitInlinerWrapper**

Class hierarchy of the real inliner:
* **TR_InlinerBase**
    * **TR::BenefitInlinerBase**
       * **TR::BenefitInliner**

* **TR::BenefitInlinerWrapper** is an adapter class which extends from **TR::Optimization**. `TR::BenefitInlinerWrapper::perform()` initializes a **TR::BenefitInliner** and controls how benefit inlining process is being done.

* **TR::BenefitInliner** is the real Inliner that contains functionalities for selecting inlining candidates. When a **TR::BenefitInliner** is initialized, it comes with an inlining budget that constrains inlining by avoiding an uncontrollable increase in binary size. The inlining budget will be calculated based on the hotness and size of the method being JIT compiled. `TR::BenefitInliner::buildingInliningDependecyTree()` builds an **TR::IDT** that contains all the candidate methods to be inlined. `TR::BenefitInliner::inlinerPacking()` consumes the **TR::IDT** and selects the optimal set of methods to be inlined given the inlining budget and the cost and benefit of each method in the **TR::IDT**. 

* **TR::BenefitInlinerBase** contains the functionalities of doing the actual inlining according to the inlining proposal proposed by `TR::BenefitInliner::inlinerPacking()`. 

### 2. InliningProposal.cpp

* **TR::InliningProposal** proposes an optimal set of methods that will be inlined. **TR::BenefitInlinerBase** takes **TR::InliningProposal** as a dependency. **TR::InliningProposal** will be set by the `TR::BenefitInliner::inlinerPacking()` and will be consulted during the inlining. 

* **TR::InliningProposalTable** is a 2D table of **TR::InliningProposal**. By the nature of `TR::BenefitInliner::inlinerPacking()` being a nested knapsack algorithm, a 2D table is required. 

### 3. IDTNode.cpp, IDT.cpp & IDTBuilder.cpp

* **TR::IDTNode** is the building block of **TR::IDT**. It is the abstract representation of a candidate method. 

A **TR::IDTNode** has the following important properties:
- **call ratio** - Estimate of how many times this node is visited per the execution of its parent. 
- **root call ratio** - Estimate of how many times this node is visited per the execution of the root node. 
- **Inlining method summary** - Captures potential optimization opportunities for inlining this method. (Refer to **InliningMethodSummary.cpp** for details)
- **static benefit** - The indirect benefit (optimizations unlocked) of inlining this method.
- **budget** - The remaining budget for the descendants of this node.
- **Bytecode Index** - The call site index of this method.


The **benefit** of inlining a candidate method is calculated as: `Benefit = root call ratio * static benefit`

The **cost** of inlining a candidate method is equal to the size of this method.

The **cost** and **benefit** will be used for solving the inliner packing problem for getting an optimal set of methods to be inlined.

* **TR::IDT** is an abstract program representation that allows us to consistently define the search space for inlining candidates. The parent-child relationship in the **TR::IDT** corresponds to the caller-callee relationship.

* **TR::IDTBuilder** builds a **TR::IDT** starting from the method (root method) being JIT compiled. The **TR::IDT** is built in **DFS** order. `TR::IDTBuilder::buildIDT()` returns a complete **TR::IDT** that can be consumed by `TR::BenefitInliner::inlinerPacking()`

**Process of building the IDT:**

```
buildIDT() --> buildIDT2() --> generateControlFlowGraph() --> performAbstractInterpretation() --> addNodesToIDT() 
             |                                                                                              |  
             |______________________________________________________________________________________________| 
``` 

Note: `generateControlFlowGraph()` and `performAbstractInterpretation()` need language-specific implementation.

The responsibilities of **abstract interpretation**:
1. Identifying call sites. Call sites will be added to the **TR::IDT** as **TR::IDTNodes**
2. Generating inlining method summaries. (Refer to **InliningMethodSummary.cpp** for details )
3. Simulating the program state. Currently, the useful part of the program state is the arguments passed to each call site. Combining those arguments with the inlining method summary, we can get the static benefit of inlining this method.

### 4. AbsVisitor.hpp & IDTBuilderVisitor
* **TR::AbsVisitor** is an interface for the derived classes to define customized callback methods during abstract interpretation. Currently, there is only one method defined in **TR::AbsVisitor** called `TR::AbsVistor::visitCallSite`. **TR::IDTBuilderVisitor** inherits from **TR::AbsVisitor** and defines it own implementation of `visitCallSite`. Whenever we encounter a call site during abstract interpretation, `TR::AbsVistor::visitCallSite` will be invoked. If **TR::AbsVisitor** is a **TR::IDTBuilderVisitor**, `TR::IDTBuilder::addNodesToIDT` will be called in order to add **TR::IDTNode** to the **TR::IDT**.

### 5. InliningMethodSummary.cpp

**TR::PotentialOptimizationPredicate** is the interface for the potential optimization predicates such as branch folding and null-check folding, etc.`TR::PotentialOptimizationPredicate::test` tests against an argument value to tell whether or not this abstract argument is a safe value to unlock the optimization.

**TR::PotentialOptimizationVPPredicate** is the extention of **TR::PotentialOptimizationPredicate**. It takes **TR::VPConstraint** as a dependency to serve as the constraint (lattice). Those constraints specify the maximal safe values to make the optimizations happen. 

**TR::InliningMethodSummary** captures potential optimization opportunities of inlining one particular method. `InliningMethodSummary::testArgument` tells the total optimizations that will be unlocked given the abstract argument value.

### 6. AbsValue.cpp, AbsOpStack.cpp & AbsOpArray.cpp

**TR::AbsValue** is an interface for abstract values. A **top** abstract value is the least accurate abstract value, analogous to the unknown value. A NULL **TR::AbsValue** represents uninitialized abstract values (bottom). The core method `merge(TR::AbsValue* other)` along with other methods need to be implemented by extension classes. The implementation of the `merge(TR::AbsValue* other)` operation should be an in-place merge and should not modify `other`. Also, `self` should not store any mutable references from `other` during the merge but immutable references are allowed.

**TR::AbsVPValue** extends from **TR::AbsValue**. It uses **TR::VPConstraint** as the lattice (constraint) to represent the value. `TR::AbsVPValue::merge` relies on `TR::VPConstraint::merge` for merging various type of values. 

**TR::AbsOpStack** is an operand stack for abstract values. **TR::AbsOpArray** is an operand array for abstract values. The `merge` functions of **TR::AbsOpStack** and **TR::AbsOpArray** merge another state in place. The merge operation does not modify the state of another state or store any references of abstract values from another state to be merged with.