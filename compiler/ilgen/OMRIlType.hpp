/*******************************************************************************
 * Copyright (c) 2016, 2018 IBM Corp. and others
 *
 * This program and the accompanying materials are made available under
 * the terms of the Eclipse Public License 2.0 which accompanies this
 * distribution and is available at http://eclipse.org/legal/epl-2.0
 * or the Apache License, Version 2.0 which accompanies this distribution
 * and is available at https://www.apache.org/licenses/LICENSE-2.0.
 *
 * This Source Code may also be made available under the following Secondary
 * Licenses when the conditions for such availability set forth in the
 * Eclipse Public License, v. 2.0 are satisfied: GNU General Public License,
 * version 2 with the GNU Classpath Exception [1] and GNU General Public
 * License, version 2 with the OpenJDK Assembly Exception [2].
 *
 * [1] https://www.gnu.org/software/classpath/license.html
 * [2] http://openjdk.java.net/legal/assembly-exception.html
 *
 * SPDX-License-Identifier: EPL-2.0 OR Apache-2.0 OR GPL-2.0 WITH Classpath-exception-2.0 OR LicenseRef-GPL-2.0 WITH Assembly-exception
 *******************************************************************************/

#ifndef OMR_ILTYPE_INCL
#define OMR_ILTYPE_INCL

#include "il/DataTypes.hpp"

class TR_Memory;
#ifndef TR_ALLOC
#define TR_ALLOC(x)
#endif


namespace TR { class IlType; }

namespace OMR
{

class IlType
   {
public:
   TR_ALLOC(TR_Memory::IlGenerator)

   IlType(const char *name) :
      _name(name)
      { }
   IlType() :
      _name(0)
      { }
   virtual ~IlType()
      { }

   const char *getName() { return _name; }
   virtual char *getSignatureName();

   virtual TR::DataType getPrimitiveType() { return TR::NoType; }

   virtual bool isArray() { return false; }
   virtual bool isPointer() { return false; }
   virtual TR::IlType *baseType() { return NULL; }

   virtual bool isStruct() {return false; }
   virtual bool isUnion() { return false; }

   virtual size_t getSize();

protected:
   const char *_name;
   static const char * signatureNameForType[TR::NumOMRTypes];
   static const uint8_t primitiveTypeAlignment[TR::NumOMRTypes];
   };

} // namespace OMR

#endif // !defined(OMR_ILTYPE_INCL)
