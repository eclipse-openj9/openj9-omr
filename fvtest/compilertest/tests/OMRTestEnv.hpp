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
 *******************************************************************************/

#ifndef TEST_TESTS_OMRTESTENV_HPP_
#define TEST_TESTS_OMRTESTENV_HPP_

#include <limits.h>
#include <stdio.h>
#include <stdint.h>
#include "compile/Method.hpp"
#include "il/DataTypes.hpp"
#include "gtest/gtest.h"

class TR_Memory;

class OMRTestEnv : public testing::Environment
   {
   public:
   virtual void SetUp();
   virtual void TearDown();
   static TR_Memory *trMemory() { return _trMemory; }

   private:
   static TR_Memory *_trMemory;
};


#endif /* TEST_TESTS_OMRTESTENV_HPP_ */
