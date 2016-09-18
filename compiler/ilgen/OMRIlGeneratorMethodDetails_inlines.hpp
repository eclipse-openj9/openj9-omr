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

#ifndef OMR_ILGENERATOR_METHOD_DETAILS_INLINES_INCL
#define OMR_ILGENERATOR_METHOD_DETAILS_INLINES_INCL

#include "ilgen/IlGeneratorMethodDetails.hpp"
#include "ilgen/OMRIlGeneratorMethodDetails.hpp"

class TR_ResolvedMethod;

TR::IlGeneratorMethodDetails *
OMR::IlGeneratorMethodDetails::self()
   {
   return static_cast<TR::IlGeneratorMethodDetails *>( this );
   }

const TR::IlGeneratorMethodDetails *
OMR::IlGeneratorMethodDetails::self() const
   {
   return static_cast<const TR::IlGeneratorMethodDetails *>( this );
   }

TR::IlGeneratorMethodDetails &
OMR::IlGeneratorMethodDetails::create(TR::IlGeneratorMethodDetails & target, TR_ResolvedMethod *method)
   {
   return * new (&target) TR::IlGeneratorMethodDetails(method);
   }

#endif
