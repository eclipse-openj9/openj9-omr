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

#include "OptTestDriver.hpp"
#include "compile/Method.hpp"
#include "control/Options.hpp"
#include "optimizer/Optimizer.hpp"
#include "ilgen/IlGeneratorMethodDetails_inlines.hpp"
#include "ilgen/IlInjector.hpp"
#include "ras/IlVerifierHelpers.hpp"
#include "ilgen/MethodInfo.hpp"

TestCompiler::OptTestDriver::~OptTestDriver()
   {
   TR::Optimizer::setMockStrategy(NULL);
   }

/**
 * Fill the array \p strategy with optimizations.
 *
 * @param[out] strategy An array with at least
 * `_optimization.size() + 1` elements.
 */
void TestCompiler::OptTestDriver::makeOptimizationStrategyArray(OptimizationStrategy *strategy)
   {
   for(unsigned int i = 0; i < _optimizations.size(); ++i)
      {
      strategy[i]._num = _optimizations[i]._num;
      strategy[i]._options = _optimizations[i]._options;
      }

   strategy[_optimizations.size()]._num = OMR::endOpts;
   strategy[_optimizations.size()]._options = 0;
   }

/**
 * Similar to VerifyAndInvoke(), however the IL is not compiled
 * after IL verification, and tests are not invoked.
 */
void TestCompiler::OptTestDriver::Verify()
   {
   OptimizationStrategy strategy[_optimizations.size() + 1];
   makeOptimizationStrategyArray(strategy);
   TR::Optimizer::setMockStrategy(strategy);

   // To stop before codegen, throw an exception at the end of IL verification.
   TR::IlVerifier *oldVerifier = _ilVer;
   TR::NoCodegenVerifier noCodegenVerifier(oldVerifier);
   setIlVerifier(&noCodegenVerifier);

   compileTestMethods();

   setIlVerifier(oldVerifier);
   TR::Optimizer::setMockStrategy(NULL);

   ASSERT_EQ(true, noCodegenVerifier.hasRun()) << "Did not run verifiers.";
   ASSERT_EQ(0, noCodegenVerifier.getReturnCode()) << "One or more verifiers failed.";
   }

/**
 * This generates IL, runs the requested optimizations,
 * verifies the optimized IL, compiles it, then invokes tests.
 *
 * Must be called after setMethodInfo() and setIlVerifier().
 * Should be called after all requested optimizations are
 * added via addOptimization().
 */
void TestCompiler::OptTestDriver::VerifyAndInvoke()
   {
   OptimizationStrategy strategy[_optimizations.size() + 1];
   makeOptimizationStrategyArray(strategy);
   TR::Optimizer::setMockStrategy(strategy);

   RunTest();

   TR::Optimizer::setMockStrategy(NULL);
   }

/**
 * Append a single optimization to the list of optimizations to perform.
 * The optimization is marked as `MustBeDone`.
 *
 * @param opt The optimization to perform.
 */
void TestCompiler::OptTestDriver::addOptimization(OMR::Optimizations opt)
   {
   OptimizationStrategy strategy = {opt, OMR::MustBeDone};
   _optimizations.push_back(strategy);
   }

/**
 * Append an optimization strategy to the list of optimizations to perform.
 *
 * @param opts An array of optimizations to perform. The last item in this
 * array must be `endOpts` or `endGroup`.
 */
void TestCompiler::OptTestDriver::addOptimizations(const OptimizationStrategy *opts)
   {
   const OptimizationStrategy *end = opts;
   while(end->_num != OMR::endOpts && end->_num != OMR::endGroup)
      ++end;

   _optimizations.insert(_optimizations.end(), opts, end);
   }

/**
 * @brief Compiles the method.
 *
 * This is called by Verify() and VerifiyAndInvoke().
 * The default implementation gets a #ResolvedMethod from #MethodInfo,
 * adds the IlVerifier, then compiles the method.
 */
void TestCompiler::OptTestDriver::compileTestMethods()
   {
   TR::ResolvedMethod resolvedMethod = _methodInfo->ResolvedMethod();
   TR::IlGeneratorMethodDetails details(&resolvedMethod);
   details.setIlVerifier(_ilVer);

   int32_t rc = 0;
   _compiledMethod = compileMethod(details, warm, rc);
   }
