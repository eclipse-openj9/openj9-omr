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


#ifndef WORKLIST_INCL
#define WORKLIST_INCL

#include "ilgen/MethodBuilder.hpp"

class WorklistMethod : public TR::MethodBuilder
   {
   public:
   WorklistMethod(TR::TypeDictionary *);
   virtual bool buildIL();
   };

#endif // !defined(WORKLIST_INCL)
