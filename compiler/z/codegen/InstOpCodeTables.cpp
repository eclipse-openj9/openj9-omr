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

#include "compiler/z/codegen/OMRInstOpCode.hpp"

const uint64_t OMR::Z::InstOpCode::properties[NumOpCodes] =
   {
   #include "codegen/ArchInstOpCodeProperties.hpp"
   };

const uint64_t OMR::Z::InstOpCode::properties2[NumOpCodes] =
   {
   #include "codegen/ArchInstOpCodeProperties2.hpp"
   };

const OMR::Z::InstOpCode::OpCodeBinaryEntry OMR::Z::InstOpCode::binaryEncodings[NumOpCodes] =
   {
   #include "codegen/ArchInstOpCodeEncoding.hpp"
   };


const char * OMR::Z::InstOpCode::opCodeToNameMap[NumOpCodes] =
   {
   #include "codegen/ArchInstOpCodeNameMap.hpp"
   };

