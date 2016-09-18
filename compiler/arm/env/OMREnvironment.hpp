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

#ifndef OMR_ARM_ENVIRONMENT_INCL
#define OMR_ARM_ENVIRONMENT_INCL

/*
 * The following #define and typedef must appear before any #includes in this file
 */
#ifndef OMR_ENVIRONMENT_CONNECTOR
#define OMR_ENVIRONMENT_CONNECTOR
namespace OMR { namespace ARM { class Environment; } }
namespace OMR { typedef OMR::ARM::Environment EnvironmentConnector; }
#else
#error OMR::ARM::Environment expected to be a primary connector, but an OMR connector is already defined
#endif

#include "compiler/env/OMREnvironment.hpp"


namespace OMR
{

namespace ARM
{

class Environment : public OMR::Environment
   {
public:

   Environment() :
      OMR::Environment(),
         _isEABI(false)
      {}

   Environment(TR::MajorOperatingSystem o, TR::Bitness b) :
      OMR::Environment(o, b),
         _isEABI(false)
      {}

   bool isEABI() { return _isEABI; }
   void setEABI(bool b) { _isEABI = b; }

private:

   bool _isEABI;
   };

}

}


#endif
