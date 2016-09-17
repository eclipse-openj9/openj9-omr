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

#ifndef TR_NODE_INCL
#define TR_NODE_INCL

#include "il/OMRNode.hpp"

#include <stddef.h>               // for NULL
#include <stdint.h>               // for uint16_t
#include "il/ILOpCodes.hpp"       // for ILOpCodes
#include "infra/Annotations.hpp"  // for OMR_EXTENSIBLE

namespace TR
{

class OMR_EXTENSIBLE Node : public OMR::NodeConnector
{

public:

   Node() : OMR::NodeConnector() {}

   Node(TR::Node *originatingByteCodeNode, TR::ILOpCodes op,
        uint16_t numChildren, OptAttributes * oa = NULL)
      : OMR::NodeConnector(originatingByteCodeNode, op, numChildren, oa)
      {}

   Node(Node *from, uint16_t numChildren = 0)
      : OMR::NodeConnector(from, numChildren)
      {}

   Node(Node& from)
      : OMR::NodeConnector(from)
      {}

};

}

#endif

