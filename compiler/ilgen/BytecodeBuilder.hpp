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


#ifndef BYTECODE_BUILDER_INCL
#define BYTECODE_BUILDER_INCL

#ifndef TR_BYTECODEBUILDER_DEFINED
#define TR_BYTECODEBUILDER_DEFINED
#define PUT_OMR_BYTECODEBUILDER_INTO_TR
#endif // !defined(TR_BYTECODEBUILDER_DEFINED)

#include "ilgen/IlBuilder.hpp"

namespace TR { class BytecodeBuilder; }
namespace TR { class MethodBuilder; }
namespace OMR { class VirtualMachineState; }

namespace OMR
{

class BytecodeBuilder : public TR::IlBuilder
   {
public:
   TR_ALLOC(TR_Memory::IlGenerator)

   BytecodeBuilder(TR::MethodBuilder *methodBuilder, int32_t bcIndex, char *name=NULL);

   virtual bool isBytecodeBuilder() { return true; }

   virtual uint32_t countBlocks();

   void AddFallThroughBuilder(TR::BytecodeBuilder *ftb);

   void AddSuccessorBuilders(uint32_t numBuilders, ...);
   void AddSuccessorBuilder(TR::BytecodeBuilder **b) { AddSuccessorBuilders(1, b); }

   OMR::VirtualMachineState *initialVMState()                { return _initialVMState; }
   OMR::VirtualMachineState *vmState()                       { return _vmState; }
   void setVMState(OMR::VirtualMachineState *vmState)        { _vmState = vmState; }

   void propagateVMState(OMR::VirtualMachineState *fromVMState);

   void Goto(TR::BytecodeBuilder *dest);
   void IfCmpEqual(TR::BytecodeBuilder **dest, TR::IlValue *v1, TR::IlValue *v2);
   void IfCmpNotEqual(TR::BytecodeBuilder **dest, TR::IlValue *v1, TR::IlValue *v2);

protected:
   TR::BytecodeBuilder       * _fallThroughBuilder;
   List<TR::BytecodeBuilder> * _successorBuilders;
   int32_t                     _bcIndex;
   char                      * _name;
   OMR::VirtualMachineState  * _initialVMState;
   OMR::VirtualMachineState  * _vmState;

   virtual void appendBlock(TR::Block *block = 0, bool addEdge=true);
   void addAllSuccessorBuildersToWorklist();
   bool connectTrees();
   virtual void setHandlerInfo(uint32_t catchType);
   void transferVMState(TR::BytecodeBuilder **b);
   };

} // namespace OMR


#if defined(PUT_OMR_BYTECODEBUILDER_INTO_TR)

namespace TR
{
   class BytecodeBuilder : public OMR::BytecodeBuilder
      {
      public:
         BytecodeBuilder(TR::MethodBuilder *methodBuilder, int32_t bcIndex, char *name=NULL)
            : OMR::BytecodeBuilder(methodBuilder, bcIndex, name)
            { }
         void initialize(TR::IlGeneratorMethodDetails * details,
                           TR::ResolvedMethodSymbol     * methodSymbol,
                           TR::FrontEnd                 * fe,
                           TR::SymbolReferenceTable     * symRefTab); 
      };

} // namespace TR

#endif // defined(PUT_OMR_BYTECODEBUILDER_INTO_TR)

#endif // !defined(OMR_ILBUILDER_INCL)
