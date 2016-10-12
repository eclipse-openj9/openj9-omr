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

#ifndef VPCONSTRAINT_INCL
#define VPCONSTRAINT_INCL

#include <stdint.h>                      // for int32_t, int16_t, int64_t, etc
#include <string.h>                      // for NULL, strncmp
#include "env/KnownObjectTable.hpp"  // for KnownObjectTable, etc
#include "env/TRMemory.hpp"              // for TR_Memory, etc
#include "env/jittypes.h"                // for uintptrj_t
#include "il/DataTypes.hpp"              // for DataTypes, DataTypes::Int16, etc
#include "infra/Assert.hpp"              // for TR_ASSERT
#include "infra/Bit.hpp"                 // for getPrecisionFromValue, etc
#include "infra/List.hpp"                // for TR_ScratchList, ListElement

class TR_FrontEnd;
class TR_OpaqueClassBlock;
class TR_ResolvedMethod;
class TR_VPArrayInfo;
class TR_VPClassPresence;
class TR_VPClassType;
class TR_VPConstString;
class TR_VPKnownObject;
class TR_VPObjectLocation;
class TR_VPPreexistentObject;
class TR_ValuePropagation;
class TR_VP_BCDValue;
class TR_VP_BCDSign;

namespace TR { class Compilation; }
namespace TR { class Node; }
namespace TR { class SymbolReference; }

#define VP_HASH_TABLE_SIZE 251

#define VP_UNDEFINED_PRECISION -1

// ***************************************************************************
//
// Value propagation constraints. A constraint is a property that puts some
// limit on a value.
//
// ***************************************************************************

class TR_VPConstraint
   {
   public:
   TR_ALLOC(TR_Memory::ValuePropagation)

   TR_VPConstraint(int32_t p) : _mergePriority(p) {_unsignedType = false;}

   virtual class TR_VPShortConstraint   *asShortConstraint();
   virtual class TR_VPShortConst        *asShortConst();
   virtual class TR_VPShortRange        *asShortRange();
   virtual class TR_VPIntConstraint     *asIntConstraint();
   virtual class TR_VPIntConst          *asIntConst();
   virtual class TR_VPIntRange          *asIntRange();
   virtual class TR_VPLongConstraint    *asLongConstraint();
   virtual class TR_VPLongConst         *asLongConst();
   virtual class TR_VPLongRange         *asLongRange();
   virtual class TR_VP_BCDValue         *asBCDValue();
   virtual class TR_VP_BCDSign          *asBCDSign();
   virtual class TR_VPClass             *asClass();
   virtual class TR_VPClassType         *asClassType();
   virtual class TR_VPResolvedClass     *asResolvedClass();
   virtual class TR_VPFixedClass        *asFixedClass();
   virtual class TR_VPKnownObject       *asKnownObject();
   virtual class TR_VPConstString       *asConstString();
   virtual class TR_VPUnresolvedClass   *asUnresolvedClass();
     //virtual class TR_VPImplementedInterface *asImplementedInterface();
   virtual class TR_VPClassPresence     *asClassPresence();
   virtual class TR_VPNullObject        *asNullObject();
   virtual class TR_VPNonNullObject     *asNonNullObject();
   virtual class TR_VPPreexistentObject *asPreexistentObject();
   virtual class TR_VPArrayInfo         *asArrayInfo();
   virtual class TR_VPObjectLocation    *asObjectLocation();
   virtual class TR_VPMergedConstraints *asMergedConstraints();
   virtual class TR_VPMergedConstraints *asMergedShortConstraints();
   virtual class TR_VPMergedConstraints *asMergedIntConstraints();
   virtual class TR_VPMergedConstraints *asMergedLongConstraints();
   virtual class TR_VPUnreachablePath   *asUnreachablePath();
   virtual class TR_VPSync              *asVPSync();
   virtual class TR_VPRelation          *asRelation();
   virtual class TR_VPLessThanOrEqual   *asLessThanOrEqual();
   virtual class TR_VPGreaterThanOrEqual*asGreaterThanOrEqual();
   virtual class TR_VPEqual             *asEqual();
   virtual class TR_VPNotEqual          *asNotEqual();

   virtual const char *name()=0;

   static TR_VPConstraint *create(TR_ValuePropagation *vp, const char *sig, int32_t len, TR_ResolvedMethod *method, bool isFixedClass);
   static bool isSpecialClass(uintptrj_t klass);


   TR_VPConstraint *merge(TR_VPConstraint *other, TR_ValuePropagation *vp);
   TR_VPConstraint *intersect(TR_VPConstraint *other, TR_ValuePropagation *vp);
   virtual TR_VPConstraint *merge1(TR_VPConstraint *other, TR_ValuePropagation *vp);
   virtual TR_VPConstraint *intersect1(TR_VPConstraint *other, TR_ValuePropagation *vp);

   // Arithmetic operations
   //
   virtual TR_VPConstraint *add(TR_VPConstraint *other, TR::DataType type, TR_ValuePropagation *vp);
   virtual TR_VPConstraint *subtract(TR_VPConstraint *other, TR::DataType type, TR_ValuePropagation *vp);
   virtual TR_YesNoMaybe canOverflow();
   virtual void setCanOverflow(TR_YesNoMaybe v) {}

   // Absolute comparisons
   //
   virtual bool mustBeEqual(TR_VPConstraint *other, TR_ValuePropagation *vp);
   virtual bool mustBeNotEqual(TR_VPConstraint *other, TR_ValuePropagation *vp);
   virtual bool mustBeLessThan(TR_VPConstraint *other, TR_ValuePropagation *vp);
   virtual bool mustBeLessThanOrEqual(TR_VPConstraint *other, TR_ValuePropagation *vp);

   // Relative comparisons
   //
   virtual bool mustBeEqual();
   virtual bool mustBeNotEqual();
   virtual bool mustBeLessThan();
   virtual bool mustBeLessThanOrEqual();
   virtual bool mustBeGreaterThan();
   virtual bool mustBeGreaterThanOrEqual();

   // Integer information
   //
   virtual int16_t getLowShort();
   virtual int16_t getHighShort();
   virtual int32_t getLowInt();
   virtual int32_t getHighInt();
   virtual int64_t getLowLong();
   virtual int64_t getHighLong();


   virtual uint16_t getUnsignedLowShort();
   virtual uint16_t getUnsignedHighShort();
   virtual uint32_t getUnsignedLowInt();
   virtual uint32_t getUnsignedHighInt();
   virtual uint64_t getUnsignedLowLong();
   virtual uint64_t getUnsignedHighLong();



   // Class information
   //
   virtual bool isNullObject();
   virtual bool isNonNullObject();
   virtual bool isPreexistentObject();
   virtual TR_OpaqueClassBlock *getClass();
   virtual bool isFixedClass();
   virtual bool isConstString();
   virtual TR_VPClassType *getClassType();
   virtual TR_VPClassPresence *getClassPresence();
   virtual TR_VPPreexistentObject *getPreexistence();
   virtual TR_VPObjectLocation *getObjectLocation();
   virtual TR_VPConstString *getConstString();
   virtual TR_VPKnownObject *getKnownObject();
   virtual TR_VPArrayInfo *getArrayInfo();
   virtual const char *getClassSignature(int32_t &len);

   virtual TR_YesNoMaybe isStackObject();
   virtual TR_YesNoMaybe isHeapObject();
   virtual TR_YesNoMaybe isClassObject();
   virtual TR_YesNoMaybe isJavaLangClassObject();
   virtual TR_YesNoMaybe isJ9ClassObject();

   // Merge priorities, used in merge and intersect methods. The method called
   // is the one belonging to the highest priority
   //
     /*
   enum MergePriorities
      {
      EqualPriority             = 16,
      GreaterThanOrEqualPriority= 15,
      LessThanOrEqualPriority   = 14,
      NotEqualPriority          = 13,
      MergedConstraintPriority  = 12,
      IntPriority               = 11,
      LongPriority              = 10,
      ClassPriority             = 9,
      ConstStringPriority       = 8,
      FixedClassPriority        = 7,
      ResolvedClassPriority     = 6,
      UnresolvedClassPriority   = 5,
      ClassPresencePriority     = 4,
      ClassPreexistencePriority = 3,
      ArrayInfoPriority         = 2,
      ObjectLocationPriority    = 1,
      ImplementsInterfacePriority = 0,
      highestPriority
      };
     */

   enum MergePriorities
      {
      EqualPriority             = 19,
      GreaterThanOrEqualPriority= 18,
      LessThanOrEqualPriority   = 17,
      NotEqualPriority          = 16,
      MergedConstraintPriority  = 15,
      ShortPriority             = 14,
      IntPriority               = 13,
      LongPriority              = 12,
      BCDPriority               = 11,
      ClassPriority             = 10,
      KnownObjectPriority       = 9,
      ConstStringPriority       = 8,
      FixedClassPriority        = 7,
      ResolvedClassPriority     = 6,
      UnresolvedClassPriority   = 5,
      ClassPresencePriority     = 4,
      ClassPreexistencePriority = 3,
      ArrayInfoPriority         = 2,
      ObjectLocationPriority    = 1,
      SyncPriority              = 0,
      highestPriority
      };

   int32_t priority() {return (_mergePriority & 0x7fffffff);}

   void setHasArtificialIncrement() { (_mergePriority = _mergePriority | 0x80000000); }
   bool hasArtificialIncrement() { return ((_mergePriority & 0x80000000) != 0); }

   void print(TR_ValuePropagation *vp);
   virtual void print(TR::Compilation *, TR::FILE *);
   virtual void print(TR::Compilation *, TR::FILE *, int32_t relative);

   bool isUnsigned() { return _unsignedType; }
   void setIsUnsigned(bool b) { TR_ASSERT(0 == 1, "I should eventually remove this property from here"); _unsignedType = b; } //FIXME//CRITICAL

   virtual int32_t getPrecision() {return TR_MAX_DECIMAL_PRECISION;}

   protected:
   bool _unsignedType;

   class Tracer {
      public:
      Tracer(TR_ValuePropagation *vp, TR_VPConstraint *self, TR_VPConstraint *other, const char *name);
      ~Tracer();

      TR_ValuePropagation *vp(){ return _vp; }
      TR::Compilation      *comp();

      protected:
      TR_ValuePropagation *_vp;
      TR_VPConstraint     *_self;
      TR_VPConstraint     *_other;
      const char          *_name;
   };

   // The following {add,sub}WithOverflow require that the build (C++) compiler
   // use two's complement (virtually guaranteed), and that type_t be a signed
   // integer type.

   template<typename type_t> type_t addWithOverflow(type_t a, type_t b, bool& overflow)
      {
      // Cast to uintmax_t to avoid undefined behaviour on signed overflow.
      //
      // The uintmax_t-typed operands are sign-extended from a and b. They
      // won't undergo integer promotions, because they're already as large as
      // possible. And they're already at a common type, so they won't undergo
      // the "usual arithmetic conversions" either. Therefore this will do
      // unsigned (modular) arithmitic. Converting back to type_t produces the
      // expected two's complement result.
      type_t sum = uintmax_t(a) + uintmax_t(b);

      //The overflow flag is set when the arithmetic used to compute the sum overflows. This happens exactly
      //when both operand have the same sign and the resulting value has the opposite sign to this.
      //A non-negative result from xoring two numbers indicates that their sign bits are either both
      //set, or both unset, meaning they have the same sign, while a negative result from xoring two
      //numbers means that exactly one of them has its sign bit set, meaning the signs of the two numbers
      //differ.
      overflow = ( a ^ b ) >= 0 && ( a ^ sum ) < 0;

      return sum;
      }

   template<typename type_t> type_t subWithOverflow(type_t a, type_t b, bool& overflow)
      {
      // About uintmax_t, see addWithOverflow above.
      type_t diff = uintmax_t(a) - uintmax_t(b);

      //The overflow flag is set when the arithmetic used to compute the difference overflows. This happens exactly
      //when both operand bounds have a differing sign and the resulting value has the same sign as the first operand.
      //A non-negative result from xoring two numbers indicates that their sign bits are either both
      //set, or both unset, meaning they have the same sign, while a negative result from xoring two
      //numbers means that exactly one of them has its sign bit set, meaning the signs of the two numbers
      //differ.
      overflow = ( a ^ b ) < 0 && ( a ^ diff ) < 0;

      return diff;
      }

   private:
   int32_t _mergePriority;
   };

#define TRACER(vp, self, other) Tracer _tracer_(vp, self, other, __func__)

class TR_VPShortConstraint: public TR_VPConstraint
   {
   public:
   TR_VPShortConstraint(int16_t v) : TR_VPConstraint(ShortPriority) {_low=v; _overflow = TR_no;}
   virtual TR_VPShortConstraint * asShortConstraint();

   int16_t getShort() {return _low; }
   int16_t getLow() {return _low; }
   virtual int16_t  getHigh() = 0;
   virtual int16_t  getLowShort();
   virtual int16_t  getHighShort();

   virtual TR_VPConstraint * merge1 ( TR_VPConstraint * other, TR_ValuePropagation * vp);
   virtual TR_VPConstraint * intersect1 (TR_VPConstraint * other, TR_ValuePropagation * vp);

   virtual bool mustBeNotEqual (TR_VPConstraint * other, TR_ValuePropagation *vp);
   virtual bool mustBeLessThan (TR_VPConstraint * other, TR_ValuePropagation *vp);
   virtual bool mustBeLessThanOrEqual (TR_VPConstraint * other, TR_ValuePropagation *vp);

   virtual TR_VPConstraint *add(TR_VPConstraint * other, TR::DataType type, TR_ValuePropagation *vp);
   virtual TR_VPConstraint *subtract(TR_VPConstraint *other, TR::DataType type, TR_ValuePropagation * vp);

   virtual TR_YesNoMaybe canOverflow() {return _overflow;}
   virtual void setCanOverflow(TR_YesNoMaybe v) {_overflow = v;}

   virtual int32_t getPrecision() {return getPrecisionFromValue(TR::getMaxSigned<TR::Int16>());}

   protected:
   int16_t _low;
   TR_YesNoMaybe _overflow;

   private:
   TR_VPConstraint *getRange(int16_t, int16_t, bool, bool, TR_ValuePropagation * vp);
   };

class TR_VPShortConst : public TR_VPShortConstraint
   {
   public:
   TR_VPShortConst(int16_t v) : TR_VPShortConstraint(v) {}
   static TR_VPShortConst *create(TR_ValuePropagation *vp, int16_t v);
   static TR_VPConstraint *createExclusion(TR_ValuePropagation *vp, int16_t v);
   // unsigned createExclusion
   //static TR_VPConstraint *createExclusion(TR_ValuePropagation *vp, int32_t v);
   virtual TR_VPShortConst *asShortConst();
   virtual int16_t getHigh() {return _low;}
   virtual bool mustBeEqual(TR_VPConstraint *other, TR_ValuePropagation *vp);

   virtual void print(TR::Compilation *, TR::FILE *);
   virtual const char *name();

   virtual int32_t getPrecision() {return getPrecisionFromValue(_low);}
   };

class TR_VPShortRange : public TR_VPShortConstraint
   {
   public:
   TR_VPShortRange(int16_t low, int16_t high) : TR_VPShortConstraint(low), _high(high) {}
   static TR_VPShortConstraint *create(TR_ValuePropagation *vp, int16_t low, int16_t high, TR_YesNoMaybe canOverflow = TR_no);
   static TR_VPShortConstraint *create(TR_ValuePropagation *vp);
   static TR_VPShortConstraint *createWithPrecision(TR_ValuePropagation *vp, int32_t precision, bool isNonNegative = false);
   virtual TR_VPShortRange *asShortRange();
   virtual int16_t getHigh() {return _high;}

   virtual void print(TR::Compilation *, TR::FILE *);
   virtual const char *name();

   virtual int32_t getPrecision() {return getPrecisionFromRange(_low, _high);}

   private:
   int16_t _high;
   };

class TR_VPIntConstraint : public TR_VPConstraint
   {
   public:
   TR_VPIntConstraint(int32_t v) : TR_VPConstraint(IntPriority) {_low = v; _overflow = TR_no;}
   virtual TR_VPIntConstraint *asIntConstraint();

   int32_t getInt() {return _low;}
   int32_t getLow() {return _low;}
   virtual int32_t getHigh() = 0;
   virtual int32_t getLowInt();
   virtual int32_t getHighInt();

   virtual TR_VPConstraint *merge1(TR_VPConstraint *other, TR_ValuePropagation *vp);
   // unsigned intMerge
   //TR_VPConstraint *merge1(TR_VPConstraint *other, TR_ValuePropagation *vp, bool isUnsigned);
   virtual TR_VPConstraint *intersect1(TR_VPConstraint *other, TR_ValuePropagation *vp);
   // unsigned intIntersect
   //TR_VPConstraint *intersect1(TR_VPConstraint *other, TR_ValuePropagation *vp, bool isUnsigned);
   virtual bool mustBeNotEqual(TR_VPConstraint *other, TR_ValuePropagation *vp);
   virtual bool mustBeLessThan(TR_VPConstraint *other, TR_ValuePropagation *vp);
   virtual bool mustBeLessThanOrEqual(TR_VPConstraint *other, TR_ValuePropagation *vp);

   virtual TR_VPConstraint *add(TR_VPConstraint *other, TR::DataType type, TR_ValuePropagation *vp);
   virtual TR_VPConstraint *subtract(TR_VPConstraint *other, TR::DataType type, TR_ValuePropagation *vp);
   // unsigned add
   //TR_VPConstraint *add(TR_VPIntConstraint *other, TR_ValuePropagation *vp, bool isUnsigned);
   // unsigned subtract
   //TR_VPConstraint *subtract(TR_VPIntConstraint *other, TR_ValuePropagation *vp, bool isUnsigned);

   virtual TR_YesNoMaybe canOverflow() {return _overflow;}
   virtual void setCanOverflow(TR_YesNoMaybe v) {_overflow = v;}

   virtual int32_t getPrecision() {return getPrecisionFromValue(TR::getMaxSigned<TR::Int32>());}

   protected:
   int32_t _low;
   TR_YesNoMaybe _overflow;

   private:
   TR_VPConstraint *getRange(int32_t, int32_t, bool, bool, TR_ValuePropagation *vp );
   };

class TR_VPIntConst : public TR_VPIntConstraint
   {
   public:
   TR_VPIntConst(int32_t v) : TR_VPIntConstraint(v) {}
   static TR_VPIntConst *create(TR_ValuePropagation *vp, int32_t v);
   static TR_VPConstraint *createExclusion(TR_ValuePropagation *vp, int32_t v);
   // unsigned createExclusion
   //static TR_VPConstraint *createExclusion(TR_ValuePropagation *vp, int32_t v);
   virtual TR_VPIntConst *asIntConst();
   virtual int32_t getHigh() {return _low;}
   virtual bool mustBeEqual(TR_VPConstraint *other, TR_ValuePropagation *vp);

   virtual void print(TR::Compilation *, TR::FILE *);
   virtual const char *name();

   virtual int32_t getPrecision() {return getPrecisionFromValue(_low);}
   };

class TR_VPIntRange : public TR_VPIntConstraint
   {
   public:
   TR_VPIntRange(int32_t low, int32_t high) : TR_VPIntConstraint(low), _high(high) {}
   //static TR_VPIntConstraint *create(TR_ValuePropagation *vp, int32_t low, int32_t high);
   static TR_VPIntConstraint *create(TR_ValuePropagation *vp, int32_t low, int32_t high, TR_YesNoMaybe canOverflow = TR_no);
   static TR_VPIntConstraint *create(TR_ValuePropagation *vp, TR::DataTypes dt, TR_YesNoMaybe isUnsigned); // Takes a TR::DataTypes instead of TR::DataType to work around ambiguous overloads
   static TR_VPIntConstraint *createWithPrecision(TR_ValuePropagation *vp, TR::DataType dt, int32_t precision, TR_YesNoMaybe isUnsigned, bool isNonNegative = false);
   virtual TR_VPIntRange *asIntRange();
   virtual int32_t getHigh() {return _high;}

   virtual void print(TR::Compilation *, TR::FILE *);
   virtual const char *name();

   virtual int32_t getPrecision() {return getPrecisionFromRange(_low, _high);}

   private:
   int32_t _high;
   };

class TR_VPLongConstraint : public TR_VPConstraint
   {
   public:
   TR_VPLongConstraint(int64_t v) : TR_VPConstraint(LongPriority) {_low = v; _overflow = TR_no;}
   virtual TR_VPLongConstraint *asLongConstraint();

   int64_t getLong() {return _low;}
   uint64_t getUnsignedLong() {return (uint64_t)_low; }
   int64_t getLow() {return _low;}
   virtual int64_t getHigh() = 0;
   virtual int64_t getLowLong();
   virtual int64_t getHighLong();


   virtual TR_VPConstraint *merge1(TR_VPConstraint *other, TR_ValuePropagation *vp);
   virtual TR_VPConstraint *intersect1(TR_VPConstraint *other, TR_ValuePropagation *vp);
   virtual bool mustBeNotEqual(TR_VPConstraint *other, TR_ValuePropagation *vp);
   virtual bool mustBeLessThan(TR_VPConstraint *other, TR_ValuePropagation *vp);
   virtual bool mustBeLessThanOrEqual(TR_VPConstraint *other, TR_ValuePropagation *vp);

   virtual TR_VPConstraint *add(TR_VPConstraint *other, TR::DataType type, TR_ValuePropagation *vp);
   virtual TR_VPConstraint *subtract(TR_VPConstraint *other, TR::DataType type, TR_ValuePropagation *vp);

   virtual TR_YesNoMaybe canOverflow() {return _overflow;}
   virtual void setCanOverflow(TR_YesNoMaybe v) {_overflow = v;}

   virtual int32_t getPrecision() {return getPrecisionFromValue(TR::getMaxSigned<TR::Int64>());}

   protected:
   int64_t _low;
   TR_YesNoMaybe _overflow;
   private:
   TR_VPConstraint *getRange(int64_t, int64_t, bool, bool, TR_ValuePropagation *vp );

   };

class TR_VPLongConst : public TR_VPLongConstraint
   {
   public:
   TR_VPLongConst(int64_t v) : TR_VPLongConstraint(v) {}
   static TR_VPLongConst *create(TR_ValuePropagation *vp, int64_t v);
   static TR_VPConstraint *createExclusion(TR_ValuePropagation *vp, int64_t v);
   virtual TR_VPLongConst *asLongConst();
   virtual int64_t getHigh() {return _low;}
   virtual bool mustBeEqual(TR_VPConstraint *other, TR_ValuePropagation *vp);

   virtual void print(TR::Compilation *, TR::FILE *);
   virtual const char *name();

   virtual int32_t getPrecision() {return getPrecisionFromValue(_low);}
   };

class TR_VPLongRange : public TR_VPLongConstraint
   {
   public:
   TR_VPLongRange(int64_t low, int64_t high) : TR_VPLongConstraint(low), _high(high) {}
   static TR_VPLongConstraint *create(TR_ValuePropagation *vp, int64_t low, int64_t high, bool powerOfTwo = false, TR_YesNoMaybe canOverflow = TR_no);
   virtual TR_VPLongRange *asLongRange();
   virtual int64_t getHigh() {return _high;}

   virtual void print(TR::Compilation *, TR::FILE *);
   virtual const char *name();

   void setIsPowerOfTwo() {_isPowerOfTwo = true;}
   bool getIsPowerOfTwo() {return _isPowerOfTwo;}

   virtual int32_t getPrecision() {return getPrecisionFromRange(_low, _high);}

   private:
   int64_t _high;
   bool    _isPowerOfTwo;
   };


class TR_VPClass : public TR_VPConstraint
   {
   public:
   TR_VPClass(TR_VPClassType *type, TR_VPClassPresence *presence, TR_VPPreexistentObject *preexistence, TR_VPArrayInfo *arrayInfo, TR_VPObjectLocation *location)
      : TR_VPConstraint(ClassPriority), _type(type), _presence(presence),
        _preexistence(preexistence), _arrayInfo(arrayInfo), _location(location)
      {}
   static TR_VPConstraint *create(TR_ValuePropagation *vp, TR_VPClassType *type, TR_VPClassPresence *presence, TR_VPPreexistentObject *preexistence, TR_VPArrayInfo *arrayInfo, TR_VPObjectLocation *location);
   virtual TR_VPClass *asClass();

   virtual TR_VPConstraint *merge1(TR_VPConstraint *other, TR_ValuePropagation *vp);
   virtual TR_VPConstraint *intersect1(TR_VPConstraint *other, TR_ValuePropagation *vp);
   void typeIntersect(TR_VPClassPresence* &presence, TR_VPClassType* &type, TR_VPConstraint *other, TR_ValuePropagation *vp);
   virtual bool mustBeEqual(TR_VPConstraint *other, TR_ValuePropagation *vp);
   virtual bool mustBeNotEqual(TR_VPConstraint *other, TR_ValuePropagation *vp);

   virtual bool isNullObject();
   virtual bool isNonNullObject();
   virtual bool isPreexistentObject();
   virtual TR_OpaqueClassBlock *getClass();
   virtual bool isFixedClass();
   virtual bool isConstString();
   virtual TR_VPClassType *getClassType();
   virtual TR_VPArrayInfo *getArrayInfo();
   virtual TR_VPClassPresence *getClassPresence();
   virtual TR_VPPreexistentObject *getPreexistence();
   virtual TR_VPObjectLocation *getObjectLocation();
   virtual TR_VPConstString *getConstString();
   virtual TR_VPKnownObject *getKnownObject();

   virtual TR_YesNoMaybe isStackObject();
   virtual TR_YesNoMaybe isHeapObject();
   virtual TR_YesNoMaybe isClassObject();
   virtual TR_YesNoMaybe isJavaLangClassObject();
   virtual TR_YesNoMaybe isJ9ClassObject();

   virtual const char *getClassSignature(int32_t &len);

   virtual void print(TR::Compilation *, TR::FILE *);
   virtual const char *name();

   private:
   TR_VPClassType         *_type;  // Conditional on presence.  "If the reference is non-null, it has this type info"
   TR_VPClassPresence     *_presence;
   TR_VPPreexistentObject *_preexistence;
   TR_VPArrayInfo         *_arrayInfo;
   TR_VPObjectLocation    *_location;
   };

class TR_VPClassType : public TR_VPConstraint
   {
   public:
   TR_VPClassType(int32_t p) : TR_VPConstraint(p) {}
   static TR_VPClassType *create(TR_ValuePropagation *vp, TR::SymbolReference *symRef, bool isFixedClass, bool isPointerToClass = false);
   static TR_VPClassType *create(TR_ValuePropagation *vp, const char *sig, int32_t len, TR_ResolvedMethod *method, bool isFixed, TR_OpaqueClassBlock *j9class = 0);
   virtual TR_VPClassType *asClassType();

   virtual const char *getClassSignature(int32_t &len) = 0;
   virtual TR_VPClassType *getClassType();
   virtual TR_VPClassType *getArrayClass(TR_ValuePropagation *vp) = 0;

   virtual bool isReferenceArray(TR::Compilation *) = 0;
   virtual bool isPrimitiveArray(TR::Compilation *) = 0;

   virtual bool isJavaLangObject(TR_ValuePropagation *vp) {return (_len == 18 && !strncmp(_sig, "Ljava/lang/Object;", 18));}

   virtual bool isCloneableOrSerializable();


   TR::DataType getPrimitiveArrayDataType();

   virtual TR_YesNoMaybe isClassObject();
   virtual TR_YesNoMaybe isJavaLangClassObject();
   virtual TR_YesNoMaybe isJ9ClassObject();
   virtual TR_YesNoMaybe isArray();

   TR_VPClassType *classTypesCompatible(TR_VPClassType * otherType, TR_ValuePropagation *vp);

   protected:
   const char *_sig;
   int32_t _len;
   };

class TR_VPResolvedClass : public TR_VPClassType
   {
   public:
   TR_VPResolvedClass(TR_OpaqueClassBlock *klass, TR::Compilation *);
   TR_VPResolvedClass(TR_OpaqueClassBlock *klass, TR::Compilation *, int32_t p);
   static TR_VPResolvedClass *create(TR_ValuePropagation *vp, TR_OpaqueClassBlock *klass);
   virtual TR_VPResolvedClass *asResolvedClass();

   virtual TR_VPConstraint *merge1(TR_VPConstraint *other, TR_ValuePropagation *vp);
   virtual TR_VPConstraint *intersect1(TR_VPConstraint *other, TR_ValuePropagation *vp);

   virtual const char *getClassSignature(int32_t &len);

   virtual TR_OpaqueClassBlock *getClass();
   virtual TR_VPClassType *getArrayClass(TR_ValuePropagation *vp);

   virtual bool isReferenceArray(TR::Compilation *);
   virtual bool isPrimitiveArray(TR::Compilation *);
   bool isJavaLangObject(TR_ValuePropagation *vp);

   virtual void print(TR::Compilation *, TR::FILE *);
   virtual const char *name();

   protected:
   TR_OpaqueClassBlock *_class;
   };

class TR_VPFixedClass : public TR_VPResolvedClass
   {
   public:
   TR_VPFixedClass(TR_OpaqueClassBlock *klass, TR::Compilation * comp, int32_t p = FixedClassPriority) : TR_VPResolvedClass(klass, comp, p) {}
   static TR_VPFixedClass *create(TR_ValuePropagation *vp, TR_OpaqueClassBlock *klass);
   virtual TR_VPFixedClass *asFixedClass();

   virtual TR_VPConstraint *merge1(TR_VPConstraint *other, TR_ValuePropagation *vp);
   virtual TR_VPConstraint *intersect1(TR_VPConstraint *other, TR_ValuePropagation *vp);

   virtual bool isFixedClass();
   virtual TR_VPClassType *getArrayClass(TR_ValuePropagation *vp);

   virtual TR_YesNoMaybe isArray();

   virtual bool hasMoreThanFixedClassInfo(){ return false; }

   virtual void print(TR::Compilation *, TR::FILE *);
   virtual const char *name();
   };

class TR_VPConstString : public TR_VPFixedClass
   {
   protected:
   TR_VPConstString(TR_OpaqueClassBlock *klass, TR::Compilation * comp, TR::SymbolReference *symRef)
      : TR_VPFixedClass(klass, comp, ConstStringPriority), _symRef(symRef) {}
   public:
   static TR_VPConstString *create(TR_ValuePropagation *vp, TR::SymbolReference *symRef);
   virtual TR_VPConstString *asConstString();
   virtual TR_VPConstString *getConstString(){ return this; }
   virtual bool isConstString();

   virtual TR_VPConstraint *merge1(TR_VPConstraint *other, TR_ValuePropagation *vp);
   virtual TR_VPConstraint *intersect1(TR_VPConstraint *other, TR_ValuePropagation *vp);

   virtual bool mustBeEqual(TR_VPConstraint *other, TR_ValuePropagation *vp);
   virtual bool mustBeNotEqual(TR_VPConstraint *other, TR_ValuePropagation *vp);

   uint16_t  charAt(int32_t index, TR::Compilation *);
   bool      getFieldByName(TR::SymbolReference *fieldRef, void* &result, TR::Compilation *);
   TR::SymbolReference* getSymRef() {return _symRef;}

   virtual bool hasMoreThanFixedClassInfo(){ return true; }

   virtual void print(TR::Compilation *, TR::FILE *);
   virtual const char *name();

   protected:
   TR::SymbolReference *_symRef;
   };

class TR_VPUnresolvedClass : public TR_VPClassType
   {
   public:
   TR_VPUnresolvedClass(const char *sig, int32_t len, TR_ResolvedMethod *method)
      : TR_VPClassType(UnresolvedClassPriority), _method(method)
      {_sig = sig; _len = len; _definiteType = false;}
   static TR_VPUnresolvedClass *create(TR_ValuePropagation *vp, const char *sig, int32_t len, TR_ResolvedMethod *method);
   virtual TR_VPUnresolvedClass *asUnresolvedClass();

   virtual TR_VPConstraint *intersect1(TR_VPConstraint *other, TR_ValuePropagation *vp);

   virtual const char *getClassSignature(int32_t &len);
   virtual TR_VPClassType *getArrayClass(TR_ValuePropagation *vp);

   TR_ResolvedMethod *getOwningMethod() { return _method; }

   virtual bool isReferenceArray(TR::Compilation *);
   virtual bool isPrimitiveArray(TR::Compilation *);

   bool isDefiniteType()  {return _definiteType;}
   void setDefiniteType(bool b) {_definiteType = b;}

   virtual void print(TR::Compilation *, TR::FILE *);
   virtual const char *name();

   TR_ResolvedMethod *getMethod() {return _method; }

   private:
   TR_ResolvedMethod *_method;
   bool _definiteType;
   };

class TR_VPClassPresence : public TR_VPConstraint
   {
   public:
   TR_VPClassPresence() : TR_VPConstraint(ClassPresencePriority) {}
   virtual TR_VPClassPresence *asClassPresence();
   virtual TR_VPClassPresence *getClassPresence();
   };

class TR_VPNullObject : public TR_VPClassPresence
   {
   public:
   static TR_VPNullObject *create(TR_ValuePropagation *vp);
   virtual TR_VPNullObject *asNullObject();

   virtual TR_VPConstraint *intersect1(TR_VPConstraint *other, TR_ValuePropagation *vp);
   virtual bool mustBeEqual(TR_VPConstraint *other, TR_ValuePropagation *vp);
   virtual bool mustBeNotEqual(TR_VPConstraint *other, TR_ValuePropagation *vp);

   virtual bool isNullObject();

   virtual void print(TR::Compilation *, TR::FILE *);
   virtual const char *name();
   };

class TR_VPNonNullObject : public TR_VPClassPresence
   {
   public:
   static TR_VPNonNullObject *create(TR_ValuePropagation *vp);
   virtual TR_VPNonNullObject *asNonNullObject();

   virtual TR_VPConstraint *intersect1(TR_VPConstraint *other, TR_ValuePropagation *vp);
   virtual bool mustBeNotEqual(TR_VPConstraint *other, TR_ValuePropagation *vp);

   virtual bool isNonNullObject();

   virtual void print(TR::Compilation *, TR::FILE *);
   virtual const char *name();
   };

class TR_VPPreexistentObject : public TR_VPConstraint
   {
   public:
   TR_VPPreexistentObject(TR_OpaqueClassBlock *c = NULL) : TR_VPConstraint(ClassPreexistencePriority) { _assumptionClass = c;}
   static TR_VPPreexistentObject *create(TR_ValuePropagation *vp, TR_OpaqueClassBlock *c = NULL);
   virtual TR_VPPreexistentObject *asPreexistentObject();

   virtual TR_VPConstraint *intersect1(TR_VPConstraint *other, TR_ValuePropagation *vp);

   virtual bool isPreexistentObject();

   virtual TR_VPPreexistentObject *getPreexistence();

   TR_OpaqueClassBlock *getAssumptionClass() { return _assumptionClass; }

   virtual void print(TR::Compilation *, TR::FILE *);
   virtual const char *name();

   TR_OpaqueClassBlock *_assumptionClass;
   };

class TR_VPArrayInfo : public TR_VPConstraint
   {
   public:
   TR_VPArrayInfo(int32_t lowBound, int32_t highBound, int32_t elementSize)
      : TR_VPConstraint(ArrayInfoPriority), _lowBound(lowBound), _highBound(highBound), _elementSize(elementSize)
      {}
   static TR_VPArrayInfo *create(TR_ValuePropagation *vp, int32_t lowBound, int32_t highBound, int32_t elementSize);
   static TR_VPArrayInfo *create(TR_ValuePropagation *vp, char *sig);
   virtual TR_VPArrayInfo *asArrayInfo();

   virtual TR_VPConstraint *merge1(TR_VPConstraint *other, TR_ValuePropagation *vp);
   virtual TR_VPConstraint *intersect1(TR_VPConstraint *other, TR_ValuePropagation *vp);
   virtual TR_VPArrayInfo *getArrayInfo();

   int32_t lowBound()  {return _lowBound;}
   int32_t highBound() {return _highBound;}
   int32_t elementSize() {return _elementSize;}

   virtual void print(TR::Compilation *, TR::FILE *);
   virtual const char *name();

   private:
   int32_t  _lowBound;
   int32_t  _highBound;
   int32_t  _elementSize;
   };

/** Constrains the referent (if any) of an address to a set of "locations".
 *
 * The atomic locations are HeapObject, StackObject, JavaLangClassObject, and
 * J9ClassObject. The sets named in TR_VPObjectLocationKind fall into the
 * following lattice structure.
 *
 * \verbatim
                              top
                (implicit in lack of constraint)
                               |
              +----------------+---------------+
              |                |               |
      NotClassObject     NotStackObject   NotHeapObject
              |     \    /          \    /     |
              |      \  /            \  /      |
              |       \/              \/       |
              |       /\              /\       |
              |      /  \            /  \      |
              |     /    \          /    \     |
          HeapObject      StackObject     ClassObject
              |                |            /      \
              |                |           /        \
              |                |  J9ClassObject   JavaLangClassObject
              |                |       |                  |
              +----------------+-------+------------------+
                               |
                            bottom
                   (either intersection failure
                    or     TR_VPUnreachablePath)
   \endverbatim
 *
 * Note that top and bottom are not represented by instances of this class.
 *
 * In general we could have other arbitrary sets of the primitive locations,
 * e.g. {StackObject, J9ClassObject}, but currently nothing should create them.
 *
 * \par Warning: interaction with TR_VPClassType
 *
 * A location constraint that requires the referent to be a representation of a
 * class (i.e. JavaLangClassObject, J9ClassObject, or ClassObject) alters the
 * interpretation of any TR_VPClassType constraint beside it in an enclosing
 * TR_VPClass. In this case the TR_VPClassType constrains the \em represented
 * class. rather than the class of which the referent is an instance.
 */
class TR_VPObjectLocation : public TR_VPConstraint
   {
   public:
   enum TR_VPObjectLocationKind
      {
      HeapObject          = 1 << 0,
      StackObject         = 1 << 1,
      JavaLangClassObject = 1 << 2,
      J9ClassObject       = 1 << 3,
      ClassObject         = JavaLangClassObject | J9ClassObject,
      NotHeapObject       = StackObject | ClassObject,
      NotStackObject      = HeapObject | ClassObject,
      NotClassObject      = HeapObject | StackObject,
      };

   static  TR_VPObjectLocation *create(TR_ValuePropagation *vp, TR_VPObjectLocationKind kind);

   virtual TR_VPObjectLocation *asObjectLocation();
   virtual TR_VPObjectLocation *getObjectLocation();
   virtual TR_VPConstraint     *intersect1(TR_VPConstraint *other, TR_ValuePropagation *vp);
   virtual TR_VPConstraint     *merge1    (TR_VPConstraint *other, TR_ValuePropagation *vp);

   virtual TR_YesNoMaybe isStackObject();
   virtual TR_YesNoMaybe isHeapObject();
   virtual TR_YesNoMaybe isClassObject();
   virtual TR_YesNoMaybe isJavaLangClassObject();
   virtual TR_YesNoMaybe isJ9ClassObject();

   virtual void print(TR::Compilation *, TR::FILE *);
   virtual const char *name();

   private:
   TR_VPObjectLocation(TR_VPObjectLocationKind kind) : TR_VPConstraint(ObjectLocationPriority), _kind(kind) {}

   TR_YesNoMaybe isWithin(TR_VPObjectLocationKind area);
   static bool isKindSubset(TR_VPObjectLocationKind x, TR_VPObjectLocationKind y)
      {
      return (x & ~y) == 0;
      }

   TR_VPObjectLocationKind _kind;
   };

class TR_VPKnownObject : public TR_VPFixedClass
   {
   // IMPORTANT NOTE: Just like with class constraints, TR_VPKnownObject tells
   // you "if this reference is non-null, it points at object X".  Consumers of
   // TR_VPKnownObject must bear in mind that NULL is always a possibility.
   //
   // This kind of conditional constraint is subtle, but we do it this way
   // because it allows us to achieve better optimization.  Usages of
   // potentially null object references are practically always preceded by
   // null checks, and in these cases, constraints that are conditional on
   // non-nullness allow us to retain information that would be lost if the
   // constraints were required to be unconditional.
   //
   // We practically always have a TR_VPClass constraint that contains both
   // Known Object and isNonNullObject info, and together, these tell us
   // reliably that a given reference definitely points at a particular object.
   //
   // (There are shades of Deductive Reaching Definitions here.  This technique
   // allows us to preserve information across control merges that would
   // otherwise be lost.)

   typedef TR_VPFixedClass Super;

   protected:
   TR_VPKnownObject(TR_OpaqueClassBlock *klass, TR::Compilation * comp, TR::KnownObjectTable::Index index, bool isJavaLangClass)
      : TR_VPFixedClass(klass, comp, KnownObjectPriority), _index(index), _isJavaLangClass(isJavaLangClass) {}

   static TR_VPKnownObject *create(TR_ValuePropagation *vp, TR::KnownObjectTable::Index index, bool isJavaLangClass);

   public:
   static TR_VPKnownObject *create(TR_ValuePropagation *vp, TR::KnownObjectTable::Index index)
      {
      return create(vp, index, false);
      }

   static TR_VPKnownObject *createForJavaLangClass(TR_ValuePropagation *vp, TR::KnownObjectTable::Index index)
      {
      return create(vp, index, true);
      }

   virtual TR_VPKnownObject *asKnownObject();
   virtual TR_VPKnownObject *getKnownObject(){ return this; }

   TR::KnownObjectTable::Index getIndex(){ return _index; }

   virtual TR_VPConstraint *merge1(TR_VPConstraint *other, TR_ValuePropagation *vp);
   virtual TR_VPConstraint *intersect1(TR_VPConstraint *other, TR_ValuePropagation *vp);

   virtual TR_YesNoMaybe isJavaLangClassObject();

   virtual bool mustBeEqual(TR_VPConstraint *other, TR_ValuePropagation *vp);
   virtual bool mustBeNotEqual(TR_VPConstraint *other, TR_ValuePropagation *vp);

   virtual bool hasMoreThanFixedClassInfo(){ return true; }

   virtual void print(TR::Compilation *, TR::FILE *);
   virtual const char *name();

   protected:
   TR::KnownObjectTable::Index _index;
   bool _isJavaLangClass;
   };


/*
class TR_VPImplementedInterface : public TR_VPConstraint
   {
   public:

   static  TR_VPImplementedInterface *create(TR_ValuePropagation *vp, char *sig, int32_t len, TR_ResolvedMethod *method);

   TR_VPImplementedInterface(char *sig, int32_t len, TR_ResolvedMethod *method)
      : _method(method)
      {_sig = sig; _len = len;}

   virtual TR_VPImplementedInterface *asImplementedInterface();

   virtual TR_VPConstraint *intersect1(TR_VPConstraint *other, TR_ValuePropagation *vp);
   virtual TR_VPConstraint *merge1    (TR_VPConstraint *other, TR_ValuePropagation *vp);

   virtual char *getInterfaceSignature(int32_t &len);

   virtual void print(TR::Compilation *, TR::FILE *);
   virtual const char *name();

   private:
   TR_ResolvedMethod *_method;
   char *_sig;
   int32_t _len;
   };
*/






class TR_VPMergedConstraints : public TR_VPConstraint
   {
   public:
   TR_VPMergedConstraints(ListElement<TR_VPConstraint> *first, TR_Memory * m)
      : TR_VPConstraint(MergedConstraintPriority),
        _type((first && first->getData()->asShortConstraint() ? TR::Int16 : ((first && first->getData()->asLongConstraint()) ? TR::Int64 : TR::Int32))),
        _constraints(m)
      {
          _constraints.setListHead(first);
      }
   static TR_VPMergedConstraints *create(TR_ValuePropagation *vp, TR_VPConstraint *first, TR_VPConstraint *second);
   static TR_VPMergedConstraints *create(TR_ValuePropagation *vp, ListElement<TR_VPConstraint> *list);
   virtual TR_VPMergedConstraints *asMergedConstraints();
   virtual TR_VPMergedConstraints *asMergedShortConstraints();
   virtual TR_VPMergedConstraints *asMergedIntConstraints();
   virtual TR_VPMergedConstraints *asMergedLongConstraints();

   virtual TR_VPConstraint *merge1(TR_VPConstraint *other, TR_ValuePropagation *vp);
   virtual TR_VPConstraint *intersect1(TR_VPConstraint *other, TR_ValuePropagation *vp);
   virtual bool mustBeNotEqual(TR_VPConstraint *other, TR_ValuePropagation *vp);
   virtual bool mustBeLessThan(TR_VPConstraint *other, TR_ValuePropagation *vp);
   virtual bool mustBeLessThanOrEqual(TR_VPConstraint *other, TR_ValuePropagation *vp);

   virtual int16_t getLowShort();
   virtual int16_t getHighShort();
   virtual int32_t getLowInt();
   virtual int32_t getHighInt();
   virtual int64_t getLowLong();
   virtual int64_t getHighLong();

   virtual uint16_t getUnsignedLowShort();
   virtual uint16_t getUnsignedHighShort();
   virtual uint32_t getUnsignedLowInt();
   virtual uint32_t getUnsignedHighInt();
   virtual uint64_t getUnsignedLowLong();
   virtual uint64_t getUnsignedHighLong();


   TR_ScratchList<TR_VPConstraint> *getList() {return &_constraints;}
   TR::DataType                     getType() {return _type;}

   virtual void print(TR::Compilation *, TR::FILE *);
   virtual const char *name();

   private:
   TR_VPConstraint *shortMerge(TR_VPConstraint * otherCur, ListElement<TR_VPConstraint> * otherNext, TR_ValuePropagation * vp);
   TR_VPConstraint *shortIntersect(TR_VPConstraint *otherCur, ListElement<TR_VPConstraint> * otherNext, TR_ValuePropagation * vp);

   // unsigned intMerge
   //   TR_VPConstraint *intMerge(TR_VPIntConstraint *otherCur, ListElement<TR_VPConstraint> *otherNext, TR_ValuePropagation *vp, bool isUnsigned);
   TR_VPConstraint *intMerge(TR_VPConstraint *otherCur, ListElement<TR_VPConstraint> *otherNext, TR_ValuePropagation *vp);
   TR_VPConstraint *intIntersect(TR_VPConstraint *otherCur, ListElement<TR_VPConstraint> *otherNext, TR_ValuePropagation *vp);

   // unsigned intIntersect
   //TR_VPConstraint *intIntersect(TR_VPIntConstraint *otherCur, ListElement<TR_VPConstraint> *otherNext, TR_ValuePropagation *vp, bool isUnsigned);
   TR_VPConstraint *longMerge(TR_VPConstraint *otherCur, ListElement<TR_VPConstraint> *otherNext, TR_ValuePropagation *vp);
   TR_VPConstraint *longIntersect(TR_VPConstraint *otherCur, ListElement<TR_VPConstraint> *otherNext, TR_ValuePropagation *vp);

   TR_ScratchList<TR_VPConstraint> _constraints;
   TR::DataType                    _type;
   };

class TR_VPUnreachablePath : public TR_VPConstraint
   {
   public:
   TR_VPUnreachablePath() : TR_VPConstraint(0) {}
   static TR_VPUnreachablePath *create(TR_ValuePropagation *vp);
   virtual TR_VPUnreachablePath *asUnreachablePath();

   virtual void print(TR::Compilation *, TR::FILE *);
   virtual const char *name();
   };

// breaks handcuff rule
class TR_VPSync : public TR_VPConstraint
   {
   public:
   TR_VPSync(TR_YesNoMaybe v) : TR_VPConstraint(0), _syncEmitted(v) {}
   static TR_VPSync *create(TR_ValuePropagation *vp, TR_YesNoMaybe v);
   virtual TR_VPSync *asVPSync();
   virtual TR_VPConstraint *intersect1(TR_VPConstraint *other, TR_ValuePropagation *vp);
   virtual TR_VPConstraint *merge1(TR_VPConstraint *other, TR_ValuePropagation *vp);
   TR_YesNoMaybe syncEmitted() { return _syncEmitted; }
   ////void setSyncEmitted(TR_YesNoMaybe v) { _syncEmitted = v; }


   virtual void print(TR::Compilation *, TR::FILE *);
   virtual const char *name();

   private:
   TR_YesNoMaybe _syncEmitted;
   };

class TR_VPRelation : public TR_VPConstraint
   {
   public:

   TR_VPRelation(int32_t incr, int32_t p)
      : TR_VPConstraint(p), _increment(incr)
      {}
   virtual TR_VPRelation *asRelation();
   int32_t      increment() {return _increment;}

   virtual TR_VPConstraint *propagateAbsoluteConstraint(
                               TR_VPConstraint *constraint,
                               int32_t relative,
                               TR_ValuePropagation *vp);
   virtual TR_VPConstraint*propagateRelativeConstraint(
                               TR_VPRelation *other,
                               int32_t relative, int32_t otherRelative,
                               TR_ValuePropagation *vp) = 0;

   virtual TR_VPRelation *getComplement(TR_ValuePropagation *vp) = 0;

   protected:
   int32_t      _increment;
   };

class TR_VPLessThanOrEqual : public TR_VPRelation
   {
   public:

   TR_VPLessThanOrEqual(int32_t incr)
      : TR_VPRelation(incr, LessThanOrEqualPriority)
      {}
   virtual TR_VPLessThanOrEqual *asLessThanOrEqual();
   static TR_VPLessThanOrEqual *create(TR_ValuePropagation *vp, int32_t incr);
   virtual TR_VPConstraint *merge1(TR_VPConstraint *other, TR_ValuePropagation *vp);
   virtual TR_VPConstraint *intersect1(TR_VPConstraint *other, TR_ValuePropagation *vp);

   virtual bool mustBeNotEqual();
   virtual bool mustBeLessThan();
   virtual bool mustBeLessThanOrEqual();

   virtual TR_VPConstraint *propagateAbsoluteConstraint(
                               TR_VPConstraint *constraint,
                               int32_t relative,
                               TR_ValuePropagation *vp);
   virtual TR_VPConstraint*propagateRelativeConstraint(
                               TR_VPRelation *other,
                               int32_t relative, int32_t otherRelative,
                               TR_ValuePropagation *vp);

   virtual TR_VPRelation *getComplement(TR_ValuePropagation *vp);

   virtual void print(TR::Compilation *, TR::FILE *);
   virtual void print(TR::Compilation *, TR::FILE *, int32_t relative);
   virtual const char *name();
   };

class TR_VPGreaterThanOrEqual : public TR_VPRelation
   {
   public:

   TR_VPGreaterThanOrEqual(int32_t incr)
      : TR_VPRelation(incr, GreaterThanOrEqualPriority)
      {}
   virtual TR_VPGreaterThanOrEqual *asGreaterThanOrEqual();
   static TR_VPGreaterThanOrEqual *create(TR_ValuePropagation *vp, int32_t incr);
   virtual TR_VPConstraint *merge1(TR_VPConstraint *other, TR_ValuePropagation *vp);
   virtual TR_VPConstraint *intersect1(TR_VPConstraint *other, TR_ValuePropagation *vp);

   virtual bool mustBeNotEqual();
   virtual bool mustBeGreaterThan();
   virtual bool mustBeGreaterThanOrEqual();

   virtual TR_VPConstraint *propagateAbsoluteConstraint(
                               TR_VPConstraint *constraint,
                               int32_t relative,
                               TR_ValuePropagation *vp);
   virtual TR_VPConstraint*propagateRelativeConstraint(
                               TR_VPRelation *other,
                               int32_t relative, int32_t otherRelative,
                               TR_ValuePropagation *vp);

   virtual TR_VPRelation *getComplement(TR_ValuePropagation *vp);

   virtual void print(TR::Compilation *, TR::FILE *);
   virtual void print(TR::Compilation *, TR::FILE *, int32_t relative);
   virtual const char *name();
   };

class TR_VPEqual : public TR_VPRelation
   {
   public:

   TR_VPEqual(int32_t incr)
      : TR_VPRelation(incr, EqualPriority)
      {}
   virtual TR_VPEqual *asEqual();
   static TR_VPEqual *create(TR_ValuePropagation *vp, int32_t incr);
   virtual TR_VPConstraint *merge1(TR_VPConstraint *other, TR_ValuePropagation *vp);
   virtual TR_VPConstraint *intersect1(TR_VPConstraint *other, TR_ValuePropagation *vp);

   virtual bool mustBeEqual();
   virtual bool mustBeNotEqual();
   virtual bool mustBeLessThan();
   virtual bool mustBeLessThanOrEqual();
   virtual bool mustBeGreaterThan();
   virtual bool mustBeGreaterThanOrEqual();

   virtual TR_VPConstraint *propagateAbsoluteConstraint(
                               TR_VPConstraint *constraint,
                               int32_t relative,
                               TR_ValuePropagation *vp);
   virtual TR_VPConstraint*propagateRelativeConstraint(
                               TR_VPRelation *other,
                               int32_t relative, int32_t otherRelative,
                               TR_ValuePropagation *vp);

   virtual TR_VPRelation *getComplement(TR_ValuePropagation *vp);

   virtual void print(TR::Compilation *, TR::FILE *);
   virtual void print(TR::Compilation *, TR::FILE *, int32_t relative);
   virtual const char *name();
   };

class TR_VPNotEqual : public TR_VPRelation
   {
   public:

   TR_VPNotEqual(int32_t incr)
      : TR_VPRelation(incr, NotEqualPriority)
      {}
   virtual TR_VPNotEqual *asNotEqual();
   static TR_VPNotEqual *create(TR_ValuePropagation *vp, int32_t incr);
   virtual TR_VPConstraint *intersect1(TR_VPConstraint *other, TR_ValuePropagation *vp);

   virtual bool mustBeNotEqual();

   virtual TR_VPConstraint *propagateAbsoluteConstraint(
                               TR_VPConstraint *constraint,
                               int32_t relative,
                               TR_ValuePropagation *vp);
   virtual TR_VPConstraint*propagateRelativeConstraint(
                               TR_VPRelation *other,
                               int32_t relative, int32_t otherRelative,
                               TR_ValuePropagation *vp);

   virtual TR_VPRelation *getComplement(TR_ValuePropagation *vp);

   virtual void print(TR::Compilation *, TR::FILE *);
   virtual void print(TR::Compilation *, TR::FILE *, int32_t relative);
   virtual const char *name();
   };

#endif
