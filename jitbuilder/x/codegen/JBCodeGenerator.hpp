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

#ifndef JITBUILDER_X86_CODEGENERATORBASE_INCL
#define JITBUILDER_X86_CODEGENERATORBASE_INCL

/*
 * The following #define and typedef must appear before any #includes in this file
 */
#ifndef JITBUILDER_CODEGENERATORBASE_CONNECTOR
#define JITBUILDER_CODEGENERATORBASE_CONNECTOR

namespace JitBuilder { namespace X86 { class CodeGenerator; } }
namespace JitBuilder { typedef X86::CodeGenerator CodeGeneratorConnector; }

#else
#error JitBuilder::X86::CodeGenerator expected to be a primary connector, but a JitBuilder connector is already defined
#endif // !defined(JITBUILDER_CODEGENERATORBASE_CONNECTOR)


#include "jitbuilder/codegen/JBCodeGenerator.hpp"
#include "codegen/LinkageConventionsEnum.hpp"

namespace JitBuilder
{

namespace X86
{

class OMR_EXTENSIBLE CodeGenerator : public JitBuilder::CodeGenerator
   {
   public:

   CodeGenerator() :
      JitBuilder::CodeGenerator() {}

   TR::Linkage *createLinkage(TR_LinkageConventions lc);

   void beginInstructionSelection();

   void endInstructionSelection();

   TR::Instruction *generateSwitchToInterpreterPrePrologue(
         TR::Instruction *prev,
         uint8_t alignment,
         uint8_t alignmentMargin);
   };

} // namespace X86

} // namespace JitBuilder

#endif // !defined(JITBUILDER_X86_CODEGENERATORBASE_INCL)


