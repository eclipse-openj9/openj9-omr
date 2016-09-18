/*******************************************************************************
 *
 * (c) Copyright IBM Corp. 2000, 2016
 *
 *  This program and the accompanying materials are made available
 *  under the terms of the Eclipse Public License v1.0 and
 *  Apache License v2.0 which accompanies this distribution.
 *
 *      The Eclipse Public License is available at
 *      http://www.eclipse.org/legal/epl-v10.html
 *
 *      The Apache License v2.0 is available at
 *      http://www.opensource.org/licenses/apache2.0.php
 *
 * Contributors:
 *    Multiple authors (IBM Corp.) - initial implementation and documentation
 *******************************************************************************/

#ifndef OPTIMIZATIONSTRATEGIES_INCL
#define OPTIMIZATIONSTRATEGIES_INCL

#include <stdint.h>                     // for uint16_t
#include "optimizer/Optimizations.hpp"  // for Optimizations

struct OptimizationStrategy
   {
   OMR::Optimizations _num;
   uint16_t      _options;
   };
//optimization groups

extern const OptimizationStrategy loopAliasRefinerOpts[];
extern const OptimizationStrategy arrayPrivatizationOpts[];
extern const OptimizationStrategy reorderArrayIndexOpts[];
extern const OptimizationStrategy localValuePropagationOpts[];
extern const OptimizationStrategy cheapObjectAllocationOpts[];
extern const OptimizationStrategy expensiveObjectAllocationOpts[];
extern const OptimizationStrategy eachEscapeAnalysisPassOpts[];
extern const OptimizationStrategy veryCheapGlobalValuePropagationOpts[];
extern const OptimizationStrategy cheapGlobalValuePropagationOpts[];
extern const OptimizationStrategy expensiveGlobalValuePropagationOpts[];
extern const OptimizationStrategy eachExpensiveGlobalValuePropagationOpts[];
extern const OptimizationStrategy veryExpensiveGlobalValuePropagationOpts[];
extern const OptimizationStrategy partialRedundancyEliminationOpts[];
extern const OptimizationStrategy isolatedStoreOpts[];
extern const OptimizationStrategy globalDeadStoreOpts[];
extern const OptimizationStrategy finalGlobalOpts[];
extern const OptimizationStrategy loopSpecializerOpts[];
extern const OptimizationStrategy loopVersionerOpts[];
extern const OptimizationStrategy lastLoopVersionerOpts[];
extern const OptimizationStrategy loopCanonicalizationOpts[];
extern const OptimizationStrategy blockManipulationOpts[];
extern const OptimizationStrategy eachLocalAnalysisPassOpts[];
extern const OptimizationStrategy stripMiningOpts[];
extern const OptimizationStrategy prefetchInsertionOpts[];
extern const OptimizationStrategy methodHandleInvokeInliningOpts[];

//arrays of optimizations

extern const OptimizationStrategy * omrCompilationStrategies[];
extern const OptimizationStrategy * rubyCompilationStrategies[];

#endif
