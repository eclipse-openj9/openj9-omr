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

#ifndef OMR_COMPILER_ENV_INCL
#define OMR_COMPILER_ENV_INCL

/*
 * The following #define and typedef must appear before any #includes in this file
 */
#ifndef OMR_COMPILER_ENV_CONNECTOR
#define OMR_COMPILER_ENV_CONNECTOR
namespace OMR { class CompilerEnv; }
namespace OMR { typedef OMR::CompilerEnv CompilerEnvConnector; }
#endif


#include "infra/Annotations.hpp"  // for OMR_EXTENSIBLE
#include "env/RawAllocator.hpp"
#include "env/Environment.hpp"
#include "env/DebugEnv.hpp"
#include "env/PersistentAllocator.hpp"
#include "env/ClassEnv.hpp"
#include "env/ObjectModel.hpp"
#include "env/ArithEnv.hpp"
#include "env/VMEnv.hpp"
#include "env/VMMethodEnv.hpp"

namespace TR { class CompilerEnv; }


namespace OMR
{

class OMR_EXTENSIBLE CompilerEnv
   {

public:

   CompilerEnv(TR::RawAllocator raw, const TR::PersistentAllocatorKit &persistentAllocatorKit);

   TR::CompilerEnv *self();

   /// Primordial raw allocator.  This is guaranteed to be thread safe.
   ///
   TR::RawAllocator rawAllocator;

   // Compilation host environment
   //
   TR::Environment host;

   // Compilation target environment
   //
   TR::Environment target;

   // Class information in this compilation environment.
   //
   TR::ClassEnv cls;

   // Information about the VM environment.
   //
   TR::VMEnv vm;

   // Information about methods in this compilation environment
   //
   TR::VMMethodEnv mth;

   // Object model in this compilation environment
   //
   TR::ObjectModel om;

   // Arithmetic semantics in this compilation environment
   //
   TR::ArithEnv arith;

   // Debug environment for the compiler.  This is not thread safe.
   //
   TR::DebugEnv debug;

   bool isInitialized() { return _initialized; }

   // --------------------------------------------------------------------------

   // Initialize a CompilerEnv.  This should only be executed after a CompilerEnv
   // object has been allocated and its constructor run.  Its intent is to
   // execute initialization logic that may require a completely initialized
   // object beforehand.
   //
   void initialize();

   TR::PersistentAllocator &persistentAllocator() { return _persistentAllocator; }

protected:
   // Initialize 'target' environment for this compiler
   //
   void initializeTargetEnvironment();

   // Initialize 'host' environment for this compiler
   //
   void initializeHostEnvironment();

private:

   bool _initialized;

   TR::PersistentAllocator _persistentAllocator;

public:

   /// Compiler-lifetime region allocator.
   /// NOTE: its a raw allocator until the region allocation work is completed.
   ///
   TR::PersistentAllocator &regionAllocator;

   };

}

#endif
