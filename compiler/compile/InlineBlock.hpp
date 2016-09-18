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

#ifndef INLINEBLOCK_INCL
#define INLINEBLOCK_INCL

#include <stddef.h>                 // for NULL
#include <stdint.h>                 // for int32_t
#include "env/FilePointerDecl.hpp"  // for FILE
#include "env/TRMemory.hpp"         // for TR_Memory, etc

class TR_FrontEnd;
namespace TR { class Block; }
namespace TR { class Compilation; }
namespace TR { class TreeTop; }
template <class T> class List;

class TR_InlineBlock
   {
   public:

   TR_ALLOC(TR_Memory::Inliner);

   TR_InlineBlock(int32_t BCIndex, int32_t originalBlockNum) : _BCIndex(BCIndex), _originalBlockNum(originalBlockNum) {};

   int32_t _BCIndex;
   int32_t _originalBlockNum;
   };

class TR_InlineBlocks
   {
   public:

   friend class TR_InlinerTracer;

   TR_ALLOC(TR_Memory::Inliner);

   TR_InlineBlocks(TR_FrontEnd *, TR::Compilation * );
   void addBlock(TR::Block *);
   void addExceptionBlock(TR::Block *);
   bool isInList(int32_t);
   bool isInExceptionList(int32_t);
   void dump(TR::FILE *);

   int32_t getNumBlocks() { return _numBlocks; }
   int32_t getNumExceptionBlocks() { return _numExceptionBlocks;}

   void setCallNodeTreeTop(TR::TreeTop *tt) { _callNodeTreeTop = tt; }
   TR::TreeTop *getCallNodeTreeTop() { return _callNodeTreeTop; }

   bool hasGeneratedRestartTree() { return _generatedRestartTree != NULL;}
   TR::TreeTop *getGeneratedRestartTree() { return _generatedRestartTree;}
   TR::TreeTop *setGeneratedRestartTree(TR::TreeTop *tt) { _generatedRestartTree=tt; return tt; }

   void setLowestBCIndex(int32_t i) { _lowestBCIndex=i; }
   void setHighestBCIndex(int32_t i) { _highestBCIndex=i; }
   int32_t getLowestBCIndex() { return _lowestBCIndex; }
   int32_t getHighestBCIndex() { return _highestBCIndex; }

   // Public due to access requirement of Inliner Tracer.
   List<TR_InlineBlock> *_inlineBlocks;
   List<TR_InlineBlock> *_exceptionBlocks;

   private:

   int32_t _numBlocks;
   int32_t _numExceptionBlocks;
   TR::TreeTop *_callNodeTreeTop;
   TR::TreeTop *_generatedRestartTree;

   int32_t _lowestBCIndex;
   int32_t _highestBCIndex;

   TR::Compilation *_comp;
   TR_FrontEnd *fe;
   };
#endif

