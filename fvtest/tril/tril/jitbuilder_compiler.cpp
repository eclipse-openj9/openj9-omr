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

int32_t Tril::JitBuilderCompiler::compile() {
    // construct an IL generator for the method
    auto methodInfo = getMethodInfo();
    TR::TypeDictionary types;
    Tril::TRLangBuilder ilgenerator{methodInfo.getBodyAST(), &types};

    // get a list of the method's argument types and transform it
    // into a list of `TR::IlType`
    auto argTypes = methodInfo.getArgTypes();
    auto argIlTypes = std::vector<TR::IlType*>{argTypes.size()};
    std::transform(argTypes.cbegin(), argTypes.cend(), argIlTypes.begin(),
                   [&types](TR::DataTypes d){ return types.PrimitiveType(d); } );

    // construct a `TR::ResolvedMethod` instance from the IL generator and use
    // to compile the method
    TR::ResolvedMethod resolvedMethod("file", "line", "name",
                                      argIlTypes.size(),
                                      argIlTypes.data(),
                                      types.PrimitiveType(methodInfo.getReturnType()),
                                      0,
                                      &ilgenerator);
    TR::IlGeneratorMethodDetails methodDetails(&resolvedMethod);
    int32_t rc = 0;
    auto entry_point = compileMethodFromDetails(nullptr, methodDetails, warm, rc);

    // if compilation was successful, set the entry point for the compiled body
    if (rc == 0) setEntryPoint(entry_point);

    // return the return code for the compilation
    return rc;
}
