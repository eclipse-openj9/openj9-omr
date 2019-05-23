###############################################################################
# Copyright (c) 2016, 2019 IBM Corp. and others
#
# This program and the accompanying materials are made available under
# the terms of the Eclipse Public License 2.0 which accompanies this
# distribution and is available at https://www.eclipse.org/legal/epl-2.0/
# or the Apache License, Version 2.0 which accompanies this distribution and
# is available at https://www.apache.org/licenses/LICENSE-2.0.
#
# This Source Code may also be made available under the following
# Secondary Licenses when the conditions for such availability set
# forth in the Eclipse Public License, v. 2.0 are satisfied: GNU
# General Public License, version 2 with the GNU Classpath
# Exception [1] and GNU General Public License, version 2 with the
# OpenJDK Assembly Exception [2].
#
# [1] https://www.gnu.org/software/classpath/license.html
# [2] http://openjdk.java.net/legal/assembly-exception.html
#
# SPDX-License-Identifier: EPL-2.0 OR Apache-2.0 OR GPL-2.0 WITH Classpath-exception-2.0 OR LicenseRef-GPL-2.0 WITH Assembly-exception
###############################################################################


JIT_PRODUCT_BACKEND_SOURCES+=\
    $(JIT_OMR_DIRTY_DIR)/compile/OSRData.cpp \
    $(JIT_OMR_DIRTY_DIR)/compile/Method.cpp \
    $(JIT_OMR_DIRTY_DIR)/compile/VirtualGuard.cpp \
    $(JIT_OMR_DIRTY_DIR)/control/OMROptions.cpp \
    $(JIT_OMR_DIRTY_DIR)/control/OptimizationPlan.cpp \
    $(JIT_OMR_DIRTY_DIR)/control/OMRRecompilation.cpp  \
    $(JIT_OMR_DIRTY_DIR)/env/ExceptionTable.cpp \
    $(JIT_OMR_DIRTY_DIR)/infra/Assert.cpp \
    $(JIT_OMR_DIRTY_DIR)/infra/BitVector.cpp \
    $(JIT_OMR_DIRTY_DIR)/infra/Checklist.cpp \
    $(JIT_OMR_DIRTY_DIR)/infra/HashTab.cpp \
    $(JIT_OMR_DIRTY_DIR)/infra/STLUtils.cpp \
    $(JIT_OMR_DIRTY_DIR)/infra/IGBase.cpp \
    $(JIT_OMR_DIRTY_DIR)/infra/IGNode.cpp \
    $(JIT_OMR_DIRTY_DIR)/infra/ILWalk.cpp \
    $(JIT_OMR_DIRTY_DIR)/infra/InterferenceGraph.cpp \
    $(JIT_OMR_DIRTY_DIR)/infra/OMRMonitor.cpp \
    $(JIT_OMR_DIRTY_DIR)/infra/OMRMonitorTable.cpp \
    $(JIT_OMR_DIRTY_DIR)/infra/Random.cpp \
    $(JIT_OMR_DIRTY_DIR)/infra/Timer.cpp \
    $(JIT_OMR_DIRTY_DIR)/infra/TreeServices.cpp \
    $(JIT_OMR_DIRTY_DIR)/infra/OMRCfg.cpp \
    $(JIT_OMR_DIRTY_DIR)/infra/SimpleRegex.cpp \
    $(JIT_OMR_DIRTY_DIR)/ilgen/IlGenRequest.cpp \
    $(JIT_OMR_DIRTY_DIR)/il/symbol/OMRSymbol.cpp \
    $(JIT_OMR_DIRTY_DIR)/il/OMRBlock.cpp \
    $(JIT_OMR_DIRTY_DIR)/compile/OMRSymbolReferenceTable.cpp \
    $(JIT_OMR_DIRTY_DIR)/compile/OMRAliasBuilder.cpp \
    $(JIT_OMR_DIRTY_DIR)/il/symbol/OMRAutomaticSymbol.cpp \
    $(JIT_OMR_DIRTY_DIR)/il/symbol/OMRLabelSymbol.cpp \
    $(JIT_OMR_DIRTY_DIR)/il/symbol/OMRMethodSymbol.cpp \
    $(JIT_OMR_DIRTY_DIR)/il/symbol/OMRParameterSymbol.cpp \
    $(JIT_OMR_DIRTY_DIR)/il/symbol/OMRRegisterMappedSymbol.cpp \
    $(JIT_OMR_DIRTY_DIR)/il/symbol/OMRResolvedMethodSymbol.cpp \
    $(JIT_OMR_DIRTY_DIR)/il/symbol/OMRStaticSymbol.cpp \
    $(JIT_OMR_DIRTY_DIR)/il/OMRNode.cpp \
    $(JIT_OMR_DIRTY_DIR)/il/NodePool.cpp \
    $(JIT_OMR_DIRTY_DIR)/il/NodeUtils.cpp \
    $(JIT_OMR_DIRTY_DIR)/il/OMRIL.cpp \
    $(JIT_OMR_DIRTY_DIR)/il/OMRSymbolReference.cpp \
    $(JIT_OMR_DIRTY_DIR)/il/Aliases.cpp \
    $(JIT_OMR_DIRTY_DIR)/compile/OMRCompilation.cpp \
    $(JIT_OMR_DIRTY_DIR)/compile/TLSCompilationManager.cpp \
    $(JIT_OMR_DIRTY_DIR)/env/OMRCPU.cpp \
    $(JIT_OMR_DIRTY_DIR)/env/OMRObjectModel.cpp \
    $(JIT_OMR_DIRTY_DIR)/env/OMRArithEnv.cpp \
    $(JIT_OMR_DIRTY_DIR)/env/OMRClassEnv.cpp \
    $(JIT_OMR_DIRTY_DIR)/env/OMRDebugEnv.cpp \
    $(JIT_OMR_DIRTY_DIR)/env/OMRVMEnv.cpp \
    $(JIT_OMR_DIRTY_DIR)/env/OMRVMMethodEnv.cpp \
    $(JIT_OMR_DIRTY_DIR)/env/SegmentProvider.cpp \
    $(JIT_OMR_DIRTY_DIR)/env/SegmentAllocator.cpp \
    $(JIT_OMR_DIRTY_DIR)/env/SystemSegmentProvider.cpp \
    $(JIT_OMR_DIRTY_DIR)/env/DebugSegmentProvider.cpp \
    $(JIT_OMR_DIRTY_DIR)/env/Region.cpp \
    $(JIT_OMR_DIRTY_DIR)/env/StackMemoryRegion.cpp \
    $(JIT_OMR_DIRTY_DIR)/env/OMRPersistentInfo.cpp \
    $(JIT_OMR_DIRTY_DIR)/env/TRMemory.cpp \
    $(JIT_OMR_DIRTY_DIR)/env/TRPersistentMemory.cpp \
    $(JIT_OMR_DIRTY_DIR)/env/VerboseLog.cpp \
    $(JIT_OMR_DIRTY_DIR)/il/OMRDataTypes.cpp \
    $(JIT_OMR_DIRTY_DIR)/il/OMRTreeTop.cpp \
    $(JIT_OMR_DIRTY_DIR)/il/OMRILOps.cpp \
    $(JIT_OMR_DIRTY_DIR)/ras/CallStack.cpp \
    $(JIT_OMR_DIRTY_DIR)/ras/CFGChecker.cpp \
    $(JIT_OMR_DIRTY_DIR)/ras/Debug.cpp \
    $(JIT_OMR_DIRTY_DIR)/ras/DebugCounter.cpp \
    $(JIT_OMR_DIRTY_DIR)/ras/IgnoreLocale.cpp \
    $(JIT_OMR_DIRTY_DIR)/ras/LimitFile.cpp \
    $(JIT_OMR_DIRTY_DIR)/ras/LogTracer.cpp \
    $(JIT_OMR_DIRTY_DIR)/ras/OptionsDebug.cpp \
    $(JIT_OMR_DIRTY_DIR)/ras/PPCOpNames.cpp \
    $(JIT_OMR_DIRTY_DIR)/ras/Tree.cpp \
    $(JIT_OMR_DIRTY_DIR)/ras/ILValidationRules.cpp \
    $(JIT_OMR_DIRTY_DIR)/ras/ILValidationUtils.cpp \
    $(JIT_OMR_DIRTY_DIR)/ras/ILValidator.cpp \
    $(JIT_OMR_DIRTY_DIR)/optimizer/AsyncCheckInsertion.cpp \
    $(JIT_OMR_DIRTY_DIR)/optimizer/BackwardBitVectorAnalysis.cpp \
    $(JIT_OMR_DIRTY_DIR)/optimizer/BackwardIntersectionBitVectorAnalysis.cpp \
    $(JIT_OMR_DIRTY_DIR)/optimizer/BackwardUnionBitVectorAnalysis.cpp \
    $(JIT_OMR_DIRTY_DIR)/optimizer/BitVectorAnalysis.cpp \
    $(JIT_OMR_DIRTY_DIR)/optimizer/CatchBlockRemover.cpp \
    $(JIT_OMR_DIRTY_DIR)/optimizer/CFGSimplifier.cpp \
    $(JIT_OMR_DIRTY_DIR)/optimizer/CompactLocals.cpp \
    $(JIT_OMR_DIRTY_DIR)/optimizer/CopyPropagation.cpp \
    $(JIT_OMR_DIRTY_DIR)/optimizer/DataFlowAnalysis.cpp \
    $(JIT_OMR_DIRTY_DIR)/optimizer/DeadStoreElimination.cpp \
    $(JIT_OMR_DIRTY_DIR)/optimizer/DeadTreesElimination.cpp \
    $(JIT_OMR_DIRTY_DIR)/optimizer/DebuggingCounters.cpp \
    $(JIT_OMR_DIRTY_DIR)/optimizer/Delayedness.cpp \
    $(JIT_OMR_DIRTY_DIR)/optimizer/Dominators.cpp \
    $(JIT_OMR_DIRTY_DIR)/optimizer/DominatorVerifier.cpp \
    $(JIT_OMR_DIRTY_DIR)/optimizer/DominatorsChk.cpp \
    $(JIT_OMR_DIRTY_DIR)/optimizer/Earliestness.cpp \
    $(JIT_OMR_DIRTY_DIR)/optimizer/ExpressionsSimplification.cpp \
    $(JIT_OMR_DIRTY_DIR)/optimizer/FieldPrivatizer.cpp \
    $(JIT_OMR_DIRTY_DIR)/optimizer/GeneralLoopUnroller.cpp \
    $(JIT_OMR_DIRTY_DIR)/optimizer/GlobalAnticipatability.cpp \
    $(JIT_OMR_DIRTY_DIR)/optimizer/GlobalRegisterAllocator.cpp \
    $(JIT_OMR_DIRTY_DIR)/optimizer/Inliner.cpp \
    $(JIT_OMR_DIRTY_DIR)/optimizer/RematTools.cpp \
    $(JIT_OMR_DIRTY_DIR)/optimizer/InductionVariable.cpp \
    $(JIT_OMR_DIRTY_DIR)/optimizer/IntersectionBitVectorAnalysis.cpp \
    $(JIT_OMR_DIRTY_DIR)/optimizer/Isolatedness.cpp \
    $(JIT_OMR_DIRTY_DIR)/optimizer/IsolatedStoreElimination.cpp \
    $(JIT_OMR_DIRTY_DIR)/optimizer/Latestness.cpp \
    $(JIT_OMR_DIRTY_DIR)/optimizer/LiveOnAllPaths.cpp \
    $(JIT_OMR_DIRTY_DIR)/optimizer/LiveVariableInformation.cpp \
    $(JIT_OMR_DIRTY_DIR)/optimizer/Liveness.cpp \
    $(JIT_OMR_DIRTY_DIR)/optimizer/LoadExtensions.cpp \
    $(JIT_OMR_DIRTY_DIR)/optimizer/LocalAnalysis.cpp \
    $(JIT_OMR_DIRTY_DIR)/optimizer/LocalAnticipatability.cpp \
    $(JIT_OMR_DIRTY_DIR)/optimizer/LocalLiveRangeReducer.cpp \
    $(JIT_OMR_DIRTY_DIR)/optimizer/LocalReordering.cpp \
    $(JIT_OMR_DIRTY_DIR)/optimizer/LocalTransparency.cpp \
    $(JIT_OMR_DIRTY_DIR)/optimizer/LoopCanonicalizer.cpp \
    $(JIT_OMR_DIRTY_DIR)/optimizer/LoopReducer.cpp \
    $(JIT_OMR_DIRTY_DIR)/optimizer/LoopReplicator.cpp \
    $(JIT_OMR_DIRTY_DIR)/optimizer/LoopVersioner.cpp \
    $(JIT_OMR_DIRTY_DIR)/optimizer/OMRLocalCSE.cpp \
    $(JIT_OMR_DIRTY_DIR)/optimizer/LocalDeadStoreElimination.cpp \
    $(JIT_OMR_DIRTY_DIR)/optimizer/LocalOpts.cpp \
    $(JIT_OMR_DIRTY_DIR)/optimizer/OMROptimization.cpp \
    $(JIT_OMR_DIRTY_DIR)/optimizer/OMROptimizationManager.cpp \
    $(JIT_OMR_DIRTY_DIR)/optimizer/OMRTransformUtil.cpp \
    $(JIT_OMR_DIRTY_DIR)/optimizer/OMROptimizer.cpp \
    $(JIT_OMR_DIRTY_DIR)/optimizer/OrderBlocks.cpp \
    $(JIT_OMR_DIRTY_DIR)/optimizer/OSRDefAnalysis.cpp \
    $(JIT_OMR_DIRTY_DIR)/optimizer/PartialRedundancy.cpp \
    $(JIT_OMR_DIRTY_DIR)/optimizer/PreExistence.cpp \
    $(JIT_OMR_DIRTY_DIR)/optimizer/PrefetchInsertion.cpp \
    $(JIT_OMR_DIRTY_DIR)/optimizer/Reachability.cpp \
    $(JIT_OMR_DIRTY_DIR)/optimizer/ReachingDefinitions.cpp \
    $(JIT_OMR_DIRTY_DIR)/optimizer/OMRRecognizedCallTransformer.cpp \
    $(JIT_OMR_DIRTY_DIR)/optimizer/RedundantAsyncCheckRemoval.cpp \
    $(JIT_OMR_DIRTY_DIR)/optimizer/RegisterCandidate.cpp \
    $(JIT_OMR_DIRTY_DIR)/optimizer/ReorderIndexExpr.cpp \
    $(JIT_OMR_DIRTY_DIR)/optimizer/SinkStores.cpp \
    $(JIT_OMR_DIRTY_DIR)/optimizer/StripMiner.cpp \
    $(JIT_OMR_DIRTY_DIR)/optimizer/VPConstraint.cpp \
    $(JIT_OMR_DIRTY_DIR)/optimizer/VPHandlers.cpp \
    $(JIT_OMR_DIRTY_DIR)/optimizer/VPHandlersCommon.cpp \
    $(JIT_OMR_DIRTY_DIR)/optimizer/OMRValuePropagation.cpp \
    $(JIT_OMR_DIRTY_DIR)/optimizer/ValuePropagationCommon.cpp \
    $(JIT_OMR_DIRTY_DIR)/optimizer/OMRSimplifier.cpp \
    $(JIT_OMR_DIRTY_DIR)/optimizer/OMRSimplifierHelpers.cpp \
    $(JIT_OMR_DIRTY_DIR)/optimizer/OMRSimplifierHandlers.cpp \
    $(JIT_OMR_DIRTY_DIR)/optimizer/RegDepCopyRemoval.cpp \
    $(JIT_OMR_DIRTY_DIR)/optimizer/StructuralAnalysis.cpp \
    $(JIT_OMR_DIRTY_DIR)/optimizer/Structure.cpp \
    $(JIT_OMR_DIRTY_DIR)/optimizer/SwitchAnalyzer.cpp \
    $(JIT_OMR_DIRTY_DIR)/optimizer/TranslateTable.cpp \
    $(JIT_OMR_DIRTY_DIR)/optimizer/TrivialDeadBlockRemover.cpp \
    $(JIT_OMR_DIRTY_DIR)/optimizer/UnionBitVectorAnalysis.cpp \
    $(JIT_OMR_DIRTY_DIR)/optimizer/UseDefInfo.cpp \
    $(JIT_OMR_DIRTY_DIR)/optimizer/ValueNumberInfo.cpp \
    $(JIT_OMR_DIRTY_DIR)/optimizer/VirtualGuardCoalescer.cpp \
    $(JIT_OMR_DIRTY_DIR)/optimizer/VirtualGuardHeadMerger.cpp \
    $(JIT_OMR_DIRTY_DIR)/codegen/OMRAheadOfTimeCompile.cpp \
    $(JIT_OMR_DIRTY_DIR)/codegen/Analyser.cpp \
    $(JIT_OMR_DIRTY_DIR)/codegen/CodeGenPrep.cpp \
    $(JIT_OMR_DIRTY_DIR)/codegen/CodeGenGC.cpp \
    $(JIT_OMR_DIRTY_DIR)/codegen/CodeGenRA.cpp \
    $(JIT_OMR_DIRTY_DIR)/codegen/FrontEnd.cpp \
    $(JIT_OMR_DIRTY_DIR)/codegen/OMRGCRegisterMap.cpp \
    $(JIT_OMR_DIRTY_DIR)/codegen/OMRGCStackAtlas.cpp \
    $(JIT_OMR_DIRTY_DIR)/codegen/OMRLinkage.cpp \
    $(JIT_OMR_DIRTY_DIR)/codegen/LiveRegister.cpp \
    $(JIT_OMR_DIRTY_DIR)/codegen/OutOfLineCodeSection.cpp \
    $(JIT_OMR_DIRTY_DIR)/codegen/Relocation.cpp \
    $(JIT_OMR_DIRTY_DIR)/codegen/RegisterIterator.cpp \
    $(JIT_OMR_DIRTY_DIR)/codegen/ScratchRegisterManager.cpp \
    $(JIT_OMR_DIRTY_DIR)/codegen/StorageInfo.cpp \
    $(JIT_OMR_DIRTY_DIR)/codegen/OMRTreeEvaluator.cpp \
    $(JIT_OMR_DIRTY_DIR)/codegen/PreInstructionSelection.cpp \
    $(JIT_OMR_DIRTY_DIR)/codegen/NodeEvaluation.cpp \
    $(JIT_OMR_DIRTY_DIR)/codegen/OMRSnippet.cpp \
    $(JIT_OMR_DIRTY_DIR)/codegen/OMRUnresolvedDataSnippet.cpp \
    $(JIT_OMR_DIRTY_DIR)/codegen/OMRSnippetGCMap.cpp \
    $(JIT_OMR_DIRTY_DIR)/codegen/OMRCodeGenerator.cpp \
    $(JIT_OMR_DIRTY_DIR)/codegen/OMRCodeGenPhase.cpp \
    $(JIT_OMR_DIRTY_DIR)/codegen/OMRMemoryReference.cpp \
    $(JIT_OMR_DIRTY_DIR)/codegen/OMRMachine.cpp \
    $(JIT_OMR_DIRTY_DIR)/codegen/OMRRegister.cpp \
    $(JIT_OMR_DIRTY_DIR)/codegen/OMRRealRegister.cpp \
    $(JIT_OMR_DIRTY_DIR)/codegen/OMRRegisterPair.cpp \
    $(JIT_OMR_DIRTY_DIR)/codegen/OMRInstruction.cpp \
    $(JIT_OMR_DIRTY_DIR)/codegen/ELFGenerator.cpp \
    $(JIT_OMR_DIRTY_DIR)/codegen/OMRELFRelocationResolver.cpp \
    $(JIT_OMR_DIRTY_DIR)/env/FEBase.cpp \
    $(JIT_OMR_DIRTY_DIR)/env/JitConfig.cpp \
    $(JIT_OMR_DIRTY_DIR)/control/CompilationController.cpp \
    $(JIT_OMR_DIRTY_DIR)/optimizer/FEInliner.cpp \
    $(JIT_OMR_DIRTY_DIR)/runtime/Runtime.cpp \
    $(JIT_OMR_DIRTY_DIR)/runtime/Trampoline.cpp \
    $(JIT_OMR_DIRTY_DIR)/control/CompileMethod.cpp \
    $(JIT_OMR_DIRTY_DIR)/env/OMRIO.cpp \
    $(JIT_OMR_DIRTY_DIR)/env/OMRKnownObjectTable.cpp \
    $(JIT_OMR_DIRTY_DIR)/env/Globals.cpp \
    $(JIT_OMR_DIRTY_DIR)/ilgen/IlInjector.cpp \
    $(JIT_OMR_DIRTY_DIR)/ilgen/OMRBytecodeBuilder.cpp \
    $(JIT_OMR_DIRTY_DIR)/ilgen/OMRIlBuilder.cpp \
    $(JIT_OMR_DIRTY_DIR)/ilgen/OMRIlType.cpp \
    $(JIT_OMR_DIRTY_DIR)/ilgen/OMRIlValue.cpp \
    $(JIT_OMR_DIRTY_DIR)/ilgen/OMRJitBuilderRecorder.cpp \
    $(JIT_OMR_DIRTY_DIR)/ilgen/OMRJitBuilderRecorderBinaryBuffer.cpp \
    $(JIT_OMR_DIRTY_DIR)/ilgen/OMRJitBuilderRecorderBinaryFile.cpp \
    $(JIT_OMR_DIRTY_DIR)/ilgen/OMRJitBuilderRecorderTextFile.cpp \
    $(JIT_OMR_DIRTY_DIR)/ilgen/OMRMethodBuilder.cpp \
    $(JIT_OMR_DIRTY_DIR)/ilgen/OMRThunkBuilder.cpp \
    $(JIT_OMR_DIRTY_DIR)/ilgen/OMRTypeDictionary.cpp \
    $(JIT_OMR_DIRTY_DIR)/ilgen/OMRVirtualMachineOperandArray.cpp \
    $(JIT_OMR_DIRTY_DIR)/ilgen/OMRVirtualMachineOperandStack.cpp \
    $(JIT_OMR_DIRTY_DIR)/ilgen/OMRVirtualMachineRegister.cpp \
    $(JIT_OMR_DIRTY_DIR)/ilgen/OMRVirtualMachineRegisterInStruct.cpp \
    $(JIT_OMR_DIRTY_DIR)/ilgen/OMRVirtualMachineState.cpp \
    $(JIT_OMR_DIRTY_DIR)/runtime/Alignment.cpp \
    $(JIT_OMR_DIRTY_DIR)/runtime/CodeCacheTypes.cpp \
    $(JIT_OMR_DIRTY_DIR)/runtime/OMRCodeCache.cpp \
    $(JIT_OMR_DIRTY_DIR)/runtime/OMRCodeCacheManager.cpp \
    $(JIT_OMR_DIRTY_DIR)/runtime/OMRCodeCacheMemorySegment.cpp \
    $(JIT_OMR_DIRTY_DIR)/runtime/OMRCodeCacheConfig.cpp \
    $(JIT_OMR_DIRTY_DIR)/env/OMRCompilerEnv.cpp \
    $(JIT_OMR_DIRTY_DIR)/env/PersistentAllocator.cpp \
    $(JIT_PRODUCT_DIR)/compile/Method.cpp \
    $(JIT_PRODUCT_DIR)/control/Jit.cpp \
    $(JIT_PRODUCT_DIR)/env/FrontEnd.cpp \
    $(JIT_PRODUCT_DIR)/ilgen/JBIlGeneratorMethodDetails.cpp \
    $(JIT_PRODUCT_DIR)/optimizer/JBOptimizer.cpp \
    $(JIT_PRODUCT_DIR)/runtime/JBCodeCacheManager.cpp \
    $(JIT_PRODUCT_DIR)/runtime/JBJitConfig.cpp \

CPP_GENERATED_SOURCE_DIR=$(JIT_PRODUCT_DIR)/client/cpp
CPP_GENERATED_API_SOURCES+=\
    $(CPP_GENERATED_SOURCE_DIR)/BytecodeBuilder.cpp \
    $(CPP_GENERATED_SOURCE_DIR)/IlBuilder.cpp \
    $(CPP_GENERATED_SOURCE_DIR)/IlType.cpp \
    $(CPP_GENERATED_SOURCE_DIR)/IlValue.cpp \
    $(CPP_GENERATED_SOURCE_DIR)/MethodBuilder.cpp \
    $(CPP_GENERATED_SOURCE_DIR)/ThunkBuilder.cpp \
    $(CPP_GENERATED_SOURCE_DIR)/TypeDictionary.cpp \
    $(CPP_GENERATED_SOURCE_DIR)/VirtualMachineOperandArray.cpp \
    $(CPP_GENERATED_SOURCE_DIR)/VirtualMachineOperandStack.cpp \
    $(CPP_GENERATED_SOURCE_DIR)/VirtualMachineRegister.cpp \
    $(CPP_GENERATED_SOURCE_DIR)/VirtualMachineRegisterInStruct.cpp \
    $(CPP_GENERATED_SOURCE_DIR)/VirtualMachineState.cpp \
    $(CPP_GENERATED_SOURCE_DIR)/JitBuilder.cpp

CPP_GENERATED_HEADER_DIR=$(JIT_PRODUCT_DIR)/release/cpp/include
CPP_GENERATED_API_HEADERS+=\
    $(CPP_GENERATED_HEADER_DIR)/BytecodeBuilder.hpp \
    $(CPP_GENERATED_HEADER_DIR)/IlBuilder.hpp \
    $(CPP_GENERATED_HEADER_DIR)/IlType.hpp \
    $(CPP_GENERATED_HEADER_DIR)/IlValue.hpp \
    $(CPP_GENERATED_HEADER_DIR)/MethodBuilder.hpp \
    $(CPP_GENERATED_HEADER_DIR)/ThunkBuilder.hpp \
    $(CPP_GENERATED_HEADER_DIR)/TypeDictionary.hpp \
    $(CPP_GENERATED_HEADER_DIR)/VirtualMachineOperandArray.hpp \
    $(CPP_GENERATED_HEADER_DIR)/VirtualMachineOperandStack.hpp \
    $(CPP_GENERATED_HEADER_DIR)/VirtualMachineRegister.hpp \
    $(CPP_GENERATED_HEADER_DIR)/VirtualMachineRegisterInStruct.hpp \
    $(CPP_GENERATED_HEADER_DIR)/VirtualMachineState.hpp \
    $(CPP_GENERATED_HEADER_DIR)/JitBuilder.hpp

CPP_API_GENERATOR=$(JIT_PRODUCT_DIR)/apigen/cppgen.py
JITBUILDER_API_DESCRIPTION=$(JIT_PRODUCT_DIR)/apigen/jitbuilder.api.json

include $(JIT_MAKE_DIR)/files/host/$(HOST_ARCH).mk
include $(JIT_MAKE_DIR)/files/target/$(TARGET_ARCH).mk
