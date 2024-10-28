/*******************************************************************************
 * Copyright IBM Corp. and others 2024
 *
 * This program and the accompanying materials are made available under
 * the terms of the Eclipse Public License 2.0 which accompanies this
 * distribution and is available at https://www.eclipse.org/legal/epl-2.0/
 * or the Apache License, Version 2.0 which accompanies this distribution
 * and is available at https://www.apache.org/licenses/LICENSE-2.0.
 *
 * This Source Code may also be made available under the following Secondary
 * Licenses when the conditions for such availability set forth in the
 * Eclipse Public License, v. 2.0 are satisfied: GNU General Public License,
 * version 2 with the GNU Classpath Exception [1] and GNU General Public
 * License, version 2 with the OpenJDK Assembly Exception [2].
 *
 * [1] https://www.gnu.org/software/classpath/license.html
 * [2] https://openjdk.org/legal/assembly-exception.html
 *
 * SPDX-License-Identifier: EPL-2.0 OR Apache-2.0 OR GPL-2.0-only WITH Classpath-exception-2.0 OR GPL-2.0-only WITH OpenJDK-assembly-exception-1.0
 *******************************************************************************/

#include "optimizer/TrivialDeadStoreElimination.hpp"

#include "compile/SymbolReferenceTable.hpp"
#include "infra/ILWalk.hpp"

TR::TrivialDeadStoreElimination::TrivialDeadStoreElimination(TR::OptimizationManager *manager)
    : TR::Optimization(manager)
{}

int32_t TR::TrivialDeadStoreElimination::perform()
{
    TR::Region &stackRegion = comp()->trMemory()->currentStackRegion();

    TR::vector<bool, TR::Region &> isAutoUsed(stackRegion);
    isAutoUsed.resize(comp()->getSymRefTab()->getNumSymRefs(), false);

    TR::vector<TR::TreeTop *, TR::Region &> autoStores(stackRegion);

    TR::TreeTop *start = comp()->getStartTree();
    for (TR::PostorderNodeIterator it(start, comp()); it != NULL; ++it) {
        TR::Node *node = it.currentNode();
        if (node->getOpCode().isStoreDirect()) {
            if (node->getSymbol()->isAutoOrParm() && !node->dontEliminateStores(false)) {
                autoStores.push_back(it.currentTree());
            }

            continue;
        }

        // loadaddr of auto/parm is used to pass references to JNI. The callee
        // can dereference the pointer to read the reference from the stack slot.
        // So count loadaddr as a use.
        if (!node->getOpCode().isLoadVarDirect() && node->getOpCodeValue() != TR::loadaddr) {
            continue;
        }

        TR::SymbolReference *symRef = node->getSymbolReference();
        if (!symRef->getSymbol()->isAutoOrParm()) {
            continue;
        }

        int32_t symRefNum = symRef->getReferenceNumber();
        TR_ASSERT_FATAL(symRefNum < isAutoUsed.size(), "sym ref number out of bounds");
        isAutoUsed.at(symRefNum) = true;
    }

    bool didEliminateStores = false;
    for (auto it = autoStores.begin(); it != autoStores.end(); it++) {
        TR::TreeTop *tt = *it;
        TR::Node *node = tt->getNode();
        TR::SymbolReference *symRef = node->getSymbolReference();
        int32_t symRefNum = symRef->getReferenceNumber();
        TR_ASSERT_FATAL(symRefNum < isAutoUsed.size(), "sym ref number out of bounds");
        if (!isAutoUsed.at(symRefNum)
            && performTransformation(comp(), "%sEliminate store n%un [%p] to nowhere-used auto/parm #%d\n",
                optDetailString(), node->getGlobalIndex(), node, symRefNum)) {
            TR::Node::recreate(node, TR::treetop);
            didEliminateStores = true;
        }
    }

    if (didEliminateStores) {
        optimizer()->setRequestOptimization(OMR::deadTreesElimination);
    }

    return 1;
}

const char *TR::TrivialDeadStoreElimination::optDetailString() const throw()
{
    return "O^O TRIVIAL DEAD STORE ELIMINATION: ";
}
