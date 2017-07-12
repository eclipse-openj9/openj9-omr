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

#include "JBTestUtil.hpp"

#include <cmath>

typedef int64_t (FooFunction)(int64_t, double, int32_t, double,
                              int64_t, double, int32_t, double);

DEFINE_BUILDER( FooBuilder,
                Int64,
                PARAM("arg0_int64", Int64),
                PARAM("arg1_double", Double),
                PARAM("arg2_int32", Int32 ),
                PARAM("arg3_double", Double),
                PARAM("arg4_int64", Int64),
                PARAM("arg5_double", Double),
                PARAM("arg6_int32", Int32),
                PARAM("arg7_double", Double))
   {
   auto comp0 = EqualTo(Load("arg0_int64"), Load("arg4_int64"));
   auto comp1 = EqualTo(Load("arg1_double"), Load("arg5_double"));
   auto comp2 = EqualTo(Load("arg2_int32"), Load("arg6_int32"));
   auto comp3 = EqualTo(Load("arg3_double"), Load("arg7_double"));
   auto comp4 = And(comp0,
                    And(comp1,
                        And(comp2,comp3)));
   Return(comp4);

   return true;
   }

class SystemLinkageTest : public JitBuilderTest {};

TEST_F(SystemLinkageTest, FooTest)
   {
   FooFunction *foo;
   ASSERT_COMPILE(TR::TypeDictionary, FooBuilder, foo);

   ASSERT_TRUE(foo(200, 3.14159, -10, 6.67300 * pow(10, -11),
                   200, 3.14159, -10, 6.67300 * pow(10, -11)));
   }
