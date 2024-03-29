/*******************************************************************************
 * Copyright IBM Corp. and others 2020
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


#ifndef TR_IDT_BUILDER_INCL
#define TR_IDT_BUILDER_INCL

#include "optimizer/abstractinterpreter/OMRIDTBuilder.hpp"

namespace TR { class ResolvedMethodSymbol; }
namespace TR { class Region; }
namespace TR { class Compilation; }
class TR_InlinerBase;

namespace TR
{

class OMR_EXTENSIBLE IDTBuilder : public OMR::IDTBuilderConnector
   {
   public:
   IDTBuilder(TR::ResolvedMethodSymbol* symbol, int32_t budget, TR::Region& region, TR::Compilation* comp, TR_InlinerBase* inliner) :
      OMR::IDTBuilderConnector(symbol, budget, region, comp, inliner) {}
   };
}

#endif
