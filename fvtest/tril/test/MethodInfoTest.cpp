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

#include "JitTest.hpp"
#include "method_info.hpp"

#define ASSERT_NULL(pointer) ASSERT_EQ(nullptr, (pointer))
#define ASSERT_NOTNULL(pointer) ASSERT_TRUE(nullptr != (pointer))

TEST(MethodInfoTest, EmptyMethod) {
    auto methodAST = parseString("(method return=\"NoType\")");
    auto methodInfo = Tril::MethodInfo{methodAST};

    ASSERT_EQ("", methodInfo.getName());
    ASSERT_EQ(TR::NoType, methodInfo.getReturnType());
    ASSERT_EQ(0, methodInfo.getArgCount());
    ASSERT_EQ(0, methodInfo.getArgTypes().size());
    ASSERT_NULL(methodInfo.getBodyAST());
}

TEST(MethodInfoTest, EmptyMethodWithInt16Return) {
    auto methodAST = parseString("(method return=\"Int16\")");
    auto methodInfo = Tril::MethodInfo{methodAST};

    ASSERT_EQ("", methodInfo.getName());
    ASSERT_EQ(TR::Int16, methodInfo.getReturnType());
    ASSERT_EQ(0, methodInfo.getArgCount());
    ASSERT_EQ(0, methodInfo.getArgTypes().size());
    ASSERT_NULL(methodInfo.getBodyAST());
}

TEST(MethodInfoTest, EmptyMethodWithDoubleReturn) {
    auto methodAST = parseString("(method return=\"Double\")");
    auto methodInfo = Tril::MethodInfo{methodAST};

    ASSERT_EQ("", methodInfo.getName());
    ASSERT_EQ(TR::Double, methodInfo.getReturnType());
    ASSERT_EQ(0, methodInfo.getArgCount());
    ASSERT_EQ(0, methodInfo.getArgTypes().size());
    ASSERT_NULL(methodInfo.getBodyAST());
}

TEST(MethodInfoTest, EmptyMethodWithName) {
    auto methodAST = parseString("(method name=\"empty\" return=\"NoType\")");
    auto methodInfo = Tril::MethodInfo{methodAST};

    ASSERT_EQ("empty", methodInfo.getName());
    ASSERT_EQ(TR::NoType, methodInfo.getReturnType());
    ASSERT_EQ(0, methodInfo.getArgCount());
    ASSERT_EQ(0, methodInfo.getArgTypes().size());
    ASSERT_NULL(methodInfo.getBodyAST());
}

TEST(MethodInfoTest, EmptyMethodWithInt32Argument) {
    auto methodAST = parseString("(method return=\"NoType\" args=[\"Int32\"])");
    auto methodInfo = Tril::MethodInfo{methodAST};

    ASSERT_EQ("", methodInfo.getName());
    ASSERT_EQ(TR::NoType, methodInfo.getReturnType());
    ASSERT_EQ(1, methodInfo.getArgCount());
    ASSERT_EQ(1, methodInfo.getArgTypes().size());
    ASSERT_EQ(TR::Int32, methodInfo.getArgTypes().at(0));
    ASSERT_NULL(methodInfo.getBodyAST());
}

TEST(MethodInfoTest, EmptyMethodWithFloatAndInt32Arguments) {
    auto methodAST = parseString("(method return=\"NoType\" args=[\"Float\", \"Int32\"])");
    auto methodInfo = Tril::MethodInfo{methodAST};

    ASSERT_EQ("", methodInfo.getName());
    ASSERT_EQ(TR::NoType, methodInfo.getReturnType());
    ASSERT_EQ(2, methodInfo.getArgCount());
    ASSERT_EQ(2, methodInfo.getArgTypes().size());
    ASSERT_EQ(TR::Float, methodInfo.getArgTypes().at(0));
    ASSERT_EQ(TR::Int32, methodInfo.getArgTypes().at(1));
    ASSERT_NULL(methodInfo.getBodyAST());
}

TEST(MethodInfoTest, MethodWithBody) {
    auto methodAST = parseString("(method return=\"NoType\" (block (return)))");
    auto methodInfo = Tril::MethodInfo{methodAST};

    ASSERT_EQ("", methodInfo.getName());
    ASSERT_EQ(TR::NoType, methodInfo.getReturnType());
    ASSERT_EQ(0, methodInfo.getArgCount());
    ASSERT_EQ(0, methodInfo.getArgTypes().size());
    ASSERT_NOTNULL(methodInfo.getBodyAST());
}

TEST(MethodInfoTest, MethodWithBodyInt8ReturnInt32andInt64Args) {
    auto methodAST = parseString("(method name=\"foo\" return=\"Int8\" args=[\"Int32\", \"Int64\"] (block (breturn (bconst 3))))");
    auto methodInfo = Tril::MethodInfo{methodAST};

    ASSERT_EQ("foo", methodInfo.getName());
    ASSERT_EQ(TR::Int8, methodInfo.getReturnType());
    ASSERT_EQ(2, methodInfo.getArgCount());
    ASSERT_EQ(2, methodInfo.getArgTypes().size());
    ASSERT_EQ(TR::Int32, methodInfo.getArgTypes().at(0));
    ASSERT_EQ(TR::Int64, methodInfo.getArgTypes().at(1));
    ASSERT_NOTNULL(methodInfo.getBodyAST());
}
