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
 ******************************************************************************/


#ifndef MATMULT_INCL
#define MATMULT_INCL

#include "ilgen/MethodBuilder.hpp"

typedef void (MatMultFunctionType)(double *, double *, double *, int32_t);

class MatMult : public TR::MethodBuilder
   {
   private:
   TR::IlType *pDouble;

   void Store2D(TR::IlBuilder *bldr,
                TR::IlValue *base,
                TR::IlValue *first,
                TR::IlValue *second,
                TR::IlValue *N,
                TR::IlValue *value);
   TR::IlValue *Load2D(TR::IlBuilder *bldr,
                       TR::IlValue *base,
                       TR::IlValue *first,
                       TR::IlValue *second,
                       TR::IlValue *N);

   public:
   MatMult(TR::TypeDictionary *);
   virtual bool buildIL();
   };

class VectorMatMult : public TR::MethodBuilder
   {
   private:
   TR::IlType *pDouble;
   TR::IlType *ppDouble;

   void VectorStore2D(TR::IlBuilder *bldr,
                      TR::IlValue *base,
                      TR::IlValue *first,
                      TR::IlValue *second,
                      TR::IlValue *N,
                      TR::IlValue *value);
   TR::IlValue *VectorLoad2D(TR::IlBuilder *bldr,
                             TR::IlValue *base,
                             TR::IlValue *first,
                             TR::IlValue *second,
                             TR::IlValue *N);
   TR::IlValue *Load2D(TR::IlBuilder *bldr,
                       TR::IlValue *base,
                       TR::IlValue *first,
                       TR::IlValue *second,
                       TR::IlValue *N);

   public:
   VectorMatMult(TR::TypeDictionary *);
   virtual bool buildIL();
   };

#endif // !defined(MATMULT_INCL)
