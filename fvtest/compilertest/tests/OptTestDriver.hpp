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
 *******************************************************************************/

#ifndef OPTTESTDRIVER_HPP
#define OPTTESTDRIVER_HPP

#include <stdint.h>
#include <vector>
#include "gtest/gtest.h"
#include "optimizer/Optimizations.hpp"
#include "optimizer/OptimizationStrategies.hpp"
#include "TestDriver.hpp"

namespace TestCompiler { class MethodInfo; }
namespace TR { class IlVerifier; }

namespace TestCompiler
{

class OptTestDriver : public TestDriver, public ::testing::Test
   {
   public:
   ~OptTestDriver();

   void setMethodInfo(TestCompiler::MethodInfo *methodInfo) { _methodInfo = methodInfo; }
   void setIlVerifier(TR::IlVerifier *ilVer) { _ilVer = ilVer; }

   typedef std::vector<OptimizationStrategy> OptVector;

   void addOptimization(OMR::Optimizations opt);
   void addOptimizations(const OptimizationStrategy *opts);

   void Verify();
   void VerifyAndInvoke();

   virtual void compileTestMethods();

   /**
    * Returns the method most recently compiled, cast to type `T`.
    */
   template<typename T> T getCompiledMethod() const { return reinterpret_cast<T>(_compiledMethod); }

   private:
   void makeOptimizationStrategyArray(OptimizationStrategy *strategy);
   void SetupStrategy();

   uint8_t *_compiledMethod;
   TestCompiler::MethodInfo *_methodInfo;
   TR::IlVerifier  *_ilVer;
   std::vector<OptimizationStrategy> _optimizations;
   };

}// namespace TestCompiler

#endif
