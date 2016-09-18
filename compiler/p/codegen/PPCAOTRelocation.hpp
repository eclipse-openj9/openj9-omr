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

#ifndef PPCAOTRELOCATION_INCL
#define PPCAOTRELOCATION_INCL

#include <stdint.h>                         // for uint8_t
#include "codegen/Relocation.hpp"           // for TR_LabelRelocation
#include "env/TRMemory.hpp"                 // for TR_Memory, etc
#include "runtime/Runtime.hpp"

namespace TR { class CodeGenerator; }
namespace TR { class Instruction; }
namespace TR { class LabelSymbol; }
namespace TR { class Node; }

class TR_PPCRelocation
   {
   public:
   TR_ALLOC(TR_Memory::PPCRelocation)

   TR_PPCRelocation(TR::Instruction *src,
                    uint8_t           *trg,
       TR_ExternalRelocationTargetKind k):
      _srcInstruction(src), _relTarget(trg), _kind(k)
      {}

   TR::Instruction *getSourceInstruction() {return _srcInstruction;}
   void setSourceInstruction(TR::Instruction *i) {_srcInstruction = i;}

   uint8_t *getRelocationTarget() {return _relTarget;}
   void setRelocationTarget(uint8_t *t) {_relTarget = t;}

   TR_ExternalRelocationTargetKind getKind() {return _kind;}
   void setKind(TR_ExternalRelocationTargetKind k) {_kind = k;}

   virtual void mapRelocation(TR::CodeGenerator *cg) = 0;

   private:
   TR::Instruction               *_srcInstruction;
   uint8_t                         *_relTarget;
   TR_ExternalRelocationTargetKind  _kind;
   };

class TR_PPCPairedRelocation: public TR_PPCRelocation
   {
   public:
   TR_PPCPairedRelocation(TR::Instruction *src1,
                          TR::Instruction *src2,
                          uint8_t           *trg,
                          TR_ExternalRelocationTargetKind k,
                          TR::Node *node) :
      TR_PPCRelocation(src1, trg, k), _src2Instruction(src2), _node(node)
      {}

   TR::Instruction *getSource2Instruction() {return _src2Instruction;}
   void setSource2Instruction(TR::Instruction *src) {_src2Instruction = src;}
   TR::Node* getNode(){return _node;}
   virtual void mapRelocation(TR::CodeGenerator *cg);

   private:
   TR::Instruction               *_src2Instruction;
   TR::Node                         *_node;
   };

// This class has nothing to do with AOT, but for the lack of more appropriate locations
// this class is defined here.
//
// PPC Specific Relocation: update the immediate fields of a pair of ppc instructions
// with the absolute address of a label (extended to 64bit as well).  For example:
//   lis  gr3, addr_hi
//   addi gr3, gr3, addr_lo
//
class TR_PPCPairedLabelAbsoluteRelocation : public TR_LabelRelocation
   {
   public:
   TR_PPCPairedLabelAbsoluteRelocation(TR::Instruction *src1,
				       TR::Instruction *src2,
                                       TR::Instruction *src3,
                                       TR::Instruction *src4,
				       TR::LabelSymbol *label)
      : TR_LabelRelocation(0, label), _instr1(src1), _instr2(src2), _instr3(src3), _instr4(src4) {}
   virtual void apply(TR::CodeGenerator *cg);

   private:
   TR::Instruction *_instr1;
   TR::Instruction *_instr2;
   TR::Instruction *_instr3;
   TR::Instruction *_instr4;
   };

#endif
