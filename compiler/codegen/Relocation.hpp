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

#ifndef RELOCATION_INCL
#define RELOCATION_INCL

#include <stddef.h>                         // for NULL
#include <stdint.h>                         // for uint8_t, uint32_t, etc
#include "env/TRMemory.hpp"                 // for TR_Memory, etc
#include "infra/Assert.hpp"                 // for TR_ASSERT
#include "infra/Flags.hpp"                  // for flags8_t
#include "infra/Link.hpp"                   // for TR_Link
#include "runtime/Runtime.hpp"

namespace TR { class CodeGenerator; }
namespace TR { class Compilation; }
namespace TR { class Instruction; }
namespace TR { class LabelSymbol; }
namespace TR { class Node; }

extern char* AOTcgDiagOn;

#if 0 //defined(ENABLE_AOT_S390)
#define AOTcgDiag0(comp,s)                   if (AOTcgDiagOn) dumpOptDetails(comp,s)
#define AOTcgDiag1(comp,s,p1)                if (AOTcgDiagOn) dumpOptDetails(comp,s,p1)
#define AOTcgDiag2(comp,s,p1,p2)             if (AOTcgDiagOn) dumpOptDetails(comp,s,p1,p2)
#define AOTcgDiag3(comp,s,p1,p2,p3)          if (AOTcgDiagOn) dumpOptDetails(comp,s,p1,p2,p3)
#define AOTcgDiag4(comp,s,p1,p2,p3,p4)       if (AOTcgDiagOn) dumpOptDetails(comp,s,p1,p2,p3,p4)
#define AOTcgDiag5(comp,s,p1,p2,p3,p4,p5)    if (AOTcgDiagOn) dumpOptDetails(comp,s,p1,p2,p3,p4,p5)
#define AOTcgDiag6(comp,s,p1,p2,p3,p4,p5,p6) if (AOTcgDiagOn) dumpOptDetails(comp,s,p1,p2,p3,p4,p5,p6)
#else
#define AOTcgDiag0(comp,s)                    AOTcgDummy()
#define AOTcgDiag1(comp,s,p1)                 AOTcgDummy()
#define AOTcgDiag2(comp,s,p1,p2)              AOTcgDummy()
#define AOTcgDiag3(comp,s,p1,p2,p3)           AOTcgDummy()
#define AOTcgDiag4(comp,s,p1,p2,p3,p4)        AOTcgDummy()
#define AOTcgDiag5(comp,s,p1,p2,p3,p4,p5)     AOTcgDummy()
#define AOTcgDiag6(comp,s,p1,p2,p3,p4,p5,p6)  AOTcgDummy()
inline  void AOTcgDummy(){}
#endif

typedef enum
   {
   TR_UnusedGlobalValue             = 0,
   // start with 1, as there is a check in PPC preventing 0 valued immediates from being relocated.
   TR_CountForRecompile             = 1,
   TR_HeapBase                      = 2,
   TR_HeapTop                       = 3,
   TR_HeapBaseForBarrierRange0      = 4,
   TR_ActiveCardTableBase           = 5,
   TR_HeapSizeForBarrierRange0      = 6,
   TR_MethodEnterHookEnabledAddress = 7,
   TR_MethodExitHookEnabledAddress  = 8,
   TR_NumGlobalValueItems           = 9
   } TR_GlobalValueItem;

/** encapsulates debug information about a relocation record */
struct TR_RelocationDebugInfo
   {
   /** the file name of the code that generated the associated RR*/
   char* file;
   /** the line number in the file file that created the associated RR*/
   uintptr_t line;
   /** the node in the IL that triggered the creation of the associated RR */
   TR::Node* node;
   public:
   TR_ALLOC(TR_Memory::RelocationDebugInfo);
   };

class TR_Relocation
   {
   uint8_t *_updateLocation;
   TR_RelocationDebugInfo* _genData;

   public:
   TR_ALLOC(TR_Memory::Relocation)

   TR_Relocation() : _updateLocation(NULL) {}
   TR_Relocation(uint8_t *p) : _updateLocation(p) {}

   virtual uint8_t *getUpdateLocation()           {return _updateLocation;}
   uint8_t *setUpdateLocation(uint8_t *p) {return (_updateLocation = p);}

   virtual bool isAOTRelocation() { return true; }

   TR_RelocationDebugInfo* getDebugInfo();

   void setDebugInfo(TR_RelocationDebugInfo* info);

   /**dumps a trace of the internals - override as required */
   virtual void trace(TR::Compilation* comp);

   virtual void addAOTRelocation(TR::CodeGenerator *codeGen) {}

   virtual void apply(TR::CodeGenerator *codeGen);
   };

class TR_LabelRelocation : public TR_Relocation
   {
   TR::LabelSymbol *_label;
   public:
   TR_LabelRelocation() : TR_Relocation(), _label(NULL) {}
   TR_LabelRelocation(uint8_t *p, TR::LabelSymbol *l) : TR_Relocation(p),
                                                       _label(l) {}

   TR::LabelSymbol *getLabel()                  {return _label;}
   TR::LabelSymbol *setLabel(TR::LabelSymbol *l) {return (_label = l);}

   bool isAOTRelocation() { return false; }
   };

class TR_8BitLabelRelativeRelocation : public TR_LabelRelocation
   {
   public:
   TR_8BitLabelRelativeRelocation() : TR_LabelRelocation() {}
   TR_8BitLabelRelativeRelocation(uint8_t *p, TR::LabelSymbol *l)
      : TR_LabelRelocation(p, l) {}
   virtual void apply(TR::CodeGenerator *codeGen);
   };

class TR_12BitLabelRelativeRelocation : public TR_LabelRelocation
   {
   bool _isCheckDisp;
   public:
   TR_12BitLabelRelativeRelocation() : TR_LabelRelocation() {}
   TR_12BitLabelRelativeRelocation(uint8_t *p, TR::LabelSymbol *l, bool isCheckDisp = true)
      : TR_LabelRelocation(p, l), _isCheckDisp(isCheckDisp) {}
   bool isCheckDisp() {return _isCheckDisp;}
   virtual void apply(TR::CodeGenerator *codeGen);
   };


class TR_16BitLabelRelativeRelocation : public TR_LabelRelocation
   {
   // field _instructionOffser is used for 16-bit relocation when we need to specify where relocation starts relative to the first byte of the instruction.
   //Eg.: BranchPreload instruction BPP (48-bit instruction), where 16-bit relocation is at 32-47 bits, TR_16BitLabelRelativeRelocation(*p,*l, 4, true)
   //
   // The regular TR_16BitLabelRelativeRelocation(uint8_t *p, TR::LabelSymbol *l) only supports instructions with 16-bit relocations,
   // where the relocation is 2 bytes (hardcoded value) form the start of the instruction
   bool _instructionOffset;
   int8_t _addressDifferenceDivisor;
   public:
   TR_16BitLabelRelativeRelocation()
      : TR_LabelRelocation(), _addressDifferenceDivisor(1) {}
   TR_16BitLabelRelativeRelocation(uint8_t *p, TR::LabelSymbol *l)
      : TR_LabelRelocation(p, l), _addressDifferenceDivisor(1) {}
   TR_16BitLabelRelativeRelocation(uint8_t *p, TR::LabelSymbol *l, int8_t divisor, bool isInstOffset = false)
      : TR_LabelRelocation(p, l), _addressDifferenceDivisor(divisor), _instructionOffset(isInstOffset) {}

   bool isInstructionOffset()  {return _instructionOffset;}
   bool setInstructionOffset(bool b) {return (_instructionOffset = b);}
   int8_t getAddressDifferenceDivisor()  {return _addressDifferenceDivisor;}
   int8_t setAddressDifferenceDivisor(int8_t d) {return (_addressDifferenceDivisor = d);}

   virtual void apply(TR::CodeGenerator *codeGen);
   };

class TR_24BitLabelRelativeRelocation : public TR_LabelRelocation
   {
   public:
   TR_24BitLabelRelativeRelocation() : TR_LabelRelocation() {}
   TR_24BitLabelRelativeRelocation(uint8_t *p, TR::LabelSymbol *l)
      : TR_LabelRelocation(p, l) {}
   virtual void apply(TR::CodeGenerator *codeGen);
   };

class TR_32BitLabelRelativeRelocation : public TR_LabelRelocation
   {
   public:
   TR_32BitLabelRelativeRelocation() : TR_LabelRelocation() {}
   TR_32BitLabelRelativeRelocation(uint8_t *p, TR::LabelSymbol *l)
      : TR_LabelRelocation(p, l) {}
   virtual void apply(TR::CodeGenerator *codeGen);
   };

class TR_InstructionAbsoluteRelocation : public TR_Relocation
   {
   public:
   TR_InstructionAbsoluteRelocation(uint8_t *updateLocation,
                                    TR::Instruction *i,
                                    bool useEndAddr) /* specified if the start or
                                                        the end address of the instruction
                                                        is required */
      : TR_Relocation(updateLocation), _instruction(i), _useEndAddr(useEndAddr) {}
   virtual void apply(TR::CodeGenerator *cg);

   bool isAOTRelocation() { return false; }

   protected:
   TR::Instruction *getInstruction() { return _instruction; }
   bool useEndAddress() { return _useEndAddr; }

   private:
   TR::Instruction *_instruction;
   bool            _useEndAddr;
   };

//FIXME: these label absolute relocations should really be a subclass of instruction
// absolute.
class TR_LabelAbsoluteRelocation : public TR_LabelRelocation
   {
   public:
   TR_LabelAbsoluteRelocation() : TR_LabelRelocation() {}
   TR_LabelAbsoluteRelocation(uint8_t *p, TR::LabelSymbol *l)
      : TR_LabelRelocation(p, l) {}
   virtual void apply(TR::CodeGenerator *codeGen);
   };


class TR_16BitLoadLabelRelativeRelocation : public TR_Relocation
   {
   TR::Instruction *_lastInstruction;
   TR::LabelSymbol *_startLabel;
   TR::LabelSymbol *_endLabel;
   int32_t _deltaToStartLabel;   // used to catch potentially nasty register dep related bugs

   public:
   TR_16BitLoadLabelRelativeRelocation() : TR_Relocation() {}
   TR_16BitLoadLabelRelativeRelocation(TR::Instruction *i, TR::LabelSymbol *start, TR::LabelSymbol *end, int32_t delta)
      : TR_Relocation(NULL), _lastInstruction(i), _startLabel(start), _endLabel(end), _deltaToStartLabel(delta) {}
   TR::Instruction *getLastInstruction() {return _lastInstruction;}
   TR::Instruction *setLastInstruction(TR::Instruction *i) {return (_lastInstruction = i);}

   TR::LabelSymbol *getStartLabel()                  {return _startLabel;}
   TR::LabelSymbol *setStartLabel(TR::LabelSymbol *l) {return (_startLabel = l);}

   TR::LabelSymbol *getEndLabel()                  {return _endLabel;}
   TR::LabelSymbol *setEndLabel(TR::LabelSymbol *l) {return (_endLabel = l);}

   int32_t setDeltaToStartLabel(int32_t d)   { return (_deltaToStartLabel = d); }
   int32_t getDeltaToStartLabel()            { return _deltaToStartLabel; }

   bool isAOTRelocation() { return false; }

   virtual void apply(TR::CodeGenerator *codeGen);
   };

class TR_32BitLoadLabelRelativeRelocation : public TR_Relocation
   {
   TR::Instruction *_lastInstruction;
   TR::LabelSymbol *_startLabel;
   TR::LabelSymbol *_endLabel;
   int32_t _deltaToStartLabel;   // used to catch potentially nasty register dep related bugs

   public:
   TR_32BitLoadLabelRelativeRelocation() : TR_Relocation() {}
   TR_32BitLoadLabelRelativeRelocation(TR::Instruction *i, TR::LabelSymbol *start, TR::LabelSymbol *end, int32_t delta)
      : TR_Relocation(NULL), _lastInstruction(i), _startLabel(start), _endLabel(end), _deltaToStartLabel(delta) {}
   TR::Instruction *getLastInstruction() {return _lastInstruction;}
   TR::Instruction *setLastInstruction(TR::Instruction *i) {return (_lastInstruction = i);}

   TR::LabelSymbol *getStartLabel()                  {return _startLabel;}
   TR::LabelSymbol *setStartLabel(TR::LabelSymbol *l) {return (_startLabel = l);}

   TR::LabelSymbol *getEndLabel()                  {return _endLabel;}
   TR::LabelSymbol *setEndLabel(TR::LabelSymbol *l) {return (_endLabel = l);}

   int32_t setDeltaToStartLabel(int32_t d)   { return (_deltaToStartLabel = d); }
   int32_t getDeltaToStartLabel()            { return _deltaToStartLabel; }

   bool isAOTRelocation() { return false; }

   virtual void apply(TR::CodeGenerator *codeGen);
   };

class TR_64BitLoadLabelRelativeRelocation : public TR_LabelRelocation
   {
   TR::Instruction *_lastInstruction;
   public:
   TR_64BitLoadLabelRelativeRelocation() : TR_LabelRelocation() {}
   TR_64BitLoadLabelRelativeRelocation(TR::Instruction *i, TR::LabelSymbol *l)
      : TR_LabelRelocation(NULL, l), _lastInstruction(i) {}
   TR::Instruction *getLastInstruction() {return _lastInstruction;}
   TR::Instruction *setLastInstruction(TR::Instruction *i) {return (_lastInstruction = i);}

   virtual void apply(TR::CodeGenerator *codeGen);
   };

#define MAX_SIZE_RELOCATION_DATA ((uint16_t)0xffff)
#define MIN_SHORT_OFFSET         -32768
#define MAX_SHORT_OFFSET         32767

class TR_IteratedExternalRelocation : public TR_Link<TR_IteratedExternalRelocation>
   {
   public:
   TR_IteratedExternalRelocation() : TR_Link<TR_IteratedExternalRelocation>(),
                                     _numberOfRelocationSites(0),
                                     _targetAddress(NULL),
                                     _targetAddress2(NULL),
                                     _relocationData(NULL),
                                     _relocationDataCursor(NULL),
                                     _sizeOfRelocationData(0),
                                     _recordModifier(0),
                                     _full(false),
                                     _kind(TR_ConstantPool) {}

   TR_IteratedExternalRelocation(uint8_t *target, TR_ExternalRelocationTargetKind k, flags8_t modifier, TR::CodeGenerator *codeGen);
   TR_IteratedExternalRelocation(uint8_t *target, uint8_t *target2, TR_ExternalRelocationTargetKind k, flags8_t modifier, TR::CodeGenerator *codeGen);

   uint32_t getNumberOfRelocationSites() {return _numberOfRelocationSites;}
   uint32_t setNumberOfRelocationSites(uint32_t s)
      {return (_numberOfRelocationSites = s);}

   uint8_t *getTargetAddress()           {return _targetAddress;}
   uint8_t *setTargetAddress(uint8_t *p) {return (_targetAddress = p);}

   uint8_t *getTargetAddress2()          {return _targetAddress2;}

   uint8_t *getRelocationData()           {return _relocationData;}
   uint8_t *setRelocationData(uint8_t *p) {return (_relocationData = p);}

   uint8_t *getRelocationDataCursor()           {return _relocationDataCursor;}
   uint8_t *setRelocationDataCursor(uint8_t *p) {return (_relocationDataCursor = p);}

   void initialiseRelocation(TR::CodeGenerator * codeGen);
   void addRelocationEntry(uint32_t locationOffset);

   uint16_t getSizeOfRelocationData()             {return _sizeOfRelocationData;}
   uint16_t setSizeOfRelocationData(uint16_t s)   {return (_sizeOfRelocationData = s);}
   uint16_t addToSizeOfRelocationData(uint16_t s) {return (_sizeOfRelocationData += s);}

   bool needsWideOffsets()
      {return _recordModifier.testAny(RELOCATION_TYPE_WIDE_OFFSET);}
   void setNeedsWideOffsets() {_recordModifier.set(RELOCATION_TYPE_WIDE_OFFSET);}

   bool isOrderedPair() {return _recordModifier.testAny(RELOCATION_TYPE_ORDERED_PAIR);}
   void setOrderedPair() {_recordModifier.set(RELOCATION_TYPE_ORDERED_PAIR);}

   uint8_t getModifierValue() {return _recordModifier.getValue();}
   void    setModifierValue(uint8_t v) {_recordModifier.setValue(0x00, v);}

   bool full()    {return _full;}
   void setFull() {_full = true;}

   TR_ExternalRelocationTargetKind getTargetKind()                                  {return _kind;}
   TR_ExternalRelocationTargetKind setTargetKind(TR_ExternalRelocationTargetKind k) {return (_kind = k);}

   private:
   uint32_t                        _numberOfRelocationSites;
   uint8_t                        *_targetAddress;
   uint8_t                        *_targetAddress2;
   uint8_t                        *_relocationData;
   uint8_t                        *_relocationDataCursor;
   uint16_t                        _sizeOfRelocationData;
   flags8_t                        _recordModifier;
   bool                            _full;
   TR_ExternalRelocationTargetKind _kind;
   };

class TR_ExternalRelocation : public TR_Relocation
   {
   public:
   TR_ExternalRelocation()
      : TR_Relocation(),
        _targetAddress(NULL),
        _targetAddress2(NULL),
        _kind(TR_ConstantPool),
        _relocationRecord(NULL)
        {}

   TR_ExternalRelocation(uint8_t                        *p,
                              uint8_t                        *target,
                              TR_ExternalRelocationTargetKind kind,
                              TR::CodeGenerator *codeGen)
      : TR_Relocation(p),
        _targetAddress(target),
        _targetAddress2(NULL),
        _kind(kind),
        _relocationRecord(NULL)
        {}

   TR_ExternalRelocation(uint8_t                        *p,
                              uint8_t                        *target,
                              uint8_t                        *target2,
                              TR_ExternalRelocationTargetKind  kind,
                              TR::CodeGenerator *codeGen)
      : TR_Relocation(p),
        _targetAddress(target),
        _targetAddress2(target2),
        _kind(kind),
        _relocationRecord(NULL)
        {}

   uint8_t *getTargetAddress()           {return _targetAddress;}
   uint8_t *setTargetAddress(uint8_t *p) {return (_targetAddress = p);}

   uint8_t *getTargetAddress2()          {return _targetAddress2;}

   TR_ExternalRelocationTargetKind getTargetKind()                                  {return _kind;}
   TR_ExternalRelocationTargetKind setTargetKind(TR_ExternalRelocationTargetKind k) {return (_kind = k);}

   TR_IteratedExternalRelocation *getRelocationRecord()
      {return _relocationRecord;}

   void trace(TR::Compilation* comp);

   void addAOTRelocation(TR::CodeGenerator *codeGen);
   virtual uint8_t collectModifier();
   virtual uint32_t getNarrowSize() {return 2;}
   virtual uint32_t getWideSize() {return 4;}

   virtual void apply(TR::CodeGenerator *codeGen);

   static char *getName(TR_ExternalRelocationTargetKind k) {return _externalRelocationTargetKindNames[k];}
   static uintptr_t    getGlobalValue(uint32_t g)
         {
         TR_ASSERT(g >= 0 && g < TR_NumGlobalValueItems, "invalid index for global item");
         return _globalValueList[g];
         }
   static void         setGlobalValue(uint32_t g, uintptr_t v)
         {
         TR_ASSERT(g >= 0 && g < TR_NumGlobalValueItems, "invalid index for global item");
         _globalValueList[g] = v;
         }
   static char *       nameOfGlobal(uint32_t g)
         {
         TR_ASSERT(g >= 0 && g < TR_NumGlobalValueItems, "invalid index for global item");
         return _globalValueNames[g];
         }

   private:
   uint8_t                         *_targetAddress;
   uint8_t                         *_targetAddress2;
   TR_IteratedExternalRelocation   *_relocationRecord;
   TR_ExternalRelocationTargetKind  _kind;
   static char                     *_externalRelocationTargetKindNames[TR_NumExternalRelocationKinds];
   static uintptr_t                 _globalValueList[TR_NumGlobalValueItems];
   static uint8_t                   _globalValueSizeList[TR_NumGlobalValueItems];
   static char                     *_globalValueNames[TR_NumGlobalValueItems];
   };

class TR_32BitExternalOrderedPairRelocation: public TR_ExternalRelocation
   {
   public:
   TR_32BitExternalOrderedPairRelocation(uint8_t *location1,
                                         uint8_t *location2,
                                         uint8_t *target,
                                         TR_ExternalRelocationTargetKind  k,
                                         TR::CodeGenerator *codeGen);

   uint8_t *getLocation2() {return _update2Location;}
   void setLocation2(uint8_t *l) {_update2Location = l;}

   virtual uint8_t collectModifier();
   virtual uint32_t getNarrowSize() {return 4;}
   virtual uint32_t getWideSize() {return 8;}
   virtual void apply(TR::CodeGenerator *codeGen);

   private:
   uint8_t                         *_update2Location;
   };

class TR_BeforeBinaryEncodingExternalRelocation : public TR_ExternalRelocation
   {
   private:
   TR::Instruction  *_instruction;

   public:
   TR_BeforeBinaryEncodingExternalRelocation()
      : TR_ExternalRelocation()
        {}

   TR_BeforeBinaryEncodingExternalRelocation(TR::Instruction *instr,
                                             uint8_t *target,
                                             TR_ExternalRelocationTargetKind kind,
                                             TR::CodeGenerator *codeGen)
      : TR_ExternalRelocation(NULL, target, kind, codeGen),
        _instruction(instr)
         {}

   TR_BeforeBinaryEncodingExternalRelocation(TR::Instruction *instr,
                                             uint8_t *target,
                                             uint8_t *target2,
                                             TR_ExternalRelocationTargetKind kind,
                                             TR::CodeGenerator *codeGen)
      : TR_ExternalRelocation(NULL, target, target2, kind, codeGen),
        _instruction(instr)
         {}

   virtual uint8_t *getUpdateLocation();

   TR::Instruction* getUpdateInstruction() { return _instruction; }

   };

class TR_32BitLabelTableRelocation : public TR_LabelRelocation
   {
   public:
   TR_32BitLabelTableRelocation() : TR_LabelRelocation() {}
   TR_32BitLabelTableRelocation(uint8_t *p, TR::LabelSymbol *l)
      : TR_LabelRelocation(p, l) {}
   virtual void apply(TR::CodeGenerator *codeGen);
   };
#endif
