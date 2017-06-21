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
#include "ilgen.hpp"

#include "env/jittypes.h"
#include "il/DataTypes.hpp"
#include "il/ILOpCodes.hpp"
#include "compile/Compilation.hpp"
#include "compile/CompilationTypes.hpp"
#include "compile/Method.hpp"
#include "control/CompileMethod.hpp"
#include "env/jittypes.h"
#include "gtest/gtest.h"
#include "il/DataTypes.hpp"
#include "ilgen.hpp"
#include "Jit.hpp"
#include "ilgen/IlGeneratorMethodDetails_inlines.hpp"

#define ASSERT_NULL(pointer) ASSERT_EQ(nullptr, (pointer))
#define ASSERT_NOTNULL(pointer) ASSERT_TRUE(nullptr != (pointer))

class IlGenTest : public JitTest {};

TEST_F(IlGenTest, Return3) {
    auto trees = parseString("(block (ireturn (iconst 3)))");

    TR::TypeDictionary types;
    auto Int32 = types.PrimitiveType(TR::Int32);
    TR::IlType* argTypes[] = {Int32};

    auto injector = TRLangBuilder{trees, &types};
    TR::ResolvedMethod compilee{__FILE__, LINETOSTR(__LINE__), "Return3InIL", sizeof(argTypes)/sizeof(TR::IlType*), argTypes, Int32, 0, &injector};
    TR::IlGeneratorMethodDetails methodDetails{&compilee};
    int32_t rc = 0;
    auto entry_point = compileMethodFromDetails(NULL, methodDetails, warm, rc);

    ASSERT_EQ(0, rc) << "Compilation failed";
    ASSERT_NOTNULL(entry_point) << "Entry point of compiled body cannot be null";

    auto entry = reinterpret_cast<int32_t(*)(void)>(entry_point);
    ASSERT_EQ(3, entry()) << "Compiled body did not return expected value";
}
