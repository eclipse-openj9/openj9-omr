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

#include "infra/Checklist.hpp"

#include "compile/Compilation.hpp"     // for Compilation, BitVectorPool
#include "il/Block.hpp"                // for Block
#include "il/Node.hpp"                 // for Node
#include "infra/BitVector.hpp"         // for TR_BitVector

namespace TR
{
   Checklist::Checklist(TR::Compilation* c): _comp(c) { _v = _comp->getBitVectorPool().get(); }
   Checklist::~Checklist()                            {      _comp->getBitVectorPool().release(_v); };

   NodeChecklist::NodeChecklist(TR::Compilation* c): Checklist(c) {};
   bool NodeChecklist::contains(TR::Node* n)  { return _v->isSet(n->getGlobalIndex()); }
   void NodeChecklist::add(TR::Node* n)       {        _v->set(n->getGlobalIndex()); }
   void NodeChecklist::remove(TR::Node* n)    {        _v->reset(n->getGlobalIndex()); }
   void NodeChecklist::add(NodeChecklist &other)       {        *_v |= *(other._v); }
   void NodeChecklist::remove(NodeChecklist &other)    { return *_v -= *(other._v); }
   bool NodeChecklist::operator==(const NodeChecklist &other) const { return *_v == *(other._v); }

   BlockChecklist::BlockChecklist(TR::Compilation* c): Checklist(c) {};
   bool BlockChecklist::contains(TR::Block* b)   { return _v->isSet(b->getNumber()); }
   void BlockChecklist::add(TR::Block* b)        {        _v->set(b->getNumber()); }
   void BlockChecklist::remove(TR::Block* b)     {        _v->reset(b->getNumber()); }
   void BlockChecklist::add(BlockChecklist &other)       {        *_v |= *(other._v); }
   void BlockChecklist::remove(BlockChecklist &other)    { return *_v -= *(other._v); }
   bool BlockChecklist::operator==(const BlockChecklist &other) const { return *_v == *(other._v); }
}
