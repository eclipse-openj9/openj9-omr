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

#ifndef TR_COMPILATION_INCL
#define TR_COMPILATION_INCL

#include "compile/OMRCompilation.hpp"

#include <stdint.h>  // for int32_t

class TR_FrontEnd;
class TR_Memory;
class TR_OptimizationPlan;
class TR_ResolvedMethod;
namespace TR { class IlGenRequest; }
namespace TR { class Options; }
struct OMR_VMThread;

namespace TR
{
class Compilation : public OMR::CompilationConnector
   {
   public:

   Compilation(
         int32_t compThreadId,
         OMR_VMThread *omrVMThread,
         TR_FrontEnd *fe,
         TR_ResolvedMethod *method,
         TR::IlGenRequest &request,
         TR::Options &options,
         const TR::Region &dispatchRegion,
         TR_Memory *memory,
         TR_OptimizationPlan *optimizationPlan) :
      OMR::CompilationConnector(
         compThreadId,
         omrVMThread,
         fe,
         method,
         request,
         options,
         dispatchRegion,
         memory,
         optimizationPlan)
      {}

   ~Compilation() {}
   };
}

#endif
