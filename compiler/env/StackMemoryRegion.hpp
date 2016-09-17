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

#ifndef TR_STACKREGION_HPP
#define TR_STACKREGION_HPP

#pragma once

#include "env/Region.hpp"

class TR_Memory;

namespace TR {

class StackMemoryRegion : public Region
   {
public:
   explicit StackMemoryRegion(TR_Memory &trMemory);
   virtual ~StackMemoryRegion() throw();

private:
   TR_Memory &_trMemory;
   TR::Region &_previousStackRegion;
   };

}

#endif // STACKREGION_HPP

