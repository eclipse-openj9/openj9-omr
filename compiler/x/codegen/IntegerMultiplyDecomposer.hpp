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

#ifndef INTEGERMULTIPLYDECOMPOSER_INCL
#define INTEGERMULTIPLYDECOMPOSER_INCL

#include <stdint.h>          // for int64_t, uint8_t, int32_t
#include "env/TRMemory.hpp"  // for TR_Memory, etc

namespace TR { class CodeGenerator; }
namespace TR { class Node; }
namespace TR { class Register; }

class TR_X86IntegerMultiplyDecomposer
   {

   public:
   TR_ALLOC(TR_Memory::CodeGenerator)

   enum {MAX_NUM_REGISTERS     =   4,
         MAX_NUM_COMPONENTS    =  10,
         NUM_CONSTS_DECOMPOSED = 100};

   TR_X86IntegerMultiplyDecomposer(int64_t           multiplier,
                                   TR::Register      *sreg,
                                   TR::Node          *node,
                                   TR::CodeGenerator *cg,
                                   bool              ccs = false,
                                   bool              sf  = false)
      : _multiplier(multiplier),
        _sourceRegister(sreg),
        _node(node),
        _cg(cg),
        _canClobberSource(ccs),
        _shiftFollows(sf)
      {}

   TR::Register *decomposeIntegerMultiplier(int32_t &tempRegArraySize, TR::Register **tempRegArray);

   TR::Register *getSourceRegister() const {return _sourceRegister;}

   TR::Node *getNode() const {return _node;}

   int64_t getConstantMultiplier() const {return _multiplier;}

   bool canClobberSource() const {return _canClobberSource;}

   TR::CodeGenerator *cg()               const {return _cg;}

   static bool hasDecomposition(int64_t multiplier);

   protected:

   private:


   int32_t findDecomposition(int64_t multiplier);

   TR::Register *generateDecompositionInstructions(int32_t index,
              int32_t &tempRegArraySize, TR::Register **tempRegArray);

   enum
      {
      shlRegImm     =  0,
      addRegReg     =  1,
      subRegReg     =  2,
      movRegReg     =  3,
      leaRegReg2    =  4,
      leaRegReg4    =  5,
      leaRegReg8    =  6,
      leaRegRegReg  =  7,
      leaRegRegReg2 =  8,
      leaRegRegReg4 =  9,
      leaRegRegReg8 = 10,
      done          = 11
      };

   typedef struct
      {
      uint8_t _operation;
      uint8_t _target;
      uint8_t _baseOrImmed;
      uint8_t _index;
      } componentOperation;


   typedef struct
      {
      int64_t            _multiplier;
      bool               _sourceDisjointWithFirstRegister; // means that if the source register can be clobbered
                                                          // will the register assigner be able to use the same
                                                          // real register to assign both source and register 1
      bool               _mustClobberSource;               // means that the decomposition
                                                          // requires a clobberable input register
      bool               _subsequentShiftTooExpensive;     // used to indicate that
                                                          // decomposition is faster than imul
                                                          // but not when a subsequent shift is added
      uint8_t            _numAdditionalRegistersNeeded;
      componentOperation _components[MAX_NUM_COMPONENTS];
      } integerMultiplyComposition;

   static const integerMultiplyComposition _integerMultiplySolutions[NUM_CONSTS_DECOMPOSED];

   int64_t           _multiplier;
   TR::Register      *_sourceRegister;
   TR::Node          *_node;
   TR::CodeGenerator *_cg;
   bool              _canClobberSource;
   bool              _shiftFollows;

};

#endif
