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

#include "codegen/Linkage.hpp"            // for Linkage
#include "codegen/RegisterConstants.hpp"  // for TR_RegisterKinds, etc
#include "il/ILOps.hpp"                   // for ILOpCode
#include "il/Node.hpp"                    // for Node
#include "il/symbol/ParameterSymbol.hpp"  // for ParameterSymbol

TR::Linkage *
OMR::Linkage::self()
   {
   return static_cast<TR::Linkage*>(this);
   }

bool
OMR::Linkage::hasToBeOnStack(TR::ParameterSymbol *parm)
   {
   // The "this" pointer of synchronized and JVMPI methods must be on the stack.
   // TODO: The PPC implementation is superior, but it uses
   // interfaces currently only supported by TR::PPCPrivateLinkage.  The first
   // order of business: currently, every platform's Linkage can access the
   // Compilation and/or CodeGenerator, but the common one can't.  This
   // prevents us from checking that the current method is synchronized and so
   // forth, and makes the implementation below more conservative than it could
   // be.  Once we move that facility to TR::Linkage, and this function could be
   // more selective.
   //
   return(parm->getAllocatedIndex()>=0       &&
          ( (  parm->getLinkageRegisterIndex()==0 &&
               parm->isCollectedReference()
            ) ||
            parm->isParmHasToBeOnStack()
          )
         );
   }

TR_RegisterKinds
OMR::Linkage::argumentRegisterKind(TR::Node *argumentNode)
   {
   if (argumentNode->getOpCode().isFloatingPoint())
      return TR_FPR;
   else if (argumentNode->getOpCode().isVector())
      return TR_VRF;
   else
      return TR_GPR;
   }
