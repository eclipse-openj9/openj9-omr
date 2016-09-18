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

#include "ilgen/IlInjector.hpp"

namespace TR { class TypeDictionary; }

namespace Test
{
class FooIlInjector : public TR::IlInjector
   {
   public:
   FooIlInjector(TR::TypeDictionary *types, TestDriver *test) :
      TR::IlInjector(types, test)
      {}
   TR_ALLOC(TR_Memory::IlGenerator)

   bool injectIL();

   protected:
   TR::Node *indexParameter() { return parameter(0, Int32); }

   };

} // namespace Test
