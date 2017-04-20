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

#ifndef OMR_X86_SNIPPET_INCL
#define OMR_X86_SNIPPET_INCL

/*
 * The following #define and typedef must appear before any #includes in this file
 */
#ifndef OMR_SNIPPET_CONNECTOR
#define OMR_SNIPPET_CONNECTOR
namespace OMR { namespace X86 { class Snippet; } }
namespace OMR { typedef OMR::X86::Snippet SnippetConnector; }
#else
#error OMR::X86::Snippet expected to be a primary connector, but an OMR connector is already defined
#endif


#include "compiler/codegen/OMRSnippet.hpp"
#include "env/CompilerEnv.hpp"

namespace TR { class X86GuardedDevirtualSnippet; }
namespace TR { class CodeGenerator; }
namespace TR { class LabelSymbol; }
namespace TR { class Node; }

namespace OMR
{

namespace X86
{

class OMR_EXTENSIBLE Snippet : public OMR::Snippet
   {
   public:

   Snippet(TR::CodeGenerator *cg, TR::Node *node, TR::LabelSymbol *label, bool isGCSafePoint);

   Snippet(TR::CodeGenerator *cg, TR::Node *node, TR::LabelSymbol *label);

   enum Kind
      {
      IsUnknown,
      IsCall,
         IsUnresolvedCall,
      IsIPicData,
      IsVPicData,
      IsUnresolvedVirtualCall,
      IsUnresolvedVTableSlot,
      IsVirtualPIC,
      IsCheckFailure,
         IsCheckFailureWithResolve,
         IsBoundCheckWithSpineCheck,
      IsSpineCheck,
      IsConstantData,
      IsData,
      IsRecompilation,
      IsRestart,
         IsDivideCheck,
         IsForceRecompilation,
         IsGuardedDevirtual,
         IsHelperCall,
            IsWriteBarrier,
            IsWriteBarrierAMD64,
            IsScratchArgHelperCall,
         IsFPConversion,
            IsFPConvertToInt,
            IsFPConvertToLong,
         IsPassJNINull,
         IsJNIPause,
      IsUnresolvedDataIA32,
      IsUnresolvedDataAMD64,
      numKinds
      };

   virtual Kind getKind() { return IsUnknown; }
   };

}

}


inline const char *commentString() { return TR::Compiler->target.isLinux() ? "#" : ";"; }
inline const char *hexPrefixString() { return TR::Compiler->target.isLinux() ? "0x" : "0"; }
inline const char *hexSuffixString() { return TR::Compiler->target.isLinux() ? "" : "h"; }
inline const char *dbString() { return TR::Compiler->target.isLinux() ? ".byte" : "db"; }
inline const char *dwString() { return TR::Compiler->target.isLinux() ? ".short" : "dw"; }
inline const char *ddString() { return TR::Compiler->target.isLinux() ? ".int" : "dd"; }
inline const char *dqString() { return TR::Compiler->target.isLinux() ? ".quad" : "dq"; }

#endif
