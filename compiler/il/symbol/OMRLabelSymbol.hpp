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

#ifndef OMR_LABELSYMBOL_INCL
#define OMR_LABELSYMBOL_INCL

/*
 * The following #define and typedef must appear before any #includes in this file
 */
#ifndef OMR_LABELSYMBOL_CONNECTOR
#define OMR_LABELSYMBOL_CONNECTOR
namespace OMR { class LabelSymbol; }
namespace OMR { typedef OMR::LabelSymbol LabelSymbolConnector; }
#endif

#include "il/Symbol.hpp"

#include <stdint.h>          // for int32_t, intptr_t, etc
#include <stdlib.h>          // for malloc, NULL
#include <string.h>          // for strcpy, strlen
#include "infra/Assert.hpp"  // for TR_ASSERT

class TR_Debug;
namespace TR { class Block; }
namespace TR { class CodeGenerator; }
namespace TR { class Compilation; }
namespace TR { class Instruction; }
namespace TR { class LabelSymbol; }
namespace TR { class ParameterSymbol; }
namespace TR { class Snippet; }
namespace TR { class StaticSymbol; }
namespace TR { class SymbolReference; }
template <class T> class List;

typedef uintptr_t TR_UniqueCompilationId;
const TR_UniqueCompilationId nullCompilationId(~(TR_UniqueCompilationId) 0);

namespace OMR
{

/**
 * A symbol representing a label.
 *
 * A label has an instruction, a code location...
 */
class LabelSymbol : public TR::Symbol
   {
public:
   TR::LabelSymbol * self();

   template <typename AllocatorType>
   static TR::LabelSymbol * create(AllocatorType);

   template <typename AllocatorType>
   static TR::LabelSymbol * create(AllocatorType, TR::CodeGenerator*);

   template <typename AllocatorType>
   static TR::LabelSymbol * create(AllocatorType, TR::CodeGenerator*, TR::Block*);

protected:

   LabelSymbol();
   LabelSymbol(TR::CodeGenerator *codeGen);
   LabelSymbol(TR::CodeGenerator *codeGen, TR::Block *labb);

public:

   // Using declaration is required here or else the
   // debug getName will hide the parent class getName
   using TR::Symbol::getName;
   const char * getName(TR_Debug * debug);

   TR::Instruction * getInstruction()                   { return _instruction;       }
   TR::Instruction * setInstruction(TR::Instruction *p) { return (_instruction = p); }

   uint8_t * getCodeLocation()           { return _codeLocation; }
   void      setCodeLocation(uint8_t *p) { _codeLocation = p; }

   int32_t getEstimatedCodeLocation()          { return _estimatedCodeLocation; }
   int32_t setEstimatedCodeLocation(int32_t p) { return (_estimatedCodeLocation = p); }

   TR::Snippet * getSnippet()               { return _snippet; }
   TR::Snippet * setSnippet(TR::Snippet *s) { return (_snippet = s); }

   TR::LabelSymbol * getVMThreadRestoringLabel()                    { return _vmThreadRestoringLabel; }
   TR::LabelSymbol * setVMThreadRestoringLabel(TR::LabelSymbol *ls) { return (_vmThreadRestoringLabel = ls); }

   void setDirectlyTargeted() { _directlyTargeted = true; }
   TR_YesNoMaybe isTargeted();

private:

   TR::Instruction *  _instruction;

   uint8_t *          _codeLocation;

   int32_t            _estimatedCodeLocation;

   TR::Snippet *      _snippet;

   TR::LabelSymbol *  _vmThreadRestoringLabel; ///< For late edge splitting

   bool               _directlyTargeted;

public:
   /*------------- TR_RelativeLabelSymbol -----------------*/
   template <typename AllocatorType>
   static TR::LabelSymbol *createRelativeLabel(AllocatorType m, TR::CodeGenerator * cg, intptr_t offset);

   /**
    * Mark a LabelSymbol as a RelativeLabelSymbol, inializing members as
    * appropriate.
    *
    * A relative label provides an offset/distance (and changes the name of a
    * symbol to the offset).
    *
    * @todo This leaks memory right now.
    */
   void makeRelativeLabelSymbol(intptr_t offset);

   intptr_t getDistance() { TR_ASSERT(isRelativeLabel(), "Must be a relative label to have a valid offset!");  return _offset; }
private:
   intptr_t     _offset;
  };

}
/**
 * Static creation function.
 */
TR::LabelSymbol *generateLabelSymbol(TR::CodeGenerator *cg);

#endif
