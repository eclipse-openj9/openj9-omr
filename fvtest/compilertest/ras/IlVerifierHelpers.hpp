/*******************************************************************************
 *
 * (c) Copyright IBM Corp. 2016, 2016
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

/**
 * @file
 *
 * This file contains base classes that verifiers can be based upon.
 * Some are common idioms, while other combine multiple verifiers.
 */

#ifndef IlVerifierHelpers_hpp
#define IlVerifierHelpers_hpp

#include <vector>
#include "ras/IlVerifier.hpp"

namespace TR { class ResolvedMethodSymbol; }

namespace TR {

/**
 * A logical && operation on IL verifiers.
 * This is similar to: true && ver1() == 0 && ver2() == 0 && ...
 *
 * If all verifiers added to this verifier return 0, this
 * verifier also returns 0. On the first non-zero error code,
 * verification stops and the same error code is returned.
 */
class AllIlVerifier : public TR::IlVerifier
   {
   public:
   int32_t verify(TR::ResolvedMethodSymbol *methodSymbol)
      {
      for(auto it = verifiers.begin(); it != verifiers.end(); ++it)
         {
         int32_t ret = (*it)->verify(methodSymbol);
         if(ret)
            return ret;
         }
      return 0;
      }

   void add(TR::IlVerifier *v)
      {
      verifiers.push_back(v);
      }

   private:
   std::vector<TR::IlVerifier*> verifiers;
   };

/**
 * Throws an exception after IL verification. This allows
 * a test to skip codegen, but still verify the optimized IL.
 * Since this exception will cause a different return code to
 * be returned, #getReturnCode can be used to get the actual
 * return code.
 */
class NoCodegenVerifier : public TR::IlVerifier
   {
   public:

   NoCodegenVerifier(TR::IlVerifier *ilVerifier)
      :
         _ilVerifier(ilVerifier),
         _hasRun(false),
         _rc(0)
      {
      }

   /**
    * Run the child verifier, then throw an exception to avoid
    * codegen.
    *
    * @return The return code of the child verifier,
    * or 1 if it did not return.
    */
   int32_t verify(TR::ResolvedMethodSymbol *methodSymbol)
      {
      _hasRun = true;

      if(_ilVerifier)
         {
         // If verify() does not return, have a non-zero return code.
         _rc = 1;
         _rc = _ilVerifier->verify(methodSymbol);
         }
      else
         {
         _rc = 0;
         }

      throw TR::CompilationException();
      }

   /**
    * Determines if verifiers were run.
    * @return `true` if the verifier was run, otherwise `false`.
    */
   bool hasRun() const { return _hasRun; }

   /**
    * Get the true return code of a compilation. Since #verify throws an
    * exception, the compilation does not return the true return code.
    *
    * @param result The compilation return code.
    * @return The true return code.
    */
   int32_t getReturnCode() const { return _rc; }

   private:
   TR::IlVerifier *_ilVerifier;
   bool _hasRun;
   int32_t _rc;
   };

}

#endif
