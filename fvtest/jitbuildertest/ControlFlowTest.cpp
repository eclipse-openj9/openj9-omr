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

#include "ilgen/BytecodeBuilder.hpp"

typedef void (*DoubleReturnFunction)(void);

DEFINE_BUILDER( DoubleReturn,
                NoType )
   {
   auto firstBuilder = OrphanBytecodeBuilder(0, (char *)"return");
   AppendBuilder(firstBuilder);
   firstBuilder->Return();

   auto secondBuilder = OrphanBytecodeBuilder(1, (char *)"return"); // should be ignored and cleaned up
   secondBuilder->Return();

   return true;
   }

class ControlfFlowTest : public JitBuilderTest {};

TEST_F(ControlfFlowTest, DoubleReturnTest)
   {
   DoubleReturnFunction doubleReturn;
   ASSERT_COMPILE(TR::TypeDictionary, DoubleReturn, doubleReturn);
   ASSERT_NO_FATAL_FAILURE(doubleReturn());
   }
