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

#ifndef JITBUILDER_OBJECTMODEL_INCL
#define JITBUILDER_OBJECTMODEL_INCL

/*
 * The following #define and typedef must appear before any #includes in this file
 */
#ifndef JITBUILDER_OBJECTMODEL_CONNECTOR
#define JITBUILDER_OBJECTMODEL_CONNECTOR
namespace JitBuilder { class ObjectModel; }
namespace JitBuilder { typedef JitBuilder::ObjectModel ObjectModelConnector; }
#endif


#include "env/OMRObjectModel.hpp"

namespace JitBuilder
{

class ObjectModel : public OMR::ObjectModelConnector
   {
   public:

   ObjectModel() :
      OMR::ObjectModelConnector() {}

   virtual int32_t sizeofReferenceField() { return sizeof(char *); }
   };

}

#endif // defined(JITBUILDER_OBJECTMODEL_INCL)
