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

#ifndef TR_TREETOP_INCL
#define TR_TREETOP_INCL

#include "il/OMRTreeTop.hpp"

#include "infra/Annotations.hpp"  // for OMR_EXTENSIBLE

namespace TR
{

class OMR_EXTENSIBLE TreeTop : public OMR::TreeTopConnector
   {

public:

   TreeTop() :
      OMR::TreeTop() {} ;

   TreeTop(TR::Node *node, TR::TreeTop *next, TR::TreeTop *prev) :
      OMR::TreeTop(node, next, prev) {} ;

   TreeTop(TR::TreeTop *precedingTreeTop, TR::Node *node, TR::Compilation *c) :
      OMR::TreeTop(precedingTreeTop, node, c) {} ;
   };

}

#endif

