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
#include "env/FrontEnd.hpp"
#include "compile/Method.hpp"
#include "tests/Qux2IlInjector.hpp"

namespace Test
{

bool
Qux2IlInjector::injectIL()
   {
   createBlocks(1);
   // Block 2: blocks(0)
   // int32_t i = parameter;
   // i = i * 2;
   // return i;
   TR::SymbolReference *newIndexSymRef = newTemp(Int32);
   storeToTemp(newIndexSymRef, intParameter());
   storeToTemp(newIndexSymRef, createWithoutSymRef(TR::imul, 2, loadTemp(newIndexSymRef), iconst(2)));
   returnValue(loadTemp(newIndexSymRef));

   return true;
   }

} // namespace Test
