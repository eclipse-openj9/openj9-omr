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

#ifndef OMR_ILGENERATOR_METHOD_DETAILS_INCL
#define OMR_ILGENERATOR_METHOD_DETAILS_INCL

/*
 * The following #define and typedef must appear before any #includes in this file
 */
#ifndef OMR_ILGENERATORMETHODDETAILS_CONNECTOR
#define OMR_ILGENERATORMETHODDETAILS_CONNECTOR
namespace OMR { class IlGeneratorMethodDetails; }
namespace OMR { typedef OMR::IlGeneratorMethodDetails IlGeneratorMethodDetailsConnector; }
#endif

#include <stddef.h>               // for size_t
#include "env/FilePointerDecl.hpp"  // for FILE
#include "infra/Annotations.hpp"  // for OMR_EXTENSIBLE

class TR_FrontEnd;
class TR_ResolvedMethod;
namespace TR { class IlGeneratorMethodDetails; }

namespace OMR
{

/**
 * IlGeneratorMethodDetailsBase defines the IlGeneratorMethodDetails API that common code can count on.  A front
 *   end will typically extend this class (perhaps even with a hierarchy of classes) and then map its own concrete
 *   base class onto OMR::IlGeneratorMethodDetails.  In this way, common code can count on a particular API
 *   existing, but IlGeneratorMethodDetails will be passed around with all front-end specific data and API intact
 *   (so less up/down casting).
 *
 * Accessing ANY front-end specific API/data in common code is prohibited.
 *
 * IlGeneratorMethodDetailsBase defines the IlGenRequest API that common code can count on, although it's more documentation
 *   then enforcement.  A front end will typically extend this class (perhaps even with a hierarchy of classes)
 *   and then map its own concrete base class onto OMR::IlGeneratorMethodDetailsBase.
 *
 * Accessing ANY front-end specific API/data in common code is prohibited since other front end builds would fail.
 *
 * Note all functions are non-virtual in this class, so declarations should never use the IlGeneratorMethodDetailsBase type
 *   directly: all variable declarations should use the OMR::IlGeneratorMethodDetails type instead.
 */

class OMR_EXTENSIBLE IlGeneratorMethodDetails
   {

public:

   inline TR::IlGeneratorMethodDetails *self();
   inline const TR::IlGeneratorMethodDetails *self() const;

   virtual bool isMethodInProgress() const { return false; }
   bool supportsInvalidation() { return false; }
   bool sameAs(TR::IlGeneratorMethodDetails & other) { return false; }
   void print(TR_FrontEnd *fe, TR::FILE *file) { }

   inline static TR::IlGeneratorMethodDetails & create(TR::IlGeneratorMethodDetails & target, TR_ResolvedMethod *method);

protected:
   IlGeneratorMethodDetails() { }
   virtual ~IlGeneratorMethodDetails() {}

   void *operator new(size_t size, TR::IlGeneratorMethodDetails *p){ return (void*) p; }
   void *operator new(size_t size, TR::IlGeneratorMethodDetails &p){ return (void*)&p; }

   };

}

#endif
