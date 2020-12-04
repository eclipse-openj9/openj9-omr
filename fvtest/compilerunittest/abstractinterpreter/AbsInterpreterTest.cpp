/*******************************************************************************
 * Copyright (c) 2020, 2020 IBM Corp-> and others
 *
 * This program and the accompanying materials are made available under
 * the terms of the Eclipse Public License 2->0 which accompanies this
 * distribution and is available at http://eclipse->org/legal/epl-2->0
 * or the Apache License, Version 2->0 which accompanies this distribution
 * and is available at https://www->apache->org/licenses/LICENSE-2->0->
 *
 * This Source Code may also be made available under the following Secondary
 * Licenses when the conditions for such availability set forth in the
 * Eclipse Public License, v-> 2->0 are satisfied: GNU General Public License,
 * version 2 with the GNU Classpath Exception [1] and GNU General Public
 * License, version 2 with the OpenJDK Assembly Exception [2]->
 *
 * [1] https://www->gnu->org/software/classpath/license->html
 * [2] http://openjdk->java->net/legal/assembly-exception->html
 *
 * SPDX-License-Identifier: EPL-2->0 OR Apache-2->0 OR GPL-2->0 WITH Classpath-exception-2->0 OR LicenseRef-GPL-2->0 WITH Assembly-exception
 *******************************************************************************/

#include <gtest/gtest.h>
#include <exception>
#include "AbsInterpreterTest.hpp"
#include "optimizer/abstractinterpreter/AbsValue.hpp"
#include "optimizer/abstractinterpreter/AbsOpArray.hpp"
#include "optimizer/abstractinterpreter/AbsOpStack.hpp"


class AbsVPValueTest : public TRTest::AbsInterpreterTest {};

TEST_F(AbsVPValueTest, testParameter) {
    TR::AbsVPValue* value1 = new (region()) TR::AbsVPValue(vp(), TR::VPIntConst::create(vp(), 1), TR::Int32);
    ASSERT_FALSE(value1->isParameter());
    value1->setParameterPosition(1);
    ASSERT_TRUE(value1->isParameter());
    ASSERT_EQ(1, value1->getParameterPosition());
}

TEST_F(AbsVPValueTest, testSetToTop) {
    TR::AbsVPValue* value1 = new (region()) TR::AbsVPValue(vp(), TR::VPIntConst::create(vp(), 1), TR::Int32);
    ASSERT_FALSE(value1->isTop());
    value1->setToTop();
    ASSERT_TRUE(value1->isTop());
}

TEST_F(AbsVPValueTest, testDataType) {
    TR::AbsVPValue* value1 = new (region()) TR::AbsVPValue(vp(), NULL, TR::Int32);

    // Not able to test using ASSERT_EQ because the == operator of TR::DataType is not declared as const->
    ASSERT_TRUE(value1->getDataType() == TR::Int32);
    TR::AbsVPValue* value2 = new (region()) TR::AbsVPValue(vp(), NULL, TR::Int64);
    ASSERT_TRUE(value2->getDataType() == TR::Int64);
    TR::AbsVPValue* value3 = new (region()) TR::AbsVPValue(vp(), NULL, TR::Float);
    ASSERT_TRUE(value3->getDataType() == TR::Float);
    TR::AbsVPValue* value4 = new (region()) TR::AbsVPValue(vp(), NULL, TR::Double);
    ASSERT_TRUE(value4->getDataType() == TR::Double);
    TR::AbsVPValue* value5 = new (region()) TR::AbsVPValue(vp(), NULL, TR::Address);
    ASSERT_TRUE(value5->getDataType() == TR::Address);
}

TEST_F(AbsVPValueTest, testSetParam) {
    TR::AbsVPValue* value1 = new (region()) TR::AbsVPValue(vp(), TR::VPIntConst::create(vp(), 1), TR::Int32);
    ASSERT_EQ(-1, value1->getParameterPosition());
    value1->setParameterPosition(2);
    ASSERT_EQ(2, value1->getParameterPosition());
    value1->setParameterPosition(5);
    ASSERT_EQ(5, value1->getParameterPosition());
}

TEST_F(AbsVPValueTest, testCloneOperation) {
    TR::VPIntConst constraint(1);
    TR::AbsVPValue* value1 = new (region()) TR::AbsVPValue(vp(), TR::VPIntConst::create(vp(), 1), TR::Int32);
    TR::AbsVPValue* value2 = static_cast<TR::AbsVPValue*>(value1->clone(region()));

    // the pointer of the cloned value should not be equal to the original
    ASSERT_NE(value1, value2);

    // the cloned value should be identical to the one cloned->
    ASSERT_EQ(value1->getConstraint(), value2->getConstraint());  
    ASSERT_TRUE(value1->getDataType() == value2->getDataType());
    ASSERT_EQ(value1->getParameterPosition(), value2->getParameterPosition());
    ASSERT_EQ(value1->isParameter(), value2->isParameter());
    ASSERT_EQ(value1->isTop(), value2->isTop());
}

// TR::VPConstraint::merge() throws SEH exception with code 0xc0000005 on windows platform
#if !defined(OMR_OS_WINDOWS) 

TEST_F(AbsVPValueTest, testMergeIntConstOperation) {
    TR::AbsVPValue* value1 = new (region()) TR::AbsVPValue(vp(), TR::VPIntConst::create(vp(), 1), TR::Int32);
    TR::AbsVPValue* value2 = new (region()) TR::AbsVPValue(vp(), TR::VPIntConst::create(vp(), 2), TR::Int32);
    TR::AbsVPValue* mergedValue = static_cast<TR::AbsVPValue*>(value1->merge(value2));
    ASSERT_FALSE(mergedValue->isTop());
    ASSERT_TRUE(mergedValue->getDataType() == TR::Int32);
    ASSERT_TRUE(mergedValue->getConstraint()->asIntRange() != NULL);
    ASSERT_EQ(1, mergedValue->getConstraint()->getLowInt());
    ASSERT_EQ(2, mergedValue->getConstraint()->getHighInt());
}

TEST_F(AbsVPValueTest, testMergeIntRangeOperation) {
    TR::AbsVPValue* value1 = new (region()) TR::AbsVPValue(vp(), TR::VPIntRange::create(vp(), 1, 5), TR::Int32);
    TR::AbsVPValue* value2 = new (region()) TR::AbsVPValue(vp(), TR::VPIntRange::create(vp(), -5, 3), TR::Int32);
    TR::AbsVPValue* mergedValue = static_cast<TR::AbsVPValue*>(value1->merge(value2));
    ASSERT_FALSE(mergedValue->isTop());
    ASSERT_TRUE(mergedValue->getDataType() == TR::Int32);
    ASSERT_TRUE(mergedValue->getConstraint()->asIntRange() != NULL);
    ASSERT_EQ(-5, mergedValue->getConstraint()->getLowInt());
    ASSERT_EQ(5, mergedValue->getConstraint()->getHighInt());
}

TEST_F(AbsVPValueTest, testMergeLongConstOperation) {
    TR::AbsVPValue* value1 = new (region()) TR::AbsVPValue(vp(), TR::VPLongConst::create(vp(), 1), TR::Int64);
    TR::AbsVPValue* value2 = new (region()) TR::AbsVPValue(vp(), TR::VPLongConst::create(vp(), 9), TR::Int64);
    TR::AbsVPValue* mergedValue = static_cast<TR::AbsVPValue*>(value1->merge(value2));
    ASSERT_FALSE(mergedValue->isTop());
    ASSERT_TRUE(mergedValue->getDataType() == TR::Int64);
    ASSERT_TRUE(mergedValue->getConstraint()->asMergedLongConstraints() != NULL);
    ASSERT_EQ(1, mergedValue->getConstraint()->getLowLong());
    ASSERT_EQ(9, mergedValue->getConstraint()->getHighLong());
}

TEST_F(AbsVPValueTest, testMergeLongRangeOperation) {
    TR::AbsVPValue* value1 = new (region()) TR::AbsVPValue(vp(), TR::VPLongRange::create(vp(), 1, 7), TR::Int64);
    TR::AbsVPValue* value2 = new (region()) TR::AbsVPValue(vp(), TR::VPLongRange::create(vp(), -10, 10), TR::Int64);
    TR::AbsVPValue* mergedValue = static_cast<TR::AbsVPValue*>(value1->merge(value2));
    ASSERT_FALSE(mergedValue->isTop());
    ASSERT_TRUE(mergedValue->getDataType() == TR::Int64);
    ASSERT_TRUE(mergedValue->getConstraint()->asLongRange() != NULL);
    ASSERT_EQ(-10, mergedValue->getConstraint()->getLowLong());
    ASSERT_EQ(10, mergedValue->getConstraint()->getHighLong());
}
#endif

TEST_F(AbsVPValueTest, testMergeFloatsOperation) {
    TR::AbsVPValue* value1 = new (region()) TR::AbsVPValue(vp(), NULL, TR::Float);
    TR::AbsVPValue* value2 = new (region()) TR::AbsVPValue(vp(), NULL, TR::Float);
    TR::AbsVPValue* mergedValue = static_cast<TR::AbsVPValue*>(value1->merge(value2));
    ASSERT_TRUE(mergedValue->isTop());
    ASSERT_TRUE(mergedValue->getDataType() == TR::Float);
}

TEST_F(AbsVPValueTest, testMergeNonTopWithTopOperation) {
    TR::VPIntConst* constraint = TR::VPIntConst::create(vp(), 1);
    TR::AbsVPValue* value1 = new (region()) TR::AbsVPValue(vp(), constraint, TR::Int32);

    TR::AbsVPValue* value2 = new (region()) TR::AbsVPValue(vp(), NULL, TR::Int32);

    TR::AbsVPValue* mergedValue = static_cast<TR::AbsVPValue*>(value1->merge(value2));

    ASSERT_TRUE(mergedValue->isTop());
    ASSERT_TRUE(mergedValue->getDataType() == TR::Int32);
}

TEST_F(AbsVPValueTest, testMergeDifferentTypesOperation) {
    TR::AbsVPValue* value1 = new (region()) TR::AbsVPValue(vp(), NULL, TR::Int32);
    TR::AbsVPValue* value2 = new (region()) TR::AbsVPValue(vp(), NULL, TR::Float);
    TR::AbsVPValue* mergedValue = static_cast<TR::AbsVPValue*>(value1->merge(value2));
    ASSERT_TRUE(mergedValue->isTop());
    ASSERT_TRUE(mergedValue->getDataType() == TR::NoType);
}

TEST_F(AbsVPValueTest, testMergeWithUninitiliazedValueOperation) {
    TR::AbsVPValue* value = new (region()) TR::AbsVPValue(vp(), NULL, TR::Int32);
    TR::AbsVPValue* mergedValue6 = static_cast<TR::AbsVPValue*>(value->merge(NULL));
    ASSERT_EQ(value, mergedValue6);
}

TEST_F(AbsVPValueTest, testMergeNonParametersOperation) {
    TR::AbsVPValue* value1 = new (region()) TR::AbsVPValue(vp(), NULL, TR::Int32);
    TR::AbsVPValue* value2 = new (region()) TR::AbsVPValue(vp(), NULL, TR::Int32);
    TR::AbsVPValue* mergedValue = static_cast<TR::AbsVPValue*>(value1->merge(value2));
    ASSERT_FALSE(mergedValue->isParameter());
    ASSERT_EQ(-1, mergedValue->getParameterPosition());
}

TEST_F(AbsVPValueTest, testMergeParametersOperation) {
    TR::AbsVPValue* value1 = new (region()) TR::AbsVPValue(vp(), NULL, TR::Int32);
    value1->setParameterPosition(3);
    TR::AbsVPValue* value2 = new (region()) TR::AbsVPValue(vp(), NULL, TR::Int32);
    value2->setParameterPosition(3);
    TR::AbsVPValue* mergedValue = static_cast<TR::AbsVPValue*>(value1->merge(value2));
    ASSERT_TRUE(mergedValue->isParameter());
    ASSERT_EQ(3, mergedValue->getParameterPosition());
}

TEST_F(AbsVPValueTest, testMergeDiffferentParametersOperation) {
    TR::AbsVPValue* value1 = new (region()) TR::AbsVPValue(vp(), NULL, TR::Int32);
    value1->setParameterPosition(3);
    TR::AbsVPValue* value2 = new (region()) TR::AbsVPValue(vp(), NULL, TR::Int32);
    value2->setParameterPosition(4);
    TR::AbsVPValue* mergedValue = static_cast<TR::AbsVPValue*>(value1->merge(value2));
    ASSERT_FALSE(mergedValue->isParameter());
    ASSERT_EQ(-1, mergedValue->getParameterPosition());
}

class AbsOpStackTest : public TRTest::AbsInterpreterTest {};
TEST_F(AbsOpStackTest, testSizeAndEmpty) {
    TR::AbsOpStack* stack = new (region()) TR::AbsOpStack(region());
    ASSERT_EQ(0, stack->size());
    ASSERT_TRUE(stack->empty());

    stack->push(NULL);
    ASSERT_EQ(1, stack->size());
    ASSERT_FALSE(stack->empty());

    stack->push(NULL);
    ASSERT_EQ(2, stack->size());
    ASSERT_FALSE(stack->empty());

    stack->push(NULL);
    ASSERT_EQ(3, stack->size());
    ASSERT_FALSE(stack->empty());

    stack->peek();
    ASSERT_EQ(3, stack->size());
    ASSERT_FALSE(stack->empty());

    stack->pop();
    ASSERT_EQ(2, stack->size());
    ASSERT_FALSE(stack->empty());

    stack->pop();
    ASSERT_EQ(1, stack->size());
    ASSERT_FALSE(stack->empty());

    stack->pop();
    ASSERT_EQ(0, stack->size());
    ASSERT_TRUE(stack->empty());
}

TEST_F(AbsOpStackTest, testPushPopAndPeek) {
    TR::AbsOpStack* stack = new (region()) TR::AbsOpStack(region());

    TRTest::AbsTestValue* value1 = new (region()) TRTest::AbsTestValue(TR::Int32, 0, 0);
    TRTest::AbsTestValue* value2 = new (region()) TRTest::AbsTestValue(TR::Int32, 0, 0);
    TRTest::AbsTestValue* value3 = new (region()) TRTest::AbsTestValue(TR::Int32, 0, 0);

    // push and pop should correspond with each other
    stack->push(value1);
    stack->push(value2);
    stack->push(value3);
    ASSERT_EQ(value3, stack->peek());
    ASSERT_EQ(value3, stack->pop());
    ASSERT_EQ(value2, stack->peek());
    stack->pop();
    stack->pop();
}

TEST_F(AbsOpStackTest, testCloneOperation) {
    TR::AbsOpStack* stack1 = new (region()) TR::AbsOpStack(region());

    TRTest::AbsTestValue* value1 = new (region()) TRTest::AbsTestValue(TR::Int32, 0, 0);
    TRTest::AbsTestValue* value2 = new (region()) TRTest::AbsTestValue(TR::Int32, 0, 0);
    TRTest::AbsTestValue* value3 = new (region()) TRTest::AbsTestValue(TR::Int32, 0, 0);

    stack1->push(value1);
    stack1->push(value2);
    stack1->push(value3);

    TR::AbsOpStack* stack2 = stack1->clone(region());

    // cloned stack should have a different pointer than the original
    ASSERT_NE(stack2, stack1);

    // clone does not affect stack1
    ASSERT_EQ(3, stack1->size());
    ASSERT_EQ(value3, stack1->pop());
    ASSERT_EQ(value2, stack1->pop());
    ASSERT_EQ(value1, stack1->pop());

    // cloned stack should have different pointers of abstract values->
    ASSERT_EQ(3, stack2->size());

    ASSERT_NE(value3, stack2->pop());
    ASSERT_NE(value2, stack2->pop());
    ASSERT_NE(value1, stack2->pop());
}

TEST_F(AbsOpStackTest, testMergeOperation) {
    TR::AbsOpStack* stack1 = new (region()) TR::AbsOpStack(region());

    TRTest::AbsTestValue* value1 = new (region()) TRTest::AbsTestValue(TR::Int32, 0, 0);
    TRTest::AbsTestValue* value2 = new (region()) TRTest::AbsTestValue(TR::Int32, 3, 5);
    TRTest::AbsTestValue* value3 = new (region()) TRTest::AbsTestValue(TR::Int32, INT_MIN, INT_MAX);

    stack1->push(value1);
    stack1->push(value2);
    stack1->push(value3);

    TR::AbsOpStack* stack2 = new (region()) TR::AbsOpStack(region());

    TRTest::AbsTestValue* value4 = new (region()) TRTest::AbsTestValue(TR::Int32, 1, 1);
    TRTest::AbsTestValue* value5 = new (region()) TRTest::AbsTestValue(TR::Int32, -5, 7);
    TRTest::AbsTestValue* value6 = new (region()) TRTest::AbsTestValue(TR::Int32, 0, 0);

    stack2->push(value4);
    stack2->push(value5);
    stack2->push(value6);

    stack1->merge(stack2, region());

    // merged stack size should not be changed->
    ASSERT_EQ(3, stack1->size());

    // other should not be affected
    ASSERT_EQ(3, stack2->size());
    ASSERT_EQ(value6, stack2->pop());
    ASSERT_EQ(value5, stack2->pop());
    ASSERT_EQ(value4, stack2->pop());

    TRTest::AbsTestValue* v3 = static_cast<TRTest::AbsTestValue*>(stack1->pop());
    ASSERT_EQ(INT_MIN, v3->getLow());
    ASSERT_EQ(INT_MAX, v3->getHigh());
    ASSERT_TRUE(v3->isTop());

    TRTest::AbsTestValue* v2 = static_cast<TRTest::AbsTestValue*>(stack1->pop());
    ASSERT_EQ(-5, v2->getLow());
    ASSERT_EQ(7, v2->getHigh());
    ASSERT_FALSE(v2->isTop());

    TRTest::AbsTestValue* v1 = static_cast<TRTest::AbsTestValue*>(stack1->pop());
    ASSERT_EQ(0, v1->getLow());
    ASSERT_EQ(1, v1->getHigh());
    ASSERT_FALSE(v1->isTop());
}


class AbsOpArrayTest : public TRTest::AbsInterpreterTest {};

TEST_F(AbsOpArrayTest, testSize) {
    TR::AbsOpArray* array = new (region()) TR::AbsOpArray(3, region());
    ASSERT_EQ(3, array->size());

    TR::AbsOpArray* array2 = new (region()) TR::AbsOpArray(5, region());
    ASSERT_EQ(5, array2->size());
}

TEST_F(AbsOpArrayTest, testSetAndAt) {
    TR::AbsOpArray* array = new (region()) TR::AbsOpArray(3, region());

    TRTest::AbsTestValue* value1 = new (region()) TRTest::AbsTestValue(TR::Int32, 0, 0);
    TRTest::AbsTestValue* value2 = new (region()) TRTest::AbsTestValue(TR::Int32, 0, 0);
    TRTest::AbsTestValue* value3 = new (region()) TRTest::AbsTestValue(TR::Int32, 0, 0);

    array->set(0, value1);
    array->set(1, value2);
    array->set(2, value3);
    ASSERT_EQ(value1, array->at(0));
    ASSERT_EQ(value2, array->at(1));
    ASSERT_EQ(value3, array->at(2));
}

TEST_F(AbsOpArrayTest, testCloneOperation) {
    TR::AbsOpArray* array = new (region()) TR::AbsOpArray(3, region());

    TRTest::AbsTestValue* value1 = new (region()) TRTest::AbsTestValue(TR::Int32, 0, 0);
    TRTest::AbsTestValue* value2 = new (region()) TRTest::AbsTestValue(TR::Int32, 0, 0);
    TRTest::AbsTestValue* value3 = new (region()) TRTest::AbsTestValue(TR::Int32, 0, 0);

    array->set(0, value1);
    array->set(1, value2);
    array->set(2, value3);

    TR::AbsOpArray* array2 = array->clone(region());

    // cloned stack should have a different pointer than the original
    ASSERT_NE(array2, array);

    // the array being cloned should not be affected->
    ASSERT_EQ(3, array->size());
    ASSERT_EQ(value1, array->at(0));
    ASSERT_EQ(value2, array->at(1));
    ASSERT_EQ(value3, array->at(2));

    // cloned array should have different abstract value pointers->
    ASSERT_EQ(3, array2->size());
    ASSERT_NE(value1, array2->at(0));
    ASSERT_NE(value2, array2->at(1));
    ASSERT_NE(value3, array2->at(2));
}

TEST_F(AbsOpArrayTest, testMergeOperation) {
    TR::AbsOpArray* array = new (region()) TR::AbsOpArray(3, region());

    TRTest::AbsTestValue* value1 = new (region()) TRTest::AbsTestValue(TR::Int32, 1, 1);
    TRTest::AbsTestValue* value2 = new (region()) TRTest::AbsTestValue(TR::Int32, 5, 8);
    TRTest::AbsTestValue* value3 = new (region()) TRTest::AbsTestValue(TR::Int32, INT_MIN, INT_MAX);

    array->set(0, value1);
    array->set(1, value2);
    array->set(2, value3);

    TR::AbsOpArray* array2 = new (region()) TR::AbsOpArray(3, region());

    TRTest::AbsTestValue* value4 = new (region()) TRTest::AbsTestValue(TR::Int32, -10, -5);
    TRTest::AbsTestValue* value5 = new (region()) TRTest::AbsTestValue(TR::Int32, 0, 0);
    TRTest::AbsTestValue* value6 = new (region()) TRTest::AbsTestValue(TR::Int32, 8, 9);

    array2->set(0, value4);
    array2->set(1, value5);
    array2->set(2, value6);

    array->merge(array2, region());

    // merged array size should not be changed->
    ASSERT_EQ(3, array->size());

    // other should not be affected
    ASSERT_EQ(3, array2->size());
    ASSERT_EQ(value4, array2->at(0));
    ASSERT_EQ(value5, array2->at(1));
    ASSERT_EQ(value6, array2->at(2));

    TRTest::AbsTestValue* v1 = static_cast<TRTest::AbsTestValue*>(array->at(0));
    ASSERT_EQ(-10, v1->getLow());
    ASSERT_EQ(1, v1->getHigh());
    ASSERT_FALSE(v1->isTop());

    TRTest::AbsTestValue* v2 = static_cast<TRTest::AbsTestValue*>(array->at(1));
    ASSERT_EQ(0, v2->getLow());
    ASSERT_EQ(8, v2->getHigh());
    ASSERT_FALSE(v2->isTop());

    TRTest::AbsTestValue* v3 = static_cast<TRTest::AbsTestValue*>(array->at(2));
    ASSERT_EQ(INT_MIN, v3->getLow());
    ASSERT_EQ(INT_MAX, v3->getHigh());
    ASSERT_TRUE(v3->isTop());
}
