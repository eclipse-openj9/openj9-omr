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

/*
 * This file will be included within an enum.  Only comments and enumerator
 * definitions are permitted.
 */

   IsNotExtended,
   IsImm,
      IsSrc1,
   IsImm2,
   IsDep,
      IsDepImm,
         IsDepImmSym,
   IsLabel,
      IsAlignedLabel,
      IsDepLabel,
         IsVirtualGuardNOP,
      IsConditionalBranch,
         IsDepConditionalBranch,
   IsAdmin,
   IsTrg1,
      IsTrg1Imm,
      IsTrg1Src1,
         IsTrg1Src1Imm,
            IsTrg1Src1Imm2,
         IsTrg1Src2,
            IsTrg1Src2Imm,
            IsTrg1Src3,
      IsTrg1Mem,
   IsSrc2,
   IsMem,
      IsMemSrc1,
   IsControlFlow,
