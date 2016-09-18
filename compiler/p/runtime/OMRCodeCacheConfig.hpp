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

#ifndef OMR_POWER_CODECACHECONFIG_INCL
#define OMR_POWER_CODECACHECONFIG_INCL

/*
 * The following #defines and typedefs must appear before any #includes in this file
 */
#ifndef OMR_CODECACHECONFIG_COMPOSED
#define OMR_CODECACHECONFIG_COMPOSED
namespace OMR { namespace Power { class CodeCacheConfig; } }
namespace OMR { typedef Power::CodeCacheConfig CodeCacheConfigConnector; }
#endif

#include "compiler/runtime/OMRCodeCacheConfig.hpp"

namespace OMR
{
namespace Power
{

class OMR_EXTENSIBLE CodeCacheConfig : public OMR::CodeCacheConfig
   {
   public:

   CodeCacheConfig();
   };


} // namespace POWER
} // namespace OMR

#endif // defined(OMR_POWER_CODECACHECONFIG_IMPL)
