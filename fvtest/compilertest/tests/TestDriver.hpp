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

#ifndef TESTDRIVER_HPP
#define TESTDRIVER_HPP

#include <stdint.h>
#include "compile/CompilationTypes.hpp"

namespace TR { class IlGeneratorMethodDetails; }
namespace TR { class MethodBuilder; }

extern "C" uint8_t *compileMethod(TR::IlGeneratorMethodDetails &, TR_Hotness, int32_t &);

namespace TestCompiler
{
#define TOSTR(x)     #x
#define LINETOSTR(x) TOSTR(x)

class TestDriver
   {
   public:
   void RunTest();
   virtual void compileTestMethods() = 0;
   virtual void invokeTests() = 0;

   int32_t compileMethodBuilder(TR::MethodBuilder *m, uint8_t **entry);
   };

}// namespace TestCompiler

#endif
