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

#ifndef OMR_ILGENREQUEST_INCL
#define OMR_ILGENREQUEST_INCL

/*
 * The following #define and typedef must appear before any #includes in this file
 */
#ifndef OMR_ILGENREQUEST_CONNECTOR
#define OMR_ILGENREQUEST_CONNECTOR
namespace OMR { class IlGenRequest; }
namespace OMR { typedef OMR::IlGenRequest IlGenRequestConnector; }
#endif

#include "env/FilePointerDecl.hpp"  // for FILE
#include "infra/Annotations.hpp"  // for OMR_EXTENSIBLE

class TR_FrontEnd;
class TR_IlGenerator;
namespace TR { class Compilation; }
namespace TR { class IlGeneratorMethodDetails; }
namespace TR { class ResolvedMethodSymbol; }
namespace TR { class SymbolReferenceTable; }

namespace OMR
{

/**
 * IlGenRequestBase defines the IlGenRequest API that common code can count on, although it's more documentation
 *   then enforcement.  A front end will typically extend this class (perhaps even with a hierarchy of classes)
 *   and then map its own concrete base class onto TR::IlGenRequest.
 *
 * Accessing ANY front-end specific API/data in common code is prohibited since other front end builds would fail.
 *
 */

class OMR_EXTENSIBLE IlGenRequest
   {

public:

   /// Create an ILGen object to execute this request
   ///
   virtual TR_IlGenerator *getIlGenerator(
         TR::ResolvedMethodSymbol *methodSymbol,
         TR_FrontEnd *fe,
         TR::Compilation *comp,
         TR::SymbolReferenceTable *symRefTab) = 0;

   virtual void print(TR_FrontEnd *fe, TR::FILE *file, const char *suffix) = 0;

   TR::IlGeneratorMethodDetails & details() { return _methodDetails; }

protected:

   IlGenRequest(TR::IlGeneratorMethodDetails & methodDetails) :
      _methodDetails(methodDetails) { }

   TR::IlGeneratorMethodDetails & _methodDetails;
   };

}

#endif
