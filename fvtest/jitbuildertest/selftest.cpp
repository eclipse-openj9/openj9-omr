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

/*
 * This file defines some minimal sanity tests for the JitBuilder Test Utilities.
 */

#include "JBTestUtil.hpp"
#include "gtest/gtest-spi.h"

DEFINE_TYPES(NoTypes) {}

/*
 * `JustReturn` generates a function that simply returns.
 */

DECLARE_BUILDER(JustReturn);

typedef void (*JustReturnFunctionType)(void);

DEFINE_BUILDER_CTOR(JustReturn)
   {
   DefineLine(LINETOSTR(__LINE__));
   DefineFile(__FILE__);

   DefineName("JustReturn");
   DefineReturnType(NoType);
   }

DEFINE_BUILDIL(JustReturn)
   {
   Return();

   return true;
   }

/*
 * `BadBuilder` simply fails to generate any IL. This should lead to a failed compilation.
 */

DECLARE_BUILDER(BadBuilder);

typedef void (*BadBuilderFunctionType)(void);

DEFINE_BUILDER_CTOR(BadBuilder)
   {
   DefineLine(LINETOSTR(__LINE__));
   DefineFile(__FILE__);

   DefineName("BadBuilder");
   DefineReturnType(NoType);
   }

DEFINE_BUILDIL(BadBuilder)
   {
   return false;
   }

class selftest : public JitBuilderTest {};

/*
 * `JustReturnTest` compiles the `JustReturn` method and executes it. The test
 * passes if the method is successfully compiled and no fatal failure occurs
 * from executing the compiled body.
 */

TEST_F(selftest, JustReturnTest)
   {
   JustReturnFunctionType justReturn;
   ASSERT_COMPILE(NoTypes, JustReturn, justReturn);
   ASSERT_NO_FATAL_FAILURE(justReturn());
   }

/*
 * `BadBuilderTest` attempts to compile the `BadBuilder` method. The test passes
 * if compilation results in a fatal failure.
 *
 * Due to technical limitations of Google Test, the test body must be encapsulated
 * into a separate function so that `EXPECT_FATAL_FAILURE` can be used to catch
 * fatal failures (the expected result of this test).
 */

static void badBuilderRunner()
   {
   BadBuilderFunctionType badBuilder;
   ASSERT_COMPILE(NoTypes,BadBuilder,badBuilder);
   }

TEST_F(selftest, BadBuilderTest)
   {
   EXPECT_FATAL_FAILURE(badBuilderRunner(), "Failed to compile method ");
   }

