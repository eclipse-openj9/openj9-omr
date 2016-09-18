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


#ifndef MANDELBROT_INCL
#define MANDELBROT_INCL

#include "ilgen/MethodBuilder.hpp"

typedef void (MandelbrotFunctionType)(int32_t, uint8_t *buffer, double *cr0);

class MandelbrotMethod : public TR::MethodBuilder
   {
   private:
   TR::IlType *pInt8;
   TR::IlType *pDouble;

   public:
   MandelbrotMethod(TR::TypeDictionary *types);
   virtual bool buildIL();
   };

#endif // !defined(MANDELBROT_INCL)
