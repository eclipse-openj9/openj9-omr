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

#ifndef REGISTERREMATERIALIZATION_INCL
#define REGISTERREMATERIALIZATION_INCL

#include "codegen/RegisterConstants.hpp"  // for TR_RematerializableTypes
#include "env/jittypes.h"                 // for intptrj_t

namespace TR { class CodeGenerator; }
namespace TR { class Instruction; }
namespace TR { class MemoryReference; }
namespace TR { class Node; }
namespace TR { class Register; }
namespace TR { class SymbolReference; }

void setDiscardableIfPossible(TR_RematerializableTypes, TR::Register *, TR::Node *, TR::Instruction *, TR::MemoryReference  *, TR::CodeGenerator *);
void setDiscardableIfPossible(TR_RematerializableTypes, TR::Register *, TR::Node *, TR::Instruction *, intptrj_t, TR::CodeGenerator *);
void setDiscardableIfPossible(TR_RematerializableTypes, TR::Register *, TR::Node *, TR::Instruction *, TR::SymbolReference*, TR::CodeGenerator *);

#endif
