/*******************************************************************************
 *
 * (c) Copyright IBM Corp. 2016, 2017
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


#ifndef OPERANDARRAYTESTS_INCL
#define OPERANDARRAYTESTS_INCL

#include "ilgen/MethodBuilder.hpp"

#define ARRAYVALUEILTYPE Int32
#define ARRAYVALUETYPE   int32_t

namespace TR { class BytecodeBuilder; }

class OperandArrayTestMethod : public TR::MethodBuilder
   {
   public:
   OperandArrayTestMethod(TR::TypeDictionary *);
   virtual bool buildIL();

   static void verify(const char *step, int32_t max, int32_t num, ...);
   static bool verifyUntouched(int32_t maxTouched);

   ARRAYVALUETYPE **getArrayPtr() { return &_realArray; }

   protected:
   bool testArray(TR::BytecodeBuilder *b, bool useEqual);

   TR::IlType                      *_valueType;

   static ARRAYVALUETYPE           *_realArray;
   static int32_t                   _realArrayLength;
   
   static void createArray();
   static ARRAYVALUETYPE *moveArray();
   static void freeArray();
   };
   
class OperandArrayTestUsingFalseMethod : public OperandArrayTestMethod
   {
   public:
   OperandArrayTestUsingFalseMethod(TR::TypeDictionary *);
   virtual bool buildIL();
   };

#endif // !defined(OPERANDARRAYTESTS_INCL)
