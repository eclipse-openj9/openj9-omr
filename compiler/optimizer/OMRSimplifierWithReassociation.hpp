/*******************************************************************************
 * Copyright IBM Corp. and others 2026
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

#ifndef OMR_SIMPLIFIER_WITH_REASSOCIATION_INCL
#define OMR_SIMPLIFIER_WITH_REASSOCIATION_INCL

#include "optimizer/Simplifier.hpp"

namespace TR {
class OptimizationManager;
class Optimization;
} // namespace TR

namespace OMR {

/**
 * SimplifierWithReassociation
 * ===========================
 *
 * A thin subclass of Simplifier that forces _withReassociation = true regardless
 * of the TR_EnableFullReassociation command-line option.  Registered as the
 * treeSimplificationWithReassociation optimization.  Only the rules that have
 * been validated for unconditional use should check _withReassociation directly;
 * all other reassociation rules must additionally check enableFullReassociation()
 * (i.e. also require TR_EnableFullReassociation).
 */
class SimplifierWithReassociation : public TR::Simplifier {
public:
    SimplifierWithReassociation(TR::OptimizationManager *manager);

    static TR::Optimization *create(TR::OptimizationManager *manager);

    virtual const char *optDetailString() const throw();
};

} // namespace OMR
#endif
