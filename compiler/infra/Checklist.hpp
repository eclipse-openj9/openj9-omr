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

#ifndef OMR_CHECKLIST_INCL
#define OMR_CHECKLIST_INCL

class TR_BitVector;

namespace TR
{

class Block;
class Compilation;
class Node;

class Checklist
   {
   protected :
   TR_BitVector* _v;
   TR::Compilation* _comp;

   public :
   Checklist(TR::Compilation* c);
   ~Checklist();
   };

class NodeChecklist: public Checklist
   {
   public:
   NodeChecklist(TR::Compilation* c);
   bool contains(TR::Node* n);
   void add(TR::Node* n);
   void remove(TR::Node* n);
   void add(NodeChecklist &other);
   void remove(NodeChecklist &other);
   bool operator==(const NodeChecklist &other) const;
   bool operator!=(const NodeChecklist &other) const { return !operator==(other); }
   };

class BlockChecklist: public Checklist
   {
   public:
   BlockChecklist(TR::Compilation* c);
   bool contains(TR::Block* b);
   void add(TR::Block* b);
   void remove(TR::Block* b);
   void add(BlockChecklist &other);
   void remove(BlockChecklist &other);
   bool operator==(const BlockChecklist &other) const;
   bool operator!=(const BlockChecklist &other) const { return !operator==(other); }
   };

}

#endif
