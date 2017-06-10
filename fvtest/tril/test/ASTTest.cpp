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
#include "ast.h"

bool operator == (ASTValue lhs, ASTValue rhs) {
   if (lhs.type == rhs.type) {
      switch (lhs.type) {
         case Int64: return lhs.value.int64 == rhs.value.int64;
         case Double: return lhs.value.f64 == rhs.value.f64;
         case String: return std::strcmp(lhs.value.str, rhs.value.str) == 0;
      }
   }
   return false;
}

TEST(ASTValueTest, CreateInt64ASTValue) {
   auto baseValue = 3UL;

   auto value = createInt64Value(baseValue);

   ASSERT_EQ(Int64, value.type);
   ASSERT_EQ(baseValue, value.value.int64);
}

TEST(ASTValueTest, CreateFloat64ASTValue) {
   auto baseValue = 3.0;

   auto value = createDoubleValue(baseValue);

   ASSERT_EQ(Double, value.type);
   ASSERT_EQ(baseValue, value.value.f64);
}

TEST(ASTValueTest, CreateStringASTValue) {
   auto baseValue = "a simple string";

   auto value = createStrValue(baseValue);

   ASSERT_EQ(String, value.type);
   ASSERT_EQ(baseValue, value.value.str);
}

TEST(ASTNodeArgumentTest, CreateNodeArgumentWithJustInt64Value) {
   auto baseValue = 3UL;
   auto value = createInt64Value(baseValue);
   auto arg = createNodeArg(NULL, value, NULL);

   ASSERT_STREQ(NULL, arg->name);
   ASSERT_EQ(value, arg->value);
   ASSERT_EQ(NULL, arg->next);
}

TEST(ASTNodeArgumentTest, CreateNodeArgumentWithJustFloat64Value) {
   auto baseValue = 3.0;
   auto value = createDoubleValue(baseValue);
   auto arg = createNodeArg(NULL, value, NULL);

   ASSERT_STREQ(NULL, arg->name);
   ASSERT_EQ(value, arg->value);
   ASSERT_EQ(NULL, arg->next);
}

TEST(ASTNodeArgumentTest, CreateNodeArgumentWithJustStringValue) {
   auto baseValue = "a simple string";
   auto value = createStrValue(baseValue);
   auto arg = createNodeArg(NULL, value, NULL);

   ASSERT_STREQ(NULL, arg->name);
   ASSERT_EQ(value, arg->value);
   ASSERT_EQ(NULL, arg->next);
}

TEST(ASTNodeArgumentTest, CreateNodeArgumentWithNameAndInt64Value) {
   auto baseValue = 3UL;
   auto argName = "the argument name";
   auto value = createInt64Value(baseValue);
   auto arg = createNodeArg(argName, value, NULL);

   ASSERT_STREQ(argName, arg->name);
   ASSERT_EQ(value, arg->value);
   ASSERT_EQ(NULL, arg->next);
}

TEST(ASTNodeArgumentTest, CreateListFrom2SingleNodeArguments) {
   auto baseValue_1 = "a simple string";
   auto argName_1 = "another name";
   auto value_1 = createStrValue(baseValue_1);
   auto arg_1 = createNodeArg(argName_1, value_1, NULL);

   auto baseValue_0 = 3.0;
   auto argName_0 = "the argument name";
   auto value_0 = createDoubleValue(baseValue_0);
   auto arg_0 = createNodeArg(argName_0, value_0, arg_1);

   ASSERT_STREQ(argName_0, arg_0->name);
   ASSERT_EQ(value_0, arg_0->value);
   ASSERT_EQ(arg_1, arg_0->next);

   ASSERT_STREQ(argName_1, arg_1->name);
   ASSERT_EQ(value_1, arg_1->value);
   ASSERT_EQ(NULL, arg_1->next);
}

TEST(ASTNodeArgumentTest, Concatenate2SingleNodeArguments) {
   auto baseValue_1 = "a simple string";
   auto argName_1 = "another name";
   auto value_1 = createStrValue(baseValue_1);
   auto arg_1 = createNodeArg(argName_1, value_1, NULL);

   auto baseValue_0 = 3.0;
   auto argName_0 = "the argument name";
   auto value_0 = createDoubleValue(baseValue_0);
   auto arg_0 = createNodeArg(argName_0, value_0, NULL);

   appendSiblingArg(arg_0, arg_1);

   ASSERT_STREQ(argName_0, arg_0->name);
   ASSERT_EQ(value_0, arg_0->value);
   ASSERT_EQ(arg_1, arg_0->next);

   ASSERT_STREQ(argName_1, arg_1->name);
   ASSERT_EQ(value_1, arg_1->value);
   ASSERT_EQ(NULL, arg_1->next);
}

TEST(ASTNodeArgumentTest, CreateListFromListOf2AndSingleArgument) {
   auto baseValue_2 = 3.0;
   auto argName_2 = "the argument name";
   auto value_2 = createDoubleValue(baseValue_2);
   auto arg_2 = createNodeArg(argName_2, value_2, NULL);

   auto baseValue_1 = "a simple string";
   auto argName_1 = "another name";
   auto value_1 = createStrValue(baseValue_1);
   auto arg_1 = createNodeArg(argName_1, value_1, NULL);

   auto baseValue_0 = 3.0;
   auto argName_0 = "yet another name";
   auto value_0 = createDoubleValue(baseValue_0);
   auto arg_0 = createNodeArg(argName_0, value_0, arg_1);

   appendSiblingArg(arg_0, arg_2);

   ASSERT_STREQ(argName_0, arg_0->name);
   ASSERT_EQ(value_0, arg_0->value);
   ASSERT_EQ(arg_1, arg_0->next);

   ASSERT_STREQ(argName_1, arg_1->name);
   ASSERT_EQ(value_1, arg_1->value);
   ASSERT_EQ(arg_2, arg_1->next);

   ASSERT_STREQ(argName_2, arg_2->name);
   ASSERT_EQ(value_2, arg_2->value);
   ASSERT_EQ(NULL, arg_2->next);
}

TEST(ASTNodeArgumentTest, CreateListFrom3SingleArguments) {
   auto baseValue_2 = 3.0;
   auto argName_2 = "the argument name";
   auto value_2 = createDoubleValue(baseValue_2);
   auto arg_2 = createNodeArg(argName_2, value_2, NULL);

   auto baseValue_1 = "a simple string";
   auto argName_1 = "another name";
   auto value_1 = createStrValue(baseValue_1);
   auto arg_1 = createNodeArg(argName_1, value_1, NULL);

   auto baseValue_0 = 3.0;
   auto argName_0 = "yet another name";
   auto value_0 = createDoubleValue(baseValue_0);
   auto arg_0 = createNodeArg(argName_0, value_0, NULL);

   appendSiblingArg(arg_0, arg_1);
   appendSiblingArg(arg_0, arg_2);

   ASSERT_STREQ(argName_0, arg_0->name);
   ASSERT_EQ(value_0, arg_0->value);
   ASSERT_EQ(arg_1, arg_0->next);

   ASSERT_STREQ(argName_1, arg_1->name);
   ASSERT_EQ(value_1, arg_1->value);
   ASSERT_EQ(arg_2, arg_1->next);

   ASSERT_STREQ(argName_2, arg_2->name);
   ASSERT_EQ(value_2, arg_2->value);
   ASSERT_EQ(NULL, arg_2->next);
}

TEST(ASTNodeTest, CreateNullNode) {
   auto node = createNode(NULL, NULL, NULL, NULL);

   ASSERT_STREQ(NULL, node->name);
   ASSERT_EQ(NULL, node->args);
   ASSERT_EQ(NULL, node->children);
   ASSERT_EQ(NULL, node->next);
}

TEST(ASTNodeTest, CreateNodeWithJustName) {
   char* nodeName = "theName";

   auto node = createNode(nodeName, NULL, NULL, NULL);

   ASSERT_STREQ(nodeName, node->name);
   ASSERT_EQ(NULL, node->args);
   ASSERT_EQ(NULL, node->children);
   ASSERT_EQ(NULL, node->next);
}

TEST(ASTNodeTest, CreateNodeWithJust1Argument) {
   auto nodeArgValue = createDoubleValue(3.0);
   auto nodeArg = createNodeArg("someArgument", nodeArgValue, NULL);
   auto node = createNode(NULL, nodeArg, NULL, NULL);

   ASSERT_STREQ(NULL, node->name);
   ASSERT_EQ(nodeArg, node->args);
   ASSERT_EQ(NULL, node->children);
   ASSERT_EQ(NULL, node->next);
}

ASTNodeArg* getNodeArgList() {
   auto nodeArgValue_2 = createDoubleValue(3.0);
   auto nodeArg_2 = createNodeArg("someArgument", nodeArgValue_2, NULL);
   auto nodeArgValue_1 = createInt64Value(3);
   auto nodeArg_1 = createNodeArg("someArgument", nodeArgValue_1, nodeArg_2);
   auto nodeArgValue_0 = createStrValue("some string");
   auto nodeArg_0 = createNodeArg("someArgument", nodeArgValue_0, nodeArg_1);

   return nodeArg_0;
}

TEST(ASTNodeTest, CreateNodeWithJustArgumentList) {
   auto argList = getNodeArgList();
   auto node = createNode(NULL, argList, NULL, NULL);

   ASSERT_STREQ(NULL, node->name);
   ASSERT_EQ(argList, node->args);
   ASSERT_EQ(argList->next, node->args->next);
   ASSERT_EQ(argList->next->next, node->args->next->next);
   ASSERT_EQ(argList->next->next->next, node->args->next->next->next);
   ASSERT_EQ(NULL, node->children);
   ASSERT_EQ(NULL, node->next);
}

TEST(ASTNodeTest, CreateListFrom2SingleNodes) {
   char* nodeName_1 = "a node name";
   ASTNodeArg* argList_1 = NULL;
   auto node_1 = createNode(nodeName_1, argList_1, NULL, NULL);
   char* nodeName_0 = "another node name";
   ASTNodeArg* argList_0 = getNodeArgList();
   auto node_0 = createNode(nodeName_0, argList_0, NULL, NULL);

   appendSiblingNode(node_0, node_1);

   ASSERT_STREQ(nodeName_0, node_0->name);
   ASSERT_EQ(argList_0, node_0->args);
   ASSERT_EQ(NULL, node_0->children);
   ASSERT_EQ(node_1, node_0->next);

   ASSERT_STREQ(nodeName_1, node_1->name);
   ASSERT_EQ(argList_1, node_1->args);
   ASSERT_EQ(NULL, node_1->children);
   ASSERT_EQ(NULL, node_1->next);
}

TEST(ASTNodeTest, Concatenate2SingleNodes)  {
   char* nodeName_1 = "a node name";
   ASTNodeArg* argList_1 = NULL;
   auto node_1 = createNode(nodeName_1, argList_1, NULL, NULL);
   char* nodeName_0 = "another node name";
   ASTNodeArg* argList_0 = getNodeArgList();
   auto node_0 = createNode(nodeName_0, argList_0, NULL, node_1);

   ASSERT_STREQ(nodeName_0, node_0->name);
   ASSERT_EQ(argList_0, node_0->args);
   ASSERT_EQ(NULL, node_0->children);
   ASSERT_EQ(node_1, node_0->next);

   ASSERT_STREQ(nodeName_1, node_1->name);
   ASSERT_EQ(argList_1, node_1->args);
   ASSERT_EQ(NULL, node_1->children);
   ASSERT_EQ(NULL, node_1->next);
}

TEST(ASTNodeTest, CreateListFromListOf2AndSingleNode) {
   char* nodeName_2 = "some node name";
   ASTNodeArg* argList_2 = getNodeArgList();
   auto node_2 = createNode(nodeName_2, argList_2, NULL, NULL);
   char* nodeName_1 = "a node name";
   ASTNodeArg* argList_1 = NULL;
   auto node_1 = createNode(nodeName_1, argList_1, NULL, NULL);
   char* nodeName_0 = "another node name";
   ASTNodeArg* argList_0 = getNodeArgList();
   auto node_0 = createNode(nodeName_0, argList_0, NULL, NULL);

   appendSiblingNode(node_0, node_1);
   appendSiblingNode(node_0, node_2);

   ASSERT_STREQ(nodeName_0, node_0->name);
   ASSERT_EQ(argList_0, node_0->args);
   ASSERT_EQ(NULL, node_0->children);
   ASSERT_EQ(node_1, node_0->next);

   ASSERT_STREQ(nodeName_1, node_1->name);
   ASSERT_EQ(argList_1, node_1->args);
   ASSERT_EQ(NULL, node_1->children);
   ASSERT_EQ(node_2, node_1->next);

   ASSERT_STREQ(nodeName_2, node_2->name);
   ASSERT_EQ(argList_2, node_2->args);
   ASSERT_EQ(NULL, node_2->children);
   ASSERT_EQ(NULL, node_2->next);
}

TEST(ASTNodeTest, Concatenate3SingleNodes)  {
   char* nodeName_2 = "some node name";
   ASTNodeArg* argList_2 = getNodeArgList();
   auto node_2 = createNode(nodeName_2, argList_2, NULL, NULL);
   char* nodeName_1 = "a node name";
   ASTNodeArg* argList_1 = NULL;
   auto node_1 = createNode(nodeName_1, argList_1, NULL, NULL);
   char* nodeName_0 = "another node name";
   ASTNodeArg* argList_0 = getNodeArgList();
   auto node_0 = createNode(nodeName_0, argList_0, NULL, node_1);

   appendSiblingNode(node_0, node_2);

   ASSERT_STREQ(nodeName_0, node_0->name);
   ASSERT_EQ(argList_0, node_0->args);
   ASSERT_EQ(NULL, node_0->children);
   ASSERT_EQ(node_1, node_0->next);

   ASSERT_STREQ(nodeName_1, node_1->name);
   ASSERT_EQ(argList_1, node_1->args);
   ASSERT_EQ(NULL, node_1->children);
   ASSERT_EQ(node_2, node_1->next);

   ASSERT_STREQ(nodeName_2, node_2->name);
   ASSERT_EQ(argList_2, node_2->args);
   ASSERT_EQ(NULL, node_2->children);
   ASSERT_EQ(NULL, node_2->next);
}

