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

#ifndef IA32_LINKAGEUTILS_INCL
#define IA32_LINKAGEUTILS_INCL

namespace TR { class CodeGenerator; }
namespace TR { class Node; }
namespace TR { class Register; }

namespace TR
{

class IA32LinkageUtils
   {
   public:

   static TR::Register *pushIntegerWordArg(TR::Node *child, TR::CodeGenerator *cg);
   static TR::Register *pushLongArg(TR::Node *child, TR::CodeGenerator *cg);
   static TR::Register *pushFloatArg(TR::Node *child, TR::CodeGenerator *cg);
   static TR::Register *pushDoubleArg(TR::Node *child, TR::CodeGenerator *cg);
   };

}
#endif
