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

#ifndef TR_SYMBOLREFERENCE_INCL
#define TR_SYMBOLREFERENCE_INCL

#include "il/OMRSymbolReference.hpp"

#include <stdint.h>                          // for int32_t
#include "compile/SymbolReferenceTable.hpp"  // for SymbolReferenceTable, etc
#include "env/KnownObjectTable.hpp"          // for KnownObjectTable, etc
#include "env/jittypes.h"                    // for intptrj_t
#include "infra/Annotations.hpp"             // for OMR_EXTENSIBLE

class mcount_t;
namespace TR { class Symbol; }

namespace TR
{

class OMR_EXTENSIBLE SymbolReference : public OMR::SymbolReferenceConnector
   {

public:

   SymbolReference(TR::SymbolReferenceTable * symRefTab) :
      OMR::SymbolReferenceConnector(symRefTab) {}

   SymbolReference(TR::SymbolReferenceTable * symRefTab,
                   TR::Symbol * symbol,
                   intptrj_t offset = 0) :
      OMR::SymbolReferenceConnector(symRefTab,
                                    symbol,
                                    offset) {}

   SymbolReference(TR::SymbolReferenceTable * symRefTab,
                   int32_t refNumber,
                   TR::Symbol *ps,
                   intptrj_t offset = 0) :
      OMR::SymbolReferenceConnector(symRefTab,
                                    refNumber,
                                    ps,
                                    offset) {}

   SymbolReference(TR::SymbolReferenceTable *symRefTab,
                   TR::SymbolReferenceTable::CommonNonhelperSymbol number,
                   TR::Symbol *ps,
                   intptrj_t offset = 0) :
      OMR::SymbolReferenceConnector(symRefTab,
                                    number,
                                    ps,
                                    offset) {}

   SymbolReference(TR::SymbolReferenceTable *symRefTab,
                   TR::Symbol *sym,
                   mcount_t owningMethodIndex,
                   int32_t cpIndex,
                   int32_t unresolvedIndex = 0) :
      OMR::SymbolReferenceConnector(symRefTab,
                                    sym,
                                    owningMethodIndex,
                                    cpIndex,
                                    unresolvedIndex) {}

   SymbolReference(TR::SymbolReferenceTable *symRefTab,
                   TR::SymbolReference& sr,
                   intptrj_t offset,
                   TR::KnownObjectTable::Index knownObjectIndex = TR::KnownObjectTable::UNKNOWN) :
      OMR::SymbolReferenceConnector(symRefTab,
                                    sr,
                                    offset,
                                    knownObjectIndex) {}

protected:

   SymbolReference(TR::SymbolReferenceTable * symRefTab,
                   TR::Symbol *               symbol,
                   intptrj_t                  offset,
                   const char *               name) :
      OMR::SymbolReferenceConnector(symRefTab,
                                    symbol,
                                    offset,
                                    name) {}

   };

}

#endif
