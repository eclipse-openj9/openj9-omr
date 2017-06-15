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

#ifndef JITTEST_HPP
#define JITTEST_HPP

#include <gtest/gtest.h>
#include "Jit.hpp"

/**
 * @brief The JitBuilderTest class is a basic test fixture for JitBuilder test cases.
 *
 * Most JitBuilder test case fixtures should publically inherit from this class.
 *
 * Example use:
 *
 *    class MyTestCase : public JitBuilderTest {};
 */
class JitTest : public ::testing::Test
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

#endif // JITTEST_HPP
