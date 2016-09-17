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

#ifndef OMR_FLAGS_INCL
#define OMR_FLAGS_INCL

#include <stdint.h>  // for uint16_t, uint32_t, uint64_t, uint8_t


#define TO_MASK8(b) (1<<(uint8_t)b)
#define TO_MASK16(b) (1<<(uint16_t)b)
#define TO_MASK32(b) (1<<(uint32_t)b)

namespace OMR
{

// Maintain a set of ``packed'' flags and values as an unsigned type.
// Supply methods to test the values of flags and values.
//
template<class T> class FlagsTemplate
   {

   public:

   FlagsTemplate(T t = 0) : _flags(t) {}

   bool testAny(T mask) const { return (_flags & mask) != 0; }
   bool testAll(T mask) const { return (_flags & mask) == mask; }

   void set(T mask) { _flags |= mask; }
   void set(FlagsTemplate<T> mask) { _flags |= mask._flags; }
   void reset(T mask) { _flags &= ~mask; }
   void set(T mask, bool b) { b ? set(mask) : reset(mask); }
   void toggle(T mask) { _flags ^= mask; }
   void clear() { _flags = 0; }
   bool isClear() { return (_flags == 0); }

   // The next three methods manipulate a single multi-bit value.
   // It can be used, for example, to pack small integers together.
   //
   bool testValue(T mask, T value) const { return (_flags & mask) == value; }
   void setValue(T mask, T value) { _flags = (_flags & ~mask) | value; }
   T getValue(T mask) const { return _flags & mask; }
   T getValue() const { return _flags; }

   bool operator==(FlagsTemplate<T> &other) { return getValue() == other.getValue(); }

   private:

   T _flags;
   };

typedef FlagsTemplate<uint8_t>  flags8_t;
typedef FlagsTemplate<uint16_t> flags16_t;
typedef FlagsTemplate<uint32_t> flags32_t;
typedef FlagsTemplate<uint64_t> flags64_t;

}

using OMR::flags8_t;
using OMR::flags16_t;
using OMR::flags32_t;
using OMR::flags64_t;

#endif
