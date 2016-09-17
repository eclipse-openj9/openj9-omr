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

#ifndef OMR_OPTIMIZATIONMANAGER_INLINES_INCL
#define OMR_OPTIMIZATIONMANAGER_INLINES_INCL

#include "optimizer/OMROptimizationManager.hpp"

inline
TR::OptimizationManager *OMR::OptimizationManager::self()
   {
   return (static_cast<TR::OptimizationManager *>(this));
   }

inline
TR::CodeGenerator *OMR::OptimizationManager::cg()
   {
   return self()->comp()->cg();
   }

inline
TR_FrontEnd *OMR::OptimizationManager::fe()
   {
   return self()->comp()->fe();
   }

inline
TR_Debug *OMR::OptimizationManager::getDebug()
   {
   return self()->comp()->getDebug();
   }

inline
TR::SymbolReferenceTable *OMR::OptimizationManager::getSymRefTab()
   {
   return self()->comp()->getSymRefTab();
   }

inline
TR_Memory *OMR::OptimizationManager::trMemory()
   {
   return self()->comp()->trMemory();
   }

inline
TR_StackMemory OMR::OptimizationManager::trStackMemory()
   {
   return self()->comp()->trStackMemory();
   }

inline
TR_HeapMemory OMR::OptimizationManager::trHeapMemory()
   {
   return self()->comp()->trHeapMemory();
   }

inline
TR_PersistentMemory *OMR::OptimizationManager::trPersistentMemory()
   {
   return self()->comp()->trPersistentMemory();
   }

inline
TR::Allocator OMR::OptimizationManager::allocator()
   {
   return self()->comp()->allocator();
   }

#endif
