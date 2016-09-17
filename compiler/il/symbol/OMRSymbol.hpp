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

/**
 * \page symbolcreation Symbol Creation
 *
 * For historical reasons, there are a number of relatively special case
 * properties on some symbols.
 *
 * In order to create symbols with those properties, without creating
 * dozens of subclasses, we have chosen to create [special-case symbol factories.][design]
 *
 * For consistency's sake, we therefore create all symbols through
 * factory methods.
 *
 */

#ifndef OMR_SYMBOL_INCL
#define OMR_SYMBOL_INCL

/*
 * The following #define and typedef must appear before any #includes in this file
 */
#ifndef OMR_SYMBOL_CONNECTOR
#define OMR_SYMBOL_CONNECTOR
namespace OMR { class Symbol; }
namespace OMR { typedef OMR::Symbol SymbolConnector; }
#endif

#include <stddef.h>                 // for size_t
#include <stdint.h>                 // for uint32_t, uint16_t, uint8_t, etc
#include "env/TRMemory.hpp"         // for TR_Memory, etc
#include "il/DataTypes.hpp"         // for TR::DataType, DataTypes
#include "infra/Assert.hpp"         // for TR_ASSERT
#include "infra/Flags.hpp"          // for flags32_t

class TR_FrontEnd;
class TR_ResolvedMethod;
namespace TR { class AutomaticSymbol; }
namespace TR { class Compilation; }
namespace TR { class LabelSymbol; }
namespace TR { class MethodSymbol; }
namespace TR { class ParameterSymbol; }
namespace TR { class RegisterMappedSymbol; }
namespace TR { class ResolvedMethodSymbol; }
namespace TR { class StaticSymbol; }
namespace TR { class Symbol; }

namespace OMR {

/**
 * Symbol
 *
 * A Symbol object contains data type, size and attribute
 * information for a symbol.
 */
class Symbol
   {

public:
   TR_ALLOC(TR_Memory::Symbol)

   /**
    * \brief Downcast to concrete instance
    */
   TR::Symbol * self();

   template <typename AllocatorType>
   static TR::Symbol * create(AllocatorType);

   template <typename AllocatorType>
   static TR::Symbol * create(AllocatorType, TR::DataTypes);

   template <typename AllocatorType>
   static TR::Symbol * create(AllocatorType, TR::DataTypes, uint32_t);

protected:

   /**
    * Generic constructor, sets size to 0
    */
   Symbol() :
      _size(0),
      _name(0),
      _flags(0),
      _flags2(0),
      _sideTableIndex(0),
      _restrictedRegisterNumber(-1)
   { }

   /**
    * Create symbol of specified data type, inferring size
    * from type.
    */
   Symbol(TR::DataTypes d) :
      _size(0),
      _name(0),
      _flags(0),
      _flags2(0),
      _sideTableIndex(0),
      _restrictedRegisterNumber(-1)
      {
      setDataType(d);
      }

   /**
    * Create symbol of specified data type, inferring size
    * from type.
    */
   Symbol(TR::DataTypes d, uint32_t size) :
      _name(0),
      _flags(0),
      _flags2(0),
      _sideTableIndex(0),
      _restrictedRegisterNumber(-1)
      {
      setDataType(d);
      _size = size;
      }

public:
   /**
    * This destructor must be virtual since we rarely use pointers directly to
    * derived classes and often free through a pointer to this class.
    */
   virtual ~Symbol() {}

   // If the symbol is of the correct type the inline get method downcasts the
   // symbol to the correct type otherwise it returns 0.
   //
   inline TR::RegisterMappedSymbol            *getRegisterMappedSymbol();
   inline TR::AutomaticSymbol                 *getAutoSymbol();
   inline TR::ParameterSymbol                 *getParmSymbol();
   inline TR::AutomaticSymbol                 *getInternalPointerAutoSymbol();
   inline TR::AutomaticSymbol                 *getLocalObjectSymbol();
   inline TR::StaticSymbol                    *getStaticSymbol();
   inline TR::ResolvedMethodSymbol            *getResolvedMethodSymbol();
   inline TR::MethodSymbol                    *getMethodSymbol();
   inline TR::Symbol                          *getShadowSymbol();
   inline TR::Symbol                          *getNamedShadowSymbol();
   inline TR::RegisterMappedSymbol            *getMethodMetaDataSymbol();
   inline TR::LabelSymbol                     *getLabelSymbol();
   inline TR::ResolvedMethodSymbol            *getJittedMethodSymbol();
   inline TR::StaticSymbol                    *getRecognizedStaticSymbol();
   inline TR::AutomaticSymbol                 *getVariableSizeSymbol();
   inline TR::StaticSymbol                    *getCallSiteTableEntrySymbol();
   inline TR::StaticSymbol                    *getMethodTypeTableEntrySymbol();
   inline TR::AutomaticSymbol                 *getRegisterSymbol();

   // The inline To methods perform an explicit (debug) assume that the symbol is a correct type
   // and then return the symbol explicitly cast to the type.
   //
   inline TR::RegisterMappedSymbol            *castToRegisterMappedSymbol();
   inline TR::AutomaticSymbol                 *castToAutoSymbol();
   inline TR::AutomaticSymbol                 *castToVariableSizeSymbol();
   inline TR::AutomaticSymbol                 *castToAutoMarkerSymbol();
   inline TR::ParameterSymbol                 *castToParmSymbol();
   inline TR::AutomaticSymbol                 *castToInternalPointerAutoSymbol();
   inline TR::AutomaticSymbol                 *castToLocalObjectSymbol();
   inline TR::ResolvedMethodSymbol            *castToResolvedMethodSymbol();
   inline TR::MethodSymbol                    *castToMethodSymbol();
   inline TR::Symbol                          *castToShadowSymbol();
   inline TR::RegisterMappedSymbol            *castToMethodMetaDataSymbol();
   inline TR::LabelSymbol                     *castToLabelSymbol();
   inline TR::ResolvedMethodSymbol            *castToJittedMethodSymbol();
   inline TR::AutomaticSymbol                 *castToRegisterSymbol();

   inline TR::StaticSymbol                    *castToStaticSymbol();
   inline TR::StaticSymbol                    *castToNamedStaticSymbol();
   inline TR::StaticSymbol                    *castToCallSiteTableEntrySymbol();
   inline TR::StaticSymbol                    *castToMethodTypeTableEntrySymbol();

   int32_t getOffset();

   uint32_t getNumberOfSlots();

   bool dontEliminateStores(TR::Compilation *comp, bool isForLocalDeadStore = false);

   bool isReferenced();

   static uint32_t convertTypeToSize(TR::DataTypes dt);

   static uint32_t convertTypeToNumberOfSlots(TR::DataTypes dt);

   static TR::DataTypes convertSigCharToType(char sigChar);

   /**
    * Field functions
    */

   size_t   getSize()                       { return _size; }
   void     setSize(size_t s)               { _size = s; }
   uint32_t getRoundedSize();

   const char * getName()                   { return _name; }
   void         setName(const char * name)  { _name = name; }

   uint32_t getFlags()                      { return _flags.getValue(); }
   uint32_t getFlags2()                     { return _flags2.getValue(); }
   void setFlagValue(uint32_t v, bool b)    { _flags.setValue(v, b); }

   uint16_t getSideTableIndex()             { return _sideTableIndex; }
   uint16_t setSideTableIndex(uint16_t sti) { return (_sideTableIndex = sti); }

   uint8_t getRestrictedRegisterNumber()    { return _restrictedRegisterNumber; }

   /**
    * Flag functions
    */

   void          setDataType(TR::DataTypes dt);
   TR::DataTypes getDataType() { return (TR::DataTypes)_flags.getValue(DataTypeMask);}
   TR::DataType  getType()     { return getDataType(); }

   int32_t getKind()             { return _flags.getValue(KindMask);}

   bool isAuto()                 { return _flags.testValue(KindMask, IsAutomatic); }
   bool isParm()                 { return _flags.testValue(KindMask, IsParameter); }
   bool isMethodMetaData()       { return _flags.testValue(KindMask, IsMethodMetaData); }
   bool isResolvedMethod()       { return _flags.testValue(KindMask, IsResolvedMethod); }
   bool isMethod()               { return getKind() == IsMethod || getKind() == IsResolvedMethod; }
   bool isStatic()               { return _flags.testValue(KindMask, IsStatic); }
   bool isShadow()               { return _flags.testValue(KindMask, IsShadow); }
   bool isLabel()                { return _flags.testValue(KindMask, IsLabel); }
   void setIsLabel()             { _flags.setValue(KindMask, IsLabel);}

   bool isRegisterMappedSymbol() { return getKind() <= LastRegisterMapped; }

   bool isAutoOrParm()           { return getKind() <= IsParameter; }
   bool isAutoField()            { return false; }
   bool isParmField()            { return false; }
   bool isWeakSymbol()           { return false; }
   bool isRegularShadow()        { return isShadow() && !isAutoField() && !isParmField(); }

   void setIsInGlobalRegister(bool b)       { _flags.set(IsInGlobalRegister, b); }
   bool isInGlobalRegister()                { return _flags.testAny(IsInGlobalRegister); }

   void setConst()                          { _flags.set(Const); }
   bool isConst()                           { return _flags.testAny(Const); }

   void setVolatile()                       { _flags.set(Volatile); }
   void resetVolatile()                     { _flags.reset(Volatile); }
   bool isVolatile()                        { return _flags.testAny(Volatile); }
   bool isSyncVolatile()                    { return isVolatile(); }

   void setInitializedReference()           { _flags.set(InitializedReference); }
   void setUninitializedReference()         { _flags.reset(InitializedReference); }
   bool isInitializedReference()            { return _flags.testAny(InitializedReference); }

   void setClassObject()                    { _flags.set(ClassObject); }
   bool isClassObject()                     { return _flags.testAny(ClassObject); }

   void setNotCollected()                   { _flags.set(NotCollected); }
   bool isNotCollected()                    { return _flags.testAny(NotCollected); }
   bool isCollectedReference();

   void setFinal()                          { _flags.set(Final); }
   bool isFinal()                           { return _flags.testAny(Final); }

   void setInternalPointer()                { _flags.set(InternalPointer); }
   bool isInternalPointer()                 { return _flags.testAny(InternalPointer); }
   bool isInternalPointerAuto();

   void setPrivate()                        { _flags.set(Private); }
   bool isPrivate()                         { return _flags.testAny(Private); }

   void setAddressOfClassObject()           { _flags.set(AddressOfClassObject); }
   bool isAddressOfClassObject()            { return _flags.testAny(AddressOfClassObject); }

   void setSlotSharedByRefAndNonRef(bool b) { _flags.set(SlotSharedByRefAndNonRef, b); }
   bool isSlotSharedByRefAndNonRef()        { return _flags.testAny(SlotSharedByRefAndNonRef); }

   void setHoldsMonitoredObject()           { _flags.set(HoldsMonitoredObject); }
   bool holdsMonitoredObject()              { return _flags.testAny(HoldsMonitoredObject); }

   bool isNamed();

   // flag methods specific to Autos
   //
   void setSpillTempAuto();
   bool isSpillTempAuto();

   void setLocalObject();
   bool isLocalObject();

   void setBehaveLikeNonTemp();
   bool behaveLikeNonTemp();

   void setPinningArrayPointer();
   bool isPinningArrayPointer();

   bool isRegisterSymbol();
   void setIsRegisterSymbol();

   void setAutoAddressTaken();
   bool isAutoAddressTaken();

   void setSpillTempLoaded();
   bool isSpillTempLoaded();

   void setAutoMarkerSymbol();
   bool isAutoMarkerSymbol();

   void setVariableSizeSymbol();
   bool isVariableSizeSymbol();

   void setThisTempForObjectCtor();
   bool isThisTempForObjectCtor();

   // flag methods specific to Parms
   //
   void setParmHasToBeOnStack();
   bool isParmHasToBeOnStack();

   void setReferencedParameter();
   void resetReferencedParameter();
   bool isReferencedParameter();

   void setReinstatedReceiver();
   bool isReinstatedReceiver();

   // flag methods specific to statics
   //
   void setConstString();
   bool isConstString();

   void setAddressIsCPIndexOfStatic(bool b);
   bool addressIsCPIndexOfStatic();

   bool isRecognizedStatic();

   void setCompiledMethod();
   bool isCompiledMethod();

   void setStartPC();
   bool isStartPC();

   void setCountForRecompile();
   bool isCountForRecompile();

   void setRecompilationCounter();
   bool isRecompilationCounter();

   void setGCRPatchPoint();
   bool isGCRPatchPoint();

   // flag methods specific to resolved
   //
   bool isJittedMethod();

   // flag methods specific to shadows
   //
   void setArrayShadowSymbol();
   bool isArrayShadowSymbol();

   bool isRecognizedShadow();

   void setArrayletShadowSymbol();
   bool isArrayletShadowSymbol();

   void setPythonLocalVariableShadowSymbol();
   bool isPythonLocalVariableShadowSymbol();

   void setGlobalFragmentShadowSymbol();
   bool isGlobalFragmentShadowSymbol();

   void setMemoryTypeShadowSymbol();
   bool isMemoryTypeShadowSymbol();

   void setOrdered() { _flags.set(Ordered); }
   bool isOrdered()  { return _flags.testAny(Ordered); }

   void setPythonConstantShadowSymbol();
   bool isPythonConstantShadowSymbol();

   void setPythonNameShadowSymbol();
   bool isPythonNameShadowSymbol();

   // flag methods specific to labels
   //
   void setStartOfColdInstructionStream();
   bool isStartOfColdInstructionStream();

   void setStartInternalControlFlow();
   bool isStartInternalControlFlow();

   void setEndInternalControlFlow();
   bool isEndInternalControlFlow();

   void setVMThreadLive();
   bool isVMThreadLive();

   void setInternalControlFlowMerge();
   bool isInternalControlFlowMerge();

   void setEndOfColdInstructionStream();
   bool isEndOfColdInstructionStream();

   bool isNonLinear();
   void setNonLinear();

   void setGlobalLabel();
   bool isGlobalLabel();

   void setRelativeLabel();
   bool isRelativeLabel();

   void setConstMethodType();
   bool isConstMethodType();

   void setConstMethodHandle();
   bool isConstMethodHandle();

   bool isConstObjectRef();
   bool isStaticField();
   bool isFixedObjectRef();

   void setCallSiteTableEntry();
   bool isCallSiteTableEntry();

   void setHasAddrTaken() {  _flags2.set(HasAddrTaken); }
   bool isHasAddrTaken()  { return _flags2.testAny(HasAddrTaken); }

   void setMethodTypeTableEntry();
   bool isMethodTypeTableEntry();

   void setNotDataAddress();
   bool isNotDataAddress();

   void setUnsafeShadowSymbol();
   bool isUnsafeShadowSymbol();

   void setNamedShadowSymbol();
   bool isNamedShadowSymbol();

   /**
    * Enum values for _flags field.
    */
   enum
      {
      /**
       * The low bits of the flag field are used to store the data type, and
       * this is the mask used to access that data
       */
      DataTypeMask              = 0x000000FF,
      // RegisterMappedSymbols must be before data symbols TODO: Why?
      IsAutomatic               = 0x00000000,
      IsParameter               = 0x00000100,
      IsMethodMetaData          = 0x00000200,
      LastRegisterMapped        = 0x00000200,

      IsStatic                  = 0x00000300,
      IsMethod                  = 0x00000400,
      IsResolvedMethod          = 0x00000500,
      IsShadow                  = 0x00000600,
      IsLabel                   = 0x00000700,

      /**
       * Mask used to access register kind
       */
      KindMask                  = 0x00000700,


      IsInGlobalRegister        = 0x00000800,
      Const                     = 0x00001000,
      Volatile                  = 0x00002000,
      InitializedReference      = 0x00004000,
      ClassObject               = 0x00008000, ///< class object pointer
      NotCollected              = 0x00010000,
      Final                     = 0x00020000,
      InternalPointer           = 0x00040000,
      Private                   = 0x00080000,
      AddressOfClassObject      = 0x00100000, ///< address of a class object pointer
      SlotSharedByRefAndNonRef  = 0x00400000, ///< used in fsd to indicate that an reference symbol shares a slot with a nonreference symbol

      HoldsMonitoredObject      = 0x08000000,
      IsNamed                   = 0x00800000, ///< non-methods: symbol is actually an instance of a named subclass

      // only use by Symbols for which isAuto is true
      //
      SpillTemp                 = 0x80000000,
      IsLocalObject             = 0x40000000,
      BehaveLikeNonTemp         = 0x20000000, ///< used for temporaries that are
                                              ///< to behave as regular locals to
                                              ///< preserve floating point semantics
      PinningArrayPointer       = 0x10000000,
      RegisterAuto              = 0x00020000, ///< Symbol to be translated to register at instruction selection
      AutoAddressTaken          = 0x04000000, ///< a loadaddr of this auto exists
      SpillTempLoaded           = 0x04000000, ///< share bit with loadaddr because spill temps will never have their address taken. Used to remove store to spill if never loaded
      AutoMarkerSymbol          = 0x02000000, ///< dummy symbol marking some auto boundary
      VariableSizeSymbol        = 0x01000000, ///< non-java only?: specially managed automatic symbols that contain both an activeSize and a size
      ThisTempForObjectCtor     = 0x01000000, ///< java only; this temp for j/l/Object constructor

      // only use by Symbols for which isParm is true
      //
      ParmHasToBeOnStack        = 0x80000000, ///< parameter is both loadAddr-ed and assigned a global register,
                                              ///< or parameter has been stored (store opcode)
      ReferencedParameter       = 0x40000000,
      ReinstatedReceiver        = 0x20000000, ///< Receiver reinstated for DLT

      // only use by Symbols for which isStatic is true
      ConstString               = 0x80000000,
      AddressIsCPIndexOfStatic  = 0x40000000,
      RecognizedStatic          = 0x20000000,
      // Available              = 0x10000000,
      SetUpDLPFlags             = 0xF0000000, ///< Used by TR::StaticSymbol::SetupDLPFlags(), == ConstString | AddressIsCPIndexOfStatic | RecognizedStatic
      CompiledMethod            = 0x08000000,
      StartPC                   = 0x04000000,
      CountForRecompile         = 0x02000000,
      RecompilationCounter      = 0x01000000,
      GCRPatchPoint             = 0x00400000,

      //Only Used by Symbols for which isResolvedMethod is true;
      IsJittedMethod            = 0x80000000,

      // only use by Symbols for which isShadow is true
      //
      ArrayShadow               = 0x80000000,
      RecognizedShadow          = 0x40000000, // recognized field
      ArrayletShadow            = 0x20000000,
      PythonLocalVariable       = 0x10000000, // Python local variable shadow  TODO: If we ever move this somewhere else, can we move UnsafeShadow from flags2 to here?
      GlobalFragmentShadow      = 0x08000000,
      MemoryTypeShadow          = 0x04000000,
      Ordered                   = 0x02000000,
      PythonConstant            = 0x01000000, // Python constant shadow
      PythonName                = 0x00800000, // Python name shadow

      // only use by Symbols for which isLabel is true
      //
      StartOfColdInstructionStream = 0x80000000, // label at the start of an out-of-line instruction stream
      StartInternalControlFlow     = 0x40000000,
      EndInternalControlFlow       = 0x20000000,
      // Available                 = 0x10000000,
      IsVMThreadLive               = 0x08000000, // reg assigner has determined that vmthread must be in the proper register at this label
      // Available                 = 0x04000000,
      InternalControlFlowMerge     = 0x02000000, // mainline merge label for OOL instructions
      EndOfColdInstructionStream   = 0x01000000,
      NonLinear                    = 0x01000000, // TAROK and temporary.  This bit is used in conjunction with StartOfColdInstructionStream
                                                 //    to distinguish "classic" OOL instructions and the new form for Tarok.

      IsGlobalLabel                = 0x30000000,
      LabelKindMask                = 0x30000000,
      OOLMask                      = 0x81000000, // Tarok and temporary

      LastEnum
      };

   enum // For _flags2
      {
      RelativeLabel             = 0x00000001, // Label Symbols only *+N
      ConstMethodType           = 0x00000002, // JSR292
      ConstMethodHandle         = 0x00000004, // JSR292
      CallSiteTableEntry        = 0x00000008, // JSR292
      HasAddrTaken              = 0x00000010, // used to denote that we have a loadaddr of this symbol
      MethodTypeTableEntry      = 0x00000020, // JSR292
      NotDataAddress            = 0x00000040, // isStatic only: AOT
      RealRegister              = 0x00000080, // RegisterSymbol is machine real register
      UnsafeShadow              = 0x00000100,
      NamedShadow               = 0x00000200,
      };

protected:

   size_t        _size;
   const char *  _name;
   flags32_t     _flags;
   flags32_t     _flags2;
   uint16_t      _sideTableIndex;
   uint8_t       _restrictedRegisterNumber;


   /**
    * Shadow Symbol
    *
    * Used on indirect loads and stores to cover the data being referenced
    */
public:

   template <typename AllocatorType>
   static TR::Symbol * createShadow(AllocatorType m);

   template <typename AllocatorType>
   static TR::Symbol * createShadow(AllocatorType m, TR::DataTypes d);

   template <typename AllocatorType>
   static TR::Symbol * createShadow(AllocatorType m, TR::DataTypes d, uint32_t );

   /**
    * TR_NamedShadowSymbol
    *
    * \deprecated These factories are deprecated and should be removed when possible.
    *
    * @{
    */
public:

   template <typename AllocatorType>
   static TR::Symbol * createNamedShadow(AllocatorType m, TR::DataTypes d, uint32_t s, char *name = NULL);

   /** @} */

   };
}

// If these implementations are to be moved, they need to be un-defined as inline.
TR::RegisterMappedSymbol * OMR::Symbol::getRegisterMappedSymbol()
   {
   return isRegisterMappedSymbol() ? (TR::RegisterMappedSymbol *)this : 0;
   }

TR::RegisterMappedSymbol * OMR::Symbol::castToRegisterMappedSymbol()
   {
   TR_ASSERT(isRegisterMappedSymbol(), "OMR::Symbol::castToRegisterMappedSymbol, symbol is not a register mapped symbol");
   return (TR::RegisterMappedSymbol *)this;
   }

TR::AutomaticSymbol * OMR::Symbol::getAutoSymbol()
   {
   return isAuto() ? (TR::AutomaticSymbol *)this : 0;
   }

TR::AutomaticSymbol * OMR::Symbol::castToAutoSymbol()
   {
   TR_ASSERT(isAuto(), "OMR::Symbol::castToAutoSymbo, symbol is not an automatic symbol");
   return (TR::AutomaticSymbol *)this;
   }

TR::ParameterSymbol * OMR::Symbol::getParmSymbol()
   {
   return isParm() ? (TR::ParameterSymbol *)this : 0;
   }

TR::ParameterSymbol * OMR::Symbol::castToParmSymbol()
   {
   TR_ASSERT(isParm(), "OMR::Symbol::castToParmSymbol, symbol is not a parameter symbol");
   return (TR::ParameterSymbol *)this;
   }

TR::AutomaticSymbol * OMR::Symbol::getInternalPointerAutoSymbol()
   {
   return isInternalPointerAuto() ? (TR::AutomaticSymbol *)this : 0;
   }

TR::AutomaticSymbol * OMR::Symbol::castToInternalPointerAutoSymbol()
   {
   TR_ASSERT(isInternalPointerAuto(), "OMR::Symbol::castToInternalAutoSymbol, symbol is not an internal pointer automatic symbol");
   return (TR::AutomaticSymbol *)this;
   }

TR::AutomaticSymbol * OMR::Symbol::getLocalObjectSymbol()
   {
   return isLocalObject() ? (TR::AutomaticSymbol *)this : 0;
   }

TR::AutomaticSymbol * OMR::Symbol::castToLocalObjectSymbol()
   {
   TR_ASSERT(isLocalObject(), "OMR::Symbol::castToLocalObjectSymbol, symbol is not an internal pointer automatic symbol");
   return (TR::AutomaticSymbol *)this;
   }

TR::StaticSymbol * OMR::Symbol::getStaticSymbol()
   {
   return isStatic() ? (TR::StaticSymbol *)this : 0;
   }

TR::StaticSymbol * OMR::Symbol::castToStaticSymbol()
   {
   TR_ASSERT(isStatic(), "OMR::Symbol::castToStaticSymbol, symbol is not a static symbol");
   return (TR::StaticSymbol *)this;
   }

TR::StaticSymbol * OMR::Symbol::castToNamedStaticSymbol()
   {
   TR_ASSERT(isNamed() && isStatic(), "OMR::Symbol::castToNamedStaticSymbol, symbol is not a named static symbol");
   return (TR::StaticSymbol *)this;
   }

TR::MethodSymbol * OMR::Symbol::getMethodSymbol()
   {
   return isMethod() ? (TR::MethodSymbol *)this : 0;
   }

TR::MethodSymbol * OMR::Symbol::castToMethodSymbol()
   {
   TR_ASSERT(isMethod(), "OMR::Symbol::castToMethodSymbol, symbol[%p] is not a method symbol",
         this);
   return (TR::MethodSymbol *)this;
   }

TR::ResolvedMethodSymbol * OMR::Symbol::getResolvedMethodSymbol()
   {
   return isResolvedMethod() ? (TR::ResolvedMethodSymbol *)this : 0;
   }

TR::ResolvedMethodSymbol * OMR::Symbol::castToResolvedMethodSymbol()
   {
   TR_ASSERT(isResolvedMethod(), "OMR::Symbol::castToResolvedMethodSymbol, symbol is not a resolved method symbol");
   return (TR::ResolvedMethodSymbol *)this;
   }

TR::Symbol * OMR::Symbol::getShadowSymbol()
   {
   return isShadow() ? (TR::Symbol *)this : 0;
   }

TR::Symbol * OMR::Symbol::castToShadowSymbol()
   {
   TR_ASSERT(isShadow(), "OMR::Symbol::castToShadowSymbol, symbol is not a shadow symbol");
   return (TR::Symbol *)this;
   }

TR::RegisterMappedSymbol * OMR::Symbol::getMethodMetaDataSymbol()
   {
   return isMethodMetaData() ? (TR::RegisterMappedSymbol *)this : 0;
   }

TR::RegisterMappedSymbol * OMR::Symbol::castToMethodMetaDataSymbol()
   {
   TR_ASSERT(isMethodMetaData(), "OMR::Symbol::castToMethodMetaDataSymbol, symbol is not a meta data symbol");
   return (TR::RegisterMappedSymbol *)this;
   }

TR::LabelSymbol * OMR::Symbol::getLabelSymbol()
   {
   return isLabel() ? (TR::LabelSymbol *)this : 0;
   }

TR::LabelSymbol * OMR::Symbol::castToLabelSymbol()
   {
   TR_ASSERT(isLabel(), "OMR::Symbol::castToLabelSymbol, symbol is not a label symbol");
   return (TR::LabelSymbol *)this;
   }

TR::ResolvedMethodSymbol * OMR::Symbol::getJittedMethodSymbol()
   {
   return isJittedMethod() ? (TR::ResolvedMethodSymbol *)this : 0;
   }

TR::ResolvedMethodSymbol * OMR::Symbol::castToJittedMethodSymbol()
   {
   TR_ASSERT(isJittedMethod(), "OMR::Symbol::castToJittedMethodSymbol, symbol is not a resolved method symbol");
   return (TR::ResolvedMethodSymbol *)this;
   }

TR::StaticSymbol *OMR::Symbol::castToCallSiteTableEntrySymbol()
   {
   TR_ASSERT(isCallSiteTableEntry(), "OMR::Symbol::castToCallSiteTableEntrySymbol expected a call site table entry symbol");
   return (TR::StaticSymbol*)this;
   }

TR::StaticSymbol *OMR::Symbol::getCallSiteTableEntrySymbol()
   {
   return isCallSiteTableEntry()? castToCallSiteTableEntrySymbol() : NULL;
   }

TR::StaticSymbol *OMR::Symbol::castToMethodTypeTableEntrySymbol()
   {
   TR_ASSERT(isMethodTypeTableEntry(), "OMR::Symbol::castToMethodTypeTableEntrySymbol expected a method type table entry symbol");
   return (TR::StaticSymbol*)this;
   }

TR::StaticSymbol *OMR::Symbol::getMethodTypeTableEntrySymbol()
   {
   return isMethodTypeTableEntry()? castToMethodTypeTableEntrySymbol() : NULL;
   }


TR::Symbol * OMR::Symbol::getNamedShadowSymbol()
   {
   return isNamedShadowSymbol() ? (TR::Symbol *)this : 0;
   }

TR::AutomaticSymbol * OMR::Symbol::getRegisterSymbol()
   {
   return isRegisterSymbol() ? (TR::AutomaticSymbol *)this : 0;
   }

TR::AutomaticSymbol *OMR::Symbol::castToRegisterSymbol()
   {
   TR_ASSERT(isRegisterSymbol(), "OMR::Symbol::castToRegisterSymbol expected a register symbol");
   return (TR::AutomaticSymbol*)this;
   }

TR::StaticSymbol * OMR::Symbol::getRecognizedStaticSymbol()
   {
   return isRecognizedStatic() ? (TR::StaticSymbol*)this : 0;
   }

TR::AutomaticSymbol * OMR::Symbol::castToAutoMarkerSymbol()
   {
   TR_ASSERT(isAutoMarkerSymbol(), "OMR::Symbol::castToAutoMarkerSymbol, symbol is not a auto marker symbol");
   return (TR::AutomaticSymbol *)this;
   }

TR::AutomaticSymbol * OMR::Symbol::getVariableSizeSymbol()
   {
   return isVariableSizeSymbol() ? (TR::AutomaticSymbol *)this : 0;
   }

TR::AutomaticSymbol * OMR::Symbol::castToVariableSizeSymbol()
   {
   TR_ASSERT(isVariableSizeSymbol(), "OMR::Symbol::castToVariableSizeSymbol, symbol is not a VariableSizeSymbol symbol");
   return (TR::AutomaticSymbol *)this;
   }

#endif
