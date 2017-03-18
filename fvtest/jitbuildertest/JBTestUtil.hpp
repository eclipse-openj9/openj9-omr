/*******************************************************************************
 *
 * (c) Copyright IBM Corp. 2017, 2017
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

#ifndef JB_TEST_UTIL_HPP
#define JB_TEST_UTIL_HPP

#include "gtest/gtest.h"

#include "Jit.hpp"
#include "ilgen/TypeDictionary.hpp"
#include "ilgen/MethodBuilder.hpp"

/*
 * Convenience macro for defining type dictionary objects. `name` is the name of
 * the class that will be created. The "body" of the macro should contain the
 * JitBuilder type definitions.
 *
 * Example use:
 *
 *    DEFINE_TYPES(MyTypeDictionary)
 *       {
 *       DefineStruct("MyStruct");
 *       DefineField("MyStruct", "field", Int32);
 *       CloseStruct("MyStruct");
 *       }
 */
#define DEFINE_TYPES(name) \
   struct name : public TR::TypeDictionary { name(); }; \
   inline name::name() : TR::TypeDictionary()

/*
 * Convenience macro for declaring a MethodBuilder class. `name` is the name of
 * the class that will be created. The macro will ensure the class inherits from
 * `TR::MethodBuilder` and that the constructor and `buildIL()` method are
 * declared. A ';' is required at the end of the macro invocation.
 *
 * Example use:
 *
 *    DECL_TEST_BUILDER(MyFunctionBuilder);
 */
#define DECL_TEST_BUILDER(name) \
   class name : public TR::MethodBuilder { \
      public: \
      name(TR::TypeDictionary *); \
      virtual bool buildIL(); \
   }

#define DEF_TEST_BUILDER_CTOR(name) \
   inline name::name(TR::TypeDictionary *types) : TR::MethodBuilder(types)

#define DEF_BUILDIL(name) \
   inline bool name::buildIL()

/**
 * @brief The JitBuilderTest class is a basic test fixture for JitBuilder test cases.
 *
 * Most JitBuilder test case fixtures should publically inherit from this class.
 *
 * Example use:
 *
 *    class MyTestCase : public JitBuilderTest {};
 */
class JitBuilderTest : public ::testing::Test
   {
   public:

   static void SetUpTestCase()
      {
      ASSERT_TRUE(initializeJit()) << "Failed to initialize the JIT.";
      }

   static void TearDownTestCase()
      {
      shutdownJit();
      }
   };

/**
 * @brief try_compile is a convenience template function for invoking JitBuilder
 *
 * Note: directly calling this function in a test is not recommend. Using the
 * ASSERT_COMPILE macro is preferred.
 *
 * The first template argument (`TypeDictionary`) is the class type that should
 * be passed to construct the MethodBuilder instance. The specified type *must*
 * inherit from `TR::TypeDictionary`.
 *
 * The second template argument (`MethodBuilder`) is the class type of the
 * MethodBuilder instance that should be constructed. The specified type *must*
 * inherit from `TR::MethodBuilder`.
 *
 * The last template argument (`Function`) is the type of the function pointer
 * that should be used to store the address of the entry point to the compiled
 * body.
 *
 * The formal parameter to the function is a reference to the variable in which
 * the address of the entry point will be stored.
 *
 * Only the first two template arguments must be specified because that last one
 * can be deduced by the compiler from the type of the argument passed to the
 * function.
 *
 * If compilation of the method fails, the function will assert and return.
 * `::testing::Test::HasFatalFailure()` can be used to detect the assert from
 * the calling test.
 *
 * Example use:
 *
 *    typedef void (*MyFunctionType)(int);   // define type of entry point
 *    MyFunctionType myFunction;             // variable to store the entry point
 *
 *    try_compile<MyTypeDictionary,MyMethodBuilder>(myFunction);
 *       // If compilation succeeds, `myFunction` will point to the entry point
 *       // of the compiled body. Otherwise, the function will assert.
 *
 *    if (::testing::Test::HasFatalFailure())
 *       return;        // return because compilation failed
 *    else
 *       myFunction(2); // execute the compiled body
 */
template <typename TypeDictionary, typename MethodBuilder, typename Function>
void try_compile(Function& f)
   {
   TypeDictionary types;
   MethodBuilder builder(&types);
   uint8_t *entry;
   int32_t rc = compileMethodBuilder(&builder, &entry);
   ASSERT_EQ(0, rc) << "Failed to compile method " << builder.getMethodName();
   f = (Function)entry;
   }

/**
 * @brief ASSERT_COMPILE is a convenience macro for invoking JitBuilder
 *
 * Note: invoking ASSERT_COMPILE is preferred to directly calling `try_compile()`.
 *
 * This macro simply calls `try_compile()` and asserts that compilation
 * succeeded. It forwards its first argument to the TypeDictionary template
 * parameter, its second argument to the MethodBuilder template parameter, and
 * its last last argument becomes the function's argument.
 *
 * Example use:
 *
 *    typedef void (*MyFunctionType)(int);   // define type of entry point
 *    MyFunctionType myFunction;             // variable to store the entry point
 *
 *    ASSERT_COMPILE(MyTypeDictionary, MyMethodBuilder, myFunction);
 *       // Compile method and assert that compilation succeeds. If successful,
 *       // `myFunction` will point to the entry point of the compiled body.
 *
 *    myFunction(2); // execute the compiled body
 */
#define ASSERT_COMPILE(TypeDictionary, MethodBuilder, function) do {\
   try_compile<TypeDictionary,MethodBuilder>(function); \
   ASSERT_FALSE(::testing::Test::HasFatalFailure()); \
} while (0)

#endif // JB_TEST_UTIL_HPP
