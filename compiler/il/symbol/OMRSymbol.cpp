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
 ******************************************************************************/

#include "il/symbol/OMRSymbol.hpp"

#include <stddef.h>                            // for size_t
#include <stdint.h>                            // for uint32_t, int32_t, etc
#include <string.h>                            // for memcmp, strncmp
#include "codegen/FrontEnd.hpp"                // for TR_FrontEnd
#include "compile/Compilation.hpp"             // for Compilation
#include "compile/ResolvedMethod.hpp"          // for TR_ResolvedMethod
#include "compile/SymbolReferenceTable.hpp"    // for SymbolReferenceTable
#include "env/KnownObjectTable.hpp"            // for KnownObjectTable, etc
#include "env/ObjectModel.hpp"                 // for ObjectModel
#include "env/TRMemory.hpp"
#include "il/DataTypes.hpp"                    // for DataType
#include "il/Symbol.hpp"                       // for Symbol
#include "il/SymbolReference.hpp"              // for SymbolReference
#include "il/symbol/AutomaticSymbol.hpp"       // for AutomaticSymbol
#include "il/symbol/RegisterMappedSymbol.hpp"  // for RegisterMappedSymbol
#include "il/symbol/StaticSymbol.hpp"          // for StaticSymbol
#include "infra/Assert.hpp"                    // for TR_ASSERT
#include "infra/Flags.hpp"                     // for flags32_t
#include "ras/Debug.hpp"                       // for TR_DebugBase

/**
 * Downcast to concrete instance
 *
 * This method is defined out-of-line because it requires a complete definition
 * of OMR::Symbol which is unavailable at the time the class OMR::Symbol is
 * being defined.
 *
 * This must be a static_cast or else the downcast will be unsafe.
 */
TR::Symbol *
OMR::Symbol::self()
   {
   return static_cast<TR::Symbol *>( this);
   }

template <typename AllocatorType>
TR::Symbol * OMR::Symbol::create(AllocatorType m)
   {
   return new (m) TR::Symbol();
   }

template <typename AllocatorType>
TR::Symbol * OMR::Symbol::create(AllocatorType m, TR::DataTypes d)
   {
   return new (m) TR::Symbol(d);
   }

template <typename AllocatorType>
TR::Symbol * OMR::Symbol::create(AllocatorType m, TR::DataTypes d, uint32_t s)
   {
   return new (m) TR::Symbol(d,s);
   }

bool
OMR::Symbol::isReferenced()
   {
   return isVariableSizeSymbol() && castToVariableSizeSymbol()->isReferenced();
   }

bool
OMR::Symbol::dontEliminateStores(TR::Compilation *comp, bool isForLocalDeadStore)
   {
   return (isAuto() && _flags.testAny(PinningArrayPointer)) ||
          (isParm() && _flags.testAny(ReinstatedReceiver)) ||
          _flags.testAny(HoldsMonitoredObject) ||
          (comp->getSymRefTab()->findThisRangeExtensionSymRef() && (self() == comp->getSymRefTab()->findThisRangeExtensionSymRef()->getSymbol()));
   }

uint32_t
OMR::Symbol::getNumberOfSlots()
   {
   uint32_t numSlots = getRoundedSize()/convertTypeToSize(TR::Address);

   // We should always give at least 1 slot.
   //  This is specifically for the case of an int type on 64bit pltfrms
   return (numSlots? numSlots : 1);
   }

TR::DataTypes
OMR::Symbol::convertSigCharToType(char sigChar)
   {
   switch (sigChar)
      {
      case 'Z': return TR::Int8;
      case 'B': return TR::Int8;
      case 'C': return TR::Int16;
      case 'S': return TR::Int16;
      case 'I': return TR::Int32;
      case 'J': return TR::Int64;
      case 'F': return TR::Float;
      case 'D': return TR::Double;
      case 'L':
      case '[': return TR::Address;
      }
   TR_ASSERT(0, "unknown signature character");
   return TR::Int32;
   }

/**
 * Sets the data type of a symbol, and the size, if the size can be inferred
 * from the data type.
 */
void
OMR::Symbol::setDataType(TR::DataTypes dt)
   {
   uint32_t inferredSize = TR::DataType::getSize(dt);
   if (inferredSize)
      {
      _size = inferredSize;
      }
   _flags.setValue(DataTypeMask, dt);
   }

uint32_t
OMR::Symbol::getRoundedSize()
   {
   int32_t roundedSize = (int32_t)((getSize()+3)&(~3)); // cast explicitly
   return roundedSize ? roundedSize : 4;
   }

uint32_t
OMR::Symbol::convertTypeToSize(TR::DataTypes dt)
   {
   //TR_ASSERT(dt != TR::Aggregate, "Cannot be called for aggregates");
   return TR::DataType::getSize(dt);
   }

uint32_t
OMR::Symbol::convertTypeToNumberOfSlots(TR::DataTypes dt)
   {
   return (dt == TR::Int64 || dt == TR::Double)? 2 : 1;
   }

int32_t
OMR::Symbol::getOffset()
   {
   TR::RegisterMappedSymbol * r = getRegisterMappedSymbol();
   return r ? r->getOffset() : 0;
   }

/**
 * Flag functions
 */

bool
OMR::Symbol::isCollectedReference()
   {
   return (getDataType()==TR::Address || isLocalObject()) && !isNotCollected();
   }

bool
OMR::Symbol::isInternalPointerAuto()
   {
   return (isInternalPointer() && isAuto());
   }

bool
OMR::Symbol::isNamed()
   {
   return isStatic() && _flags.testAny(IsNamed);
   }

void
OMR::Symbol::setSpillTempAuto()
   {
   TR_ASSERT(isAuto(), "assertion failure");
   _flags.set(SpillTemp);
   }

bool
OMR::Symbol::isSpillTempAuto()
   {
   return isAuto() && _flags.testAny(SpillTemp);
   }

void
OMR::Symbol::setLocalObject()
   {
   TR_ASSERT(isAuto(), "assertion failure");
   _flags.set(IsLocalObject);
   }

bool
OMR::Symbol::isLocalObject()
   {
   return isAuto() && _flags.testAny(IsLocalObject);
   }

void
OMR::Symbol::setBehaveLikeNonTemp()
   {
   TR_ASSERT(isAuto(), "assertion failure");
   _flags.set(BehaveLikeNonTemp);
   }

bool
OMR::Symbol::behaveLikeNonTemp()
   {
   return isAuto() && _flags.testAny(BehaveLikeNonTemp);
   }

void
OMR::Symbol::setPinningArrayPointer()
   {
   TR_ASSERT(isAuto(), "assertion failure");
   _flags.set(PinningArrayPointer);
   }

bool
OMR::Symbol::isPinningArrayPointer()
   {
   return isAuto() && _flags.testAny(PinningArrayPointer);
   }

bool
OMR::Symbol::isRegisterSymbol()
   {
   return isAuto() && _flags.testAny(RegisterAuto);
   }

void
OMR::Symbol::setIsRegisterSymbol()
   {
   _flags.set(RegisterAuto);
   }

void
OMR::Symbol::setAutoAddressTaken()
   {
   TR_ASSERT(isAuto(), "assertion failure");
   _flags.set(AutoAddressTaken);
   }

bool
OMR::Symbol::isAutoAddressTaken()
   {
   return isAuto() && _flags.testAny(AutoAddressTaken);
   }

void
OMR::Symbol::setSpillTempLoaded()
   {
   if (isAuto()) // For non auto spills (ie. to original location) we can't optimize removing spills
     _flags.set(SpillTempLoaded);
   }

bool
OMR::Symbol::isSpillTempLoaded()
   {
   return isSpillTempAuto() && _flags.testAny(SpillTempLoaded);
   }

void
OMR::Symbol::setAutoMarkerSymbol()
   {
   TR_ASSERT(isAuto(), "assertion failure");
   _flags.set(AutoMarkerSymbol);
   }

bool
OMR::Symbol::isAutoMarkerSymbol()
   {
   return (isAuto() && _flags.testAny(AutoMarkerSymbol));
   }

void
OMR::Symbol::setVariableSizeSymbol()
   {
   TR_ASSERT(isAuto(), "assertion failure");
   _flags.set(VariableSizeSymbol);
   }

bool
OMR::Symbol::isVariableSizeSymbol()
   {
   return (isAuto() && _flags.testAny(VariableSizeSymbol));
   }

void
OMR::Symbol::setThisTempForObjectCtor()
   {
   TR_ASSERT(isAuto(), "assertion failure");
   _flags.set(ThisTempForObjectCtor);
   }

bool
OMR::Symbol::isThisTempForObjectCtor()
   {
   return isAuto() && _flags.testAny(ThisTempForObjectCtor);
   }

void
OMR::Symbol::setParmHasToBeOnStack()
   {
   TR_ASSERT(isParm(), "assertion failure");
   _flags.set(ParmHasToBeOnStack);
   }

bool
OMR::Symbol::isParmHasToBeOnStack()
   {
   return isParm() && _flags.testAny(ParmHasToBeOnStack);
   }

void
OMR::Symbol::setReferencedParameter()
   {
   TR_ASSERT(isParm(), "assertion failure");
   _flags.set(ReferencedParameter);
   }

void
OMR::Symbol::resetReferencedParameter()
   {
   TR_ASSERT(isParm(), "assertion failure");
   _flags.reset(ReferencedParameter);
   }

bool
OMR::Symbol::isReferencedParameter()
   {
   return isParm() && _flags.testAny(ReferencedParameter);
   }

void
OMR::Symbol::setReinstatedReceiver()
   {
   TR_ASSERT(isParm(), "assertion failure");
   _flags.set(ReinstatedReceiver);
   }

bool
OMR::Symbol::isReinstatedReceiver()
   {
   return isParm() && _flags.testAny(ReinstatedReceiver);
   }

void
OMR::Symbol::setConstString()
   {
   TR_ASSERT(isStatic(), "assertion failure");
   _flags.set(ConstString);
   }

bool
OMR::Symbol::isConstString()
   {
   return isStatic() && _flags.testAny(ConstString);
   }

void
OMR::Symbol::setAddressIsCPIndexOfStatic(bool b)
   {
   TR_ASSERT(isStatic(), "assertion failure");
   _flags.set(AddressIsCPIndexOfStatic, b);
   }

bool
OMR::Symbol::addressIsCPIndexOfStatic()
   {
   return isStatic() && _flags.testAny(AddressIsCPIndexOfStatic);
   }

bool
OMR::Symbol::isRecognizedStatic()
   {
   return isStatic() && _flags.testAny(RecognizedStatic);
   }

void
OMR::Symbol::setCompiledMethod()
   {
   TR_ASSERT(isStatic(), "assertion failure");
   _flags.set(CompiledMethod);
   }

bool
OMR::Symbol::isCompiledMethod()
   {
   return isStatic() && _flags.testAny(CompiledMethod);
   }

void
OMR::Symbol::setStartPC()
   {
   TR_ASSERT(isStatic(), "assertion failure");
   _flags.set(StartPC);
   }

bool
OMR::Symbol::isStartPC()
   {
   return isStatic() && _flags.testAny(StartPC);
   }

void
OMR::Symbol::setCountForRecompile()
   {
   TR_ASSERT(isStatic(), "assertion failure");
   _flags.set(CountForRecompile);
   }

bool
OMR::Symbol::isCountForRecompile()
   {
   return isStatic() && _flags.testAny(CountForRecompile);
   }

void
OMR::Symbol::setRecompilationCounter()
   {
   TR_ASSERT(isStatic(), "assertion failure");
   _flags.set(RecompilationCounter);
   }

bool
OMR::Symbol::isRecompilationCounter()
   {
   return isStatic() && _flags.testAny(RecompilationCounter);
   }

void
OMR::Symbol::setGCRPatchPoint()
   {
   TR_ASSERT(isStatic(), "assertion failure");
   _flags.set(GCRPatchPoint);
   }

bool
OMR::Symbol::isGCRPatchPoint()
   {
   return isStatic() && _flags.testAny(GCRPatchPoint);
   }

bool
OMR::Symbol::isJittedMethod()
   {
   return isResolvedMethod() && _flags.testAny(IsJittedMethod);
   }

void
OMR::Symbol::setArrayShadowSymbol()
   {
   TR_ASSERT(isShadow(), "assertion failure");
   _flags.set(ArrayShadow);
   }

bool
OMR::Symbol::isArrayShadowSymbol()
   {
   return isShadow() && _flags.testAny(ArrayShadow);
   }

bool
OMR::Symbol::isRecognizedShadow()
   {
   return isShadow() && _flags.testAny(RecognizedShadow);
   }

void
OMR::Symbol::setArrayletShadowSymbol()
   {
   TR_ASSERT(isShadow(), "assertion failure");
   _flags.set(ArrayletShadow);
   }

bool
OMR::Symbol::isArrayletShadowSymbol()
   {
   return isShadow() && _flags.testAny(ArrayletShadow);
   }

void
OMR::Symbol::setPythonLocalVariableShadowSymbol()
   {
   TR_ASSERT(isShadow(), "assertion failure");
   _flags.set(PythonLocalVariable);
   }

bool
OMR::Symbol::isPythonLocalVariableShadowSymbol()
   {
   return isShadow() && _flags.testAny(PythonLocalVariable);
   }

void
OMR::Symbol::setGlobalFragmentShadowSymbol()
   {
   TR_ASSERT(isShadow(), "assertion failure");
   _flags.set(GlobalFragmentShadow);
   }

bool
OMR::Symbol::isGlobalFragmentShadowSymbol()
   {
   return isShadow() && _flags.testAny(GlobalFragmentShadow);
   }

void
OMR::Symbol::setMemoryTypeShadowSymbol()
   {
   TR_ASSERT(isShadow(), "assertion failure");
   _flags.set(MemoryTypeShadow);
   }

bool
OMR::Symbol::isMemoryTypeShadowSymbol()
   {
   return isShadow() && _flags.testAny(MemoryTypeShadow);
   }

void
OMR::Symbol::setPythonConstantShadowSymbol()
   {
   TR_ASSERT(isShadow(), "assertion failure");
   _flags.set(PythonConstant);
   }

bool
OMR::Symbol::isPythonConstantShadowSymbol()
   {
   return isShadow() && _flags.testAny(PythonConstant);
   }

void
OMR::Symbol::setPythonNameShadowSymbol()
   {
   TR_ASSERT(isShadow(), "assertion failure");
   _flags.set(PythonName);
   }

bool
OMR::Symbol::isPythonNameShadowSymbol()
   {
   return isShadow() && _flags.testAny(PythonName);
   }

void
OMR::Symbol::setStartOfColdInstructionStream()
   {
   TR_ASSERT(isLabel(), "assertion failure"); _flags.set(StartOfColdInstructionStream);
   }

bool
OMR::Symbol::isStartOfColdInstructionStream()
   {
   return isLabel() && _flags.testAny(StartOfColdInstructionStream);
   }

void
OMR::Symbol::setStartInternalControlFlow()
   {
   TR_ASSERT(isLabel(), "assertion failure"); _flags.set(StartInternalControlFlow);
   }

bool
OMR::Symbol::isStartInternalControlFlow()
   {
   return isLabel() && _flags.testAny(StartInternalControlFlow) && !isGlobalLabel();
   }

void
OMR::Symbol::setEndInternalControlFlow()
   {
   TR_ASSERT(isLabel(), "assertion failure"); _flags.set(EndInternalControlFlow);
   }

bool
OMR::Symbol::isEndInternalControlFlow()
   {
   return isLabel() && !isGlobalLabel() && _flags.testAny(EndInternalControlFlow) && !isGlobalLabel();
   }

void
OMR::Symbol::setVMThreadLive()
   {
   TR_ASSERT(isLabel(), "assertion failure"); _flags.set(IsVMThreadLive);
   }

bool
OMR::Symbol::isVMThreadLive()
   {
   return isLabel() && _flags.testAny(IsVMThreadLive);
   }

void
OMR::Symbol::setInternalControlFlowMerge()
   {
   TR_ASSERT(isLabel(), "assertion failure"); _flags.set(InternalControlFlowMerge);
   }

bool
OMR::Symbol::isInternalControlFlowMerge()
   {
   return isLabel() && _flags.testAny(InternalControlFlowMerge);
   }

void
OMR::Symbol::setEndOfColdInstructionStream()
   {
   TR_ASSERT(isLabel(), "assertion failure"); _flags.set(EndOfColdInstructionStream);
   }

bool
OMR::Symbol::isEndOfColdInstructionStream()
   {
   return isLabel() && _flags.testAny(EndOfColdInstructionStream);
   }

void
OMR::Symbol::setNonLinear()
   {
   _flags.set(NonLinear);
   }

bool
OMR::Symbol::isNonLinear()
   {
   return (isLabel() && _flags.testValue(OOLMask, (StartOfColdInstructionStream|NonLinear)));
   }

void
OMR::Symbol::setGlobalLabel()
   {
   TR_ASSERT(isLabel(), "assertion failure"); _flags.setValue(LabelKindMask, IsGlobalLabel);
   }

bool
OMR::Symbol::isGlobalLabel()
   {
   return isLabel() && _flags.testValue(LabelKindMask, IsGlobalLabel);
   }

void
OMR::Symbol::setRelativeLabel()
   {
   TR_ASSERT(isLabel(), "assertion failure"); _flags2.set(RelativeLabel);
   }

bool
OMR::Symbol::isRelativeLabel()
   {
   return isLabel() && _flags2.testAny(RelativeLabel);
   }

void
OMR::Symbol::setConstMethodType()
   {
   TR_ASSERT(isStatic(), "assertion failure");
   _flags2.set(ConstMethodType);
   }

bool
OMR::Symbol::isConstMethodType()
   {
   return isStatic() && _flags2.testAny(ConstMethodType);
   }

void
OMR::Symbol::setConstMethodHandle()
   {
   TR_ASSERT(isStatic(), "assertion failure");
   _flags2.set(ConstMethodHandle);
   }

bool
OMR::Symbol::isConstMethodHandle()
   {
   return isStatic() && _flags2.testAny(ConstMethodHandle);
   }

bool
OMR::Symbol::isConstObjectRef()
   {
   return isStatic() && (_flags.testAny(ConstString) || _flags2.testAny(ConstMethodType|ConstMethodHandle));
   }

bool
OMR::Symbol::isStaticField()
   {
   return isStatic() && !(isConstObjectRef() || isClassObject() || isAddressOfClassObject() || isConst());
   }

bool
OMR::Symbol::isFixedObjectRef()
   {
   return isConstObjectRef() || isCallSiteTableEntry() || isMethodTypeTableEntry();
   }

void
OMR::Symbol::setCallSiteTableEntry()
   {
   TR_ASSERT(isStatic(), "assertion failure");
   _flags2.set(CallSiteTableEntry);
   }

bool
OMR::Symbol::isCallSiteTableEntry()
   {
   return isStatic() && _flags2.testAny(CallSiteTableEntry);
   }

void
OMR::Symbol::setMethodTypeTableEntry()
   {
   TR_ASSERT(isStatic(), "assertion failure");
   _flags2.set(MethodTypeTableEntry);
   }

bool
OMR::Symbol::isMethodTypeTableEntry()
   {
   return isStatic() && _flags2.testAny(MethodTypeTableEntry);
   }

void
OMR::Symbol::setNotDataAddress()
   {
   TR_ASSERT(isStatic(), "assertion failure");
   _flags2.set(NotDataAddress);
   }

bool
OMR::Symbol::isNotDataAddress()
   {
   return isStatic() && _flags2.testAny(NotDataAddress);
   }

void
OMR::Symbol::setUnsafeShadowSymbol()
   {
   TR_ASSERT(isShadow(), "assertion failure"); _flags2.set(UnsafeShadow);
   }

bool
OMR::Symbol::isUnsafeShadowSymbol()
   {
   return isShadow() && _flags2.testAny(UnsafeShadow);
   }

void
OMR::Symbol::setNamedShadowSymbol()
   {
   TR_ASSERT(isShadow(), "assertion failure"); _flags2.set(NamedShadow);
   }

bool
OMR::Symbol::isNamedShadowSymbol()
   {
   return isShadow() && _flags2.testAny(NamedShadow);
   }

/*
 * Static factory methods
 */
template <typename AllocatorType>
TR::Symbol * OMR::Symbol::createShadow(AllocatorType m)
   {
   TR::Symbol * sym = new (m) TR::Symbol();
   sym->_flags.setValue(KindMask, IsShadow);
   return sym;
   }

template <typename AllocatorType>
TR::Symbol * OMR::Symbol::createShadow(AllocatorType m, TR::DataTypes d)
   {
   TR::Symbol * sym = new (m) TR::Symbol(d);
   sym->_flags.setValue(KindMask, IsShadow);
   return sym;
   }

template <typename AllocatorType>
TR::Symbol * OMR::Symbol::createShadow(AllocatorType m, TR::DataTypes d, uint32_t s)
   {
   TR::Symbol * sym = new (m) TR::Symbol(d,s);
   sym->_flags.setValue(KindMask, IsShadow);
   return sym;
   }

template <typename AllocatorType>
TR::Symbol * OMR::Symbol::createNamedShadow(AllocatorType m, TR::DataTypes d, uint32_t s, char *name)
   {
   auto * sym = createShadow(m,d,s);
   sym->_name = name;
   sym->_flags2.set(NamedShadow);
   return sym;
   }

/*
 * Explicit instantiation of factories for each TR_Memory type.
 */

template TR::Symbol * OMR::Symbol::create(TR_StackMemory);
template TR::Symbol * OMR::Symbol::create(TR_HeapMemory);
template TR::Symbol * OMR::Symbol::create(PERSISTENT_NEW_DECLARE);

template TR::Symbol * OMR::Symbol::create(TR_StackMemory, TR::DataTypes);
template TR::Symbol * OMR::Symbol::create(TR_HeapMemory, TR::DataTypes);
template TR::Symbol * OMR::Symbol::create(PERSISTENT_NEW_DECLARE, TR::DataTypes);

template TR::Symbol * OMR::Symbol::create(TR_StackMemory, TR::DataTypes, uint32_t);
template TR::Symbol * OMR::Symbol::create(TR_HeapMemory, TR::DataTypes, uint32_t);
template TR::Symbol * OMR::Symbol::create(PERSISTENT_NEW_DECLARE, TR::DataTypes, uint32_t);

template TR::Symbol * OMR::Symbol::createShadow(TR_StackMemory);
template TR::Symbol * OMR::Symbol::createShadow(TR_HeapMemory);
template TR::Symbol * OMR::Symbol::createShadow(PERSISTENT_NEW_DECLARE);

template TR::Symbol * OMR::Symbol::createShadow(TR_StackMemory,         TR::DataTypes);
template TR::Symbol * OMR::Symbol::createShadow(TR_HeapMemory,          TR::DataTypes);
template TR::Symbol * OMR::Symbol::createShadow(PERSISTENT_NEW_DECLARE, TR::DataTypes);

template TR::Symbol * OMR::Symbol::createShadow(TR_StackMemory,         TR::DataTypes, uint32_t);
template TR::Symbol * OMR::Symbol::createShadow(TR_HeapMemory,          TR::DataTypes, uint32_t);
template TR::Symbol * OMR::Symbol::createShadow(PERSISTENT_NEW_DECLARE, TR::DataTypes, uint32_t);

template TR::Symbol * OMR::Symbol::createNamedShadow(TR_StackMemory,         TR::DataTypes, uint32_t, char *);
template TR::Symbol * OMR::Symbol::createNamedShadow(TR_HeapMemory,          TR::DataTypes, uint32_t, char *);
template TR::Symbol * OMR::Symbol::createNamedShadow(PERSISTENT_NEW_DECLARE, TR::DataTypes, uint32_t, char *);
