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

#include "jitbuilder_compiler.hpp"

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

#include <algorithm>

int32_t JitBuilderCompiler::compile() {
    auto methodInfo = getMethodInfo();
    TR::TypeDictionary types;
    TRLangBuilder ilgenerator{methodInfo->getBodyAST(), &types};

    auto argTypes = methodInfo->getArgTypes();
    auto argIlTypes = std::vector<TR::IlType*>{argTypes.size()};
    std::transform(argTypes.cbegin(), argTypes.cend(), argIlTypes.begin(),
                   [&types](TR::DataTypes d){ return types.PrimitiveType(d); } );

    TR::ResolvedMethod resolvedMethod("file", "line", "name",
                                      argIlTypes.size(),
                                      argIlTypes.data(),
                                      types.PrimitiveType(methodInfo->getReturnType()),
                                      0,
                                      &ilgenerator);
    TR::IlGeneratorMethodDetails methodDetails(&resolvedMethod);
    int32_t rc = 0;
    setEntryPoint(compileMethodFromDetails(nullptr, methodDetails, warm, rc));

    return rc;
}
