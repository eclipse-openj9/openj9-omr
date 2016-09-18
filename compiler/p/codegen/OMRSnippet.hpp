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

#ifndef OMR_Power_SNIPPET_INCL
#define OMR_Power_SNIPPET_INCL

/*
 * The following #define and typedef must appear before any #includes in this file
 */
#ifndef OMR_SNIPPET_CONNECTOR
#define OMR_SNIPPET_CONNECTOR
namespace OMR { namespace Power { class Snippet; } }
namespace OMR { typedef OMR::Power::Snippet SnippetConnector; }
#else
#error OMR::Power::Snippet expected to be a primary connector, but an OMR connector is already defined
#endif

#include "compiler/codegen/OMRSnippet.hpp"

namespace TR { class CodeGenerator; }
namespace TR { class LabelSymbol; }
namespace TR { class Node; }

namespace OMR
{

namespace Power
{

class OMR_EXTENSIBLE Snippet : public OMR::Snippet
   {
   public:

   Snippet(TR::CodeGenerator *cg, TR::Node * node, TR::LabelSymbol * label, bool isGCSafePoint);

   Snippet(TR::CodeGenerator *cg, TR::Node *node, TR::LabelSymbol *label);

   enum Kind
      {
      IsUnknown,
      IsCall,
         IsUnresolvedCall,
      IsVirtual,
         IsVirtualUnresolved,
         IsInterfaceCall,
      IsHelperCall,
         IsMonitorEnter,
         IsMonitorExit,
         IsReadMonitor,
         IsLockReservationEnter,
         IsLockReservationExit,
         IsArrayCopyCall,
      IsHeapAlloc,
      IsAllocPrefetch,
      IsNonZeroAllocPrefetch,
      IsRecompilation,
      IsForceRecompilation,
      IsStackCheckFailure,
      IsUnresolvedData,
      IsInterfaceCastSnippet,
      IsTLB1,
         IsTLB1LoadArgs,
      numKinds
      };

   virtual Kind getKind() { return IsUnknown; }
   };

}

}

#endif
