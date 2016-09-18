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

#ifndef OMR_TABLEOFCONSTANTS_INCL
#define OMR_TABLEOFCONSTANTS_INCL

/*
 * This class is used to manage the mapping/assigning of table of constant (TOC)
 * slots on the platforms which choose to support it.  Each code generator should
 * extend this class to realize exactly how the TOC is managed (e.g., linearly
 * allocated or hashed).
 */

#include <stdint.h>  // for uint32_t

namespace OMR
{

class TableOfConstants
   {

   public:

   TableOfConstants(uint32_t size)
      : _sizeInBytes(size) {}

   uint32_t getTOCSize() {return _sizeInBytes;}
   void setTOCSize(uint32_t s) {_sizeInBytes = s;}

   private:

   uint32_t _sizeInBytes;
   };

}

using OMR::TableOfConstants;

#endif
