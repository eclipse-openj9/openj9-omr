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

#include "compile/Compilation.hpp"
#include "compile/SymbolReferenceTable.hpp"
#include "env/FrontEnd.hpp"
#include "compile/Method.hpp"
#include "tests/IndirectStoreIlInjector.hpp"
#include "tests/OpCodesTest.hpp"

namespace Test
{

bool
IndirectStoreIlInjector::injectIL()
   {
   if (!isOpCodeSupported())
      return false;

   createBlocks(1);
   TR::SymbolReference *storeSymRef = symRefTab()->findOrCreateArrayShadowSymbolRef(_dataType, parm(1));
   genTreeTop(TR::Node::createWithSymRef(_opCode, 2, parm(1), parm(2), 0, storeSymRef));
   returnNoValue();
   return true;
   }

} // namespace Test
