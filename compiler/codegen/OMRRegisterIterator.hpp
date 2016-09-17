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

#ifndef OMR_REGISTER_ITERATOR_INCL
#define OMR_REGISTER_ITERATOR_INCL

/*
 * The following #define and typedef must appear before any #includes in this file
 */
#ifndef OMR_REGISTER_ITERATOR_CONNECTOR
#define OMR_REGISTER_ITERATOR_CONNECTOR
namespace OMR { class RegisterIterator; }
namespace OMR { typedef OMR::RegisterIterator RegisterIteratorConnector; }
#endif

#include "env/TRMemory.hpp"  // for TR_Memory, etc
namespace TR { class Register; }

namespace OMR
{

/*
 * This interface must be implemented by platform-specific descendents,
 * using either List and ListIterator, or some cheaper alternatives.
 */
class OMR_EXTENSIBLE RegisterIterator
   {
   public:
   TR_ALLOC(TR_Memory::RegisterIterator)

   virtual TR::Register *getFirst() = 0;

   virtual TR::Register *getCurrent() = 0;

   virtual TR::Register *getNext() = 0;

   };

}

#endif
