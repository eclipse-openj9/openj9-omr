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

#include "il/IL.hpp"
#include "il/ILOpCodes.hpp"
#include "infra/Assert.hpp"


TR::ILOpCodes OMR::IL::opCodesForConst[] =
   {
   TR::BadILOp,  // NoType
   TR::bconst,   // Int8
   TR::sconst,   // Int16
   TR::iconst,   // Int32
   TR::lconst,   // Int64
   TR::fconst,   // Float
   TR::dconst,   // Double
   TR::aconst,   // Address
   TR::vconst,   // VectorInt8
   TR::vconst,   // VectorInt16
   TR::vconst,   // VectorInt32
   TR::vconst,   // VectorInt64
   TR::vconst,   // VectorFloat
   TR::vconst,   // VectorDouble
   TR::BadILOp,  // TR::Aggregate
   };

TR::ILOpCodes OMR::IL::opCodesForDirectLoad[] =
   {
   TR::BadILOp,  // NoType
   TR::bload,    // Int8
   TR::sload,    // Int16
   TR::iload,    // Int32
   TR::lload,    // Int64
   TR::fload,    // Float
   TR::dload,    // Double
   TR::aload,    // Address
   TR::vload,    // VectorInt8
   TR::vload,    // VectorInt16
   TR::vload,    // VectorInt32
   TR::vload,    // VectorInt64
   TR::vload,    // VectorFloat
   TR::vload,    // VectorDouble
   TR::BadILOp,  // TR::Aggregate
   };

TR::ILOpCodes OMR::IL::opCodesForDirectStore[] =
   {
   TR::BadILOp,  // NoType
   TR::bstore,   // Int8
   TR::sstore,   // Int16
   TR::istore,   // Int32
   TR::lstore,   // Int64
   TR::fstore,   // Float
   TR::dstore,   // Double
   TR::astore,   // Address
   TR::vstore,   // VectorInt8
   TR::vstore,   // VectorInt16
   TR::vstore,   // VectorInt32
   TR::vstore,   // VectorInt64
   TR::vstore,   // VectorFloat
   TR::vstore,   // VectorDouble
   TR::BadILOp,  // TR::Aggregate
   };

TR::ILOpCodes OMR::IL::opCodesForIndirectLoad[] =
   {
   TR::BadILOp,  // NoType
   TR::bloadi,   // Int8
   TR::sloadi,   // Int16
   TR::iloadi,   // Int32
   TR::lloadi,   // Int64
   TR::floadi,   // Float
   TR::dloadi,   // Double
   TR::aloadi,   // Address
   TR::vloadi,   // TR::VectorInt8
   TR::vloadi,   // TR::VectorInt16
   TR::vloadi,   // TR::VectorInt32
   TR::vloadi,   // TR::VectorInt64
   TR::vloadi,   // TR::VectorFloat
   TR::vloadi,   // TR::VectorDouble
   TR::BadILOp,  // TR::Aggregate
   };

TR::ILOpCodes OMR::IL::opCodesForIndirectStore[] =
   {
   TR::BadILOp,  // NoType
   TR::bstorei,  // Int8
   TR::sstorei,  // Int16
   TR::istorei,  // Int32
   TR::lstorei,  // Int64
   TR::fstorei,  // Float
   TR::dstorei,  // Double
   TR::astorei,  // Address
   TR::vstorei,  // TR::VectorInt8
   TR::vstorei,  // TR::VectorInt16
   TR::vstorei,  // TR::VectorInt32
   TR::vstorei,  // TR::VectorInt64
   TR::vstorei,  // TR::VectorFloat
   TR::vstorei,  // TR::VectorDouble
   TR::BadILOp,  // TR::Aggregate
   };

TR::ILOpCodes OMR::IL::opCodesForIndirectArrayLoad[] =
   {
   TR::BadILOp,  // NoType
   TR::bloadi,   // Int8
   TR::sloadi,   // Int16
   TR::iloadi,   // Int32
   TR::lloadi,   // Int64
   TR::floadi,   // Float
   TR::dloadi,   // Double
   TR::aloadi,   // Address
   TR::vloadi,   // TR::VectorInt8
   TR::vloadi,   // TR::VectorInt16
   TR::vloadi,   // TR::VectorInt32
   TR::vloadi,   // TR::VectorInt64
   TR::vloadi,   // TR::VectorFloat
   TR::vloadi,   // TR::VectorDouble
   TR::BadILOp,  // TR::Aggregate
   };

TR::ILOpCodes OMR::IL::opCodesForIndirectArrayStore[] =
   {
   TR::BadILOp,  // NoType
   TR::bstorei,  // Int8
   TR::sstorei,  // Int16
   TR::istorei,  // Int32
   TR::lstorei,  // Int64
   TR::fstorei,  // Float
   TR::dstorei,  // Double
   TR::astorei,  // Address
   TR::vstorei,  // TR::VectorInt8
   TR::vstorei,  // TR::VectorInt16
   TR::vstorei,  // TR::VectorInt32
   TR::vstorei,  // TR::VectorInt64
   TR::vstorei,  // TR::VectorFloat
   TR::vstorei,  // TR::VectorDouble
   TR::BadILOp,  // TR::Aggregate
   };

TR::ILOpCodes OMR::IL::opCodesForRegisterLoad[] =
   {
   TR::BadILOp,   // NoType
   TR::bRegLoad,  // Int8
   TR::sRegLoad,  // Int16
   TR::iRegLoad,  // Int32
   TR::lRegLoad,  // Int64
   TR::fRegLoad,  // Float
   TR::dRegLoad,  // Double
   TR::aRegLoad,  // Address
   TR::vsRegLoad, // TR::VectorInt8
   TR::vbRegLoad, // TR::VectorInt16
   TR::viRegLoad, // TR::VectorInt32
   TR::vlRegLoad, // TR::VectorInt64
   TR::vfRegLoad, // TR::VectorFloat
   TR::vdRegLoad, // TR::VectorDouble
   TR::BadILOp,   // TR::Aggregate
   };

TR::ILOpCodes OMR::IL::opCodesForRegisterStore[] =
   {
   TR::BadILOp,    // NoType
   TR::bRegStore,  // Int8
   TR::sRegStore,  // Int16
   TR::iRegStore,  // Int32
   TR::lRegStore,  // Int64
   TR::fRegStore,  // Float
   TR::dRegStore,  // Double
   TR::aRegStore,  // Address
   TR::vbRegStore, // TR::VectorInt8
   TR::vsRegStore, // TR::VectorInt16
   TR::viRegStore, // TR::VectorInt32
   TR::vlRegStore, // TR::VectorInt64
   TR::vfRegStore, // TR::VectorFloat
   TR::vdRegStore, // TR::VectorDouble
   TR::BadILOp,    // TR::Aggregate
   };

TR::ILOpCodes OMR::IL::opCodesForCompareEquals[] =
   {
   TR::BadILOp,  // NoType
   TR::bcmpeq,   // Int8
   TR::scmpeq,   // Int16
   TR::icmpeq,   // Int32
   TR::lcmpeq,   // Int64
   TR::fcmpeq,   // Float
   TR::dcmpeq,   // Double
   TR::acmpeq,   // Address
   TR::vcmpeq,   // TR::VectorInt8
   TR::vcmpeq,   // TR::VectorInt16
   TR::vcmpeq,   // TR::VectorInt32
   TR::vcmpeq,   // TR::VectorInt64
   TR::vcmpeq,   // TR::VectorFloat
   TR::vcmpeq,   // TR::VectorDouble
   TR::BadILOp,  // TR::Aggregate
   };

TR::ILOpCodes OMR::IL::opCodesForIfCompareEquals[] =
   {
   TR::BadILOp,   // NoType
   TR::ifbcmpeq,  // Int8
   TR::ifscmpeq,  // Int16
   TR::ificmpeq,  // Int32
   TR::iflcmpeq,  // Int64
   TR::iffcmpeq,  // Float
   TR::ifdcmpeq,  // Double
   TR::ifacmpeq,  // Address
   TR::BadILOp,   // TR::VectorInt8
   TR::BadILOp,   // TR::VectorInt16
   TR::BadILOp,   // TR::VectorInt32
   TR::BadILOp,   // TR::VectorInt64
   TR::BadILOp,   // TR::VectorFloat
   TR::BadILOp,   // TR::VectorDouble
   TR::BadILOp,   // TR::Aggregate
   };

TR::ILOpCodes OMR::IL::opCodesForCompareNotEquals[] =
   {
   TR::BadILOp,  // NoType
   TR::bcmpne,   // Int8
   TR::scmpne,   // Int16
   TR::icmpne,   // Int32
   TR::lcmpne,   // Int64
   TR::fcmpne,   // Float
   TR::dcmpne,   // Double
   TR::acmpne,   // Address
   TR::vcmpne,   // TR::VectorInt8
   TR::vcmpne,   // TR::VectorInt16
   TR::vcmpne,   // TR::VectorInt32
   TR::vcmpne,   // TR::VectorInt64
   TR::vcmpne,   // TR::VectorFloat
   TR::vcmpne,   // TR::VectorDouble
   TR::BadILOp,  // TR::Aggregate
   };

TR::ILOpCodes OMR::IL::opCodesForIfCompareNotEquals[] =
   {
   TR::BadILOp,   // NoType
   TR::ifbcmpne,  // Int8
   TR::ifscmpne,  // Int16
   TR::ificmpne,  // Int32
   TR::iflcmpne,  // Int64
   TR::iffcmpne,  // Float
   TR::ifdcmpne,  // Double
   TR::ifacmpne,  // Address
   TR::BadILOp,   // TR::VectorInt8
   TR::BadILOp,   // TR::VectorInt16
   TR::BadILOp,   // TR::VectorInt32
   TR::BadILOp,   // TR::VectorInt64
   TR::BadILOp,   // TR::VectorFloat
   TR::BadILOp,   // TR::VectorDouble
   TR::BadILOp,   // TR::Aggregate
   };

TR::ILOpCodes OMR::IL::opCodesForCompareLessThan[] =
   {
   TR::BadILOp,  // NoType
   TR::bcmplt,   // Int8
   TR::scmplt,   // Int16
   TR::icmplt,   // Int32
   TR::lcmplt,   // Int64
   TR::fcmplt,   // Float
   TR::dcmplt,   // Double
   TR::acmplt,   // Address
   TR::vcmplt,   // TR::VectorInt8
   TR::vcmplt,   // TR::VectorInt16
   TR::vcmplt,   // TR::VectorInt32
   TR::vcmplt,   // TR::VectorInt64
   TR::vcmplt,   // TR::VectorFloat
   TR::vcmplt,   // TR::VectorDouble
   TR::BadILOp,  // TR::Aggregate
   };

TR::ILOpCodes OMR::IL::opCodesForIfCompareLessThan[] =
   {
   TR::BadILOp,   // NoType
   TR::ifbcmplt,  // Int8
   TR::ifscmplt,  // Int16
   TR::ificmplt,  // Int32
   TR::iflcmplt,  // Int64
   TR::iffcmplt,  // Float
   TR::ifdcmplt,  // Double
   TR::ifacmplt,  // Address
   TR::BadILOp,   // TR::VectorInt8
   TR::BadILOp,   // TR::VectorInt16
   TR::BadILOp,   // TR::VectorInt32
   TR::BadILOp,   // TR::VectorInt64
   TR::BadILOp,   // TR::VectorFloat
   TR::BadILOp,   // TR::VectorDouble
   TR::BadILOp,   // TR::Aggregate
   };

TR::ILOpCodes OMR::IL::opCodesForCompareGreaterThan[] =
   {
   TR::BadILOp,  // NoType
   TR::bcmpgt,   // Int8
   TR::scmpgt,   // Int16
   TR::icmpgt,   // Int32
   TR::lcmpgt,   // Int64
   TR::fcmpgt,   // Float
   TR::dcmpgt,   // Double
   TR::acmpgt,   // Address
   TR::vcmpgt,   // TR::VectorInt8
   TR::vcmpgt,   // TR::VectorInt16
   TR::vcmpgt,   // TR::VectorInt32
   TR::vcmpgt,   // TR::VectorInt64
   TR::vcmpgt,   // TR::VectorFloat
   TR::vcmpgt,   // TR::VectorDouble
   TR::BadILOp,  // TR::Aggregate
   };

TR::ILOpCodes OMR::IL::opCodesForIfCompareGreaterThan[] =
   {
   TR::BadILOp,   // NoType
   TR::ifbcmpgt,  // Int8
   TR::ifscmpgt,  // Int16
   TR::ificmpgt,  // Int32
   TR::iflcmpgt,  // Int64
   TR::iffcmpgt,  // Float
   TR::ifdcmpgt,  // Double
   TR::ifacmpgt,  // Address
   TR::BadILOp,   // TR::VectorInt8
   TR::BadILOp,   // TR::VectorInt16
   TR::BadILOp,   // TR::VectorInt32
   TR::BadILOp,   // TR::VectorInt64
   TR::BadILOp,   // TR::VectorFloat
   TR::BadILOp,   // TR::VectorDouble
   TR::BadILOp,   // TR::Aggregate
   };

static_assert(TR::NumOMRTypes == (sizeof(OMR::IL::opCodesForConst) / sizeof(OMR::IL::opCodesForConst[0])),
              "OMR::IL::opCodesForConst is not the correct size");

static_assert(TR::NumOMRTypes == (sizeof(OMR::IL::opCodesForDirectLoad) / sizeof(OMR::IL::opCodesForDirectLoad[0])),
              "OMR::IL::opCodesForDirectLoad is not the correct size");

static_assert(TR::NumOMRTypes == (sizeof(OMR::IL::opCodesForDirectStore) / sizeof(OMR::IL::opCodesForDirectStore[0])),
              "OMR::IL::opCodesForDirectStore is not the correct size");

static_assert(TR::NumOMRTypes == (sizeof(OMR::IL::opCodesForIndirectLoad) / sizeof(OMR::IL::opCodesForIndirectLoad[0])),
              "OMR::IL::opCodesForIndirectLoad is not the correct size");

static_assert(TR::NumOMRTypes == (sizeof(OMR::IL::opCodesForIndirectStore) / sizeof(OMR::IL::opCodesForIndirectStore[0])),
              "OMR::IL::opCodesForIndirectStore is not the correct size");

static_assert(TR::NumOMRTypes == (sizeof(OMR::IL::opCodesForIndirectArrayLoad) / sizeof(OMR::IL::opCodesForIndirectArrayLoad[0])),
              "OMR::IL::opCodesForIndirectArrayLoad is not the correct size");

static_assert(TR::NumOMRTypes == (sizeof(OMR::IL::opCodesForIndirectArrayStore) / sizeof(OMR::IL::opCodesForIndirectArrayStore[0])),
              "OMR::IL::opCodesForIndirectArrayStore is not the correct size");

static_assert(TR::NumOMRTypes == (sizeof(OMR::IL::opCodesForRegisterLoad) / sizeof(OMR::IL::opCodesForRegisterLoad[0])),
              "OMR::IL::opCodesForRegisterLoad is not the correct size");

static_assert(TR::NumOMRTypes == (sizeof(OMR::IL::opCodesForRegisterStore) / sizeof(OMR::IL::opCodesForRegisterStore[0])),
              "OMR::IL::opCodesForRegisterStore is not the correct size");

static_assert(TR::NumOMRTypes == (sizeof(OMR::IL::opCodesForCompareEquals) / sizeof(OMR::IL::opCodesForCompareEquals[0])),
              "OMR::IL::opCodesForCompareEquals is not the correct size");

static_assert(TR::NumOMRTypes == (sizeof(OMR::IL::opCodesForIfCompareEquals) / sizeof(OMR::IL::opCodesForIfCompareEquals[0])),
              "OMR::IL::opCodesForIfCompareEquals is not the correct size");

static_assert(TR::NumOMRTypes == (sizeof(OMR::IL::opCodesForCompareNotEquals) / sizeof(OMR::IL::opCodesForCompareNotEquals[0])),
              "OMR::IL::opCodesForCompareNotEquals is not the correct size");

static_assert(TR::NumOMRTypes == (sizeof(OMR::IL::opCodesForIfCompareNotEquals) / sizeof(OMR::IL::opCodesForIfCompareNotEquals[0])),
              "OMR::IL::opCodesForIfCompareNotEquals is not the correct size");

static_assert(TR::NumOMRTypes == (sizeof(OMR::IL::opCodesForCompareLessThan) / sizeof(OMR::IL::opCodesForCompareLessThan[0])),
              "OMR::IL::opCodesForCompareLessThan is not the correct size");

static_assert(TR::NumOMRTypes == (sizeof(OMR::IL::opCodesForIfCompareLessThan) / sizeof(OMR::IL::opCodesForIfCompareLessThan[0])),
              "OMR::IL::opCodesForIfCompareLessThan is not the correct size");

static_assert(TR::NumOMRTypes == (sizeof(OMR::IL::opCodesForCompareGreaterThan) / sizeof(OMR::IL::opCodesForCompareGreaterThan[0])),
              "OMR::IL::opCodesForCompareGreaterThan is not the correct size");

static_assert(TR::NumOMRTypes == (sizeof(OMR::IL::opCodesForIfCompareGreaterThan) / sizeof(OMR::IL::opCodesForIfCompareGreaterThan[0])),
              "OMR::IL::opCodesForIfCompareGreaterThan is not the correct size");


TR::ILOpCodes
OMR::IL::opCodeForCorrespondingIndirectLoad(TR::ILOpCodes loadOpCode)
   {
   switch (loadOpCode)
      {
      case TR::bloadi: return TR::bstorei;
      case TR::sloadi: return TR::sstorei;
      case TR::iloadi: return TR::istorei;
      case TR::lloadi: return TR::lstorei;
      case TR::floadi: return TR::fstorei;
      case TR::dloadi: return TR::dstorei;
      case TR::aloadi: return TR::astorei;
      case TR::vloadi: return TR::vstorei;
      case TR::cloadi: return TR::cstorei;
      case TR::buloadi: return TR::bustorei;
      case TR::iuload: return TR::iustore;
      case TR::iuloadi: return TR::iustorei;
      case TR::luloadi: return TR::lustorei;

      default: break;
      }

   TR_ASSERT(0, "no corresponding store opcode for specified load opcode");
   return TR::BadILOp;
   }


TR::ILOpCodes
OMR::IL::opCodeForCorrespondingIndirectStore(TR::ILOpCodes storeOpCode)
   {

   switch (storeOpCode)
      {
      case TR::bstorei:  return TR::bloadi;
      case TR::sstorei:  return TR::sloadi;
      case TR::istorei:  return TR::iloadi;
      case TR::lstorei:  return TR::lloadi;
      case TR::fstorei:  return TR::floadi;
      case TR::dstorei:  return TR::dloadi;
      case TR::astorei:  return TR::aloadi;
      case TR::wrtbari:  return TR::aloadi;
      case TR::vstorei:  return TR::vloadi;
      case TR::cstorei: return TR::cloadi;
      case TR::bustorei: return TR::buloadi;
      case TR::iustore: return TR::iuload;
      case TR::iustorei: return TR::iuloadi;
      case TR::lustorei: return TR::luloadi;

      default: break;
      }

   TR_ASSERT(0, "no corresponding load opcode for specified store opcode");
   return TR::BadILOp;
   }


TR::ILOpCodes
OMR::IL::opCodeForConst(TR::DataTypes dt)
   {
   TR_ASSERT(dt < TR::NumOMRTypes, "unexpected opcode");

   return OMR::IL::opCodesForConst[dt];
   }

TR::ILOpCodes
OMR::IL::opCodeForDirectLoad(TR::DataTypes dt)
   {
   TR_ASSERT(dt < TR::NumOMRTypes, "unexpected opcode");

   return OMR::IL::opCodesForDirectLoad[dt];
   }

TR::ILOpCodes
OMR::IL::opCodeForDirectStore(TR::DataTypes dt)
   {
   TR_ASSERT(dt < TR::NumOMRTypes, "unexpected opcode");

   return OMR::IL::opCodesForDirectStore[dt];
   }

TR::ILOpCodes
OMR::IL::opCodeForIndirectLoad(TR::DataTypes dt)
   {
   TR_ASSERT(dt < TR::NumOMRTypes, "unexpected opcode");

   return OMR::IL::opCodesForIndirectLoad[dt];
   }

TR::ILOpCodes
OMR::IL::opCodeForIndirectStore(TR::DataTypes dt)
   {
   TR_ASSERT(dt < TR::NumOMRTypes, "unexpected opcode");

   return OMR::IL::opCodesForIndirectStore[dt];
   }

TR::ILOpCodes
OMR::IL::opCodeForIndirectArrayLoad(TR::DataTypes dt)
   {
   TR_ASSERT(dt < TR::NumOMRTypes, "unexpected opcode");

   return OMR::IL::opCodesForIndirectArrayLoad[dt];
   }

TR::ILOpCodes
OMR::IL::opCodeForIndirectArrayStore(TR::DataTypes dt)
   {
   TR_ASSERT(dt < TR::NumOMRTypes, "unexpected opcode");

   return OMR::IL::opCodesForIndirectArrayStore[dt];
   }

TR::ILOpCodes
OMR::IL::opCodeForRegisterLoad(TR::DataTypes dt)
   {
   TR_ASSERT(dt < TR::NumOMRTypes, "unexpected opcode");

   return OMR::IL::opCodesForRegisterLoad[dt];
   }

TR::ILOpCodes
OMR::IL::opCodeForRegisterStore(TR::DataTypes dt)
   {
   TR_ASSERT(dt < TR::NumOMRTypes, "unexpected opcode");

   return OMR::IL::opCodesForRegisterStore[dt];
   }

TR::ILOpCodes
OMR::IL::opCodeForCompareEquals(TR::DataTypes dt)
   {
   TR_ASSERT(dt < TR::NumOMRTypes, "unexpected opcode");

   return OMR::IL::opCodesForCompareEquals[dt];
   }

TR::ILOpCodes
OMR::IL::opCodeForIfCompareEquals(TR::DataTypes dt)
   {
   TR_ASSERT(dt < TR::NumOMRTypes, "unexpected opcode");

   return OMR::IL::opCodesForIfCompareEquals[dt];
   }

TR::ILOpCodes
OMR::IL::opCodeForCompareNotEquals(TR::DataTypes dt)
   {
   TR_ASSERT(dt < TR::NumOMRTypes, "unexpected opcode");

   return OMR::IL::opCodesForCompareNotEquals[dt];
   }

TR::ILOpCodes
OMR::IL::opCodeForIfCompareNotEquals(TR::DataTypes dt)
   {
   TR_ASSERT(dt < TR::NumOMRTypes, "unexpected opcode");

   return OMR::IL::opCodesForIfCompareNotEquals[dt];
   }

TR::ILOpCodes
OMR::IL::opCodeForCompareLessThan(TR::DataTypes dt)
   {
   TR_ASSERT(dt < TR::NumOMRTypes, "unexpected opcode");

   return OMR::IL::opCodesForCompareLessThan[dt];
   }

TR::ILOpCodes
OMR::IL::opCodeForIfCompareLessThan(TR::DataTypes dt)
   {
   TR_ASSERT(dt < TR::NumOMRTypes, "unexpected opcode");

   return OMR::IL::opCodesForIfCompareLessThan[dt];
   }

TR::ILOpCodes
OMR::IL::opCodeForCompareGreaterThan(TR::DataTypes dt)
   {
   TR_ASSERT(dt < TR::NumOMRTypes, "unexpected opcode");

   return OMR::IL::opCodesForCompareGreaterThan[dt];
   }

TR::ILOpCodes
OMR::IL::opCodeForIfCompareGreaterThan(TR::DataTypes dt)
   {
   TR_ASSERT(dt < TR::NumOMRTypes, "unexpected opcode");

   return OMR::IL::opCodesForIfCompareGreaterThan[dt];
   }
