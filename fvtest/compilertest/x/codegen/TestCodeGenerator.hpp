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

#ifndef TEST_X86_CODEGENERATORBASE_INCL
#define TEST_X86_CODEGENERATORBASE_INCL

/*
 * The following #define and typedef must appear before any #includes in this file
 */
#ifndef TEST_CODEGENERATORBASE_CONNECTOR
#define TEST_CODEGENERATORBASE_CONNECTOR

namespace Test { namespace X86 { class CodeGenerator; } }
namespace Test { typedef X86::CodeGenerator CodeGeneratorConnector; }

#else
#error Test::X86::CodeGenerator expected to be a primary connector, but a Test connector is already defined
#endif


#include "compilertest/codegen/TestCodeGenerator.hpp"
#include "codegen/LinkageConventionsEnum.hpp"

namespace Test
{

namespace X86
{

class OMR_EXTENSIBLE CodeGenerator : public Test::CodeGenerator
   {
   public:

   CodeGenerator() :
      Test::CodeGenerator() {}

   TR::Linkage *createLinkage(TR_LinkageConventions lc);

   void beginInstructionSelection();

   void endInstructionSelection();

   TR::Instruction *generateSwitchToInterpreterPrePrologue(
         TR::Instruction *prev,
         uint8_t alignment,
         uint8_t alignmentMargin);
   };

} // namespace X86

} // namespace Test

#endif // !defined(TEST_CODEGENERATORBASE_CONNECTOR)


