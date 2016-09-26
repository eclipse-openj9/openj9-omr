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


#ifndef TEST_ILINJECTOR_INCL
#define TEST_ILINJECTOR_INCL

#ifndef TR_ILINJECTOR_DEFINED
#define TR_ILINJECTOR_DEFINED
#define PUT_TEST_ILINJECTOR_INTO_TR
#endif

#include "compiler/ilgen/IlInjector.hpp"

namespace TR { class Block; }
namespace TR { class CFG; }
namespace TR { class IlGeneratorMethodDetails; }
namespace TR { class ResolvedMethod; }
namespace TR { class ResolvedMethodSymbol; }
namespace TR { class SymbolReferenceTable; }
namespace TR { class IlType; }

namespace TestCompiler
{

class TestDriver;

class IlInjector : public OMR::IlInjector
   {
public:
   TR_ALLOC(TR_Memory::IlGenerator)

   IlInjector(TR::TypeDictionary *types)
      : OMR::IlInjector(types),
      _test(NULL),
      _method(NULL)
      { }

   // these constructors can only be used to create IlInjector itself or
   // explicit subclasses of IlInjector
   // e.g. TestIlBuilder will need to call setMethodAndTest explicitly because
   // these constructors will be hidden below the OMR::IlBuilder class in
   IlInjector(TR::TypeDictionary *types, TestDriver *test)
      : OMR::IlInjector(types)
      {
      setMethodAndTest(NULL, test);
      }
   IlInjector(TR::ResolvedMethod *method, TR::TypeDictionary *types, TestDriver *test)
      : OMR::IlInjector(types)
      {
      setMethodAndTest(method, test);
      }
   IlInjector(TR::IlInjector *source)
      : OMR::IlInjector(source)
      {
      setMethodAndTest(source);
      }

   virtual void initialize(TR::IlGeneratorMethodDetails * details,
                           TR::ResolvedMethodSymbol     * methodSymbol,
                           TR::FrontEnd                 * fe,
                           TR::SymbolReferenceTable     * symRefTab);

   TestDriver                * test()             const { return _test; };
   TR::ResolvedMethod        * resolvedMethod()   const { return _method; }

   TR::Node                  * callFunction(TR::ResolvedMethod *function, TR::IlType *returnType, int32_t numArgs, TR::Node *firstArg);

protected:
   void setMethodAndTest(TR::IlInjector *source);
   void setMethodAndTest(TR::ResolvedMethod *method, TestDriver *test)
      {
      _test = test;
      _method = method;
      }

   // data
   //
   TestDriver                * _test;

   TR::ResolvedMethod        * _method;
   };

} // namespace TestCompiler


#if defined(PUT_TEST_ILINJECTOR_INTO_TR)

namespace TR
{
   class IlInjector : public TestCompiler::IlInjector
      {
      public:
         IlInjector(TR::TypeDictionary *types) 
            : TestCompiler::IlInjector(types)
            { }

         IlInjector(TR::TypeDictionary *types, TestCompiler::TestDriver *test)
            : TestCompiler::IlInjector(types, test)
            { }

         IlInjector(TR::ResolvedMethod *method, TR::TypeDictionary *types, TestCompiler::TestDriver *test)
            : TestCompiler::IlInjector(method, types, test)
            { }

         IlInjector(TR::IlInjector *source)
            : TestCompiler::IlInjector(source)
            { }
      };

} // namespace TR

#endif // defined(PUT_TEST_ILINJECTOR_INTO_TR)

#endif // !defined(TEST_ILINJECTOR_INCL)
