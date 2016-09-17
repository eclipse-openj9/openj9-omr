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

#include "il/OMRILOps.hpp"

#include <stdint.h>             // for int32_t
#include <stddef.h>
#include "env/CompilerEnv.hpp"
#include "il/DataTypes.hpp"     // for DataTypes::NumCoreTypes, DataTypes
#include "il/ILOpCodes.hpp"     // for ILOpCodes
#include "il/ILOps.hpp"         // for OpCodeProperties, TR::ILOpCode, etc
#include "il/ILProps.hpp"       // for ILOpCodeProperties.hpp
#include "infra/Assert.hpp"     // for TR_ASSERT
#include "infra/Flags.hpp"      // for flags32_t

/**
 * Table of opcode properties.
 *
 * \note A note on the syntax of the table below. The commented field names,
 * or rather, the designated initializer syntax would actually work in
 * C99, however we can't use that feature because microsoft compiler
 * still doesn't support C99. If microsoft ever supports this we can
 * make the table even more robust and type safe by uncommenting the
 * field initializers.
 */
OMR::OpCodeProperties OMR::ILOpCode::_opCodeProperties[] =
   {
#include "il/ILOpCodeProperties.hpp"
   };

void
OMR::ILOpCode::checkILOpArrayLengths()
   {
   for (int i = TR::FirstOMROp; i < TR::NumIlOps; i++)
      {
      TR::ILOpCodes opCode = (TR::ILOpCodes)i;
      TR::ILOpCode  op(opCode);
      OMR::OpCodeProperties &props = TR::ILOpCode::_opCodeProperties[opCode];

      TR_ASSERT(props.opcode == opCode, "_opCodeProperties table out of sync at index %d, has %s\n", i, op.getName());
      }
   }

// FIXME: We should put the smarts in the getSize() routine in TR::DataType
// instead of modifying this large table that should in fact be read-only
//
void
OMR::ILOpCode::setTarget()
   {
   if (TR::Compiler->target.is64Bit())
      {
      for (int32_t i = 0; i < TR::NumIlOps; ++i)
         {
         flags32_t *tp = (flags32_t*)(&_opCodeProperties[i].typeProperties); // so ugly
         if (tp->getValue() == ILTypeProp::Reference)
            {
            tp->reset(ILTypeProp::Size_Mask);
            tp->set(ILTypeProp::Size_8);
            }
         }
      TR::DataType::setSize(TR::Address, 8);
      }
   else
      {
      for (int32_t i = 0; i < TR::NumIlOps; ++i)
         {
         flags32_t *tp = (flags32_t*)(&_opCodeProperties[i].typeProperties); // so ugly
         if (tp->getValue() == ILTypeProp::Reference)
            {
            tp->reset(ILTypeProp::Size_Mask);
            tp->set(ILTypeProp::Size_4);
            }
         }
      TR::DataType::setSize(TR::Address, 4);
      }
   }


#if !defined(_MSC_VER)
// the microsoft compiler cannot handle these templates
namespace
   {
   enum signedUnsigned
      {
      isSigned,
      isUnSigned
      };
   // originally, getCompareOp0, getCompareOp1 and getCompareOp2
   // were all called getCompareOp.  However, this caused a
   // compiler crash on AIX.
   //
   template<enum signedUnsigned>
   class getCompareOpCodeHelper_
      {
      public:
      template<enum TR::DataTypes, enum TR_ComparisonTypes>
         static TR::ILOpCodes getCompareOp0();
      template<enum TR::DataTypes>
      static TR::ILOpCodes getCompareOp1(enum TR_ComparisonTypes ct);
      static TR::ILOpCodes getCompareOp2(enum TR::DataTypes dt,
                                        enum TR_ComparisonTypes ct);
      };

   template<enum signedUnsigned s>
   template<enum TR::DataTypes, enum TR_ComparisonTypes>
   inline TR::ILOpCodes getCompareOpCodeHelper_<s>::
      getCompareOp0() { return TR::BadILOp; }

#define declCmp0(S, DT, CT, OP) \
   template<> template<> inline TR::ILOpCodes \
   getCompareOpCodeHelper_<S>::getCompareOp0<DT, CT>() { return OP; }
#define declCmp1(S, DT, OPBase) \
   declCmp0(S, DT, TR_cmpEQ, OPBase ## eq) \
   declCmp0(S, DT, TR_cmpNE, OPBase ## ne) \
   declCmp0(S, DT, TR_cmpLT, OPBase ## lt) \
   declCmp0(S, DT, TR_cmpLE, OPBase ## le) \
   declCmp0(S, DT, TR_cmpGT, OPBase ## gt) \
   declCmp0(S, DT, TR_cmpGE, OPBase ## ge)
#define declCmp2(DT, sOPBase, uOPBase) \
   declCmp1(isSigned, DT, sOPBase) \
   declCmp1(isUnSigned, DT, uOPBase)
#define declCmp3(DT, OPBase) declCmp1(isSigned, DT, OPBase)
declCmp2(TR::Int8,    TR::bcmp, TR::bucmp)
declCmp2(TR::Int16,   TR::scmp, TR::sucmp)
declCmp2(TR::Int32,   TR::icmp, TR::iucmp)
declCmp2(TR::Int64,   TR::lcmp, TR::lucmp)
declCmp3(TR::Float,   TR::fcmp)
declCmp3(TR::Double,  TR::dcmp)
declCmp3(TR::Address, TR::acmp)
#undef declCmp3
#undef declCmp2
#undef declCmp1
#undef declCmp0

   template<signedUnsigned s>
   template<TR::DataTypes DT>
   inline TR::ILOpCodes getCompareOpCodeHelper_<s>::getCompareOp1(TR_ComparisonTypes ct)
      {
      switch (ct)
         {
         case TR_cmpEQ: return getCompareOp0<DT, TR_cmpEQ>();
         case TR_cmpNE: return getCompareOp0<DT, TR_cmpNE>();
         case TR_cmpLT: return getCompareOp0<DT, TR_cmpLT>();
         case TR_cmpLE: return getCompareOp0<DT, TR_cmpLE>();
         case TR_cmpGT: return getCompareOp0<DT, TR_cmpGT>();
         case TR_cmpGE: return getCompareOp0<DT, TR_cmpGE>();
         default: return TR::BadILOp;
         }
      }

   template<enum signedUnsigned s> inline
      TR::ILOpCodes getCompareOpCodeHelper_<s>::
      getCompareOp2(enum TR::DataTypes dt,
                    enum TR_ComparisonTypes ct)
      {
      switch (dt)
         {
         case TR::Int8:    return getCompareOp1<TR::Int8>(ct);
         case TR::Int16:   return getCompareOp1<TR::Int16>(ct);
         case TR::Int32:   return getCompareOp1<TR::Int32>(ct);
         case TR::Int64:   return getCompareOp1<TR::Int64>(ct);
         case TR::Float:   return getCompareOp1<TR::Float>(ct);
         case TR::Double:  return getCompareOp1<TR::Double>(ct);
         case TR::Address: return getCompareOp1<TR::Address>(ct);
         default: return TR::BadILOp;
         }
      }
   }

TR::ILOpCodes
OMR::ILOpCode::compareOpCode(enum TR::DataTypes dt,
                                        enum TR_ComparisonTypes ct,
                                        bool unsignedCompare)
   {
   return unsignedCompare ?
      getCompareOpCodeHelper_<isUnSigned>::getCompareOp2(dt, ct) :
      getCompareOpCodeHelper_<isSigned>::getCompareOp2(dt, ct);
   }

#else
// version for the microsoft compiler
TR::ILOpCodes
OMR::ILOpCode::compareOpCode(enum TR::DataTypes dt,
                                        enum TR_ComparisonTypes ct,
                                        bool unsignedCompare)
   {
   if (unsignedCompare)
      {
      switch(dt)
         {
         case TR::Int8:
            {
            switch(ct)
               {
               case TR_cmpEQ: return TR::bucmpeq;
               case TR_cmpNE: return TR::bucmpne;
               case TR_cmpLT: return TR::bucmplt;
               case TR_cmpLE: return TR::bucmple;
               case TR_cmpGT: return TR::bucmpgt;
               case TR_cmpGE: return TR::bucmpge;
               default: return TR::BadILOp;
               }
            break;
            }
         case TR::Int16:
            {
            switch(ct)
               {
               case TR_cmpEQ: return TR::sucmpeq;
               case TR_cmpNE: return TR::sucmpne;
               case TR_cmpLT: return TR::sucmplt;
               case TR_cmpLE: return TR::sucmple;
               case TR_cmpGT: return TR::sucmpgt;
               case TR_cmpGE: return TR::sucmpge;
               default: return TR::BadILOp;
               }
            break;
            }
         case TR::Int32:
            {
            switch(ct)
               {
               case TR_cmpEQ: return TR::iucmpeq;
               case TR_cmpNE: return TR::iucmpne;
               case TR_cmpLT: return TR::iucmplt;
               case TR_cmpLE: return TR::iucmple;
               case TR_cmpGT: return TR::iucmpgt;
               case TR_cmpGE: return TR::iucmpge;
               default: return TR::BadILOp;
               }
            break;
            }
         case TR::Int64:
            {
            switch(ct)
               {
               case TR_cmpEQ: return TR::lucmpeq;
               case TR_cmpNE: return TR::lucmpne;
               case TR_cmpLT: return TR::lucmplt;
               case TR_cmpLE: return TR::lucmple;
               case TR_cmpGT: return TR::lucmpgt;
               case TR_cmpGE: return TR::lucmpge;
               default: return TR::BadILOp;
               }
            break;
            }
         case TR::Float:
            {
            switch(ct)
               {
               default: return TR::BadILOp;
               }
            break;
            }
         case TR::Double:
            {
            switch(ct)
               {
               default: return TR::BadILOp;
               }
            break;
            }
         case TR::Address:
            {
            switch(ct)
               {
               default: return TR::BadILOp;
               }
            break;
            }
         default: return TR::BadILOp;
         }
      }
   else
      {
      switch(dt)
         {
         case TR::Int8:
            {
            switch(ct)
               {
               case TR_cmpEQ: return TR::bcmpeq;
               case TR_cmpNE: return TR::bcmpne;
               case TR_cmpLT: return TR::bcmplt;
               case TR_cmpLE: return TR::bcmple;
               case TR_cmpGT: return TR::bcmpgt;
               case TR_cmpGE: return TR::bcmpge;
               default: return TR::BadILOp;
               }
            break;
            }
         case TR::Int16:
            {
            switch(ct)
               {
               case TR_cmpEQ: return TR::scmpeq;
               case TR_cmpNE: return TR::scmpne;
               case TR_cmpLT: return TR::scmplt;
               case TR_cmpLE: return TR::scmple;
               case TR_cmpGT: return TR::scmpgt;
               case TR_cmpGE: return TR::scmpge;
               default: return TR::BadILOp;
               }
            break;
            }
         case TR::Int32:
            {
            switch(ct)
               {
               case TR_cmpEQ: return TR::icmpeq;
               case TR_cmpNE: return TR::icmpne;
               case TR_cmpLT: return TR::icmplt;
               case TR_cmpLE: return TR::icmple;
               case TR_cmpGT: return TR::icmpgt;
               case TR_cmpGE: return TR::icmpge;
               default: return TR::BadILOp;
               }
            break;
            }
         case TR::Int64:
            {
            switch(ct)
               {
               case TR_cmpEQ: return TR::lcmpeq;
               case TR_cmpNE: return TR::lcmpne;
               case TR_cmpLT: return TR::lcmplt;
               case TR_cmpLE: return TR::lcmple;
               case TR_cmpGT: return TR::lcmpgt;
               case TR_cmpGE: return TR::lcmpge;
               default: return TR::BadILOp;
               }
            break;
            }
         case TR::Float:
            {
            switch(ct)
               {
               case TR_cmpEQ: return TR::fcmpeq;
               case TR_cmpNE: return TR::fcmpne;
               case TR_cmpLT: return TR::fcmplt;
               case TR_cmpLE: return TR::fcmple;
               case TR_cmpGT: return TR::fcmpgt;
               case TR_cmpGE: return TR::fcmpge;
               default: return TR::BadILOp;
               }
            break;
            }
         case TR::Double:
            {
            switch(ct)
               {
               case TR_cmpEQ: return TR::dcmpeq;
               case TR_cmpNE: return TR::dcmpne;
               case TR_cmpLT: return TR::dcmplt;
               case TR_cmpLE: return TR::dcmple;
               case TR_cmpGT: return TR::dcmpgt;
               case TR_cmpGE: return TR::dcmpge;
               default: return TR::BadILOp;
               }
            break;
            }
         case TR::Address:
            {
            switch(ct)
               {
               case TR_cmpEQ: return TR::acmpeq;
               case TR_cmpNE: return TR::acmpne;
               case TR_cmpLT: return TR::acmplt;
               case TR_cmpLE: return TR::acmple;
               case TR_cmpGT: return TR::acmpgt;
               case TR_cmpGE: return TR::acmpge;
               default: return TR::BadILOp;
               }
            break;
            }
         default: return TR::BadILOp;
         }
      }
   return TR::BadILOp;
   }
#endif

