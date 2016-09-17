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

#ifndef TR_Z_ZOSSYSTEMLINKAGE_BASE_INCL
#define TR_Z_ZOSSYSTEMLINKAGE_BASE_INCL

/*
 * The following #define and typedef must appear before any #includes in this file
 */
#ifndef TR_ZOSSYSTEMLINKAGE_BASE_CONNECTOR
#define TR_ZOSSYSTEMLINKAGE_BASE_CONNECTOR
namespace TR { namespace Z {class ZOSBaseSystemLinkage; } }
namespace TR { typedef TR::Z::ZOSBaseSystemLinkage ZOSBaseSystemLinkageConnector; }
#endif

#include <stdint.h>                            // for int32_t
#include "codegen/Linkage.hpp"
#include "codegen/LinkageConventionsEnum.hpp"  // for TR_LinkageConventions, etc
#include "codegen/RealRegister.hpp"
#include "codegen/SystemLinkage.hpp"           // for SystemLinkage

class TR_zOSGlobalCompilationInfo;
namespace TR { class CodeGenerator; }

////////////////////////////////////////////////////////////////////////////////
//  TR::Z::ZOSBaseSystemLinkage Definition
//
//  Intention for common zOS behavior across multiple zOS system linkages
////////////////////////////////////////////////////////////////////////////////

namespace TR
{

namespace Z
{

class ZOSBaseSystemLinkage : public TR::SystemLinkage
   {
protected:
   TR::RealRegister::RegNum _CAAPointerRegister;
   TR::RealRegister::RegNum _parentDSAPointerRegister;

   TR_zOSGlobalCompilationInfo* _globalCompilationInfo;

public:
   enum
      {
      CEECAAPLILWS    = 640,
      CEECAAAESS      = 788,
      CEECAAOGETS     = 796,
      CEECAAAGTS      = 944,

      CEEDSAType1Size = 0x80,
      };

   ZOSBaseSystemLinkage(TR::CodeGenerator * cg, TR_S390LinkageConventions elc=TR_S390LinkageDefault, TR_LinkageConventions lc=TR_System)
      : TR::SystemLinkage(cg, elc,lc)
      {
      }

   virtual TR::RealRegister::RegNum setCAAPointerRegister (TR::RealRegister::RegNum r)         { return _CAAPointerRegister = r; }
   virtual TR::RealRegister::RegNum getCAAPointerRegister()         { return _CAAPointerRegister; }
   virtual TR::RealRegister *getCAAPointerRealRegister() {return getS390RealRegister(_CAAPointerRegister);}

   virtual TR::RealRegister::RegNum setParentDSAPointerRegister (TR::RealRegister::RegNum r)         { return _parentDSAPointerRegister = r; }
   virtual TR::RealRegister::RegNum getParentDSAPointerRegister()         { return _parentDSAPointerRegister; }
   virtual TR::RealRegister *getParentDSAPointerRealRegister() {return getS390RealRegister(_parentDSAPointerRegister);}

   // === Fastlink specific
   virtual bool isAggregateReturnedInIntRegisters(int32_t aggregateLenth)   { return isFastLinkLinkageType(); }
   virtual bool isAggregateReturnedInIntRegistersAndMemory(int32_t aggregateLenth);

   };

}
}
#endif
