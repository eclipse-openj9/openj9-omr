/*******************************************************************************
 *
 * (c) Copyright IBM Corp. 2017, 2017
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

#ifndef FIELDADDRESS_INCL
#define FIELDADDRESS_INCL

#include "ilgen/MethodBuilder.hpp"

namespace TR { class TypeDictionary; }

struct Struct
   {
   uint16_t f1;
   uint8_t f2;
   };

union Union
   {
   uint16_t f1;
   uint8_t f2;
   };

typedef uint8_t* (GetStructFieldAddressFunction)(Struct*);
typedef uint8_t* (GetUnionFieldAddressFunction)(Union*);

class GetStructFieldAddressBuilder : public TR::MethodBuilder
   {
   private:
   TR::IlType *pStructType;

   public:
   GetStructFieldAddressBuilder(TR::TypeDictionary *);
   virtual bool buildIL();
   };

class GetUnionFieldAddressBuilder : public TR::MethodBuilder
   {
   private:
   TR::IlType *pUnionType;

   public:
   GetUnionFieldAddressBuilder(TR::TypeDictionary *);
   virtual bool buildIL();
   };

#endif // FIELDADDRESS_INCL
