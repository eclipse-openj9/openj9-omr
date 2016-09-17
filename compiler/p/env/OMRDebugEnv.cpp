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

#include "env/DebugEnv.hpp"

OMR::Power::DebugEnv::DebugEnv() :
      OMR::DebugEnv()
   {

#ifdef TR_TARGET_64BIT
   _hexAddressWidthInChars = 16;
   _hexAddressFieldWidthInChars = 18;
   _codeByteColumnWidth = 19;
#else
   _hexAddressWidthInChars = 8;
   _hexAddressFieldWidthInChars = 10;
   _codeByteColumnWidth = 19;
#endif

   }
