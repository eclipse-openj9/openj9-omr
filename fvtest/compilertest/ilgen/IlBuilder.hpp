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


#ifndef TEST_ILBUILDER_INCL
#define TEST_ILBUILDER_INCL

#ifndef TR_ILBUILDER_DEFINED
#define TR_ILBUILDER_DEFINED
#define PUT_TEST_ILBUILDER_INTO_TR
#endif


#include "compiler/ilgen/IlBuilder.hpp"

namespace TestCompiler
{

class TestDriver;

class IlBuilder : public OMR::IlBuilder
   {
public:
   TR_ALLOC(TR_Memory::IlGenerator)

   IlBuilder(TR::MethodBuilder *methodBuilder, TR::TypeDictionary *types)
      : OMR::IlBuilder(methodBuilder, types)
      { }

   IlBuilder(TR::IlBuilder *source)
      : OMR::IlBuilder(source)
      { }

   IlBuilder(TestDriver *test, TR::MethodBuilder *methodBuilder, TR::TypeDictionary *types)
      : OMR::IlBuilder(methodBuilder, types)
      {
      // need to explicitly initialize TestCompiler::IlInjector layer because
      // it's hiding behind our OMR::IlBuilder base class
      setMethodAndTest((TR::ResolvedMethod *)NULL, test);
      }
   };

} // namespace TestCompiler


#ifdef PUT_TEST_ILBUILDER_INTO_TR

namespace TR
{
   class IlBuilder : public TestCompiler::IlBuilder
      {
      public:
         IlBuilder(TR::MethodBuilder *methodBuilder, TypeDictionary *types)
            : TestCompiler::IlBuilder(methodBuilder, types)
            { }

         IlBuilder(TR::IlBuilder *source)
            : TestCompiler::IlBuilder(source)
            { }

         IlBuilder(TestCompiler::TestDriver *test, TR::MethodBuilder *methodBuilder, TR::TypeDictionary *types)
            : TestCompiler::IlBuilder(test, methodBuilder, types)
            { }
      };

} // namespace TR

#endif // defined(PUT_TEST_ILBUILDER_INTO_TR)

#endif // !defined(TEST_ILBUILDER_INCL)
