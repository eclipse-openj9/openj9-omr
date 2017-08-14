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

#include <cstring>
#include <gtest/gtest.h>
#include "ast.hpp"

TEST(ASTValueTest, CreateInt64ASTValue) {
   auto baseValue = 3UL;

   auto value = createIntegerValue(baseValue);

   ASSERT_EQ(ASTValue::Integer, value->getType());
   ASSERT_EQ(baseValue, value->get<ASTValue::Integer_t>());
}

TEST(ASTValueTest, CreateFloat64ASTValue) {
   auto baseValue = 3.0;

   auto value = createFloatingPointValue(baseValue);

   ASSERT_EQ(ASTValue::FloatingPoint, value->getType());
   ASSERT_EQ(baseValue, value->get<ASTValue::FloatingPoint_t>());
}

TEST(ASTValueTest, CreateStringASTValue) {
   auto baseValue = "a simple string";

   auto value = createStrValue(baseValue);

   ASSERT_EQ(ASTValue::String, value->getType());
   ASSERT_STREQ(baseValue, value->get<ASTValue::String_t>());
}

TEST(ASTValueTest, CreateValueList) {
    auto intBaseValue = 3UL;
    auto doubleBaseValue = 2.71828;
    auto stringBaseValue = "a string";

    auto intValue = createIntegerValue(intBaseValue);
    auto doubleValue = createFloatingPointValue(doubleBaseValue);
    auto stringValue = createStrValue(stringBaseValue);

    appendSiblingValue(intValue, doubleValue);
    appendSiblingValue(intValue, stringValue);

    auto v = intValue;
    ASSERT_EQ(ASTValue::Integer, v->getType());
    ASSERT_EQ(intBaseValue, v->get<ASTValue::Integer_t>());
    ASSERT_EQ(doubleValue, v->next);

    v = v->next;
    ASSERT_EQ(ASTValue::FloatingPoint, v->getType());
    ASSERT_EQ(doubleBaseValue, v->get<ASTValue::FloatingPoint_t>());
    ASSERT_EQ(stringValue, v->next);

    v = v->next;
    ASSERT_EQ(ASTValue::String, v->getType());
    ASSERT_STREQ(stringBaseValue, v->get<ASTValue::String_t>());
    ASSERT_EQ(NULL, v->next);
}

TEST(ASTNodeArgumentTest, CreateNodeArgumentWithJustInt64Value) {
   auto baseValue = 3UL;
   auto value = createIntegerValue(baseValue);
   auto arg = createNodeArg(NULL, value, NULL);

   ASSERT_STREQ(NULL, arg->getName());
   ASSERT_EQ(value, arg->getValue());
   ASSERT_EQ(NULL, arg->next);
}

TEST(ASTNodeArgumentTest, CreateNodeArgumentWithJustFloat64Value) {
   auto baseValue = 3.0;
   auto value = createFloatingPointValue(baseValue);
   auto arg = createNodeArg(NULL, value, NULL);

   ASSERT_STREQ(NULL, arg->getName());
   ASSERT_EQ(value, arg->getValue());
   ASSERT_EQ(NULL, arg->next);
}

TEST(ASTNodeArgumentTest, CreateNodeArgumentWithJustStringValue) {
   auto baseValue = "a simple string";
   auto value = createStrValue(baseValue);
   auto arg = createNodeArg(NULL, value, NULL);

   ASSERT_STREQ(NULL, arg->getName());
   ASSERT_EQ(value, arg->getValue());
   ASSERT_EQ(NULL, arg->next);
}

TEST(ASTNodeArgumentTest, CreateNodeArgumentWithNameAndInt64Value) {
   auto baseValue = 3UL;
   auto argName = "the argument name";
   auto value = createIntegerValue(baseValue);
   auto arg = createNodeArg(argName, value, NULL);

   ASSERT_STREQ(argName, arg->getName());
   ASSERT_EQ(value, arg->getValue());
   ASSERT_EQ(NULL, arg->next);
}

TEST(ASTNodeArgumentTest, CreateListFrom2SingleNodeArguments) {
   auto baseValue_1 = "a simple string";
   auto argName_1 = "another name";
   auto value_1 = createStrValue(baseValue_1);
   auto arg_1 = createNodeArg(argName_1, value_1, NULL);

   auto baseValue_0 = 3.0;
   auto argName_0 = "the argument name";
   auto value_0 = createFloatingPointValue(baseValue_0);
   auto arg_0 = createNodeArg(argName_0, value_0, arg_1);

   ASSERT_STREQ(argName_0, arg_0->getName());
   ASSERT_EQ(value_0, arg_0->getValue());
   ASSERT_EQ(arg_1, arg_0->next);

   ASSERT_STREQ(argName_1, arg_1->getName());
   ASSERT_EQ(value_1, arg_1->getValue());
   ASSERT_EQ(NULL, arg_1->next);
}

TEST(ASTNodeArgumentTest, Concatenate2SingleNodeArguments) {
   auto baseValue_1 = "a simple string";
   auto argName_1 = "another name";
   auto value_1 = createStrValue(baseValue_1);
   auto arg_1 = createNodeArg(argName_1, value_1, NULL);

   auto baseValue_0 = 3.0;
   auto argName_0 = "the argument name";
   auto value_0 = createFloatingPointValue(baseValue_0);
   auto arg_0 = createNodeArg(argName_0, value_0, NULL);

   appendSiblingArg(arg_0, arg_1);

   ASSERT_STREQ(argName_0, arg_0->getName());
   ASSERT_EQ(value_0, arg_0->getValue());
   ASSERT_EQ(arg_1, arg_0->next);

   ASSERT_STREQ(argName_1, arg_1->getName());
   ASSERT_EQ(value_1, arg_1->getValue());
   ASSERT_EQ(NULL, arg_1->next);
}

TEST(ASTNodeArgumentTest, CreateListFromListOf2AndSingleArgument) {
   auto baseValue_2 = 3.0;
   auto argName_2 = "the argument name";
   auto value_2 = createFloatingPointValue(baseValue_2);
   auto arg_2 = createNodeArg(argName_2, value_2, NULL);

   auto baseValue_1 = "a simple string";
   auto argName_1 = "another name";
   auto value_1 = createStrValue(baseValue_1);
   auto arg_1 = createNodeArg(argName_1, value_1, NULL);

   auto baseValue_0 = 3.0;
   auto argName_0 = "yet another name";
   auto value_0 = createFloatingPointValue(baseValue_0);
   auto arg_0 = createNodeArg(argName_0, value_0, arg_1);

   appendSiblingArg(arg_0, arg_2);

   ASSERT_STREQ(argName_0, arg_0->getName());
   ASSERT_EQ(value_0, arg_0->getValue());
   ASSERT_EQ(arg_1, arg_0->next);

   ASSERT_STREQ(argName_1, arg_1->getName());
   ASSERT_EQ(value_1, arg_1->getValue());
   ASSERT_EQ(arg_2, arg_1->next);

   ASSERT_STREQ(argName_2, arg_2->getName());
   ASSERT_EQ(value_2, arg_2->getValue());
   ASSERT_EQ(NULL, arg_2->next);
}

TEST(ASTNodeArgumentTest, CreateListFrom3SingleArguments) {
   auto baseValue_2 = 3.0;
   auto argName_2 = "the argument name";
   auto value_2 = createFloatingPointValue(baseValue_2);
   auto arg_2 = createNodeArg(argName_2, value_2, NULL);

   auto baseValue_1 = "a simple string";
   auto argName_1 = "another name";
   auto value_1 = createStrValue(baseValue_1);
   auto arg_1 = createNodeArg(argName_1, value_1, NULL);

   auto baseValue_0 = 3.0;
   auto argName_0 = "yet another name";
   auto value_0 = createFloatingPointValue(baseValue_0);
   auto arg_0 = createNodeArg(argName_0, value_0, NULL);

   appendSiblingArg(arg_0, arg_1);
   appendSiblingArg(arg_0, arg_2);

   ASSERT_STREQ(argName_0, arg_0->getName());
   ASSERT_EQ(value_0, arg_0->getValue());
   ASSERT_EQ(arg_1, arg_0->next);

   ASSERT_STREQ(argName_1, arg_1->getName());
   ASSERT_EQ(value_1, arg_1->getValue());
   ASSERT_EQ(arg_2, arg_1->next);

   ASSERT_STREQ(argName_2, arg_2->getName());
   ASSERT_EQ(value_2, arg_2->getValue());
   ASSERT_EQ(NULL, arg_2->next);
}

TEST(ASTNodeTest, CreateNullNode) {
   auto node = createNode(NULL, NULL, NULL, NULL);

   ASSERT_STREQ(NULL, node->getName());
   ASSERT_EQ(NULL, node->getArgs());
   ASSERT_EQ(NULL, node->getChildren());
   ASSERT_EQ(NULL, node->next);
}

TEST(ASTNodeTest, CreateNodeWithJustName) {
   auto nodeName = "theName";
   auto node = createNode(nodeName, NULL, NULL, NULL);

   ASSERT_STREQ(nodeName, node->getName());
   ASSERT_EQ(NULL, node->getArgs());
   ASSERT_EQ(NULL, node->getChildren());
   ASSERT_EQ(NULL, node->next);
}

TEST(ASTNodeTest, CreateNodeWithJust1Argument) {
   auto nodeArgValue = createFloatingPointValue(3.0);
   auto nodeArg = createNodeArg("someArgument", nodeArgValue, NULL);
   auto node = createNode(NULL, nodeArg, NULL, NULL);

   ASSERT_STREQ(NULL, node->getName());
   ASSERT_EQ(nodeArg, node->getArgs());
   ASSERT_EQ(NULL, node->getChildren());
   ASSERT_EQ(NULL, node->next);
}

ASTNodeArg* getNodeArgList() {
   auto nodeArgValue_2 = createFloatingPointValue(3.0);
   auto nodeArg_2 = createNodeArg("someArgument", nodeArgValue_2, NULL);
   auto nodeArgValue_1 = createIntegerValue(3);
   auto nodeArg_1 = createNodeArg("someArgument", nodeArgValue_1, nodeArg_2);
   auto nodeArgValue_0 = createStrValue("some string");
   auto nodeArg_0 = createNodeArg("someArgument", nodeArgValue_0, nodeArg_1);

   return nodeArg_0;
}

TEST(ASTNodeTest, CreateNodeWithJustArgumentList) {
   auto argList = getNodeArgList();
   auto node = createNode(NULL, argList, NULL, NULL);

   ASSERT_STREQ(NULL, node->getName());
   ASSERT_EQ(argList, node->getArgs());
   ASSERT_EQ(argList->next, node->getArgs()->next);
   ASSERT_EQ(argList->next->next, node->getArgs()->next->next);
   ASSERT_EQ(argList->next->next->next, node->getArgs()->next->next->next);
   ASSERT_EQ(NULL, node->getChildren());
   ASSERT_EQ(NULL, node->next);
}

TEST(ASTNodeTest, CreateListFrom2SingleNodes) {
   auto nodeName_1 = "a node name";
   ASTNodeArg* argList_1 = NULL;
   auto node_1 = createNode(nodeName_1, argList_1, NULL, NULL);
   auto nodeName_0 = "another node name";
   ASTNodeArg* argList_0 = getNodeArgList();
   auto node_0 = createNode(nodeName_0, argList_0, NULL, NULL);

   appendSiblingNode(node_0, node_1);

   ASSERT_STREQ(nodeName_0, node_0->getName());
   ASSERT_EQ(argList_0, node_0->getArgs());
   ASSERT_EQ(NULL, node_0->getChildren());
   ASSERT_EQ(node_1, node_0->next);

   ASSERT_STREQ(nodeName_1, node_1->getName());
   ASSERT_EQ(argList_1, node_1->getArgs());
   ASSERT_EQ(NULL, node_1->getChildren());
   ASSERT_EQ(NULL, node_1->next);
}

TEST(ASTNodeTest, Concatenate2SingleNodes)  {
   auto nodeName_1 = "a node name";
   ASTNodeArg* argList_1 = NULL;
   auto node_1 = createNode(nodeName_1, argList_1, NULL, NULL);
   auto nodeName_0 = "another node name";
   ASTNodeArg* argList_0 = getNodeArgList();
   auto node_0 = createNode(nodeName_0, argList_0, NULL, node_1);

   ASSERT_STREQ(nodeName_0, node_0->getName());
   ASSERT_EQ(argList_0, node_0->getArgs());
   ASSERT_EQ(NULL, node_0->getChildren());
   ASSERT_EQ(node_1, node_0->next);

   ASSERT_STREQ(nodeName_1, node_1->getName());
   ASSERT_EQ(argList_1, node_1->getArgs());
   ASSERT_EQ(NULL, node_1->getChildren());
   ASSERT_EQ(NULL, node_1->next);
}

TEST(ASTNodeTest, CreateListFromListOf2AndSingleNode) {
   auto nodeName_2 = "some node name";
   ASTNodeArg* argList_2 = getNodeArgList();
   auto node_2 = createNode(nodeName_2, argList_2, NULL, NULL);
   auto nodeName_1 = "a node name";
   ASTNodeArg* argList_1 = NULL;
   auto node_1 = createNode(nodeName_1, argList_1, NULL, NULL);
   auto nodeName_0 = "another node name";
   ASTNodeArg* argList_0 = getNodeArgList();
   auto node_0 = createNode(nodeName_0, argList_0, NULL, NULL);

   appendSiblingNode(node_0, node_1);
   appendSiblingNode(node_0, node_2);

   ASSERT_STREQ(nodeName_0, node_0->getName());
   ASSERT_EQ(argList_0, node_0->getArgs());
   ASSERT_EQ(NULL, node_0->getChildren());
   ASSERT_EQ(node_1, node_0->next);

   ASSERT_STREQ(nodeName_1, node_1->getName());
   ASSERT_EQ(argList_1, node_1->getArgs());
   ASSERT_EQ(NULL, node_1->getChildren());
   ASSERT_EQ(node_2, node_1->next);

   ASSERT_STREQ(nodeName_2, node_2->getName());
   ASSERT_EQ(argList_2, node_2->getArgs());
   ASSERT_EQ(NULL, node_2->getChildren());
   ASSERT_EQ(NULL, node_2->next);
}

TEST(ASTNodeTest, Concatenate3SingleNodes)  {
   auto nodeName_2 = "some node name";
   ASTNodeArg* argList_2 = getNodeArgList();
   auto node_2 = createNode(nodeName_2, argList_2, NULL, NULL);
   auto nodeName_1 = "a node name";
   ASTNodeArg* argList_1 = NULL;
   auto node_1 = createNode(nodeName_1, argList_1, NULL, NULL);
   auto nodeName_0 = "another node name";
   ASTNodeArg* argList_0 = getNodeArgList();
   auto node_0 = createNode(nodeName_0, argList_0, NULL, node_1);

   appendSiblingNode(node_0, node_2);

   ASSERT_STREQ(nodeName_0, node_0->getName());
   ASSERT_EQ(argList_0, node_0->getArgs());
   ASSERT_EQ(NULL, node_0->getChildren());
   ASSERT_EQ(node_1, node_0->next);

   ASSERT_STREQ(nodeName_1, node_1->getName());
   ASSERT_EQ(argList_1, node_1->getArgs());
   ASSERT_EQ(NULL, node_1->getChildren());
   ASSERT_EQ(node_2, node_1->next);

   ASSERT_STREQ(nodeName_2, node_2->getName());
   ASSERT_EQ(argList_2, node_2->getArgs());
   ASSERT_EQ(NULL, node_2->getChildren());
   ASSERT_EQ(NULL, node_2->next);
}

