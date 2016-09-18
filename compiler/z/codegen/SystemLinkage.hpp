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

#ifndef TR_SYSTEMLINKAGE_INCL
#define TR_SYSTEMLINKAGE_INCL

#include "codegen/S390SystemLinkage.hpp"

#include "codegen/Linkage.hpp"
#include "codegen/LinkageConventionsEnum.hpp"

namespace TR { class CodeGenerator; }

namespace TR
{

class SystemLinkage : public OMR::SystemLinkageConnector
   {
   public:

   SystemLinkage(TR::CodeGenerator * cg, TR_S390LinkageConventions elc=TR_S390LinkageDefault, TR_LinkageConventions lc=TR_System)
      : OMR::SystemLinkageConnector(cg, elc, lc) {}
   };
}

inline TR::SystemLinkage *
toSystemLinkage(TR::Linkage * l)
   {
   return (TR::SystemLinkage *) l;
   }

#endif
