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

#ifndef OMR_ILHELPERS_INCL
#define OMR_ILHELPERS_INCL

#include <stdint.h>          // for uint32_t, uint8_t
#include "il/DataTypes.hpp"  // for DataTypes
#include "il/ILOpCodes.hpp"  // for ILOpCodes, ILOpCodes::NumIlOps

enum TR_ComparisonTypes
   {
   TR_cmpEQ,
   TR_cmpNE,
   TR_cmpLT,
   TR_cmpLE,
   TR_cmpGT,
   TR_cmpGE
   };

#endif // !defined(OMR_ILHELPERS_INCL)
