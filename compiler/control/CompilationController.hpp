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

#ifndef COMPILATIONCONTROLLER_INCL
#define COMPILATIONCONTROLLER_INCL

#include "control/OptimizationPlan.hpp"
#include "env/TRMemory.hpp"              // for TR_Memory, etc

class TR_MethodEvent;
namespace TR { class Recompilation; }

class TR_DefaultCompilationStrategy : public TR_CompilationStrategy
   {
   public:
   TR_PERSISTENT_ALLOC(TR_Memory::PersistentInfo);
   TR_DefaultCompilationStrategy() {}
   TR_OptimizationPlan *processEvent(TR_MethodEvent *event, bool *newPlanCreated);
   void shutdown() {}
   virtual bool enableSwitchToProfiling() { return true; }
   };

#endif
