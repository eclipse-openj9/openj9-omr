/*******************************************************************************
 *
 * (c) Copyright IBM Corp. 2016, 2016
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


#ifndef TEST_METHODBUILDER_INCL
#define TEST_METHODBUILDER_INCL

#ifndef TR_METHODBUILDER_DEFINED
#define TR_METHODBUILDER_DEFINED
#define PUT_TEST_METHODBUILDER_INTO_TR
#endif


#include "compiler/ilgen/MethodBuilder.hpp"

class TR_Memory;

namespace TestCompiler
{

class TestDriver;

class MethodBuilder : public OMR::MethodBuilder
   {
public:
   TR_ALLOC(TR_Memory::IlGenerator)

   MethodBuilder(TR::TypeDictionary *types)
      : OMR::MethodBuilder(types)
      {
      }

   MethodBuilder(TR::TypeDictionary *types, TestDriver *test)
      : OMR::MethodBuilder(types)
      {
      // need to explicitly initialize TestCompiler::IlInjector layer
      setMethodAndTest(NULL, test);
      }
   };

} // namespace TestCompiler


#if defined(PUT_TEST_METHODBUILDER_INTO_TR)

namespace TR
{
   class MethodBuilder : public TestCompiler::MethodBuilder
      {
      public:
         MethodBuilder(TR::TypeDictionary *types)
            : TestCompiler::MethodBuilder(types)
            { }

         MethodBuilder(TR::TypeDictionary *types, TestCompiler::TestDriver *test)
            : TestCompiler::MethodBuilder(types, test)
            { }
      };

} // namespace TR

#endif // defined(PUT_TEST_METHODBUILDER_INTO_TR)

#endif // !defined(TEST_METHODBUILDER_INCL)
