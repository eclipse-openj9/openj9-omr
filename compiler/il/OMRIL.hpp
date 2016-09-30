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
 ******************************************************************************/

#ifndef OMR_IL_INCL
#define OMR_IL_INCL

/*
 * The following #define and typedef must appear before any #includes in this file
 */
#ifndef OMR_IL_CONNECTOR
#define OMR_IL_CONNECTOR
namespace OMR { class IL; }
namespace OMR { typedef OMR::IL ILConnector; }
#endif

#include "infra/Annotations.hpp"
#include "il/ILOpCodes.hpp"
#include "il/DataTypes.hpp"

namespace OMR
{

class OMR_EXTENSIBLE IL
   {

   public:

   static TR::ILOpCodes opCodesForConst[];
   static TR::ILOpCodes opCodesForDirectLoad[];
   static TR::ILOpCodes opCodesForDirectStore[];
   static TR::ILOpCodes opCodesForIndirectLoad[];
   static TR::ILOpCodes opCodesForIndirectStore[];
   static TR::ILOpCodes opCodesForIndirectArrayLoad[];
   static TR::ILOpCodes opCodesForIndirectArrayStore[];
   static TR::ILOpCodes opCodesForRegisterLoad[];
   static TR::ILOpCodes opCodesForRegisterStore[];
   static TR::ILOpCodes opCodesForCompareEquals[];
   static TR::ILOpCodes opCodesForIfCompareEquals[];
   static TR::ILOpCodes opCodesForCompareNotEquals[];
   static TR::ILOpCodes opCodesForIfCompareNotEquals[];
   static TR::ILOpCodes opCodesForCompareLessThan[];
   static TR::ILOpCodes opCodesForIfCompareLessThan[];
   static TR::ILOpCodes opCodesForCompareGreaterThan[];
   static TR::ILOpCodes opCodesForIfCompareGreaterThan[];

   TR::ILOpCodes opCodeForCorrespondingIndirectLoad(TR::ILOpCodes loadOpCode);
   TR::ILOpCodes opCodeForCorrespondingIndirectStore(TR::ILOpCodes storeOpCode);

   TR::ILOpCodes opCodeForConst(TR::DataType dt);
   TR::ILOpCodes opCodeForDirectLoad(TR::DataType dt);
   TR::ILOpCodes opCodeForDirectStore(TR::DataType dt);
   TR::ILOpCodes opCodeForIndirectLoad(TR::DataType dt);
   TR::ILOpCodes opCodeForIndirectStore(TR::DataType dt);
   TR::ILOpCodes opCodeForIndirectArrayLoad(TR::DataType dt);
   TR::ILOpCodes opCodeForIndirectArrayStore(TR::DataType dt);
   TR::ILOpCodes opCodeForRegisterLoad(TR::DataType dt);
   TR::ILOpCodes opCodeForRegisterStore(TR::DataType dt);
   TR::ILOpCodes opCodeForCompareEquals(TR::DataType dt);
   TR::ILOpCodes opCodeForIfCompareEquals(TR::DataType dt);
   TR::ILOpCodes opCodeForCompareNotEquals(TR::DataType dt);
   TR::ILOpCodes opCodeForIfCompareNotEquals(TR::DataType dt);
   TR::ILOpCodes opCodeForCompareLessThan(TR::DataType dt);
   TR::ILOpCodes opCodeForIfCompareLessThan(TR::DataType dt);
   TR::ILOpCodes opCodeForCompareGreaterThan(TR::DataType dt);
   TR::ILOpCodes opCodeForIfCompareGreaterThan(TR::DataType dt);

   };

}

#endif
