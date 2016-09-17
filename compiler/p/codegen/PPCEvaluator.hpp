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

#ifndef PPC_TREE_EVALUATOR_INCL
#define PPC_TREE_EVALUATOR_INCL


#include <stddef.h>                         // for NULL
#include <stdint.h>                         // for int32_t
#include "codegen/TreeEvaluator.hpp"

namespace TR { class CodeGenerator; }
namespace TR { class Node; }
namespace TR { class Register; }



void simplifyANDRegImm(TR::Node *, TR::Register *trgReg, TR::Register *srcReg, int32_t value, TR::CodeGenerator *cg, TR::Node *constNode=NULL);

#define VOID_BODY
#define NULL_BODY
#define BOOL_BODY

class TR_PPCComputeCC : public TR::TreeEvaluator
   {
   public:
   static bool setCarryBorrow(TR::Node *flagNode, bool invertValue, TR::Register **flagReg, TR::CodeGenerator *cg) BOOL_BODY;
   };

#undef NULL_BODY
#undef VOID_BODY

#endif
