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

#ifndef TR_OPTIONS_INCL
#define TR_OPTIONS_INCL

#include "control/OMROptions.hpp"

namespace TR
{

class OMR_EXTENSIBLE Options : public OMR::OptionsConnector
{
public:

   Options() : OMR::OptionsConnector() {}

   Options(TR_Memory * m,
           int32_t index,
           int32_t lineNumber,
           TR_ResolvedMethod *compilee,
           void *oldStartPC,
           TR_OptimizationPlan *optimizationPlan,
           bool isAOT=false,
           int32_t compThreadID=-1)
      : OMR::OptionsConnector(m,index,lineNumber,compilee,oldStartPC,optimizationPlan,isAOT,compThreadID)
      {}

   Options(TR::Options &other) : OMR::OptionsConnector(other) {}
};

}

#endif

