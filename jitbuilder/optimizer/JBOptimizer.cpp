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

#include "optimizer/Optimizer.hpp"

#include <stddef.h>                                       // for NULL
#include <stdint.h>                                       // for uint16_t
#include "compile/Compilation.hpp"
#include "compile/Method.hpp"                             // for TR_Method
#include "control/Options.hpp"
#include "control/Options_inlines.hpp"
#include "il/symbol/ResolvedMethodSymbol.hpp"

#include "optimizer/Optimization.hpp"
#include "optimizer/OptimizationManager.hpp"
#include "optimizer/OptimizationStrategies.hpp"
#include "optimizer/Optimizations.hpp"
#include "optimizer/Structure.hpp"
#include "optimizer/StructuralAnalysis.hpp"
#include "optimizer/UseDefInfo.hpp"
#include "optimizer/ValueNumberInfo.hpp"

#include "optimizer/CFGSimplifier.hpp"
#include "optimizer/CompactLocals.hpp"
#include "optimizer/CopyPropagation.hpp"
#include "optimizer/DeadStoreElimination.hpp"
#include "optimizer/DeadTreesElimination.hpp"
#include "optimizer/ExpressionsSimplification.hpp"
#include "optimizer/GeneralLoopUnroller.hpp"
#include "optimizer/GlobalRegisterAllocator.hpp"
#include "optimizer/LocalCSE.hpp"
#include "optimizer/LocalDeadStoreElimination.hpp"
#include "optimizer/LocalLiveRangeReducer.hpp"
#include "optimizer/LocalOpts.hpp"
#include "optimizer/LocalReordering.hpp"
#include "optimizer/LoopCanonicalizer.hpp"
#include "optimizer/LoopReducer.hpp"
#include "optimizer/LoopReplicator.hpp"
#include "optimizer/LoopVersioner.hpp"
#include "optimizer/OrderBlocks.hpp"
#include "optimizer/PartialRedundancy.hpp"
#include "optimizer/IsolatedStoreElimination.hpp"
#include "optimizer/RegDepCopyRemoval.hpp"
#include "optimizer/Simplifier.hpp"
#include "optimizer/SinkStores.hpp"
#include "optimizer/ShrinkWrapping.hpp"
#include "optimizer/TrivialDeadBlockRemover.hpp"


static const OptimizationStrategy tacticalGlobalRegisterAllocatorOpts[] =
   {
//   { OMR::inductionVariableAnalysis,             OMR::IfLoops                      },
   { OMR::loopCanonicalization,                  OMR::IfLoops                      },
   { OMR::liveRangeSplitter,                     OMR::IfLoops                      },
   { OMR::redundantGotoElimination,              OMR::IfNotProfiling               }, // need to be run before global register allocator
   { OMR::treeSimplification,                    OMR::MarkLastRun                  }, // Cleanup the trees after redundantGotoElimination
   { OMR::tacticalGlobalRegisterAllocator,       OMR::IfEnabled                    },
   { OMR::localCSE                                                                 },
// { isolatedStoreGroup,                         OMR::IfEnabled                    }, // if global register allocator created stores from registers
   { OMR::globalCopyPropagation,                 OMR::IfEnabledAndMoreThanOneBlock }, // if live range splitting created copies
   { OMR::localCSE                                                                 }, // localCSE after post-PRE + post-GRA globalCopyPropagation to clean up whole expression remat (rtc 64659)
   { OMR::globalDeadStoreGroup,                  OMR::IfEnabled                    },
   { OMR::redundantGotoElimination,              OMR::IfEnabled                    }, // if global register allocator created new block
   { OMR::deadTreesElimination                                                     }, // remove dangling GlRegDeps
   { OMR::deadTreesElimination,                  OMR::IfEnabled                    }, // remove dead RegStores produced by previous deadTrees pass
   { OMR::deadTreesElimination,                  OMR::IfEnabled                    }, // remove dead RegStores produced by previous deadTrees pass
   { OMR::endGroup                                                                 }
   };

static const OptimizationStrategy cheapTacticalGlobalRegisterAllocatorOpts[] =
   {
   { OMR::redundantGotoElimination,              OMR::IfNotProfiling               }, // need to be run before global register allocator
   { OMR::tacticalGlobalRegisterAllocator,       OMR::IfEnabled                    },
   { OMR::endGroup                        }
   };

static const OptimizationStrategy JBwarmStrategyOpts[] =
   {
   { OMR::trivialDeadTreeRemoval,                    OMR::IfEnabled                },
   { OMR::treeSimplification                                                       },
   { OMR::lastLoopVersionerGroup,                    OMR::IfLoops                  },
   { OMR::globalDeadStoreElimination,                OMR::IfEnabledAndLoops        },
   { OMR::deadTreesElimination                                                     },
   { OMR::basicBlockOrdering,                        OMR::IfLoops                  },
   { OMR::treeSimplification                                                       },
   { OMR::blockSplitter                                                            },
   { OMR::treeSimplification                                                       },
//   { OMR::inductionVariableAnalysis,                 OMR::IfLoops                  },
   { OMR::generalLoopUnroller,                       OMR::IfLoops                  },
   { OMR::basicBlockExtension,                       OMR::MarkLastRun              }, // extend blocks; move trees around if reqd
   { OMR::treeSimplification                                                       }, // revisit; not really required ?
   { OMR::treeSimplification,                        OMR::IfEnabled                },
   { OMR::localDeadStoreElimination                                                }, // after latest copy propagation
   { OMR::deadTreesElimination                                                     }, // remove dead anchors created by check/store removal
   { OMR::treeSimplification,                        OMR::IfEnabled                },
   { OMR::localCSE                                                                 },
   { OMR::treeSimplification,                        OMR::MarkLastRun              },
#ifdef TR_HOST_S390
   { OMR::longRegA llocation                                                       },
#endif
   { OMR::andSimplification,                         OMR::IfEnabled                },  //clean up after versioner
   { OMR::deadTreesElimination,                      OMR::IfEnabled                }, // cleanup at the end
   { OMR::generalStoreSinking                                                      },
   { OMR::treesCleansing,                            OMR::IfEnabled                },
   { OMR::deadTreesElimination,                      OMR::IfEnabled                }, // cleanup at the end
   { OMR::localCSE,                                  OMR::IfEnabled                }, // common up expressions for sunk stores
   { OMR::treeSimplification,                        OMR::IfEnabledMarkLastRun     }, // cleanup the trees after sunk store and localCSE
   { OMR::trivialBlockExtension                                                    },
   { OMR::localDeadStoreElimination,                 OMR::IfEnabled                }, //remove the astore if no literal pool is required
   { OMR::localCSE,                                  OMR::IfEnabled                },  //common up lit pool refs in the same block
   { OMR::deadTreesElimination,                      OMR::IfEnabled                }, // cleanup at the end
   { OMR::treeSimplification,                        OMR::IfEnabledMarkLastRun     }, // Simplify non-normalized address computations introduced by prefetch insertion
   { OMR::trivialDeadTreeRemoval,                    OMR::IfEnabled                }, // final cleanup before opcode expansion
   { OMR::cheapTacticalGlobalRegisterAllocatorGroup, OMR::IfEnabled                },
   { OMR::globalDeadStoreGroup,                                                    },
   { OMR::rematerialization                                                        },
   { OMR::deadTreesElimination,                      OMR::IfEnabled                }, // remove dead anchors created by check/store removal
   { OMR::deadTreesElimination,                      OMR::IfEnabled                }, // remove dead RegStores produced by previous deadTrees pass
   //{ OMR::compactLocals                                                            },
   { OMR::regDepCopyRemoval                                                        },

#if 0
   // omrWarm strategy!
   { OMR::basicBlockExtension                  },
   { OMR::localCSE                             },
   { OMR::treeSimplification                   },
   { OMR::localCSE                             },
   { OMR::localDeadStoreElimination            },
   { OMR::globalDeadStoreGroup                 },
#endif

   { OMR::endOpts                                                                  },
   };


namespace JitBuilder
{

Optimizer::Optimizer(TR::Compilation *comp, TR::ResolvedMethodSymbol *methodSymbol, bool isIlGen,
      const OptimizationStrategy *strategy, uint16_t VNType)
   : OMR::Optimizer(comp, methodSymbol, isIlGen, strategy, VNType)
   {
//   _opts[OMR::inductionVariableAnalysis] =
//      new (comp->allocator()) TR::OptimizationManager(self(), TR_InductionVariableAnalysis::create, OMR::inductionVariableAnalysis, "O^O INDUCTION VARIABLE ANALYSIS: ");
   _opts[OMR::partialRedundancyElimination] =
      new (comp->allocator()) TR::OptimizationManager(self(), TR_PartialRedundancy::create, OMR::partialRedundancyElimination, "O^O PARTIAL REDUNDANCY ELIMINATION: ");
   _opts[OMR::isolatedStoreElimination] =
      new (comp->allocator()) TR::OptimizationManager(self(), TR_IsolatedStoreElimination::create, OMR::isolatedStoreElimination, "O^O ISOLATED STORE ELIMINATION: ");
   _opts[OMR::tacticalGlobalRegisterAllocator] =
      new (comp->allocator()) TR::OptimizationManager(self(), TR_GlobalRegisterAllocator::create, OMR::tacticalGlobalRegisterAllocator, "O^O GLOBAL REGISTER ASSIGNER: ");
   _opts[OMR::loopInversion] =
      new (comp->allocator()) TR::OptimizationManager(self(), TR_LoopInverter::create, OMR::loopInversion, "O^O LOOP INVERTER: ");
   _opts[OMR::loopSpecializer] =
      new (comp->allocator()) TR::OptimizationManager(self(), TR_LoopSpecializer::create, OMR::loopSpecializer, "O^O LOOP SPECIALIZER: ");
   _opts[OMR::trivialStoreSinking] =
      new (comp->allocator()) TR::OptimizationManager(self(), TR_TrivialSinkStores::create, OMR::trivialStoreSinking, "O^O TRIVIAL SINK STORES: ");
   _opts[OMR::generalStoreSinking] =
      new (comp->allocator()) TR::OptimizationManager(self(), TR_GeneralSinkStores::create, OMR::generalStoreSinking, "O^O GENERAL SINK STORES: ");
   _opts[OMR::liveRangeSplitter] =
      new (comp->allocator()) TR::OptimizationManager(self(), TR_LiveRangeSplitter::create, OMR::liveRangeSplitter, "O^O LIVE RANGE SPLITTER: ");
   _opts[OMR::redundantInductionVarElimination] =
      new (comp->allocator()) TR::OptimizationManager(self(), TR_RedundantInductionVarElimination::create, OMR::redundantInductionVarElimination, "O^O REDUNDANT INDUCTION VAR ELIMINATION: ");
   _opts[OMR::trivialDeadBlockRemover] =
      new (comp->allocator()) TR::OptimizationManager(self(), TR_TrivialDeadBlockRemover::create, OMR::trivialDeadBlockRemover, "O^O TRIVIAL DEAD BLOCK REMOVAL ");
   _opts[OMR::regDepCopyRemoval] =
      new (comp->allocator()) TR::OptimizationManager(self(), TR::RegDepCopyRemoval::create, OMR::regDepCopyRemoval, "O^O REGISTER DEPENDENCY COPY REMOVAL: ");
   // NOTE: Please add new IBM optimizations here!

   // initialize additional IBM optimization groups

   _opts[OMR::arrayPrivatizationGroup] =
      new (comp->allocator()) TR::OptimizationManager(self(), NULL, OMR::arrayPrivatizationGroup, "", arrayPrivatizationOpts);
   _opts[OMR::reorderArrayExprGroup] =
      new (comp->allocator()) TR::OptimizationManager(self(), NULL, OMR::reorderArrayExprGroup, "", reorderArrayIndexOpts);
   _opts[OMR::partialRedundancyEliminationGroup] =
      new (comp->allocator()) TR::OptimizationManager(self(), NULL, OMR::partialRedundancyEliminationGroup, "", partialRedundancyEliminationOpts);
   _opts[OMR::isolatedStoreGroup] =
      new (comp->allocator()) TR::OptimizationManager(self(), NULL, OMR::isolatedStoreGroup, "", isolatedStoreOpts);
   _opts[OMR::cheapTacticalGlobalRegisterAllocatorGroup] =
      new (comp->allocator()) TR::OptimizationManager(self(), NULL, OMR::cheapTacticalGlobalRegisterAllocatorGroup, "", cheapTacticalGlobalRegisterAllocatorOpts);
   _opts[OMR::tacticalGlobalRegisterAllocatorGroup] =
      new (comp->allocator()) TR::OptimizationManager(self(), NULL, OMR::tacticalGlobalRegisterAllocatorGroup, "", tacticalGlobalRegisterAllocatorOpts);
   _opts[OMR::finalGlobalGroup] =
      new (comp->allocator()) TR::OptimizationManager(self(), NULL, OMR::finalGlobalGroup, "", finalGlobalOpts);
   _opts[OMR::loopVersionerGroup] =
      new (comp->allocator()) TR::OptimizationManager(self(), NULL, OMR::loopVersionerGroup, "", loopVersionerOpts);
   _opts[OMR::lastLoopVersionerGroup] =
      new (comp->allocator()) TR::OptimizationManager(self(), NULL, OMR::lastLoopVersionerGroup, "", lastLoopVersionerOpts);
   _opts[OMR::blockManipulationGroup] =
      new (comp->allocator()) TR::OptimizationManager(self(), NULL, OMR::blockManipulationGroup, "", blockManipulationOpts);
   _opts[OMR::eachLocalAnalysisPassGroup] =
      new (comp->allocator()) TR::OptimizationManager(self(), NULL, OMR::eachLocalAnalysisPassGroup, "", eachLocalAnalysisPassOpts);

   // NOTE: Please add new IBM optimization groups here!

   // turn requested on for optimizations/groups
   self()->setRequestOptimization(OMR::cheapTacticalGlobalRegisterAllocatorGroup, true);
   self()->setRequestOptimization(OMR::tacticalGlobalRegisterAllocatorGroup, true);
   self()->setRequestOptimization(OMR::tacticalGlobalRegisterAllocator, true);

   // force warm strategy for now
   if (!isIlGen)
      self()->setStrategy(JBwarmStrategyOpts);
   }

const OptimizationStrategy *
Optimizer::optimizationStrategy(TR::Compilation *c)
   {
   // force warm strategy for now
   return JBwarmStrategyOpts;
   }

inline
TR::Optimizer *Optimizer::self()
   {
   return (static_cast<TR::Optimizer *>(this));
   }

} // namespace JitBuilder
