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

#include "method_handler.hpp"

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

MethodHandler::MethodHandler(ASTNode* methodNode)
    : _methodNode{methodNode}, _ilInjector{methodNode->children, &_types}, _entry_point{nullptr} {

    // assume the first argument of the method's AST node specifies the return type
    _returnType = _types.PrimitiveType(getTRDataTypes(_methodNode->args->value.value.str));

    // assume the remaining arguments specify the types of the arguments
    auto argType = _methodNode->args->next;
    while (argType) {
        _argTypes.push_back(_types.PrimitiveType(getTRDataTypes(argType->value.value.str)));
        argType = argType->next;
    }
}

int32_t MethodHandler::compile() {
    TR::ResolvedMethod compilee("no_file", "no_line", "no_name", _argTypes.size(), _argTypes.data(), _returnType, 0, &_ilInjector);
    TR::IlGeneratorMethodDetails methodDetails(&compilee);
    int32_t rc = 0;
    _entry_point = compileMethodFromDetails(NULL, methodDetails, warm, rc);
    return rc;
}

TR::DataTypes MethodHandler::getTRDataTypes(const std::string& name) {
    if (name == "Int8") return TR::Int8;
    else if (name == "Int16") return TR::Int16;
    else if (name == "Int32") return TR::Int32;
    else if (name == "Int64") return TR::Int64;
    else if (name == "Address") return TR::Address;
    else if (name == "Float") return TR::Float;
    else if (name == "Double") return TR::Double;
    else if (name == "NoType") return TR::NoType;
    else {
        TR_ASSERT(0, "Unkown type name: %s", name.c_str());
        return TR::NoType;
    }
}
