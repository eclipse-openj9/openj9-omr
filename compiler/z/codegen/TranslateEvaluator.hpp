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

#ifndef S390TRANSLATEEVALUATOR_INCL
#define S390TRANSLATEEVALUATOR_INCL

#include <stddef.h>                // for NULL
#include <stdint.h>                // for uint8_t
#include "codegen/InstOpCode.hpp"  // for InstOpCode, InstOpCode::Mnemonic, etc

namespace TR { class CodeGenerator; }
namespace TR { class Node; }
namespace TR { class Register; }

TR::InstOpCode::S390BranchCondition getConditionCodeFromCCCompare(uint8_t maskOnIf, uint8_t compVal);
TR::Register *inlineTrtEvaluator(TR::Node *node, TR::CodeGenerator *cg, TR::InstOpCode::Mnemonic opCode, bool isFoldedIf = false, bool isFoldedLookup = false, TR::Node *parent = NULL);
TR::Register *inlineTrEvaluator(TR::Node *node, TR::CodeGenerator *cg);

#endif
