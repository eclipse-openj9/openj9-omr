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

#ifndef OMR_RECOMPILATION_INCL
#define OMR_RECOMPILATION_INCL

/*
 * The following #define and typedef must appear before any #includes in this file
 */
#ifndef OMR_RECOMPILATION_CONNECTOR
#define OMR_RECOMPILATION_CONNECTOR
namespace OMR { class Recompilation; }
namespace OMR { typedef OMR::Recompilation RecompilationConnector; }
#endif

#include <stddef.h>                       // for NULL
#include <stdint.h>                       // for int32_t, uint32_t, etc
#include "compile/Compilation.hpp"        // for Compilation
#include "env/TRMemory.hpp"               // for TR_Memory, etc

namespace TR { class Instruction; }
namespace TR { class Recompilation; }

namespace OMR
{

class Recompilation
   {
public:

   TR_ALLOC(TR_Memory::Recompilation)

   TR::Recompilation *self();

   virtual TR::Instruction *generatePrePrologue() { return 0; }
   virtual TR::Instruction *generatePrologue(TR::Instruction *) { return 0; }

   bool isProfilingCompilation() { return false; }
   bool couldBeCompiledAgain() { return false; }
   bool shouldBeCompiledAgain() { return false; }

   void startOfCompilation() { return; }
   void beforeOptimization() { return; }
   void beforeCodeGen() { return; }
   void endOfCompilation() { return; }
   virtual void postCompilation() { return; }

   static void shutdown();

protected:

   Recompilation(TR::Compilation *);

   TR::Compilation *comp() { return _compilation; }
   TR_Memory *trMemory() { return comp()->trMemory(); }
   TR_HeapMemory trHeapMemory() { return trMemory(); }

   TR::Compilation *_compilation;

   };

}

#endif
