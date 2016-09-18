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

#include "ilgen/IlInjector.hpp"

#include "codegen/CodeGenerator.hpp"
#include "compile/Compilation.hpp"
#include "compile/SymbolReferenceTable.hpp"
#include "control/Recompilation.hpp"
#include "env/FrontEnd.hpp"
#include "compile/Method.hpp"
#include "env/StackMemoryRegion.hpp"
#include "il/Block.hpp"
#include "il/Node.hpp"
#include "il/Node_inlines.hpp"
#include "ilgen/IlGeneratorMethodDetails_inlines.hpp"
#include "ilgen/TypeDictionary.hpp"
#include "infra/Cfg.hpp"

#define OPT_DETAILS "O^O ILGEN: "

void
Test::IlInjector::setMethodAndTest(TR::IlInjector *source)
   {
   setMethodAndTest(source->_method, source->_test);
   }

void
Test::IlInjector::initialize(TR::IlGeneratorMethodDetails * details,
                             TR::ResolvedMethodSymbol     * methodSymbol,
                             TR::FrontEnd                 * fe,
                             TR::SymbolReferenceTable     * symRefTab)
   {
   this->OMR::IlInjector::initialize(details, methodSymbol, fe, symRefTab);
   _method = reinterpret_cast<TR::ResolvedMethod *>(methodSymbol->getResolvedMethod());
   }

TR::Node *
Test::IlInjector::callFunction(TR::ResolvedMethod *resolvedMethod, TR::IlType *returnType, int32_t numArgs, TR::Node *firstArg)
   {
   TR_ASSERT(numArgs == 1, "Hack alert: currently only supports single argument function calls!");

   // arbitrarily treat as "Static" so no receiver expected and should match use of a direct call opcode
   TR::SymbolReference *methodSymRef = symRefTab()->findOrCreateMethodSymbol(_methodSymbol->getResolvedMethodIndex(), 0, resolvedMethod, TR::MethodSymbol::Kinds::Static);
   TR::Node *callNode = TR::Node::createWithSymRef(TR::ILOpCode::getDirectCall(returnType->getPrimitiveType()), numArgs, methodSymRef);
   callNode->setAndIncChild(0, firstArg);
   return callNode;
   }
