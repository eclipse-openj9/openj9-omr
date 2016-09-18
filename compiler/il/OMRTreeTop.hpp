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

#ifndef OMR_TREETOP_INCL
#define OMR_TREETOP_INCL

/*
 * The following #define and typedef must appear before any #includes in this file
 */
#ifndef OMR_TREETOP_CONNECTOR
#define OMR_TREETOP_CONNECTOR
namespace OMR { class TreeTop; }
namespace OMR { typedef OMR::TreeTop TreeTopConnector; }
#endif

#include <stddef.h>                         // for NULL, size_t
#include "env/TRMemory.hpp"                 // for TR_ALLOC_WITHOUT_NEW, etc
#include "infra/Annotations.hpp"            // for OMR_EXTENSIBLE

namespace TR { class Block; }
namespace TR { class Compilation; }
namespace TR { class Instruction; }
namespace TR { class Node; }
namespace TR { class SymbolReference; }
namespace TR { class TreeTop; }

namespace OMR
{

class OMR_EXTENSIBLE TreeTop
   {

public:
   friend class TR_DebugExt;
   TR_ALLOC_WITHOUT_NEW(TR_Memory::TreeTop)

   /// Downcast to concrete type
   TR::TreeTop * self();

   static TR::TreeTop *create(TR::Compilation *comp);
   static TR::TreeTop *create(TR::Compilation *comp, TR::Node *node, TR::TreeTop *next = NULL, TR::TreeTop *prev = NULL);
   static TR::TreeTop *create(TR::Compilation *comp, TR::TreeTop *precedingTreeTop, TR::Node *node);

   static TR::TreeTop *createIncTree  (TR::Compilation * comp, TR::Node *, TR::SymbolReference *, int32_t incAmount, TR::TreeTop *precedingTreeTop = NULL, bool isRecompCounter = false);
   static TR::TreeTop *createResetTree(TR::Compilation * comp, TR::Node *, TR::SymbolReference *, int32_t resetAmount, TR::TreeTop *precedingTreeTop = NULL, bool isRecompCounter = false);

   TR::TreeTop *duplicateTree();

   static void insertTreeTops(TR::Compilation *comp, TR::TreeTop* beforeInsertionPoint, TR::TreeTop *firstTree, TR::TreeTop *lastTree);
   void insertTreeTopsAfterMe(TR::TreeTop *firstTree, TR::TreeTop *lastTree = NULL);
   void insertTreeTopsBeforeMe(TR::TreeTop *firstTree, TR::TreeTop *lastTree = NULL);
   static void removeDeadTrees(TR::Compilation * comp, TR::TreeTop* list[]);
   static void removeDeadTrees(TR::Compilation * comp, TR::TreeTop* first, TR::TreeTop* last);

   void * operator new(size_t s, bool trace, TR_Memory *m);

   explicit TreeTop(
            TR::Node  *node = NULL,
            TR::TreeTop *next = NULL,
            TR::TreeTop *prev = NULL) :
   _pNext(next), _pPrev(prev), _pNode(node) {} ;

   TreeTop(TR::TreeTop *precedingTreeTop, TR::Node *node, TR::Compilation *c);

   TR::TreeTop *getNextTreeTop();
   TR::TreeTop *setNextTreeTop(TR::TreeTop *p);
   TR::TreeTop *getPrevTreeTop();
   TR::TreeTop *setPrevTreeTop(TR::TreeTop *p);
   TR::Node    *getNode();
   TR::Node    *setNode(TR::Node *p);

   void join(TR::TreeTop * p);
   TR::TreeTop* insertAfter(TR::TreeTop *tt);  // tt is inserted after this
   TR::TreeTop* insertBefore(TR::TreeTop *tt); // tt is inserted before this
   void unlink(bool decRefCountRecursively);   // unlink and single out "this" tree; only one tree is unlinked

   bool isPossibleDef();

   void insertNewTreeTop(TR::TreeTop *beforeNewTreeTop, TR::TreeTop * afterNewTreeTop);

   TR::TreeTop *getExtendedBlockExitTreeTop();
   TR::Block   *getEnclosingBlock(bool forward=false);
   TR::TreeTop *getNextRealTreeTop();
   TR::TreeTop *getPrevRealTreeTop();

   bool isLegalToChangeBranchDestination(TR::Compilation *);
   bool adjustBranchOrSwitchTreeTop(TR::Compilation *, TR::TreeTop *, TR::TreeTop *);

   TR::Instruction *getLastInstruction() {return *(TR::Instruction **)((char *)self() - sizeof(void *)); }
   void setLastInstruction(TR::Instruction *i){ *(TR::Instruction **)((char *)self() - sizeof(void *)) = i;}

protected:
   TR::TreeTop * _pNext;
   TR::TreeTop * _pPrev;
   TR::Node    * _pNode;

   };

}

#endif

