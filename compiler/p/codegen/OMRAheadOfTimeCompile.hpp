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

#ifndef OMR_POWER_AHEADOFTIMECOMPILE_INCL
#define OMR_POWER_AHEADOFTIMECOMPILE_INCL

#ifndef OMR_AHEADOFTIMECOMPILE_CONNECTOR
#define OMR_AHEADOFTIMECOMPILE_CONNECTOR
namespace OMR { namespace Power { class AheadOfTimeCompile; } }
namespace OMR { typedef OMR::Power::AheadOfTimeCompile AheadOfTimeCompileConnector; }
#endif // OMR_AHEADOFTIMECOMPILE_CONNECTOR

#include "compiler/codegen/OMRAheadOfTimeCompile.hpp"
#include "codegen/PPCAOTRelocation.hpp"

namespace OMR
{

namespace Power
{

class OMR_EXTENSIBLE AheadOfTimeCompile : public OMR::AheadOfTimeCompile
   {
   public:
   AheadOfTimeCompile(uint32_t *headerSizeMap, TR::Compilation * c)
       : OMR::AheadOfTimeCompile(headerSizeMap, c),
        _relocationList(getTypedAllocator<TR::PPCRelocation*>(c->allocator()))
     {
     }

   TR::list<TR::PPCRelocation*>& getRelocationList() {return _relocationList;}

   private:
   TR::list<TR::PPCRelocation*> _relocationList;
   };

} // namespace Power

} // namespace OMR

#endif
