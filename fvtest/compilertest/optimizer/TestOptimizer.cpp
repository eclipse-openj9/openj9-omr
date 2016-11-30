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

#include "optimizer/Optimizer.hpp"

const OptimizationStrategy *TestCompiler::Optimizer::_mockStrategy = NULL;

const OptimizationStrategy *
TestCompiler::Optimizer::optimizationStrategy(TR::Compilation *c)
   {
   if(TestCompiler::Optimizer::_mockStrategy)
      return TestCompiler::Optimizer::_mockStrategy;
   else
      return OMR::OptimizerConnector::optimizationStrategy(c);
   }
