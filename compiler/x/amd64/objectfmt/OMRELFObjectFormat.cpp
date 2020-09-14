/*******************************************************************************
 * Copyright (c) 2020, 2020 IBM Corp. and others
 *
 * This program and the accompanying materials are made available under
 * the terms of the Eclipse Public License 2.0 which accompanies this
 * distribution and is available at http://eclipse.org/legal/epl-2.0
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
 * [2] http://openjdk.java.net/legal/assembly-exception.html
 *
 * SPDX-License-Identifier: EPL-2.0 OR Apache-2.0 OR GPL-2.0 WITH Classpath-exception-2.0 OR LicenseRef-GPL-2.0 WITH Assembly-exception
 *******************************************************************************/

#include "codegen/CodeGenerator.hpp"
#include "codegen/MemoryReference.hpp"
#include "codegen/X86Instruction.hpp"
#include "compile/Compilation.hpp"
#include "il/MethodSymbol.hpp"
#include "il/SymbolReference.hpp"
#include "infra/Assert.hpp"
#include "objectfmt/ELFObjectFormat.hpp"
#include "objectfmt/GlobalFunctionCallData.hpp"


TR::Symbol *
OMR::X86::AMD64::ELFObjectFormat::preparePLTEntry(TR::SymbolReference *methodSymRef)
   {
   TR_UNIMPLEMENTED();

   return NULL;
   }


TR::Instruction *
OMR::X86::AMD64::ELFObjectFormat::emitGlobalFunctionCall(TR::GlobalFunctionCallData &data)
   {
   TR_UNIMPLEMENTED();
   return 0;

/*
   // To enable the following requires a self() method to be added to the base class of
   // the ELFObjectFormat hierarchy.

   TR::Compilation *comp = data.cg->comp();

   TR::Symbol *pltEntrySymbol = self()->preparePLTEntry(data.globalMethodSymRef);

   TR::SymbolReference *pltEntrySymRef = new (comp->trHeapMemory()) TR::SymbolReference(comp->getSymRefTab(), pltEntrySymbol, 0);

   TR::Instruction *callInstruction = generateMemInstruction(CALLMem, data.callNode,
      new (comp->trHeapMemory()) TR::MemoryReference(pltEntrySymRef, data.cg, true),
      data.cg);

   return callInstruction;
*/

   }
