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

#ifndef OMR_DEBUG_ENV_INCL
#define OMR_DEBUG_ENV_INCL

/*
 * The following #define and typedef must appear before any #includes in this file
 */
#ifndef OMR_DEBUG_ENV_CONNECTOR
#define OMR_DEBUG_ENV_CONNECTOR
namespace OMR { class DebugEnv; }
namespace OMR { typedef OMR::DebugEnv DebugEnvConnector; }
#endif


#include "infra/Annotations.hpp"  // for OMR_EXTENSIBLE
#include "env/jittypes.h"

namespace OMR
{

class OMR_EXTENSIBLE DebugEnv
   {
public:
   DebugEnv() {}

   void breakPoint();

   char *extraAssertMessage(TR::Compilation *comp) { return ""; }

   int32_t hexAddressWidthInChars() { return _hexAddressWidthInChars; }

   // Deprecate in favour of 'pointerPrintfMaxLenInChars' ?
   //
   int32_t hexAddressFieldWidthInChars() { return _hexAddressFieldWidthInChars; }

   int32_t codeByteColumnWidth() { return _codeByteColumnWidth; }

   int32_t pointerPrintfMaxLenInChars() { return _hexAddressFieldWidthInChars; }

protected:

   int32_t _hexAddressWidthInChars;

   int32_t _hexAddressFieldWidthInChars;

   int32_t _codeByteColumnWidth;
   };

}

#endif
