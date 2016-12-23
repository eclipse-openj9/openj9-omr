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

#ifndef IlVerifier_hpp
#define IlVerifier_hpp

namespace TR { class ResolvedMethodSymbol; }

namespace TR {

class IlVerifier
   {
   public:
   /**
    * Verify the IL of a method has certain properties.
    *
    * @return 0 on success, or a non-zero error code. If 0 is returned,
    * compilation stops.
    */
   virtual int32_t verify(TR::ResolvedMethodSymbol *methodSymbol) = 0; 
   };

}

#endif
