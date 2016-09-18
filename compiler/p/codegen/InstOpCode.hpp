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

#ifndef TR_POWER_INSTOPCODE_INCL
#define TR_POWER_INSTOPCODE_INCL

#include "codegen/OMRInstOpCode.hpp"

#ifndef OPS_MAX
#define OPS_MAX    TR::InstOpCode::PPCNumOpCodes
#endif

namespace TR
{

class InstOpCode: public OMR::InstOpCodeConnector
   {
   public:
   InstOpCode()	: 			OMR::InstOpCodeConnector(bad) {}
   InstOpCode(TR::InstOpCode::Mnemonic m): OMR::InstOpCodeConnector(m) {}

   TR::InstOpCode::Mnemonic getOpCodeValue()                 {return _mnemonic;}
   TR::InstOpCode::Mnemonic setOpCodeValue(TR::InstOpCode::Mnemonic op) {return (_mnemonic = op);}
   TR::InstOpCode::Mnemonic getRecordFormOpCodeValue()       {return (TR::InstOpCode::Mnemonic)(_mnemonic+1);}

   };
}
#endif
