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

#include "gtest/gtest.h"

#include "ast.hpp"

#define ASSERT_NULL(pointer) ASSERT_EQ(nullptr, (pointer))
#define ASSERT_NOTNULL(pointer) ASSERT_TRUE(nullptr != (pointer))

TEST(ParserTest, SingleNodeWithJustName) {
    auto trees = parseString("(nodeName)");

    ASSERT_NOTNULL(trees);
    ASSERT_STREQ("nodeName", trees->getName());
    ASSERT_NULL(trees->getArgs());
    ASSERT_NULL(trees->getChildren());
    ASSERT_NULL(trees->next);
}

TEST(ParserTest, SingleCommandNodeWithJustName) {
    auto trees = parseString("(@commandName)");

    ASSERT_NOTNULL(trees);
    ASSERT_STREQ("@commandName", trees->getName());
    ASSERT_NULL(trees->getArgs());
    ASSERT_NULL(trees->getChildren());
    ASSERT_NULL(trees->next);
}

TEST(ParserTest, TwoNodesWithJustName) {
    auto trees = parseString("(nodeName)(otherNode)");

    ASSERT_NOTNULL(trees);
    ASSERT_STREQ("nodeName", trees->getName());
    ASSERT_NULL(trees->getArgs());
    ASSERT_NULL(trees->getChildren());
    ASSERT_NOTNULL(trees->next);

    trees = trees->next;
    ASSERT_STREQ("otherNode", trees->getName());
    ASSERT_NULL(trees->getArgs());
    ASSERT_NULL(trees->getChildren());
    ASSERT_NULL(trees->next);
}

TEST(ParserTest, SingleNodeWithChild) {
    const auto* trees = parseString("(nodeName (childNode))");

    ASSERT_NOTNULL(trees);
    ASSERT_STREQ("nodeName", trees->getName());
    ASSERT_NULL(trees->getArgs());
    ASSERT_NOTNULL(trees->getChildren());
    ASSERT_NULL(trees->next);

    trees = trees->getChildren();
    ASSERT_STREQ("childNode", trees->getName());
    ASSERT_NULL(trees->getArgs());
    ASSERT_NULL(trees->getChildren());
    ASSERT_NULL(trees->next);
}

TEST(ParserTest, NodeWithChildAndSibling) {
    auto trees = parseString("(nodeName (childNode)) (siblingNode)");

    ASSERT_NOTNULL(trees);
    ASSERT_STREQ("nodeName", trees->getName());
    ASSERT_NULL(trees->getArgs());
    ASSERT_NOTNULL(trees->getChildren());
    ASSERT_NOTNULL(trees->next);

    auto child = trees->getChildren();
    ASSERT_STREQ("childNode", child->getName());
    ASSERT_NULL(child->getArgs());
    ASSERT_NULL(child->getChildren());
    ASSERT_NULL(child->next);

    auto sibling = trees->next;
    ASSERT_STREQ("siblingNode", sibling->getName());
    ASSERT_NULL(sibling->getArgs());
    ASSERT_NULL(sibling->getChildren());
    ASSERT_NULL(sibling->next);
}

TEST(ParserTest, SingleNodeWithIntArg) {
    auto trees = parseString("(nodeName 3)");

    ASSERT_NOTNULL(trees);
    ASSERT_STREQ("nodeName", trees->getName());
    ASSERT_NOTNULL(trees->getArgs());
    ASSERT_NULL(trees->getChildren());
    ASSERT_NULL(trees->next);

    auto arg = trees->getArgs();
    ASSERT_STREQ("", arg->getName());
    ASSERT_EQ(ASTValue::Integer, arg->getValue()->getType());
    ASSERT_EQ(3, arg->getValue()->get<ASTValue::Integer_t>());
    ASSERT_NULL(arg->next);
}

TEST(ParserTest, SingleNodeWithFloatArg) {
    auto trees = parseString("(nodeName 3.00)");

    ASSERT_NOTNULL(trees);
    ASSERT_STREQ("nodeName", trees->getName());
    ASSERT_NOTNULL(trees->getArgs());
    ASSERT_NULL(trees->getChildren());
    ASSERT_NULL(trees->next);

    auto arg = trees->getArgs();
    ASSERT_STREQ("", arg->getName());
    ASSERT_EQ(ASTValue::FloatingPoint, arg->getValue()->getType());
    ASSERT_EQ(3.00, arg->getValue()->get<ASTValue::FloatingPoint_t>());
    ASSERT_NULL(arg->next);
}

TEST(ParserTest, SingleNodeWithStringArg) {
    auto trees = parseString("(nodeName \"foo\")");

    ASSERT_NOTNULL(trees);
    ASSERT_STREQ("nodeName", trees->getName());
    ASSERT_NOTNULL(trees->getArgs());
    ASSERT_NULL(trees->getChildren());
    ASSERT_NULL(trees->next);

    auto arg = trees->getArgs();
    ASSERT_STREQ("", arg->getName());
    ASSERT_EQ(ASTValue::String, arg->getValue()->getType());
    ASSERT_STREQ("foo", arg->getValue()->get<ASTValue::String_t>());
    ASSERT_NULL(arg->next);
}

TEST(ParserTest, SingleNodeWithIdentiferArg) {
    auto trees = parseString("(nodeName id)");

    ASSERT_NOTNULL(trees);
    ASSERT_STREQ("nodeName", trees->getName());
    ASSERT_NOTNULL(trees->getArgs());
    ASSERT_NULL(trees->getChildren());
    ASSERT_NULL(trees->next);

    auto arg = trees->getArgs();
    ASSERT_STREQ("", arg->getName());
    ASSERT_EQ(ASTValue::String, arg->getValue()->getType());
    ASSERT_STREQ("id", arg->getValue()->get<ASTValue::String_t>());
    ASSERT_NULL(arg->next);
}

TEST(ParserTest, SingleNodeWithCommandArg) {
    auto trees = parseString("(nodeName @cmd)");

    ASSERT_NOTNULL(trees);
    ASSERT_STREQ("nodeName", trees->getName());
    ASSERT_NOTNULL(trees->getArgs());
    ASSERT_NULL(trees->getChildren());
    ASSERT_NULL(trees->next);

    auto arg = trees->getArgs();
    ASSERT_STREQ("", arg->getName());
    ASSERT_EQ(ASTValue::String, arg->getValue()->getType());
    ASSERT_STREQ("@cmd", arg->getValue()->get<ASTValue::String_t>());
    ASSERT_NULL(arg->next);
}

TEST(ParserTest, SingleNodeWithNamedArg) {
    auto trees = parseString("(nodeName arg=3.14)");

    ASSERT_NOTNULL(trees);
    ASSERT_STREQ("nodeName", trees->getName());
    ASSERT_NOTNULL(trees->getArgs());
    ASSERT_NULL(trees->getChildren());
    ASSERT_NULL(trees->next);

    auto arg = trees->getArgs();
    ASSERT_STREQ("arg", arg->getName());
    ASSERT_EQ(ASTValue::FloatingPoint, arg->getValue()->getType());
    ASSERT_EQ(3.14, arg->getValue()->get<ASTValue::FloatingPoint_t>());
    ASSERT_NULL(arg->next);
}

TEST(ParserTest, SingleNodeWithNamedIdentifierArg) {
    auto trees = parseString("(nodeName arg=ID)");

    ASSERT_NOTNULL(trees);
    ASSERT_STREQ("nodeName", trees->getName());
    ASSERT_NOTNULL(trees->getArgs());
    ASSERT_NULL(trees->getChildren());
    ASSERT_NULL(trees->next);

    auto arg = trees->getArgs();
    ASSERT_STREQ("arg", arg->getName());
    ASSERT_EQ(ASTValue::String, arg->getValue()->getType());
    ASSERT_STREQ("ID", arg->getValue()->get<ASTValue::String_t>());
    ASSERT_NULL(arg->next);
}

TEST(ParserTest, SingleNodeWithNamedCommandArg) {
    auto trees = parseString("(nodeName arg=@cmd)");

    ASSERT_NOTNULL(trees);
    ASSERT_STREQ("nodeName", trees->getName());
    ASSERT_NOTNULL(trees->getArgs());
    ASSERT_NULL(trees->getChildren());
    ASSERT_NULL(trees->next);

    auto arg = trees->getArgs();
    ASSERT_STREQ("arg", arg->getName());
    ASSERT_EQ(ASTValue::String, arg->getValue()->getType());
    ASSERT_STREQ("@cmd", arg->getValue()->get<ASTValue::String_t>());
    ASSERT_NULL(arg->next);
}

TEST(ParserTest, SingleNodeWithNamedArgAndAnonymousArg) {
    auto trees = parseString("(nodeName arg=\"foo\" 6.33)");

    ASSERT_NOTNULL(trees);
    ASSERT_STREQ("nodeName", trees->getName());
    ASSERT_NOTNULL(trees->getArgs());
    ASSERT_NULL(trees->getChildren());
    ASSERT_NULL(trees->next);

    auto arg = trees->getArgs();
    ASSERT_STREQ("arg", arg->getName());
    ASSERT_EQ(ASTValue::String, arg->getValue()->getType());
    ASSERT_STREQ("foo", arg->getValue()->get<ASTValue::String_t>());
    ASSERT_NOTNULL(arg->next);

    arg = arg->next;
    ASSERT_STREQ("", arg->getName());
    ASSERT_EQ(ASTValue::FloatingPoint, arg->getValue()->getType());
    ASSERT_EQ(6.33, arg->getValue()->get<ASTValue::FloatingPoint_t>());
    ASSERT_NULL(arg->next);
}

TEST(ParserTest, SingleNodeWithNamedListArg) {
    auto trees = parseString("(nodeName arg=[5, 3.14159, \"foo\"])");

    ASSERT_NOTNULL(trees);
    ASSERT_STREQ("nodeName", trees->getName());
    ASSERT_NOTNULL(trees->getArgs());
    ASSERT_NULL(trees->getChildren());
    ASSERT_NULL(trees->next);

    auto arg = trees->getArgs();
    ASSERT_STREQ("arg", arg->getName());
    ASSERT_NOTNULL(arg->getValue());

    auto value = arg->getValue();
    ASSERT_EQ(ASTValue::Integer, value->getType());
    ASSERT_EQ(5, value->get<ASTValue::Integer_t>());
    ASSERT_NOTNULL(value->next);

    value = value->next;
    ASSERT_EQ(ASTValue::FloatingPoint, value->getType());
    ASSERT_EQ(3.14159, value->get<ASTValue::FloatingPoint_t>());
    ASSERT_NOTNULL(value->next);

    value = value->next;
    ASSERT_EQ(ASTValue::String, value->getType());
    ASSERT_STREQ("foo", value->get<ASTValue::String_t>());
    ASSERT_NULL(value->next);
}

TEST(ParserTest, SingleNodeWithAnonymousArgAndNamedArg) {
    auto trees = parseString("(nodeName 5.11 arg2=2)");

    ASSERT_NOTNULL(trees);
    ASSERT_STREQ("nodeName", trees->getName());
    ASSERT_NOTNULL(trees->getArgs());
    ASSERT_NULL(trees->getChildren());
    ASSERT_NULL(trees->next);

    auto arg = trees->getArgs();
    ASSERT_STREQ("", arg->getName());
    ASSERT_EQ(ASTValue::FloatingPoint, arg->getValue()->getType());
    ASSERT_EQ(5.11, arg->getValue()->get<ASTValue::FloatingPoint_t>());
    ASSERT_NOTNULL(arg->next);

    arg = arg->next;
    ASSERT_STREQ("arg2", arg->getName());
    ASSERT_EQ(ASTValue::Integer, arg->getValue()->getType());
    ASSERT_EQ(2, arg->getValue()->get<ASTValue::Integer_t>());
    ASSERT_NULL(arg->next);
}

TEST(ParserTest, SingleNodeWithNamedArgAndChild) {
    const auto* trees = parseString("(nodeName arg=3 (childNode))");

    ASSERT_NOTNULL(trees);
    ASSERT_STREQ("nodeName", trees->getName());
    ASSERT_NOTNULL(trees->getArgs());
    ASSERT_NOTNULL(trees->getChildren());
    ASSERT_NULL(trees->next);

    auto arg = trees->getArgs();
    ASSERT_STREQ("arg", arg->getName());
    ASSERT_EQ(ASTValue::Integer, arg->getValue()->getType());
    ASSERT_EQ(3, arg->getValue()->get<ASTValue::Integer_t>());
    ASSERT_NULL(arg->next);

    trees = trees->getChildren();
    ASSERT_STREQ("childNode", trees->getName());
    ASSERT_NULL(trees->getArgs());
    ASSERT_NULL(trees->getChildren());
    ASSERT_NULL(trees->next);
}

TEST(ParserTest, SingleNodeWithAnonymousArgAndChildWithNamedArg) {
    const auto* trees = parseString("(nodeName \"bar\" (childNode arg=4.0))");

    ASSERT_NOTNULL(trees);
    ASSERT_STREQ("nodeName", trees->getName());
    ASSERT_NOTNULL(trees->getArgs());
    ASSERT_NOTNULL(trees->getChildren());
    ASSERT_NULL(trees->next);

    auto arg = trees->getArgs();
    ASSERT_STREQ("", arg->getName());
    ASSERT_EQ(ASTValue::String, arg->getValue()->getType());
    ASSERT_STREQ("bar", arg->getValue()->get<ASTValue::String_t>());
    ASSERT_NULL(arg->next);

    trees = trees->getChildren();
    ASSERT_STREQ("childNode", trees->getName());
    ASSERT_NOTNULL(trees->getArgs());
    ASSERT_NULL(trees->getChildren());
    ASSERT_NULL(trees->next);

    arg = trees->getArgs();
    ASSERT_STREQ("arg", arg->getName());
    ASSERT_EQ(ASTValue::FloatingPoint, arg->getValue()->getType());
    ASSERT_EQ(4.0, arg->getValue()->get<ASTValue::FloatingPoint_t>());
    ASSERT_NULL(arg->next);
}

TEST(ParserTest, SingleNodeWithTwoAnonymousArgs) {
    auto trees = parseString("(nodeName 2.71828 3.14159)");

    ASSERT_NOTNULL(trees);
    ASSERT_STREQ("nodeName", trees->getName());
    ASSERT_NOTNULL(trees->getArgs());
    ASSERT_NULL(trees->getChildren());
    ASSERT_NULL(trees->next);

    auto arg = trees->getArgs();
    ASSERT_STREQ("", arg->getName());
    ASSERT_EQ(ASTValue::FloatingPoint, arg->getValue()->getType());
    ASSERT_EQ(2.71828, arg->getValue()->get<ASTValue::FloatingPoint_t>());
    ASSERT_NOTNULL(arg->next);

    arg = arg->next;
    ASSERT_STREQ("", arg->getName());
    ASSERT_EQ(ASTValue::FloatingPoint, arg->getValue()->getType());
    ASSERT_EQ(3.14159, arg->getValue()->get<ASTValue::FloatingPoint_t>());
    ASSERT_NULL(arg->next);
}

TEST(ParserTest, SingleNodeWithTwoNamedArgs) {
    auto trees = parseString("(nodeName pi=3.14159 e=2.71828)");

    ASSERT_NOTNULL(trees);
    ASSERT_STREQ("nodeName", trees->getName());
    ASSERT_NOTNULL(trees->getArgs());
    ASSERT_NULL(trees->getChildren());
    ASSERT_NULL(trees->next);

    auto arg = trees->getArgs();
    ASSERT_STREQ("pi", arg->getName());
    ASSERT_EQ(ASTValue::FloatingPoint, arg->getValue()->getType());
    ASSERT_EQ(3.14159, arg->getValue()->get<ASTValue::FloatingPoint_t>());
    ASSERT_NOTNULL(arg->next);

    arg = arg->next;
    ASSERT_STREQ("e", arg->getName());
    ASSERT_EQ(ASTValue::FloatingPoint, arg->getValue()->getType());
    ASSERT_EQ(2.71828, arg->getValue()->get<ASTValue::FloatingPoint_t>());
    ASSERT_NULL(arg->next);
}
