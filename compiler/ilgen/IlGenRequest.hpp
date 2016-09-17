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

#ifndef TR_ILGENREQUEST_INCL
#define TR_ILGENREQUEST_INCL

#include "env/FilePointerDecl.hpp"    // for FILE
#include "ilgen/OMRIlGenRequest.hpp"  // for IlGenRequestConnector
#include "infra/Annotations.hpp"      // for OMR_EXTENSIBLE

class TR_FrontEnd;
class TR_IlGenerator;
class TR_InlineBlocks;
namespace TR { class Compilation; }
namespace TR { class IlGeneratorMethodDetails; }
namespace TR { class ResolvedMethodSymbol; }
namespace TR { class SymbolReferenceTable; }

namespace TR
{

class OMR_EXTENSIBLE IlGenRequest : public OMR::IlGenRequestConnector
   {

   public:

   IlGenRequest(TR::IlGeneratorMethodDetails & methodDetails) :
      OMR::IlGenRequestConnector(methodDetails) {}

   virtual bool allowIlGenOptimizations() { return true; }

   };
}

// -----------------------------------------------------------------------------
// Specialized IL gen requests.  These could be made extensible classes if
// necessary.
// -----------------------------------------------------------------------------

namespace TR
{

class CompileIlGenRequest : public IlGenRequest
   {

public:
   CompileIlGenRequest(TR::IlGeneratorMethodDetails & methodDetails) :
      IlGenRequest(methodDetails) { }

   virtual TR_IlGenerator *getIlGenerator(
         TR::ResolvedMethodSymbol *methodSymbol,
         TR_FrontEnd *fe,
         TR::Compilation *comp,
         TR::SymbolReferenceTable *symRefTab);

   virtual void print(TR_FrontEnd *fe, TR::FILE *file, const char *suffix);
   };


class InliningIlGenRequest : public IlGenRequest
   {
protected:
   TR::ResolvedMethodSymbol *_callerSymbol;

public:
   InliningIlGenRequest(TR::IlGeneratorMethodDetails & methodDetails, TR::ResolvedMethodSymbol *callerSymbol) :
      IlGenRequest(methodDetails), _callerSymbol(callerSymbol) { }

   virtual bool allowIlGenOptimizations() { return false; }

   virtual TR_IlGenerator *getIlGenerator(
         TR::ResolvedMethodSymbol *methodSymbol,
         TR_FrontEnd *fe,
         TR::Compilation *comp,
         TR::SymbolReferenceTable *symRefTab);

   virtual void print(TR_FrontEnd *fe, TR::FILE *file, const char *suffix);
   };


class PartialInliningIlGenRequest : public InliningIlGenRequest
   {
protected:
   TR_InlineBlocks *_inlineBlocks;

public:
   PartialInliningIlGenRequest(
         TR::IlGeneratorMethodDetails & methodDetails,
         TR::ResolvedMethodSymbol *callerSymbol,
         TR_InlineBlocks *inlineBlocks) :
      InliningIlGenRequest(methodDetails, callerSymbol), _inlineBlocks(inlineBlocks) { }

   virtual TR_IlGenerator *getIlGenerator(
         TR::ResolvedMethodSymbol *methodSymbol,
         TR_FrontEnd *fe,
         TR::Compilation *comp,
         TR::SymbolReferenceTable *symRefTab);

   virtual void print(TR_FrontEnd *fe, TR::FILE *file, const char *suffix);
   };


}

#endif
