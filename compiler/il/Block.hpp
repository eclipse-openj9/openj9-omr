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

#ifndef TR_BLOCK_INCL
#define TR_BLOCK_INCL

#include "il/OMRBlock.hpp"

#include "infra/Annotations.hpp"  // for OMR_EXTENSIBLE

class TR_Memory;
namespace TR { class TreeTop; }

namespace TR
{

class OMR_EXTENSIBLE Block : public OMR::BlockConnector
{
   public:
   Block(TR_Memory * m) :
      OMR::BlockConnector(m) {};

   Block(TR::TreeTop *entry, TR::TreeTop *exit, TR_Memory * m) :
      OMR::BlockConnector(entry,exit,m) {};

   Block(Block &other, TR::TreeTop *entry, TR::TreeTop *exit) :
      OMR::BlockConnector(other,entry,exit) {};

};

}

#endif

