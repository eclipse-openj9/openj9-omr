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

#include "ilgen/OpIlInjector.hpp"

namespace TR { class TypeDictionary; }

namespace Test
{
class StoreOpIlInjector : public OpIlInjector
   {
   public:
   StoreOpIlInjector(TR::TypeDictionary *types, TestDriver *test, TR::ILOpCodes opCode)
      : OpIlInjector(types, test, opCode)
      {
      initOptArgs(1);
      }
   TR_ALLOC(TR_Memory::IlGenerator)

   bool injectIL();

//   protected:
//   TR::Node *parameter1() { return parameter(0, _dataType); }
   };

} // namespace Test
