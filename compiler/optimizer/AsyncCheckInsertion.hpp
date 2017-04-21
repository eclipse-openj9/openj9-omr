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

#ifndef ASYNC_CHECK_INSERTION_H
#define ASYNC_CHECK_INSERTION_H

#include <stdint.h>                           // for int32_t, int64_t, etc
#include "env/TRMemory.hpp"                   // for TR_Memory, etc
#include "optimizer/Optimization.hpp"         // for Optimization
#include "optimizer/OptimizationManager.hpp"  // for OptimizationManager



namespace TR { class Block; }
namespace TR { class Compilation; }


class TR_AsyncCheckInsertion : public TR::Optimization
   {
   public:
   TR_AsyncCheckInsertion(TR::OptimizationManager *manager);
   static TR::Optimization *create(TR::OptimizationManager *manager)
      {
      return new (manager->allocator()) TR_AsyncCheckInsertion(manager);
      }


   static int32_t insertReturnAsyncChecks(TR::Optimization *opt, const char *counterPrefix);
   static void insertAsyncCheck(TR::Block *block, TR::Compilation *comp, const char *counterPrefix);

   virtual bool    shouldPerform();
   virtual int32_t perform();

   private:

   };

#endif

