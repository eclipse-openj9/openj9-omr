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

#include "il/NodeUtils.hpp"

#include "il/Node.hpp"  // for Node
#include "il/Node_inlines.hpp"
#include "infra/Assert.hpp"

void
GlobalRegisterInfo::freeNodeExtension(TR::Node * owner)
   {
   owner->freeExtensionIfExists();
   }

void
ChildUnionMembers::CaseInfo::freeNodeExtension( TR::Node * owner)
   {
   owner->freeExtensionIfExists();
   }

void
ChildUnionMembers::MonitorInfo::freeNodeExtension(TR::Node * owner)
   {
   owner->freeExtensionIfExists();
   }

TR::Node *
TR_ParentOfChildNode::getParent()
   {
   return _parent;
   }

void
TR_ParentOfChildNode::setParent(TR::Node * parent)
   {
   _parent = parent;
   }

int32_t
TR_ParentOfChildNode::getChildNumber()
   {
   return _childNumber;
   }

void
TR_ParentOfChildNode::setParentAndChildNumber(TR::Node * parent, int32_t childNumber)
   {
   _parent = parent; _childNumber = childNumber;
   }

void
TR_ParentOfChildNode::setChild(TR::Node * newChild)
   {
   TR_ASSERT(!isNull(), "tried to update a NULL ParentOfChildNode");

   TR::Node * oldChild = _parent->getChild(_childNumber);
   _parent->setChild(_childNumber, newChild);
   oldChild->decReferenceCount();
   newChild->incReferenceCount();
   }

TR::Node *
TR_ParentOfChildNode::getChild()
   {
   return _parent->getChild(_childNumber);
   }

bool
TR_ParentOfChildNode::isNull()
   {
   return (_parent == NULL);
   }

