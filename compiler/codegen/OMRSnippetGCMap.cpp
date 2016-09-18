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

#include <stdint.h>
#include "codegen/GCStackAtlas.hpp" // REMOVE_THIS_LATER
#include "codegen/SnippetGCMap.hpp"
#include "codegen/CodeGenerator.hpp"
#include "codegen/Instruction.hpp"


void
OMR::SnippetGCMap::registerStackMap(
      TR::Instruction *instruction,
      TR::CodeGenerator *cg)
   {
   if (_stackMap)
      {
      _stackMap->addToAtlas(instruction, cg);
      }
   }


void
OMR::SnippetGCMap::registerStackMap(
      uint8_t *callSiteAddress,
      TR::CodeGenerator *cg)
   {
   if (_stackMap)
      {
      _stackMap->addToAtlas(callSiteAddress, cg);
      }
   }
