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

#ifndef TEST_BINARYOPILINJECTOR_INCL
#define TEST_BINARYOPILINJECTOR_INCL

#include "ilgen/OpIlInjector.hpp"

namespace TR { class TypeDictionary; }

namespace TestCompiler
{
class BinaryOpIlInjector : public OpIlInjector
   {
   public:
   BinaryOpIlInjector(TR::TypeDictionary *types, TestDriver *test, TR::ILOpCodes opCode)
      : OpIlInjector(types, test, opCode)
      {
      initOptArgs(2);
      }
   TR_ALLOC(TR_Memory::IlGenerator)

   bool injectIL();
   };

} // namespace TestCompiler

#endif // !defined(TEST_BINARYOPILINJECTOR_INCL)
