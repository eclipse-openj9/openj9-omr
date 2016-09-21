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

#ifndef COMPILATIONEXCEPTION_HPP
#define COMPILATIONEXCEPTION_HPP

#pragma once

#include <exception>

namespace TR {

struct CompilationException : public virtual std::exception
   {
   virtual const char* what() const throw() { return "Compilation Exception"; }
   };

// For COMPILATION_IL_GEN_FAILURE
struct ILGenFailure : public virtual CompilationException
   {
   virtual const char* what() const throw() { return "IL Gen Failure"; }
   };

// For:
//    COMPILATION_LOOPS_OR_BASICBLOCKS_EXCEEDED
//    COMPILATION_REMOVE_EDGE_NESTING_DEPTH_EXCEEDED
//    COMPILATION_MAX_DEPTH_EXCEEDED
//    COMPILATION_MAX_NODE_COUNT_EXCEEDED
//    COMPILATION_INTERNAL_POINTER_EXCEED_LIMIT
struct ExcessiveComplexity : public virtual CompilationException
   {
   virtual const char* what() const throw() { return "Excessive Complexity"; }
   };

// For COMPILATION_MAX_CALLER_INDEX_EXCEEDED
struct MaxCallerIndexExceeded : public virtual CompilationException
   {
   virtual const char* what() const throw() { return "Max Caller Index Exceeded"; }
   };

// For COMPILATION_INTERRUPTED
struct CompilationInterrupted : public virtual CompilationException
   {
   virtual const char* what() const throw() { return "Compilation Interrupted"; }
   };

// For COMPILATION_UNIMPL_OPCODE
struct UnimplementedOpCode : public virtual CompilationException
   {
   virtual const char* what() const throw() { return "Unimplemented Op Code"; }
   };

// For COMPILATION_NEEDED_AT_HIGHER_LEVEL
struct InsufficientlyAggressiveCompilation : public virtual CompilationException
   {
   virtual const char* what() const throw() { return "Insufficiently Aggressive Compilation"; }
   };

// For COMPILATION_GCRPATCH_FAILURE
struct GCRPatchFailure : public virtual CompilationException
   {
   virtual const char* what() const throw() { return "GCR Patch Failure"; }
   };

}

#endif // COMPILATIONEXCEPTION_HPP

