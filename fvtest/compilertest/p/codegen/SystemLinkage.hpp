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

#ifndef TEST_POWER_SYSTEMLINKAGE_INCL
#define TEST_POWER_SYSTEMLINKAGE_INCL

#include "p/codegen/PPCSystemLinkage.hpp"

namespace TR { class CodeGenerator; }

namespace Test
{

namespace Power
{

class SystemLinkage : public TR_PPCSystemLinkage
   {
   public:

   SystemLinkage(TR::CodeGenerator *cg);
   };

} // namespace Power
} // namespace Test

#endif // !defined9TEST_PPC_SYSTEMLINKAGE_INCL
