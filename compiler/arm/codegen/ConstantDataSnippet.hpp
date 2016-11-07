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

#ifndef ARMCONSTANTDATASNIPPET_INCL
#define ARMCONSTANTDATASNIPPET_INCL

#include <stddef.h>                         // for NULL
#include <stdint.h>                         // for uint8_t, int32_t, etc
#include "codegen/CodeGenerator.hpp"        // for CodeGenerator
#include "codegen/Instruction.hpp"          // for Instruction
#include "env/jittypes.h"                   // for intptrj_t
#include "env/TRMemory.hpp"                 // for TR_Memory, etc
#include "il/DataTypes.hpp"                 // for DataTypes
#include "infra/Array.hpp"                  // for TR_Array
#include "infra/List.hpp"                   // for List

namespace TR { class Node; }

namespace TR {

template <class T> class ARMConstant
   {
   TR_Array<TR::Instruction *>     _instructionPairs;
   int32_t                         _tocOffset;
   T                               _value;
   TR::Node                        *_node;
   bool                            _isUnloadablePicSite;

   public:

   TR_ALLOC(TR_Memory::ARMConstant)

   ARMConstant(TR::CodeGenerator * cg, T v, TR::Node *n=NULL, bool ps=false) : _instructionPairs(cg->trMemory()), _value(v), _node(n), _isUnloadablePicSite(ps) {};

   T getConstantValue() {return _value;}
   bool isUnloadablePicSite() {return _isUnloadablePicSite;}

   TR_Array<TR::Instruction *> &getRequestors() {return _instructionPairs;}

   void addValueRequest(TR::Instruction *n0, TR::Instruction *n1, TR::Instruction *n2, TR::Instruction *n3)
      {
      if (n0 != NULL)
         {
         _instructionPairs.add(n0);
         _instructionPairs.add(n1);
         _instructionPairs.add(n2);
         _instructionPairs.add(n3);
         n0->setWillBePatched();
         n1->setWillBePatched();
         n2->setWillBePatched();
         n3->setWillBePatched();
         }
      }

   int32_t getTOCOffset() {return _tocOffset;}
   void setTOCOffset(int32_t o) {_tocOffset = o;}
   TR::Node *getNode() { return _node; }
   };

class ARMConstantDataSnippet
   {
#if 0
   List< TR::ARMConstant<double> >     _doubleConstants;
   List< TR::ARMConstant<float> >      _floatConstants;
#endif
   List< TR::ARMConstant<intptrj_t> > _addressConstants;
   uint8_t                                *_snippetBinaryStart;
   TR::CodeGenerator                       *_cg;

   public:

   TR_ALLOC(TR_Memory::ARMConstantDataSnippet)

   ARMConstantDataSnippet(TR::CodeGenerator *cg) : _cg(cg), _addressConstants(cg->trMemory())
      /* , _doubleConstants(cg->trMemory()), _floatConstants(cg->trMemory()) */
      {
      };

   uint8_t *getSnippetBinaryStart() {return _snippetBinaryStart;}
   uint8_t *setSnippetBinaryStart(uint8_t *p) {return _snippetBinaryStart=p;}

   int32_t addConstantRequest(void              *v,
                           TR::DataType       type,
                           TR::Instruction *nibble0,
                           TR::Instruction *nibble1,
                           TR::Instruction *nibble2,
                           TR::Instruction *nibble3,
                           TR::Instruction *nibble4,
                           TR::Node *node,
                           bool isUnloadablePicSite);

   bool getRequestorsFromNibble(TR::Instruction *nibble, TR::Instruction **q, bool remove);

   virtual uint8_t *emitSnippetBody();
   virtual uint32_t getLength();

   TR::CodeGenerator *cg() {return _cg;}

#ifdef DEBUG
   virtual void print(TR::FILE *outFile);
#endif

   };

}

#endif
