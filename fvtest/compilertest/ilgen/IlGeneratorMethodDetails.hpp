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

#ifndef TR_ILGENERATOR_METHOD_DETAILS_INCL
#define TR_ILGENERATOR_METHOD_DETAILS_INCL

#include "infra/Annotations.hpp"
#include "ilgen/TestIlGeneratorMethodDetails.hpp"

class TR_ResolvedMethod;
namespace TR { class ResolvedMethod; }

namespace TR
{

class OMR_EXTENSIBLE IlGeneratorMethodDetails : public TestCompiler::IlGeneratorMethodDetailsConnector
   {

public:

   IlGeneratorMethodDetails() :
      TestCompiler::IlGeneratorMethodDetailsConnector() {}

   IlGeneratorMethodDetails(TR::ResolvedMethod *method) :
      TestCompiler::IlGeneratorMethodDetailsConnector(method) {}

   IlGeneratorMethodDetails(TR_ResolvedMethod *method) :
      TestCompiler::IlGeneratorMethodDetailsConnector(method) {}

   };
}

#endif
