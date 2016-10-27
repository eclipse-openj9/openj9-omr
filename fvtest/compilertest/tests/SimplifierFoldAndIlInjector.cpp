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
#include "tests/SimplifierFoldAndIlInjector.hpp"
#include "ilgen/TypeDictionary.hpp"

namespace TestCompiler
{
bool
SimplifierFoldAndIlInjector::injectIL()
   {
   createBlocks(1);

   TR::SymbolReference* i = newTemp(Int64);

   // int64_t i = ((int64_t) parameter) & 0xFFFFFFFF00000000ll;
   storeToTemp(i,
         createWithoutSymRef(TR::land, 2,
               iu2l
                    (parameter(0, Int32)),
               lconst(0xFFFFFFFF00000000ll)));

   // return i;
   returnValue(loadTemp(i));

   return true;
   }
}
