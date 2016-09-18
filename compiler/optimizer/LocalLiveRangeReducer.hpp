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

#ifndef LLRREDUC_INCL
#define LLRREDUC_INCL

#include <stddef.h>                           // for NULL
#include <stdint.h>                           // for int32_t
#include "env/TRMemory.hpp"                   // for TR_Memory, etc
#include "il/Node.hpp"                        // for Node, vcount_t
#include "infra/BitVector.hpp"                // for TR_BitVector
#include "infra/List.hpp"                     // for List
#include "optimizer/Optimization.hpp"         // for Optimization
#include "optimizer/OptimizationManager.hpp"  // for OptimizationManager

class DepPair;
class TR_TreeRefInfo;
namespace TR { class TreeTop; }

// Local Live Range Reduction
//
// Performs Reordering locally within a basic block.
// It does not take into account any information from other blocks.
// Basically tries to schedule the instructions so it reduces live range of registers.
//

/*
 * Class TR_LocalLiveRangeReduction
 * ================================
 *
 * Local live range reduction is the more general optimization/implementation 
 * of local reordering.
 * 
 * It is essentially a general pass that operates at the granularity of a block 
 * at a time and aims to reduce the live range of commoned nodes (virtual registers 
 * in the code generator) with a view to reducing the likelihood of register spilling. 
 * Entire trees are moved if necessary and also subtrees are extracted and anchored 
 * under treetop nodes with a view of keeping all the references to a particular node 
 * as close together as possible (while also trying to do the same wrt live ranges 
 * of the children of the node). Since the optimization moves nodes around it needs 
 * to consult aliasing to ensure that the semantics of the original program are not 
 * changed. Some other IL rules that are not expressed as aliasing relationships 
 * (e.g. an idiv node must be under a DIVCHK in Java) need to be enforced by 
 * checking explicitly that the code motion would still result in correct IL.
 * 
 */

class TR_LocalLiveRangeReduction  : public TR::Optimization
   {
   public:
   // Performs local LR reduction within a basic block.

   TR_LocalLiveRangeReduction(TR::OptimizationManager *manager);
   static TR::Optimization *create(TR::OptimizationManager *manager)
      {
      return new (manager->allocator()) TR_LocalLiveRangeReduction(manager);
      }

   virtual int32_t perform();
   virtual void prePerformOnBlocks();
   virtual void postPerformOnBlocks();

   private :
   bool transformExtendedBlock(TR::TreeTop *, TR::TreeTop *);
   void collectInfo(TR::TreeTop *, TR::TreeTop *);
   bool investigateAndMove(TR_TreeRefInfo *,int32_t);
   bool moveTreeBefore(TR_TreeRefInfo *,TR_TreeRefInfo *, int32_t);
   //manipulating the depList
   void addDepPair(TR_TreeRefInfo *,TR_TreeRefInfo *);
   void updateDepList();

   //helper functions
   bool isWorthMoving(TR_TreeRefInfo *);
   bool isNeedToBeInvestigated(TR_TreeRefInfo *);
   void collectRefInfo(TR_TreeRefInfo *, TR::Node *,vcount_t, int32_t *);
   void initPotentialDeps(TR_TreeRefInfo *treeRefInfo);
   void populatePotentialDeps(TR_TreeRefInfo *, TR::Node *);
   bool isAnySymInDefinedOrUsedBy(TR_TreeRefInfo *,TR::Node *,TR_TreeRefInfo * );
   bool isAnyDataConstraint(TR_TreeRefInfo *,TR_TreeRefInfo *);
   TR_TreeRefInfo *findLocationToMove(TR_TreeRefInfo *);
   int32_t getIndexInArray(TR_TreeRefInfo *);
   bool matchFirstOrMidToLastRef(TR_TreeRefInfo *, TR_TreeRefInfo *);
   bool containsCallOrCheck(TR_TreeRefInfo *, TR::Node *);
   void updateRefInfo(TR::Node *n, TR_TreeRefInfo *currentTreeRefInfo, TR_TreeRefInfo *movingTreeRefInfo, bool underEval);
   void printRefInfo(TR_TreeRefInfo *);
   bool verifyRefInfo(List<TR::Node> *verifier,List<TR::Node> *refList);
   void printOnVerifyError(TR_TreeRefInfo *,TR_TreeRefInfo *);



   int32_t _numTreeTops;
   TR_TreeRefInfo **_treesRefInfoArray;
   List<TR_TreeRefInfo> _movedTreesList;     //List of moved trees during pass 1 of the optimization
   TR_BitVector *_temp;
   List<DepPair> _depPairList;


   };



   //helper classes
class DepPair
   {
   public:
   TR_ALLOC(TR_Memory::LocalLiveRangeReduction)
   DepPair(TR_TreeRefInfo *dep, TR_TreeRefInfo *anchor)
      {_dep=dep;_anchor=anchor;}
   TR_TreeRefInfo * getDep(){return _dep;}
   TR_TreeRefInfo * getAnchor(){return _anchor;}

   private:
   TR_TreeRefInfo *_dep;
   TR_TreeRefInfo *_anchor;
   };


class TR_TreeRefInfo
   {
   public:
   TR_ALLOC(TR_Memory::LocalLiveRangeReduction)

   TR_TreeRefInfo(TR::TreeTop *treeTop, TR_Memory * m)
      : _treeTop(treeTop),
        _firstRefNodes(m),
        _midRefNodes(m),
        _lastRefNodes(m)
      {_defSym=NULL;_useSym=NULL;} //need also to init lists

   TR::TreeTop* getTreeTop() {return _treeTop; }
   List<TR::Node> *getFirstRefNodesList() {return &_firstRefNodes; }
   List<TR::Node> *getMidRefNodesList() {return &_midRefNodes; }
   List<TR::Node> *getLastRefNodesList() {return &_lastRefNodes; }

   TR_BitVector *getDefSym() {return _defSym; }
   TR_BitVector *getUseSym() {return _useSym; }
   TR_BitVector *setDefSym(TR_BitVector *vec) {return  _defSym = vec; }
   TR_BitVector *setUseSym(TR_BitVector *vec) {return _useSym = vec; }
   void resetSyms()  {_defSym->empty();_useSym->empty();}
   private:
   TR::TreeTop* _treeTop;
   List<TR::Node> _firstRefNodes;
   List<TR::Node> _midRefNodes;
   List<TR::Node> _lastRefNodes;

   TR_BitVector *_defSym;
   TR_BitVector *_useSym;

   };

#endif
