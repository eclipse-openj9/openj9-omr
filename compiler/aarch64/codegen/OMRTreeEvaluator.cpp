/*******************************************************************************
 * Copyright (c) 2018, 2022 IBM Corp. and others
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

#include <utility>
#include "codegen/ARM64HelperCallSnippet.hpp"
#include "codegen/ARM64Instruction.hpp"
#include "codegen/ARM64ShiftCode.hpp"
#include "codegen/CodeGenerator.hpp"
#include "codegen/CodeGeneratorUtils.hpp"
#include "codegen/CodeGenerator_inlines.hpp"
#include "codegen/ConstantDataSnippet.hpp"
#include "codegen/GenerateInstructions.hpp"
#include "codegen/Linkage.hpp"
#include "codegen/Linkage_inlines.hpp"
#include "codegen/MemoryReference.hpp"
#include "codegen/RegisterDependency.hpp"
#include "codegen/Relocation.hpp"
#include "codegen/TreeEvaluator.hpp"
#include "il/AutomaticSymbol.hpp"
#include "il/LabelSymbol.hpp"
#include "il/Node.hpp"
#include "il/Node_inlines.hpp"
#include "il/ParameterSymbol.hpp"
#include "il/StaticSymbol.hpp"

TR::Register*
OMR::ARM64::TreeEvaluator::BadILOpEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   return TR::TreeEvaluator::unImpOpEvaluator(node, cg);
   }

TR::Register*
OMR::ARM64::TreeEvaluator::iloadiEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   return TR::TreeEvaluator::iloadEvaluator(node, cg);
   }

TR::Register*
OMR::ARM64::TreeEvaluator::floadiEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   return TR::TreeEvaluator::floadEvaluator(node, cg);
   }

TR::Register*
OMR::ARM64::TreeEvaluator::dloadiEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   return TR::TreeEvaluator::dloadEvaluator(node, cg);
   }

TR::Register*
OMR::ARM64::TreeEvaluator::aloadiEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   return TR::TreeEvaluator::aloadEvaluator(node, cg);
   }

TR::Register*
OMR::ARM64::TreeEvaluator::bloadiEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   return TR::TreeEvaluator::bloadEvaluator(node, cg);
   }

TR::Register*
OMR::ARM64::TreeEvaluator::sloadiEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   return TR::TreeEvaluator::sloadEvaluator(node, cg);
   }

TR::Register*
OMR::ARM64::TreeEvaluator::lloadiEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   return TR::TreeEvaluator::lloadEvaluator(node, cg);
   }

TR::Register*
OMR::ARM64::TreeEvaluator::iwrtbarEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   return TR::TreeEvaluator::unImpOpEvaluator(node, cg);
   }

TR::Register*
OMR::ARM64::TreeEvaluator::lwrtbarEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   return TR::TreeEvaluator::unImpOpEvaluator(node, cg);
   }

TR::Register*
OMR::ARM64::TreeEvaluator::fwrtbarEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   return TR::TreeEvaluator::unImpOpEvaluator(node, cg);
   }

TR::Register*
OMR::ARM64::TreeEvaluator::dwrtbarEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   return TR::TreeEvaluator::unImpOpEvaluator(node, cg);
   }

TR::Register*
OMR::ARM64::TreeEvaluator::bwrtbarEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   return TR::TreeEvaluator::unImpOpEvaluator(node, cg);
   }

TR::Register*
OMR::ARM64::TreeEvaluator::swrtbarEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   return TR::TreeEvaluator::unImpOpEvaluator(node, cg);
   }

TR::Register*
OMR::ARM64::TreeEvaluator::lstoreiEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   return TR::TreeEvaluator::lstoreEvaluator(node, cg);
   }

TR::Register*
OMR::ARM64::TreeEvaluator::fstoreiEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   return TR::TreeEvaluator::fstoreEvaluator(node, cg);
   }

TR::Register*
OMR::ARM64::TreeEvaluator::dstoreiEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   return TR::TreeEvaluator::dstoreEvaluator(node, cg);
   }

TR::Register*
OMR::ARM64::TreeEvaluator::astoreiEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   return TR::TreeEvaluator::astoreEvaluator(node, cg);
   }

TR::Register*
OMR::ARM64::TreeEvaluator::bstoreiEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   return TR::TreeEvaluator::bstoreEvaluator(node, cg);
   }

TR::Register*
OMR::ARM64::TreeEvaluator::sstoreiEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   return TR::TreeEvaluator::sstoreEvaluator(node, cg);
   }

TR::Register*
OMR::ARM64::TreeEvaluator::istoreiEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   return TR::TreeEvaluator::istoreEvaluator(node, cg);
   }

TR::Register*
OMR::ARM64::TreeEvaluator::lwrtbariEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   return TR::TreeEvaluator::unImpOpEvaluator(node, cg);
   }

TR::Register*
OMR::ARM64::TreeEvaluator::fwrtbariEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   return TR::TreeEvaluator::unImpOpEvaluator(node, cg);
   }

TR::Register*
OMR::ARM64::TreeEvaluator::dwrtbariEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   return TR::TreeEvaluator::unImpOpEvaluator(node, cg);
   }

TR::Register*
OMR::ARM64::TreeEvaluator::awrtbariEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   return TR::TreeEvaluator::unImpOpEvaluator(node, cg);
   }

TR::Register*
OMR::ARM64::TreeEvaluator::bwrtbariEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   return TR::TreeEvaluator::unImpOpEvaluator(node, cg);
   }

TR::Register*
OMR::ARM64::TreeEvaluator::swrtbariEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   return TR::TreeEvaluator::unImpOpEvaluator(node, cg);
   }

TR::Register*
OMR::ARM64::TreeEvaluator::iwrtbariEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   return TR::TreeEvaluator::unImpOpEvaluator(node, cg);
   }

TR::Register*
OMR::ARM64::TreeEvaluator::GotoEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   return TR::TreeEvaluator::gotoEvaluator(node, cg);
   }

TR::Register*
OMR::ARM64::TreeEvaluator::ReturnEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   return TR::TreeEvaluator::returnEvaluator(node, cg);
   }

TR::Register*
OMR::ARM64::TreeEvaluator::athrowEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   return TR::TreeEvaluator::unImpOpEvaluator(node, cg);
   }

TR::Register*
OMR::ARM64::TreeEvaluator::icallEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   return TR::TreeEvaluator::directCallEvaluator(node, cg);
   }

TR::Register*
OMR::ARM64::TreeEvaluator::lcallEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   return TR::TreeEvaluator::directCallEvaluator(node, cg);
   }

TR::Register*
OMR::ARM64::TreeEvaluator::fcallEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   return TR::TreeEvaluator::directCallEvaluator(node, cg);
   }

TR::Register*
OMR::ARM64::TreeEvaluator::dcallEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   return TR::TreeEvaluator::directCallEvaluator(node, cg);
   }

TR::Register*
OMR::ARM64::TreeEvaluator::acallEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   return TR::TreeEvaluator::directCallEvaluator(node, cg);
   }

TR::Register*
OMR::ARM64::TreeEvaluator::callEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   return TR::TreeEvaluator::directCallEvaluator(node, cg);
   }

TR::Register*
OMR::ARM64::TreeEvaluator::baddEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   return TR::TreeEvaluator::iaddEvaluator(node, cg);
   }

TR::Register*
OMR::ARM64::TreeEvaluator::saddEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   return TR::TreeEvaluator::iaddEvaluator(node, cg);
   }

TR::Register*
OMR::ARM64::TreeEvaluator::bsubEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   return TR::TreeEvaluator::isubEvaluator(node, cg);
   }

TR::Register*
OMR::ARM64::TreeEvaluator::ssubEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   return TR::TreeEvaluator::isubEvaluator(node, cg);
   }

TR::Register*
OMR::ARM64::TreeEvaluator::asubEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   return TR::TreeEvaluator::unImpOpEvaluator(node, cg);
   }

TR::Register*
OMR::ARM64::TreeEvaluator::bmulEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   return TR::TreeEvaluator::imulEvaluator(node, cg);
   }

TR::Register*
OMR::ARM64::TreeEvaluator::smulEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   return TR::TreeEvaluator::imulEvaluator(node, cg);
   }

TR::Register*
OMR::ARM64::TreeEvaluator::bremEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   return TR::TreeEvaluator::unImpOpEvaluator(node, cg);
   }

TR::Register*
OMR::ARM64::TreeEvaluator::sremEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   return TR::TreeEvaluator::unImpOpEvaluator(node, cg);
   }

TR::Register*
OMR::ARM64::TreeEvaluator::bnegEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   return TR::TreeEvaluator::inegEvaluator(node, cg);
   }

TR::Register*
OMR::ARM64::TreeEvaluator::snegEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   return TR::TreeEvaluator::inegEvaluator(node, cg);
   }

TR::Register*
OMR::ARM64::TreeEvaluator::lshlEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   return TR::TreeEvaluator::ishlEvaluator(node, cg);
   }

TR::Register*
OMR::ARM64::TreeEvaluator::bshlEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   return TR::TreeEvaluator::ishlEvaluator(node, cg);
   }

TR::Register*
OMR::ARM64::TreeEvaluator::sshlEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   return TR::TreeEvaluator::ishlEvaluator(node, cg);
   }

TR::Register*
OMR::ARM64::TreeEvaluator::lshrEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   return TR::TreeEvaluator::ishrEvaluator(node, cg);
   }

TR::Register*
OMR::ARM64::TreeEvaluator::bshrEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   return TR::TreeEvaluator::ishrEvaluator(node, cg);
   }

TR::Register*
OMR::ARM64::TreeEvaluator::sshrEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   return TR::TreeEvaluator::ishrEvaluator(node, cg);
   }

TR::Register*
OMR::ARM64::TreeEvaluator::lushrEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   return TR::TreeEvaluator::iushrEvaluator(node, cg);
   }

TR::Register*
OMR::ARM64::TreeEvaluator::bushrEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   return TR::TreeEvaluator::iushrEvaluator(node, cg);
   }

TR::Register*
OMR::ARM64::TreeEvaluator::sushrEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   return TR::TreeEvaluator::iushrEvaluator(node, cg);
   }

TR::Register*
OMR::ARM64::TreeEvaluator::lrolEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   return TR::TreeEvaluator::irolEvaluator(node, cg);
   }

TR::Register*
OMR::ARM64::TreeEvaluator::bandEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   return TR::TreeEvaluator::iandEvaluator(node, cg);
   }

TR::Register*
OMR::ARM64::TreeEvaluator::sandEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   return TR::TreeEvaluator::iandEvaluator(node, cg);
   }

TR::Register*
OMR::ARM64::TreeEvaluator::borEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   return TR::TreeEvaluator::iorEvaluator(node, cg);
   }

TR::Register*
OMR::ARM64::TreeEvaluator::sorEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   return TR::TreeEvaluator::iorEvaluator(node, cg);
   }

TR::Register*
OMR::ARM64::TreeEvaluator::bxorEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   return TR::TreeEvaluator::ixorEvaluator(node, cg);
   }

TR::Register*
OMR::ARM64::TreeEvaluator::sxorEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   return TR::TreeEvaluator::ixorEvaluator(node, cg);
   }

TR::Register*
OMR::ARM64::TreeEvaluator::i2bEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   return TR::TreeEvaluator::l2iEvaluator(node, cg);
   }

TR::Register*
OMR::ARM64::TreeEvaluator::i2sEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   return TR::TreeEvaluator::l2iEvaluator(node, cg);
   }

TR::Register*
OMR::ARM64::TreeEvaluator::i2aEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   return TR::TreeEvaluator::unImpOpEvaluator(node, cg);
   }

TR::Register*
OMR::ARM64::TreeEvaluator::iu2fEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   return TR::TreeEvaluator::unImpOpEvaluator(node, cg);
   }

TR::Register*
OMR::ARM64::TreeEvaluator::iu2dEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   return TR::TreeEvaluator::unImpOpEvaluator(node, cg);
   }

TR::Register*
OMR::ARM64::TreeEvaluator::iu2aEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   return TR::TreeEvaluator::unImpOpEvaluator(node, cg);
   }

TR::Register*
OMR::ARM64::TreeEvaluator::l2bEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   return TR::TreeEvaluator::l2iEvaluator(node, cg);
   }

TR::Register*
OMR::ARM64::TreeEvaluator::l2sEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   return TR::TreeEvaluator::l2iEvaluator(node, cg);
   }

TR::Register*
OMR::ARM64::TreeEvaluator::lu2fEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   return TR::TreeEvaluator::unImpOpEvaluator(node, cg);
   }

TR::Register*
OMR::ARM64::TreeEvaluator::lu2dEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   return TR::TreeEvaluator::unImpOpEvaluator(node, cg);
   }

TR::Register*
OMR::ARM64::TreeEvaluator::lu2aEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   return TR::TreeEvaluator::passThroughEvaluator(node, cg);
   }

TR::Register*
OMR::ARM64::TreeEvaluator::f2bEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   return TR::TreeEvaluator::unImpOpEvaluator(node, cg);
   }

TR::Register*
OMR::ARM64::TreeEvaluator::f2sEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   return TR::TreeEvaluator::unImpOpEvaluator(node, cg);
   }

TR::Register*
OMR::ARM64::TreeEvaluator::b2fEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   return TR::TreeEvaluator::unImpOpEvaluator(node, cg);
   }

TR::Register*
OMR::ARM64::TreeEvaluator::b2sEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   return TR::TreeEvaluator::unImpOpEvaluator(node, cg);
   }

TR::Register*
OMR::ARM64::TreeEvaluator::b2aEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   return TR::TreeEvaluator::unImpOpEvaluator(node, cg);
   }

TR::Register*
OMR::ARM64::TreeEvaluator::bu2fEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   return TR::TreeEvaluator::unImpOpEvaluator(node, cg);
   }

TR::Register*
OMR::ARM64::TreeEvaluator::bu2dEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   return TR::TreeEvaluator::unImpOpEvaluator(node, cg);
   }

TR::Register*
OMR::ARM64::TreeEvaluator::bu2sEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   return TR::TreeEvaluator::unImpOpEvaluator(node, cg);
   }

TR::Register*
OMR::ARM64::TreeEvaluator::bu2aEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   return TR::TreeEvaluator::unImpOpEvaluator(node, cg);
   }

TR::Register*
OMR::ARM64::TreeEvaluator::s2fEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   return TR::TreeEvaluator::unImpOpEvaluator(node, cg);
   }

TR::Register*
OMR::ARM64::TreeEvaluator::s2bEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   return TR::TreeEvaluator::l2iEvaluator(node, cg);
   }

TR::Register*
OMR::ARM64::TreeEvaluator::s2aEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   return TR::TreeEvaluator::unImpOpEvaluator(node, cg);
   }

TR::Register*
OMR::ARM64::TreeEvaluator::su2fEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   return TR::TreeEvaluator::unImpOpEvaluator(node, cg);
   }

TR::Register*
OMR::ARM64::TreeEvaluator::su2aEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   return TR::TreeEvaluator::unImpOpEvaluator(node, cg);
   }

TR::Register*
OMR::ARM64::TreeEvaluator::a2iEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   return TR::TreeEvaluator::l2iEvaluator(node, cg);
   }

TR::Register*
OMR::ARM64::TreeEvaluator::a2lEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   return TR::TreeEvaluator::passThroughEvaluator(node, cg);
   }

TR::Register*
OMR::ARM64::TreeEvaluator::a2bEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   return TR::TreeEvaluator::unImpOpEvaluator(node, cg);
   }

TR::Register*
OMR::ARM64::TreeEvaluator::a2sEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   return TR::TreeEvaluator::unImpOpEvaluator(node, cg);
   }

TR::Register*
OMR::ARM64::TreeEvaluator::acmpeqEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   return TR::TreeEvaluator::lcmpeqEvaluator(node, cg);
   }

TR::Register*
OMR::ARM64::TreeEvaluator::acmpneEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   return TR::TreeEvaluator::lcmpneEvaluator(node, cg);
   }

TR::Register*
OMR::ARM64::TreeEvaluator::acmpltEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   return TR::TreeEvaluator::lucmpltEvaluator(node, cg);
   }

TR::Register*
OMR::ARM64::TreeEvaluator::acmpgeEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   return TR::TreeEvaluator::lucmpgeEvaluator(node, cg);
   }

TR::Register*
OMR::ARM64::TreeEvaluator::acmpgtEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   return TR::TreeEvaluator::lucmpgtEvaluator(node, cg);
   }

TR::Register*
OMR::ARM64::TreeEvaluator::acmpleEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   return TR::TreeEvaluator::lucmpleEvaluator(node, cg);
   }

TR::Register*
OMR::ARM64::TreeEvaluator::bcmpeqEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   return TR::TreeEvaluator::icmpeqEvaluator(node, cg);
   }

TR::Register*
OMR::ARM64::TreeEvaluator::bcmpneEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   return TR::TreeEvaluator::icmpneEvaluator(node, cg);
   }

TR::Register*
OMR::ARM64::TreeEvaluator::bcmpltEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   return TR::TreeEvaluator::icmpltEvaluator(node, cg);
   }

TR::Register*
OMR::ARM64::TreeEvaluator::bcmpgeEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   return TR::TreeEvaluator::icmpgeEvaluator(node, cg);
   }

TR::Register*
OMR::ARM64::TreeEvaluator::bcmpgtEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   return TR::TreeEvaluator::icmpgtEvaluator(node, cg);
   }

TR::Register*
OMR::ARM64::TreeEvaluator::bcmpleEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   return TR::TreeEvaluator::icmpleEvaluator(node, cg);
   }

TR::Register*
OMR::ARM64::TreeEvaluator::bucmpltEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   return TR::TreeEvaluator::iucmpltEvaluator(node, cg);
   }

TR::Register*
OMR::ARM64::TreeEvaluator::bucmpgeEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   return TR::TreeEvaluator::iucmpgeEvaluator(node, cg);
   }

TR::Register*
OMR::ARM64::TreeEvaluator::bucmpgtEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   return TR::TreeEvaluator::iucmpgtEvaluator(node, cg);
   }

TR::Register*
OMR::ARM64::TreeEvaluator::bucmpleEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   return TR::TreeEvaluator::iucmpleEvaluator(node, cg);
   }

TR::Register*
OMR::ARM64::TreeEvaluator::scmpeqEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   return TR::TreeEvaluator::icmpeqEvaluator(node, cg);
   }

TR::Register*
OMR::ARM64::TreeEvaluator::scmpneEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   return TR::TreeEvaluator::icmpneEvaluator(node, cg);
   }

TR::Register*
OMR::ARM64::TreeEvaluator::scmpltEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   return TR::TreeEvaluator::icmpltEvaluator(node, cg);
   }

TR::Register*
OMR::ARM64::TreeEvaluator::scmpgeEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   return TR::TreeEvaluator::icmpgeEvaluator(node, cg);
   }

TR::Register*
OMR::ARM64::TreeEvaluator::scmpgtEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   return TR::TreeEvaluator::icmpgtEvaluator(node, cg);
   }

TR::Register*
OMR::ARM64::TreeEvaluator::scmpleEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   return TR::TreeEvaluator::icmpleEvaluator(node, cg);
   }

TR::Register*
OMR::ARM64::TreeEvaluator::sucmpltEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   return TR::TreeEvaluator::iucmpltEvaluator(node, cg);
   }

TR::Register*
OMR::ARM64::TreeEvaluator::sucmpgeEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   return TR::TreeEvaluator::iucmpgeEvaluator(node, cg);
   }

TR::Register*
OMR::ARM64::TreeEvaluator::sucmpgtEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   return TR::TreeEvaluator::iucmpgtEvaluator(node, cg);
   }

TR::Register*
OMR::ARM64::TreeEvaluator::sucmpleEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   return TR::TreeEvaluator::iucmpleEvaluator(node, cg);
   }

TR::Register*
OMR::ARM64::TreeEvaluator::ifacmpeqEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   return TR::TreeEvaluator::iflcmpeqEvaluator(node, cg);
   }

TR::Register*
OMR::ARM64::TreeEvaluator::ifacmpneEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   return TR::TreeEvaluator::iflcmpneEvaluator(node, cg);
   }

TR::Register*
OMR::ARM64::TreeEvaluator::ifacmpltEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   return TR::TreeEvaluator::iflucmpltEvaluator(node, cg);
   }

TR::Register*
OMR::ARM64::TreeEvaluator::ifacmpgeEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   return TR::TreeEvaluator::iflucmpgeEvaluator(node, cg);
   }

TR::Register*
OMR::ARM64::TreeEvaluator::ifacmpgtEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   return TR::TreeEvaluator::iflucmpgtEvaluator(node, cg);
   }

TR::Register*
OMR::ARM64::TreeEvaluator::ifacmpleEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   return TR::TreeEvaluator::iflucmpleEvaluator(node, cg);
   }

TR::Register*
OMR::ARM64::TreeEvaluator::ifbcmpeqEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   return TR::TreeEvaluator::unImpOpEvaluator(node, cg);
   }

TR::Register*
OMR::ARM64::TreeEvaluator::ifbcmpneEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   return TR::TreeEvaluator::unImpOpEvaluator(node, cg);
   }

TR::Register*
OMR::ARM64::TreeEvaluator::ifbcmpltEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   return TR::TreeEvaluator::unImpOpEvaluator(node, cg);
   }

TR::Register*
OMR::ARM64::TreeEvaluator::ifbcmpgeEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   return TR::TreeEvaluator::unImpOpEvaluator(node, cg);
   }

TR::Register*
OMR::ARM64::TreeEvaluator::ifbcmpgtEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   return TR::TreeEvaluator::unImpOpEvaluator(node, cg);
   }

TR::Register*
OMR::ARM64::TreeEvaluator::ifbcmpleEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   return TR::TreeEvaluator::unImpOpEvaluator(node, cg);
   }

TR::Register*
OMR::ARM64::TreeEvaluator::ifbucmpltEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   return TR::TreeEvaluator::unImpOpEvaluator(node, cg);
   }

TR::Register*
OMR::ARM64::TreeEvaluator::ifbucmpgeEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   return TR::TreeEvaluator::unImpOpEvaluator(node, cg);
   }

TR::Register*
OMR::ARM64::TreeEvaluator::ifbucmpgtEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   return TR::TreeEvaluator::unImpOpEvaluator(node, cg);
   }

TR::Register*
OMR::ARM64::TreeEvaluator::ifbucmpleEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   return TR::TreeEvaluator::unImpOpEvaluator(node, cg);
   }

TR::Register*
OMR::ARM64::TreeEvaluator::ifscmpeqEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   return TR::TreeEvaluator::unImpOpEvaluator(node, cg);
   }

TR::Register*
OMR::ARM64::TreeEvaluator::ifscmpneEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   return TR::TreeEvaluator::unImpOpEvaluator(node, cg);
   }

TR::Register*
OMR::ARM64::TreeEvaluator::ifscmpltEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   return TR::TreeEvaluator::unImpOpEvaluator(node, cg);
   }

TR::Register*
OMR::ARM64::TreeEvaluator::ifscmpgeEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   return TR::TreeEvaluator::unImpOpEvaluator(node, cg);
   }

TR::Register*
OMR::ARM64::TreeEvaluator::ifscmpgtEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   return TR::TreeEvaluator::unImpOpEvaluator(node, cg);
   }

TR::Register*
OMR::ARM64::TreeEvaluator::ifscmpleEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   return TR::TreeEvaluator::unImpOpEvaluator(node, cg);
   }

TR::Register*
OMR::ARM64::TreeEvaluator::ifsucmpltEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   return TR::TreeEvaluator::unImpOpEvaluator(node, cg);
   }

TR::Register*
OMR::ARM64::TreeEvaluator::ifsucmpgeEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   return TR::TreeEvaluator::unImpOpEvaluator(node, cg);
   }

TR::Register*
OMR::ARM64::TreeEvaluator::ifsucmpgtEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   return TR::TreeEvaluator::unImpOpEvaluator(node, cg);
   }

TR::Register*
OMR::ARM64::TreeEvaluator::ifsucmpleEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   return TR::TreeEvaluator::unImpOpEvaluator(node, cg);
   }

TR::Register*
OMR::ARM64::TreeEvaluator::ZEROCHKEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   return TR::TreeEvaluator::unImpOpEvaluator(node, cg);
   }

TR::Register*
OMR::ARM64::TreeEvaluator::lRegLoadEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   return TR::TreeEvaluator::iRegLoadEvaluator(node, cg);
   }

TR::Register*
OMR::ARM64::TreeEvaluator::dRegLoadEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   return TR::TreeEvaluator::fRegLoadEvaluator(node, cg);
   }

TR::Register*
OMR::ARM64::TreeEvaluator::sRegLoadEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   return TR::TreeEvaluator::iRegLoadEvaluator(node, cg);
   }

TR::Register*
OMR::ARM64::TreeEvaluator::bRegLoadEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   return TR::TreeEvaluator::iRegLoadEvaluator(node, cg);
   }

TR::Register*
OMR::ARM64::TreeEvaluator::aRegStoreEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   return TR::TreeEvaluator::iRegStoreEvaluator(node, cg);
   }

TR::Register*
OMR::ARM64::TreeEvaluator::lRegStoreEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   return TR::TreeEvaluator::iRegStoreEvaluator(node, cg);
   }

TR::Register*
OMR::ARM64::TreeEvaluator::fRegStoreEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   return TR::TreeEvaluator::iRegStoreEvaluator(node, cg);
   }

TR::Register*
OMR::ARM64::TreeEvaluator::dRegStoreEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   return TR::TreeEvaluator::iRegStoreEvaluator(node, cg);
   }

TR::Register*
OMR::ARM64::TreeEvaluator::sRegStoreEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   return TR::TreeEvaluator::iRegStoreEvaluator(node, cg);
   }

TR::Register*
OMR::ARM64::TreeEvaluator::bRegStoreEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   return TR::TreeEvaluator::iRegStoreEvaluator(node, cg);
   }

TR::Register*
OMR::ARM64::TreeEvaluator::lselectEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   return TR::TreeEvaluator::iselectEvaluator(node, cg);
   }

TR::Register*
OMR::ARM64::TreeEvaluator::bselectEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   return TR::TreeEvaluator::iselectEvaluator(node, cg);
   }

TR::Register*
OMR::ARM64::TreeEvaluator::sselectEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   return TR::TreeEvaluator::iselectEvaluator(node, cg);
   }

TR::Register*
OMR::ARM64::TreeEvaluator::aselectEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   return TR::TreeEvaluator::iselectEvaluator(node, cg);
   }

TR::Register*
OMR::ARM64::TreeEvaluator::dselectEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   return TR::TreeEvaluator::fselectEvaluator(node, cg);
   }

TR::Register*
OMR::ARM64::TreeEvaluator::MethodEnterHookEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   return TR::TreeEvaluator::unImpOpEvaluator(node, cg);
   }

TR::Register*
OMR::ARM64::TreeEvaluator::MethodExitHookEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   return TR::TreeEvaluator::unImpOpEvaluator(node, cg);
   }

TR::Register*
OMR::ARM64::TreeEvaluator::PassThroughEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   return TR::TreeEvaluator::passThroughEvaluator(node, cg);
   }


/**
 * @brief Tries to generate a SIMD 16bit shifted immediate instruction
 *
 * @param[in] node: node
 * @param[in] cg: CodeGenerator
 * @param[in] treg: target register
 * @param[in] op: opcode
 * @param[in] value: immediate value
 *
 * @return instruction cursor if an instruction is successfully generated and otherwise returns NULL
 */
static
TR::Instruction *tryToGenerateImm16ShiftedInstrucion(TR::Node *node, TR::CodeGenerator *cg, TR::Register *treg, TR::InstOpCode::Mnemonic op, uint16_t value)
   {
   if ((value & 0xff00) == 0)
      {
      return generateTrg1ImmInstruction(cg, op, node, treg, value & 0xff);
      }
   else if ((value & 0xff) == 0)
      {
      return generateTrg1ImmShiftedInstruction(cg, op, node, treg, (value >> 8) & 0xff, 8);
      }
   return NULL;
   }

/**
 * @brief Tries to generate a SIMD 32bit shifted immediate instruction
 *
 * @param[in] node: node
 * @param[in] cg: CodeGenerator
 * @param[in] treg: target register
 * @param[in] op: opcode
 * @param[in] value: immediate value
 *
 * @return instruction cursor if an instruction is successfully generated and otherwise returns NULL
 */
static
TR::Instruction *tryToGenerateImm32ShiftedInstruction(TR::Node *node, TR::CodeGenerator *cg, TR::Register *treg, TR::InstOpCode::Mnemonic op, uint32_t value)
   {
   if ((value & 0xffffff00) == 0)
      {
      return generateTrg1ImmInstruction(cg, op, node, treg, value & 0xff);
      }
   else if ((value & 0xffff00ff) == 0)
      {
      return generateTrg1ImmShiftedInstruction(cg, op, node, treg, (value >> 8) & 0xff, 8);
      }
   else if ((value & 0xff00ffff) == 0)
      {
      return generateTrg1ImmShiftedInstruction(cg, op, node, treg, (value >> 16) & 0xff, 16);
      }
   else if ((value & 0xffffff) == 0)
      {
      return generateTrg1ImmShiftedInstruction(cg, op, node, treg, (value >> 24) & 0xff, 24);
      }
   return NULL;
   }

/**
 * @brief Tries to generate a SIMD 32bit shifting ones instruction
 *
 * @param[in] node: node
 * @param[in] cg: CodeGenerator
 * @param[in] treg: target register
 * @param[in] op: opcode
 * @param[in] value: immediate value
 *
 * @return instruction cursor if an instruction is successfully generated and otherwise returns NULL
 */
static
TR::Instruction *tryToGenerateImm32ShiftingOnesInstruction(TR::Node *node, TR::CodeGenerator *cg, TR::Register *treg, TR::InstOpCode::Mnemonic op, uint32_t value)
   {
   if ((value & 0xffff00ff) == 0xff)
      {
      return generateTrg1ImmShiftedInstruction(cg, op, node, treg, (value >> 8) & 0xff, 8);
      }
   else if ((value & 0xff00ffff) == 0xffff)
      {
      return generateTrg1ImmShiftedInstruction(cg, op, node, treg, (value >> 16) & 0xff, 16);
      }
   return NULL;
   }

/**
 * @brief Tries to generate a SIMD 64bit immediate instruction
 *
 * @param[in] node: node
 * @param[in] cg: CodeGenerator
 * @param[in] treg: target register
 * @param[in] op: opcode
 * @param[in] value: immediate value
 *
 * @return instruction cursor if an instruction is successfully generated and otherwise returns NULL
 */
static
TR::Instruction *tryToGenerateImm64Instruction(TR::Node *node, TR::CodeGenerator *cg, TR::Register *treg, TR::InstOpCode::Mnemonic op, uint64_t value)
   {
   /* Special encoding for the 64-bit variant of MOVI */
   uint8_t imm = 0;
   int32_t i;
   for (i = 0; i < 8; i++)
      {
      uint8_t v = (value >> (i * 8)) & 0xff;
      if (v == 0xff)
         {
         imm |= (1 << i);
         }
      else if (v != 0)
         {
         return NULL;
         }
      }
   return generateTrg1ImmInstruction(cg, op, node, treg, imm);
   }

/**
 * @brief Tries to generate a SIMD move immediate instruction for 16bit value.
 *
 * @param[in] node: node
 * @param[in] cg: CodeGenerator
 * @param[in] treg: target register
 * @param[in] value: immediate value
 *
 * @return instruction cursor if an instruction is successfully generated and otherwise returns NULL
 */
static
TR::Instruction *tryToGenerateMovImm16ShiftedInstruction(TR::Node *node, TR::CodeGenerator *cg, TR::Register *treg, uint16_t value)
   {
   uint8_t lower8bit = value & 0xff;
   uint8_t upper8bit = (value >> 8) & 0xff;
   if (lower8bit == upper8bit)
      {
      return generateTrg1ImmInstruction(cg, TR::InstOpCode::vmovi16b, node, treg, lower8bit);
      }

   TR::Instruction *instr;
   if ((instr = tryToGenerateImm16ShiftedInstrucion(node, cg, treg, TR::InstOpCode::vmovi8h, value)))
      {
      return instr;
      }
   else if ((instr = tryToGenerateImm16ShiftedInstrucion(node, cg, treg, TR::InstOpCode::vmvni8h, ~value)))
      {
      return instr;
      }
   return NULL;
   }

/**
 * @brief Tries to generate a SIMD move immediate instruction for 32bit value.
 *
 * @param[in] node: node
 * @param[in] cg: CodeGenerator
 * @param[in] treg: target register
 * @param[in] value: immediate value
 *
 * @return instruction cursor if an instruction is successfully generated and otherwise returns NULL
 */
static
TR::Instruction *tryToGenerateMovImm32ShiftedInstruction(TR::Node *node, TR::CodeGenerator *cg, TR::Register *treg, uint32_t value)
   {
   uint16_t lower16bit = value & 0xffff;
   uint16_t upper16bit = (value >> 16) & 0xffff;
   TR::Instruction *instr;

   if (lower16bit == upper16bit)
      {
      if ((instr = tryToGenerateMovImm16ShiftedInstruction(node, cg, treg, lower16bit)))
         {
         return instr;
         }
      }
   else if ((instr = tryToGenerateImm32ShiftedInstruction(node, cg, treg, TR::InstOpCode::vmovi4s, value)))
      {
      return instr;
      }
   else if ((instr = tryToGenerateImm32ShiftedInstruction(node, cg, treg, TR::InstOpCode::vmvni4s, ~value)))
      {
      return instr;
      }
   else if ((instr = tryToGenerateImm32ShiftingOnesInstruction(node, cg, treg, TR::InstOpCode::vmovi4s_one, value)))
      {
      return instr;
      }
   else if ((instr = tryToGenerateImm32ShiftingOnesInstruction(node, cg, treg, TR::InstOpCode::vmvni4s_one, ~value)))
      {
      return instr;
      }
   return NULL;
   }

// mask evaluators
TR::Register*
OMR::ARM64::TreeEvaluator::mAnyTrueEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   return TR::TreeEvaluator::unImpOpEvaluator(node, cg);
   }

TR::Register*
OMR::ARM64::TreeEvaluator::mAllTrueEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   return TR::TreeEvaluator::unImpOpEvaluator(node, cg);
   }

TR::Register*
OMR::ARM64::TreeEvaluator::mmAnyTrueEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   return TR::TreeEvaluator::unImpOpEvaluator(node, cg);
   }

TR::Register*
OMR::ARM64::TreeEvaluator::mmAllTrueEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   return TR::TreeEvaluator::unImpOpEvaluator(node, cg);
   }

TR::Register*
OMR::ARM64::TreeEvaluator::mloadEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   return TR::TreeEvaluator::vloadEvaluator(node, cg);
   }

TR::Register*
OMR::ARM64::TreeEvaluator::mloadiEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   return TR::TreeEvaluator::mloadEvaluator(node, cg);
   }

TR::Register*
OMR::ARM64::TreeEvaluator::mstoreEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   return TR::TreeEvaluator::vstoreEvaluator(node, cg);
   }

TR::Register*
OMR::ARM64::TreeEvaluator::mstoreiEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   return TR::TreeEvaluator::mstoreEvaluator(node, cg);
   }

TR::Register*
OMR::ARM64::TreeEvaluator::mRegLoadEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   TR::Register *globalReg = node->getRegister();

   if (globalReg == NULL)
      {
      globalReg = cg->allocateRegister(TR_VRF);
      node->setRegister(globalReg);
      }
   return globalReg;
   }

TR::Register*
OMR::ARM64::TreeEvaluator::mRegStoreEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   return TR::TreeEvaluator::iRegStoreEvaluator(node, cg);
   }

/* This template function should be declared as constexpr if the compiler supports it. */
template<TR::VectorOperation op>
TR::ILOpCodes getLoadOpCodeForMaskConversion()
   {
   static_assert((op == TR::s2m) || (op == TR::i2m) || (op == TR::l2m) || (op == TR::v2m), "Expects s2m, i2m, l2m or v2m as opcode");
   switch (op)
      {
      case TR::s2m:
         return TR::sloadi;
      case TR::i2m:
         return TR::iloadi;
      case TR::l2m:
         return TR::lloadi;
      case TR::v2m:
      default:
         return TR::lloadi; /* does not matter. Unused for v2m */
      }
   }

/* This template function should be declared as constexpr if the compiler supports it. */
template<TR::VectorOperation op>
TR::InstOpCode::Mnemonic getLoadInstOpCodeForMaskConversion()
   {
   static_assert((op == TR::s2m) || (op == TR::i2m) || (op == TR::l2m) || (op == TR::v2m), "Expects s2m, i2m, l2m or v2m as opcode");
   switch (op)
      {
      case TR::s2m:
         return TR::InstOpCode::vldrimmh;
      case TR::i2m:
         return TR::InstOpCode::vldrimms;
      case TR::l2m:
         return TR::InstOpCode::vldrimmd;
      case TR::v2m:
      default:
         return TR::InstOpCode::vldrimmq;
      }
   }

/* This template function should be declared as constexpr if the compiler supports it. */
template<TR::VectorOperation op>
TR::InstOpCode::Mnemonic getCopyToGPRInstOpCodeForMaskConversion()
   {
   static_assert((op == TR::s2m) || (op == TR::i2m) || (op == TR::l2m) || (op == TR::v2m), "Expects s2m, i2m, l2m or v2m as opcode");
   switch (op)
      {
      case TR::s2m:
         return TR::InstOpCode::vinswh;
      case TR::i2m:
         return TR::InstOpCode::vinsws;
      case TR::l2m:
         return TR::InstOpCode::vinsxd;
      case TR::v2m:
      default:
         return TR::InstOpCode::bad;
      }
   }

/**
 * @brief Helper template function to generate instructions for converting boolean array to mask
 *
 * @details This helper template function takes TR::VectorOperation as a template parameter.
 *          op must be either of s2m, i2m, l2m or v2m. If the first child node is a load node and
 *          the first child node is not referenced by other nodes, we directly load it into the vector register.
 *          Otherwise, we evaluate it as usual and if it is loaded into a general purpose register,
 *          we move it from the general purpose register to the vector register.
 *          Then, we generate the appropriate instruction sequence for converting boolean array into the mask.
 *          For s2m and v2m, NOT instruction is generated as the last instruction. If omitNot is true,
 *          that NOT instruction is not generated.
 *
 * @param[in] node: node
 * @param[in] omitNot: if true, NOT instruction coming last of the instruction sequence is not generated
 * @param[in] cg: code generator
 *
 * @returns result register
 */
template<TR::VectorOperation op>
TR::Register *toMaskConversionHelper(TR::Node *node, bool omitNot, TR::CodeGenerator *cg)
   {
   TR_ASSERT_FATAL_WITH_NODE(node, node->getDataType().getVectorLength() == TR::VectorLength128,
                   "Only 128-bit vectors are supported %s", node->getDataType().toString());
   static_assert((op == TR::s2m) || (op == TR::i2m) || (op == TR::l2m) || (op == TR::v2m), "Expects s2m, i2m, l2m or v2m as opcode");

   /* These three variables should be declared as constexpr if the compiler supports it. */
   const TR::ILOpCodes loadOp = getLoadOpCodeForMaskConversion<op>();
   const TR::InstOpCode::Mnemonic loadInstOp = getLoadInstOpCodeForMaskConversion<op>();
   const TR::InstOpCode::Mnemonic copyToGPRInstOp = getCopyToGPRInstOpCodeForMaskConversion<op>();

   TR::Node *firstChild = node->getFirstChild();
   TR::Register *firstReg;
   TR::Register *maskReg;
   if (((op == TR::v2m) || (firstChild->getOpCodeValue() == loadOp)) && firstChild->getRegister() == NULL && firstChild->getReferenceCount() == 1)
      {
      firstReg = commonLoadEvaluator(firstChild, loadInstOp, cg->allocateRegister(TR_VRF), cg);
      maskReg = firstReg;
      }
   else
      {
      firstReg = cg->evaluate(firstChild);
      maskReg = cg->allocateRegister(TR_VRF);
      if (op != TR::v2m)
         {
         /* Clear maskReg, then insert the mask value in GPR to the first element of maskReg. */
         generateTrg1ImmInstruction(cg, TR::InstOpCode::vmovi16b, node, maskReg, 0);
         generateMovGPRToVectorElementInstruction(cg, copyToGPRInstOp, node, maskReg, firstReg, 0);
         }
      }

   if (op == TR::s2m)
      {
      /*
      * Use vector extract to move the first and second element to 7th and 8th element position respectively.
      * This makes the first element to be in lower 64bit and the second element to be in upper 64bit.
      * Then, use cmeq to compare each 64bit element with 0.
      */
      generateTrg1Src2ImmInstruction(cg, TR::InstOpCode::vext16b, node, maskReg, maskReg, maskReg, 9);
      generateTrg1Src1Instruction(cg, TR::InstOpCode::vcmeq2d_zero, node, maskReg, maskReg);
      }
   else if (op == TR::i2m)
      {
      /*
      * Use unsigned extend long instruction to convert 8bit elements to 16bit elements,
      * then 16bit elements to 32bit elements.
      * It is guaranteed that the msb of each 32bit element is not set, so we can safely
      * use cmgt_zero to check if the mask is set.
      */
      generateVectorUXTLInstruction(cg, TR::Int8, node, maskReg, maskReg, false);
      generateVectorUXTLInstruction(cg, TR::Int16, node, maskReg, maskReg, false);
      generateTrg1Src1Instruction(cg, TR::InstOpCode::vcmgt4s_zero, node, maskReg, maskReg);
      }
   else if (op == TR::l2m)
      {
      /*
      * Use unsigned extend long instruction to convert 8bit elements to 16bit elements.
      * It is guaranteed that the msb of each 16bit element is not set, so we can safely
      * use cmgt_zero to check if the mask is set.
      */
      generateVectorUXTLInstruction(cg, TR::Int8, node, maskReg, maskReg, false);
      generateTrg1Src1Instruction(cg, TR::InstOpCode::vcmgt8h_zero, node, maskReg, maskReg);
      }
   else /* op == TR::v2m */
      {
      generateTrg1Src1Instruction(cg, TR::InstOpCode::vcmeq16b_zero, node, maskReg, firstReg);
      }

   if ((op == TR::s2m) || (op == TR::v2m))
      {
      if (omitNot)
         {
         TR::Compilation *comp = cg->comp();
         if (comp->getOption(TR_TraceCG))
            {
            traceMsg(comp, "omitting vnot instruction at node %p\n", node);
            }
         }
      else
         {
         generateTrg1Src1Instruction(cg, TR::InstOpCode::vnot16b, node, maskReg, maskReg);
         }
      }

   node->setRegister(maskReg);
   cg->decReferenceCount(firstChild);
   return maskReg;
   }

TR::Register*
OMR::ARM64::TreeEvaluator::b2mEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   return TR::TreeEvaluator::unImpOpEvaluator(node, cg);
   }

TR::Register*
OMR::ARM64::TreeEvaluator::s2mEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   return toMaskConversionHelper<TR::s2m>(node, false, cg);
   }

TR::Register*
OMR::ARM64::TreeEvaluator::i2mEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   return toMaskConversionHelper<TR::i2m>(node, false, cg);
   }

TR::Register*
OMR::ARM64::TreeEvaluator::l2mEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   return toMaskConversionHelper<TR::l2m>(node, false, cg);
   }

TR::Register*
OMR::ARM64::TreeEvaluator::v2mEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   return toMaskConversionHelper<TR::v2m>(node, false, cg);
   }

TR::Register*
OMR::ARM64::TreeEvaluator::m2bEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   return TR::TreeEvaluator::unImpOpEvaluator(node, cg);
   }

TR::Register*
OMR::ARM64::TreeEvaluator::m2sEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   TR::Node *firstChild = node->getFirstChild();
   TR_ASSERT_FATAL_WITH_NODE(node, firstChild->getDataType().getVectorLength() == TR::VectorLength128,
                   "Only 128-bit vectors are supported %s", firstChild->getDataType().toString());

   TR::Register *maskReg = cg->evaluate(firstChild);
   TR::Register *tempReg = cg->allocateRegister(TR_VRF);
   TR::Register *resReg = cg->allocateRegister(TR_GPR);

   /*
    * Use vector extract to move the 7th and 8th element to the 1st and 2nd element position respectively.
    * Then, perform unsigned shift right by 7bit to get boolean results.
    */
   generateTrg1Src2ImmInstruction(cg, TR::InstOpCode::vext16b, node, tempReg, maskReg, maskReg, 7);
   generateVectorShiftImmediateInstruction(cg, TR::InstOpCode::vushr16b, node, tempReg, tempReg, 7);
   generateMovVectorElementToGPRInstruction(cg, TR::InstOpCode::umovwh, node, resReg, tempReg, 0);

   node->setRegister(resReg);
   cg->stopUsingRegister(tempReg);
   cg->decReferenceCount(firstChild);
   return resReg;
   }

TR::Register*
OMR::ARM64::TreeEvaluator::m2iEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   TR::Node *firstChild = node->getFirstChild();
   TR_ASSERT_FATAL_WITH_NODE(node, firstChild->getDataType().getVectorLength() == TR::VectorLength128,
                   "Only 128-bit vectors are supported %s", firstChild->getDataType().toString());

   TR::Register *maskReg = cg->evaluate(firstChild);
   TR::Register *tempReg = cg->allocateRegister(TR_VRF);
   TR::Register *resReg = cg->allocateRegister(TR_GPR);
   generateTrg1Src1Instruction(cg, TR::InstOpCode::vxtn_4h, node, tempReg, maskReg);
   generateTrg1Src1Instruction(cg, TR::InstOpCode::vxtn_8b, node, tempReg, tempReg);
   generateTrg1Src1Instruction(cg, TR::InstOpCode::vneg16b, node, tempReg, tempReg);
   generateMovVectorElementToGPRInstruction(cg, TR::InstOpCode::umovws, node, resReg, tempReg, 0);

   node->setRegister(resReg);
   cg->stopUsingRegister(tempReg);
   cg->decReferenceCount(firstChild);
   return resReg;
   }

TR::Register*
OMR::ARM64::TreeEvaluator::m2lEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   TR::Node *firstChild = node->getFirstChild();
   TR_ASSERT_FATAL_WITH_NODE(node, firstChild->getDataType().getVectorLength() == TR::VectorLength128,
                   "Only 128-bit vectors are supported %s", firstChild->getDataType().toString());

   TR::Register *maskReg = cg->evaluate(firstChild);
   TR::Register *tempReg = cg->allocateRegister(TR_VRF);
   TR::Register *resReg = cg->allocateRegister(TR_GPR);
   generateTrg1Src1Instruction(cg, TR::InstOpCode::vxtn_8b, node, tempReg, maskReg);
   generateTrg1Src1Instruction(cg, TR::InstOpCode::vneg16b, node, tempReg, tempReg);
   generateMovVectorElementToGPRInstruction(cg, TR::InstOpCode::umovxd, node, resReg, tempReg, 0);

   node->setRegister(resReg);
   cg->stopUsingRegister(tempReg);
   cg->decReferenceCount(firstChild);
   return resReg;
   }

TR::Register*
OMR::ARM64::TreeEvaluator::m2vEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   TR::Node *firstChild = node->getFirstChild();
   TR_ASSERT_FATAL_WITH_NODE(node, firstChild->getDataType().getVectorLength() == TR::VectorLength128,
                   "Only 128-bit vectors are supported %s", firstChild->getDataType().toString());

   TR::Register *maskReg = cg->evaluate(firstChild);
   TR::Register *resReg = cg->allocateRegister(TR_VRF);
   generateTrg1Src1Instruction(cg, TR::InstOpCode::vneg16b, node, resReg, maskReg);

   node->setRegister(resReg);
   cg->decReferenceCount(firstChild);
   return resReg;
   }

// vector evaluators
TR::Instruction *
OMR::ARM64::TreeEvaluator::vsplatsImmediateHelper(TR::Node *node, TR::CodeGenerator *cg, TR::Node *firstChild, TR::DataType elementType, TR::Register *treg)
   {
   if (firstChild->getOpCode().isLoadConst())
      {
      auto constValue = firstChild->getConstValue();
      switch (elementType)
         {
         case TR::Int8:
            return generateTrg1ImmInstruction(cg, TR::InstOpCode::vmovi16b, node, treg, constValue & 0xff);

         case TR::Int16:
            {
            uint16_t value = static_cast<uint16_t>(constValue & 0xffff);
            TR::Instruction *instr = tryToGenerateMovImm16ShiftedInstruction(node, cg, treg, value);

            if (instr)
               {
               return instr;
               }
            break;
            }

         case TR::Int32:
            {
            uint32_t value = static_cast<uint32_t>(constValue & 0xffffffff);
            TR::Instruction *instr = tryToGenerateMovImm32ShiftedInstruction(node, cg, treg, value);

            if (instr)
               {
               return instr;
               }

            uint64_t concatValue = (static_cast<uint64_t>(value) << 32) | value;
            if ((instr = tryToGenerateImm64Instruction(node, cg, treg, TR::InstOpCode::vmovi2d, concatValue)))
               {
               return instr;
               }
            break;
            }

         case TR::Int64:
            {
            uint32_t lower32bit = constValue & 0xffffffff;
            uint32_t upper32bit = (constValue >> 32) & 0xffffffff;
            TR::Instruction *instr;

            if (lower32bit == upper32bit)
               {
               if ((instr = tryToGenerateMovImm32ShiftedInstruction(node, cg, treg, lower32bit)))
                  {
                  return instr;
                  }
               }

            if ((instr = tryToGenerateImm64Instruction(node, cg, treg, TR::InstOpCode::vmovi2d, constValue)))
               {
               return instr;
               }
            break;
            }

         default:
            break;
         }

      }
   return NULL;
   }

TR::Register*
OMR::ARM64::TreeEvaluator::vsplatsEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   TR::Node *firstChild = node->getFirstChild();

   TR::InstOpCode::Mnemonic op;

   TR_ASSERT_FATAL_WITH_NODE(node, node->getDataType().getVectorLength() == TR::VectorLength128,
                   "Only 128-bit vectors are supported %s", node->getDataType().toString());
   auto elementType = node->getDataType().getVectorElementType();

   TR::Register *resReg = cg->allocateRegister(TR_VRF);
   node->setRegister(resReg);

   if (!vsplatsImmediateHelper(node, cg, firstChild, elementType, resReg))
      {
      TR::Register *srcReg = cg->evaluate(firstChild);
      switch (elementType)
         {
         case TR::Int8:
            TR_ASSERT(srcReg->getKind() == TR_GPR, "unexpected Register kind");
            op = TR::InstOpCode::vdup16b;
            break;
         case TR::Int16:
            TR_ASSERT(srcReg->getKind() == TR_GPR, "unexpected Register kind");
            op = TR::InstOpCode::vdup8h;
            break;
         case TR::Int32:
            TR_ASSERT(srcReg->getKind() == TR_GPR, "unexpected Register kind");
            op = TR::InstOpCode::vdup4s;
            break;
         case TR::Int64:
            TR_ASSERT(srcReg->getKind() == TR_GPR, "unexpected Register kind");
            op = TR::InstOpCode::vdup2d;
            break;
         case TR::Float:
            TR_ASSERT(srcReg->getKind() == TR_FPR, "unexpected Register kind");
            op = TR::InstOpCode::vdupe4s;
            break;
         case TR::Double:
            TR_ASSERT(srcReg->getKind() == TR_FPR, "unexpected Register kind");
            op = TR::InstOpCode::vdupe2d;
            break;
         default:
            TR_ASSERT(false, "unrecognized vector type %s", node->getDataType().toString());
            return NULL;
         }

      generateTrg1Src1Instruction(cg, op, node, resReg, srcReg);
      }

   cg->decReferenceCount(firstChild);
   return resReg;
   }

TR::Register*
OMR::ARM64::TreeEvaluator::vfmaEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   TR::Node *firstChild = node->getFirstChild();
   TR::Node *secondChild = node->getSecondChild();
   TR::Node *thirdChild = node->getThirdChild();

   TR::Register *firstReg  = cg->evaluate(firstChild);
   TR::Register *secondReg  = cg->evaluate(secondChild);
   TR::Register *thirdReg  = cg->evaluate(thirdChild);

   TR::InstOpCode::Mnemonic op;

   switch (node->getDataType().getVectorElementType())
      {
      case TR::Float:
         op = TR::InstOpCode::vfmla4s;
         break;
      case TR::Double:
         op = TR::InstOpCode::vfmla2d;
         break;
      default:
         TR_ASSERT_FATAL_WITH_NODE(node, false, "unrecognized vector type %s", node->getDataType().toString());
         return NULL;
      }

   TR::Register *targetReg;

   if (cg->canClobberNodesRegister(thirdChild))
      {
      targetReg = thirdReg;
      }
   else
      {
      targetReg = cg->allocateRegister(TR_VRF);
      generateTrg1Src2Instruction(cg, TR::InstOpCode::vorr16b, node, targetReg, thirdReg, thirdReg);
      }
   generateTrg1Src2Instruction(cg, op, node, targetReg, firstReg, secondReg);
   node->setRegister(targetReg);
   cg->decReferenceCount(firstChild);
   cg->decReferenceCount(secondChild);
   cg->decReferenceCount(thirdChild);

   return targetReg;
   }

enum VectorCompareOps : int32_t
   {
   VECTOR_COMPARE_INVALID,
   VECTOR_COMPARE_EQ,
   VECTOR_COMPARE_NE,
   VECTOR_COMPARE_LT,
   VECTOR_COMPARE_LE,
   VECTOR_COMPARE_GT,
   VECTOR_COMPARE_GE,
   NumVectorCompareOps
   };

static const TR::InstOpCode::Mnemonic vectorCompareOpCodes[NumVectorCompareOps][TR::NumVectorElementTypes] =
   {  //       TR::Int8                 TR::Int16                TR::Int32                TR::Int64                TR::Float                TR::Double
      { TR::InstOpCode::vcmeq16b, TR::InstOpCode::vcmeq8h, TR::InstOpCode::vcmeq4s, TR::InstOpCode::vcmeq2d, TR::InstOpCode::vfcmeq4s, TR::InstOpCode::vfcmeq2d}, // EQ
      { TR::InstOpCode::vcmeq16b, TR::InstOpCode::vcmeq8h, TR::InstOpCode::vcmeq4s, TR::InstOpCode::vcmeq2d, TR::InstOpCode::vfcmeq4s, TR::InstOpCode::vfcmeq2d}, // NE (apply not after compare)
      { TR::InstOpCode::vcmgt16b, TR::InstOpCode::vcmgt8h, TR::InstOpCode::vcmgt4s, TR::InstOpCode::vcmgt2d, TR::InstOpCode::vfcmgt4s, TR::InstOpCode::vfcmgt2d}, // LT (flip lhs and rhs)
      { TR::InstOpCode::vcmge16b, TR::InstOpCode::vcmge8h, TR::InstOpCode::vcmge4s, TR::InstOpCode::vcmge2d, TR::InstOpCode::vfcmge4s, TR::InstOpCode::vfcmge2d}, // LE (flip lhs and rhs)
      { TR::InstOpCode::vcmgt16b, TR::InstOpCode::vcmgt8h, TR::InstOpCode::vcmgt4s, TR::InstOpCode::vcmgt2d, TR::InstOpCode::vfcmgt4s, TR::InstOpCode::vfcmgt2d}, // GT
      { TR::InstOpCode::vcmge16b, TR::InstOpCode::vcmge8h, TR::InstOpCode::vcmge4s, TR::InstOpCode::vcmge2d, TR::InstOpCode::vfcmge4s, TR::InstOpCode::vfcmge2d}, // GE
   };

static const TR::InstOpCode::Mnemonic vectorCompareZeroOpCodes[NumVectorCompareOps][TR::NumVectorElementTypes] =
   {  //       TR::Int8                      TR::Int16                     TR::Int32                     TR::Int64                     TR::Float                     TR::Double
      { TR::InstOpCode::vcmeq16b_zero, TR::InstOpCode::vcmeq8h_zero, TR::InstOpCode::vcmeq4s_zero, TR::InstOpCode::vcmeq2d_zero, TR::InstOpCode::vfcmeq4s_zero, TR::InstOpCode::vfcmeq2d_zero}, // EQ
      { TR::InstOpCode::vcmeq16b_zero, TR::InstOpCode::vcmeq8h_zero, TR::InstOpCode::vcmeq4s_zero, TR::InstOpCode::vcmeq2d_zero, TR::InstOpCode::vfcmeq4s_zero, TR::InstOpCode::vfcmeq2d_zero}, // NE (apply not after compare)
      { TR::InstOpCode::vcmlt16b_zero, TR::InstOpCode::vcmlt8h_zero, TR::InstOpCode::vcmlt4s_zero, TR::InstOpCode::vcmlt2d_zero, TR::InstOpCode::vfcmlt4s_zero, TR::InstOpCode::vfcmlt2d_zero}, // LT
      { TR::InstOpCode::vcmle16b_zero, TR::InstOpCode::vcmle8h_zero, TR::InstOpCode::vcmle4s_zero, TR::InstOpCode::vcmle2d_zero, TR::InstOpCode::vfcmle4s_zero, TR::InstOpCode::vfcmle2d_zero}, // LE
      { TR::InstOpCode::vcmgt16b_zero, TR::InstOpCode::vcmgt8h_zero, TR::InstOpCode::vcmgt4s_zero, TR::InstOpCode::vcmgt2d_zero, TR::InstOpCode::vfcmgt4s_zero, TR::InstOpCode::vfcmgt2d_zero}, // GT
      { TR::InstOpCode::vcmge16b_zero, TR::InstOpCode::vcmge8h_zero, TR::InstOpCode::vcmge4s_zero, TR::InstOpCode::vcmge2d_zero, TR::InstOpCode::vfcmge4s_zero, TR::InstOpCode::vfcmge2d_zero}, // GE
   };

/**
 * @brief A helper function for generating instuction sequence for vector compare operations
 *
 * @param[in] node:               node
 * @param[in] compareOp:          enum describing the compare operation
 * @param[in] omitNot:            if true, NOT instruction after comparison is not generated
 * @param[out] flipCompareResult: when not NULL, true is returned if the compare result needs to be flipped
 * @param[in] cg:                 CodeGenerator
 * @return vector register containing all 1 or all 0 elements depending on the result of the comparison.
 */
static TR::Register*
vcmpHelper(TR::Node *node, VectorCompareOps compareOp, bool omitNot, bool *flipCompareResult, TR::CodeGenerator *cg)
   {
   TR::Node *firstChild = node->getFirstChild();
   TR::Node *secondChild = node->getSecondChild();
   TR::InstOpCode::Mnemonic op;
   const bool notAfterCompare = (compareOp == VECTOR_COMPARE_NE);
   bool recursivelyDecRefCountOnSecondChild;
   TR::Register *firstReg  = cg->evaluate(firstChild);
   TR::Register *targetReg = cg->allocateRegister(TR_VRF);
   TR::DataType elemType = firstChild->getDataType().getVectorElementType();

   TR_ASSERT_FATAL_WITH_NODE(node, (elemType >= TR::Int8) && (elemType <= TR::Double) , "unrecognized vector type %s", firstChild->getDataType().toString());

   if ((secondChild->getOpCode().getVectorOperation() == TR::vsplats) && (secondChild->getRegister() == NULL) && (secondChild->getFirstChild()->isConstZeroValue()))
      {
      recursivelyDecRefCountOnSecondChild = true;
      op = vectorCompareZeroOpCodes[compareOp - 1][elemType - 1];
      generateTrg1Src1Instruction(cg, op, node, targetReg, firstReg);
      }
   else
      {
      TR::Register *secondReg  = cg->evaluate(secondChild);
      recursivelyDecRefCountOnSecondChild = false;
      op = vectorCompareOpCodes[compareOp - 1][elemType - 1];
      if ((compareOp == VECTOR_COMPARE_LT) || (compareOp == VECTOR_COMPARE_LE))
         {
         /* Flip lhs and rhs for those compare operation since aarch64 does not have vector compare lt nor le. */
         generateTrg1Src2Instruction(cg, op, node, targetReg, secondReg, firstReg);
         }
      else
         {
         generateTrg1Src2Instruction(cg, op, node, targetReg, firstReg, secondReg);
         }
      }

   /*
    * If this vector compare node only appears once as a child of masked binary operations,
    * the NOT instruction can be omitted because the inlineVectorMaskedBinaryOp generates `bit` instead of `bif`.
    */
   if (!omitNot)
      {
      if (notAfterCompare)
         {
         generateTrg1Src1Instruction(cg, TR::InstOpCode::vnot16b, node, targetReg, targetReg);
         }
      }
   else
      {
      if (notAfterCompare)
         {
         TR::Compilation *comp = cg->comp();
         if (comp->getOption(TR_TraceCG))
            {
            traceMsg(comp, "omitting vnot instruction at node %p\n", node);
            }
         }
      }
   if (flipCompareResult != NULL)
      {
      *flipCompareResult = notAfterCompare;
      }
   node->setRegister(targetReg);
   cg->decReferenceCount(firstChild);
   if (recursivelyDecRefCountOnSecondChild)
      {
      cg->recursivelyDecReferenceCount(secondChild);
      }
   else
      {
      cg->decReferenceCount(secondChild);
      }
   return targetReg;
   }

TR::Register*
OMR::ARM64::TreeEvaluator::vcmpeqEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   return vcmpHelper(node, VECTOR_COMPARE_EQ, false, NULL, cg);
   }

TR::Register*
OMR::ARM64::TreeEvaluator::vcmpneEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   return vcmpHelper(node, VECTOR_COMPARE_NE, false, NULL, cg);
   }

TR::Register*
OMR::ARM64::TreeEvaluator::vcmpltEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   return vcmpHelper(node, VECTOR_COMPARE_LT, false, NULL, cg);
   }

TR::Register*
OMR::ARM64::TreeEvaluator::vcmpgtEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   return vcmpHelper(node,VECTOR_COMPARE_GT, false, NULL, cg);
   }

TR::Register*
OMR::ARM64::TreeEvaluator::vcmpleEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   return vcmpHelper(node, VECTOR_COMPARE_LE, false, NULL, cg);
   }

TR::Register*
OMR::ARM64::TreeEvaluator::vcmpgeEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   return vcmpHelper(node, VECTOR_COMPARE_GE, false, NULL, cg);
   }

TR::Register*
OMR::ARM64::TreeEvaluator::vloadiEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   return TR::TreeEvaluator::vloadEvaluator(node, cg);
   }

TR::Register*
OMR::ARM64::TreeEvaluator::vstoreiEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   return TR::TreeEvaluator::vstoreEvaluator(node, cg);
   }

typedef TR::Register *(*reductionEvaluatorHelper)(TR::Node *node, TR::DataType et, TR::Register *resReg, TR::Register *srcReg, TR::CodeGenerator *cg);
/**
 * @brief A helper function for generating instruction sequence for reduction operations
 *
 * @param[in] node: node
 * @param[in] cg: CodeGenerator
 * @param[in] et: element type
 * @param[in] op: reduction opcode
 * @param[in] evaluatorHelper: optional pointer to helper function which generates instruction stream for operation
 * @return vector register containing the result
 */
static TR::Register *
inlineVectorReductionOp(TR::Node *node, TR::CodeGenerator *cg, TR::DataType et, TR::InstOpCode::Mnemonic op, reductionEvaluatorHelper evaluatorHelper = NULL)
   {
   TR::Node *firstChild = node->getFirstChild();
   TR::Register *sourceReg = cg->evaluate(firstChild);

   TR_ASSERT_FATAL_WITH_NODE(node, sourceReg->getKind() == TR_VRF, "unexpected Register kind");

   TR::Register *resReg = et.isIntegral() ? cg->allocateRegister(TR_GPR) : cg->allocateRegister(TR_VRF);

   node->setRegister(resReg);
   TR_ASSERT_FATAL_WITH_NODE(node, (op != TR::InstOpCode::bad) || (evaluatorHelper != NULL), "If op is TR::InstOpCode::bad, evaluatorHelper must not be NULL");
   if (evaluatorHelper != NULL)
      {
      (*evaluatorHelper)(node, et, resReg, sourceReg, cg);
      }
   else
      {
      TR::Register *tmpReg = NULL;
      if (et.isIntegral())
         {
         tmpReg = cg->allocateRegister(TR_VRF);
         TR::InstOpCode::Mnemonic movOp;
         switch (et)
            {
            case TR::Int8:
               movOp = TR::InstOpCode::smovwb;
               break;
            case TR::Int16:
               movOp = TR::InstOpCode::smovwh;
               break;
            case TR::Int32:
               movOp = TR::InstOpCode::umovws;
               break;
            case TR::Int64:
               movOp = TR::InstOpCode::umovxd;
               break;
            default:
               TR_ASSERT_FATAL_WITH_NODE(node, false, "Unexpected element type");
               break;
            }
         generateTrg1Src1Instruction(cg, op, node, tmpReg, sourceReg);
         generateMovVectorElementToGPRInstruction(cg, movOp, node, resReg, tmpReg, 0);
         cg->stopUsingRegister(tmpReg);
         }
      else
         {
         generateTrg1Src1Instruction(cg, op, node, resReg, sourceReg);
         }
      }

   cg->decReferenceCount(firstChild);
   return resReg;
   }

/**
 * @brief A helper function for generating instuction sequence for reduction add operations for vectors of float elements.
 *
 * @param[in] node: node
 * @param[in] et: element type
 * @param[in] resReg: result register
 * @param[in] srcReg: source register
 * @param[in] cg: CodeGenerator
 * @return floating point register containing the result
 */
static TR::Register*
vreductionAddFloatHelper(TR::Node *node, TR::DataType et, TR::Register *resReg, TR::Register *srcReg, TR::CodeGenerator *cg)
   {
   if (et == TR::Float)
      {
      TR::Register *tmpReg = cg->allocateRegister(TR_VRF);
      generateTrg1Src2Instruction(cg, TR::InstOpCode::vfaddp4s, node, tmpReg, srcReg, srcReg);
      generateTrg1Src1Instruction(cg, TR::InstOpCode::faddp2s, node, resReg, tmpReg);
      cg->stopUsingRegister(tmpReg);
      }
   else if (et == TR::Double)
      {
      generateTrg1Src1Instruction(cg, TR::InstOpCode::faddp2d, node, resReg, srcReg);
      }
   else
      {
      TR_ASSERT_FATAL_WITH_NODE(node, false, "Unexpected element type");
      }

   return resReg;
   }

TR::Register*
OMR::ARM64::TreeEvaluator::vreductionAddEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   TR_ASSERT_FATAL_WITH_NODE(node, node->getFirstChild()->getDataType().getVectorLength() == TR::VectorLength128,
                   "Only 128-bit vectors are supported %s", node->getFirstChild()->getDataType().toString());

   TR::DataType et = node->getFirstChild()->getDataType().getVectorElementType();
   TR::InstOpCode::Mnemonic op = TR::InstOpCode::bad;
   reductionEvaluatorHelper evaluationHelper = NULL;
   switch(et)
      {
      case TR::Int8:
         op = TR::InstOpCode::vaddv16b;
         break;
      case TR::Int16:
         op = TR::InstOpCode::vaddv8h;
         break;
      case TR::Int32:
         op = TR::InstOpCode::vaddv4s;
         break;
      case TR::Int64:
         op = TR::InstOpCode::addp2d;
         break;
      case TR::Float:
      case TR::Double:
         evaluationHelper = vreductionAddFloatHelper;
         break;
      default:
         TR_ASSERT_FATAL_WITH_NODE(node, false, "unrecognized vector type %s", node->getFirstChild()->getDataType().toString());
         return NULL;
      }
   return inlineVectorReductionOp(node, cg, et, op, evaluationHelper);
   }

/**
 * @brief A helper function for generating instuction sequence for reduction operations.
 *
 * @param[in] node: node
 * @param[in] et: element type
 * @param[in] op: opcode
 * @param[in] resReg: result register
 * @param[in] srcReg: source register
 * @param[in] cg: CodeGenerator
 * @return general purpose register containing the result
 */
static TR::Register*
vreductionHelper(TR::Node *node, TR::DataType et, TR::InstOpCode::Mnemonic op, TR::Register *resReg, TR::Register *srcReg, TR::CodeGenerator *cg)
   {
   bool useGPRForResult = true;

   TR::InstOpCode::Mnemonic movOp;
   switch (et)
      {
      case TR::Int8:
         movOp = TR::InstOpCode::smovwb;
         break;
      case TR::Int16:
         movOp = TR::InstOpCode::smovwh;
         break;
      case TR::Int32:
         movOp = TR::InstOpCode::umovws;
         break;
      case TR::Int64:
         movOp = TR::InstOpCode::umovxd;
         break;
      default:
         useGPRForResult = false;
         movOp = TR::InstOpCode::bad;
         break;
      }

   TR::Register *tmp0Reg = useGPRForResult ? cg->allocateRegister(TR_VRF) : resReg;
   TR::Register *tmp1Reg = cg->allocateRegister(TR_VRF);

   /*
    * Generating an ext instruction to split elements into 2 vector registers and perform a lanewise operation.
    */
   generateTrg1Src2ImmInstruction(cg, TR::InstOpCode::vext16b, node, tmp1Reg, srcReg, srcReg, 8);
   generateTrg1Src2Instruction(cg, op, node, tmp0Reg, srcReg, tmp1Reg);

   if ((et != TR::Int64) && (et != TR::Double))
      {
      generateTrg1Src2ImmInstruction(cg, TR::InstOpCode::vext16b, node, tmp1Reg, tmp0Reg, tmp0Reg, 4);
      generateTrg1Src2Instruction(cg, op, node, tmp0Reg, tmp0Reg, tmp1Reg);

      if ((et != TR::Int32) && (et != TR::Float))
         {
         generateTrg1Src2ImmInstruction(cg, TR::InstOpCode::vext16b, node, tmp1Reg, tmp0Reg, tmp0Reg, 2);
         generateTrg1Src2Instruction(cg, op, node, tmp0Reg, tmp0Reg, tmp1Reg);

         if (et == TR::Int8)
            {
            generateTrg1Src2ImmInstruction(cg, TR::InstOpCode::vext16b, node, tmp1Reg, tmp0Reg, tmp0Reg, 1);
            generateTrg1Src2Instruction(cg, op, node, tmp0Reg, tmp0Reg, tmp1Reg);
            }
         }
      }

   if (useGPRForResult)
      {
      generateMovVectorElementToGPRInstruction(cg, movOp, node, resReg, tmp0Reg, 0);
      cg->stopUsingRegister(tmp0Reg);
      }

   cg->stopUsingRegister(tmp1Reg);

   return resReg;
   }

/**
 * @brief A helper function for generating instuction sequence for reduction and operation.
 *
 * @param[in] node: node
 * @param[in] et: element type
 * @param[in] resReg: result register
 * @param[in] srcReg: source register
 * @param[in] cg: CodeGenerator
 * @return general purpose register containing the result
 */
static TR::Register*
vreductionAndHelper(TR::Node *node, TR::DataType et, TR::Register *resReg, TR::Register *srcReg, TR::CodeGenerator *cg)
   {
   return vreductionHelper(node, et, TR::InstOpCode::vand16b, resReg, srcReg, cg);
   }

TR::Register*
OMR::ARM64::TreeEvaluator::vreductionAndEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   TR_ASSERT_FATAL_WITH_NODE(node, node->getFirstChild()->getDataType().getVectorLength() == TR::VectorLength128,
                   "Only 128-bit vectors are supported %s", node->getFirstChild()->getDataType().toString());

   TR::DataType et = node->getFirstChild()->getDataType().getVectorElementType();
   switch(et)
      {
      case TR::Int8:
      case TR::Int16:
      case TR::Int32:
      case TR::Int64:
         return inlineVectorReductionOp(node, cg, et, TR::InstOpCode::bad, vreductionAndHelper);
      case TR::Float:
      case TR::Double:
         TR_ASSERT_FATAL_WITH_NODE(node, false, "Unexpected element type %s", node->getFirstChild()->getDataType().toString());
         return NULL;
      default:
         TR_ASSERT_FATAL_WITH_NODE(node, false, "unrecognized vector type %s", node->getFirstChild()->getDataType().toString());
         return NULL;
      }
   }

TR::Register*
OMR::ARM64::TreeEvaluator::vreductionFirstNonZeroEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   return TR::TreeEvaluator::unImpOpEvaluator(node, cg);
   }

/**
 * @brief A helper function for generating instuction sequence for reduction min/max operations for vectors of 64-bit integer elements.
 *
 * @param[in] node: node
 * @param[in] isMax: true if operation is max
 * @param[in] resReg: result register
 * @param[in] srcReg: source register
 * @param[in] cg: CodeGenerator
 * @return register containing the result
 */
static TR::Register *
vreductionMinMaxInt64Helper(TR::Node *node, bool isMax, TR::Register *resReg, TR::Register *srcReg, TR::CodeGenerator *cg)
   {
   TR::Register *tmpReg = cg->allocateRegister(TR_GPR);

   generateMovVectorElementToGPRInstruction(cg, TR::InstOpCode::umovxd, node, tmpReg, srcReg, 0);
   generateMovVectorElementToGPRInstruction(cg, TR::InstOpCode::umovxd, node, resReg, srcReg, 1);

   generateCompareInstruction(cg, node, tmpReg, resReg, true);
   generateCondTrg1Src2Instruction(cg, TR::InstOpCode::cselx, node, resReg, tmpReg, resReg, isMax ? TR::CC_GT : TR::CC_LT);

   cg->stopUsingRegister(tmpReg);

   return resReg;
   }

/**
 * @brief A helper function for generating instuction sequence for reduction max operations for vectors of 64-bit integer elements.
 *
 * @param[in] node: node
 * @param[in] et: element type
 * @param[in] resReg: result register
 * @param[in] srcReg: source register
 * @param[in] cg: CodeGenerator
 * @return register containing the result
 */
static TR::Register *
vreductionMaxInt64Helper(TR::Node *node, TR::DataType et, TR::Register *resReg, TR::Register *srcReg, TR::CodeGenerator *cg)
   {
   return vreductionMinMaxInt64Helper(node, true, resReg, srcReg, cg);
   }

TR::Register*
OMR::ARM64::TreeEvaluator::vreductionMaxEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   TR_ASSERT_FATAL_WITH_NODE(node, node->getFirstChild()->getDataType().getVectorLength() == TR::VectorLength128,
                   "Only 128-bit vectors are supported %s", node->getFirstChild()->getDataType().toString());

   TR::DataType et = node->getFirstChild()->getDataType().getVectorElementType();
   TR::InstOpCode::Mnemonic op = TR::InstOpCode::bad;
   reductionEvaluatorHelper evaluationHelper = NULL;
   switch(et)
      {
      case TR::Int8:
         op = TR::InstOpCode::vsmaxv16b;
         break;
      case TR::Int16:
         op = TR::InstOpCode::vsmaxv8h;
         break;
      case TR::Int32:
         op = TR::InstOpCode::vsmaxv4s;
         break;
      case TR::Int64:
         /* SMAXV does not accept 64bit elements */
         evaluationHelper = vreductionMaxInt64Helper;
         break;
      case TR::Float:
         op = TR::InstOpCode::vfmaxv4s;
         break;
      case TR::Double:
         op = TR::InstOpCode::fmaxp2d;
         break;
      default:
         TR_ASSERT_FATAL_WITH_NODE(node, false, "unrecognized vector type %s", node->getFirstChild()->getDataType().toString());
         return NULL;
      }
   return inlineVectorReductionOp(node, cg, et, op, evaluationHelper);
   }

/**
 * @brief A helper function for generating instuction sequence for reduction min operations for vectors of 64-bit integer elements.
 *
 * @param[in] node: node
 * @param[in] et: element type
 * @param[in] resReg: result register
 * @param[in] srcReg: source register
 * @param[in] cg: CodeGenerator
 * @return register containing the result
 */
static TR::Register *
vreductionMinInt64Helper(TR::Node *node, TR::DataType et, TR::Register *resReg, TR::Register *srcReg, TR::CodeGenerator *cg)
   {
   return vreductionMinMaxInt64Helper(node, false, resReg, srcReg, cg);
   }

TR::Register*
OMR::ARM64::TreeEvaluator::vreductionMinEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   TR_ASSERT_FATAL_WITH_NODE(node, node->getFirstChild()->getDataType().getVectorLength() == TR::VectorLength128,
                   "Only 128-bit vectors are supported %s", node->getFirstChild()->getDataType().toString());

   TR::DataType et = node->getFirstChild()->getDataType().getVectorElementType();
   TR::InstOpCode::Mnemonic op = TR::InstOpCode::bad;
   reductionEvaluatorHelper evaluationHelper = NULL;
   switch(et)
      {
      case TR::Int8:
         op = TR::InstOpCode::vsminv16b;
         break;
      case TR::Int16:
         op = TR::InstOpCode::vsminv8h;
         break;
      case TR::Int32:
         op = TR::InstOpCode::vsminv4s;
         break;
      case TR::Int64:
         /* SMINV does not accept 64bit elements */
         evaluationHelper = vreductionMinInt64Helper;
         break;
      case TR::Float:
         op = TR::InstOpCode::vfminv4s;
         break;
      case TR::Double:
         op = TR::InstOpCode::fminp2d;
         break;
      default:
         TR_ASSERT_FATAL_WITH_NODE(node, false, "unrecognized vector type %s", node->getFirstChild()->getDataType().toString());
         return NULL;
      }
   return inlineVectorReductionOp(node, cg, et, op, evaluationHelper);
   }

/**
 * @brief A helper function for generating instuction sequence for reduce multiplication of vectors with 64-bit integer elements.
 *
 * @param[in] node: node
 * @param[in] resReg: result register
 * @param[in] srcReg: source register
 * @param[in] cg: CodeGenerator
 * @return general purpose register containing the result
 */
static TR::Register*
vreductionMulInt64Helper(TR::Node *node, TR::Register *resReg, TR::Register *srcReg, TR::CodeGenerator *cg)
   {
   TR::Register *tmp0Reg = cg->allocateRegister(TR_GPR);

   generateMovVectorElementToGPRInstruction(cg, TR::InstOpCode::umovxd, node, tmp0Reg, srcReg, 0);
   generateMovVectorElementToGPRInstruction(cg, TR::InstOpCode::umovxd, node, resReg, srcReg, 1);
   generateMulInstruction(cg, node, resReg, tmp0Reg, resReg, true);

   cg->stopUsingRegister(tmp0Reg);

   return resReg;
   }

/**
 * @brief A helper function for generating instuction sequence for reduce multiplication of vectors with float or double elements.
 *
 * @details We cannot use vreductionHelper because the result of multiplication with helper is (a[0] * a[1]) * (a[2] * a[3]),
 *          which is not exactly the same as (a[0] * a[1] * a[2] * a[3]) for floating point numbers.
 *
 * @param[in] node: node
 * @param[in] et: element type
 * @param[in] resReg: result register
 * @param[in] srcReg: source register
 * @param[in] cg: CodeGenerator
 * @return general purpose register containing the result
 */
static TR::Register*
vreductionMulFloatingPointHelper(TR::Node *node, TR::DataType et, TR::Register *resReg, TR::Register *srcReg, TR::CodeGenerator *cg)
   {
   if (et == TR::Float)
      {
      generateTrg1Src2IndexedElementInstruction(cg, TR::InstOpCode::fmulelem_4s, node, resReg, srcReg, srcReg, 1);
      generateTrg1Src2IndexedElementInstruction(cg, TR::InstOpCode::fmulelem_4s, node, resReg, resReg, srcReg, 2);
      generateTrg1Src2IndexedElementInstruction(cg, TR::InstOpCode::fmulelem_4s, node, resReg, resReg, srcReg, 3);
      }
   else
      {
      generateTrg1Src2IndexedElementInstruction(cg, TR::InstOpCode::fmulelem_2d, node, resReg, srcReg, srcReg, 1);
      }

   return resReg;
   }

/**
 * @brief A helper function for generating instuction sequence for reduce multiplication of vectors.
 *
 * @param[in] node: node
 * @param[in] et: element type
 * @param[in] resReg: result register
 * @param[in] srcReg: source register
 * @param[in] cg: CodeGenerator
 * @return general purpose register containing the result
 */
static TR::Register*
vreductionMulHelper(TR::Node *node, TR::DataType et, TR::Register *resReg, TR::Register *srcReg, TR::CodeGenerator *cg)
   {
   switch(et)
      {
      case TR::Int8:
         return vreductionHelper(node, et, TR::InstOpCode::vmul16b, resReg, srcReg, cg);
      case TR::Int16:
         return vreductionHelper(node, et, TR::InstOpCode::vmul8h, resReg, srcReg, cg);
      case TR::Int32:
         return vreductionHelper(node, et, TR::InstOpCode::vmul4s, resReg, srcReg, cg);
      case TR::Int64:
         return vreductionMulInt64Helper(node, resReg, srcReg, cg);
      case TR::Float:
      case TR::Double:
         return vreductionMulFloatingPointHelper(node, et, resReg, srcReg, cg);
      default:
         TR_ASSERT_FATAL_WITH_NODE(node, false, "unrecognized vector type %s", et.toString());
         return NULL;
      }
   }

TR::Register*
OMR::ARM64::TreeEvaluator::vreductionMulEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   TR_ASSERT_FATAL_WITH_NODE(node, node->getFirstChild()->getDataType().getVectorLength() == TR::VectorLength128,
                   "Only 128-bit vectors are supported %s", node->getFirstChild()->getDataType().toString());

   TR::DataType et = node->getFirstChild()->getDataType().getVectorElementType();
   return inlineVectorReductionOp(node, cg, et, TR::InstOpCode::bad, vreductionMulHelper);
   }

/**
 * @brief A helper function for generating instuction sequence for reduction or operation.
 *
 * @param[in] node: node
 * @param[in] et: element type
 * @param[in] resReg: result register
 * @param[in] srcReg: source register
 * @param[in] cg: CodeGenerator
 * @return general purpose register containing the result
 */
static TR::Register*
vreductionOrHelper(TR::Node *node, TR::DataType et, TR::Register *resReg, TR::Register *srcReg, TR::CodeGenerator *cg)
   {
   return vreductionHelper(node, et, TR::InstOpCode::vorr16b, resReg, srcReg, cg);
   }

TR::Register*
OMR::ARM64::TreeEvaluator::vreductionOrEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   TR_ASSERT_FATAL_WITH_NODE(node, node->getFirstChild()->getDataType().getVectorLength() == TR::VectorLength128,
                   "Only 128-bit vectors are supported %s", node->getFirstChild()->getDataType().toString());

   TR::DataType et = node->getFirstChild()->getDataType().getVectorElementType();
   switch(et)
      {
      case TR::Int8:
      case TR::Int16:
      case TR::Int32:
      case TR::Int64:
         return inlineVectorReductionOp(node, cg, et, TR::InstOpCode::bad, vreductionOrHelper);
      case TR::Float:
      case TR::Double:
         TR_ASSERT_FATAL_WITH_NODE(node, false, "Unexpected element type %s", node->getFirstChild()->getDataType().toString());
         return NULL;
      default:
         TR_ASSERT_FATAL_WITH_NODE(node, false, "unrecognized vector type %s", node->getFirstChild()->getDataType().toString());
         return NULL;
      }
   }

TR::Register*
OMR::ARM64::TreeEvaluator::vreductionOrUncheckedEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   return TR::TreeEvaluator::unImpOpEvaluator(node, cg);
   }

/**
 * @brief A helper function for generating instuction sequence for reduction xor operation.
 *
 * @param[in] node: node
 * @param[in] et: element type
 * @param[in] resReg: result register
 * @param[in] srcReg: source register
 * @param[in] cg: CodeGenerator
 * @return general purpose register containing the result
 */
static TR::Register*
vreductionXorHelper(TR::Node *node, TR::DataType et, TR::Register *resReg, TR::Register *srcReg, TR::CodeGenerator *cg)
   {
   return vreductionHelper(node, et, TR::InstOpCode::veor16b, resReg, srcReg, cg);
   }

TR::Register*
OMR::ARM64::TreeEvaluator::vreductionXorEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   TR_ASSERT_FATAL_WITH_NODE(node, node->getFirstChild()->getDataType().getVectorLength() == TR::VectorLength128,
                   "Only 128-bit vectors are supported %s", node->getFirstChild()->getDataType().toString());

   TR::DataType et = node->getFirstChild()->getDataType().getVectorElementType();
   switch(et)
      {
      case TR::Int8:
      case TR::Int16:
      case TR::Int32:
      case TR::Int64:
         return inlineVectorReductionOp(node, cg, et, TR::InstOpCode::bad, vreductionXorHelper);
      case TR::Float:
      case TR::Double:
         TR_ASSERT_FATAL_WITH_NODE(node, false, "Unexpected element type %s", node->getFirstChild()->getDataType().toString());
         return NULL;
      default:
         TR_ASSERT_FATAL_WITH_NODE(node, false, "unrecognized vector type %s", node->getFirstChild()->getDataType().toString());
         return NULL;
      }
   }

TR::Register*
OMR::ARM64::TreeEvaluator::vreturnEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   return TR::TreeEvaluator::unImpOpEvaluator(node, cg);
   }

TR::Register*
OMR::ARM64::TreeEvaluator::vcallEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   return TR::TreeEvaluator::unImpOpEvaluator(node, cg);
   }

TR::Register*
OMR::ARM64::TreeEvaluator::vcalliEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   return TR::TreeEvaluator::unImpOpEvaluator(node, cg);
   }

TR::Register*
OMR::ARM64::TreeEvaluator::vbitselectEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   TR_ASSERT_FATAL_WITH_NODE(node, node->getFirstChild()->getDataType().getVectorLength() == TR::VectorLength128,
                   "Only 128-bit vectors are supported %s", node->getFirstChild()->getDataType().toString());

   TR::DataType et = node->getDataType().getVectorElementType();

   switch(et)
      {
      case TR::Int8:
      case TR::Int16:
      case TR::Int32:
      case TR::Int64:
         break;
      case TR::Float:
      case TR::Double:
         TR_ASSERT_FATAL_WITH_NODE(node, false, "Unexpected element type %s", node->getFirstChild()->getDataType().toString());
         return NULL;
      default:
         TR_ASSERT_FATAL_WITH_NODE(node, false, "unrecognized vector type %s", node->getFirstChild()->getDataType().toString());
         return NULL;
      }

   TR::Node *firstChild = node->getFirstChild();
   TR::Node *secondChild = node->getSecondChild();
   TR::Node *thirdChild = node->getThirdChild();

   TR::Register *firstReg = cg->evaluate(firstChild);
   TR::Register *secondReg = cg->evaluate(secondChild);
   TR::Register *thirdReg = cg->evaluate(thirdChild);
   TR::Register *targetReg;

   if (cg->canClobberNodesRegister(thirdChild))
      {
      targetReg = thirdReg;
      }
   else
      {
      targetReg = cg->allocateRegister(TR_VRF);
      generateTrg1Src2Instruction(cg, TR::InstOpCode::vorr16b, node, targetReg, thirdReg, thirdReg);
      }

   /*
    * vbitselect extracts bits from the first operand if the corresponding bit of the third operand is 1,
    * otherwise from the second operand.
    */
   generateTrg1Src2Instruction(cg, TR::InstOpCode::vbsl16b, node, targetReg, secondReg, firstReg);
   node->setRegister(targetReg);
   cg->decReferenceCount(firstChild);
   cg->decReferenceCount(secondChild);
   cg->decReferenceCount(thirdChild);

   return targetReg;
   }

TR::Register*
OMR::ARM64::TreeEvaluator::vcastEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   return TR::TreeEvaluator::unImpOpEvaluator(node, cg);
   }

TR::Register*
OMR::ARM64::TreeEvaluator::vconvEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   return TR::TreeEvaluator::unImpOpEvaluator(node, cg);
   }

TR::Register*
OMR::ARM64::TreeEvaluator::vsetelemEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   return TR::TreeEvaluator::unImpOpEvaluator(node, cg);
   }

TR::Register*
OMR::ARM64::TreeEvaluator::vRegLoadEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   return TR::TreeEvaluator::unImpOpEvaluator(node, cg);
   }

TR::Register*
OMR::ARM64::TreeEvaluator::vRegStoreEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   return TR::TreeEvaluator::unImpOpEvaluator(node, cg);
   }

static
VectorCompareOps getVectorCompareOp(TR::VectorOperation op)
   {
   switch (op)
      {
      case TR::vcmpeq:
         return VECTOR_COMPARE_EQ;
      case TR::vcmpne:
         return VECTOR_COMPARE_NE;
      case TR::vcmpgt:
         return VECTOR_COMPARE_GT;
      case TR::vcmpge:
         return VECTOR_COMPARE_GE;
      case TR::vcmplt:
         return VECTOR_COMPARE_LT;
      case TR::vcmple:
         return VECTOR_COMPARE_LE;
      default:
         return VECTOR_COMPARE_INVALID;
      }
   }

typedef TR::Register *(*binaryEvaluatorHelper)(TR::Node *node, TR::Register *resReg, TR::Register *lhsRes, TR::Register *rhsReg, TR::CodeGenerator *cg);
/**
 * @brief Helper functions for generating instruction sequence for masked binary operations
 *
 * @param[in] node: node
 * @param[in] cg: CodeGenerator
 * @param[in] op: binary opcode
 * @param[in] evaluatorHelper: optional pointer to helper function which generates instruction stream for operation
 * @return vector register containing the result
 */
static TR::Register *
inlineVectorMaskedBinaryOp(TR::Node *node, TR::CodeGenerator *cg, TR::InstOpCode::Mnemonic op, binaryEvaluatorHelper evaluatorHelper = NULL)
   {
   TR::Node *firstChild = node->getFirstChild();
   TR::Node *secondChild = node->getSecondChild();
   TR::Node *thirdChild = node->getThirdChild();
   TR::Register *lhsReg = cg->evaluate(firstChild);
   TR::Register *rhsReg = cg->evaluate(secondChild);

   TR_ASSERT_FATAL_WITH_NODE(node, lhsReg->getKind() == TR_VRF, "unexpected Register kind");
   TR_ASSERT_FATAL_WITH_NODE(node, rhsReg->getKind() == TR_VRF, "unexpected Register kind");

   TR::Register *resReg = cg->allocateRegister(TR_VRF);

   node->setRegister(resReg);
   TR_ASSERT_FATAL_WITH_NODE(node, (op != TR::InstOpCode::bad) || (evaluatorHelper != NULL), "If op is TR::InstOpCode::bad, evaluatorHelper must not be NULL");
   if (evaluatorHelper != NULL)
      {
      (*evaluatorHelper)(node, resReg, lhsReg, rhsReg, cg);
      }
   else
      {
      generateTrg1Src2Instruction(cg, op, node, resReg, lhsReg, rhsReg);
      }

   TR::ILOpCode thirdOp = thirdChild->getOpCode();
   bool flipMask = false;
   TR::Register *maskReg = NULL;
   VectorCompareOps compareOp;
   TR::VectorOperation convOp;
   if (thirdOp.isVectorOpCode() && thirdOp.isBooleanCompare() && (!thirdOp.isVectorMasked())
       && ((compareOp = getVectorCompareOp(thirdOp.getVectorOperation())) != VECTOR_COMPARE_INVALID)
       && (thirdChild->getReferenceCount() == 1) && (thirdChild->getRegister() == NULL))
      {
      maskReg = vcmpHelper(thirdChild, compareOp, true, &flipMask, cg);
      }
   else if (thirdOp.isVectorOpCode() && thirdOp.isConversion() && thirdOp.isMaskResult()
       && (((convOp = thirdOp.getVectorOperation()) == TR::s2m) || (convOp == TR::v2m))
       && (thirdChild->getReferenceCount() == 1) && (thirdChild->getRegister() == NULL))
      {
      flipMask = true;
      maskReg = (convOp == TR::s2m) ? toMaskConversionHelper<TR::s2m>(thirdChild, true, cg) : toMaskConversionHelper<TR::v2m>(thirdChild, true, cg);
      }
   else
      {
      maskReg = cg->evaluate(thirdChild);
      }
   TR_ASSERT_FATAL_WITH_NODE(node, maskReg->getKind() == TR_VRF, "unexpected Register kind");

   /*
    * BIT inserts each bit from the first source if the corresponding bit of the second source is 1.
    * BIF inserts each bit from the first source if the corresponding bit of the second source is 0.
    */
   generateTrg1Src2Instruction(cg, flipMask ? TR::InstOpCode::vbit16b : TR::InstOpCode::vbif16b, node, resReg, lhsReg, maskReg);

   cg->decReferenceCount(firstChild);
   cg->decReferenceCount(secondChild);
   cg->decReferenceCount(thirdChild);
   return resReg;
   }

/**
 * @brief Helper functions for generating instruction sequence for masked unary operations
 *
 * @param[in] node: node
 * @param[in] cg: CodeGenerator
 * @param[in] op: unary opcode
 * @return vector register containing the result
 */
static TR::Register *
inlineVectorMaskedUnaryOp(TR::Node *node, TR::CodeGenerator *cg, TR::InstOpCode::Mnemonic op)
   {
   TR::Node *firstChild = node->getFirstChild();
   TR::Node *secondChild = node->getSecondChild();

   TR::Register *srcReg = cg->evaluate(firstChild);

   TR_ASSERT_FATAL_WITH_NODE(node, srcReg->getKind() == TR_VRF, "unexpected Register kind");

   TR::Register *resReg = cg->allocateRegister(TR_VRF);
   node->setRegister(resReg);

   generateTrg1Src1Instruction(cg, op, node, resReg, srcReg);

   TR::ILOpCode secondOp = secondChild->getOpCode();
   bool flipMask = false;
   TR::Register *maskReg = NULL;
   VectorCompareOps compareOp;
   TR::VectorOperation convOp;
   if (secondOp.isVectorOpCode() && secondOp.isBooleanCompare() && (!secondOp.isVectorMasked())
       && ((compareOp = getVectorCompareOp(secondOp.getVectorOperation())) != VECTOR_COMPARE_INVALID)
       && (secondChild->getReferenceCount() == 1) && (secondChild->getRegister() == NULL))
      {
      maskReg = vcmpHelper(secondChild, compareOp, true, &flipMask, cg);
      }
   else if (secondOp.isVectorOpCode() && secondOp.isConversion() && secondOp.isMaskResult()
       && (((convOp = secondOp.getVectorOperation()) == TR::s2m) || (convOp == TR::v2m))
       && (secondChild->getReferenceCount() == 1) && (secondChild->getRegister() == NULL))
      {
      flipMask = true;
      maskReg = (convOp == TR::s2m) ? toMaskConversionHelper<TR::s2m>(secondChild, true, cg) : toMaskConversionHelper<TR::v2m>(secondChild, true, cg);
      }
   else
      {
      maskReg = cg->evaluate(secondChild);
      }
   TR_ASSERT_FATAL_WITH_NODE(node, maskReg->getKind() == TR_VRF, "unexpected Register kind");

   /*
    * BIT inserts each bit from the first source if the corresponding bit of the second source is 1.
    * BIF inserts each bit from the first source if the corresponding bit of the second source is 0.
    */
   generateTrg1Src2Instruction(cg, flipMask ? TR::InstOpCode::vbit16b : TR::InstOpCode::vbif16b, node, resReg, srcReg, maskReg);

   cg->decReferenceCount(firstChild);
   cg->decReferenceCount(secondChild);
   return resReg;
   }

TR::Register*
OMR::ARM64::TreeEvaluator::vmabsEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   TR_ASSERT_FATAL_WITH_NODE(node, node->getDataType().getVectorLength() == TR::VectorLength128,
                     "Only 128-bit vectors are supported %s", node->getDataType().toString());
   TR::InstOpCode::Mnemonic absOp;

   switch(node->getDataType().getVectorElementType())
      {
      case TR::Int8:
         absOp = TR::InstOpCode::vabs16b;
         break;
      case TR::Int16:
         absOp = TR::InstOpCode::vabs8h;
         break;
      case TR::Int32:
         absOp = TR::InstOpCode::vabs4s;
         break;
      case TR::Int64:
         absOp = TR::InstOpCode::vabs2d;
         break;
      case TR::Float:
         absOp = TR::InstOpCode::vfabs4s;
         break;
      case TR::Double:
         absOp = TR::InstOpCode::vfabs2d;
         break;
      default:
         TR_ASSERT_FATAL(false, "unrecognized vector type %s", node->getDataType().toString());
         return NULL;
      }
   return inlineVectorMaskedUnaryOp(node, cg, absOp);
   }

TR::Register*
OMR::ARM64::TreeEvaluator::vmaddEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   TR::InstOpCode::Mnemonic op;

   switch (node->getDataType().getVectorElementType())
      {
      case TR::Int8:
         op = TR::InstOpCode::vadd16b;
         break;
      case TR::Int16:
         op = TR::InstOpCode::vadd8h;
         break;
      case TR::Int32:
         op = TR::InstOpCode::vadd4s;
         break;
      case TR::Int64:
         op = TR::InstOpCode::vadd2d;
         break;
      case TR::Float:
         op = TR::InstOpCode::vfadd4s;
         break;
      case TR::Double:
         op = TR::InstOpCode::vfadd2d;
         break;
      default:
         TR_ASSERT_FATAL_WITH_NODE(node, false, "unrecognized vector type %s", node->getDataType().toString());
         return NULL;
      }
   return inlineVectorMaskedBinaryOp(node, cg, op);
   }

TR::Register*
OMR::ARM64::TreeEvaluator::vmandEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   TR::InstOpCode::Mnemonic op;

   switch (node->getDataType().getVectorElementType())
      {
      case TR::Int8:
      case TR::Int16:
      case TR::Int32:
      case TR::Int64:
         op = TR::InstOpCode::vand16b;
         break;

      default:
         TR_ASSERT_FATAL_WITH_NODE(node, false, "unrecognized vector type %s", node->getDataType().toString());
         return NULL;
      }
   return inlineVectorMaskedBinaryOp(node, cg, op);
   }

TR::Register*
OMR::ARM64::TreeEvaluator::vmcmpeqEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   return TR::TreeEvaluator::unImpOpEvaluator(node, cg);
   }

TR::Register*
OMR::ARM64::TreeEvaluator::vmcmpneEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   return TR::TreeEvaluator::unImpOpEvaluator(node, cg);
   }

TR::Register*
OMR::ARM64::TreeEvaluator::vmcmpgtEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   return TR::TreeEvaluator::unImpOpEvaluator(node, cg);
   }

TR::Register*
OMR::ARM64::TreeEvaluator::vmcmpgeEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   return TR::TreeEvaluator::unImpOpEvaluator(node, cg);
   }

TR::Register*
OMR::ARM64::TreeEvaluator::vmcmpltEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   return TR::TreeEvaluator::unImpOpEvaluator(node, cg);
   }

TR::Register*
OMR::ARM64::TreeEvaluator::vmcmpleEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   return TR::TreeEvaluator::unImpOpEvaluator(node, cg);
   }

TR::Register*
OMR::ARM64::TreeEvaluator::vmdivEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   TR_ASSERT_FATAL_WITH_NODE(node, node->getDataType().getVectorLength() == TR::VectorLength128,
                   "Only 128-bit vectors are supported %s", node->getDataType().toString());

   TR::InstOpCode::Mnemonic divOp = TR::InstOpCode::bad;
   binaryEvaluatorHelper evaluatorHelper = NULL;
   switch(node->getDataType().getVectorElementType())
      {
      case TR::Int8:
      case TR::Int16:
      case TR::Int32:
      case TR::Int64:
         evaluatorHelper = vdivIntHelper;
         break;
      case TR::Float:
         divOp = TR::InstOpCode::vfdiv4s;
         break;
      case TR::Double:
         divOp = TR::InstOpCode::vfdiv2d;
         break;
      default:
         TR_ASSERT(false, "unrecognized vector type %s", node->getDataType().toString());
         return NULL;
      }
   return inlineVectorMaskedBinaryOp(node, cg, divOp, evaluatorHelper);
   }

TR::Register*
OMR::ARM64::TreeEvaluator::vmfmaEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   TR::Node *firstChild = node->getFirstChild();
   TR::Node *secondChild = node->getSecondChild();
   TR::Node *thirdChild = node->getThirdChild();
   TR::Node *fourthChild = node->getChild(3);

   TR::Register *firstReg = cg->evaluate(firstChild);
   TR::Register *secondReg = cg->evaluate(secondChild);
   TR::Register *thirdReg = cg->evaluate(thirdChild);
   TR_ASSERT_FATAL_WITH_NODE(node, firstReg->getKind() == TR_VRF, "unexpected Register kind");
   TR_ASSERT_FATAL_WITH_NODE(node, secondReg->getKind() == TR_VRF, "unexpected Register kind");
   TR_ASSERT_FATAL_WITH_NODE(node, thirdReg->getKind() == TR_VRF, "unexpected Register kind");

   TR::InstOpCode::Mnemonic op;

   switch (node->getDataType().getVectorElementType())
      {
      case TR::Float:
         op = TR::InstOpCode::vfmla4s;
         break;
      case TR::Double:
         op = TR::InstOpCode::vfmla2d;
         break;
      default:
         TR_ASSERT_FATAL_WITH_NODE(node, false, "unrecognized vector type %s", node->getDataType().toString());
         return NULL;
      }

   TR::Register *targetReg;

   if (cg->canClobberNodesRegister(thirdChild))
      {
      targetReg = thirdReg;
      }
   else
      {
      targetReg = cg->allocateRegister(TR_VRF);
      generateTrg1Src2Instruction(cg, TR::InstOpCode::vorr16b, node, targetReg, thirdReg, thirdReg);
      }
   generateTrg1Src2Instruction(cg, op, node, targetReg, firstReg, secondReg);

   TR::ILOpCode fourthOp = fourthChild->getOpCode();
   bool flipMask = false;
   TR::Register *maskReg = NULL;
   VectorCompareOps compareOp;
   TR::VectorOperation convOp;
   if (fourthOp.isVectorOpCode() && fourthOp.isBooleanCompare() && (!fourthOp.isVectorMasked())
       && ((compareOp = getVectorCompareOp(fourthOp.getVectorOperation())) != VECTOR_COMPARE_INVALID)
       && (fourthChild->getReferenceCount() == 1) && (fourthChild->getRegister() == NULL))
      {
      maskReg = vcmpHelper(fourthChild, compareOp, true, &flipMask, cg);
      }
   else if (fourthOp.isVectorOpCode() && fourthOp.isConversion() && fourthOp.isMaskResult()
       && (((convOp = fourthOp.getVectorOperation()) == TR::s2m) || (convOp == TR::v2m))
       && (fourthChild->getReferenceCount() == 1) && (fourthChild->getRegister() == NULL))
      {
      flipMask = true;
      maskReg = (convOp == TR::s2m) ? toMaskConversionHelper<TR::s2m>(fourthChild, true, cg) : toMaskConversionHelper<TR::v2m>(fourthChild, true, cg);
      }
   else
      {
      maskReg = cg->evaluate(fourthChild);
      }
   TR_ASSERT_FATAL_WITH_NODE(node, maskReg->getKind() == TR_VRF, "unexpected Register kind");

   /*
    * BIT inserts each bit from the first source if the corresponding bit of the second source is 1.
    * BIF inserts each bit from the first source if the corresponding bit of the second source is 0.
    */
   generateTrg1Src2Instruction(cg, flipMask ? TR::InstOpCode::vbit16b : TR::InstOpCode::vbif16b, node, targetReg, firstReg, maskReg);

   node->setRegister(targetReg);
   cg->decReferenceCount(firstChild);
   cg->decReferenceCount(secondChild);
   cg->decReferenceCount(thirdChild);
   cg->decReferenceCount(fourthChild);

   return targetReg;
   }

TR::Register*
OMR::ARM64::TreeEvaluator::vmindexVectorEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   return TR::TreeEvaluator::unImpOpEvaluator(node, cg);
   }

TR::Register*
OMR::ARM64::TreeEvaluator::vmloadiEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   return TR::TreeEvaluator::unImpOpEvaluator(node, cg);
   }

TR::Register*
OMR::ARM64::TreeEvaluator::vmmaxEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   TR_ASSERT_FATAL_WITH_NODE(node, node->getDataType().getVectorLength() == TR::VectorLength128,
                   "Only 128-bit vectors are supported %s", node->getDataType().toString());

   TR::InstOpCode::Mnemonic op = TR::InstOpCode::bad;
   binaryEvaluatorHelper evaluatorHelper = NULL;
   switch(node->getDataType().getVectorElementType())
      {
      case TR::Int8:
         op = TR::InstOpCode::vsmax16b;
         break;
      case TR::Int16:
         op = TR::InstOpCode::vsmax8h;
         break;
      case TR::Int32:
         op = TR::InstOpCode::vsmax4s;
         break;
      case TR::Int64:
         evaluatorHelper = vmaxInt64Helper;
         break;
      case TR::Float:
         op = TR::InstOpCode::vfmax4s;
         break;
      case TR::Double:
         op = TR::InstOpCode::vfmax2d;
         break;
      default:
         TR_ASSERT(false, "unrecognized vector type %s", node->getDataType().toString());
         return NULL;
      }
   return inlineVectorMaskedBinaryOp(node, cg, op, evaluatorHelper);
   }

TR::Register*
OMR::ARM64::TreeEvaluator::vmminEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   TR_ASSERT_FATAL_WITH_NODE(node, node->getDataType().getVectorLength() == TR::VectorLength128,
                   "Only 128-bit vectors are supported %s", node->getDataType().toString());

   TR::InstOpCode::Mnemonic op = TR::InstOpCode::bad;
   binaryEvaluatorHelper evaluatorHelper = NULL;
   switch(node->getDataType().getVectorElementType())
      {
      case TR::Int8:
         op = TR::InstOpCode::vsmin16b;
         break;
      case TR::Int16:
         op = TR::InstOpCode::vsmin8h;
         break;
      case TR::Int32:
         op = TR::InstOpCode::vsmin4s;
         break;
      case TR::Int64:
         evaluatorHelper = vminInt64Helper;
         break;
      case TR::Float:
         op = TR::InstOpCode::vfmin4s;
         break;
      case TR::Double:
         op = TR::InstOpCode::vfmin2d;
         break;
      default:
         TR_ASSERT(false, "unrecognized vector type %s", node->getDataType().toString());
         return NULL;
      }
   return inlineVectorMaskedBinaryOp(node, cg, op, evaluatorHelper);
   }

TR::Register*
OMR::ARM64::TreeEvaluator::vmmulEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   TR_ASSERT_FATAL_WITH_NODE(node, node->getDataType().getVectorLength() == TR::VectorLength128,
                   "Only 128-bit vectors are supported %s", node->getDataType().toString());

   TR::InstOpCode::Mnemonic mulOp = TR::InstOpCode::bad;
   binaryEvaluatorHelper evaluatorHelper = NULL;
   switch(node->getDataType().getVectorElementType())
      {
      case TR::Int8:
         mulOp = TR::InstOpCode::vmul16b;
         break;
      case TR::Int16:
         mulOp = TR::InstOpCode::vmul8h;
         break;
      case TR::Int32:
         mulOp = TR::InstOpCode::vmul4s;
         break;
      case TR::Int64:
         evaluatorHelper = vmulInt64Helper;
         break;
      case TR::Float:
         mulOp = TR::InstOpCode::vfmul4s;
         break;
      case TR::Double:
         mulOp = TR::InstOpCode::vfmul2d;
         break;
      default:
         TR_ASSERT(false, "unrecognized vector type %s", node->getDataType().toString());
         return NULL;
      }
   return inlineVectorMaskedBinaryOp(node, cg, mulOp, evaluatorHelper);
   }

TR::Register*
OMR::ARM64::TreeEvaluator::vmnegEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   TR::InstOpCode::Mnemonic negOp;

   TR_ASSERT_FATAL_WITH_NODE(node, node->getDataType().getVectorLength() == TR::VectorLength128,
                   "Only 128-bit vectors are supported %s", node->getDataType().toString());

   switch(node->getDataType().getVectorElementType())
      {
      case TR::Int8:
         negOp = TR::InstOpCode::vneg16b;
         break;
      case TR::Int16:
         negOp = TR::InstOpCode::vneg8h;
         break;
      case TR::Int32:
         negOp = TR::InstOpCode::vneg4s;
         break;
      case TR::Int64:
         negOp = TR::InstOpCode::vneg2d;
         break;
      case TR::Float:
         negOp = TR::InstOpCode::vfneg4s;
         break;
      case TR::Double:
         negOp = TR::InstOpCode::vfneg2d;
         break;
      default:
         TR_ASSERT(false, "unrecognized vector type %s", node->getDataType().toString());
         return NULL;
      }
   return inlineVectorMaskedUnaryOp(node, cg, negOp);
   }

TR::Register*
OMR::ARM64::TreeEvaluator::vmnotEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   TR::InstOpCode::Mnemonic notOp;

   TR_ASSERT_FATAL_WITH_NODE(node, node->getDataType().getVectorLength() == TR::VectorLength128,
                   "Only 128-bit vectors are supported %s", node->getDataType().toString());

   switch(node->getDataType().getVectorElementType())
      {
      case TR::Int8:
      case TR::Int16:
      case TR::Int32:
      case TR::Int64:
         notOp = TR::InstOpCode::vnot16b;
         break;
      default:
         TR_ASSERT(false, "unrecognized vector type %s", node->getDataType().toString());
         return NULL;
      }
   return inlineVectorMaskedUnaryOp(node, cg, notOp);
   }

TR::Register*
OMR::ARM64::TreeEvaluator::vmorEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   TR::InstOpCode::Mnemonic op;

   switch (node->getDataType().getVectorElementType())
      {
      case TR::Int8:
      case TR::Int16:
      case TR::Int32:
      case TR::Int64:
         op = TR::InstOpCode::vorr16b;
         break;

      default:
         TR_ASSERT_FATAL_WITH_NODE(node, false, "unrecognized vector type %s", node->getDataType().toString());
         return NULL;
      }
   return inlineVectorMaskedBinaryOp(node, cg, op);
   }

TR::Register*
OMR::ARM64::TreeEvaluator::vmorUncheckedEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   return TR::TreeEvaluator::unImpOpEvaluator(node, cg);
   }

typedef void (*loadIdentityVectorHelper)(TR::Node *node, TR::DataType et, TR::Register *tmpReg, TR::CodeGenerator *cg);
/**
 * @brief A helper function for generating instruction sequence for masked reduction operations
 *
 * @param[in] node: node
 * @param[in] cg: CodeGenerator
 * @param[in] et: element type
 * @param[in] op: reduction opcode
 * @param[in] loadHelper: pointer to helper function which loads identity vector for the operation
 * @param[in] evaluatorHelper: optional pointer to helper function which generates instruction stream for operation
 * @return vector register containing the result
 */
static TR::Register *
inlineVectorMaskedReductionOp(TR::Node *node, TR::CodeGenerator *cg, TR::DataType et, TR::InstOpCode::Mnemonic op, loadIdentityVectorHelper loadHelper, reductionEvaluatorHelper evaluatorHelper = NULL)
   {
   TR::Node *firstChild = node->getFirstChild();
   TR::Node *secondChild = node->getSecondChild();
   TR::Register *sourceReg = cg->evaluate(firstChild);

   TR_ASSERT_FATAL_WITH_NODE(node, sourceReg->getKind() == TR_VRF, "unexpected Register kind");

   TR::ILOpCode secondOp = secondChild->getOpCode();
   bool flipMask = false;
   TR::Register *maskReg = NULL;
   VectorCompareOps compareOp;
   TR::VectorOperation convOp;
   if (secondOp.isVectorOpCode() && secondOp.isBooleanCompare() && (!secondOp.isVectorMasked())
       && ((compareOp = getVectorCompareOp(secondOp.getVectorOperation())) != VECTOR_COMPARE_INVALID)
       && (secondChild->getReferenceCount() == 1) && (secondChild->getRegister() == NULL))
      {
      maskReg = vcmpHelper(secondChild, compareOp, true, &flipMask, cg);
      }
   else if (secondOp.isVectorOpCode() && secondOp.isConversion() && secondOp.isMaskResult()
       && (((convOp = secondOp.getVectorOperation()) == TR::s2m) || (convOp == TR::v2m))
       && (secondChild->getReferenceCount() == 1) && (secondChild->getRegister() == NULL))
      {
      flipMask = true;
      maskReg = (convOp == TR::s2m) ? toMaskConversionHelper<TR::s2m>(secondChild, true, cg) : toMaskConversionHelper<TR::v2m>(secondChild, true, cg);
      }
   else
      {
      maskReg = cg->evaluate(secondChild);
      }
   TR_ASSERT_FATAL_WITH_NODE(node, maskReg->getKind() == TR_VRF, "unexpected Register kind");

   TR::Register *tmpReg = cg->allocateRegister(TR_VRF);
   /* loads identity vector to tmpReg */
   (*loadHelper)(node, et, tmpReg, cg);

   /*
    * BIT inserts each bit from the first source if the corresponding bit of the second source is 1.
    * BIF inserts each bit from the first source if the corresponding bit of the second source is 0.
    */
   generateTrg1Src2Instruction(cg, flipMask ? TR::InstOpCode::vbif16b : TR::InstOpCode::vbit16b, node, tmpReg, sourceReg, maskReg);

   TR::Register *resReg = et.isIntegral() ? cg->allocateRegister(TR_GPR) : cg->allocateRegister(TR_VRF);

   node->setRegister(resReg);
   TR_ASSERT_FATAL_WITH_NODE(node, (op != TR::InstOpCode::bad) || (evaluatorHelper != NULL), "If op is TR::InstOpCode::bad, evaluatorHelper must not be NULL");
   if (evaluatorHelper != NULL)
      {
      (*evaluatorHelper)(node, et, resReg, tmpReg, cg);
      }
   else
      {
      if (et.isIntegral())
         {
         TR::InstOpCode::Mnemonic movOp;
         switch (et)
            {
            case TR::Int8:
               movOp = TR::InstOpCode::smovwb;
               break;
            case TR::Int16:
               movOp = TR::InstOpCode::smovwh;
               break;
            case TR::Int32:
               movOp = TR::InstOpCode::umovws;
               break;
            case TR::Int64:
               movOp = TR::InstOpCode::umovxd;
               break;
            default:
               TR_ASSERT_FATAL_WITH_NODE(node, false, "Unexpected element type");
               break;
            }
         generateTrg1Src1Instruction(cg, op, node, tmpReg, tmpReg);
         generateMovVectorElementToGPRInstruction(cg, movOp, node, resReg, tmpReg, 0);
         }
      else
         {
         generateTrg1Src1Instruction(cg, op, node, resReg, tmpReg);
         }
      }

   cg->stopUsingRegister(tmpReg);
   cg->decReferenceCount(firstChild);
   cg->decReferenceCount(secondChild);
   return resReg;
   }

/**
 * @brief A helper function for loading a zero vector, which is an identity vector for reduction add/or/xor
 *
 * @param[in] node: node
 * @param[in] et: element type
 * @param[in] identityReg: register to load identity vector
 * @param[in] cg: CodeGenerator
 */
static void
loadZeroVector(TR::Node *node, TR::DataType et, TR::Register *identityReg, TR::CodeGenerator *cg)
   {
   generateTrg1ImmInstruction(cg, TR::InstOpCode::vmovi16b, node, identityReg, 0);
   }

TR::Register*
OMR::ARM64::TreeEvaluator::vmreductionAddEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   TR_ASSERT_FATAL_WITH_NODE(node, node->getFirstChild()->getDataType().getVectorLength() == TR::VectorLength128,
                   "Only 128-bit vectors are supported %s", node->getFirstChild()->getDataType().toString());

   TR::DataType et = node->getFirstChild()->getDataType().getVectorElementType();
   TR::InstOpCode::Mnemonic op = TR::InstOpCode::bad;
   reductionEvaluatorHelper evaluationHelper = NULL;
   switch(et)
      {
      case TR::Int8:
         op = TR::InstOpCode::vaddv16b;
         break;
      case TR::Int16:
         op = TR::InstOpCode::vaddv8h;
         break;
      case TR::Int32:
         op = TR::InstOpCode::vaddv4s;
         break;
      case TR::Int64:
         op = TR::InstOpCode::addp2d;
         break;
      case TR::Float:
      case TR::Double:
         evaluationHelper = vreductionAddFloatHelper;
         break;
      default:
         TR_ASSERT_FATAL_WITH_NODE(node, false, "unrecognized vector type %s", node->getFirstChild()->getDataType().toString());
         return NULL;
      }
   return inlineVectorMaskedReductionOp(node, cg, et, op, loadZeroVector, evaluationHelper);
   }

/**
 * @brief A helper function for loading identity vector for reduction add
 *
 * @param[in] node: node
 * @param[in] et: element type
 * @param[in] identityReg: register to load identity vector
 * @param[in] cg: CodeGenerator
 */
static void
loadIdentityVectorForReductionAnd(TR::Node *node, TR::DataType et, TR::Register *identityReg, TR::CodeGenerator *cg)
   {
   generateTrg1ImmInstruction(cg, TR::InstOpCode::vmovi16b, node, identityReg, 0xff);
   }

TR::Register*
OMR::ARM64::TreeEvaluator::vmreductionAndEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   TR_ASSERT_FATAL_WITH_NODE(node, node->getFirstChild()->getDataType().getVectorLength() == TR::VectorLength128,
                   "Only 128-bit vectors are supported %s", node->getFirstChild()->getDataType().toString());

   TR::DataType et = node->getFirstChild()->getDataType().getVectorElementType();
   switch(et)
      {
      case TR::Int8:
      case TR::Int16:
      case TR::Int32:
      case TR::Int64:
         return inlineVectorMaskedReductionOp(node, cg, et, TR::InstOpCode::bad, loadIdentityVectorForReductionAnd, vreductionAndHelper);
      case TR::Float:
      case TR::Double:
         TR_ASSERT_FATAL_WITH_NODE(node, false, "Unexpected element type %s", node->getFirstChild()->getDataType().toString());
         return NULL;
      default:
         TR_ASSERT_FATAL_WITH_NODE(node, false, "unrecognized vector type %s", node->getFirstChild()->getDataType().toString());
         return NULL;
      }
   }

TR::Register*
OMR::ARM64::TreeEvaluator::vmreductionFirstNonZeroEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   return TR::TreeEvaluator::unImpOpEvaluator(node, cg);
   }

/**
 * @brief A helper function for loading identity vector for reduction max
 *
 * @param[in] node: node
 * @param[in] et: element type
 * @param[in] identityReg: register to load identity vector
 * @param[in] cg: CodeGenerator
 */
static void
loadIdentityVectorForReductionMax(TR::Node *node, TR::DataType et, TR::Register *identityReg, TR::CodeGenerator *cg)
   {
   switch(et)
      {
      case TR::Int8:
         generateTrg1ImmInstruction(cg, TR::InstOpCode::vmovi16b, node, identityReg, 0x80);
         break;
      case TR::Int16:
         generateTrg1ImmShiftedInstruction(cg, TR::InstOpCode::vmovi8h, node, identityReg, 0x80, 8);
         break;
      case TR::Int32:
         generateTrg1ImmShiftedInstruction(cg, TR::InstOpCode::vmovi4s, node, identityReg, 0x80, 24);
         break;
      case TR::Int64:
         generateTrg1ImmInstruction(cg, TR::InstOpCode::vmovi2d, node, identityReg, 0x80); /* Loads 0xff000000_00000000 into each element */
         generateTrg1ImmShiftedInstruction(cg, TR::InstOpCode::vbicimm4s, node, identityReg, 0x7f, 24); /* Clear unneeded bits. We do not have bic for 64bit integer, but vbicimm4s works for this case. */
         break;
      case TR::Float:
         /* Negative Infinity */
         generateTrg1ImmShiftedInstruction(cg, TR::InstOpCode::vmovi4s, node, identityReg, 0xff, 24);
         generateTrg1ImmShiftedInstruction(cg, TR::InstOpCode::vorrimm4s, node, identityReg, 0x80, 16);
         break;
      case TR::Double:
         /* Negative Infinity */
         generateTrg1ImmInstruction(cg, TR::InstOpCode::vmovi2d, node, identityReg, 0x80); /* Loads 0xff000000_00000000 into each element */
         generateVectorShiftImmediateInstruction(cg, TR::InstOpCode::vsshr2d, node, identityReg, identityReg, 4); /* Do signed shift right by 4bits to get 0xfff00000_00000000 */
         break;
      default:
         TR_ASSERT_FATAL_WITH_NODE(node, false, "unrecognized vector type %s", node->getFirstChild()->getDataType().toString());
         break;
      }
   }

TR::Register*
OMR::ARM64::TreeEvaluator::vmreductionMaxEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   TR::DataType et = node->getFirstChild()->getDataType().getVectorElementType();
   TR::InstOpCode::Mnemonic op = TR::InstOpCode::bad;
   reductionEvaluatorHelper evaluationHelper = NULL;
   switch(et)
      {
      case TR::Int8:
         op = TR::InstOpCode::vsmaxv16b;
         break;
      case TR::Int16:
         op = TR::InstOpCode::vsmaxv8h;
         break;
      case TR::Int32:
         op = TR::InstOpCode::vsmaxv4s;
         break;
      case TR::Int64:
         /* SMAXV does not accept 64bit elements */
         evaluationHelper = vreductionMaxInt64Helper;
         break;
      case TR::Float:
         op = TR::InstOpCode::vfmaxv4s;
         break;
      case TR::Double:
         op = TR::InstOpCode::fmaxp2d;
         break;
      default:
         TR_ASSERT_FATAL_WITH_NODE(node, false, "unrecognized vector type %s", node->getFirstChild()->getDataType().toString());
         return NULL;
      }
   return inlineVectorMaskedReductionOp(node, cg, et, op, loadIdentityVectorForReductionMax, evaluationHelper);
   }

/**
 * @brief A helper function for loading identity vector for reduction min
 *
 * @param[in] node: node
 * @param[in] et: element type
 * @param[in] identityReg: register to load identity vector
 * @param[in] cg: CodeGenerator
 */
static void
loadIdentityVectorForReductionMin(TR::Node *node, TR::DataType et, TR::Register *identityReg, TR::CodeGenerator *cg)
   {
   switch(et)
      {
      case TR::Int8:
         generateTrg1ImmInstruction(cg, TR::InstOpCode::vmovi16b, node, identityReg, 0x7f);
         break;
      case TR::Int16:
         generateTrg1ImmShiftedInstruction(cg, TR::InstOpCode::vmvni8h, node, identityReg, 0x80, 8);
         break;
      case TR::Int32:
         generateTrg1ImmShiftedInstruction(cg, TR::InstOpCode::vmvni4s, node, identityReg, 0x80, 24);
         break;
      case TR::Int64:
         generateTrg1ImmInstruction(cg, TR::InstOpCode::vmovi16b, node, identityReg, 0xff);
         generateVectorShiftImmediateInstruction(cg, TR::InstOpCode::vushr2d, node, identityReg, identityReg, 1);
         break;
      case TR::Float:
         /* Positive Infinity */
         generateTrg1ImmShiftedInstruction(cg, TR::InstOpCode::vmovi4s, node, identityReg, 0x7f, 24);
         generateTrg1ImmShiftedInstruction(cg, TR::InstOpCode::vorrimm4s, node, identityReg, 0x80, 16);
         break;
      case TR::Double:
         /* Positive Infinity */
         generateTrg1ImmInstruction(cg, TR::InstOpCode::vmovi2d, node, identityReg, 0x80); /* Loads 0xff000000_00000000 into each element */
         generateVectorShiftImmediateInstruction(cg, TR::InstOpCode::vsshr2d, node, identityReg, identityReg, 4); /* Do signed shift right by 4bits to get 0xfff00000_00000000 */
         generateTrg1ImmShiftedInstruction(cg, TR::InstOpCode::vbicimm4s, node, identityReg, 0x80, 24); /* Clear sign bit. We do not have bic for 64bit integer, but vbicimm4s works for this case. */
         break;
      default:
         TR_ASSERT_FATAL_WITH_NODE(node, false, "unrecognized vector type %s", node->getFirstChild()->getDataType().toString());
         break;
      }
   }

TR::Register*
OMR::ARM64::TreeEvaluator::vmreductionMinEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   TR_ASSERT_FATAL_WITH_NODE(node, node->getFirstChild()->getDataType().getVectorLength() == TR::VectorLength128,
                   "Only 128-bit vectors are supported %s", node->getFirstChild()->getDataType().toString());

   TR::DataType et = node->getFirstChild()->getDataType().getVectorElementType();
   TR::InstOpCode::Mnemonic op = TR::InstOpCode::bad;
   reductionEvaluatorHelper evaluationHelper = NULL;
   switch(et)
      {
      case TR::Int8:
         op = TR::InstOpCode::vsminv16b;
         break;
      case TR::Int16:
         op = TR::InstOpCode::vsminv8h;
         break;
      case TR::Int32:
         op = TR::InstOpCode::vsminv4s;
         break;
      case TR::Int64:
         /* SMINV does not accept 64bit elements */
         evaluationHelper = vreductionMinInt64Helper;
         break;
      case TR::Float:
         op = TR::InstOpCode::vfminv4s;
         break;
      case TR::Double:
         op = TR::InstOpCode::fminp2d;
         break;
      default:
         TR_ASSERT_FATAL_WITH_NODE(node, false, "unrecognized vector type %s", node->getFirstChild()->getDataType().toString());
         return NULL;
      }
   return inlineVectorMaskedReductionOp(node, cg, et, op, loadIdentityVectorForReductionMin, evaluationHelper);
   }

/**
 * @brief A helper function for loading identity vector for reduction mul
 *
 * @param[in] node: node
 * @param[in] et: element type
 * @param[in] identityReg: register to load identity vector
 * @param[in] cg: CodeGenerator
 */
static void
loadIdentityVectorForReductionMul(TR::Node *node, TR::DataType et, TR::Register *identityReg, TR::CodeGenerator *cg)
   {
   switch(et)
      {
      case TR::Int8:
         generateTrg1ImmInstruction(cg, TR::InstOpCode::vmovi16b, node, identityReg, 1);
         break;
      case TR::Int16:
         generateTrg1ImmInstruction(cg, TR::InstOpCode::vmovi8h, node, identityReg, 1);
         break;
      case TR::Int32:
         generateTrg1ImmInstruction(cg, TR::InstOpCode::vmovi4s, node, identityReg, 1);
         break;
      case TR::Int64:
         generateTrg1ImmInstruction(cg, TR::InstOpCode::vmovi2d, node, identityReg, 1); /* Loads 0x00000000_000000ff into each element */
         generateTrg1ImmInstruction(cg, TR::InstOpCode::vbicimm4s, node, identityReg, 0xfe); /* Clear unneeded bits. We do not have bic for 64bit integer, but vbicimm4s works for this case. */
         break;
      case TR::Float:
         generateTrg1ImmInstruction(cg, TR::InstOpCode::vfmov4s, node, identityReg, 0x70); /* Loads 0x3f800000 (1.0f) into each element */
         break;
      case TR::Double:
         generateTrg1ImmInstruction(cg, TR::InstOpCode::vfmov2d, node, identityReg, 0x70); /* Loads 0x3ff00000_00000000 (1.0) into each element */
         break;
      default:
         TR_ASSERT_FATAL_WITH_NODE(node, false, "unrecognized vector type %s", node->getFirstChild()->getDataType().toString());
         break;
      }
   }

TR::Register*
OMR::ARM64::TreeEvaluator::vmreductionMulEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   TR_ASSERT_FATAL_WITH_NODE(node, node->getFirstChild()->getDataType().getVectorLength() == TR::VectorLength128,
                   "Only 128-bit vectors are supported %s", node->getFirstChild()->getDataType().toString());

   TR::DataType et = node->getFirstChild()->getDataType().getVectorElementType();
   return inlineVectorMaskedReductionOp(node, cg, et, TR::InstOpCode::bad, loadIdentityVectorForReductionMul, vreductionMulHelper);
   }

TR::Register*
OMR::ARM64::TreeEvaluator::vmreductionOrEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   TR_ASSERT_FATAL_WITH_NODE(node, node->getFirstChild()->getDataType().getVectorLength() == TR::VectorLength128,
                   "Only 128-bit vectors are supported %s", node->getFirstChild()->getDataType().toString());

   TR::DataType et = node->getFirstChild()->getDataType().getVectorElementType();
   switch(et)
      {
      case TR::Int8:
      case TR::Int16:
      case TR::Int32:
      case TR::Int64:
         return inlineVectorMaskedReductionOp(node, cg, et, TR::InstOpCode::bad, loadZeroVector, vreductionOrHelper);
      case TR::Float:
      case TR::Double:
         TR_ASSERT_FATAL_WITH_NODE(node, false, "Unexpected element type %s", node->getFirstChild()->getDataType().toString());
         return NULL;
      default:
         TR_ASSERT_FATAL_WITH_NODE(node, false, "unrecognized vector type %s", node->getFirstChild()->getDataType().toString());
         return NULL;
      }
   }

TR::Register*
OMR::ARM64::TreeEvaluator::vmreductionOrUncheckedEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   return TR::TreeEvaluator::unImpOpEvaluator(node, cg);
   }

TR::Register*
OMR::ARM64::TreeEvaluator::vmreductionXorEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   TR_ASSERT_FATAL_WITH_NODE(node, node->getFirstChild()->getDataType().getVectorLength() == TR::VectorLength128,
                   "Only 128-bit vectors are supported %s", node->getFirstChild()->getDataType().toString());

   TR::DataType et = node->getFirstChild()->getDataType().getVectorElementType();
   switch(et)
      {
      case TR::Int8:
      case TR::Int16:
      case TR::Int32:
      case TR::Int64:
         return inlineVectorMaskedReductionOp(node, cg, et, TR::InstOpCode::bad, loadZeroVector, vreductionXorHelper);
      case TR::Float:
      case TR::Double:
         TR_ASSERT_FATAL_WITH_NODE(node, false, "Unexpected element type %s", node->getFirstChild()->getDataType().toString());
         return NULL;
      default:
         TR_ASSERT_FATAL_WITH_NODE(node, false, "unrecognized vector type %s", node->getFirstChild()->getDataType().toString());
         return NULL;
      }
   }

TR::Register*
OMR::ARM64::TreeEvaluator::vmsqrtEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   TR_ASSERT_FATAL_WITH_NODE(node, node->getDataType().getVectorLength() == TR::VectorLength128,
                           "Only 128-bit vectors are supported %s", node->getDataType().toString());
   TR::InstOpCode::Mnemonic sqrtOp;

   switch(node->getDataType().getVectorElementType())
      {
      case TR::Float:
         sqrtOp = TR::InstOpCode::vfsqrt4s;
         break;
      case TR::Double:
         sqrtOp = TR::InstOpCode::vfsqrt2d;
         break;
      default:
         TR_ASSERT_FATAL(false, "unrecognized vector type %s", node->getDataType().toString());
         return NULL;
      }
   return inlineVectorMaskedUnaryOp(node, cg, sqrtOp);
   }

TR::Register*
OMR::ARM64::TreeEvaluator::vmstoreiEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   return TR::TreeEvaluator::unImpOpEvaluator(node, cg);
   }

TR::Register*
OMR::ARM64::TreeEvaluator::vmsubEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   TR::InstOpCode::Mnemonic op;

   switch (node->getDataType().getVectorElementType())
      {
      case TR::Int8:
         op = TR::InstOpCode::vsub16b;
         break;
      case TR::Int16:
         op = TR::InstOpCode::vsub8h;
         break;
      case TR::Int32:
         op = TR::InstOpCode::vsub4s;
         break;
      case TR::Int64:
         op = TR::InstOpCode::vsub2d;
         break;
      case TR::Float:
         op = TR::InstOpCode::vfsub4s;
         break;
      case TR::Double:
         op = TR::InstOpCode::vfsub2d;
         break;
      default:
         TR_ASSERT_FATAL_WITH_NODE(node, false, "unrecognized vector type %s", node->getDataType().toString());
         return NULL;
      }
   return inlineVectorMaskedBinaryOp(node, cg, op);
   }

TR::Register*
OMR::ARM64::TreeEvaluator::vmxorEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   TR::InstOpCode::Mnemonic op;

   switch (node->getDataType().getVectorElementType())
      {
      case TR::Int8:
      case TR::Int16:
      case TR::Int32:
      case TR::Int64:
         op = TR::InstOpCode::veor16b;
         break;

      default:
         TR_ASSERT_FATAL_WITH_NODE(node, false, "unrecognized vector type %s", node->getDataType().toString());
         return NULL;
      }
   return inlineVectorMaskedBinaryOp(node, cg, op);
   }

TR::Register*
OMR::ARM64::TreeEvaluator::vmfirstNonZeroEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   return TR::TreeEvaluator::unImpOpEvaluator(node, cg);
   }

TR::Register*
OMR::ARM64::TreeEvaluator::f2iuEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   return TR::TreeEvaluator::unImpOpEvaluator(node, cg);
   }

TR::Register*
OMR::ARM64::TreeEvaluator::f2luEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   return TR::TreeEvaluator::unImpOpEvaluator(node, cg);
   }

TR::Register*
OMR::ARM64::TreeEvaluator::f2buEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   return TR::TreeEvaluator::unImpOpEvaluator(node, cg);
   }

TR::Register*
OMR::ARM64::TreeEvaluator::f2cEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   return TR::TreeEvaluator::unImpOpEvaluator(node, cg);
   }

TR::Register*
OMR::ARM64::TreeEvaluator::d2iuEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   return TR::TreeEvaluator::unImpOpEvaluator(node, cg);
   }

TR::Register*
OMR::ARM64::TreeEvaluator::d2luEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   return TR::TreeEvaluator::unImpOpEvaluator(node, cg);
   }

TR::Register*
OMR::ARM64::TreeEvaluator::d2buEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   return TR::TreeEvaluator::unImpOpEvaluator(node, cg);
   }

TR::Register*
OMR::ARM64::TreeEvaluator::monexitfenceEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   return TR::TreeEvaluator::unImpOpEvaluator(node, cg);
   }

TR::Register*
OMR::ARM64::TreeEvaluator::tstartEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   return TR::TreeEvaluator::unImpOpEvaluator(node, cg);
   }

TR::Register*
OMR::ARM64::TreeEvaluator::tfinishEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   return TR::TreeEvaluator::unImpOpEvaluator(node, cg);
   }

TR::Register*
OMR::ARM64::TreeEvaluator::tabortEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   return TR::TreeEvaluator::unImpOpEvaluator(node, cg);
   }

TR::Register*
OMR::ARM64::TreeEvaluator::NewEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   return TR::TreeEvaluator::unImpOpEvaluator(node, cg);
   }

TR::Register*
OMR::ARM64::TreeEvaluator::newvalueEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   return TR::TreeEvaluator::unImpOpEvaluator(node, cg);
   }

TR::Register*
OMR::ARM64::TreeEvaluator::newarrayEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   return TR::TreeEvaluator::unImpOpEvaluator(node, cg);
   }

TR::Register*
OMR::ARM64::TreeEvaluator::anewarrayEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   return TR::TreeEvaluator::unImpOpEvaluator(node, cg);
   }

TR::Register*
OMR::ARM64::TreeEvaluator::variableNewEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   return TR::TreeEvaluator::unImpOpEvaluator(node, cg);
   }

TR::Register*
OMR::ARM64::TreeEvaluator::variableNewArrayEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   return TR::TreeEvaluator::unImpOpEvaluator(node, cg);
   }

TR::Register*
OMR::ARM64::TreeEvaluator::multianewarrayEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   return TR::TreeEvaluator::unImpOpEvaluator(node, cg);
   }

TR::Register*
OMR::ARM64::TreeEvaluator::arraylengthEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   return TR::TreeEvaluator::unImpOpEvaluator(node, cg);
   }

TR::Register*
OMR::ARM64::TreeEvaluator::contigarraylengthEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   return TR::TreeEvaluator::unImpOpEvaluator(node, cg);
   }

TR::Register*
OMR::ARM64::TreeEvaluator::discontigarraylengthEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   return TR::TreeEvaluator::unImpOpEvaluator(node, cg);
   }

TR::Register*
OMR::ARM64::TreeEvaluator::icalliEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   return TR::TreeEvaluator::indirectCallEvaluator(node, cg);
   }

TR::Register*
OMR::ARM64::TreeEvaluator::lcalliEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   return TR::TreeEvaluator::indirectCallEvaluator(node, cg);
   }

TR::Register*
OMR::ARM64::TreeEvaluator::fcalliEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   return TR::TreeEvaluator::indirectCallEvaluator(node, cg);
   }

TR::Register*
OMR::ARM64::TreeEvaluator::dcalliEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   return TR::TreeEvaluator::indirectCallEvaluator(node, cg);
   }

TR::Register*
OMR::ARM64::TreeEvaluator::acalliEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   return TR::TreeEvaluator::indirectCallEvaluator(node, cg);
   }

TR::Register*
OMR::ARM64::TreeEvaluator::calliEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   return TR::TreeEvaluator::indirectCallEvaluator(node, cg);
   }

TR::Register*
OMR::ARM64::TreeEvaluator::fenceEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   return TR::TreeEvaluator::unImpOpEvaluator(node, cg);
   }

TR::Register*
OMR::ARM64::TreeEvaluator::luaddhEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   return TR::TreeEvaluator::unImpOpEvaluator(node, cg);
   }

TR::Register*
OMR::ARM64::TreeEvaluator::aiaddEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   return TR::TreeEvaluator::unImpOpEvaluator(node, cg);
   }

TR::Register*
OMR::ARM64::TreeEvaluator::lusubhEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   return TR::TreeEvaluator::unImpOpEvaluator(node, cg);
   }

TR::Register*
OMR::ARM64::TreeEvaluator::iumulhEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   return TR::TreeEvaluator::unImpOpEvaluator(node, cg);
   }

TR::Register*
OMR::ARM64::TreeEvaluator::lumulhEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   return TR::TreeEvaluator::unImpOpEvaluator(node, cg);
   }

TR::Register*
OMR::ARM64::TreeEvaluator::CaseEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   return TR::TreeEvaluator::unImpOpEvaluator(node, cg);
   }

TR::Register*
OMR::ARM64::TreeEvaluator::NULLCHKEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   return TR::TreeEvaluator::badILOpEvaluator(node, cg);
   }

TR::Register*
OMR::ARM64::TreeEvaluator::ResolveCHKEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   return TR::TreeEvaluator::unImpOpEvaluator(node, cg);
   }

TR::Register*
OMR::ARM64::TreeEvaluator::ResolveAndNULLCHKEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   return TR::TreeEvaluator::badILOpEvaluator(node, cg);
   }

TR::Register*
OMR::ARM64::TreeEvaluator::DIVCHKEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   return TR::TreeEvaluator::unImpOpEvaluator(node, cg);
   }

TR::Register*
OMR::ARM64::TreeEvaluator::OverflowCHKEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   return TR::TreeEvaluator::unImpOpEvaluator(node, cg);
   }

TR::Register*
OMR::ARM64::TreeEvaluator::UnsignedOverflowCHKEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   return TR::TreeEvaluator::unImpOpEvaluator(node, cg);
   }

TR::Register*
OMR::ARM64::TreeEvaluator::BNDCHKEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   return TR::TreeEvaluator::unImpOpEvaluator(node, cg);
   }

static TR::Instruction *compareIntsAndBranchForArrayCopyBNDCHK(TR::ARM64ConditionCode  branchCond,
                                           TR::Node             *node,
                                           TR::CodeGenerator    *cg,
                                           TR::SymbolReference  *sr)
   {
   TR::Node *firstChild = node->getFirstChild();
   TR::Node *secondChild = node->getSecondChild();
   TR::LabelSymbol *snippetLabel = generateLabelSymbol(cg);
   TR::Register *src1Reg;
   TR::Register *src2Reg;

   if (firstChild->getOpCodeValue() == TR::isub &&
       firstChild->getRegister() == NULL &&
       firstChild->getReferenceCount() == 1 &&
       secondChild->getOpCode().isLoadConst() &&
       secondChild->getInt() == 0)
      {
      TR::Node *sub1Child = firstChild->getFirstChild();
      TR::Node *sub2Child = firstChild->getSecondChild();
      src1Reg = cg->evaluate(sub1Child);
      src2Reg = cg->evaluate(sub2Child);
      generateCompareInstruction(cg, node, src1Reg, src2Reg); // 32-bit comparison
      cg->decReferenceCount(sub1Child);
      cg->decReferenceCount(sub2Child);
      }
   else
      {
      src1Reg = cg->evaluate(firstChild);

      bool foundConst = false;
      if (secondChild->getOpCode().isLoadConst())
         {
         int64_t value = static_cast<int64_t>(secondChild->getInt());
         if (constantIsUnsignedImm12(value))
            {
            generateCompareImmInstruction(cg, node, src1Reg, value); // 32-bit comparison
            foundConst = true;
            }
         else if (constantIsUnsignedImm12(-value))
            {
            generateCompareImmInstruction(cg, node, src1Reg, value); // 32-bit comparison
            foundConst = true;
            }
         }
      if (!foundConst)
         {
         src2Reg = cg->evaluate(secondChild);
         generateCompareInstruction(cg, node, src1Reg, src2Reg); // 32-bit comparison
         }
      }

   TR_ASSERT_FATAL_WITH_NODE(node, sr, "Must provide an ArrayCopyBNDCHK symref");
   cg->addSnippet(new (cg->trHeapMemory()) TR::ARM64HelperCallSnippet(cg, node, snippetLabel, sr));
   TR::Instruction *instr = generateConditionalBranchInstruction(cg, TR::InstOpCode::b_cond, node, snippetLabel, branchCond);

   cg->machine()->setLinkRegisterKilled(true);
   cg->decReferenceCount(firstChild);
   cg->decReferenceCount(secondChild);
   return instr;
   }

TR::Register*
OMR::ARM64::TreeEvaluator::ArrayCopyBNDCHKEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   // check that child[0] >= child[1], if not branch to check failure
   // If the first child is a constant and the second isn't, swap the children

   TR::Node *firstChild = node->getFirstChild();
   TR::Node *secondChild = node->getSecondChild();
   TR::SymbolReference *exceptionBNDCHK = node->getSymbolReference();
   TR::Instruction *instr = NULL;

   if (firstChild->getOpCode().isLoadConst())
      {
      if (secondChild->getOpCode().isLoadConst())
         {
         if (firstChild->getInt() < secondChild->getInt())
            {
            // Check will always fail, just jump to the exception handler
            instr = generateImmSymInstruction(cg, TR::InstOpCode::bl, node, (uintptr_t)exceptionBNDCHK->getMethodAddress(), NULL, exceptionBNDCHK, NULL);
            cg->machine()->setLinkRegisterKilled(true);
            }
         else
            {
            // Check will always succeed, no need for an instruction
            }
         cg->decReferenceCount(firstChild);
         cg->decReferenceCount(secondChild);
         }
      else
         {
         node->swapChildren();
         instr = compareIntsAndBranchForArrayCopyBNDCHK(TR::CC_GT, node, cg, exceptionBNDCHK);
         node->swapChildren();
         }
      }
   else
      {
      instr = compareIntsAndBranchForArrayCopyBNDCHK(TR::CC_LT, node, cg, exceptionBNDCHK);
      }

   if (instr)
      {
      instr->ARM64NeedsGCMap(cg, 0xFFFFFFFF);
      }

   return NULL;
   }

TR::Register*
OMR::ARM64::TreeEvaluator::BNDCHKwithSpineCHKEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   return TR::TreeEvaluator::unImpOpEvaluator(node, cg);
   }

TR::Register*
OMR::ARM64::TreeEvaluator::SpineCHKEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   return TR::TreeEvaluator::unImpOpEvaluator(node, cg);
   }

TR::Register*
OMR::ARM64::TreeEvaluator::ArrayStoreCHKEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   return TR::TreeEvaluator::unImpOpEvaluator(node, cg);
   }

TR::Register*
OMR::ARM64::TreeEvaluator::ArrayCHKEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   return TR::TreeEvaluator::unImpOpEvaluator(node, cg);
   }

TR::Register*
OMR::ARM64::TreeEvaluator::long2StringEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   return TR::TreeEvaluator::unImpOpEvaluator(node, cg);
   }

TR::Register*
OMR::ARM64::TreeEvaluator::bitOpMemEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   return TR::TreeEvaluator::unImpOpEvaluator(node, cg);
   }

TR::Register*
OMR::ARM64::TreeEvaluator::allocationFenceEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   return TR::TreeEvaluator::unImpOpEvaluator(node, cg);
   }

TR::Register*
OMR::ARM64::TreeEvaluator::loadFenceEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   return TR::TreeEvaluator::unImpOpEvaluator(node, cg);
   }

TR::Register*
OMR::ARM64::TreeEvaluator::storeFenceEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   return TR::TreeEvaluator::unImpOpEvaluator(node, cg);
   }

TR::Register*
OMR::ARM64::TreeEvaluator::fullFenceEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   return TR::TreeEvaluator::unImpOpEvaluator(node, cg);
   }

TR::Register*
OMR::ARM64::TreeEvaluator::computeCCEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   return TR::TreeEvaluator::unImpOpEvaluator(node, cg);
   }

TR::Register*
OMR::ARM64::TreeEvaluator::butestEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   return TR::TreeEvaluator::unImpOpEvaluator(node, cg);
   }

TR::Register*
OMR::ARM64::TreeEvaluator::sutestEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   return TR::TreeEvaluator::unImpOpEvaluator(node, cg);
   }

TR::Register*
OMR::ARM64::TreeEvaluator::bucmpEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   return TR::TreeEvaluator::unImpOpEvaluator(node, cg);
   }

TR::Register*
OMR::ARM64::TreeEvaluator::bcmpEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   return TR::TreeEvaluator::unImpOpEvaluator(node, cg);
   }

TR::Register*
OMR::ARM64::TreeEvaluator::sucmpEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   return TR::TreeEvaluator::unImpOpEvaluator(node, cg);
   }

TR::Register*
OMR::ARM64::TreeEvaluator::scmpEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   return TR::TreeEvaluator::unImpOpEvaluator(node, cg);
   }

TR::Register*
OMR::ARM64::TreeEvaluator::iucmpEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   return TR::TreeEvaluator::unImpOpEvaluator(node, cg);
   }

TR::Register*
OMR::ARM64::TreeEvaluator::icmpEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   return TR::TreeEvaluator::unImpOpEvaluator(node, cg);
   }

TR::Register*
OMR::ARM64::TreeEvaluator::lucmpEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   return TR::TreeEvaluator::unImpOpEvaluator(node, cg);
   }

TR::Register*
OMR::ARM64::TreeEvaluator::ificmpoEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   return TR::TreeEvaluator::unImpOpEvaluator(node, cg);
   }

TR::Register*
OMR::ARM64::TreeEvaluator::ificmpnoEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   return TR::TreeEvaluator::unImpOpEvaluator(node, cg);
   }

TR::Register*
OMR::ARM64::TreeEvaluator::iflcmpoEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   return TR::TreeEvaluator::unImpOpEvaluator(node, cg);
   }

TR::Register*
OMR::ARM64::TreeEvaluator::iflcmpnoEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   return TR::TreeEvaluator::unImpOpEvaluator(node, cg);
   }

TR::Register*
OMR::ARM64::TreeEvaluator::ificmnoEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   return TR::TreeEvaluator::unImpOpEvaluator(node, cg);
   }

TR::Register*
OMR::ARM64::TreeEvaluator::ificmnnoEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   return TR::TreeEvaluator::unImpOpEvaluator(node, cg);
   }

TR::Register*
OMR::ARM64::TreeEvaluator::iflcmnoEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   return TR::TreeEvaluator::unImpOpEvaluator(node, cg);
   }

TR::Register*
OMR::ARM64::TreeEvaluator::iflcmnnoEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   return TR::TreeEvaluator::unImpOpEvaluator(node, cg);
   }

TR::Register*
OMR::ARM64::TreeEvaluator::iuaddcEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   return TR::TreeEvaluator::unImpOpEvaluator(node, cg);
   }

TR::Register*
OMR::ARM64::TreeEvaluator::luaddcEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   return TR::TreeEvaluator::unImpOpEvaluator(node, cg);
   }

TR::Register*
OMR::ARM64::TreeEvaluator::iusubbEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   return TR::TreeEvaluator::unImpOpEvaluator(node, cg);
   }

TR::Register*
OMR::ARM64::TreeEvaluator::lusubbEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   return TR::TreeEvaluator::unImpOpEvaluator(node, cg);
   }

TR::Register*
OMR::ARM64::TreeEvaluator::icmpsetEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   return TR::TreeEvaluator::unImpOpEvaluator(node, cg);
   }

TR::Register*
OMR::ARM64::TreeEvaluator::lcmpsetEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   return TR::TreeEvaluator::unImpOpEvaluator(node, cg);
   }

TR::Register*
OMR::ARM64::TreeEvaluator::bztestnsetEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   return TR::TreeEvaluator::unImpOpEvaluator(node, cg);
   }

TR::Register*
OMR::ARM64::TreeEvaluator::ibatomicorEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   return TR::TreeEvaluator::unImpOpEvaluator(node, cg);
   }

TR::Register*
OMR::ARM64::TreeEvaluator::isatomicorEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   return TR::TreeEvaluator::unImpOpEvaluator(node, cg);
   }

TR::Register*
OMR::ARM64::TreeEvaluator::iiatomicorEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   return TR::TreeEvaluator::unImpOpEvaluator(node, cg);
   }

TR::Register*
OMR::ARM64::TreeEvaluator::ilatomicorEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   return TR::TreeEvaluator::unImpOpEvaluator(node, cg);
   }

TR::Register*
OMR::ARM64::TreeEvaluator::branchEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   return TR::TreeEvaluator::unImpOpEvaluator(node, cg);
   }

TR::Register*
OMR::ARM64::TreeEvaluator::dfloorEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   return TR::TreeEvaluator::unImpOpEvaluator(node, cg);
   }

TR::Register*
OMR::ARM64::TreeEvaluator::ffloorEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   return TR::TreeEvaluator::unImpOpEvaluator(node, cg);
   }

TR::Register*
OMR::ARM64::TreeEvaluator::dceilEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   return TR::TreeEvaluator::unImpOpEvaluator(node, cg);
   }

TR::Register*
OMR::ARM64::TreeEvaluator::fceilEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   return TR::TreeEvaluator::unImpOpEvaluator(node, cg);
   }

TR::Register*
OMR::ARM64::TreeEvaluator::iumaxEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   return TR::TreeEvaluator::unImpOpEvaluator(node, cg);
   }

TR::Register*
OMR::ARM64::TreeEvaluator::lumaxEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   return TR::TreeEvaluator::unImpOpEvaluator(node, cg);
   }

TR::Register*
OMR::ARM64::TreeEvaluator::iuminEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   return TR::TreeEvaluator::unImpOpEvaluator(node, cg);
   }

TR::Register*
OMR::ARM64::TreeEvaluator::luminEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   return TR::TreeEvaluator::unImpOpEvaluator(node, cg);
   }

TR::Register*
OMR::ARM64::TreeEvaluator::ipopcntEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   return TR::TreeEvaluator::unImpOpEvaluator(node, cg);
   }

TR::Register*
OMR::ARM64::TreeEvaluator::lpopcntEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   return TR::TreeEvaluator::unImpOpEvaluator(node, cg);
   }

TR::Register*
OMR::ARM64::TreeEvaluator::bbitpermuteEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   return TR::TreeEvaluator::unImpOpEvaluator(node, cg);
   }

TR::Register*
OMR::ARM64::TreeEvaluator::sbitpermuteEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   return TR::TreeEvaluator::unImpOpEvaluator(node, cg);
   }

TR::Register*
OMR::ARM64::TreeEvaluator::ibitpermuteEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   return TR::TreeEvaluator::unImpOpEvaluator(node, cg);
   }

TR::Register*
OMR::ARM64::TreeEvaluator::lbitpermuteEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   return TR::TreeEvaluator::unImpOpEvaluator(node, cg);
   }

TR::Instruction *loadAddressConstantInSnippet(TR::CodeGenerator *cg, TR::Node *node, intptr_t address, TR::Register *targetRegister, TR_ExternalRelocationTargetKind reloKind, bool isClassUnloadingConst, TR::Instruction *cursor)
   {
   TR::Compilation *comp = cg->comp();
   // We use LDR literal to load a value from the snippet. Offset to PC will be patched by LabelRelative24BitRelocation
   auto snippet = cg->findOrCreate8ByteConstant(node, address);
   auto labelSym = snippet->getSnippetLabel();
   snippet->setReloType(reloKind);

   if (isClassUnloadingConst)
      {
      if (node->isMethodPointerConstant())
         {
         cg->getMethodSnippetsToBePatchedOnClassUnload()->push_front(snippet);
         }
      else
         {
         cg->getSnippetsToBePatchedOnClassUnload()->push_front(snippet);
         }
      }
   return generateTrg1ImmSymInstruction(cg, TR::InstOpCode::ldrx, node, targetRegister, 0, labelSym, cursor);
   }

bool shouldLoadNegatedConstant32(int32_t value)
   {
   int32_t negatedValue = -value;
   if ((value >= -65535 && value <= 65535) ||
       ((value & 0xFFFF) == 0) ||
       ((value & 0xFFFF) == 0xFFFF))
      {
      return false;
      }
   else if ((negatedValue >= -65535 && negatedValue <= 65535) ||
       ((negatedValue & 0xFFFF) == 0) ||
       ((negatedValue & 0xFFFF) == 0xFFFF))
      {
      return true;
      }
   else
      {
      bool n;
      uint32_t immEncoded;
      if (logicImmediateHelper(static_cast<uint32_t>(value), false, n, immEncoded))
         {
         return false;
         }
      else if (logicImmediateHelper(static_cast<uint32_t>(negatedValue), false, n, immEncoded))
         {
         return true;
         }
      return false;
      }
   }

/**
 * @brief Helper function for analyzing instructions required for loading 64bit constant value.
 *        This functions returns a pair of number of instructions required and a bool flag.
 *        If the bool flag is true, movz instruction should be used. Otherwise, movz should be used.
 *
 * @param[out] h : 4 elements array of 16bit integer
 * @param[in] value: 64bit value to load
 *
 * @return a pair of number of instructions required and a bool flag
 */
static
std::pair<int32_t, bool> analyzeLoadConstant64(uint16_t h[4], int64_t value)
   {
   int32_t count0000 = 0, countFFFF = 0;
   int32_t i;

   for (i = 0; i < 4; i++)
      {
      h[i] = (value >> (i * 16)) & 0xFFFF;
      if (h[i] == 0)
         {
         count0000++;
         }
      else if (h[i] == 0xFFFF)
         {
         countFFFF++;
         }
      }

   return std::make_pair(4 - std::max(count0000, countFFFF), count0000 >= countFFFF);
   }

bool shouldLoadNegatedConstant64(int64_t value)
   {
   int64_t negatedValue = -value;
   // If upper 48bit of value is all 0 or value is -1
   if (((value & (~static_cast<int64_t>(0xffff))) == 0) || (~value == 0LL))
      {
      return false;
      }
   else if ((negatedValue & (~static_cast<int64_t>(0xffff))) == 0)
      {
      return true;
      }
   uint16_t h[4];

   auto numInstrAndUseMovz = analyzeLoadConstant64(h, value);
   if (numInstrAndUseMovz.first == 1)
      {
      return false;
      }

   auto numInstrAndUseMovzNeg = analyzeLoadConstant64(h, negatedValue);
   if (numInstrAndUseMovzNeg.first == 1)
      {
      return true;
      }

   bool n;
   uint32_t immEncoded;
   if (logicImmediateHelper(value, true, n, immEncoded))
      {
      return false;
      }
   else if (logicImmediateHelper(negatedValue, true, n, immEncoded))
      {
      return true;
      }

   return numInstrAndUseMovzNeg.first < numInstrAndUseMovz.first;
   }

TR::Instruction *loadConstant32(TR::CodeGenerator *cg, TR::Node *node, int32_t value, TR::Register *trgReg, TR::Instruction *cursor)
   {
   TR::Instruction *insertingInstructions = cursor;
   if (cursor == NULL)
      cursor = cg->getAppendInstruction();

   TR::InstOpCode::Mnemonic op = TR::InstOpCode::bad;
   uint32_t imm;

   if (value >= 0 && value <= 65535)
      {
      op = TR::InstOpCode::movzw;
      imm = value & 0xFFFF;
      }
   else if (value >= -65535 && value < 0)
      {
      op = TR::InstOpCode::movnw;
      imm = ~value & 0xFFFF;
      }
   else if ((value & 0xFFFF) == 0)
      {
      op = TR::InstOpCode::movzw;
      imm = ((value >> 16) & 0xFFFF) | TR::MOV_LSL16;
      }
   else if ((value & 0xFFFF) == 0xFFFF)
      {
      op = TR::InstOpCode::movnw;
      imm = ((~value >> 16) & 0xFFFF) | TR::MOV_LSL16;
      }

   if (op != TR::InstOpCode::bad)
      {
      cursor = generateTrg1ImmInstruction(cg, op, node, trgReg, imm, cursor);
      }
   else
      {
      bool n;
      uint32_t immEncoded;
      if (logicImmediateHelper(static_cast<uint32_t>(value), false, n, immEncoded))
         {
         cursor = generateMovBitMaskInstruction(cg, node, trgReg, n, immEncoded, false, cursor);
         }
      else
         {
         // need two instructions
         cursor = generateTrg1ImmInstruction(cg, TR::InstOpCode::movzw, node, trgReg,
                                             (value & 0xFFFF), cursor);
         cursor = generateTrg1ImmInstruction(cg, TR::InstOpCode::movkw, node, trgReg,
                                             (((value >> 16) & 0xFFFF) | TR::MOV_LSL16), cursor);
         }
      }

   if (!insertingInstructions)
      cg->setAppendInstruction(cursor);

   return cursor;
   }

TR::Instruction *loadConstant64(TR::CodeGenerator *cg, TR::Node *node, int64_t value, TR::Register *trgReg, TR::Instruction *cursor)
   {
   TR::Instruction *insertingInstructions = cursor;
   if (cursor == NULL)
      cursor = cg->getAppendInstruction();

   if (value == 0LL)
      {
      // 0
      cursor = generateTrg1ImmInstruction(cg, TR::InstOpCode::movzx, node, trgReg, 0, cursor);
      }
   else if (~value == 0LL)
      {
      // -1
      cursor = generateTrg1ImmInstruction(cg, TR::InstOpCode::movnx, node, trgReg, 0, cursor);
      }
   else
      {
      uint16_t h[4];
      int32_t i;
      auto numInstrAndUseMovz = analyzeLoadConstant64(h, value);
      int32_t use_movz = numInstrAndUseMovz.second;

      bool n;
      uint32_t immEncoded;
      if ((numInstrAndUseMovz.first > 1) && logicImmediateHelper(value, true, n, immEncoded))
         {
         cursor = generateMovBitMaskInstruction(cg, node, trgReg, n, immEncoded, true, cursor);
         }
      else
         {
         TR::Instruction *start = cursor;

         for (i = 0; i < 4; i++)
            {
            uint32_t shift = TR::MOV_LSL16 * i;
            TR::InstOpCode::Mnemonic op = TR::InstOpCode::bad;
            uint32_t imm;

            if (use_movz && (h[i] != 0))
               {
               imm = h[i] | shift;
               if (cursor != start)
                  {
                  op = TR::InstOpCode::movkx;
                  }
               else
                  {
                  op = TR::InstOpCode::movzx;
                  }
               }
            else if (!use_movz && (h[i] != 0xFFFF))
               {
               if (cursor != start)
                  {
                  op = TR::InstOpCode::movkx;
                  imm = h[i] | shift;
                  }
               else
                  {
                  op = TR::InstOpCode::movnx;
                  imm = (~h[i] & 0xFFFF) | shift;
                  }
               }

            if (op != TR::InstOpCode::bad)
               {
               cursor = generateTrg1ImmInstruction(cg, op, node, trgReg, imm, cursor);
               }
            else
               {
               // generate no instruction here
               }
            }
         }
      }

   if (!insertingInstructions)
      cg->setAppendInstruction(cursor);

   return cursor;
   }

TR::Instruction *addConstant64(TR::CodeGenerator *cg, TR::Node *node, TR::Register *trgReg, TR::Register *srcReg, int64_t value)
   {
   TR::Instruction *cursor;

   if (constantIsUnsignedImm12(value))
      {
      cursor = generateTrg1Src1ImmInstruction(cg, TR::InstOpCode::addimmx, node, trgReg, srcReg, value);
      }
   else
      {
      TR::Register *tempReg = cg->allocateRegister();
      loadConstant64(cg, node, value, tempReg);
      cursor = generateTrg1Src2Instruction(cg, TR::InstOpCode::addx, node, trgReg, srcReg, tempReg);
      cg->stopUsingRegister(tempReg);
      }

   return cursor;
   }

TR::Instruction *addConstant32(TR::CodeGenerator *cg, TR::Node *node, TR::Register *trgReg, TR::Register *srcReg, int32_t value)
   {
   TR::Instruction *cursor;

   if (constantIsUnsignedImm12(value))
      {
      cursor = generateTrg1Src1ImmInstruction(cg, TR::InstOpCode::addimmw, node, trgReg, srcReg, value);
      }
   else
      {
      TR::Register *tempReg = cg->allocateRegister();
      loadConstant32(cg, node, value, tempReg);
      cursor = generateTrg1Src2Instruction(cg, TR::InstOpCode::addw, node, trgReg, srcReg, tempReg);
      cg->stopUsingRegister(tempReg);
      }

   return cursor;
   }

/**
 * Add meta data to instruction loading address constant
 * @param[in] cg : CodeGenerator
 * @param[in] node : node
 * @param[in] firstInstruction : instruction cursor
 * @param[in] typeAddress : type of address
 * @param[in] value : address value
 */
static void
addMetaDataForLoadAddressConstantFixed(TR::CodeGenerator *cg, TR::Node *node, TR::Instruction *firstInstruction, int16_t typeAddress, intptr_t value)
   {
   if (value == 0x0)
      return;

   if (typeAddress == -1)
      typeAddress = TR_FixedSequenceAddress2;

   TR::Compilation *comp = cg->comp();

   TR::Relocation *relo = NULL;

   switch (typeAddress)
      {
      case TR_DataAddress:
         {
         relo = new (cg->trHeapMemory()) TR::BeforeBinaryEncodingExternalRelocation(
            firstInstruction,
            (uint8_t *)node->getSymbolReference(),
            (uint8_t *)(intptr_t)node->getInlinedSiteIndex(),
            TR_DataAddress, cg);
         break;
         }

      case TR_DebugCounter:
         {
         TR::DebugCounterBase *counter = comp->getCounterFromStaticAddress(node->getSymbolReference());
         if (counter == NULL)
            comp->failCompilation<TR::CompilationException>("Could not generate relocation for debug counter in addMetaDataForLoadAddressConstantFixed\n");

         TR::DebugCounter::generateRelocation(comp, firstInstruction, node, counter);
         return;
         }

      case TR_ClassAddress:
         {
         if (comp->getOption(TR_UseSymbolValidationManager))
            {
            TR::SymbolReference *symRef = (TR::SymbolReference *)value;

            relo = new (cg->trHeapMemory()) TR::BeforeBinaryEncodingExternalRelocation(
               firstInstruction,
               (uint8_t *)symRef->getSymbol()->getStaticSymbol()->getStaticAddress(),
               (uint8_t *)TR::SymbolType::typeClass,
               TR_DiscontiguousSymbolFromManager, cg);
            }
         else
            {
            TR::SymbolReference *symRef = (TR::SymbolReference *)value;

            relo = new (cg->trHeapMemory()) TR::BeforeBinaryEncodingExternalRelocation(
               firstInstruction,
               (uint8_t *)symRef,
               (uint8_t *)(node == NULL ? -1 : (intptr_t)node->getInlinedSiteIndex()),
               TR_ClassAddress, cg);
            }
         break;
         }

      case TR_RamMethodSequence:
         {
         if (comp->getOption(TR_UseSymbolValidationManager))
            {
            relo = new (cg->trHeapMemory()) TR::BeforeBinaryEncodingExternalRelocation(
               firstInstruction,
               (uint8_t *)comp->getJittedMethodSymbol()->getResolvedMethod()->resolvedMethodAddress(),
               (uint8_t *)TR::SymbolType::typeMethod,
               TR_DiscontiguousSymbolFromManager,
               cg);
            }
         break;
         }
      }

   if (!relo)
      {
      relo = new (cg->trHeapMemory()) TR::BeforeBinaryEncodingExternalRelocation(
         firstInstruction,
         (uint8_t *)value,
         (TR_ExternalRelocationTargetKind)typeAddress,
         cg);
      }

   cg->addExternalRelocation(
      relo,
      __FILE__,
      __LINE__,
      node);
   }

/**
 * Generates relocatable instructions for loading 64-bit integer value to a register
 * @param[in] cg : CodeGenerator
 * @param[in] node : node
 * @param[in] value : integer value
 * @param[in] trgReg : target register
 * @param[in] cursor : instruction cursor
 * @param[in] typeAddress : type of address
 */
static TR::Instruction *
loadAddressConstantRelocatable(TR::CodeGenerator *cg, TR::Node *node, intptr_t value, TR::Register *trgReg, TR::Instruction *cursor=NULL, int16_t typeAddress = -1)
   {
   TR::Compilation *comp = cg->comp();
   // load a 64-bit constant into a register with a fixed 4 instruction sequence
   TR::Instruction *temp = cursor;
   TR::Instruction *firstInstruction;

   if (cursor == NULL)
      cursor = cg->getAppendInstruction();

   cursor = firstInstruction = generateTrg1ImmInstruction(cg, TR::InstOpCode::movzx, node, trgReg, value & 0x0000ffff, cursor);
   cursor = generateTrg1ImmInstruction(cg, TR::InstOpCode::movkx, node, trgReg, ((value >> 16) & 0x0000ffff) | TR::MOV_LSL16, cursor);
   cursor = generateTrg1ImmInstruction(cg, TR::InstOpCode::movkx, node, trgReg, ((value >> 32) & 0x0000ffff) | (TR::MOV_LSL16 * 2) , cursor);
   cursor = generateTrg1ImmInstruction(cg, TR::InstOpCode::movkx, node, trgReg, (value >> 48) | (TR::MOV_LSL16 * 3) , cursor);

   addMetaDataForLoadAddressConstantFixed(cg, node, firstInstruction, typeAddress, value);

   if (temp == NULL)
      cg->setAppendInstruction(cursor);

   return cursor;
   }

TR::Instruction *
loadAddressConstant(TR::CodeGenerator *cg, TR::Node *node, intptr_t value, TR::Register *trgReg, TR::Instruction *cursor, bool isPicSite, int16_t typeAddress)
   {
   if (cg->comp()->compileRelocatableCode())
      {
      return loadAddressConstantRelocatable(cg, node, value, trgReg, cursor, typeAddress);
      }

   return loadConstant64(cg, node, value, trgReg, cursor);
   }

TR::Register *
OMR::ARM64::TreeEvaluator::unImpOpEvaluator(TR::Node *node, TR::CodeGenerator *cg)
	{
	TR_ASSERT_FATAL(false, "Opcode %s is not implemented", node->getOpCode().getName());
	return NULL;
	}

TR::Register *
OMR::ARM64::TreeEvaluator::badILOpEvaluator(TR::Node *node, TR::CodeGenerator *cg)
	{
	// TODO:ARM64: Enable TR::TreeEvaluator::badILOpEvaluator in compiler/aarch64/codegen/TreeEvaluatorTable.hpp when Implemented.
	return OMR::ARM64::TreeEvaluator::unImpOpEvaluator(node, cg);
	}

TR::Register *commonLoadEvaluator(TR::Node *node, TR::InstOpCode::Mnemonic op, TR::CodeGenerator *cg)
   {
   TR::Register *tempReg;

   if (op == TR::InstOpCode::vldrimms)
      {
      tempReg = cg->allocateSinglePrecisionRegister();
      }
   else if (op == TR::InstOpCode::vldrimmd)
      {
      tempReg = cg->allocateRegister(TR_FPR);
      }
   else if (op == TR::InstOpCode::vldrimmq)
      {
      tempReg = cg->allocateRegister(TR_VRF);
      }
   else
      {
      tempReg = cg->allocateRegister();
      }

   return commonLoadEvaluator(node, op, tempReg, cg);
   }

TR::Register *commonLoadEvaluator(TR::Node *node, TR::InstOpCode::Mnemonic op, TR::Register *targetReg, TR::CodeGenerator *cg)
   {
   bool needSync = (node->getSymbolReference()->getSymbol()->isSyncVolatile() && cg->comp()->target().isSMP());

   node->setRegister(targetReg);
   TR::MemoryReference *tempMR = TR::MemoryReference::createWithRootLoadOrStore(cg, node);
   generateTrg1MemInstruction(cg, op, node, targetReg, tempMR);

   if (needSync)
      {
      generateSynchronizationInstruction(cg, TR::InstOpCode::dmb, node, 0x9); // dmb ishld
      }

   tempMR->decNodeReferenceCounts(cg);

   return targetReg;
   }

// also handles iloadi
TR::Register *
OMR::ARM64::TreeEvaluator::iloadEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   return commonLoadEvaluator(node, TR::InstOpCode::ldrimmw, cg);
   }

// also handles aloadi
TR::Register *
OMR::ARM64::TreeEvaluator::aloadEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   TR::Compilation *comp = cg->comp();
   TR::Register *tempReg;

   if (!node->getSymbolReference()->getSymbol()->isInternalPointer())
      {
      if (node->getSymbolReference()->getSymbol()->isNotCollected())
         tempReg = cg->allocateRegister();
      else
         tempReg = cg->allocateCollectedReferenceRegister();
      }
   else
      {
      tempReg = cg->allocateRegister();
      tempReg->setPinningArrayPointer(node->getSymbolReference()->getSymbol()->castToInternalPointerAutoSymbol()->getPinningArrayPointer());
      tempReg->setContainsInternalPointer();
      }

   node->setRegister(tempReg);

   TR::InstOpCode::Mnemonic op;

   if (TR::Compiler->om.generateCompressedObjectHeaders() &&
       (node->getSymbol()->isClassObject() ||
        (node->getSymbolReference() == comp->getSymRefTab()->findVftSymbolRef())))
      {
      op = TR::InstOpCode::ldrimmw;
      }
   else
      {
      op = TR::InstOpCode::ldrimmx;
      }
   TR::MemoryReference *tempMR = TR::MemoryReference::createWithRootLoadOrStore(cg, node);
   generateTrg1MemInstruction(cg, op, node, tempReg, tempMR);

   if (node->getSymbolReference() == comp->getSymRefTab()->findVftSymbolRef())
      {
      TR::TreeEvaluator::generateVFTMaskInstruction(cg, node, tempReg);
      }

   bool needSync = (node->getSymbolReference()->getSymbol()->isSyncVolatile() && cg->comp()->target().isSMP());
   if (needSync)
      {
      generateSynchronizationInstruction(cg, TR::InstOpCode::dmb, node, 0x9); // dmb ishld
      }

   tempMR->decNodeReferenceCounts(cg);

   return tempReg;
   }

// also handles lloadi
TR::Register *
OMR::ARM64::TreeEvaluator::lloadEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   return commonLoadEvaluator(node, TR::InstOpCode::ldrimmx, cg);
   }

// also handles bloadi
TR::Register *
OMR::ARM64::TreeEvaluator::bloadEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   return commonLoadEvaluator(node, TR::InstOpCode::ldrsbimmx, cg);
   }

// also handles sloadi
TR::Register *
OMR::ARM64::TreeEvaluator::sloadEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   return commonLoadEvaluator(node, TR::InstOpCode::ldrshimmx, cg);
   }

// also handles vloadi
TR::Register *
OMR::ARM64::TreeEvaluator::vloadEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   return commonLoadEvaluator(node, TR::InstOpCode::vldrimmq, cg);
   }

TR::Register *
OMR::ARM64::TreeEvaluator::awrtbarEvaluator(TR::Node *node, TR::CodeGenerator *cg)
	{
	// TODO:ARM64: Enable TR::TreeEvaluator::awrtbarEvaluator in compiler/aarch64/codegen/TreeEvaluatorTable.hpp when Implemented.
	return OMR::ARM64::TreeEvaluator::unImpOpEvaluator(node, cg);
	}

TR::Register *commonStoreEvaluator(TR::Node *node, TR::InstOpCode::Mnemonic op, TR::CodeGenerator *cg)
   {
   TR::MemoryReference *tempMR = TR::MemoryReference::createWithRootLoadOrStore(cg, node);
   bool needSync = (node->getSymbolReference()->getSymbol()->isSyncVolatile() && cg->comp()->target().isSMP());
   bool lazyVolatile = false;
   if (node->getSymbolReference()->getSymbol()->isShadow() &&
       node->getSymbolReference()->getSymbol()->isOrdered() && cg->comp()->target().isSMP())
      {
      needSync = true;
      lazyVolatile = true;
      }

   TR::Node *valueChild;

   if (node->getOpCode().isIndirect())
      {
      valueChild = node->getSecondChild();
      }
   else
      {
      valueChild = node->getFirstChild();
      }

   if (needSync)
      {
      generateSynchronizationInstruction(cg, TR::InstOpCode::dmb, node, 0xA); // dmb ishst
      }

   TR::Node *valueChildRoot = NULL;
   /*
    *  Pattern matching compressed refs sequence of address constant NULL
    +
    *  treetop
    *    istorei
    *      aload
    *      l2i (X==0 )
    *        lushr (compressionSequence )
    *          a2l
    *            aconst NULL (X==0 sharedMemory )
    *          iconst 3
    */
   if (cg->comp()->useCompressedPointers() &&
       (node->getSymbolReference()->getSymbol()->getDataType() == TR::Address) &&
       (valueChild->getDataType() != TR::Address) &&
       (valueChild->getOpCodeValue() == TR::l2i) &&
       (valueChild->isZero()))
      {
      TR::Node *tmpNode = valueChild;
      while (tmpNode->getNumChildren() && tmpNode->getOpCodeValue() != TR::a2l)
         tmpNode = tmpNode->getFirstChild();
      if (tmpNode->getNumChildren())
         tmpNode = tmpNode->getFirstChild();

      if (tmpNode->getDataType().isAddress() && tmpNode->isConstZeroValue() && (tmpNode->getRegister() == NULL))
         {
         valueChildRoot = valueChild;
         }
      }

   /*
    * Use xzr as source register of str instruction
    * if valueChild is a compressed refs sequence of address constant NULL,
    * or valueChild is a zero constant integer.
    */
   if ((valueChildRoot != NULL) || (valueChild->getDataType().isIntegral() && valueChild->isConstZeroValue() && (valueChild->getRegister() == NULL)))
      {
      TR::Register *zeroReg = cg->allocateRegister();
      generateMemSrc1Instruction(cg, op, node, tempMR, zeroReg);
      TR::RegisterDependencyConditions *deps = new (cg->trHeapMemory()) TR::RegisterDependencyConditions(0, 1, cg->trMemory());
      deps->addPostCondition(zeroReg, TR::RealRegister::xzr);
      generateLabelInstruction(cg, TR::InstOpCode::label, node, generateLabelSymbol(cg), deps);
      cg->stopUsingRegister(zeroReg);
      }
   else
      {
      generateMemSrc1Instruction(cg, op, node, tempMR, cg->evaluate(valueChild));
      }

   if (needSync)
      {
      // ordered and lazySet operations will not generate a post-write sync
      if (!lazyVolatile)
         {
         generateSynchronizationInstruction(cg, TR::InstOpCode::dmb, node, 0xB); // dmb ish
         }
      }

   if (valueChildRoot != NULL)
      {
      cg->recursivelyDecReferenceCount(valueChildRoot);
      }
   else
      {
      cg->decReferenceCount(valueChild);
      }
   tempMR->decNodeReferenceCounts(cg);

   return NULL;
   }

// also handles lstorei
TR::Register *
OMR::ARM64::TreeEvaluator::lstoreEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   return commonStoreEvaluator(node, TR::InstOpCode::strimmx, cg);
   }

// also handles bstorei
TR::Register *
OMR::ARM64::TreeEvaluator::bstoreEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   TR::Compilation *comp = cg->comp();
   if (comp->getOption(TR_EnableGCRPatching))
      {
      TR::SymbolReference *symref = node->getSymbolReference();
      if (symref)
         {
         TR::Symbol *symbol = symref->getSymbol();
         if (symbol->isGCRPatchPoint())
            {
            TR::MemoryReference *tempMR = TR::MemoryReference::createWithRootLoadOrStore(cg, node);
            TR::SymbolReference *patchGCRHelperRef = cg->symRefTab()->findOrCreateRuntimeHelper(TR_ARM64PatchGCRHelper);
            TR::RegisterDependencyConditions *deps = new (cg->trHeapMemory()) TR::RegisterDependencyConditions(0, 2, cg->trMemory());
            TR::Register *tempReg = cg->allocateRegister();
            deps->addPostCondition(tempMR->getBaseRegister(), TR::RealRegister::x0);
            deps->addPostCondition(tempReg, TR::RealRegister::x1);
            TR::Instruction *blInstruction = generateImmSymInstruction(cg, TR::InstOpCode::bl, node,
                                                                        reinterpret_cast<uintptr_t>(patchGCRHelperRef->getMethodAddress()),
                                                                        deps, patchGCRHelperRef, NULL);

            cg->stopUsingRegister(tempReg);
            cg->recursivelyDecReferenceCount(node->getFirstChild());
            tempMR->decNodeReferenceCounts(cg);
            cg->machine()->setLinkRegisterKilled(true);
            return NULL;
            }
         }
      }
   return commonStoreEvaluator(node, TR::InstOpCode::strbimm, cg);
   }

// also handles sstorei
TR::Register *
OMR::ARM64::TreeEvaluator::sstoreEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   return commonStoreEvaluator(node, TR::InstOpCode::strhimm, cg);
   }

// also handles istorei
TR::Register *
OMR::ARM64::TreeEvaluator::istoreEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   TR::Compilation *comp = cg->comp();

   commonStoreEvaluator(node, TR::InstOpCode::strimmw, cg);

   if (comp->useCompressedPointers() && node->getOpCode().isIndirect())
      node->setStoreAlreadyEvaluated(true);

   return NULL;
   }

// also handles astore, astorei
TR::Register *
OMR::ARM64::TreeEvaluator::astoreEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   TR::Compilation *comp = cg->comp();
   bool isCompressedClassPointerOfObjectHeader = TR::Compiler->om.generateCompressedObjectHeaders() &&
         (node->getSymbol()->isClassObject() ||
         (node->getSymbolReference() == comp->getSymRefTab()->findVftSymbolRef()));
   TR::InstOpCode::Mnemonic op = isCompressedClassPointerOfObjectHeader ? TR::InstOpCode::strimmw : TR::InstOpCode::strimmx;

   return commonStoreEvaluator(node, op, cg);
   }

// also handles vstorei
TR::Register *
OMR::ARM64::TreeEvaluator::vstoreEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   return commonStoreEvaluator(node, TR::InstOpCode::vstrimmq, cg);
   }

TR::Register *
OMR::ARM64::TreeEvaluator::vindexVectorEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   return TR::TreeEvaluator::unImpOpEvaluator(node, cg);
   }

TR::Register *
OMR::ARM64::TreeEvaluator::vorUncheckedEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   return TR::TreeEvaluator::unImpOpEvaluator(node, cg);
   }

TR::Register *
OMR::ARM64::TreeEvaluator::vfirstNonZeroEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   return TR::TreeEvaluator::unImpOpEvaluator(node, cg);
   }

TR::Register *
OMR::ARM64::TreeEvaluator::vgetelemEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   return TR::TreeEvaluator::unImpOpEvaluator(node, cg);
   }

TR::Register *
OMR::ARM64::TreeEvaluator::monentEvaluator(TR::Node *node, TR::CodeGenerator *cg)
	{
	// TODO:ARM64: Enable TR::TreeEvaluator::monentEvaluator in compiler/aarch64/codegen/TreeEvaluatorTable.hpp when Implemented.
	return OMR::ARM64::TreeEvaluator::unImpOpEvaluator(node, cg);
	}

TR::Register *
OMR::ARM64::TreeEvaluator::monexitEvaluator(TR::Node *node, TR::CodeGenerator *cg)
	{
	// TODO:ARM64: Enable TR::TreeEvaluator::monexitEvaluator in compiler/aarch64/codegen/TreeEvaluatorTable.hpp when Implemented.
	return OMR::ARM64::TreeEvaluator::unImpOpEvaluator(node, cg);
	}

TR::Register *
OMR::ARM64::TreeEvaluator::arraytranslateAndTestEvaluator(TR::Node *node, TR::CodeGenerator *cg)
	{
	// TODO:ARM64: Enable TR::TreeEvaluator::arraytranslateAndTestEvaluator in compiler/aarch64/codegen/TreeEvaluatorTable.hpp when Implemented.
	return OMR::ARM64::TreeEvaluator::unImpOpEvaluator(node, cg);
	}

TR::Register *
OMR::ARM64::TreeEvaluator::arraytranslateEvaluator(TR::Node *node, TR::CodeGenerator *cg)
	{
	// TODO:ARM64: Enable TR::TreeEvaluator::arraytranslateEvaluator in compiler/aarch64/codegen/TreeEvaluatorTable.hpp when Implemented.
	return OMR::ARM64::TreeEvaluator::unImpOpEvaluator(node, cg);
	}

TR::Register *
OMR::ARM64::TreeEvaluator::arraysetEvaluator(TR::Node *node, TR::CodeGenerator *cg)
	{
	// TODO:ARM64: Enable TR::TreeEvaluator::arraysetEvaluator in compiler/aarch64/codegen/TreeEvaluatorTable.hpp when Implemented.
	return OMR::ARM64::TreeEvaluator::unImpOpEvaluator(node, cg);
	}

TR::Register *
OMR::ARM64::TreeEvaluator::arraycmpEvaluator(TR::Node *node, TR::CodeGenerator *cg)
	{
	// TODO:ARM64: Enable TR::TreeEvaluator::arraycmpEvaluator in compiler/aarch64/codegen/TreeEvaluatorTable.hpp when Implemented.
	return OMR::ARM64::TreeEvaluator::unImpOpEvaluator(node, cg);
	}

static void
inlineConstantLengthForwardArrayCopy(TR::Node *node, int64_t byteLen, TR::Register *srcReg, TR::Register *dstReg, TR::CodeGenerator *cg)
   {
   if (byteLen == 0)
      return;

   int64_t iteration64 = byteLen >> 6;
   int32_t residue64 = byteLen & 0x3F;
   TR::Register *dataReg1 = (byteLen >= 16) ? cg->allocateRegister(TR_VRF) : NULL;
   TR::Register *dataReg2 = (residue64 & 0xF) ? cg->allocateRegister() : NULL;

   if (iteration64 > 1)
      {
      TR::Register *cntReg = cg->allocateRegister();
      loadConstant64(cg, node, iteration64, cntReg);

      TR::LabelSymbol *loopLabel = generateLabelSymbol(cg);
      generateLabelInstruction(cg, TR::InstOpCode::label, node, loopLabel);

      // Copy 16x4 bytes in a loop
      generateTrg1MemInstruction(cg, TR::InstOpCode::vldrpostq, node, dataReg1, TR::MemoryReference::createWithDisplacement(cg, srcReg, 16));
      generateMemSrc1Instruction(cg, TR::InstOpCode::vstrpostq, node, TR::MemoryReference::createWithDisplacement(cg, dstReg, 16), dataReg1);
      generateTrg1MemInstruction(cg, TR::InstOpCode::vldrpostq, node, dataReg1, TR::MemoryReference::createWithDisplacement(cg, srcReg, 16));
      generateMemSrc1Instruction(cg, TR::InstOpCode::vstrpostq, node, TR::MemoryReference::createWithDisplacement(cg, dstReg, 16), dataReg1);
      generateTrg1MemInstruction(cg, TR::InstOpCode::vldrpostq, node, dataReg1, TR::MemoryReference::createWithDisplacement(cg, srcReg, 16));
      generateMemSrc1Instruction(cg, TR::InstOpCode::vstrpostq, node, TR::MemoryReference::createWithDisplacement(cg, dstReg, 16), dataReg1);
      generateTrg1MemInstruction(cg, TR::InstOpCode::vldrpostq, node, dataReg1, TR::MemoryReference::createWithDisplacement(cg, srcReg, 16));
      generateMemSrc1Instruction(cg, TR::InstOpCode::vstrpostq, node, TR::MemoryReference::createWithDisplacement(cg, dstReg, 16), dataReg1);
      generateTrg1Src1ImmInstruction(cg, TR::InstOpCode::subimmx, node, cntReg, cntReg, 1);
      generateCompareBranchInstruction(cg, TR::InstOpCode::cbnzx, node, cntReg, loopLabel);

      cg->stopUsingRegister(cntReg);
      }
   else if (iteration64 == 1)
      {
      residue64 += 64;
      }

   int32_t offset = 0;
   while (residue64 > 0)
      {
      TR::InstOpCode::Mnemonic loadOp;
      TR::InstOpCode::Mnemonic storeOp;
      int32_t dataSize;
      TR::Register *dataReg = (residue64 >= 16) ? dataReg1 : dataReg2;

      if (residue64 >= 16)
         {
         loadOp  = TR::InstOpCode::vldrimmq;
         storeOp = TR::InstOpCode::vstrimmq;
         dataSize = 16;
         }
      else if (residue64 >= 8)
         {
         loadOp  = TR::InstOpCode::ldrimmx;
         storeOp = TR::InstOpCode::strimmx;
         dataSize = 8;
         }
      else if (residue64 >= 4)
         {
         loadOp  = TR::InstOpCode::ldrimmw;
         storeOp = TR::InstOpCode::strimmw;
         dataSize = 4;
         }
      else if (residue64 >= 2)
         {
         loadOp  = TR::InstOpCode::ldrhimm;
         storeOp = TR::InstOpCode::strhimm;
         dataSize = 2;
         }
      else
         {
         loadOp  = TR::InstOpCode::ldrbimm;
         storeOp = TR::InstOpCode::strbimm;
         dataSize = 1;
         }

      generateTrg1MemInstruction(cg, loadOp, node, dataReg, TR::MemoryReference::createWithDisplacement(cg, srcReg, offset));
      generateMemSrc1Instruction(cg, storeOp, node, TR::MemoryReference::createWithDisplacement(cg, dstReg, offset), dataReg);
      offset += dataSize;
      residue64 -= dataSize;
      }

   if (dataReg1)
      cg->stopUsingRegister(dataReg1);
   if (dataReg2)
      cg->stopUsingRegister(dataReg2);

   return;
   }

static void
inlineConstantLengthBackwardArrayCopy(TR::Node *node, int64_t byteLen, TR::Register *srcReg, TR::Register *dstReg, TR::CodeGenerator *cg)
   {
   if (byteLen == 0)
      return;

   int64_t iteration64 = byteLen >> 6;
   int32_t residue64 = byteLen & 0x3F;
   TR::Register *dataReg1 = (byteLen >= 16) ? cg->allocateRegister(TR_VRF) : NULL;
   TR::Register *dataReg2 = (residue64 & 0xF) ? cg->allocateRegister() : NULL;

   // Adjusting scrReg and dstReg
   addConstant64(cg, node, srcReg, srcReg, byteLen);
   addConstant64(cg, node, dstReg, dstReg, byteLen);

   if (iteration64 > 1)
      {
      TR::Register *cntReg = cg->allocateRegister();
      loadConstant64(cg, node, iteration64, cntReg);

      TR::LabelSymbol *loopLabel = generateLabelSymbol(cg);
      generateLabelInstruction(cg, TR::InstOpCode::label, node, loopLabel);

      // Copy 16x4 bytes in a loop
      generateTrg1MemInstruction(cg, TR::InstOpCode::vldrpreq, node, dataReg1, TR::MemoryReference::createWithDisplacement(cg, srcReg, -16));
      generateMemSrc1Instruction(cg, TR::InstOpCode::vstrpreq, node, TR::MemoryReference::createWithDisplacement(cg, dstReg, -16), dataReg1);
      generateTrg1MemInstruction(cg, TR::InstOpCode::vldrpreq, node, dataReg1, TR::MemoryReference::createWithDisplacement(cg, srcReg, -16));
      generateMemSrc1Instruction(cg, TR::InstOpCode::vstrpreq, node, TR::MemoryReference::createWithDisplacement(cg, dstReg, -16), dataReg1);
      generateTrg1MemInstruction(cg, TR::InstOpCode::vldrpreq, node, dataReg1, TR::MemoryReference::createWithDisplacement(cg, srcReg, -16));
      generateMemSrc1Instruction(cg, TR::InstOpCode::vstrpreq, node, TR::MemoryReference::createWithDisplacement(cg, dstReg, -16), dataReg1);
      generateTrg1MemInstruction(cg, TR::InstOpCode::vldrpreq, node, dataReg1, TR::MemoryReference::createWithDisplacement(cg, srcReg, -16));
      generateMemSrc1Instruction(cg, TR::InstOpCode::vstrpreq, node, TR::MemoryReference::createWithDisplacement(cg, dstReg, -16), dataReg1);
      generateTrg1Src1ImmInstruction(cg, TR::InstOpCode::subimmx, node, cntReg, cntReg, 1);
      generateCompareBranchInstruction(cg, TR::InstOpCode::cbnzx, node, cntReg, loopLabel);

      cg->stopUsingRegister(cntReg);
      }
   else if (iteration64 == 1)
      {
      residue64 += 64;
      }

   while (residue64 > 0)
      {
      TR::InstOpCode::Mnemonic loadOp;
      TR::InstOpCode::Mnemonic storeOp;
      int32_t dataSize;
      TR::Register *dataReg = (residue64 >= 16) ? dataReg1 : dataReg2;

      if (residue64 >= 16)
         {
         loadOp  = TR::InstOpCode::vldrpreq;
         storeOp = TR::InstOpCode::vstrpreq;
         dataSize = 16;
         }
      else if (residue64 >= 8)
         {
         loadOp  = TR::InstOpCode::ldrprex;
         storeOp = TR::InstOpCode::strprex;
         dataSize = 8;
         }
      else if (residue64 >= 4)
         {
         loadOp  = TR::InstOpCode::ldrprew;
         storeOp = TR::InstOpCode::strprew;
         dataSize = 4;
         }
      else if (residue64 >= 2)
         {
         loadOp  = TR::InstOpCode::ldrhpre;
         storeOp = TR::InstOpCode::strhpre;
         dataSize = 2;
         }
      else
         {
         loadOp  = TR::InstOpCode::ldrbpre;
         storeOp = TR::InstOpCode::strbpre;
         dataSize = 1;
         }

      generateTrg1MemInstruction(cg, loadOp, node, dataReg, TR::MemoryReference::createWithDisplacement(cg, srcReg, -dataSize));
      generateMemSrc1Instruction(cg, storeOp, node, TR::MemoryReference::createWithDisplacement(cg, dstReg, -dataSize), dataReg);
      residue64 -= dataSize;
      }

   if (dataReg1)
      cg->stopUsingRegister(dataReg1);
   if (dataReg2)
      cg->stopUsingRegister(dataReg2);

   return;
   }

bool
OMR::ARM64::TreeEvaluator::stopUsingCopyReg(TR::Node *node, TR::Register *&reg, TR::CodeGenerator *cg)
   {
   if (node != NULL)
      {
      reg = cg->evaluate(node);
      if (!cg->canClobberNodesRegister(node))
         {
         TR::Register *copyReg;
         if (reg->containsInternalPointer() || !reg->containsCollectedReference())
            {
            copyReg = cg->allocateRegister();
            if (reg->containsInternalPointer())
               {
               copyReg->setPinningArrayPointer(reg->getPinningArrayPointer());
               copyReg->setContainsInternalPointer();
               }
            }
         else
            {
            copyReg = cg->allocateCollectedReferenceRegister();
            }
         generateMovInstruction(cg, node, copyReg, reg);
         reg = copyReg;
         return true;
         }
      }

   return false;
   }

static void
generateCallToArrayCopyHelper(TR::Node *node, TR::Register *srcAddrReg, TR::Register *dstAddrReg, TR::Register *lengthReg,  TR::RegisterDependencyConditions *deps, TR::CodeGenerator *cg)
   {
   // Start of assembly helper path.
   TR_RuntimeHelper helper;
   TR::DataType dt = node->getArrayCopyElementType();
   uint32_t elementSize;
   if (node->isReferenceArrayCopy() || dt == TR::Address)
      elementSize = TR::Compiler->om.sizeofReferenceField();
   else
      elementSize = TR::Symbol::convertTypeToSize(dt);

   if (node->isForwardArrayCopy())
      {
      switch (elementSize)
         {
         case 16:
            helper = TR_ARM64forwardQuadWordArrayCopy;
            break;
         case 8:
            helper = TR_ARM64forwardDoubleWordArrayCopy;
            break;
         case 4:
            helper = TR_ARM64forwardWordArrayCopy;
            break;
         case 2:
            helper = TR_ARM64forwardHalfWordArrayCopy;
            break;
         default:
            helper = TR_ARM64forwardArrayCopy;
            break;
         }
      }
   else if (node->isBackwardArrayCopy())
      {
      // Adjusting src and dst addresses
      generateTrg1Src2Instruction(cg, TR::InstOpCode::addx, node, srcAddrReg, srcAddrReg, lengthReg);
      generateTrg1Src2Instruction(cg, TR::InstOpCode::addx, node, dstAddrReg, dstAddrReg, lengthReg);
      switch (elementSize)
         {
         case 16:
            helper = TR_ARM64backwardQuadWordArrayCopy;
            break;
         case 8:
            helper = TR_ARM64backwardDoubleWordArrayCopy;
            break;
         case 4:
            helper = TR_ARM64backwardWordArrayCopy;
            break;
         case 2:
            helper = TR_ARM64backwardHalfWordArrayCopy;
            break;
         default:
            helper = TR_ARM64backwardArrayCopy;
            break;
         }
      }
   else // We are not sure it is forward or we have to do backward.
      helper = TR_ARM64arrayCopy;

   TR::SymbolReference *arrayCopyHelper = cg->symRefTab()->findOrCreateRuntimeHelper(helper, false, false, false);

   generateImmSymInstruction(cg, TR::InstOpCode::bl, node,
                             (uintptr_t)arrayCopyHelper->getMethodAddress(),
                             deps, arrayCopyHelper, NULL);
   cg->machine()->setLinkRegisterKilled(true);

   return;
   }
TR::Register *
OMR::ARM64::TreeEvaluator::arraycopyEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   bool simpleCopy = (node->getNumChildren() == 3);
   bool arrayStoreCheckIsNeeded = !simpleCopy && !node->chkNoArrayStoreCheckArrayCopy();

   if (arrayStoreCheckIsNeeded)
      {
      // call the "C" helper, handle the exception case
      TR::TreeEvaluator::genArrayCopyWithArrayStoreCHK(node, cg);
      return NULL;
      }

   TR::Node *srcObjNode, *dstObjNode, *srcAddrNode, *dstAddrNode, *lengthNode;
   TR::Register *srcObjReg = NULL, *dstObjReg = NULL, *srcAddrReg = NULL, *dstAddrReg = NULL, *lengthReg;
   bool stopUsingCopyReg1, stopUsingCopyReg2, stopUsingCopyReg3, stopUsingCopyReg4, stopUsingCopyReg5 = false;

   if (simpleCopy)
      {
      // child 0: Source byte address
      // child 1: Destination byte address
      // child 2: Copy length in bytes
      srcObjNode = NULL;
      dstObjNode = NULL;
      srcAddrNode = node->getChild(0);
      dstAddrNode = node->getChild(1);
      lengthNode = node->getChild(2);
      }
   else
      {
      // child 0: Source array object
      // child 1: Destination array object
      // child 2: Source byte address
      // child 3: Destination byte address
      // child 4: Copy length in bytes
      srcObjNode = node->getChild(0);
      dstObjNode = node->getChild(1);
      srcAddrNode = node->getChild(2);
      dstAddrNode = node->getChild(3);
      lengthNode = node->getChild(4);
      }

   stopUsingCopyReg1 = stopUsingCopyReg(srcObjNode, srcObjReg, cg);
   stopUsingCopyReg2 = stopUsingCopyReg(dstObjNode, dstObjReg, cg);
   stopUsingCopyReg3 = stopUsingCopyReg(srcAddrNode, srcAddrReg, cg);
   stopUsingCopyReg4 = stopUsingCopyReg(dstAddrNode, dstAddrReg, cg);

   static const bool disableArrayCopyInlining = feGetEnv("TR_disableArrayCopyInlining") != NULL;
   if ((simpleCopy || !arrayStoreCheckIsNeeded) &&
       (node->isForwardArrayCopy() || node->isBackwardArrayCopy()) &&
       lengthNode->getOpCode().isLoadConst() && !disableArrayCopyInlining)
      {
      int64_t len = lengthNode->getType().isInt32() ? lengthNode->getInt() : lengthNode->getLongInt();
      if (node->isForwardArrayCopy())
         inlineConstantLengthForwardArrayCopy(node, len, srcAddrReg, dstAddrReg, cg);
      else
         inlineConstantLengthBackwardArrayCopy(node, len, srcAddrReg, dstAddrReg, cg);

      if (!simpleCopy)
         {
         TR::TreeEvaluator::genWrtbarForArrayCopy(node, srcObjReg, dstObjReg, cg);
         cg->decReferenceCount(srcObjNode);
         cg->decReferenceCount(dstObjNode);
         }
      if (stopUsingCopyReg1)
         cg->stopUsingRegister(srcObjReg);
      if (stopUsingCopyReg2)
         cg->stopUsingRegister(dstObjReg);
      if (stopUsingCopyReg3)
         cg->stopUsingRegister(srcAddrReg);
      if (stopUsingCopyReg4)
         cg->stopUsingRegister(dstAddrReg);

      cg->decReferenceCount(srcAddrNode);
      cg->decReferenceCount(dstAddrNode);
      cg->decReferenceCount(lengthNode);

      return NULL;
      }

   lengthReg = cg->evaluate(lengthNode);
   if (!cg->canClobberNodesRegister(lengthNode))
      {
      TR::Register *lenCopyReg = cg->allocateRegister();
      generateMovInstruction(cg, lengthNode, lenCopyReg, lengthReg);
      lengthReg = lenCopyReg;
      stopUsingCopyReg5 = true;
      }

   // x0-x4 are destroyed in the helper
   TR::RegisterDependencyConditions *deps = new (cg->trHeapMemory()) TR::RegisterDependencyConditions(5, 5, cg->trMemory());
   TR::addDependency(deps, lengthReg, TR::RealRegister::x0, TR_GPR, cg);
   TR::addDependency(deps, srcAddrReg, TR::RealRegister::x1, TR_GPR, cg);
   TR::addDependency(deps, dstAddrReg, TR::RealRegister::x2, TR_GPR, cg);
   TR::addDependency(deps, NULL, TR::RealRegister::x3, TR_GPR, cg);
   TR::addDependency(deps, NULL, TR::RealRegister::x4, TR_GPR, cg);
   TR::Register *x3Reg = deps->searchPostConditionRegister(TR::RealRegister::x3);
   TR::Register *x4Reg = deps->searchPostConditionRegister(TR::RealRegister::x4);

   generateCallToArrayCopyHelper(node, srcAddrReg, dstAddrReg, lengthReg, deps, cg);

   if (!simpleCopy)
      {
      TR::TreeEvaluator::genWrtbarForArrayCopy(node, srcObjReg, dstObjReg, cg);
      cg->decReferenceCount(srcObjNode);
      cg->decReferenceCount(dstObjNode);
      }

   if (stopUsingCopyReg1)
      cg->stopUsingRegister(srcObjReg);
   if (stopUsingCopyReg2)
      cg->stopUsingRegister(dstObjReg);
   if (stopUsingCopyReg3)
      cg->stopUsingRegister(srcAddrReg);
   if (stopUsingCopyReg4)
      cg->stopUsingRegister(dstAddrReg);
   if (stopUsingCopyReg5)
      cg->stopUsingRegister(lengthReg);

   cg->stopUsingRegister(x3Reg);
   cg->stopUsingRegister(x4Reg);

   cg->decReferenceCount(srcAddrNode);
   cg->decReferenceCount(dstAddrNode);
   cg->decReferenceCount(lengthNode);

   return NULL;
   }

TR::Register *
OMR::ARM64::TreeEvaluator::asynccheckEvaluator(TR::Node *node, TR::CodeGenerator *cg)
	{
	// TODO:ARM64: Enable TR::TreeEvaluator::asynccheckEvaluator in compiler/aarch64/codegen/TreeEvaluatorTable.hpp when Implemented.
	return OMR::ARM64::TreeEvaluator::unImpOpEvaluator(node, cg);
	}

TR::Register *
OMR::ARM64::TreeEvaluator::instanceofEvaluator(TR::Node *node, TR::CodeGenerator *cg)
	{
	// TODO:ARM64: Enable TR::TreeEvaluator::instanceofEvaluator in compiler/aarch64/codegen/TreeEvaluatorTable.hpp when Implemented.
	return OMR::ARM64::TreeEvaluator::unImpOpEvaluator(node, cg);
	}

TR::Register *
OMR::ARM64::TreeEvaluator::checkcastEvaluator(TR::Node *node, TR::CodeGenerator *cg)
	{
	// TODO:ARM64: Enable TR::TreeEvaluator::checkcastEvaluator in compiler/aarch64/codegen/TreeEvaluatorTable.hpp when Implemented.
	return OMR::ARM64::TreeEvaluator::unImpOpEvaluator(node, cg);
	}

TR::Register *
OMR::ARM64::TreeEvaluator::checkcastAndNULLCHKEvaluator(TR::Node *node, TR::CodeGenerator *cg)
	{
	// TODO:ARM64: Enable TR::TreeEvaluator::checkcastAndNULLCHKEvaluator in compiler/aarch64/codegen/TreeEvaluatorTable.hpp when Implemented.
	return OMR::ARM64::TreeEvaluator::unImpOpEvaluator(node, cg);
	}

// handles call, icall, lcall, fcall, dcall, acall
TR::Register *
OMR::ARM64::TreeEvaluator::directCallEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   TR::Register *resultReg;
   if (!cg->inlineDirectCall(node, resultReg))
      {
      TR::SymbolReference *symRef = node->getSymbolReference();
      TR::MethodSymbol *callee = symRef->getSymbol()->castToMethodSymbol();
      TR::Linkage *linkage = cg->getLinkage(callee->getLinkageConvention());

      resultReg = linkage->buildDirectDispatch(node);
      }
   return resultReg;
   }

// handles calli, icalli, lcalli, fcalli, dcalli, acalli
TR::Register *
OMR::ARM64::TreeEvaluator::indirectCallEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   TR::SymbolReference *symRef = node->getSymbolReference();
   TR::MethodSymbol *callee = symRef->getSymbol()->castToMethodSymbol();
   TR::Linkage *linkage = cg->getLinkage(callee->getLinkageConvention());

   return linkage->buildIndirectDispatch(node);
   }

TR::Register *
OMR::ARM64::TreeEvaluator::treetopEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   TR::Register *tempReg = cg->evaluate(node->getFirstChild());
   cg->decReferenceCount(node->getFirstChild());
   return tempReg;
   }

TR::Register *
OMR::ARM64::TreeEvaluator::exceptionRangeFenceEvaluator(TR::Node *node, TR::CodeGenerator *cg)
	{
	// TODO:ARM64: Enable TR::TreeEvaluator::exceptionRangeFenceEvaluator in compiler/aarch64/codegen/TreeEvaluatorTable.hpp when Implemented.
	return OMR::ARM64::TreeEvaluator::unImpOpEvaluator(node, cg);
	}

TR::Register *
OMR::ARM64::TreeEvaluator::loadaddrEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   TR::Register *resultReg;
   TR::Symbol *sym = node->getSymbol();
   TR::Compilation *comp = cg->comp();
   TR::MemoryReference *mref = TR::MemoryReference::createWithSymRef(cg, node, node->getSymbolReference());

   if (mref->getUnresolvedSnippet() != NULL)
      {
      resultReg = sym->isLocalObject() ? cg->allocateCollectedReferenceRegister() : cg->allocateRegister();
      if (mref->useIndexedForm())
         {
         TR_ASSERT(false, "Unresolved indexed snippet is not supported");
         }
      else
         {
         TR_UNIMPLEMENTED();
         }
      }
   else
      {
      if (mref->useIndexedForm())
         {
         resultReg = sym->isLocalObject() ? cg->allocateCollectedReferenceRegister() : cg->allocateRegister();
         generateTrg1Src2Instruction(cg, TR::InstOpCode::addx, node, resultReg, mref->getBaseRegister(), mref->getIndexRegister());
         }
      else
         {
         int32_t offset = mref->getOffset();
         if (mref->hasDelayedOffset() || offset != 0)
            {
            resultReg = sym->isLocalObject() ? cg->allocateCollectedReferenceRegister() : cg->allocateRegister();
            if (mref->hasDelayedOffset())
               {
               generateTrg1MemInstruction(cg, TR::InstOpCode::addimmx, node, resultReg, mref);
               }
            else
               {
               if (offset >= 0 && constantIsUnsignedImm12(offset))
                  {
                  generateTrg1Src1ImmInstruction(cg, TR::InstOpCode::addimmx, node, resultReg, mref->getBaseRegister(), offset);
                  }
               else
                  {
                  loadConstant64(cg, node, offset, resultReg);
                  generateTrg1Src2Instruction(cg, TR::InstOpCode::addx, node, resultReg, mref->getBaseRegister(), resultReg);
                  }
               }
            }
         else
            {
            resultReg = mref->getBaseRegister();
            if (resultReg == cg->getMethodMetaDataRegister())
               {
               resultReg = cg->allocateRegister();
               generateMovInstruction(cg, node, resultReg, mref->getBaseRegister());
               }
            }
         }
      }
   node->setRegister(resultReg);
   mref->decNodeReferenceCounts(cg);
   return resultReg;
   }

TR::Register *
OMR::ARM64::TreeEvaluator::aRegLoadEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   TR::Register *globalReg = node->getRegister();

   if (globalReg == NULL)
      {
      if (node->getRegLoadStoreSymbolReference()->getSymbol()->isNotCollected() ||
          node->getRegLoadStoreSymbolReference()->getSymbol()->isInternalPointer())
         {
         globalReg = cg->allocateRegister();
         if (node->getRegLoadStoreSymbolReference()->getSymbol()->isInternalPointer())
            {
            globalReg->setContainsInternalPointer();
            globalReg->setPinningArrayPointer(node->getRegLoadStoreSymbolReference()->getSymbol()->castToInternalPointerAutoSymbol()->getPinningArrayPointer());
            }
         }
      else
         {
         globalReg = cg->allocateCollectedReferenceRegister();
         }

      node->setRegister(globalReg);
      }
   return globalReg;
   }

// Also handles sRegLoad, bRegLoad, and lRegLoad
TR::Register *
OMR::ARM64::TreeEvaluator::iRegLoadEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   TR::Register *globalReg = node->getRegister();

   if (globalReg == NULL)
      {
      globalReg = cg->allocateRegister();
      node->setRegister(globalReg);
      }
   return(globalReg);
   }

// Also handles sRegStore, bRegStore, lRegStore, and aRegStore
TR::Register *
OMR::ARM64::TreeEvaluator::iRegStoreEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   TR::Node *child = node->getFirstChild();
   TR::Register *globalReg = cg->evaluate(child);
   cg->decReferenceCount(child);
   return globalReg;
   }

TR::Register *
OMR::ARM64::TreeEvaluator::GlRegDepsEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   int32_t i;

   for (i = 0; i < node->getNumChildren(); i++)
      {
      cg->evaluate(node->getChild(i));
      cg->decReferenceCount(node->getChild(i));
      }
   return NULL;
   }

TR::Register *
OMR::ARM64::TreeEvaluator::BBStartEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   TR::Compilation *comp = cg->comp();
   TR::Block *block = node->getBlock();
   cg->setCurrentBlock(block);

   TR::RegisterDependencyConditions *deps = NULL;

   if (!block->isExtensionOfPreviousBlock())
      {
      TR::Machine *machine = cg->machine();
      // REG ASSOC
      machine->clearRegisterAssociations();
      machine->setRegisterWeightsFromAssociations();

      if (node->getNumChildren() > 0)
         {
         int32_t i;
         TR::Node *child = node->getFirstChild();

         cg->evaluate(child);

         deps = generateRegisterDependencyConditions(cg, child, 0);
         if (cg->getCurrentEvaluationTreeTop() == comp->getStartTree())
            {
            for (i=0; i<child->getNumChildren(); i++)
               {
               TR::ParameterSymbol *sym = child->getChild(i)->getSymbol()->getParmSymbol();
               if (sym != NULL)
                  {
                  sym->setAssignedGlobalRegisterIndex(cg->getGlobalRegister(child->getChild(i)->getGlobalRegisterNumber()));
                  }
               }
            }
         cg->decReferenceCount(child);
         }
      }

   TR::LabelSymbol *labelSym = node->getLabel();
   if (!labelSym)
      {
      labelSym = generateLabelSymbol(cg);
      node->setLabel(labelSym);
      }
   TR::Instruction *labelInst = generateLabelInstruction(cg, TR::InstOpCode::label, node, labelSym, deps);
   labelSym->setInstruction(labelInst);
   block->setFirstInstruction(labelInst);

   TR::Node *fenceNode = TR::Node::createRelative32BitFenceNode(node, &block->getInstructionBoundaries()._startPC);
   TR::Instruction *fence = generateAdminInstruction(cg, TR::InstOpCode::fence, node, fenceNode);

   if (block->isCatchBlock())
      {
      cg->generateCatchBlockBBStartPrologue(node, fence);
      }

   return NULL;
   }

TR::Register *
OMR::ARM64::TreeEvaluator::BBEndEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   TR::Block *block = node->getBlock();
   TR::Compilation *comp = cg->comp();
   TR::Node *fenceNode = TR::Node::createRelative32BitFenceNode(node, &node->getBlock()->getInstructionBoundaries()._endPC);

   if (NULL == block->getNextBlock())
      {
      TR::Instruction *lastInstruction = cg->getAppendInstruction();
      if (lastInstruction->getOpCodeValue() == TR::InstOpCode::bl
              && lastInstruction->getNode()->getSymbolReference()->getReferenceNumber() == TR_aThrow)
         {
         lastInstruction = generateInstruction(cg, TR::InstOpCode::bad, node, lastInstruction);
         }
      }

   TR::TreeTop *nextTT = cg->getCurrentEvaluationTreeTop()->getNextTreeTop();

   TR::RegisterDependencyConditions *deps = NULL;
   if (!nextTT || !nextTT->getNode()->getBlock()->isExtensionOfPreviousBlock())
      {
      if (cg->enableRegisterAssociations() &&
          cg->getAppendInstruction()->getOpCodeValue() != TR::InstOpCode::assocreg)
         {
         cg->machine()->createRegisterAssociationDirective(cg->getAppendInstruction());
         }

      if (node->getNumChildren() > 0)
         {
         TR::Node *child = node->getFirstChild();
         cg->evaluate(child);
         deps = generateRegisterDependencyConditions(cg, child, 0);
         cg->decReferenceCount(child);
         }
      }

   // put the dependencies (if any) on the fence
   generateAdminInstruction(cg, TR::InstOpCode::fence, node, deps, fenceNode);

   return NULL;
   }

// handles l2a, lu2a, a2l
TR::Register *
OMR::ARM64::TreeEvaluator::passThroughEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   TR::Node *child = node->getFirstChild();
   TR::Register *trgReg = cg->evaluate(child);
   node->setRegister(trgReg);
   cg->decReferenceCount(child);
   return trgReg;
   }

TR::Register *
OMR::ARM64::TreeEvaluator::PrefetchEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   TR_ASSERT(node->getNumChildren() == 4, "TR::Prefetch should contain 4 child nodes");

   TR::Compilation *comp = cg->comp();
   TR::Node *firstChild = node->getFirstChild();
   TR::Node *secondChild = node->getChild(1);
   TR::Node *sizeChild = node->getChild(2);
   TR::Node *typeChild = node->getChild(3);

   // Do nothing for now

   cg->recursivelyDecReferenceCount(firstChild);
   cg->recursivelyDecReferenceCount(secondChild);
   cg->recursivelyDecReferenceCount(sizeChild);
   cg->recursivelyDecReferenceCount(typeChild);
   return NULL;
   }

TR::Register *
OMR::ARM64::TreeEvaluator::performCall(TR::Node *node, bool isIndirect, TR::CodeGenerator *cg)
   {
   TR::SymbolReference *symRef = node->getSymbolReference();
   TR::MethodSymbol *callee = symRef->getSymbol()->castToMethodSymbol();
   TR::Linkage *linkage = cg->getLinkage(callee->getLinkageConvention());
   TR::Register *returnRegister;

   if (isIndirect)
      returnRegister = linkage->buildIndirectDispatch(node);
   else
      returnRegister = linkage->buildDirectDispatch(node);

   return returnRegister;
   }

TR::Instruction *
OMR::ARM64::TreeEvaluator::generateVFTMaskInstruction(TR::CodeGenerator *cg, TR::Node *node, TR::Register *dstReg, TR::Register *srcReg, TR::Instruction *preced)
   {
   // Do nothing in OMR
   return preced;
   }

TR::Instruction *
OMR::ARM64::TreeEvaluator::generateVFTMaskInstruction(TR::CodeGenerator *cg, TR::Node *node, TR::Register *reg, TR::Instruction *preced)
   {
   // Do nothing in OMR
   return preced;
   }

/**
 * @brief Inlines an intrinsic for calls to atomicAddSymbol which are represented by a call node of the form for 32-bit (64-bit similar):
 *
 *     This implies `volatile` memory access mode.
 *
 *     @code
 *       icall <atomicAddSymbol>
 *         <address>
 *         <value>
 *     @endcode
 *
 *     Which performs the following operation atomically:
 *
 *     @code
 *       [address] = [address] + <value>
 *       return <value>
 *     @endcode
 *
 * @param node: The respective (i|l)call node.
 * @param   cg: The code generator used to generate the instructions.
 * @returns A register holding the <value> node.
 */
static TR::Register *intrinsicAtomicAdd(TR::Node *node, TR::CodeGenerator *cg)
   {
   TR::Node *addressNode = node->getChild(0);
   TR::Node *valueNode = node->getChild(1);

   TR::Register *addressReg = cg->evaluate(addressNode);
   TR::Register *valueReg = cg->gprClobberEvaluate(valueNode);
   const bool is64Bit = valueNode->getDataType().isInt64();

   TR::Register *newValueReg = cg->allocateRegister();
   TR::Compilation *comp = cg->comp();

   static const bool disableLSE = feGetEnv("TR_aarch64DisableLSE") != NULL;
   if (comp->target().cpu.supportsFeature(OMR_FEATURE_ARM64_LSE) && (!disableLSE))
      {
      /*
       * The newValue is not required for this case,
       * but if we use staddl, acquire semantics is not applied.
       * We use ldaddal to ensure volatile semantics.
       */
      auto op = is64Bit ? TR::InstOpCode::ldaddalx : TR::InstOpCode::ldaddalw;
      /*
       * As Trg1MemSrc1Instruction was introduced to support ldxr/stxr instructions, target and source register convention
       * is somewhat confusing. Its `treg` register actually is a source register and `sreg` register is a target register.
       * This needs to be fixed at some point.
       */
      generateTrg1MemSrc1Instruction(cg, op, node, valueReg, TR::MemoryReference::createWithDisplacement(cg, addressReg, 0), newValueReg);
      }
   else
      {
      /*
      * Generating non-intuitive instruction sequence which uses load exclusive register
      * and store release exclusive register followed by full memory barrier.
      *
      * Because this atomic add has `volatile` semantics,
      * no loads/stores before this sequence can be reordred after it and
      * no loads/stores after it can be reordered before it.
      *
      * loop:
      *    ldxrx   oldValueReg, [addressReg]
      *    addx    newValueReg, oldValueReg, valueReg
      *    stlxrx  oldValueReg, newValueReg, [addressReg]
      *    cbnzx   oldValueReg, loop
      *    dmb     ish
      *
      * For rationale behind this instruction sequence,
      * see https://patchwork.kernel.org/project/linux-arm-kernel/patch/1391516953-14541-1-git-send-email-will.deacon@arm.com/
      *
      */

      TR::LabelSymbol *doneLabel = TR::LabelSymbol::create(cg->trHeapMemory(), cg);
      TR::LabelSymbol *loopLabel = TR::LabelSymbol::create(cg->trHeapMemory(), cg);
      TR::Register *oldValueReg = cg->allocateRegister();

      loopLabel->setStartInternalControlFlow();
      generateLabelInstruction(cg, TR::InstOpCode::label, node, loopLabel);

      auto loadop = is64Bit ? TR::InstOpCode::ldxrx : TR::InstOpCode::ldxrw;
      generateTrg1MemInstruction(cg, loadop, node, oldValueReg, TR::MemoryReference::createWithDisplacement(cg, addressReg, 0));

      generateTrg1Src2Instruction(cg, (is64Bit ? TR::InstOpCode::addx : TR::InstOpCode::addw), node, newValueReg, oldValueReg, valueReg);

      // store release exclusive register
      auto storeop = is64Bit ? TR::InstOpCode::stlxrx : TR::InstOpCode::stlxrw;
      generateTrg1MemSrc1Instruction(cg, storeop, node, oldValueReg, TR::MemoryReference::createWithDisplacement(cg, addressReg, 0), newValueReg);
      generateCompareBranchInstruction(cg, TR::InstOpCode::cbnzx, node, oldValueReg, loopLabel);

      generateSynchronizationInstruction(cg, TR::InstOpCode::dmb, node, 0xB); // dmb ish

      //Set the conditions and dependencies
      auto conditions = new (cg->trHeapMemory()) TR::RegisterDependencyConditions(0, 4, cg->trMemory());

      conditions->addPostCondition(newValueReg, TR::RealRegister::NoReg);
      conditions->addPostCondition(oldValueReg, TR::RealRegister::NoReg);
      conditions->addPostCondition(addressReg, TR::RealRegister::NoReg);
      conditions->addPostCondition(valueReg, TR::RealRegister::NoReg);

      doneLabel->setEndInternalControlFlow();
      generateLabelInstruction(cg, TR::InstOpCode::label, node, doneLabel, conditions);
      cg->stopUsingRegister(oldValueReg);
      }

   node->setRegister(valueReg);
   cg->stopUsingRegister(newValueReg);

   cg->decReferenceCount(addressNode);
   cg->decReferenceCount(valueNode);

   return valueReg;
   }

/**
 * @brief Inlines an intrinsic for calls to atomicFetchAndAddSymbol which are represented by a call node of the form for 32-bit (64-bit similar):
 *
 *     This implies `volatile` memory access mode.
 *
 *     @code
 *       icall <atomicFetchAndAddSymbol>
 *         <address>
 *         <value>
 *     @endcode
 *
 *     Which performs the following operation atomically:
 *
 *     @code
 *       temp = [address]
 *       [address] = [address] + <value>
 *       return temp
 *     @endcode
 *
 * @param node: The respective (i|l)call node.
 * @param   cg: The code generator used to generate the instructions.
 * @returns A register holding the original value in memory (before the addition) at the <address> location.
 */
TR::Register *intrinsicAtomicFetchAndAdd(TR::Node *node, TR::CodeGenerator *cg)
   {
   TR::Node *addressNode = node->getChild(0);
   TR::Node *valueNode = node->getChild(1);

   TR::Register *addressReg = cg->evaluate(addressNode);
   TR::Register *valueReg = NULL;
   const bool is64Bit = valueNode->getDataType().isInt64();
   TR::Register *oldValueReg = cg->allocateRegister();

   TR::Compilation *comp = cg->comp();
   static const bool disableLSE = feGetEnv("TR_aarch64DisableLSE") != NULL;
   if (comp->target().cpu.supportsFeature(OMR_FEATURE_ARM64_LSE) && (!disableLSE))
      {
      valueReg = cg->evaluate(valueNode);

      auto op = is64Bit ? TR::InstOpCode::ldaddalx : TR::InstOpCode::ldaddalw;
      /*
       * As Trg1MemSrc1Instruction was introduced to support ldxr/stxr instructions, target and source register convention
       * is somewhat confusing. Its `treg` register actually is a source register and `sreg` register is a target register.
       * This needs to be fixed at some point.
       */
      generateTrg1MemSrc1Instruction(cg, op, node, valueReg, TR::MemoryReference::createWithDisplacement(cg, addressReg, 0), oldValueReg);
      }
   else
      {
      int64_t value = 0;
      bool negate = false;
      bool killValueReg = false;
      if (valueNode->getOpCode().isLoadConst() && valueNode->getRegister() == NULL)
         {
         if (is64Bit)
            {
            value = valueNode->getLongInt();
            }
         else
            {
            value = valueNode->getInt();
            }
         if (!constantIsUnsignedImm12(value))
            {
            if (constantIsUnsignedImm12(-value))
               {
               negate = true;
               }
            else
               {
               valueReg = cg->allocateRegister();
               killValueReg = true;
               if(is64Bit)
                  {
                  loadConstant64(cg, node, value, valueReg);
                  }
               else
                  {
                  loadConstant32(cg, node, value, valueReg);
                  }
               }
            }
         }
      else
         {
         valueReg = cg->evaluate(valueNode);
         }

      TR::Register *newValueReg = cg->allocateRegister();
      TR::Register *tempReg = cg->allocateRegister();

      /*
      * Generating non-intuitive instruction sequence which uses load exclusive register
      * and store release exclusive register followed by full memory barrier.
      *
      * Because this atomic add has `volatile` semantics,
      * no loads/stores before this sequence can be reordred after it and
      * no loads/stores after it can be reordered before it.
      *
      * loop:
      *    ldxrx   oldValueReg, [addressReg]
      *    addx    newValueReg, oldValueReg, valueReg
      *    stlxrx  tempReg, newValueReg, [addressReg]
      *    cbnzx   tempReg, loop
      *    dmb     ish
      *
      * For rationale behind this instruction sequence,
      * see https://patchwork.kernel.org/project/linux-arm-kernel/patch/1391516953-14541-1-git-send-email-will.deacon@arm.com/
      *
      */

      TR::LabelSymbol *doneLabel = TR::LabelSymbol::create(cg->trHeapMemory(), cg);
      TR::LabelSymbol *loopLabel = TR::LabelSymbol::create(cg->trHeapMemory(), cg);

      loopLabel->setStartInternalControlFlow();
      generateLabelInstruction(cg, TR::InstOpCode::label, node, loopLabel);

      // load acquire exclusive register
      auto loadop = is64Bit ? TR::InstOpCode::ldxrx : TR::InstOpCode::ldxrw;
      generateTrg1MemInstruction(cg, loadop, node, oldValueReg, TR::MemoryReference::createWithDisplacement(cg, addressReg, 0));

      if (valueReg == NULL)
         {
         if (!negate)
            {
            generateTrg1Src1ImmInstruction(cg, (is64Bit ? TR::InstOpCode::addimmx : TR::InstOpCode::addimmw), node, newValueReg, oldValueReg, value);
            }
         else
            {
            generateTrg1Src1ImmInstruction(cg, (is64Bit ? TR::InstOpCode::subimmx : TR::InstOpCode::subimmw), node, newValueReg, oldValueReg, -value);
            }
         }
      else
         {
         generateTrg1Src2Instruction(cg, (is64Bit ? TR::InstOpCode::addx : TR::InstOpCode::addw), node, newValueReg, oldValueReg, valueReg);
         }
      // store release exclusive register
      auto storeop = is64Bit ? TR::InstOpCode::stlxrx : TR::InstOpCode::stlxrw;
      generateTrg1MemSrc1Instruction(cg, storeop, node, tempReg, TR::MemoryReference::createWithDisplacement(cg, addressReg, 0), newValueReg);
      generateCompareBranchInstruction(cg, TR::InstOpCode::cbnzx, node, tempReg, loopLabel);

      generateSynchronizationInstruction(cg, TR::InstOpCode::dmb, node, 0xB); // dmb ish

      //Set the conditions and dependencies
      const int numDeps = (valueReg != NULL) ? 5 : 4;
      auto conditions = new (cg->trHeapMemory()) TR::RegisterDependencyConditions(0, numDeps, cg->trMemory());

      conditions->addPostCondition(newValueReg, TR::RealRegister::NoReg);
      conditions->addPostCondition(oldValueReg, TR::RealRegister::NoReg);
      conditions->addPostCondition(addressReg, TR::RealRegister::NoReg);
      conditions->addPostCondition(tempReg, TR::RealRegister::NoReg);
      if (valueReg != NULL)
         {
         conditions->addPostCondition(valueReg, TR::RealRegister::NoReg);
         }

      doneLabel->setEndInternalControlFlow();
      generateLabelInstruction(cg, TR::InstOpCode::label, node, doneLabel, conditions);

      cg->stopUsingRegister(newValueReg);
      cg->stopUsingRegister(tempReg);
      if (killValueReg)
         {
         cg->stopUsingRegister(valueReg);
         }
      }

   node->setRegister(oldValueReg);
   cg->decReferenceCount(addressNode);
   cg->decReferenceCount(valueNode);

   return oldValueReg;
   }

/**
 * @brief Inlines an intrinsic for calls to atomicSwapSymbol which are represented by a call node of the form for 32-bit (64-bit similar):
 *
 *     This implies `volatile` memory access mode.
 *
 *     @code
 *       icall <atomicSwapSymbol>
 *         <address>
 *         <value>
 *     @endcode
 *
 *     Which performs the following operation atomically:
 *
 *     @code
 *       temp = [address]
 *       [address] = <value>
 *       return temp
 *     @endcode
 *
 * @param node: The respective (i|l)call node.
 * @param   cg: The code generator used to generate the instructions.
 * @returns A register holding the original value in memory (before the swap) at the <address> location.
 */
TR::Register *intrinsicAtomicSwap(TR::Node *node, TR::CodeGenerator *cg)
   {
   TR::Node *addressNode = node->getChild(0);
   TR::Node *valueNode = node->getChild(1);

   TR::Register *addressReg = cg->evaluate(addressNode);
   TR::Register *valueReg = cg->evaluate(valueNode);
   TR::Register *oldValueReg = cg->allocateRegister();
   const bool is64Bit = valueNode->getDataType().isInt64();

   TR::Compilation *comp = cg->comp();
   static const bool disableLSE = feGetEnv("TR_aarch64DisableLSE") != NULL;
   if (comp->target().cpu.supportsFeature(OMR_FEATURE_ARM64_LSE) && (!disableLSE))
      {
      auto op = is64Bit ? TR::InstOpCode::swpalx : TR::InstOpCode::swpalw;
      /*
       * As Trg1MemSrc1Instruction was introduced to support ldxr/stxr instructions, target and source register convention
       * is somewhat confusing. Its `treg` register actually is a source register and `sreg` register is a target register.
       * This needs to be fixed at some point.
       */
      generateTrg1MemSrc1Instruction(cg, op, node, valueReg, TR::MemoryReference::createWithDisplacement(cg, addressReg, 0), oldValueReg);
      }
   else
      {
      /*
      * Generating non-intuitive instruction sequence which uses load exclusive register
      * and store release exclusive register followed by full memory barrier.
      *
      * Because this atomic swap has `volatile` semantics,
      * no loads/stores before this sequence can be reordred after it and
      * no loads/stores after it can be reordered before it.
      *
      * loop:
      *    ldxrx   oldValueReg, [addressReg]
      *    stlxrx  tempReg, valueReg, [addressReg]
      *    cbnzx   tempReg, loop
      *    dmb     ish
      *
      * For rationale behind this instruction sequence,
      * see https://patchwork.kernel.org/project/linux-arm-kernel/patch/1391516953-14541-1-git-send-email-will.deacon@arm.com/
      *
      */

      TR::Register *tempReg = cg->allocateRegister();
      TR::LabelSymbol *doneLabel = TR::LabelSymbol::create(cg->trHeapMemory(), cg);
      TR::LabelSymbol *loopLabel = TR::LabelSymbol::create(cg->trHeapMemory(), cg);

      loopLabel->setStartInternalControlFlow();
      generateLabelInstruction(cg, TR::InstOpCode::label, node, loopLabel);

      // load acquire exclusive register
      auto loadop = is64Bit ? TR::InstOpCode::ldxrx : TR::InstOpCode::ldxrw;
      generateTrg1MemInstruction(cg, loadop, node, oldValueReg, TR::MemoryReference::createWithDisplacement(cg, addressReg, 0));

      // store release exclusive register
      auto storeop = is64Bit ? TR::InstOpCode::stlxrx : TR::InstOpCode::stlxrw;
      generateTrg1MemSrc1Instruction(cg, storeop, node, tempReg, TR::MemoryReference::createWithDisplacement(cg, addressReg, 0), valueReg);
      generateCompareBranchInstruction(cg, TR::InstOpCode::cbnzx, node, tempReg, loopLabel);

      generateSynchronizationInstruction(cg, TR::InstOpCode::dmb, node, 0xB); // dmb ish

      //Set the conditions and dependencies
      auto conditions = new (cg->trHeapMemory()) TR::RegisterDependencyConditions(0, 4, cg->trMemory());

      conditions->addPostCondition(oldValueReg, TR::RealRegister::NoReg);
      conditions->addPostCondition(addressReg, TR::RealRegister::NoReg);
      conditions->addPostCondition(valueReg, TR::RealRegister::NoReg);
      conditions->addPostCondition(tempReg, TR::RealRegister::NoReg);

      doneLabel->setEndInternalControlFlow();
      generateLabelInstruction(cg, TR::InstOpCode::label, node, doneLabel, conditions);

      cg->stopUsingRegister(tempReg);
      }

   node->setRegister(oldValueReg);

   cg->decReferenceCount(addressNode);
   cg->decReferenceCount(valueNode);

   return oldValueReg;
   }

bool OMR::ARM64::CodeGenerator::inlineDirectCall(TR::Node *node, TR::Register *&resultReg)
   {
   TR::CodeGenerator *cg = self();
   TR::Compilation *comp = cg->comp();
   TR::SymbolReference* symRef = node->getSymbolReference();

   if (symRef && symRef->getSymbol()->castToMethodSymbol()->isInlinedByCG())
      {
      if (comp->getSymRefTab()->isNonHelper(symRef, TR::SymbolReferenceTable::atomicAddSymbol))
         {
         resultReg = intrinsicAtomicAdd(node, cg);
         return true;
         }
      else if (comp->getSymRefTab()->isNonHelper(symRef, TR::SymbolReferenceTable::atomicFetchAndAddSymbol))
         {
         resultReg = intrinsicAtomicFetchAndAdd(node, cg);
         return true;
         }
      else if (comp->getSymRefTab()->isNonHelper(symRef, TR::SymbolReferenceTable::atomicSwapSymbol))
         {
         resultReg = intrinsicAtomicSwap(node, cg);
         return true;
         }
      }

   return false;
   }
