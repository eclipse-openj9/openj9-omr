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

#ifdef DEBUG

#include "codegen/CodeGenerator.hpp"  // for CodeGenerator
#include "compile/Compilation.hpp"    // for Compilation
#include "ras/Debug.hpp"              // for TR_DebugBase
#include "x/codegen/X86Ops.hpp"       // for TR_X86OpCode

const char *
TR_X86OpCode::getOpCodeName(TR::CodeGenerator *cg)
   {
   return cg->comp()->getDebug()->getOpCodeName(this);
   }

const char *
TR_X86OpCode::getMnemonicName(TR::CodeGenerator *cg)
   {
   return cg->comp()->getDebug()->getMnemonicName(this);
   }

#endif // ifdef DEBUG
