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

#include "optimizer/VPConstraint.hpp"

#include <ctype.h>                              // for isdigit
#include <stddef.h>                             // for size_t
#include "codegen/FrontEnd.hpp"                 // for TR::IO::fprintf, etc
#include "compile/Compilation.hpp"              // for Compilation
#include "compile/ResolvedMethod.hpp"           // for TR_ResolvedMethod
#include "control/Options.hpp"
#include "control/Options_inlines.hpp"
#include "env/CompilerEnv.hpp"
#include "env/IO.hpp"
#include "env/KnownObjectTable.hpp"         // for KnownObjectTable, etc
#include "env/ObjectModel.hpp"                  // for ObjectModel
#include "env/PersistentInfo.hpp"               // for PersistentInfo
#include "env/jittypes.h"
#ifdef J9_PROJECT_SPECIFIC
#include "env/VMAccessCriticalSection.hpp"      // for VMAccessCriticalSection
#endif
#include "il/DataTypes.hpp"                     // for getMaxSigned, etc
#include "il/ILOps.hpp"                         // for ILOpCode
#include "il/Node.hpp"                          // for Node, etc
#include "il/Symbol.hpp"                        // for Symbol
#include "il/SymbolReference.hpp"               // for SymbolReference, etc
#include "il/symbol/StaticSymbol.hpp"           // for StaticSymbol
#include "ilgen/IlGenRequest.hpp"               // for IlGenRequest
#include "ilgen/IlGeneratorMethodDetails.hpp"
#include "optimizer/Optimization_inlines.hpp"
#include "optimizer/ValuePropagation.hpp"       // for TR_ValuePropagation, etc

#ifdef J9_PROJECT_SPECIFIC
#include "env/PersistentCHTable.hpp"            // for TR_PersistentCHTable
#include "runtime/RuntimeAssumptions.hpp"
#include "env/VMJ9.h"
#endif

// ***************************************************************************
//
// Methods of Value Propagation Constraints
//
// ***************************************************************************
TR_VPShortConstraint   *TR_VPConstraint::asShortConstraint()   { return NULL; }
TR_VPShortConst        *TR_VPConstraint::asShortConst()        { return NULL; }
TR_VPShortRange        *TR_VPConstraint::asShortRange()        { return NULL; }
TR_VPIntConstraint     *TR_VPConstraint::asIntConstraint()     { return NULL; }
TR_VPIntConst          *TR_VPConstraint::asIntConst()          { return NULL; }
TR_VPIntRange          *TR_VPConstraint::asIntRange()          { return NULL; }
TR_VPLongConstraint    *TR_VPConstraint::asLongConstraint()    { return NULL; }
TR_VPLongConst         *TR_VPConstraint::asLongConst()         { return NULL; }
TR_VPLongRange         *TR_VPConstraint::asLongRange()         { return NULL; }
TR_VP_BCDValue         *TR_VPConstraint::asBCDValue()          { return NULL; }
TR_VP_BCDSign          *TR_VPConstraint::asBCDSign()           { return NULL; }
TR_VPClass             *TR_VPConstraint::asClass()             { return NULL; }
TR_VPClassType         *TR_VPConstraint::asClassType()         { return NULL; }
TR_VPResolvedClass     *TR_VPConstraint::asResolvedClass()     { return NULL; }
TR_VPFixedClass        *TR_VPConstraint::asFixedClass()        { return NULL; }
TR_VPConstString       *TR_VPConstraint::asConstString()       { return NULL; }
TR_VPKnownObject       *TR_VPConstraint::asKnownObject()       { return NULL; }
TR_VPUnresolvedClass   *TR_VPConstraint::asUnresolvedClass()   { return NULL; }
TR_VPClassPresence     *TR_VPConstraint::asClassPresence()     { return NULL; }
TR_VPNullObject        *TR_VPConstraint::asNullObject()        { return NULL; }
TR_VPNonNullObject     *TR_VPConstraint::asNonNullObject()     { return NULL; }
TR_VPPreexistentObject *TR_VPConstraint::asPreexistentObject() { return NULL; }
TR_VPArrayInfo         *TR_VPConstraint::asArrayInfo()         { return NULL; }
TR_VPObjectLocation    *TR_VPConstraint::asObjectLocation()    { return NULL; }
TR_VPMergedConstraints *TR_VPConstraint::asMergedConstraints() { return NULL; }
TR_VPMergedConstraints *TR_VPConstraint::asMergedShortConstraints() {return NULL;}
TR_VPMergedConstraints *TR_VPConstraint::asMergedIntConstraints() { return NULL; }
TR_VPMergedConstraints *TR_VPConstraint::asMergedLongConstraints(){ return NULL; }
TR_VPUnreachablePath   *TR_VPConstraint::asUnreachablePath()   { return NULL; }
TR_VPSync              *TR_VPConstraint::asVPSync()            { return NULL; }
TR_VPRelation          *TR_VPConstraint::asRelation()          { return NULL; }
TR_VPLessThanOrEqual   *TR_VPConstraint::asLessThanOrEqual()   { return NULL; }
TR_VPGreaterThanOrEqual*TR_VPConstraint::asGreaterThanOrEqual(){ return NULL; }
TR_VPEqual             *TR_VPConstraint::asEqual()             { return NULL; }
TR_VPNotEqual          *TR_VPConstraint::asNotEqual()          { return NULL; }
TR_VPShortConstraint   *TR_VPShortConstraint::asShortConstraint()     { return this;}
TR_VPShortConst        *TR_VPShortConst::asShortConst()          { return this;}
TR_VPShortRange        *TR_VPShortRange::asShortRange()          { return this;}
TR_VPIntConstraint     *TR_VPIntConstraint::asIntConstraint()         { return this; }
TR_VPIntConst          *TR_VPIntConst::asIntConst()                   { return this; }
TR_VPIntRange          *TR_VPIntRange::asIntRange()                   { return this; }
TR_VPLongConstraint    *TR_VPLongConstraint::asLongConstraint()       { return this; }
TR_VPLongConst         *TR_VPLongConst::asLongConst()                 { return this; }
TR_VPLongRange         *TR_VPLongRange::asLongRange()                 { return this; }
TR_VPClass             *TR_VPClass::asClass()                         { return this; }
TR_VPClassType         *TR_VPClassType::asClassType()                 { return this; }
TR_VPResolvedClass     *TR_VPResolvedClass::asResolvedClass()         { return this; }
TR_VPFixedClass        *TR_VPFixedClass::asFixedClass()               { return this; }
TR_VPConstString       *TR_VPConstString::asConstString()             { return this; }
TR_VPKnownObject       *TR_VPKnownObject::asKnownObject()             { return this; }
TR_VPUnresolvedClass   *TR_VPUnresolvedClass::asUnresolvedClass()     { return this; }
TR_VPClassPresence     *TR_VPClassPresence::asClassPresence()         { return this; }
TR_VPNullObject        *TR_VPNullObject::asNullObject()               { return this; }
TR_VPNonNullObject     *TR_VPNonNullObject::asNonNullObject()         { return this; }
TR_VPPreexistentObject *TR_VPPreexistentObject::asPreexistentObject() { return this; }
TR_VPArrayInfo         *TR_VPArrayInfo::asArrayInfo()                 { return this; }
TR_VPObjectLocation    *TR_VPObjectLocation::asObjectLocation()       { return this; }
TR_VPMergedConstraints *TR_VPMergedConstraints::asMergedConstraints() { return this; }
TR_VPMergedConstraints *TR_VPMergedConstraints::asMergedShortConstraints() { return (_type.isInt16()) ? this : NULL; }
TR_VPMergedConstraints *TR_VPMergedConstraints::asMergedIntConstraints() { return (_type.isInt32()) ? this : NULL; }
TR_VPMergedConstraints *TR_VPMergedConstraints::asMergedLongConstraints() { return (_type.isInt64()) ? this : NULL; }
TR_VPUnreachablePath   *TR_VPUnreachablePath::asUnreachablePath()     { return this; }
TR_VPSync              *TR_VPSync::asVPSync()                         { return this; }
TR_VPRelation          *TR_VPRelation::asRelation()                   { return this; }
TR_VPLessThanOrEqual   *TR_VPLessThanOrEqual::asLessThanOrEqual()     { return this; }
TR_VPGreaterThanOrEqual*TR_VPGreaterThanOrEqual::asGreaterThanOrEqual(){ return this; }
TR_VPEqual             *TR_VPEqual::asEqual()                         { return this; }
TR_VPNotEqual          *TR_VPNotEqual::asNotEqual()                   { return this; }

int16_t TR_VPConstraint::getLowShort()
   {
   if (isUnsigned())
       return TR::getMinUnsigned<TR::Int16>();
   return TR::getMinSigned<TR::Int16>();
   }

int16_t TR_VPConstraint::getHighShort()
   {
   if (isUnsigned())
       return TR::getMaxUnsigned<TR::Int16>();
   return TR::getMaxSigned<TR::Int16>();
   }

int32_t TR_VPConstraint::getLowInt()
   {
   if (isUnsigned())
      return TR::getMinUnsigned<TR::Int32>();
   return TR::getMinSigned<TR::Int32>();
   }

int32_t TR_VPConstraint::getHighInt()
   {
   if (isUnsigned())
      return TR::getMaxUnsigned<TR::Int32>();
   return TR::getMaxSigned<TR::Int32>();
   }

int64_t TR_VPConstraint::getLowLong()
   {
   return TR::getMinSigned<TR::Int64>();
   }

int64_t TR_VPConstraint::getHighLong()
   {
   return TR::getMaxSigned<TR::Int64>();
   }


uint16_t TR_VPConstraint::getUnsignedLowShort()
   {
   if ( (getLowShort() ^ getHighShort()) >= 0)       // if both numbers are the same sign, return the small value
      return (uint16_t)getLowShort();

   return TR::getMinUnsigned<TR::Int16>();
   }

uint16_t TR_VPConstraint::getUnsignedHighShort()
   {
   if ( (getLowShort() ^ getHighShort()) >= 0)       // if both numbers have the same sign, getHigh is the high value
      return (uint16_t)getHighShort();

   return TR::getMaxUnsigned<TR::Int16>();
   }

uint32_t TR_VPConstraint::getUnsignedLowInt()
   {
   if ( (getLowInt() ^ getHighInt()) >= 0)       // if both numbers are the same sign, return the small value
      return (uint32_t)getLowInt();

   return TR::getMinUnsigned<TR::Int32>();
   }

uint32_t TR_VPConstraint::getUnsignedHighInt()
   {
   if ( (getLowInt() ^ getHighInt()) >= 0)       // if both numbers have the same sign, getHigh is the high value
      return (uint32_t)getHighInt();

   return TR::getMaxUnsigned<TR::Int32>();
   }

uint64_t TR_VPConstraint::getUnsignedLowLong()
   {
   if ( (getLowLong() ^ getHighLong()) >= 0)       // if both numbers are the same sign, return the small value
      return (uint64_t)getLowLong();

   return TR::getMinUnsigned<TR::Int64>();
   }

uint64_t TR_VPConstraint::getUnsignedHighLong()
   {
   if ( (getLowLong() ^ getHighLong()) >= 0)       // if both numbers have the same sign, getHigh is the high value
      return (uint64_t)getHighLong();

   return TR::getMaxUnsigned<TR::Int64>();    // consumer beware! You don't want to assign this to a int64_t
   }


bool TR_VPConstraint::isNullObject()
   {
   return false;
   }

bool TR_VPConstraint::isNonNullObject()
   {
   return false;
   }

bool TR_VPConstraint::isPreexistentObject()
   {
   return false;
   }

TR_OpaqueClassBlock *TR_VPConstraint::getClass()
   {
   return NULL;
   }

bool TR_VPConstraint::isFixedClass()
   {
   return false;
   }

bool TR_VPConstraint::isConstString()
   {
   return false;
   }

// VP_SPECIALKLASS is created so that any type that
// intersects with it results in a null type. It is
// not meant to be propagated during the analysis
//
bool TR_VPConstraint::isSpecialClass(uintptrj_t klass)
   {
   if (klass == VP_SPECIALKLASS)
      return true;
   return false;
   }

TR_YesNoMaybe TR_VPConstraint::canOverflow()
   {
   return TR_maybe;
   }

TR_VPClassType *TR_VPConstraint::getClassType()
   {
   return NULL;
   }

TR_VPClassPresence *TR_VPConstraint::getClassPresence()
   {
   return NULL;
   }

TR_VPArrayInfo *TR_VPConstraint::getArrayInfo()
   {
   return NULL;
   }

TR_VPPreexistentObject *TR_VPConstraint::getPreexistence()
   {
   return NULL;
   }

TR_VPObjectLocation *TR_VPConstraint::getObjectLocation()
   {
   return NULL;
   }

TR_VPKnownObject *TR_VPConstraint::getKnownObject()
   {
   return NULL;
   }

TR_VPConstString *TR_VPConstraint::getConstString()
   {
   return NULL;
   }

const char *TR_VPConstraint::getClassSignature(int32_t &len)
   {
   return NULL;
   }

TR_YesNoMaybe TR_VPConstraint::isStackObject()
   {
   return TR_maybe;
   }

TR_YesNoMaybe TR_VPConstraint::isHeapObject()
   {
   return TR_maybe;
   }

TR_YesNoMaybe TR_VPConstraint::isClassObject()
   {
   return TR_maybe;
   }

TR_YesNoMaybe TR_VPConstraint::isJavaLangClassObject()
   {
   return TR_maybe;
   }

TR_YesNoMaybe TR_VPConstraint::isJ9ClassObject()
   {
   return TR_maybe;
   }

int16_t TR_VPShortConstraint::getLowShort()
   {
   return getLow();
   }

int16_t TR_VPShortConstraint::getHighShort()
   {
   return getHigh();
   }

int32_t TR_VPIntConstraint::getLowInt()
   {
   return getLow();
   }

int32_t TR_VPIntConstraint::getHighInt()
   {
   return getHigh();
   }


int64_t TR_VPLongConstraint::getLowLong()
   {
   return getLow();
   }

int64_t TR_VPLongConstraint::getHighLong()
   {
   return getHigh();
   }

int16_t TR_VPMergedConstraints::getLowShort()
   {
   return _constraints.getListHead()->getData()->getLowShort();
   }

int16_t TR_VPMergedConstraints::getHighShort()
   {
   return _constraints.getListHead()->getData()->getHighShort();
   }

int32_t TR_VPMergedConstraints::getLowInt()
   {
   return _constraints.getListHead()->getData()->getLowInt();
   }

int32_t TR_VPMergedConstraints::getHighInt()
   {
   return _constraints.getLastElement()->getData()->getHighInt();
   }

int64_t TR_VPMergedConstraints::getLowLong()
   {
   return _constraints.getListHead()->getData()->getLowLong();
   }

int64_t TR_VPMergedConstraints::getHighLong()
   {
   return _constraints.getLastElement()->getData()->getHighLong();
   }

uint16_t TR_VPMergedConstraints::getUnsignedLowShort()
   {
   return _constraints.getListHead()->getData()->getUnsignedLowShort();
   }

uint16_t TR_VPMergedConstraints::getUnsignedHighShort()
   {
   return _constraints.getListHead()->getData()->getUnsignedHighShort();
   }

uint32_t TR_VPMergedConstraints::getUnsignedLowInt()
   {
   return _constraints.getListHead()->getData()->getUnsignedLowInt();
   }

uint32_t TR_VPMergedConstraints::getUnsignedHighInt()
   {
   return _constraints.getLastElement()->getData()->getHighInt();
   }

uint64_t TR_VPMergedConstraints::getUnsignedLowLong()
   {
   return _constraints.getListHead()->getData()->getUnsignedLowLong();
   }

uint64_t TR_VPMergedConstraints::getUnsignedHighLong()
   {
   return _constraints.getLastElement()->getData()->getUnsignedHighLong();
   }

const char *TR_VPClass::getClassSignature(int32_t &len)
   {
   if (_type)
      return _type->getClassSignature(len);
   return NULL;
   }

bool TR_VPClass::isNullObject()
   {
   if (_presence)
      return _presence->isNullObject();
   return false;
   }

bool TR_VPClass::isNonNullObject()
   {
   if (_presence)
      return _presence->isNonNullObject();
   return false;
   }

bool TR_VPClass::isPreexistentObject()
   {
   if (_preexistence)
      return _preexistence->isPreexistentObject();
   return false;
   }

TR_OpaqueClassBlock *TR_VPClass::getClass()
   {
   if (_type)
      return _type->getClass();
   return NULL;
   }

bool TR_VPClass::isFixedClass()
   {
   if (_type)
      return _type->isFixedClass();
   return false;
   }

bool TR_VPClass::isConstString()
   {
   if (_type)
      return _type->isConstString();
   return false;
   }

TR_YesNoMaybe TR_VPClass::isStackObject()
   {
   if (_location)
      return _location->isStackObject();
   return TR_maybe;
   }

TR_YesNoMaybe TR_VPClass::isHeapObject()
   {
   if (_location)
      return _location->isHeapObject();
   return TR_maybe;
   }

TR_YesNoMaybe TR_VPClass::isClassObject()
   {
   if (_location && _location->isClassObject() != TR_maybe)
      return _location->isClassObject();
   if (_type && _type->isJavaLangClassObject() != TR_maybe)
      return _type->isJavaLangClassObject();
   return TR_maybe;
   }

TR_YesNoMaybe TR_VPClass::isJavaLangClassObject()
   {
   if (_location && _location->isJavaLangClassObject() != TR_maybe)
      return _location->isJavaLangClassObject();
   if (_type && _type->isJavaLangClassObject() != TR_maybe)
      return _type->isJavaLangClassObject();
   return TR_maybe;
   }

TR_YesNoMaybe TR_VPClass::isJ9ClassObject()
   {
   if (_location != NULL)
      return _location->isJ9ClassObject();
   else if (_type != NULL)
      return TR_no; // in absence of _location, _type means an instance
   else
      return TR_maybe;
   }

TR_VPClassType *TR_VPClass::getClassType()
   {
   return _type;
   }

TR_VPArrayInfo *TR_VPClass::getArrayInfo()
   {
   return _arrayInfo;
   }

TR_VPClassPresence *TR_VPClass::getClassPresence()
   {
   return _presence;
   }

TR_VPPreexistentObject *TR_VPClass::getPreexistence()
   {
   return _preexistence;
   }

TR_VPObjectLocation *TR_VPClass::getObjectLocation()
   {
   return _location;
   }

TR_VPKnownObject *TR_VPClass::getKnownObject()
   {
   return _type? _type->asKnownObject() : NULL;
   }

TR_VPConstString *TR_VPClass::getConstString()
   {
   return _type? _type->asConstString() : NULL;
   }

TR_VPClassType *TR_VPClassType::getClassType()
   {
   return this;
   }

TR_VPClassPresence *TR_VPClassPresence::getClassPresence()
   {
   return this;
   }

TR_VPPreexistentObject *TR_VPPreexistentObject::getPreexistence()
   {
   return this;
   }

TR_VPObjectLocation *TR_VPObjectLocation::getObjectLocation()
   {
   return this;
   }

TR::DataTypes TR_VPClassType::getPrimitiveArrayDataType()
   {
   if (_sig[0] != '[')
      return TR::NoType;
   switch (_sig[1])
      {
      case 'Z':
      case 'B': return TR::Int8;
      case 'C':
      case 'S': return TR::Int16;
      case 'I': return TR::Int32;
      case 'J': return TR::Int64;
      case 'F': return TR::Float;
      case 'D': return TR::Double;
      }
   return TR::NoType;
   }

TR_YesNoMaybe TR_VPClassType::isClassObject()
   {
   return isJavaLangClassObject();
   }

TR_YesNoMaybe TR_VPClassType::isJavaLangClassObject()
   {
   // TR_yes will cause callers to misinterpret
   // this->getClassType()==this as the represented class, i.e.
   // it would make this look just like Class.class.
   if (_len == 17 && strncmp(_sig, "Ljava/lang/Class;", 17) == 0)
      return TR_maybe;
   if ((_len == 18 && strncmp(_sig, "Ljava/lang/Object;", 18) == 0) ||
         (_len == 22 && strncmp(_sig, "Ljava/io/Serializable;", 22) == 0) ||
         (_len == 36 && strncmp(_sig, "Ljava/lang/reflect/AnnotatedElement;", 36) == 0) ||
         (_len == 38 && strncmp(_sig, "Ljava/lang/reflect/GenericDeclaration;", 38) == 0) ||
         (_len == 24 && strncmp(_sig, "Ljava/lang/reflect/Type;", 24) == 0))
      return TR_maybe;
   return TR_no; // java.lang.Class is final and is the direct subclass of Object.
   }

TR_YesNoMaybe TR_VPClassType::isJ9ClassObject()
   {
   return TR_no;
   }

TR_YesNoMaybe TR_VPKnownObject::isJavaLangClassObject()
   {
   // The class held in the base TR_VPFixedClass is usually the class to which
   // the instance belongs, but if that would be java/lang/Class, then it's
   // instead the represented class.
   return _isJavaLangClass ? TR_yes : TR_no;
   }

TR_YesNoMaybe TR_VPClassType::isArray()
   {
   if (_sig[0] == '[')
      return TR_yes;
   if ((strncmp(_sig, "Ljava/lang/Object;", 18) == 0) ||
        isCloneableOrSerializable())
      return TR_maybe;
   return TR_no; // all arrays are direct subclasses of Object or other arrays (both cases covered above)
   }

TR_VPResolvedClass::TR_VPResolvedClass(TR_OpaqueClassBlock *klass, TR::Compilation * comp)
   : TR_VPClassType(ResolvedClassPriority), _class(klass)
   {
   if (TR_VPConstraint::isSpecialClass((uintptrj_t)klass))
      { _len = 0; _sig = 0; }
   else
      _sig = TR::Compiler->cls.classSignature_DEPRECATED(comp, klass, _len, comp->trMemory());
   }

TR_VPResolvedClass::TR_VPResolvedClass(TR_OpaqueClassBlock *klass, TR::Compilation * comp, int32_t p)
   : TR_VPClassType(p), _class(klass)
   {
   if (TR_VPConstraint::isSpecialClass((uintptrj_t)klass))
      { _len = 0; _sig = 0; }
   else
      _sig = TR::Compiler->cls.classSignature_DEPRECATED(comp, klass, _len, comp->trMemory());
   }

TR_OpaqueClassBlock *TR_VPResolvedClass::getClass()
   {
   return _class;
   }

const char *TR_VPResolvedClass::getClassSignature(int32_t &len)
   {
   len = _len;
   return _sig;
   }

TR_VPClassType *TR_VPResolvedClass::getArrayClass(TR_ValuePropagation *vp)
   {
   TR_OpaqueClassBlock *arrayClass = vp->fe()->getArrayClassFromComponentClass(getClass());
   if (arrayClass)
      return TR_VPResolvedClass::create(vp, arrayClass);

   // TODO - when getArrayClassFromComponentClass is fixed up to always return
   // the array class, remove the above "if" and the following code.
   //
   char *arraySig = (char *)vp->trMemory()->allocateStackMemory(_len+2);
   arraySig[0] = '[';
   arraySig[_len+1] = 0;
   memcpy(arraySig+1, _sig, _len);
   return TR_VPUnresolvedClass::create(vp, arraySig, _len+1, vp->comp()->getCurrentMethod());
   }

bool TR_VPResolvedClass::isReferenceArray(TR::Compilation *comp)
   {
   return TR::Compiler->cls.isReferenceArray(comp, _class);
   }

bool TR_VPResolvedClass::isPrimitiveArray(TR::Compilation *comp)
   {
   return TR::Compiler->cls.isPrimitiveArray(comp, _class);
   }

bool TR_VPResolvedClass::isJavaLangObject(TR_ValuePropagation *vp)
   {
   void *javaLangObject = vp->comp()->getObjectClassPointer();
   if (javaLangObject)
      return javaLangObject == _class;

   return (_len == 18 && !strncmp(_sig, "Ljava/lang/Object;", 18));
   }

bool TR_VPClassType::isCloneableOrSerializable()
   {
   if (_len == 21 && !strncmp(_sig, "Ljava/lang/Cloneable;", 21) )
      return true;
   if (_len == 22 && !strncmp(_sig, "Ljava/io/Serializable;", 22) )
      return true;
   return false;
   }

bool TR_VPFixedClass::isFixedClass()
   {
   return true;
   }

bool TR_VPConstString::isConstString()
   {
   return true;
   }

TR_VPClassType *TR_VPFixedClass::getArrayClass(TR_ValuePropagation *vp)
   {
   TR_OpaqueClassBlock *arrayClass = vp->fe()->getArrayClassFromComponentClass(getClass());
   if (arrayClass)
      return TR_VPFixedClass::create(vp, arrayClass);

   // TODO - when getArrayClassFromComponentClass is fixed up to always return
   // the array class, remove the above "if" and the following code.
   //
   char *arraySig = (char *)vp->trMemory()->allocateStackMemory(_len+2);
   arraySig[0] = '[';
   arraySig[_len+1] = 0;
   memcpy(arraySig+1, _sig, _len);
   return TR_VPUnresolvedClass::create(vp, arraySig, _len+1, vp->comp()->getCurrentMethod());
   }

TR_YesNoMaybe TR_VPFixedClass::isArray()
   {
   if (*_sig == '[')
      return TR_yes;
   return TR_no;
   }

const char *TR_VPUnresolvedClass::getClassSignature(int32_t &len)
   {
   len = _len;
   return _sig;
   }

TR_VPClassType *TR_VPUnresolvedClass::getArrayClass(TR_ValuePropagation *vp)
   {
   char *arraySig = (char *)vp->trMemory()->allocateStackMemory(_len+2);
   arraySig[0] = '[';
   arraySig[_len+1] = 0;
   memcpy(arraySig+1, _sig, _len);
   return TR_VPUnresolvedClass::create(vp, arraySig, _len+1, _method);
   }

bool TR_VPUnresolvedClass::isReferenceArray(TR::Compilation *comp)
   {
   return _sig[0] == '[' && (_sig[1] == '[' || _sig[1] == 'L');
   }

bool TR_VPUnresolvedClass::isPrimitiveArray(TR::Compilation *comp)
   {
   return _sig[0] == '[' && _sig[1] != '[' && _sig[1] != 'L';
   }

bool TR_VPNullObject::isNullObject()
   {
   return true;
   }

bool TR_VPNonNullObject::isNonNullObject()
   {
   return true;
   }

bool TR_VPPreexistentObject::isPreexistentObject()
   {
   return true;
   }

TR_VPArrayInfo *TR_VPArrayInfo::getArrayInfo()
   {
   return this;
   }

TR_YesNoMaybe TR_VPObjectLocation::isWithin(TR_VPObjectLocationKind area)
   {
   if (isKindSubset(_kind, area))
      return TR_yes;
   else if ((_kind & area) != 0)
      return TR_maybe;
   else
      return TR_no;
   }

TR_YesNoMaybe TR_VPObjectLocation::isStackObject()
   {
   return isWithin(StackObject);
   }

TR_YesNoMaybe TR_VPObjectLocation::isHeapObject()
   {
   return isWithin(HeapObject);
   }

TR_YesNoMaybe TR_VPObjectLocation::isClassObject()
   {
   return isWithin(ClassObject);
   }

TR_YesNoMaybe TR_VPObjectLocation::isJavaLangClassObject()
   {
   return isWithin(JavaLangClassObject);
   }

TR_YesNoMaybe TR_VPObjectLocation::isJ9ClassObject()
   {
   return isWithin(J9ClassObject);
   }

TR_VPRelation *TR_VPLessThanOrEqual::getComplement(TR_ValuePropagation *vp)
   {
   TR_VPRelation *rel = TR_VPGreaterThanOrEqual::create(vp, -increment());
   if (hasArtificialIncrement())
      rel->setHasArtificialIncrement();
   return rel;
   }

TR_VPRelation *TR_VPGreaterThanOrEqual::getComplement(TR_ValuePropagation *vp)
   {
   TR_VPRelation *rel = TR_VPLessThanOrEqual::create(vp, -increment());
   if (hasArtificialIncrement())
      rel->setHasArtificialIncrement();
   return rel;
   }

TR_VPRelation *TR_VPEqual::getComplement(TR_ValuePropagation *vp)
   {
   if (increment() == 0)
      return this;
   return TR_VPEqual::create(vp, -increment());
   }

TR_VPRelation *TR_VPNotEqual::getComplement(TR_ValuePropagation *vp)
   {
   if (increment() == 0)
      return this;
   return TR_VPNotEqual::create(vp, -increment());
   }

// ***************************************************************************
//
// Creation of Value Propagation Constraints
//
// ***************************************************************************

TR_VPConstraint *TR_VPConstraint::create(TR_ValuePropagation *vp, const char *sig, int32_t len, TR_ResolvedMethod *method, bool isFixedClass)
   {
   // Create a constraint if possible from any arbitrary signature
   //
   switch (sig[0])
      {
      case 'L':
      case '[':
         return TR_VPClassType::create(vp, sig, len, method, isFixedClass);
      case 'B':
         return TR_VPIntRange::create(vp, TR::Int8, TR_no /* signed */);
      case 'Z':
         return TR_VPIntRange::create(vp, TR::Int8, TR_yes /* unsigned */); // FIXME: we can probably do 0/1 here
      case 'C':
         return TR_VPIntRange::create(vp, TR::Int16, TR_yes /* unsigned */);
      case 'S':
         return TR_VPIntRange::create(vp, TR::Int16, TR_no /* signed */);
      }
   return NULL;
   }

TR_VPShortConst *TR_VPShortConst::create(TR_ValuePropagation *vp, int16_t v)
   {
   int32_t hash = ((uint32_t)v) % VP_HASH_TABLE_SIZE;
   TR_VPShortConst * constraint;
   TR_ValuePropagation::ConstraintsHashTableEntry *entry;
   for (entry = vp->_constraintsHashTable[hash]; entry; entry = entry->next)
       {
       constraint = entry->constraint->asShortConst();
       if (constraint && constraint->getShort() == v)
           return constraint;
       }
   constraint = new (vp->trStackMemory()) TR_VPShortConst(v);
   vp->addConstraint(constraint,hash);
   return constraint;
   }

TR_VPIntConst *TR_VPIntConst::create(TR_ValuePropagation *vp, int32_t v)
   {
   // If the constant is zero, return the cached constraint
   //
   if (v == 0)
      return vp->_constantZeroConstraint;

   // If the constraint does not already exist, create it
   //
   int32_t hash = ((uint32_t)v) % VP_HASH_TABLE_SIZE;
   TR_VPIntConst *constraint;
   TR_ValuePropagation::ConstraintsHashTableEntry *entry;
   for (entry = vp->_constraintsHashTable[hash]; entry; entry = entry->next)
      {
      constraint = entry->constraint->asIntConst();
      if (constraint && constraint->getInt() == v)
         return constraint;
      }
   constraint = new (vp->trStackMemory()) TR_VPIntConst(v);
   vp->addConstraint(constraint, hash);
   return constraint;
   }

TR_VPConstraint *TR_VPShortConst::createExclusion(TR_ValuePropagation *vp, int16_t v)
   {
   if (v == TR::getMinSigned<TR::Int16>())
       return TR_VPShortRange::create(vp,v+1,TR::getMaxSigned<TR::Int16>());
   if (v == TR::getMaxSigned<TR::Int16>())
       return TR_VPShortRange::create(vp,TR::getMinSigned<TR::Int16>(),v-1);
   return TR_VPMergedConstraints::create(vp, TR_VPShortRange::create(vp,TR::getMinSigned<TR::Int16>(),v-1),TR_VPShortRange::create(vp,v+1,TR::getMaxSigned<TR::Int16>()));
   }

TR_VPConstraint *TR_VPIntConst::createExclusion(TR_ValuePropagation *vp, int32_t v)
   {
   if (v == TR::getMinSigned<TR::Int32>())
      return TR_VPIntRange::create(vp, v+1, TR::getMaxSigned<TR::Int32>());
   if (v == TR::getMaxSigned<TR::Int32>())
      return TR_VPIntRange::create(vp, TR::getMinSigned<TR::Int32>(), v-1);
   return TR_VPMergedConstraints::create(vp, TR_VPIntRange::create(vp, TR::getMinSigned<TR::Int32>(), v-1), TR_VPIntRange::create(vp, v+1, TR::getMaxSigned<TR::Int32>()));
   }

TR_VPShortConstraint * TR_VPShortRange::create(TR_ValuePropagation * vp, int16_t low, int16_t high, TR_YesNoMaybe canOverflow)
   {
   if (low == TR::getMinSigned<TR::Int16>() && high == TR::getMaxSigned<TR::Int16>())
       return NULL;

   if (low == high)
       return TR_VPShortConst::create(vp,low);

   uint32_t uint32low = (uint32_t)low;
   uint32_t uint32high = (uint32_t)high;

   int32_t hash = ((uint32low<<8)+uint32high) % VP_HASH_TABLE_SIZE;
   TR_VPShortRange *constraint;
   TR_ValuePropagation::ConstraintsHashTableEntry *entry;
   for(entry = vp->_constraintsHashTable[hash]; entry; entry = entry->next)
       {
        constraint = entry->constraint->asShortRange();
        if(constraint &&
           constraint->_low == low &&
           constraint->_high == high &&
           constraint->_overflow == canOverflow)
           return constraint;
       }
   constraint = new (vp->trStackMemory()) TR_VPShortRange(low,high);
   constraint->setCanOverflow(canOverflow);
   vp->addConstraint(constraint,hash);
   return constraint;
   }

TR_VPShortConstraint *TR_VPShortRange::create(TR_ValuePropagation *vp)
   {
   return TR_VPShortRange::createWithPrecision(vp, VP_UNDEFINED_PRECISION);
   }

TR_VPShortConstraint *TR_VPShortRange::createWithPrecision(TR_ValuePropagation *vp, int32_t precision, bool isNonNegative)
   {
   int64_t lo, hi;
   constrainRangeByPrecision(TR::getMinSigned<TR::Int16>(), TR::getMaxSigned<TR::Int16>(), precision, lo, hi, isNonNegative);
   return TR_VPShortRange::create(vp, lo, hi);
   }

TR_VPIntConstraint *TR_VPIntRange::create(TR_ValuePropagation *vp, int32_t low, int32_t high, TR_YesNoMaybe canOverflow)
   {
   if (low == TR::getMinSigned<TR::Int32>() && high == TR::getMaxSigned<TR::Int32>())
      return NULL;

//    if (isUnsigned &&
//          ((uint32_t)low == TR::getMinUnsigned<TR::Int32>() && (uint32_t)high == TR::getMaxUnsigned<TR::Int32>()))
//       return NULL;

   if (low == high)
      return TR_VPIntConst::create(vp, low);

   // If the constraint does not already exist, create it
   //
   int32_t hash = ((uint32_t)((low << 16) + high)) % VP_HASH_TABLE_SIZE;
   TR_VPIntRange *constraint;
   TR_ValuePropagation::ConstraintsHashTableEntry *entry;
   for (entry = vp->_constraintsHashTable[hash]; entry; entry = entry->next)
      {
      constraint = entry->constraint->asIntRange();
      if (constraint &&
            constraint->_low == low &&
            constraint->_high == high &&
            constraint->_overflow == canOverflow)
         return constraint;
      }
   constraint = new (vp->trStackMemory()) TR_VPIntRange(low, high);
   constraint->setCanOverflow(canOverflow);
   //if (isUnsigned)
   //   constraint->setIsUnsigned(true);
   vp->addConstraint(constraint, hash);
   return constraint;
   }

TR_VPIntConstraint *TR_VPIntRange::create(TR_ValuePropagation *vp, TR::DataTypes dt, TR_YesNoMaybe isUnsigned)
   {
   return TR_VPIntRange::createWithPrecision(vp, dt, VP_UNDEFINED_PRECISION, isUnsigned);
   }

TR_VPIntConstraint *TR_VPIntRange::createWithPrecision(TR_ValuePropagation *vp, TR::DataTypes dt, int32_t precision, TR_YesNoMaybe isUnsigned, bool isNonNegative)
   {
   TR_ASSERT(dt > TR::NoType && dt < TR::Int64, "Bad range for datatype in integerLoad constant propagation\n");

   int64_t lo = TR::getMinSigned<TR::Int64>();
   int64_t hi = TR::getMaxSigned<TR::Int64>();

   if (dt == TR::Int32)
      constrainRangeByPrecision(TR::getMinSigned<TR::Int32>(), TR::getMaxSigned<TR::Int32>(), precision, lo, hi, isNonNegative);

   if (isUnsigned == TR_no)
      {
      if (dt == TR::Int8)
         constrainRangeByPrecision(TR::getMinSigned<TR::Int8>(), TR::getMaxSigned<TR::Int8>(), precision, lo, hi, isNonNegative);
      else if (dt == TR::Int16)
         constrainRangeByPrecision(TR::getMinSigned<TR::Int16>(), TR::getMaxSigned<TR::Int16>(), precision, lo, hi, isNonNegative);
      }
   else if (isUnsigned == TR_yes)
      {
      if (dt == TR::Int8)
         constrainRangeByPrecision(0, TR::getMaxUnsigned<TR::Int8>(), precision, lo, hi, isNonNegative);
      else if (dt == TR::Int16)
         constrainRangeByPrecision(0, TR::getMaxUnsigned<TR::Int16>(), precision, lo, hi, isNonNegative);
      }
   else
      {
      if (dt == TR::Int8)
         constrainRangeByPrecision(TR::getMinSigned<TR::Int8>(), TR::getMaxUnsigned<TR::Int8>(), precision, lo, hi, isNonNegative);
      else if (dt == TR::Int16)
         constrainRangeByPrecision(TR::getMinSigned<TR::Int16>(), TR::getMaxUnsigned<TR::Int16>(), precision, lo, hi, isNonNegative);
      }
   return TR_VPIntRange::create(vp, lo, hi);
   }

TR_VPLongConst *TR_VPLongConst::create(TR_ValuePropagation *vp, int64_t v)
   {
   // If the constraint does not already exist, create it
   //
   int32_t hash = (int32_t)(v >> 32);
   hash += (int32_t)v;
   hash = ((uint32_t)hash) % VP_HASH_TABLE_SIZE;
   TR_VPLongConst *constraint;
   TR_ValuePropagation::ConstraintsHashTableEntry *entry;
   for (entry = vp->_constraintsHashTable[hash]; entry; entry = entry->next)
      {
      constraint = entry->constraint->asLongConst();
      if (constraint && constraint->_low == v)
         return constraint;
      }
   constraint = new (vp->trStackMemory()) TR_VPLongConst(v);
   vp->addConstraint(constraint, hash);
   return constraint;
   }
TR_VPConstraint *TR_VPLongConst::createExclusion(TR_ValuePropagation *vp, int64_t v)
   {
   if (v == TR::getMinSigned<TR::Int64>())
      return TR_VPLongRange::create(vp, v+1, TR::getMaxSigned<TR::Int64>());
   if (v == TR::getMaxSigned<TR::Int64>())
      return TR_VPLongRange::create(vp, TR::getMinSigned<TR::Int64>(), v-1);
   return TR_VPMergedConstraints::create(vp, TR_VPLongRange::create(vp, TR::getMinSigned<TR::Int64>(), v-1), TR_VPLongRange::create(vp, v+1, TR::getMaxSigned<TR::Int64>()));
   }

TR_VPLongConstraint *TR_VPLongRange::create(TR_ValuePropagation *vp, int64_t low, int64_t high,
                                            bool powerOfTwo, TR_YesNoMaybe canOverflow)
   {
   if (low == TR::getMinSigned<TR::Int64>() && high == TR::getMaxSigned<TR::Int64>() && !powerOfTwo)
      return NULL;

   if (low == high)
      return TR_VPLongConst::create(vp, low);

   // If the constraint does not already exist, create it
   //
   int32_t hash = ((uint32_t)low + (uint32_t)high) % VP_HASH_TABLE_SIZE;
   TR_VPLongRange *constraint;
   TR_ValuePropagation::ConstraintsHashTableEntry *entry;
   for (entry = vp->_constraintsHashTable[hash]; entry; entry = entry->next)
      {
      constraint = entry->constraint->asLongRange();
      if (constraint &&
            constraint->_low == low &&
            constraint->_high == high &&
            constraint->_overflow == canOverflow)
         return constraint;
      }
   constraint = new (vp->trStackMemory()) TR_VPLongRange(low, high);
   constraint->setCanOverflow(canOverflow);
   vp->addConstraint(constraint, hash);

   if (powerOfTwo)
      constraint->setIsPowerOfTwo();

   return constraint;
   }

TR_VPConstraint *TR_VPClass::create(TR_ValuePropagation *vp, TR_VPClassType *type, TR_VPClassPresence *presence,
                                    TR_VPPreexistentObject *preexistence, TR_VPArrayInfo *arrayInfo, TR_VPObjectLocation *location)
   {
   // We shouldn't create a class constraint that contains the "null" constraint
   // since null objects are not typed.
   //
   if (presence)
      {
      TR_ASSERT(!presence->isNullObject(), "Can't create a class constraint with a NULL constraint");
      }

   // If only one of the parts is non-null we don't need a TR_VPClass constraint
   //
   if (!type)
      {
      if (!presence)
         {
         if (!preexistence)
            {
            if (!arrayInfo)
               return location;
            else if (!location)
               return arrayInfo;
            }
         else if (!arrayInfo && !location)
            return preexistence;
         }
      else if (!preexistence && !arrayInfo && !location)
         return presence;
      }
   else if (!presence && !preexistence && !arrayInfo && !location)
      return type;

#ifdef J9_PROJECT_SPECIFIC
   // TR_VPFixedClass combined with JavaLangClassObject location picks out a
   // particular java/lang/Class instance, for which we can have a known
   // object. Inject one here if we don't already have one.
   if (location != NULL
       && location->isJavaLangClassObject() == TR_yes
       && type != NULL
       && type->asFixedClass() != NULL
       && type->asKnownObject() == NULL
       && !isSpecialClass((uintptrj_t)type->getClass()))
      {
      TR_J9VMBase *fej9 = (TR_J9VMBase *)(vp->comp()->fe());
      uintptrj_t objRefOffs = fej9->getOffsetOfJavaLangClassFromClassField();
      uintptrj_t *objRef = (uintptrj_t*)(type->getClass() + objRefOffs);
      TR::KnownObjectTable *knot = vp->comp()->getOrCreateKnownObjectTable();
      TR::KnownObjectTable::Index index = knot->getIndexAt(objRef);
      type = TR_VPKnownObject::createForJavaLangClass(vp, index);
      }
#endif

   // If the constraint does not already exist, create it
   //
   int32_t hash = (((int32_t)(intptrj_t)type)>>2) + (((int32_t)(intptrj_t)presence)>>2) + (((int32_t)(intptrj_t)preexistence)>>2) +
      (((int32_t)(intptrj_t)arrayInfo)>>2) + (((int32_t)(intptrj_t)location)>>2);
   hash = ((uint32_t)hash) % VP_HASH_TABLE_SIZE;
   TR_VPClass *constraint;
   TR_ValuePropagation::ConstraintsHashTableEntry *entry;
   for (entry = vp->_constraintsHashTable[hash]; entry; entry = entry->next)
      {
      constraint = entry->constraint->asClass();
      if (constraint &&
          constraint->_type         == type &&
          constraint->_presence     == presence &&
          constraint->_preexistence == preexistence &&
          constraint->_arrayInfo    == arrayInfo &&
          constraint->_location     == location)
         return constraint;
      }
   constraint = new (vp->trStackMemory()) TR_VPClass(type, presence, preexistence, arrayInfo, location);
   vp->addConstraint(constraint, hash);
   return constraint;
   }

TR_VPClassType *TR_VPClassType::create(TR_ValuePropagation *vp, TR::SymbolReference *symRef, bool isFixedClass, bool isPointerToClass)
   {
   if (!symRef->isUnresolved())
      {
      TR_OpaqueClassBlock *classObject = (TR_OpaqueClassBlock*)symRef->getSymbol()->getStaticSymbol()->getStaticAddress();
      if (isPointerToClass)
         classObject = *((TR_OpaqueClassBlock**)classObject);
      if (isFixedClass)
         return TR_VPFixedClass::create(vp, classObject);
      return TR_VPResolvedClass::create(vp, classObject);
      }

   int32_t len;
   char *name = TR::Compiler->cls.classNameChars(vp->comp(), symRef, len);
   TR_ASSERT(name, "can't get class name from symbol reference");
   char *sig = classNameToSignature(name, len, vp->comp());
   //return TR_VPUnresolvedClass::create(vp, sig, len, symRef->getOwningMethod(vp->comp()));
   return TR_VPClassType::create(vp, sig, len, symRef->getOwningMethod(vp->comp()), isFixedClass);
   }

TR_VPClassType *TR_VPClassType::create(TR_ValuePropagation *vp, const char *sig, int32_t len, TR_ResolvedMethod *method, bool isFixed, TR_OpaqueClassBlock *classObject)
   {
#ifdef J9_PROJECT_SPECIFIC
   if (!classObject)
      classObject = vp->fe()->getClassFromSignature(sig, len, method);
   if (classObject)
      {
      bool isClassInitialized = false;
      TR_PersistentClassInfo * classInfo =
         vp->comp()->getPersistentInfo()->getPersistentCHTable()->findClassInfoAfterLocking(classObject, vp->comp());
      if (classInfo && classInfo->isInitialized())
         {
         if (isFixed)
            return TR_VPFixedClass::create(vp, classObject);
         return TR_VPResolvedClass::create(vp, classObject);
         }
      }
#endif
   return TR_VPUnresolvedClass::create(vp, sig, len, method);
   }

//Workaround for zOS V1R11 bug
#if defined(J9ZOS390)
#pragma noinline(TR_VPResolvedClass::TR_VPResolvedClass(TR_OpaqueClassBlock *,TR::Compilation *))
#pragma noinline(TR_VPResolvedClass::TR_VPResolvedClass(TR_OpaqueClassBlock *, TR::Compilation *, int32_t))
#endif

TR_VPResolvedClass *TR_VPResolvedClass::create(TR_ValuePropagation *vp, TR_OpaqueClassBlock *klass)
   {
   // If the class is final, we really want to make this a fixed class
   //
   if (!TR_VPConstraint::isSpecialClass((uintptrj_t)klass) && TR::Compiler->cls.isClassFinal(vp->comp(), klass))
      {
      if (TR::Compiler->cls.isClassArray(vp->comp(), klass))
         {
         // An array class is fixed if the base class for the array is final
         //
         TR_OpaqueClassBlock * baseClass = vp->fe()->getLeafComponentClassFromArrayClass(klass);
         if (baseClass && TR::Compiler->cls.isClassFinal(vp->comp(), baseClass))
            return TR_VPFixedClass::create(vp, klass);
         }
      else
         return TR_VPFixedClass::create(vp, klass);
      }

   // If the constraint does not already exist, create it
   //
   int32_t hash = (int32_t)((((uintptrj_t)klass) >> 2) % VP_HASH_TABLE_SIZE);
   TR_VPResolvedClass *constraint;
   TR_ValuePropagation::ConstraintsHashTableEntry *entry;
   for (entry = vp->_constraintsHashTable[hash]; entry; entry = entry->next)
      {
      constraint = entry->constraint->asResolvedClass();
      if (constraint && !constraint->asFixedClass() &&
          constraint->getClass() == klass)
         return constraint;
      }
   constraint = new (vp->trStackMemory()) TR_VPResolvedClass(klass, vp->comp());
   vp->addConstraint(constraint, hash);
   return constraint;
   }

TR_VPFixedClass *TR_VPFixedClass::create(TR_ValuePropagation *vp, TR_OpaqueClassBlock *klass)
   {
   // If the constraint does not already exist, create it
   //
   int32_t hash = (int32_t)((((uintptrj_t)klass) << 2) % VP_HASH_TABLE_SIZE);
   TR_VPFixedClass *constraint;
   TR_ValuePropagation::ConstraintsHashTableEntry *entry;
   for (entry = vp->_constraintsHashTable[hash]; entry; entry = entry->next)
      {
      constraint = entry->constraint->asFixedClass();
      if (constraint && !constraint->hasMoreThanFixedClassInfo() && constraint->getClass() == klass)
         {
         return constraint;
         }
      }
   constraint = new (vp->trStackMemory()) TR_VPFixedClass(klass, vp->comp());
   vp->addConstraint(constraint, hash);
   return constraint;
   }

TR_VPKnownObject *TR_VPKnownObject::create(TR_ValuePropagation *vp, TR::KnownObjectTable::Index index, bool isJavaLangClass)
   {
   TR::KnownObjectTable *knot = vp->comp()->getKnownObjectTable();
   TR_ASSERT(knot, "Can't create a TR_VPKnownObject without a known-object table");
   if (knot->isNull(index)) // No point in worrying about the NULL case because existing constraints handle that optimally
      return NULL;

   int32_t hash = (index * 3331) % VP_HASH_TABLE_SIZE;
   TR_VPKnownObject *constraint;
   TR_ValuePropagation::ConstraintsHashTableEntry *entry;
   for (entry = vp->_constraintsHashTable[hash]; entry; entry = entry->next)
      {
      constraint = entry->constraint->getKnownObject();
      if (constraint && constraint->_index == index)
         return constraint;
      }

   // Must make a new one; try to acquire VM access and grab the object's class
   //
   constraint = NULL;
#ifdef J9_PROJECT_SPECIFIC
      {
      TR::VMAccessCriticalSection vpKnownObjectCriticalSection(vp->comp(),
                                                                TR::VMAccessCriticalSection::tryToAcquireVMAccess);

      if (vpKnownObjectCriticalSection.hasVMAccess())
         {
         TR_OpaqueClassBlock *clazz = TR::Compiler->cls.objectClass(vp->comp(), *vp->comp()->getKnownObjectTable()->getPointerLocation(index));
         TR_OpaqueClassBlock *jlClass = vp->fe()->getClassClassPointer(clazz);
         if (isJavaLangClass)
            {
            TR_ASSERT(clazz == jlClass, "Use createForJavaLangClass only for instances of java/lang/Class");
            clazz = TR::Compiler->cls.classFromJavaLangClass(vp->comp(), *vp->comp()->getKnownObjectTable()->getPointerLocation(index));
            }
         else
            {
            // clazz is already right
            TR_ASSERT(clazz != jlClass, "For java/lang/Class instances, caller needs to use createForJavaLangClass.");
            }
         constraint = new (vp->trStackMemory()) TR_VPKnownObject(clazz, vp->comp(), index, isJavaLangClass);
         vp->addConstraint(constraint, hash);
         }
      }
#endif
   return constraint;
   }

TR_VPConstString *TR_VPConstString::create(TR_ValuePropagation *vp, TR::SymbolReference *symRef)
   {
#ifdef J9_PROJECT_SPECIFIC
   TR::VMAccessCriticalSection vpConstStringCriticalSection(vp->comp(),
                                                             TR::VMAccessCriticalSection::tryToAcquireVMAccess);

   if (vpConstStringCriticalSection.hasVMAccess())
      {
      void *string = *((void **) symRef->getSymbol()->castToStaticSymbol()->getStaticAddress());
      // with no vmaccess, staticAddress cannot be guaranteed to remain the same
      // during the analysis. so use a different hash input
      //
      ////int32_t hash = ((uintptrj_t)string >> 2) % VP_HASH_TABLE_SIZE;
      ////int32_t hash = ((uintptrj_t)(symRef->getSymbol()->castToStaticSymbol()) >> 2) % VP_HASH_TABLE_SIZE;

      // since vmaccess has been acquired already, chars cannot be null
      //
      int32_t len = vp->comp()->fej9()->getStringLength((uintptrj_t)string);
      int32_t i = 0;
      uint32_t hashValue = 0;
      for (int32_t i = 0; i < len && i < TR_MAX_CHARS_FOR_HASH; i++)
         hashValue += TR::Compiler->cls.getStringCharacter(vp->comp(), (uintptrj_t)string, i);

      int32_t hash = (int32_t)(((uintptrj_t)hashValue) % VP_HASH_TABLE_SIZE);

      TR_ValuePropagation::ConstraintsHashTableEntry *entry;
      TR_VPConstString *constraint;
      for (entry = vp->_constraintsHashTable[hash]; entry; entry = entry->next)
         {
         constraint = entry->constraint->asConstString();
         if (constraint &&
             string == *((void **) constraint->_symRef->getSymbol()->castToStaticSymbol()->getStaticAddress()))
            {
            return constraint;
            }
         }
      constraint = new (vp->trStackMemory()) TR_VPConstString(vp->comp()->getStringClassPointer(), vp->comp(), symRef);
      vp->addConstraint(constraint, hash);
      return constraint;
      }
#endif
   return NULL;
   }

uint16_t TR_VPConstString::charAt(int32_t i, TR::Compilation * comp)
   {
   uint16_t result = 0;
#ifdef J9_PROJECT_SPECIFIC
   TR_FrontEnd * fe = comp->fe();
   TR::VMAccessCriticalSection charAtCriticalSection(comp,
                                                      TR::VMAccessCriticalSection::tryToAcquireVMAccess);
   if (charAtCriticalSection.hasVMAccess())
      {
      uintptrj_t string = *(uintptrj_t*)_symRef->getSymbol()->castToStaticSymbol()->getStaticAddress();
      int32_t len = comp->fej9()->getStringLength(string);
      bool canRead = true;
      if (i < 0 || i >= len)
         canRead = false;
      if (canRead)
         result = TR::Compiler->cls.getStringCharacter(comp, string, i);
      }
#endif
   return result;
   }


bool TR_VPConstString::getFieldByName(TR::SymbolReference *symRef, void* &val, TR::Compilation * comp)
   {
   return TR::Compiler->cls.getStringFieldByName(comp, _symRef, symRef, val);
   }

TR_VPUnresolvedClass *TR_VPUnresolvedClass::create(TR_ValuePropagation *vp, const char *sig, int32_t len, TR_ResolvedMethod *method)
   {
   // If the constraint does not already exist, create it
   //
   int32_t hash = (((uint32_t)(uintptrj_t)method >> 2) + len) % VP_HASH_TABLE_SIZE;
   TR_VPUnresolvedClass *constraint;
   TR_ValuePropagation::ConstraintsHashTableEntry *entry;
   for (entry = vp->_constraintsHashTable[hash]; entry; entry = entry->next)
      {
      constraint = entry->constraint->asUnresolvedClass();
      if (constraint && constraint->_len == len &&
          constraint->_method == method)
         {
         if (!strncmp(constraint->_sig, sig, len))
            return constraint;
         }
      }
   constraint = new (vp->trStackMemory()) TR_VPUnresolvedClass(sig, len, method);
   vp->addConstraint(constraint, hash);
   return constraint;
   }

TR_VPNullObject *TR_VPNullObject::create(TR_ValuePropagation *vp)
   {
   return vp->_nullObjectConstraint;
   }

TR_VPNonNullObject *TR_VPNonNullObject::create(TR_ValuePropagation *vp)
   {
   return vp->_nonNullObjectConstraint;
   }

TR_VPPreexistentObject *TR_VPPreexistentObject::create(TR_ValuePropagation *vp, TR_OpaqueClassBlock *c)
   {
   TR_ASSERT(vp->comp()->ilGenRequest().details().supportsInvalidation(), "Can't use TR_VPPreexistentObject unless the compiled method supports invalidation");
   int32_t hash = (int32_t)((((uintptrj_t)c) << 2) % VP_HASH_TABLE_SIZE);
   TR_VPPreexistentObject *constraint;
   TR_ValuePropagation::ConstraintsHashTableEntry *entry;
   for (entry = vp->_constraintsHashTable[hash]; entry; entry = entry->next)
      {
      constraint = entry->constraint->getPreexistence();
      if (constraint && constraint->getPreexistence()->getAssumptionClass() == c)
         {
         return constraint;
         }
      }

   constraint = new (vp->trStackMemory()) TR_VPPreexistentObject(c);
   vp->addConstraint(constraint, hash);
   return constraint;
   //return vp->_preexistentObjectConstraint;
   }

TR_VPArrayInfo *TR_VPArrayInfo::create(TR_ValuePropagation *vp, int32_t lowBound, int32_t highBound, int32_t elementSize)
   {
   // If the constraint does not already exist, create it
   //
   int32_t hash = ((uint32_t)((lowBound << 16) + highBound + elementSize)) % VP_HASH_TABLE_SIZE;
   TR_VPArrayInfo *constraint;
   TR_ValuePropagation::ConstraintsHashTableEntry *entry;
   for (entry = vp->_constraintsHashTable[hash]; entry; entry = entry->next)
      {
      constraint = entry->constraint->asArrayInfo();
      if (constraint && constraint->lowBound() == lowBound && constraint->highBound() == highBound && constraint->elementSize() == elementSize)
         return constraint;
      }
   constraint = new (vp->trStackMemory()) TR_VPArrayInfo(lowBound, highBound, elementSize);
   vp->addConstraint(constraint, hash);
   return constraint;
   }

TR_VPArrayInfo *TR_VPArrayInfo::create(TR_ValuePropagation *vp, char *sig)
   {
   TR_ASSERT(*sig == '[', "expecting array signature");
   TR::DataTypes d = TR::Symbol::convertSigCharToType(sig[1]);
   int32_t stride;
   if (d == TR::Address)
      stride = TR::Compiler->om.sizeofReferenceField();
   else
      stride = TR::Symbol::convertTypeToSize(d);

   return TR_VPArrayInfo::create(vp, 0, TR::getMaxSigned<TR::Int32>() / stride, stride);
   }

TR_VPMergedConstraints *TR_VPMergedConstraints::create(TR_ValuePropagation *vp, TR_VPConstraint *first, TR_VPConstraint *second)
   {
   // If the constraint does not already exist, create it
   //
   int32_t hash = (int32_t)(((((uintptrj_t)first) >> 2) + (((uintptrj_t)second) >> 2)) % VP_HASH_TABLE_SIZE);

   TR_VPMergedConstraints *constraint;
   TR_ValuePropagation::ConstraintsHashTableEntry *entry;
   for (entry = vp->_constraintsHashTable[hash]; entry; entry = entry->next)
      {
      constraint = entry->constraint->asMergedConstraints();
      if (constraint)
         {
         ListElement<TR_VPConstraint> *p = constraint->_constraints.getListHead();
         if (p->getData() == first)
            {
            p = p->getNextElement();
            if (p->getData() == second && !p->getNextElement())
               return constraint;
            }
         }
      }
   TR_ScratchList<TR_VPConstraint> list(vp->trMemory());
   list.add(second);
   list.add(first);

   constraint = new (vp->trStackMemory()) TR_VPMergedConstraints(list.getListHead(), vp->trMemory());
   if (first->isUnsigned() && second->isUnsigned())
      constraint->setIsUnsigned(true);

   vp->addConstraint(constraint, hash);
   return constraint;
   }

TR_VPObjectLocation *TR_VPObjectLocation::create(TR_ValuePropagation *vp, TR_VPObjectLocationKind kind)
   {
   int32_t hash = (kind * 4109) % VP_HASH_TABLE_SIZE;
   TR_VPObjectLocation *constraint;
   TR_ValuePropagation::ConstraintsHashTableEntry *entry;
   for (entry = vp->_constraintsHashTable[hash]; entry; entry = entry->next)
      {
      constraint = entry->constraint->asObjectLocation();
      if (constraint && constraint->_kind == kind)
         return constraint;
      }
   constraint = new (vp->trStackMemory()) TR_VPObjectLocation(kind);
   vp->addConstraint(constraint, hash);
   return constraint;
   }

TR_VPMergedConstraints *TR_VPMergedConstraints::create(TR_ValuePropagation *vp, ListElement<TR_VPConstraint> *list)
   {
   // If the constraint does not already exist, create it
   //
   int32_t hash = 0;
   bool allUnsigned = true;
   if (!list)
      allUnsigned = false;

   ListElement<TR_VPConstraint> *p1, *p2;
   for (p1 = list; p1; p1 = p1->getNextElement())
      {
      if (!p1->getData()->isUnsigned())
         allUnsigned = false;
      hash += (int32_t)(((uintptrj_t)(p1->getData())) >> 2);
      }
   hash = ((uint32_t)hash) % VP_HASH_TABLE_SIZE;

   TR_VPMergedConstraints *constraint;
   TR_ValuePropagation::ConstraintsHashTableEntry *entry;
   for (entry = vp->_constraintsHashTable[hash]; entry; entry = entry->next)
      {
      constraint = entry->constraint->asMergedConstraints();
      if (constraint)
         {
         for (p1 = list, p2 = constraint->_constraints.getListHead();
              p1 && p2 && p1->getData() == p2->getData();
              p1 = p1->getNextElement(), p2 = p2->getNextElement())
            ;
         if (!p1 && !p2)
            return constraint;
         }
      }
   constraint = new (vp->trStackMemory()) TR_VPMergedConstraints(list, vp->trMemory());
   if (allUnsigned)
      constraint->setIsUnsigned(true);

   vp->addConstraint(constraint, hash);
   return constraint;
   }

TR_VPUnreachablePath *TR_VPUnreachablePath::create(TR_ValuePropagation *vp)
   {
   return vp->_unreachablePathConstraint;
   }

TR_VPSync *TR_VPSync::create(TR_ValuePropagation *vp, TR_YesNoMaybe v)
   {
   int32_t hash = ((uint32_t)(v << 2) * 4109) % VP_HASH_TABLE_SIZE;
   TR_VPSync *constraint;
   TR_ValuePropagation::ConstraintsHashTableEntry *entry;
   for (entry = vp->_constraintsHashTable[hash]; entry; entry = entry->next)
      {
      constraint = entry->constraint->asVPSync();
      if (constraint &&
            (constraint->syncEmitted() == v))
         return constraint;
      }
   constraint = new (vp->trStackMemory()) TR_VPSync(v);
   vp->addConstraint(constraint, hash);
   return constraint;
   }

TR_VPLessThanOrEqual *TR_VPLessThanOrEqual::create(TR_ValuePropagation *vp, int32_t incr)
   {
   // If the constraint does not already exist, create it
   //
   int32_t hash = ((uint32_t)((incr << 16) + LessThanOrEqualPriority)) % VP_HASH_TABLE_SIZE;
   TR_VPLessThanOrEqual *constraint;
   TR_ValuePropagation::ConstraintsHashTableEntry *entry;
   for (entry = vp->_constraintsHashTable[hash]; entry; entry = entry->next)
      {
      constraint = entry->constraint->asLessThanOrEqual();
      if (constraint && constraint->increment() == incr)
         return constraint;
      }
   constraint = new (vp->trStackMemory()) TR_VPLessThanOrEqual(incr);
   vp->addConstraint(constraint, hash);
   return constraint;
   }

TR_VPGreaterThanOrEqual *TR_VPGreaterThanOrEqual::create(TR_ValuePropagation *vp, int32_t incr)
   {
   // If the constraint does not already exist, create it
   //
   int32_t hash = ((uint32_t)((incr << 16) + GreaterThanOrEqualPriority)) % VP_HASH_TABLE_SIZE;
   TR_VPGreaterThanOrEqual *constraint;
   TR_ValuePropagation::ConstraintsHashTableEntry *entry;
   for (entry = vp->_constraintsHashTable[hash]; entry; entry = entry->next)
      {
      constraint = entry->constraint->asGreaterThanOrEqual();
      if (constraint && constraint->increment() == incr)
         return constraint;
      }
   constraint = new (vp->trStackMemory()) TR_VPGreaterThanOrEqual(incr);
   vp->addConstraint(constraint, hash);
   return constraint;
   }

TR_VPEqual *TR_VPEqual::create(TR_ValuePropagation *vp, int32_t incr)
   {
   // If the constraint does not already exist, create it
   //
   int32_t hash = ((uint32_t)((incr << 16) + EqualPriority)) % VP_HASH_TABLE_SIZE;
   TR_VPEqual *constraint;
   TR_ValuePropagation::ConstraintsHashTableEntry *entry;
   for (entry = vp->_constraintsHashTable[hash]; entry; entry = entry->next)
      {
      constraint = entry->constraint->asEqual();
      if (constraint && constraint->increment() == incr)
         return constraint;
      }
   constraint = new (vp->trStackMemory()) TR_VPEqual(incr);
   vp->addConstraint(constraint, hash);
   return constraint;
   }

TR_VPNotEqual *TR_VPNotEqual::create(TR_ValuePropagation *vp, int32_t incr)
   {
   // If the constraint does not already exist, create it
   //
   int32_t hash = ((uint32_t)((incr << 16) + NotEqualPriority)) % VP_HASH_TABLE_SIZE;
   TR_VPNotEqual *constraint;
   TR_ValuePropagation::ConstraintsHashTableEntry *entry;
   for (entry = vp->_constraintsHashTable[hash]; entry; entry = entry->next)
      {
      constraint = entry->constraint->asNotEqual();
      if (constraint && constraint->increment() == incr)
         return constraint;
      }
   constraint = new (vp->trStackMemory()) TR_VPNotEqual(incr);
   vp->addConstraint(constraint, hash);
   return constraint;
   }

// ***************************************************************************
//
// Merging Value Propagation Constraints (logical OR)
//
// ***************************************************************************

TR_VPConstraint * TR_VPConstraint::merge(TR_VPConstraint *other, TR_ValuePropagation *vp)
   {
   // If this is the same constraint, just return it
   //
   if (this == other)
      return this;

   // Call the merge method on the constraint with the highest merge priority
   //
   if (other->priority() > priority())
      return other->merge1(this, vp);
   return merge1(other, vp);
   }

TR_VPConstraint * TR_VPConstraint::merge1(TR_VPConstraint *other, TR_ValuePropagation *vp)
   {
   TRACER(vp, this, other);

   // Default action is to generalize the constraint
   //
   return NULL;
   }


TR_VPConstraint *TR_VPShortConstraint::merge1(TR_VPConstraint * other, TR_ValuePropagation * vp)
   {
   TRACER(vp, this, other);

     TR_VPShortConstraint *otherShort = other->asShortConstraint();
     if (otherShort)
        {
        if (otherShort->getLow() < getLow())
            return otherShort->merge1(this,vp);
        if (otherShort->getHigh() <= getHigh())
            return this;
        if (otherShort->getLow() <= getHigh() + 1)
           {
           if (getLow() == TR::getMinSigned<TR::Int16>() && otherShort->getHigh() == TR::getMaxSigned<TR::Int16>())
               return NULL;
           return TR_VPShortRange::create(vp, getLow(),otherShort->getHigh());
           }
        }
     return NULL;
   }

TR_VPConstraint *TR_VPIntConstraint::merge1(TR_VPConstraint *other, TR_ValuePropagation *vp)
   {
   TRACER(vp, this, other);

   TR_VPIntConstraint *otherInt = other->asIntConstraint();
   //if (otherInt && otherInt->isUnsigned() && isUnsigned())
   //   return merge1(other, vp, true);

   if (otherInt)
      {
      if (otherInt->getLow() < getLow())
         return otherInt->merge1(this, vp);
      if (otherInt->getHigh() <= getHigh())
         return this; // This range covers the other
      if (otherInt->getLow() <= getHigh()+1)
         {
         // Ranges overlap
         //
         if (getLow() == TR::getMinSigned<TR::Int32>() && otherInt->getHigh() == TR::getMaxSigned<TR::Int32>())
            return NULL; // Constraint has now gone
         return TR_VPIntRange::create(vp, getLow(), otherInt->getHigh());
         }

      return TR_VPMergedConstraints::create(vp, this, other);
      }
   else
      {
      // due to the presence of aladd's, ints might merge with longs
      TR_VPLongConstraint *otherLong = other->asLongConstraint();
      if (otherLong)
         {
         int64_t lowVal, highVal;
         // if wrap-around is possible, the constraint should be gone
         if (((int64_t)otherLong->getLow() < (int64_t)TR::getMinSigned<TR::Int32>()) ||
             ((int64_t)otherLong->getHigh() > (int64_t)TR::getMaxSigned<TR::Int32>()))
            return NULL;

         // compute lower of the lows
         if ((int64_t)otherLong->getLow() < (int64_t)getLow())
            lowVal = otherLong->getLow();
         else
            lowVal = getLow();

         // compute higher of the highs
         if ((int64_t)otherLong->getHigh() <= (int64_t)getHigh())
            highVal = getHigh();
         else
            highVal = otherLong->getHigh();

         // check for range overlap. if there is an overlap, create
         // the appropriate Int Range.
         if ((int64_t)otherLong->getLow() <= (int64_t)(getHigh()+1))
            {
            if (getLow() == TR::getMinSigned<TR::Int32>() && (int64_t)otherLong->getHigh() == (int64_t)TR::getMaxSigned<TR::Int32>())
               return NULL;
            return TR_VPIntRange::create(vp, (int32_t)lowVal, (int32_t)highVal);
            }
         else
            {
            // the ranges are distinct, before merging, they should be ordered
            //
            if ((int64_t)otherLong->getLow() < (int64_t)getLow())
               return TR_VPMergedConstraints::create(vp, TR_VPIntRange::create(vp, (int32_t)otherLong->getLow(), (int32_t)otherLong->getHigh()), this);
            else
               return TR_VPMergedConstraints::create(vp, this, TR_VPIntRange::create(vp, (int32_t)otherLong->getLow(), (int32_t)otherLong->getHigh()));
            }
         }
      }
   return NULL;
   }

TR_VPConstraint *TR_VPLongConstraint::merge1(TR_VPConstraint *other, TR_ValuePropagation *vp)
   {
   TRACER(vp, this, other);

   TR_VPLongConstraint *otherLong = other->asLongConstraint();
   if (otherLong)
      {
      if (otherLong->getLow() < getLow())
         return otherLong->merge1(this, vp);
      if (otherLong->getHigh() <= getHigh())
         return this; // This range covers the other
      if (otherLong->getLow() <= getHigh()+1)
         {
         // Ranges overlap
         //
         if (getLow() == TR::getMinSigned<TR::Int64>() && otherLong->getHigh() == TR::getMaxSigned<TR::Int64>())
            return NULL; // Constraint has now gone
         return TR_VPLongRange::create(vp, getLow(), otherLong->getHigh());
         }
      return TR_VPMergedConstraints::create(vp, this, other);
      }
   else
      {
      TR_VPIntConstraint *otherInt = other->asIntConstraint();
      if (otherInt)
         {
         int64_t lowVal, highVal;

         if ((int64_t)otherInt->getLow() < (int64_t)getLow())
            lowVal = otherInt->getLow();
         else
            lowVal = getLow();
         if ((int64_t)otherLong->getHigh() <= (int64_t)getHigh())
            highVal = getHigh();
         else
            highVal = otherInt->getHigh();
         // check for range overlap. if there is an overlap, create
         // the appropriate Long Range.
         if ((int64_t)otherInt->getLow() <= (int64_t)(getHigh()+1))
            {
            if (lowVal == TR::getMinSigned<TR::Int64>() && highVal == TR::getMaxSigned<TR::Int64>())
               return NULL;
            return TR_VPLongRange::create(vp, lowVal, highVal);
            }
         else
            {
            // the ranges are distinct, before merging, they should be ordered
            //
            if ((int64_t)otherInt->getLow() < (int64_t)getLow())
               return TR_VPMergedConstraints::create(vp, TR_VPLongRange::create(vp, otherInt->getLow(), otherInt->getHigh()), this);
            else
               return TR_VPMergedConstraints::create(vp, this, TR_VPLongRange::create(vp, otherInt->getLow(), otherInt->getHigh()));
            }
         }
      }
   return NULL;
   }

TR_VPConstraint *TR_VPClass::merge1(TR_VPConstraint *other, TR_ValuePropagation *vp)
   {
   TRACER(vp, this, other);

   TR_VPClassType         *type         = NULL;
   TR_VPClassPresence     *presence     = NULL;
   TR_VPPreexistentObject *preexistence = NULL;
   TR_VPArrayInfo         *arrayInfo    = NULL;
   TR_VPObjectLocation    *location     = NULL;

   if (other->asClass())
      {
      TR_VPClass *otherClass = other->asClass();
      if (_type && otherClass->_type)
         type = (TR_VPClassType*)_type->merge(otherClass->_type, vp);
      if (_presence && otherClass->_presence)
         presence = (TR_VPClassPresence*)_presence->merge(otherClass->_presence, vp);
      if (_preexistence && otherClass->_preexistence)
         preexistence = _preexistence;
      if (_arrayInfo && otherClass->_arrayInfo)
         arrayInfo = (TR_VPArrayInfo*)_arrayInfo->merge(otherClass->_arrayInfo, vp);
      if (_location && otherClass->_location)
         location = (TR_VPObjectLocation*)_location->merge(otherClass->_location, vp);
      }
   else if (other->asClassType())
      {
      TR_VPClassType *otherType = other->asClassType();
      if (_type)
         type = (TR_VPClassType*)_type->merge(otherType, vp);
      }
   else if (other->asClassPresence())
      {
      if (other->isNullObject())
         {
         type = _type;
         location = _location;
         }
      TR_VPClassPresence *otherPresence = other->asClassPresence();
      if (_presence)
         presence = (TR_VPClassPresence*)_presence->merge(otherPresence, vp);
      }
   else if (other->asPreexistentObject())
      {
      if (_preexistence && (_preexistence->getAssumptionClass() == other->asPreexistentObject()->getAssumptionClass()))
         preexistence = _preexistence;
      }
   else if (other->asArrayInfo())
      {
      TR_VPArrayInfo *otherInfo = other->asArrayInfo();
      if (_arrayInfo)
         arrayInfo = (TR_VPArrayInfo*)_arrayInfo->merge(otherInfo, vp);
      }
   else if (other->asObjectLocation())
      {
      TR_VPObjectLocation *otherInfo = other->asObjectLocation();
      if (_location)
         location = (TR_VPObjectLocation*)_location->merge(otherInfo, vp);
      }
   else
      return NULL;

   if (type || presence || preexistence || arrayInfo || location)
      return TR_VPClass::create(vp, type, presence, preexistence, arrayInfo, location);
   return NULL;
   }

TR_VPConstraint *TR_VPResolvedClass::merge1(TR_VPConstraint *other, TR_ValuePropagation *vp)
   {
   TRACER(vp, this, other);

   TR_VPResolvedClass *otherClass = other->asResolvedClass();
   if (otherClass)
      {
      TR_OpaqueClassBlock *c1 = getClass();
      TR_OpaqueClassBlock *c2 = otherClass->getClass();
      if ((vp->fe()->isInstanceOf(c1, c2, false, true, true)) == TR_yes)
         {
         return otherClass;
         }
      if ((vp->fe()->isInstanceOf(c2, c1, false, true, true)) == TR_yes)
         {
         return this;
         }
      }
   else if (other->asClassPresence())
      {
      if (other->isNullObject())
         return this;
      }
   return NULL;
   }

TR_VPConstraint *TR_VPFixedClass::merge1(TR_VPConstraint *other, TR_ValuePropagation *vp)
   {
   TRACER(vp, this, other);

   if (other->asFixedClass())
      return NULL; // They can't be the same class

   TR_VPResolvedClass *otherClass = other->asResolvedClass();
   if (otherClass)
      {
      TR_OpaqueClassBlock *c1 = getClass();
      TR_OpaqueClassBlock *c2 = otherClass->getClass();
      if (vp->fe()->isInstanceOf(c1, c2, true, true, true) == TR_yes)
         return other;
      }
   else if (other->asClassPresence())
      {
      if (other->isNullObject())
         return this;
      }
   return NULL;
   }

TR_VPConstraint *TR_VPKnownObject::merge1(TR_VPConstraint *other, TR_ValuePropagation *vp)
   {
   TRACER(vp, this, other);

   TR_VPKnownObject *otherKnownObject = other->getKnownObject();
   TR_VPConstString *otherConstString = other->asConstString();
   if (otherKnownObject)
      {
      if (getIndex() == otherKnownObject->getIndex())
         return this; // Provably the same object.  Return "this" because it is no stricter than "other"
      else
         return NULL; // Provably different objects, so now we know nothing
      }
   else if (otherConstString)
      {
      // TODO:
      // - neither of known object and const string is necessarily weaker,
      // - we should be looking at the string contents,
      // - even if they don't match, we may have, e.g. fixed class String.
      TR::KnownObjectTable *knot = vp->comp()->getKnownObjectTable();
      TR_ASSERT(knot, "Can't create a TR_VPKnownObject without a known-object table");
      if (getIndex() == knot->getExistingIndexAt((uintptrj_t*)otherConstString->getSymRef()->getSymbol()->castToStaticSymbol()->getStaticAddress()))
         {
         // Now we're in an interesting position: which is stronger?  A
         // TR_VPConstString, or a TR_VPKnownObject on the same object?
         // TR_VPConstString doesn't know about object identity--only
         // contents--and there's a lot of VP machinery that kicks in for
         // strings, so that's probably better.
         //
         // HOWEVER, this has always returned the known object constraint,
         // so let's not change it just yet...
         //
         return this;
         }
      else
         {
         // Provably different objects; now we know nothing
         return NULL;
         }
      }

   // If the known object info tells us nothing, do what we can based on type info.
   //
   return Super::merge1(other, vp);
   }

TR_VPConstraint *TR_VPConstString::merge1(TR_VPConstraint *other, TR_ValuePropagation *vp)
   {
   TRACER(vp, this, other);

   if (other->asConstString())
      return NULL; // can't be the same string

   TR_VPResolvedClass *otherClass = other->asResolvedClass();
   if (otherClass)
      {
      if (otherClass->getClass() == getClass())
         return other;
      }
   return NULL;
   }

TR_VPConstraint *TR_VPArrayInfo::merge1(TR_VPConstraint *other, TR_ValuePropagation *vp)
   {
   TRACER(vp, this, other);

   TR_VPArrayInfo *otherInfo = other->asArrayInfo();
   if (!otherInfo)
      return NULL;

   int32_t lowBound = _lowBound;
   int32_t highBound = _highBound;
   int32_t elementSize = _elementSize;
   if (otherInfo->_lowBound < lowBound)
      lowBound = otherInfo->_lowBound;
   if (otherInfo->_highBound > highBound)
      highBound = otherInfo->_highBound;
   if (otherInfo->_elementSize != elementSize)
      elementSize = 0;
   if (lowBound == 0 && highBound == TR::getMaxSigned<TR::Int32>() && elementSize == 0)
      return NULL;
   return TR_VPArrayInfo::create(vp, lowBound, highBound, elementSize);
   }

TR_VPConstraint *TR_VPObjectLocation::merge1(TR_VPConstraint *other, TR_ValuePropagation *vp)
   {
   TRACER(vp, this, other);

   TR_VPObjectLocation *otherInfo = other->asObjectLocation();
   if (!otherInfo)
      return NULL;

   if (_kind == otherInfo->_kind)
      return this;

   // for now just extend to join distinct locations where they're
   // both subsets of ClassObject, i.e. in cases where previously they would
   // have both been ClassObject. However, most (or all?) combinations have a
   // natural join. Possibly TODO
   if (isKindSubset(_kind, ClassObject) && isKindSubset(otherInfo->_kind, ClassObject))
      return TR_VPObjectLocation::create(vp, ClassObject);

   return NULL;
   }

TR_VPConstraint *TR_VPSync::merge1(TR_VPConstraint *other, TR_ValuePropagation *vp)
   {
   TRACER(vp, this, other);

   TR_VPSync *otherSync = other->asVPSync();
   if (!otherSync)
      return NULL;

   if (otherSync->syncEmitted() == TR_no)
      return other;
   else if (syncEmitted() == TR_no)
      return this;
   return this;
   }

TR_VPConstraint *TR_VPMergedConstraints::merge1(TR_VPConstraint *other, TR_ValuePropagation *vp)
   {
   TRACER(vp, this, other);

   TR_VPConstraint *otherCur;
   ListElement<TR_VPConstraint> *otherNext;

   TR_VPMergedConstraints *otherList = other->asMergedConstraints();
   if (otherList)
      {
      otherNext = otherList->_constraints.getListHead();
      otherCur = otherNext->getData();
      otherNext = otherNext->getNextElement();
      }
   else
      {
      otherCur = other;
      otherNext = NULL;
      }

   if (_type.isInt16())
       return shortMerge(otherCur,otherNext,vp);
   if (_type.isInt32())
      return intMerge(otherCur, otherNext, vp);
   if (_type.isInt64())
      return longMerge(otherCur, otherNext, vp);

   return NULL;
   }

TR_VPConstraint *TR_VPMergedConstraints::shortMerge(TR_VPConstraint * other, ListElement<TR_VPConstraint> *otherNext, TR_ValuePropagation * vp)
   {
   TR_VPShortConstraint *otherCur = other->asShortConstraint();

   TR_ScratchList<TR_VPConstraint>  result (vp->trMemory());
   ListElement <TR_VPConstraint> *  next   = _constraints.getListHead();
   TR_VPShortConstraint          *  cur    = next->getData()->asShortConstraint();
   ListElement<TR_VPConstraint>  *  lastResultEntry = NULL;
   TR_VPConstraint               *  mergeResult;

   if (otherCur)
      {
      next = next->getNextElement();
      while (cur || otherCur)
         {
         if (lastResultEntry &&
             lastResultEntry->getData()->asShortConstraint())
            {
            // Merge the last result entry with cur and/or otherCur
            //
            TR_VPShortConstraint *lastResult = lastResultEntry->getData()->asShortConstraint();
            if (cur && (!otherCur || cur->getLow() <= otherCur->getLow()))
               {
               if (lastResult->getHigh() == TR::getMaxSigned<TR::Int16>() || cur->getLow() <= lastResult->getHigh()+1)
                  {
                  mergeResult = lastResult->merge(cur, vp);
                  if (!mergeResult)
                     return NULL;
                  lastResultEntry->setData(mergeResult);
                  }
               else
                  {
                  lastResultEntry = result.addAfter(cur, lastResultEntry);
                  }
               if (next)
                  {
                  cur = next->getData()->asShortConstraint();
                  TR_ASSERT(cur, "Expecting short constraints in shortMerge");
                  next = next->getNextElement();
                  }
               else
                  cur = NULL;
               }
            else
               {
               if (lastResult->getHigh() == TR::getMaxSigned<TR::Int16>() || otherCur->getLow() <= lastResult->getHigh()+1)
                  {
                  mergeResult = lastResult->merge(otherCur, vp);
                  if (!mergeResult)
                     return NULL;
                  lastResultEntry->setData(mergeResult);
                  }
               else
                  {
                  lastResultEntry = result.addAfter(otherCur, lastResultEntry);
                  }
               if (otherNext)
                  {
                  otherCur = otherNext->getData()->asShortConstraint();
                  TR_ASSERT(otherCur, "Expecting short constraints in shortMerge");
                  otherNext = otherNext->getNextElement();
                  }
               else
                  otherCur = NULL;
               }
            }
         else
            {
            // Put the lower of cur and otherCur into the result list
            //
            if (cur && (!otherCur || cur->getLow() <= otherCur->getLow()))
               {
               lastResultEntry = result.add(cur);
               if (next)
                  {
                  cur = next->getData()->asShortConstraint();
                  TR_ASSERT(cur, "Expecting short constraints in shortMerge");
                  next = next->getNextElement();
                  }
               else
                  cur = NULL;
               }
            else
               {
               lastResultEntry = result.add(otherCur);
               if (otherNext)
                  {
                  otherCur = otherNext->getData()->asShortConstraint();
                  TR_ASSERT(otherCur, "Expecting short constraints in shortMerge");
                  otherNext = otherNext->getNextElement();
                  }
               else
                  otherCur = NULL;
               }
            }
         }

      lastResultEntry = result.getListHead();
      if (!lastResultEntry->getNextElement())
         return lastResultEntry->getData();
      return TR_VPMergedConstraints::create(vp, lastResultEntry);
      }
    else
      {
       TR_ASSERT(false, "Merging short with another type");
      }

   return NULL;
   }

TR_VPConstraint *TR_VPMergedConstraints::intMerge(TR_VPConstraint *other, ListElement<TR_VPConstraint> *otherNext, TR_ValuePropagation *vp)
   {
   TR_VPIntConstraint *otherCur = other->asIntConstraint();
   //if (otherCur && otherCur->isUnsigned())
   //   return intMerge(otherCur, otherNext, vp, true);

   TR_ScratchList<TR_VPConstraint>         result(vp->trMemory());
   ListElement<TR_VPConstraint> *next = _constraints.getListHead();
   TR_VPIntConstraint           *cur  = next->getData()->asIntConstraint();
   ListElement<TR_VPConstraint> *lastResultEntry = NULL;
   TR_VPConstraint              *mergeResult;


   //TR_ASSERT(cur && otherCur, "Expecting int constraints in intMerge");

   if (otherCur)
      {
      next = next->getNextElement();
      while (cur || otherCur)
         {
         if (lastResultEntry &&
             lastResultEntry->getData()->asIntConstraint())
            {
            // Merge the last result entry with cur and/or otherCur
            //
            TR_VPIntConstraint *lastResult = lastResultEntry->getData()->asIntConstraint();
            if (cur && (!otherCur || cur->getLow() <= otherCur->getLow()))
               {
               if (lastResult->getHigh() == TR::getMaxSigned<TR::Int32>() || cur->getLow() <= lastResult->getHigh()+1)
                  {
                  mergeResult = lastResult->merge(cur, vp);
                  if (!mergeResult)
                     return NULL;
                  lastResultEntry->setData(mergeResult);
                  }
               else
                  {
                  lastResultEntry = result.addAfter(cur, lastResultEntry);
                  }
               if (next)
                  {
                  cur = next->getData()->asIntConstraint();
                  TR_ASSERT(cur, "Expecting int constraints in intMerge");
                  next = next->getNextElement();
                  }
               else
                  cur = NULL;
               }
            else
               {
               if (lastResult->getHigh() == TR::getMaxSigned<TR::Int32>() || otherCur->getLow() <= lastResult->getHigh()+1)
                  {
                  mergeResult = lastResult->merge(otherCur, vp);
                  if (!mergeResult)
                     return NULL;
                  lastResultEntry->setData(mergeResult);
                  }
               else
                  {
                  lastResultEntry = result.addAfter(otherCur, lastResultEntry);
                  }
               if (otherNext)
                  {
                  otherCur = otherNext->getData()->asIntConstraint();
                  TR_ASSERT(otherCur, "Expecting int constraints in intMerge");
                  otherNext = otherNext->getNextElement();
                  }
               else
                  otherCur = NULL;
               }
            }
         else
            {
            // Put the lower of cur and otherCur into the result list
            //
            if (cur && (!otherCur || cur->getLow() <= otherCur->getLow()))
               {
               lastResultEntry = result.add(cur);
               if (next)
                  {
                  cur = next->getData()->asIntConstraint();
                  TR_ASSERT(cur, "Expecting int constraints in intMerge");
                  next = next->getNextElement();
                  }
               else
                  cur = NULL;
               }
            else
               {
               lastResultEntry = result.add(otherCur);
               if (otherNext)
                  {
                  otherCur = otherNext->getData()->asIntConstraint();
                  TR_ASSERT(otherCur, "Expecting int constraints in intMerge");
                  otherNext = otherNext->getNextElement();
                  }
               else
                  otherCur = NULL;
               }
            }
         }

      lastResultEntry = result.getListHead();
      if (!lastResultEntry->getNextElement())
         return lastResultEntry->getData();
      return TR_VPMergedConstraints::create(vp, lastResultEntry);
      }

   else
      {
      // merging an int with a long
      TR_VPLongConstraint *otherCur = other->asLongConstraint();
      if (otherCur)
         {
         next = next->getNextElement();
         while (cur || otherCur)
            {
            if (lastResultEntry)
               {
               // Merge the last result entry with cur and/or otherCur
               //
               TR_VPIntConstraint *lastResult = lastResultEntry->getData()->asIntConstraint();
               if (cur && (!otherCur || (int64_t)cur->getLow() <= (int64_t)otherCur->getLow()))
                  {
                  if (lastResult->getHigh() == TR::getMaxSigned<TR::Int32>() || cur->getLow() <= lastResult->getHigh()+1)
                     {
                     mergeResult = lastResult->merge(cur, vp);
                     if (!mergeResult)
                        return NULL;
                     lastResultEntry->setData(mergeResult);
                     }
                  else
                     {
                     lastResultEntry = result.addAfter(cur, lastResultEntry);
                     }
                  if (next)
                     {
                     cur = next->getData()->asIntConstraint();
                     TR_ASSERT(cur, "Expecting int constraints in intMerge");
                     next = next->getNextElement();
                     }
                  else
                     cur = NULL;
                  }
               else
                  {
                  if (lastResult->getHigh() == TR::getMaxSigned<TR::Int32>() || (int64_t)otherCur->getLow() <= (int64_t)lastResult->getHigh()+1)
                     {
                     mergeResult = lastResult->merge(otherCur, vp);
                     if (!mergeResult)
                        return NULL;
                     lastResultEntry->setData(mergeResult);
                     }
                  else
                     {
                     // check for the possibility of a wrap-around
                     if ((int64_t)otherCur->getLow() < (int64_t)TR::getMinSigned<TR::Int32>() ||
                         (int64_t)otherCur->getHigh() > (int64_t)TR::getMaxSigned<TR::Int32>())
                        return NULL;
                     else
                        lastResultEntry = result.addAfter(TR_VPIntRange::create(vp, (int32_t)otherCur->getLow(), (int32_t)otherCur->getHigh()), lastResultEntry);
                     }
                  if (otherNext)
                     {
                     otherCur = otherNext->getData()->asLongConstraint();
                     //TR_ASSERT(otherCur, "Expecting int constraints in intMerge");
                     otherNext = otherNext->getNextElement();
                     }
                  else
                     otherCur = NULL;
                  }
               }
            else
               {
               // Put the lower of cur and otherCur into the result list
               //
               if (cur && (!otherCur || (int64_t)cur->getLow() <= (int64_t)otherCur->getLow()))
                  {
                  lastResultEntry = result.add(cur);
                  if (next)
                     {
                     cur = next->getData()->asIntConstraint();
                     TR_ASSERT(cur, "Expecting int constraints in intMerge");
                     next = next->getNextElement();
                     }
                  else
                     cur = NULL;
                  }
               else
                  {
                  // check for the possibility of a wrap-around
                  if ((int64_t)otherCur->getLow() < (int64_t)TR::getMinSigned<TR::Int32>() ||
                      (int64_t)otherCur->getHigh() > (int64_t)TR::getMaxSigned<TR::Int32>())
                     return NULL;
                  else
                     lastResultEntry = result.add(TR_VPIntRange::create(vp, (int32_t)otherCur->getLow(), (int32_t)otherCur->getHigh()));
                  if (otherNext)
                     {
                     otherCur = otherNext->getData()->asLongConstraint();
                     TR_ASSERT(otherCur, "Expecting int constraints in intMerge");
                     otherNext = otherNext->getNextElement();
                     }
                  else
                     otherCur = NULL;
                  }
               }
            }

         lastResultEntry = result.getListHead();
         if (!lastResultEntry->getNextElement())
            return lastResultEntry->getData();
         return TR_VPMergedConstraints::create(vp, lastResultEntry);
         }
      }
   return NULL;
   }

TR_VPConstraint *TR_VPMergedConstraints::longMerge(TR_VPConstraint *other, ListElement<TR_VPConstraint> *otherNext, TR_ValuePropagation *vp)
   {
   TR_ScratchList<TR_VPConstraint>         result(vp->trMemory());
   ListElement<TR_VPConstraint> *next = _constraints.getListHead();
   TR_VPLongConstraint          *cur  = next->getData()->asLongConstraint();
   ListElement<TR_VPConstraint> *lastResultEntry = NULL;
   TR_VPConstraint              *mergeResult;

   TR_VPLongConstraint *otherCur = other->asLongConstraint();

   TR_ASSERT(cur && otherCur, "Expecting long constraints in longMerge");
   if (otherCur)
      {
      next = next->getNextElement();
      while (cur || otherCur)
         {
         if (lastResultEntry)
            {
            // Merge the last result entry with cur and/or otherCur
            //
            TR_VPLongConstraint *lastResult = lastResultEntry->getData()->asLongConstraint();
            if (cur && (!otherCur || cur->getLow() <= otherCur->getLow()))
               {
               if (lastResult->getHigh() == TR::getMaxSigned<TR::Int64>() || cur->getLow() <= lastResult->getHigh()+1)
                  {
                  mergeResult = lastResult->merge(cur, vp);
                  if (!mergeResult)
                     return NULL;
                  lastResultEntry->setData(mergeResult);
                  }
               else
                  {
                  lastResultEntry = result.addAfter(cur, lastResultEntry);
                  }
               if (next)
                  {
                  cur = next->getData()->asLongConstraint();
                  TR_ASSERT(cur, "Expecting long constraints in longMerge");
                  next = next->getNextElement();
                  }
               else
                  cur = NULL;
               }
            else
               {
               if (lastResult->getHigh() == TR::getMaxSigned<TR::Int64>() || otherCur->getLow() <= lastResult->getHigh()+1)
                  {
                  mergeResult = lastResult->merge(otherCur, vp);
                  if (!mergeResult)
                     return NULL;
                  lastResultEntry->setData(mergeResult);
                  }
               else
                  {
                  lastResultEntry = result.addAfter(otherCur, lastResultEntry);
                  }
               if (otherNext)
                  {
                  otherCur = otherNext->getData()->asLongConstraint();
                  TR_ASSERT(otherCur, "Expecting long constraints in longMerge");
                  otherNext = otherNext->getNextElement();
                  }
               else
                  otherCur = NULL;
               }
            }
         else
            {
            // Put the lower of cur and otherCur into the result list
            //
            if (cur && (!otherCur || cur->getLow() <= otherCur->getLow()))
               {
               lastResultEntry = result.add(cur);
               if (next)
                  {
                  cur = next->getData()->asLongConstraint();
                  TR_ASSERT(cur, "Expecting long constraints in longMerge");
                  next = next->getNextElement();
                  }
               else
                  cur = NULL;
               }
            else
               {
               lastResultEntry = result.add(otherCur);
               if (otherNext)
                  {
                  otherCur = otherNext->getData()->asLongConstraint();
                  TR_ASSERT(otherCur, "Expecting long constraints in longMerge");
                  otherNext = otherNext->getNextElement();
                  }
               else
                  otherCur = NULL;
               }
            }
         }

      lastResultEntry = result.getListHead();
      if (!lastResultEntry->getNextElement())
         return lastResultEntry->getData();
      return TR_VPMergedConstraints::create(vp, lastResultEntry);
      }
   else
      {
      // merging a long with an int
      TR_VPIntConstraint *otherCur = other->asIntConstraint();
      if (otherCur)
         {
         next = next->getNextElement();
         while (cur || otherCur)
            {
            if (lastResultEntry)
               {
               // Merge the last result entry with cur and/or otherCur
               //
               TR_VPLongConstraint *lastResult = lastResultEntry->getData()->asLongConstraint();
               if (cur && (!otherCur || (int64_t)cur->getLow() <= (int64_t)otherCur->getLow()))
                  {
                  if (lastResult->getHigh() == TR::getMaxSigned<TR::Int64>() || cur->getLow() <= lastResult->getHigh()+1)
                     {
                     mergeResult = lastResult->merge(cur, vp);
                     if (!mergeResult)
                        return NULL;
                     lastResultEntry->setData(mergeResult);
                     }
                  else
                     {
                     lastResultEntry = result.addAfter(cur, lastResultEntry);
                     }
                  if (next)
                     {
                     cur = next->getData()->asLongConstraint();
                     TR_ASSERT(cur, "Expecting long constraints in longMerge");
                     next = next->getNextElement();
                     }
                  else
                     cur = NULL;
                  }
               else
                  {
                  int64_t otherLow = (int64_t)otherCur->getLow();
                  int64_t otherHigh = (int64_t)otherCur->getHigh();
                  TR_VPLongConstraint *otherCurLong = TR_VPLongRange::create(vp, otherLow, otherHigh);
                  if (lastResult->getHigh() == TR::getMaxSigned<TR::Int64>() || otherLow <= (int64_t)lastResult->getHigh()+1)
                     {
                     mergeResult = lastResult->merge(otherCurLong, vp);
                     if (!mergeResult)
                        return NULL;
                     lastResultEntry->setData(mergeResult);
                     }
                  else
                     {
                     lastResultEntry = result.addAfter(otherCurLong, lastResultEntry);
                     }
                  if (otherNext)
                     {
                     otherCur = otherNext->getData()->asIntConstraint();
                     //TR_ASSERT(otherCur, "Expecting long constraints in longMerge");
                     otherNext = otherNext->getNextElement();
                     }
                  else
                     otherCur = NULL;
                  }
               }
            else
               {
               // Put the lower of cur and otherCur into the result list
               //
               if (cur && (!otherCur || (int64_t)cur->getLow() <= (int64_t)otherCur->getLow()))
                  {
                  lastResultEntry = result.add(cur);
                  if (next)
                     {
                     cur = next->getData()->asLongConstraint();
                     TR_ASSERT(cur, "Expecting long constraints in longMerge");
                     next = next->getNextElement();
                     }
                  else
                     cur = NULL;
                  }
               else
                  {
                  lastResultEntry = result.add(TR_VPLongRange::create(vp, (int64_t)otherCur->getLow(), (int64_t)otherCur->getHigh()));
                  if (otherNext)
                     {
                     otherCur = otherNext->getData()->asIntConstraint();
                     //TR_ASSERT(otherCur, "Expecting long constraints in longMerge");
                     otherNext = otherNext->getNextElement();
                     }
                  else
                     otherCur = NULL;
                  }
               }
            }

         lastResultEntry = result.getListHead();
         if (!lastResultEntry->getNextElement())
            return lastResultEntry->getData();
         return TR_VPMergedConstraints::create(vp, lastResultEntry);
         }
      }
   return NULL;
   }

TR_VPConstraint *TR_VPLessThanOrEqual::merge1(TR_VPConstraint *other, TR_ValuePropagation *vp)
   {
   TRACER(vp, this, other);

   TR_VPLessThanOrEqual *otherLE = other->asLessThanOrEqual();
   if (otherLE)
      {
      if (otherLE->increment() < increment())
         return this;
      return other;
      }
   return NULL;
   }

TR_VPConstraint *TR_VPGreaterThanOrEqual::merge1(TR_VPConstraint *other, TR_ValuePropagation *vp)
   {
   TRACER(vp, this, other);

   TR_VPGreaterThanOrEqual *otherGE = other->asGreaterThanOrEqual();
   if (otherGE)
      {
      if (otherGE->increment() > increment())
         return this;
      return other;
      }
   return NULL;
   }

TR_VPConstraint *TR_VPEqual::merge1(TR_VPConstraint *other, TR_ValuePropagation *vp)
   {
   TRACER(vp, this, other);

   TR_VPGreaterThanOrEqual *otherGE = other->asGreaterThanOrEqual();
   if (otherGE)
      {
      if (otherGE->increment() <= increment())
         return other;
      return NULL;
      }
   TR_VPLessThanOrEqual *otherLE = other->asLessThanOrEqual();
   if (otherLE)
      {
      if (otherLE->increment() >= increment())
         return other;
      return NULL;
      }
   return NULL;
   }

// ***************************************************************************
//
// Intersecting Value Propagation Constraints (logical AND)
//
// ***************************************************************************

TR_VPConstraint * TR_VPConstraint::intersect(TR_VPConstraint *other, TR_ValuePropagation *vp)
   {
   // If this is the same constraint, just return it
   //
   if (!this || !other)
      {
      if (vp->trace())
         traceMsg(vp->comp(), "setIntersectionFailed to true because NULL constraint found this = 0x%p, other = 0x%p\n", this, other);
      vp->setIntersectionFailed(true);
      return NULL;
      }

   if ((this == other)) // && !this->asClass())
      return this;

   // Call the intersect method on the constraint with the highest merge priority
   //
   TR_VPConstraint *result;
   if (other->priority() > priority())
      result = other->intersect1(this, vp);
   else
      result = intersect1(other, vp);

   if (vp->trace() && !result)
      {
      traceMsg(vp->comp(), "\nCannot intersect constraints:\n   ");
      print(vp->comp(), vp->comp()->getOutFile());
      traceMsg(vp->comp(), "\n   ");
      other->print(vp->comp(), vp->comp()->getOutFile());
      traceMsg(vp->comp(), "\n");
      traceMsg(vp->comp(), "priority: %d; other->priority: %d\n", priority(), other->priority());
      }

   return result;
   }


TR_VPConstraint * TR_VPConstraint::intersect1(TR_VPConstraint *other, TR_ValuePropagation *vp)
   {
   TRACER(vp, this, other);

   // Default action is to generalize the constraint
   //
   return NULL;
   }

// //unsigned version of the intersect routine below
// TR_VPConstraint *TR_VPIntConstraint::intersect1(TR_VPConstraint *other, TR_ValuePropagation *vp, bool Unsigned)
//    {
//    TR_VPIntConstraint *otherInt = other->asIntConstraint();
//     if (otherInt)
//       {
//       TR_ASSERT(isUnsigned() && other->isUnsigned(), "Expecting unsigned constraints in integer intersect\n");
//       if ((uint32_t)otherInt->getLow() < (uint32_t)getLow())
//          return otherInt->intersect(this, vp);
//       if ((uint32_t)otherInt->getHigh() <= (uint32_t)getHigh())
//          return other;
//       if ((uint32_t)otherInt->getLow() <= (uint32_t)getHigh())
//          return TR_VPIntRange::create(vp, otherInt->getLow(), getHigh(), true);
//       }
//    return NULL;
//    }

TR_VPConstraint *TR_VPShortConstraint::intersect1(TR_VPConstraint * other, TR_ValuePropagation *vp)
   {
   TRACER(vp, this, other);

   TR_VPShortConstraint *otherShort = other->asShortConstraint();
   TR_VPIntConstraint *otherInt = other->asIntConstraint();
   TR_VPLongConstraint *otherLong = other->asLongConstraint();
   if (otherShort)
      {
      if (otherShort->getLow() < getLow())
         return otherShort->intersect(this, vp);
      if (otherShort->getHigh() <= getHigh())
         return other;
      if (otherShort->getLow() <= getHigh())
         return TR_VPShortRange::create(vp, otherShort->getLow(), getHigh());
      return NULL;
      }
   else if (otherInt)
      {
      int64_t lowVal, highVal;
      if ((int64_t)otherInt->getLow() < (int64_t)getLow())
         lowVal = getLow();
      else
         lowVal = otherInt->getLow();
      if ((int64_t)otherInt->getHigh() <= (int64_t)getHigh())
         highVal = otherInt->getHigh();
      else
         highVal = getHigh();

      return TR_VPShortRange::create(vp, (int16_t)lowVal, (int16_t)highVal);
      }
   else if (otherLong)
      {
      int64_t lowVal, highVal;
      if ((int64_t)otherLong->getLow() < (int64_t)getLow())
         lowVal = getLow();
      else
         lowVal = otherLong->getLow();
      if ((int64_t)otherLong->getHigh() <= (int64_t)getHigh())
         highVal = otherLong->getHigh();
      else
         highVal = getHigh();

      return TR_VPShortRange::create(vp, (int16_t)lowVal, (int16_t)highVal);
      }
   return NULL;
   }

TR_VPConstraint *TR_VPIntConstraint::intersect1(TR_VPConstraint *other, TR_ValuePropagation *vp)
   {
   TRACER(vp, this, other);

   TR_VPIntConstraint *otherInt = other->asIntConstraint();
   //if (otherInt && otherInt->isUnsigned() && isUnsigned())
   //   return intersect1(other, vp, true);

   if (otherInt)
      {
      if (otherInt->getLow() < getLow())
         return otherInt->intersect(this, vp);
      if (otherInt->getHigh() <= getHigh())
         return other;
      if (otherInt->getLow() <= getHigh())
         return TR_VPIntRange::create(vp, otherInt->getLow(), getHigh());
      return NULL;
      }
   else
      {
      // due to the presence of aladd's, ints might intersect
      // with longs. take care of that here by creating an appropriate
      // IntRange
      TR_VPLongConstraint *otherLong = other->asLongConstraint();
      if (otherLong)
         {
         int64_t lowVal, highVal;
         if ((int64_t)otherLong->getLow() < (int64_t)getLow())
            lowVal = getLow();
         else
            lowVal = otherLong->getLow();
         if ((int64_t)otherLong->getHigh() <= (int64_t)getHigh())
            highVal = otherLong->getHigh();
         else
            highVal = getHigh();

         return TR_VPLongRange::create(vp, (int32_t)lowVal, (int32_t)highVal);
         }
      }
   return NULL;
   }

TR_VPConstraint *TR_VPLongConstraint::intersect1(TR_VPConstraint *other, TR_ValuePropagation *vp)
   {
   TRACER(vp, this, other);

   TR_VPLongConstraint *otherLong = other->asLongConstraint();
   if (otherLong)
      {
      if (otherLong->getLow() < getLow())
         return otherLong->intersect(this, vp);
      if (otherLong->getHigh() <= getHigh())
         return other;
      if (otherLong->getLow() <= getHigh())
         return TR_VPLongRange::create(vp, otherLong->getLow(), getHigh());
      return NULL;
      }
   else
      {
      // see comment on aladd's above
      TR_VPIntConstraint *otherInt = other->asIntConstraint();
      if (otherInt)
         {
         if ((int64_t)otherInt->getLow() < (int64_t)getLow())
            return otherInt->intersect(this, vp);
         if ((int64_t)otherInt->getHigh() <= (int64_t)getHigh())
            return TR_VPLongRange::create(vp, getLow(), otherInt->getHigh());
         if ((int64_t)otherInt->getLow() <= (int64_t)getHigh())
            {
            if ((int64_t)getHigh() > (int64_t)TR::getMaxSigned<TR::Int32>())
               return TR_VPLongRange::create(vp, otherInt->getLow(), TR::getMaxSigned<TR::Int32>());
            else
               return TR_VPLongRange::create(vp, otherInt->getLow(), getHigh());
            }
         return NULL;
         }
      }
   return NULL;
   }


TR_VPClassType *TR_VPClassType::classTypesCompatible(TR_VPClassType * otherType, TR_ValuePropagation *vp)
   {
   TR_VPResolvedClass *rc = asResolvedClass();
   TR_VPResolvedClass *otherRc = otherType->asResolvedClass();
   if (rc && otherRc && !rc->isFixedClass() && !otherRc->isFixedClass())
      {
      TR_OpaqueClassBlock *c1 = rc->getClass();
      TR_OpaqueClassBlock *c2 = otherRc->getClass();

      if (!TR::Compiler->cls.isInterfaceClass(vp->comp(), c1) && !TR::Compiler->cls.isInterfaceClass(vp->comp(), c2))
         {
         TR_YesNoMaybe ans = vp->fe()->isInstanceOf(c1, c2, false, false, true);
         if (ans == TR_no)
            return NULL;

         ans = vp->fe()->isInstanceOf(c1, c2, true, true, true);

         if (ans == TR_yes)
            return this;

         ans = vp->fe()->isInstanceOf(c2, c1, true, true, true);

         if (ans == TR_yes)
            return otherType;
         }

      return this;
      }
   else
      {
      return (TR_VPClassType*)this->intersect(otherType, vp);
      }
   }




// this routine encapsulates code that used to exist
// in VPClass::intersect
// it is called directly by handlers for instanceOf, checkCast
void TR_VPClass::typeIntersect(TR_VPClassPresence* &presence, TR_VPClassType* &type, TR_VPConstraint *other, TR_ValuePropagation *vp)
   {
   if (type && TR_VPConstraint::isSpecialClass((uintptrj_t)type->getClass()))
      type = NULL;

   if (other->asClass())
      {
      TR_VPClass *otherClass = other->asClass();
      bool classTypeFound = false;
      bool constrainPresence = true;
      if (_presence)
         {
         if (otherClass->_presence)
            {
            presence = (TR_VPClassPresence*)_presence->intersect(otherClass->_presence, vp);
            if (!presence)
               constrainPresence = false;
            }
         }
      else
         presence = otherClass->_presence;

      if (!constrainPresence)
         return;

      if (presence && presence->isNullObject())
         return;

      if (otherClass->_type && TR_VPConstraint::isSpecialClass((uintptrj_t)otherClass->_type->getClass()))
         type = NULL;
      else if (type)
         {
         // any type intersecting with the specialClass will result
         // in no type
         if (TR_VPConstraint::isSpecialClass((uintptrj_t)type->getClass()))
            type = NULL;
         else if (otherClass->_type)
            {
            TR_VPClassType *otherType = otherClass->_type;
            ///if (otherType && otherType->isClassObject() == TR_yes)
            ///   dumpOptDetails(vp->comp(), "type is classobject: %d\n", TR_yes);
            //
            // the following cases are due to the fact that for loadaddrs
            // and ialoads<vft-symbol> vp keeps track of the underlying type ;
            // but these are actually of type java/lang/Class. when confronted
            // with the intersection between a loadaddr (whose underlying type is A)
            // and an aload (whose actual type is java/lang/Class), vp cannot intersect
            // types (when it should succeed) causing it to do wrong things
            // like fold branches etc. to detect this scenario, loadaddrs and
            // iaload<vft-symbols> are primed with a ClassObject property.
            //
            // case 1: VPClass wrapper intersects with a VPClass wrapper (i)
            //e.g. <fixedClass, classObject> with <resolvedClass, non-null>
            if (_location && _location->isClassObject() == TR_yes)
               {
               if (otherType->asResolvedClass())
                  {
                  TR_VPResolvedClass *rc = otherType->asResolvedClass();
                  if (rc->getClass() == vp->fe()->getClassClassPointer(rc->getClass()))
                     {
                     if (vp->trace())
                        {
                        traceMsg(vp->comp(), "   1Intersecting type is a class object\n");
                        otherType->print(vp->comp(), vp->comp()->getOutFile());
                        traceMsg(vp->comp(), "\n");
                        }

                     // this means otherType could be a java/lang/Class
                     // actual intersection of the types below is going to fail & return
                     // NULL because _type cannot be an instanceOf java/lang/Class
                     // (since java/lang/Class is final) ; so type becomes otherType
                     //
                     // addendum: dont propagate the type, since the constraint can
                     // be identified using the classobject property anyway
                     ///type = otherType;
                     classTypeFound = true;
                     }
                  //case 2: VPClass wrapper intersects with a VPClass wrapper (ii)
                  //e.g. <fixedClass, classObject> with <resolvedClass, classObject>
                  else if (otherClass->_location && otherClass->_location->isClassObject() == TR_yes)
                     {
                     TR_VPResolvedClass *rc = type->asResolvedClass();
                     if (rc && rc->getClass() == vp->fe()->getClassClassPointer(rc->getClass()))
                        {
                        if (vp->trace())
                           {
                           traceMsg(vp->comp(), "   Current type is a class object\n");
                           this->print(vp->comp(), vp->comp()->getOutFile());
                           traceMsg(vp->comp(), "\n");
                           }
                        // resulting type is this type
                        //////type = _type;
                        classTypeFound = true;
                        }
                     }
                  }
               }
            //case 3: VPClass wrapper intersects with a VPClass wrapper (iii)
            //e.g. <fixedClass, non-null> with <resolvedClass, classObject>
            else if (otherClass->_location && otherClass->_location->isClassObject() == TR_yes)
               {
               TR_VPResolvedClass *rc = type->asResolvedClass();
               if (rc && rc->getClass() == vp->fe()->getClassClassPointer(rc->getClass()))
                  {
                  if (vp->trace())
                     {
                     traceMsg(vp->comp(), "   2Intersecting type is a class object\n");
                     this->print(vp->comp(), vp->comp()->getOutFile());
                     traceMsg(vp->comp(), "\n");
                     }
                  // resulting type is this type
                  ///////type = _type;
                  classTypeFound = true;
                  }
               }
            // now do the actual type intersection
            if (!classTypeFound)
               {
               type = type->classTypesCompatible(otherType, vp);
               }
            }
         }
      else
         type = otherClass->_type;
      }
   else if (other->asClassPresence())
      {
      TR_VPClassPresence *otherPresence = other->asClassPresence();
      if (_presence)
         {
         presence = (TR_VPClassPresence*)_presence->intersect(otherPresence, vp);
         }
      else
         presence = otherPresence;
      }
   else if (other->asClassType())
      {
      bool classTypeFound = false;
      TR_VPClassType *otherType = other->asClassType();
      if (TR_VPConstraint::isSpecialClass((uintptrj_t)otherType->getClass()))
         type = NULL;
      else if (type)
         {
         ///if (otherType && otherType->isClassObject() == TR_yes)
         ///   dumpOptDetails(vp->comp(), "type is classobject: %d\n", TR_yes);
         if (TR_VPConstraint::isSpecialClass((uintptrj_t)type->getClass()))
            {
            type = NULL;
            classTypeFound = true;
            }
         //
         //case 4: VPClass wrapper intersects with a VPClassType
         //e.g. <fixedClass, classObject> with <resolvedClass>
         else if (_location && _location->isClassObject() == TR_yes)
            {
            if (otherType && otherType->asResolvedClass())
               {
               TR_VPResolvedClass *rc = otherType->asResolvedClass();
               if (rc->getClass() == vp->fe()->getClassClassPointer(rc->getClass()))
                  {
                  if (vp->trace())
                     {
                     traceMsg(vp->comp(), "   Intersecting type is a class object\n");
                     otherType->print(vp->comp(), vp->comp()->getOutFile());
                     traceMsg(vp->comp(), "\n");
                     }
                  // resulting type is the otherType
                  //
                  // addendum: dont propagate the type, since the constraint can
                  // be identified using the classobject property anyway
                  ///type = otherType;
                  classTypeFound = true;
                  }
               }
            }
         if (!classTypeFound)
            type = type->classTypesCompatible(otherType, vp);
         }
      else
         type = otherType;
      }
   }

TR_VPConstraint *TR_VPClass::intersect1(TR_VPConstraint *other, TR_ValuePropagation *vp)
   {
   TRACER(vp, this, other);

   TR_VPClassType         *type          = _type;
   TR_VPClassPresence     *presence      = _presence;
   TR_VPPreexistentObject *preexistence  = _preexistence;
   TR_VPArrayInfo         *arrayInfo     = _arrayInfo;
   TR_VPObjectLocation    *location      = _location;

   const bool thisClassObjectYes = isClassObject() == TR_yes;
   const bool otherClassObjectYes = other->isClassObject() == TR_yes;
   if (thisClassObjectYes != otherClassObjectYes)
      {
      // One is known to be a class object and the other isn't, so don't try to
      // intersect the types (which have different meanings). The intersection
      // is a class of some kind (if nonempty).
      TR_VPConstraint *classObj = this, *nonClassObj = other;
      if (otherClassObjectYes)
         std::swap(classObj, nonClassObj);

      TR_VPObjectLocation *classLoc = classObj->getObjectLocation();
      TR_ASSERT(classLoc != NULL, "classObj has no location\n");
      if (classObj->isNullObject()
          || nonClassObj->isNullObject()
          || nonClassObj->isClassObject() == TR_no
          || (classLoc->isJavaLangClassObject() == TR_yes && nonClassObj->isJavaLangClassObject() == TR_no)
          || (classLoc->isJ9ClassObject() == TR_yes && nonClassObj->isJ9ClassObject() == TR_no))
         {
         // Referent can only be null.
         if (isNonNullObject() || other->isNonNullObject())
            return NULL;
         else
            return TR_VPNullObject::create(vp);
         }

      // At this point we know the nonClassObj permits classLoc.
      location = classLoc;

      // classLoc means that any associated TR_VPClass constrains the
      // represented class, about which nonClassObj has no information.
      // Make sure to avoid propagating the "special class" constraint.
      type = classObj->getClassType();
      if (type != NULL && isSpecialClass((uintptrj_t)type->getClass()))
         type = NULL;

      arrayInfo = NULL; // doesn't apply to classes
      presence = NULL;
      if (isNonNullObject() || other->isNonNullObject())
         presence = TR_VPNonNullObject::create(vp);
      }
   else if (other->asClass())
      {
      TR_VPClass *otherClass = other->asClass();
      typeIntersect(presence, type, otherClass, vp);
      // If intersection of presence failed, return null
      if (!presence && _presence && otherClass->_presence)
         return NULL;
      // If the resulting object is null, return
      //
      else if (presence && presence->isNullObject())
         return presence;
      // If intersection of types failed, return null
      if (!type && _type && otherClass->_type)
         {
         if (TR_VPConstraint::isSpecialClass((uintptrj_t)_type->getClass()) ||
               TR_VPConstraint::isSpecialClass((uintptrj_t)otherClass->_type->getClass()))
            ; // do nothing
         else if ((_presence && _presence->isNonNullObject()) || (other->asClassPresence() && other->asClassPresence()->isNonNullObject()))
            return NULL;
         else
            return TR_VPNullObject::create(vp);
         }

      if (_preexistence)
         preexistence = _preexistence;
      else
         preexistence = otherClass->_preexistence;

      if (_arrayInfo)
         {
         if (otherClass->_arrayInfo)
            {
            arrayInfo = (TR_VPArrayInfo*)_arrayInfo->intersect(otherClass->_arrayInfo, vp);
            if (!arrayInfo)
               return NULL;
            }
         }
      else
         arrayInfo = otherClass->_arrayInfo;

      if (_location)
         {
         if (otherClass->_location)
            {
            location = (TR_VPObjectLocation *)_location->intersect(otherClass->_location, vp);
            if (!location)
               return NULL;
            }
         }
      else
         location = otherClass->_location;
      }

   else if (other->asClassType())
      {
      TR_VPClassType *otherType = other->asClassType();
      // We get the normal interpretation of _type (type of an instance).
      TR_VPClassPresence *p = NULL;
      typeIntersect(p, type, other, vp);
      // If intersection of types failed, return null
      if (!type && _type && otherType)
         {
         if (TR_VPConstraint::isSpecialClass((uintptrj_t)_type->getClass()) ||
               TR_VPConstraint::isSpecialClass((uintptrj_t)otherType->getClass()))
            ; // do nothing
         else if (_presence && _presence->isNonNullObject())
            return NULL;
         else
            return TR_VPNullObject::create(vp);
         }

      // code below moved to typeIntersect
#if 0
      if (_type)
         {
         ///if (otherType && otherType->isClassObject() == TR_yes)
         ///   dumpOptDetails(vp->comp(), "type is classobject: %d\n", TR_yes);
         //case 4: VPClass intersects with a VPClassType
         //e.g. <fixedClass,classObject> with <resolvedClass>
         if (_location && _location->isClassObject() == TR_yes)
            {
            if (otherType && otherType->asResolvedClass())
               {
               TR_VPResolvedClass *rc = otherType->asResolvedClass();
               if (rc->getClass() == vp->fe()->getClassClassPointer(rc->getClass()))
                  {
                  if (vp->trace())
                     {
                     traceMsg(vp->comp(), "   Intersecting type is a class object\n");
                     otherType->print(vp->comp(), vp->comp()->getOutFile());
                     traceMsg(vp->comp(), "\n");
                     }
                  return other;
                  }
               }
            }
         type = (TR_VPClassType*)_type->intersect(otherType, vp);
         if (!type)
            return NULL;
         }
      else
         type = otherType;
#endif
      }

   else if (other->asClassPresence())
      {
      TR_VPClassType *t = NULL;
      typeIntersect(presence, t, other, vp);
      // If intersection of presence failed, return null
      if (!presence && _presence && other->asClassPresence())
         return NULL;
      // If the result is null, just return null
      //
      if (presence && presence->isNullObject())
         return presence;

      // code below moved to typeIntersect
#if 0
      TR_VPClassPresence *otherPresence = other->asClassPresence();
      if (_presence)
         {
         presence = (TR_VPClassPresence*)_presence->intersect(otherPresence, vp);
         if (!presence)
            return NULL;
         }
      else
         presence = otherPresence;

      // If the result is null, just return null
      //
      if (presence && presence->isNullObject())
         return presence;
#endif
      }

   else if (other->asPreexistentObject())
      {
      if (!_preexistence)
         preexistence = other->asPreexistentObject();
      else if (_preexistence->getAssumptionClass() != other->asPreexistentObject()->getAssumptionClass())
         preexistence = NULL;
      }

   else if (other->asArrayInfo())
      {
      TR_VPArrayInfo *otherInfo = other->asArrayInfo();
      if (_arrayInfo)
         {
         arrayInfo = (TR_VPArrayInfo*)_arrayInfo->intersect(otherInfo, vp);
         if (!arrayInfo)
            return NULL;
         }
      else
         arrayInfo = otherInfo;
      }
   else if (other->asObjectLocation())
      {
      TR_VPObjectLocation *otherInfo = other->asObjectLocation();
      if (_location)
         {
         location = (TR_VPObjectLocation *)_location->intersect(otherInfo, vp);
         if (!location)
            return NULL;
         }
      else
         location = otherInfo;
      }
   else
      return NULL;

   if (type || presence || preexistence || arrayInfo || location)
      return TR_VPClass::create(vp, type, presence, preexistence, arrayInfo, location);
   return NULL;
   }

TR_VPConstraint *TR_VPResolvedClass::intersect1(TR_VPConstraint *other, TR_ValuePropagation *vp)
   {
   TRACER(vp, this, other);

   TR_VPResolvedClass *otherRes = other->asResolvedClass();
   if (otherRes)
      {
      TR_OpaqueClassBlock *c1 = getClass();
      TR_OpaqueClassBlock *c2 = otherRes->getClass();
      if ((vp->fe()->isInstanceOf(c2, c1, false, true, true)) == TR_yes)
         {
         return otherRes;
         }
      }
   else if (other->asUnresolvedClass())
      {
      if (isJavaLangObject(vp))
         return other;

      // Make sure they are both arrays or both classes
      //
      int32_t len = 0, thisLen, otherLen;
      const char *thisSig = getClassSignature(thisLen);
      const char *otherSig = other->getClassSignature(otherLen);
      if (*thisSig == *otherSig)
         {
         while (*thisSig == '[')
            {
            if (*otherSig != *thisSig)
               {
                 if (!((otherLen == 21 && !strncmp(otherSig, "Ljava/lang/Cloneable;", 21)) ||
                       (otherLen == 22 && !strncmp(otherSig, "Ljava/io/Serializable;", 22)) ||
                       (otherLen == 18 && !strncmp(otherSig, "Ljava/lang/Object;", 18))))
                  return NULL;
               break;
               }
            thisSig++;
            otherSig++;
            otherLen--;
            }

         if (((*thisSig != 'L') && (*thisSig != '[')) &&
             ((*otherSig == 'L') || (*otherSig == '[')))
            return NULL;

         return this;
         }
      // JLS Section 10.7: "Every array implements the interfaces Cloneable and java.io.Serializable"
      else if (*getClassSignature(len) == '[' && other->asUnresolvedClass()->isCloneableOrSerializable())
         return this;
      else if (((thisLen == 21 && !strncmp(thisSig, "Ljava/lang/Cloneable;", 21)) ||
                (thisLen == 22 && !strncmp(thisSig, "Ljava/io/Serializable;", 22))) &&
               (*otherSig == '['))
         {
         return other;
         }
          // Plumhall rtresolve failure, every array is instance of java/lang/Object
      else if (*getClassSignature(len) == '[' && other->asUnresolvedClass()->isJavaLangObject(vp))
         return this;
      return NULL;
      }
   else if (other->asClassPresence())
      {
      if (other->isNullObject())
         return other;
      return TR_VPClass::create(vp, this, other->asClassPresence(), NULL, NULL, NULL);
      }
   else if (other->asPreexistentObject())
      return TR_VPClass::create(vp, this, NULL, other->asPreexistentObject(), NULL, NULL);
   else if (other->asArrayInfo())
      return TR_VPClass::create(vp, this, NULL, NULL, other->asArrayInfo(), NULL);
   else if (other->asObjectLocation())
      {
      TR_VPObjectLocation *location = other->asObjectLocation();
      TR_YesNoMaybe classObject = isClassObject();
      if (classObject != TR_maybe)
         {
         location = TR_VPObjectLocation::create(vp, classObject == TR_yes ?
                                                TR_VPObjectLocation::ClassObject : TR_VPObjectLocation::NotClassObject);
         location = (TR_VPObjectLocation *) location->intersect(other->asObjectLocation(), vp);
         if (!location) return NULL;
         }
      return TR_VPClass::create(vp, this, NULL, NULL, NULL, location);
      }
   return this;
   }

TR_VPConstraint *TR_VPFixedClass::intersect1(TR_VPConstraint *other, TR_ValuePropagation *vp)
   {
   TRACER(vp, this, other);

   TR_VPResolvedClass *otherClass = other->asResolvedClass();
   if (otherClass)
      {
      TR_OpaqueClassBlock *c1 = getClass();
      TR_OpaqueClassBlock *c2 = otherClass->getClass();
      if (vp->fe()->isInstanceOf(c1, c2, true, true, true) != TR_no)
         return this;
      }
   else if (other->asUnresolvedClass())
      {
      // Make sure they are both arrays or both classes
      //
      int32_t len = 0, thisLen, otherLen;
      const char *thisSig = getClassSignature(thisLen);
      const char *otherSig = other->getClassSignature(otherLen);

      // if the current is a java.lang.Object
      // then the other could be a class or an array
      if (isJavaLangObject(vp))
         {
         if (*otherSig == '[')
            {
            while (*otherSig != '[')
               otherSig++;
            if (!((otherLen == 21 && !strncmp(otherSig, "Ljava/lang/Cloneable;", 21)) ||
                  (otherLen == 22 && !strncmp(otherSig, "Ljava/io/Serializable;", 22)) ||
                  (otherLen == 18 && !strncmp(otherSig, "Ljava/lang/Object;", 18))))
               return NULL;
            }
         else if (!(other->asUnresolvedClass()->isCloneableOrSerializable() ||
               other->asUnresolvedClass()->isJavaLangObject(vp)))
            return NULL;
         //return other;
         }

      if (*thisSig == *otherSig)
         {
         while (*thisSig == '[')
            {
            if (*otherSig != *thisSig)
               {
               if (!((otherLen == 21 && !strncmp(otherSig, "Ljava/lang/Cloneable;", 21)) ||
                     (otherLen == 22 && !strncmp(otherSig, "Ljava/io/Serializable;", 22)) ||
                     (otherLen == 18 && !strncmp(otherSig, "Ljava/lang/Object;", 18))))
                  return NULL;
               break;
               }
            thisSig++;
            otherSig++;
            otherLen--;
            }

         if ((*thisSig != 'L') && ((*otherSig == 'L') || (*otherSig == '[')))
            return NULL;

         // retain the fact that its a fixed type
         return this;
         //return TR_VPResolvedClass::create(vp, _class);
         }
      // JLS Section 10.7: "Every array implements the interfaces Cloneable and java.io.Serializable"
      else if (*getClassSignature(len) == '[' && other->asUnresolvedClass()->isCloneableOrSerializable())
         return this;
          // Plumhall rtresolve failure, every array is instance of java/lang/Object
      else if (*getClassSignature(len) == '[' && other->asUnresolvedClass()->isJavaLangObject(vp))
         return this;
      return NULL;
      }
   else if (other->asClassPresence())
      {
      if (other->isNullObject())
         return other;
      return TR_VPClass::create(vp, this, other->asClassPresence(), NULL, NULL, NULL);
      }
   else if (other->asPreexistentObject())
      return TR_VPClass::create(vp, this, NULL, other->asPreexistentObject(), NULL, NULL);
   else if (other->asArrayInfo())
      return TR_VPClass::create(vp, this, NULL, NULL, other->asArrayInfo(), NULL);
   else if (other->asObjectLocation())
      {
      TR_VPObjectLocation *location = other->asObjectLocation();
      TR_YesNoMaybe classObject = isClassObject();
      if (classObject != TR_maybe)
         {
         location = TR_VPObjectLocation::create(vp, classObject == TR_yes ?
                                                TR_VPObjectLocation::ClassObject : TR_VPObjectLocation::NotClassObject);
         location = (TR_VPObjectLocation *) location->intersect(other->asObjectLocation(), vp);
         if (!location) return NULL;
         }
      return TR_VPClass::create(vp, this, NULL, NULL, NULL, location);
      }
   return NULL;
   }

TR_VPConstraint *TR_VPKnownObject::intersect1(TR_VPConstraint *other, TR_ValuePropagation *vp)
   {
   TRACER(vp, this, other);

   TR_VPKnownObject *otherKnownObject = other->getKnownObject();
   if (otherKnownObject)
      {
      if (getIndex() == otherKnownObject->getIndex())
         return other; // Provably the same object.  Return "other" because it is no less strict than "this"
      else
         return NULL;  // Impossible: provably different objects
      }

   TR_VPConstString *otherConstString = other->getConstString();
   if (otherConstString)
      {
      // TODO:
      // - we should be looking at the string contents,
      // - known object should be more specific (though it allows null).
      TR::KnownObjectTable *knot = vp->comp()->getKnownObjectTable();
      if (getIndex() == knot->getIndexAt((uintptrj_t*)otherConstString->getSymRef()->getSymbol()->castToStaticSymbol()->getStaticAddress()))
         return other; // A const string constraint is more specific than known object
      else
         return NULL; // Two different objects
      }

   // If the known object info tells us nothing, do what we can based on type info.
   //
   return Super::intersect1(other, vp);
   }

TR_VPConstraint *TR_VPConstString::intersect1(TR_VPConstraint *other, TR_ValuePropagation *vp)
   {
   TRACER(vp, this, other);

   if (other->asConstString())
      return NULL; // can't be the same string

   TR_VPResolvedClass *otherClass = other->asResolvedClass();
   if (otherClass)
      {
      TR_OpaqueClassBlock *c1 = getClass();
      TR_OpaqueClassBlock *c2 = otherClass->getClass();
	if (vp->fe()->isInstanceOf(c1, c2, true, true, true) != TR_no)
         return this;
      }
   else if (other->asUnresolvedClass())
     {
      // Make sure they are both arrays or both classes
      //
      int32_t len = 0, otherLen;
      const char thisSig[] = "Ljava/lang/String;";
      const char *otherSig = other->getClassSignature(otherLen);
      if (*thisSig == *otherSig)
         return TR_VPResolvedClass::create(vp, vp->comp()->getStringClassPointer());

      return NULL;
      }
   else if (other->asClassPresence())
      {
      if (other->isNullObject() &&
          !isNonNullObject())
         return other;

      if (!other->isNullObject() ||
          !isNonNullObject())
         return this;
      }
   else if (other->asObjectLocation())
      {
      TR_VPObjectLocation *location = TR_VPObjectLocation::create(vp, TR_VPObjectLocation::HeapObject);
      location = (TR_VPObjectLocation*)location->intersect(other->asObjectLocation(), vp);
      if (!location) return NULL;
      return TR_VPClass::create(vp, this, NULL, NULL, NULL, location);
      }
   return NULL;
   }

TR_VPConstraint *TR_VPUnresolvedClass::intersect1(TR_VPConstraint *other, TR_ValuePropagation *vp)
   {
   TRACER(vp, this, other);

   if (other->asClassPresence())
      {
      if (other->isNullObject())
         return other;
      return TR_VPClass::create(vp, this, other->asClassPresence(), NULL, NULL, NULL);
      }
   else if (other->asPreexistentObject())
      return TR_VPClass::create(vp, this, NULL, other->asPreexistentObject(), NULL, NULL);
   else if (other->asArrayInfo())
      return TR_VPClass::create(vp, this, NULL, NULL, other->asArrayInfo(), NULL);
   else if (other->asObjectLocation())
      return TR_VPClass::create(vp, this, NULL, NULL, NULL, other->asObjectLocation()); // FIXME: unless rtresolve, should be on heap
   return this;
   }

TR_VPConstraint *TR_VPNullObject::intersect1(TR_VPConstraint *other, TR_ValuePropagation *vp)
   {
   TRACER(vp, this, other);

   if (isNullObject())
      return this;
   if (other->asPreexistentObject())
      return TR_VPClass::create(vp, NULL, this, other->asPreexistentObject(), NULL, NULL);
   if (other->asArrayInfo())
      return TR_VPClass::create(vp, NULL, this, NULL, other->asArrayInfo(), NULL);
   if (other->asObjectLocation())
      {
      if (other->isStackObject() == TR_yes ||
          other->isHeapObject()  == TR_yes ||
          other->isClassObject() == TR_yes)
         return NULL;
      return this; // no advantage in knowing that a null object is not on the heap, etc
      }
   return NULL;
   }

TR_VPConstraint *TR_VPNonNullObject::intersect1(TR_VPConstraint *other, TR_ValuePropagation *vp)
   {
   TRACER(vp, this, other);

   if (other->asPreexistentObject())
      return TR_VPClass::create(vp, NULL, this, other->asPreexistentObject(), NULL, NULL);
   if (other->asArrayInfo())
      return TR_VPClass::create(vp, NULL, this, NULL, other->asArrayInfo(), NULL);
   if (other->asObjectLocation())
      return TR_VPClass::create(vp, NULL, this, NULL, NULL, other->asObjectLocation());
   return NULL;
   }

TR_VPConstraint *TR_VPPreexistentObject::intersect1(TR_VPConstraint *other, TR_ValuePropagation *vp)
   {
   TRACER(vp, this, other);

   if (other->asArrayInfo())
      return TR_VPClass::create(vp, NULL, NULL, this, other->asArrayInfo(), NULL);
   if (other->asObjectLocation())
      return TR_VPClass::create(vp, NULL, NULL, this, NULL, other->asObjectLocation());
   return NULL;
   }

TR_VPConstraint *TR_VPArrayInfo::intersect1(TR_VPConstraint *other, TR_ValuePropagation *vp)
   {
   TRACER(vp, this, other);

   if (other->asObjectLocation())
      {
      TR_VPObjectLocation *location = TR_VPObjectLocation::create(vp, TR_VPObjectLocation::NotClassObject);
      location = (TR_VPObjectLocation *) location->intersect(other->asObjectLocation(), vp);
      if (!location) return NULL;
      return TR_VPClass::create(vp, NULL, NULL, NULL, this, location);
      }

   TR_VPArrayInfo *otherInfo = other->asArrayInfo();
   if (!otherInfo)
      return NULL;

   int32_t lowBound = _lowBound;
   int32_t highBound = _highBound;
   int32_t elementSize = _elementSize;
   if (otherInfo->_lowBound > lowBound)
      lowBound = otherInfo->_lowBound;
   if (otherInfo->_highBound < highBound)
      highBound = otherInfo->_highBound;
   if (otherInfo->_elementSize)
      {
      if (!elementSize)
         elementSize = otherInfo->_elementSize;
      else if (elementSize != otherInfo->_elementSize)
         return NULL;
      }
   if (lowBound == 0 && highBound == TR::getMaxSigned<TR::Int32>() && elementSize == 0)
      return NULL;
   return TR_VPArrayInfo::create(vp, lowBound, highBound, elementSize);
   }

TR_VPConstraint *TR_VPObjectLocation::intersect1(TR_VPConstraint *other, TR_ValuePropagation *vp)
   {
   TRACER(vp, this, other);

   TR_VPObjectLocation *otherInfo = other->asObjectLocation();
   if (!otherInfo)
      return NULL;

   TR_VPObjectLocationKind result =
      (TR_VPObjectLocationKind)(_kind & otherInfo->_kind);

   //FIXME: since loadaddrs (or ialoads of vft-symbols) are primed
   //with a ClassObject property, we could intersect ClassObject with a HeapObject
   //Leaving this here for now, in all cases where we previously would
   //have had ClassObject (and where we now have a subset of ClassObject).
   if ((_kind == HeapObject && isKindSubset(otherInfo->_kind, ClassObject)) ||
       (isKindSubset(_kind, ClassObject) && otherInfo->_kind == HeapObject))
      result = HeapObject;

   if (result == _kind)
      return this;
   else if (result == otherInfo->_kind)
      return otherInfo;
   else if (result == (TR_VPObjectLocationKind)0)
      return NULL;
   else
      return TR_VPObjectLocation::create(vp, result);
   }

TR_VPConstraint *TR_VPSync::intersect1(TR_VPConstraint *other, TR_ValuePropagation *vp)
   {
   TRACER(vp, this, other);

   //FIXME
   TR_VPSync *otherSync = other->asVPSync();
   if (!otherSync)
      return NULL;

   if ((syncEmitted() == TR_maybe && otherSync->syncEmitted() == TR_yes) ||
       (syncEmitted() == TR_yes && otherSync->syncEmitted() == TR_maybe))
      return TR_VPSync::create(vp, TR_no);

   if ((syncEmitted() == TR_maybe && otherSync->syncEmitted() == TR_no) ||
       (syncEmitted() == TR_no && otherSync->syncEmitted() == TR_maybe))
      return TR_VPSync::create(vp, TR_yes);

   return NULL;
   }

TR_VPConstraint *TR_VPMergedConstraints::intersect1(TR_VPConstraint *other, TR_ValuePropagation *vp)
   {
   TRACER(vp, this, other);

   TR_VPConstraint *otherCur;
   ListElement<TR_VPConstraint> *otherNext;

   TR_VPMergedConstraints *otherList = other->asMergedConstraints();
   if (otherList)
      {
      otherNext = otherList->_constraints.getListHead();
      otherCur = otherNext->getData();
      otherNext = otherNext->getNextElement();
      }
   else
      {
      otherCur = other;
      otherNext = NULL;
      }

   if (_type.isInt16())
      return shortIntersect(otherCur,otherNext,vp);
   if (_type.isInt32())
      return intIntersect(otherCur, otherNext, vp);
   if (_type.isInt64())
      return longIntersect(otherCur, otherNext, vp);

   return NULL;
   }

//this is the unsigned version of the
//intersect routine below
// TR_VPConstraint *TR_VPMergedConstraints::intIntersect(TR_VPIntConstraint *otherCur, ListElement<TR_VPConstraint> *otherNext, TR_ValuePropagation *vp, bool isUnsigned)
//    {
//    TR_ScratchList<TR_VPConstraint>         result(vp->trMemory());
//    ListElement<TR_VPConstraint> *next = _constraints.getListHead();
//    TR_VPIntConstraint           *cur  = next->getData()->asIntConstraint();
//    ListElement<TR_VPConstraint> *lastResultEntry = NULL;

//    //TR_VPIntConstraint *otherCur = other->asIntConstraint();

//    //TR_ASSERT(cur && otherCur, "Expecting int constraints in intIntersect");

//    //if (otherCur)
//       {
//       uint32_t curLow = (uint32_t)cur->getLow();
//       uint32_t curHigh = (uint32_t)cur->getHigh();
//       uint32_t otherLow = (uint32_t)otherCur->getLow();
//       uint32_t otherHigh = (uint32_t)otherCur->getHigh();

//       next = next->getNextElement();
//       while (cur && otherCur)
//          {
//          bool skipCur      = false;
//          bool skipOtherCur = false;

//          TR_ASSERT(cur->isUnsigned() && otherCur->isUnsigned(), "Expecting unsigned int constraints");
//          // If the two current ranges do not overlap, skip the lower range and
//          // try again
//          //
//          if (curHigh < otherLow)
//             skipCur = true;
//          else if (otherHigh < curLow)
//             skipOtherCur = true;

//          else
//             {
//             // Put the intersection of the two current ranges into the result list
//             //
//             uint32_t resultLow = (curLow > otherLow) ? curLow : otherLow;
//             uint32_t resultHigh = (curHigh < otherHigh) ? curHigh : otherHigh;
//             TR_VPConstraint *constraint = TR_VPIntRange::create(vp, resultLow, resultHigh, true);
//             ///if (constraint)
//             ///   constraint->setIsUnsigned(true);
//             lastResultEntry = result.addAfter(constraint, lastResultEntry);

//             // Reduce the two current ranges. If either is exhausted, move to the
//             // next
//             //
//             if (resultHigh == TR::getMaxUnsigned<TR::Int32>())
//                break;
//             curLow = otherLow = resultHigh+1;
//             if (curLow > curHigh)
//                skipCur = true;
//             if (otherLow > otherHigh)
//                skipOtherCur = true;
//             }

//          if (skipCur)
//             {
//             if (next)
//                {
//                cur = next->getData()->asIntConstraint();
//                TR_ASSERT(cur, "Expecting int constraints in intIntersect");
//                next = next->getNextElement();
//                curLow = cur->getLow();
//                curHigh = cur->getHigh();
//                }
//             else
//                break;
//             }
//          if (skipOtherCur)
//             {
//             if (otherNext)
//                {
//                otherCur = otherNext->getData()->asIntConstraint();
//                TR_ASSERT(otherCur, "Expecting int constraints in intIntersect");
//                otherNext = otherNext->getNextElement();
//                otherLow = otherCur->getLow();
//                otherHigh = otherCur->getHigh();
//                }
//             else
//                break;
//             }
//          }

//       lastResultEntry = result.getListHead();
//       if (!lastResultEntry)
//          return NULL;

//       // If only one entry, collapse the merged list into the single entry
//       //
//       if (!lastResultEntry->getNextElement())
//          return lastResultEntry->getData();

//       return TR_VPMergedConstraints::create(vp, lastResultEntry);
//       }
//    }

TR_VPConstraint *TR_VPMergedConstraints::shortIntersect(TR_VPConstraint * other, ListElement<TR_VPConstraint> *otherNext, TR_ValuePropagation *vp)
   {
   TR_VPShortConstraint *otherCur = other->asShortConstraint();

   TR_ScratchList<TR_VPConstraint>  result (vp->trMemory());
   ListElement<TR_VPConstraint>    *next = _constraints.getListHead();
   TR_VPShortConstraint            *cur  = next->getData()->asShortConstraint();
   ListElement<TR_VPConstraint>    *lastResultEntry = NULL;

   if (otherCur)
      {
      int16_t curLow = cur->getLow();
      int16_t curHigh = cur->getHigh();
      int16_t otherLow = otherCur->getLow();
      int16_t otherHigh = otherCur->getHigh();

      next = next->getNextElement();
      while (cur && otherCur)
         {
         bool skipCur      = false;
         bool skipOtherCur = false;

         // If the two current ranges do not overlap, skip the lower range and
         // try again
         //
         if (curHigh < otherLow)
            skipCur = true;
         else if (otherHigh < curLow)
            skipOtherCur = true;

         else
            {
            // Put the intersection of the two current ranges into the result list
            //
            int16_t resultLow = (curLow > otherLow) ? curLow : otherLow;
            int16_t resultHigh = (curHigh < otherHigh) ? curHigh : otherHigh;
            lastResultEntry = result.addAfter(TR_VPShortRange::create(vp, resultLow, resultHigh), lastResultEntry);

            // Reduce the two current ranges. If either is exhausted, move to the
            // next
            //
            if (resultHigh == TR::getMaxSigned<TR::Int16>())
               break;
            curLow = otherLow = resultHigh+1;
            if (curLow > curHigh)
               skipCur = true;
            if (otherLow > otherHigh)
               skipOtherCur = true;
            }

         if (skipCur)
            {
            if (next)
               {
               cur = next->getData()->asShortConstraint();
               TR_ASSERT(cur, "Expecting short constraints in shortIntersect");
               next = next->getNextElement();
               curLow = cur->getLow();
               curHigh = cur->getHigh();
               }
            else
               break;
            }
         if (skipOtherCur)
            {
            if (otherNext)
               {
               otherCur = otherNext->getData()->asShortConstraint();
               TR_ASSERT(otherCur, "Expecting short constraints in shortIntersect");
               otherNext = otherNext->getNextElement();
               otherLow = otherCur->getLow();
               otherHigh = otherCur->getHigh();
               }
            else
               break;
            }
         }

      lastResultEntry = result.getListHead();
      if (!lastResultEntry)
         return NULL;

      // If only one entry, collapse the merged list into the single entry
      //
      if (!lastResultEntry->getNextElement())
         return lastResultEntry->getData();

      return TR_VPMergedConstraints::create(vp, lastResultEntry);
      }
    else
      {
        TR_ASSERT(false,"Intersecting with another type for short");
      }
    return NULL;
   }

TR_VPConstraint *TR_VPMergedConstraints::intIntersect(TR_VPConstraint *other, ListElement<TR_VPConstraint> *otherNext, TR_ValuePropagation *vp)
   {

   TR_VPIntConstraint *otherCur = other->asIntConstraint();
   //if (otherCur && otherCur->isUnsigned())
   //   return intIntersect(otherCur, otherNext, vp, true);

   TR_ScratchList<TR_VPConstraint>         result(vp->trMemory());
   ListElement<TR_VPConstraint> *next = _constraints.getListHead();
   TR_VPIntConstraint           *cur  = next->getData()->asIntConstraint();
   ListElement<TR_VPConstraint> *lastResultEntry = NULL;


   //TR_ASSERT(cur && otherCur, "Expecting int constraints in intIntersect");

   if (otherCur)
      {
      int32_t curLow = cur->getLow();
      int32_t curHigh = cur->getHigh();
      int32_t otherLow = otherCur->getLow();
      int32_t otherHigh = otherCur->getHigh();

      next = next->getNextElement();
      while (cur && otherCur)
         {
         bool skipCur      = false;
         bool skipOtherCur = false;

         // If the two current ranges do not overlap, skip the lower range and
         // try again
         //
         if (curHigh < otherLow)
            skipCur = true;
         else if (otherHigh < curLow)
            skipOtherCur = true;

         else
            {
            // Put the intersection of the two current ranges into the result list
            //
            int32_t resultLow = (curLow > otherLow) ? curLow : otherLow;
            int32_t resultHigh = (curHigh < otherHigh) ? curHigh : otherHigh;
            lastResultEntry = result.addAfter(TR_VPIntRange::create(vp, resultLow, resultHigh), lastResultEntry);

            // Reduce the two current ranges. If either is exhausted, move to the
            // next
            //
            if (resultHigh == TR::getMaxSigned<TR::Int32>())
               break;
            curLow = otherLow = resultHigh+1;
            if (curLow > curHigh)
               skipCur = true;
            if (otherLow > otherHigh)
               skipOtherCur = true;
            }

         if (skipCur)
            {
            if (next)
               {
               cur = next->getData()->asIntConstraint();
               TR_ASSERT(cur, "Expecting int constraints in intIntersect");
               next = next->getNextElement();
               curLow = cur->getLow();
               curHigh = cur->getHigh();
               }
            else
               break;
            }
         if (skipOtherCur)
            {
            if (otherNext)
               {
               otherCur = otherNext->getData()->asIntConstraint();
               TR_ASSERT(otherCur, "Expecting int constraints in intIntersect");
               otherNext = otherNext->getNextElement();
               otherLow = otherCur->getLow();
               otherHigh = otherCur->getHigh();
               }
            else
               break;
            }
         }

      lastResultEntry = result.getListHead();
      if (!lastResultEntry)
         return NULL;

      // If only one entry, collapse the merged list into the single entry
      //
      if (!lastResultEntry->getNextElement())
         return lastResultEntry->getData();

      return TR_VPMergedConstraints::create(vp, lastResultEntry);
      }

   else
      {
      TR_VPLongConstraint *otherCur = other->asLongConstraint();

      next = next->getNextElement();
      while (cur && otherCur)
         {
         int32_t curLow = cur->getLow();
         int32_t curHigh = cur->getHigh();
         int64_t otherLow = otherCur->getLow();
         int64_t otherHigh = otherCur->getHigh();

         bool skipCur      = false;
         bool skipOtherCur = false;

         // If the two current ranges do not overlap, skip the lower range and
         // try again
         //
         if ((int64_t)curHigh < otherLow)
            skipCur = true;
         else if (otherHigh < (int64_t)curLow)
            skipOtherCur = true;

         else
            {
            // Put the intersection of the two current ranges into the result list
            //
            int32_t resultLow = ((int64_t)curLow > (int64_t)otherLow) ? (int32_t)curLow : (int32_t)otherLow;
            int32_t resultHigh = ((int64_t)curHigh < (int64_t)otherHigh) ? (int32_t)curHigh : (int32_t)otherHigh;
            lastResultEntry = result.addAfter(TR_VPIntRange::create(vp, resultLow, resultHigh), lastResultEntry);

            // Reduce the two current ranges. If either is exhausted, move to the
            // next
            //
            if (resultHigh == TR::getMaxSigned<TR::Int32>())
               break;
            otherLow = resultHigh+1;
            curLow = (int32_t)otherLow;
            if (curLow > curHigh)
               skipCur = true;
            if (otherLow > otherHigh)
               skipOtherCur = true;
            }

         if (skipCur)
            {
            if (next)
               {
               cur = next->getData()->asIntConstraint();
               TR_ASSERT(cur, "Expecting int constraints in intIntersect");
               next = next->getNextElement();
               curLow = cur->getLow();
               curHigh = cur->getHigh();
               }
            else
               break;
            }
         if (skipOtherCur)
            {
            if (otherNext)
               {
               otherCur = otherNext->getData()->asLongConstraint();
               //TR_ASSERT(otherCur, "Expecting int constraints in intIntersect");
               otherNext = otherNext->getNextElement();
               otherLow = otherCur->getLow();
               otherHigh = otherCur->getHigh();
               }
            else
               break;
            }
         }

      lastResultEntry = result.getListHead();
      if (!lastResultEntry)
         return NULL;

      // If only one entry, collapse the merged list into the single entry
      //
      if (!lastResultEntry->getNextElement())
         return lastResultEntry->getData();

      return TR_VPMergedConstraints::create(vp, lastResultEntry);
      }
   }

TR_VPConstraint *TR_VPMergedConstraints::longIntersect(TR_VPConstraint *other, ListElement<TR_VPConstraint> *otherNext, TR_ValuePropagation *vp)
   {
   TR_ScratchList<TR_VPConstraint>         result(vp->trMemory());
   ListElement<TR_VPConstraint> *next = _constraints.getListHead();
   TR_VPLongConstraint          *cur  = next->getData()->asLongConstraint();
   ListElement<TR_VPConstraint> *lastResultEntry = NULL;

   //TR_ASSERT(cur && otherCur, "Expecting long constraints in longIntersect");

   TR_VPLongConstraint *otherCur = other->asLongConstraint();

   if (otherCur)
      {
      int64_t curLow = cur->getLow();
      int64_t curHigh = cur->getHigh();
      int64_t otherLow = otherCur->getLow();
      int64_t otherHigh = otherCur->getHigh();

      next = next->getNextElement();
      while (cur && otherCur)
         {
         bool skipCur      = false;
         bool skipOtherCur = false;

         // If the two current ranges do not overlap, skip the lower range and
         // try again
         //
         if (curHigh < otherLow)
            skipCur = true;
         else if (otherHigh < curLow)
            skipOtherCur = true;

         else
            {
            // Put the intersection of the two current ranges into the result list
            //
            int64_t resultLow = (curLow > otherLow) ? curLow : otherLow;
            int64_t resultHigh = (curHigh < otherHigh) ? curHigh : otherHigh;
            lastResultEntry = result.addAfter(TR_VPLongRange::create(vp, resultLow, resultHigh), lastResultEntry);

            // Reduce the two current ranges. If either is exhausted, move to the
            // next
            //
            if (resultHigh == TR::getMaxSigned<TR::Int64>())
               break;
            curLow = otherLow = resultHigh+1;
            if (curLow > curHigh)
               skipCur = true;
            if (otherLow > otherHigh)
               skipOtherCur = true;
            }

         if (skipCur)
            {
            if (next)
               {
               cur = next->getData()->asLongConstraint();
               TR_ASSERT(cur, "Expecting long constraints in longIntersect");
               next = next->getNextElement();
               curLow = cur->getLow();
               curHigh = cur->getHigh();
               }
            else
               break;
            }
         if (skipOtherCur)
            {
            if (otherNext)
               {
               otherCur = otherNext->getData()->asLongConstraint();
               TR_ASSERT(otherCur, "Expecting long constraints in longIntersect");
               otherNext = otherNext->getNextElement();
               otherLow = otherCur->getLow();
               otherHigh = otherCur->getHigh();
               }
            else
               break;
            }
         }

      lastResultEntry = result.getListHead();
      if (!lastResultEntry)
         return NULL;

      // If only one entry, collapse the merged list into the single entry
      //
      if (!lastResultEntry->getNextElement())
         return lastResultEntry->getData();

      return TR_VPMergedConstraints::create(vp, lastResultEntry);
      }

   else
      {
      TR_VPIntConstraint *otherCur = other->asIntConstraint();
      if (otherCur == NULL)
         return NULL;
      int64_t curLow = cur->getLow();
      int64_t curHigh = cur->getHigh();
      int32_t otherLow = otherCur->getLow();
      int32_t otherHigh = otherCur->getHigh();

      next = next->getNextElement();
      while (cur && otherCur)
         {
         bool skipCur      = false;
         bool skipOtherCur = false;

         // If the two current ranges do not overlap, skip the lower range and
         // try again
         //
         if ((int64_t)curHigh < (int64_t)otherLow)
            skipCur = true;
         else if ((int64_t)otherHigh < (int64_t)curLow)
            skipOtherCur = true;

         else
            {
            // Put the intersection of the two current ranges into the result list
            //
            int64_t resultLow = ((int64_t)curLow > (int64_t)otherLow) ? curLow : otherLow;
            int64_t resultHigh = ((int64_t)curHigh < (int64_t)otherHigh) ? curHigh : otherHigh;
            lastResultEntry = result.addAfter(TR_VPLongRange::create(vp, resultLow, resultHigh), lastResultEntry);

            // Reduce the two current ranges. If either is exhausted, move to the
            // next
            //
            if (resultHigh == TR::getMaxSigned<TR::Int64>())
               break;
            curLow = resultHigh+1;
            otherLow = (int32_t)(resultHigh+1);
            if (curLow > curHigh)
               skipCur = true;
            if (otherLow > otherHigh)
               skipOtherCur = true;
            }

         if (skipCur)
            {
            if (next)
               {
               cur = next->getData()->asLongConstraint();
               TR_ASSERT(cur, "Expecting long constraints in longIntersect");
               next = next->getNextElement();
               curLow = cur->getLow();
               curHigh = cur->getHigh();
               }
            else
               break;
            }
         if (skipOtherCur)
            {
            if (otherNext)
               {
               otherCur = otherNext->getData()->asIntConstraint();
               //TR_ASSERT(otherCur, "Expecting long constraints in longIntersect");
               otherNext = otherNext->getNextElement();
               otherLow = otherCur->getLow();
               otherHigh = otherCur->getHigh();
               }
            else
               break;
            }
         }

      lastResultEntry = result.getListHead();
      if (!lastResultEntry)
         return NULL;

      // If only one entry, collapse the merged list into the single entry
      //
      if (!lastResultEntry->getNextElement())
         return lastResultEntry->getData();

      return TR_VPMergedConstraints::create(vp, lastResultEntry);
      }
   }

TR_VPConstraint *TR_VPLessThanOrEqual::intersect1(TR_VPConstraint *other, TR_ValuePropagation *vp)
   {
   TRACER(vp, this, other);

   TR_VPNotEqual *otherNE = other->asNotEqual();
   if (otherNE)
      {
      if (otherNE->increment() != increment())
         return this;
      TR_VPConstraint *rel = TR_VPLessThanOrEqual::create(vp, increment()-1);
      if (hasArtificialIncrement())
        rel->setHasArtificialIncrement();
      return rel;
      }
   TR_VPLessThanOrEqual *otherLE = other->asLessThanOrEqual();
   if (otherLE)
      {
      if (otherLE->increment() < increment())
         return other;
      return this;
      }
   return NULL;
   }

TR_VPConstraint *TR_VPGreaterThanOrEqual::intersect1(TR_VPConstraint *other, TR_ValuePropagation *vp)
   {
   TRACER(vp, this, other);

   TR_VPNotEqual *otherNE = other->asNotEqual();
   if (otherNE)
      {
      if (otherNE->increment() != increment())
         return this;
      TR_VPConstraint *rel = TR_VPGreaterThanOrEqual::create(vp, increment()+1);
      if (hasArtificialIncrement())
        rel->setHasArtificialIncrement();
      return rel;
      }
   TR_VPLessThanOrEqual *otherLE = other->asLessThanOrEqual();
   if (otherLE)
      {
      if (otherLE->increment() == increment())
         return TR_VPEqual::create(vp, increment());
      return this;
      }
   TR_VPGreaterThanOrEqual *otherGE = other->asGreaterThanOrEqual();
   if (otherGE)
      {
      if (otherGE->increment() > increment())
         return other;
      return this;
      }
   return NULL;
   }

TR_VPConstraint *TR_VPNotEqual::intersect1(TR_VPConstraint *other, TR_ValuePropagation *vp)
   {
   TRACER(vp, this, other);

   TR_VPNotEqual *otherNE = other->asNotEqual();
   if (otherNE)
      {
      if (otherNE->increment() == 0)
         return other;
      return this;
      }
   return NULL;
   }

TR_VPConstraint *TR_VPEqual::intersect1(TR_VPConstraint *other, TR_ValuePropagation *vp)
   {
   TRACER(vp, this, other);

   TR_VPNotEqual *otherNE = other->asNotEqual();
   if (otherNE)
      {
      if (otherNE->increment() != increment())
         return this;
      return NULL;
      }
   if (other->asLessThanOrEqual())
      return this;
   if (other->asGreaterThanOrEqual())
      return this;
   TR_VPEqual *otherEQ = other->asEqual();
   if (otherEQ)
      {
      if (otherEQ->increment() != increment())
         return NULL;
      return this;
      }
   return NULL;
   }

// ***************************************************************************
//
// Arithmetic operations on Value Propagation Constraints
//
// ***************************************************************************



TR_VPConstraint *TR_VPConstraint::add(TR_VPConstraint *other, TR::DataTypes type, TR_ValuePropagation *vp)
   {
   return NULL;
   }

TR_VPConstraint *TR_VPConstraint::subtract(TR_VPConstraint *other, TR::DataTypes type, TR_ValuePropagation *vp)
   {
   return NULL;
   }

TR_VPConstraint *TR_VPShortConstraint::add(TR_VPConstraint *other, TR::DataTypes dt, TR_ValuePropagation *vp)
   {
    TR_VPShortConstraint *otherShort = other->asShortConstraint();
    if(!otherShort)
        return NULL;

    TR::DataType type(dt);

    if(!type.isInt16())
        return NULL;

   //Compute the lower and upper bound values, and determine whether or not the arithmetic
   //has overflowed in either case.
   bool lowOverflow;
   int16_t low  = addWithOverflow<int16_t>(getLow(), otherShort->getLow(), lowOverflow);

   bool highOverflow;
   int16_t high = addWithOverflow<int16_t>(getHigh(), otherShort->getHigh(), highOverflow);

   return getRange(low, high, lowOverflow, highOverflow, vp);
   }

TR_VPConstraint *TR_VPIntConstraint::add(TR_VPConstraint *other, TR::DataTypes dt, TR_ValuePropagation *vp)
   {
   // TODO - handle add and subtract for merged constraints
   //
   TR_VPIntConstraint *otherInt = other->asIntConstraint();
   if (!otherInt)
      return NULL;

   TR::DataType type(dt);

   if (!type.isInt32())
      // TODO - we don't handle overflow of 1-byte and 2-byte adds correctly
      return NULL;

   //if (type.isUnsignedInt())
   //   return add(otherInt, vp, true);

   //Compute the lower and upper bound values, and determine whether or not the arithmetic
   //has overflowed in either case.
   bool lowOverflow;
   int32_t low  = addWithOverflow<int32_t>(getLow(), otherInt->getLow(), lowOverflow);

   bool highOverflow;
   int32_t high = addWithOverflow<int32_t>(getHigh(), otherInt->getHigh(), highOverflow);

   return getRange(low, high, lowOverflow, highOverflow, vp);
   }

// // unsigned subtract operation
// TR_VPConstraint *TR_VPIntConstraint::subtract(TR_VPIntConstraint *otherInt, TR_ValuePropagation *vp, bool Unsigned)
//    {
//    TR_ASSERT(isUnsigned() && otherInt->isUnsigned(), "Expecting unsigned constraints in subtract\n");
//    uint32_t low = (uint32_t)getLow() - (uint32_t)otherInt->getHigh();
//    uint32_t subLow = (uint32_t)getLow() - (uint32_t)otherInt->getLow();
//    uint32_t high = (uint32_t)getHigh() - (uint32_t)otherInt->getLow();
//    uint32_t subHigh = (uint32_t)getHigh() - (uint32_t)otherInt->getHigh();

//    TR_VPIntConstraint *wrapRange = NULL;
//    TR_VPConstraint *range = NULL;
//    //
//    // Check for unsigned integer arithmetic wrap
//    //
//    if ((uint32_t) otherInt->getLow() > 0 &&
//          ((high > (uint32_t)getHigh())||
//           (subHigh > (uint32_t)getHigh())))
//       {
//       if (subHigh < high)
//          high = subHigh;
//       if (subLow > low)
//          low = subLow;

//       wrapRange = TR_VPIntRange::create(vp, high, TR::getMaxUnsigned<TR::Int32>(), true);
//       range = TR_VPIntRange::create(vp, TR::getMinUnsigned<TR::Int32>(), low, true);
//       if (!range ||
//           !wrapRange)
//          return NULL;
//       range = TR_VPMergedConstraints::create(vp, range, wrapRange);
//       ///range->setIsUnsigned(true);
//       }
//    if ((uint32_t)otherInt->getHigh() > 0 &&
//        ((low > (uint32_t)getLow()) ||
//         (subLow > (uint32_t)getLow())))
//       {
//       if (range)
//          return NULL;

//       if (subHigh > high)
//          high = subHigh;
//       if (subLow < low)
//          low = subLow;

//       wrapRange = TR_VPIntRange::create(vp, low, TR::getMaxUnsigned<TR::Int32>(), true);
//       range = TR_VPIntRange::create(vp, TR::getMinUnsigned<TR::Int32>(), high, true);
//       if (!range ||
//           !wrapRange)
//          return NULL;
//       range = TR_VPMergedConstraints::create(vp, range, wrapRange);
//       ///range->setIsUnsigned(true);
//       }

//    if (!range)
//       range = TR_VPIntRange::create(vp, low, high, true);
//    return range;
//    }

TR_VPConstraint *TR_VPShortConstraint::subtract(TR_VPConstraint *other, TR::DataTypes dt, TR_ValuePropagation * vp)
   {
   TR_VPShortConstraint *otherShort = other->asShortConstraint();

   if(!otherShort)
       return NULL;

   TR::DataType type(dt);

   if (!type.isInt16())
       return NULL;

   //Compute the lower and upper bound values, and determine whether or not the arithmetic
   //has overflowed in either case.
   bool lowOverflow;
   int16_t low  = subWithOverflow<int16_t>(getLow(), otherShort->getHigh(), lowOverflow);

   bool highOverflow;
   int16_t high = subWithOverflow<int16_t>(getHigh(), otherShort->getLow(), highOverflow);

   return getRange(low, high, lowOverflow, highOverflow, vp);
   }

TR_VPConstraint *TR_VPIntConstraint::subtract(TR_VPConstraint *other, TR::DataTypes dt, TR_ValuePropagation *vp)
   {
   TR_VPIntConstraint *otherInt = other->asIntConstraint();
   if (!otherInt)
      return NULL;

   TR::DataType type(dt);
   if (!type.isInt32())
      // TODO - we don't handle overflow of 1-byte and 2-byte subtracts correctly
      return NULL;

   //if (type.isUnsignedInt())
   //   return subtract(otherInt, vp, true);

   //Compute the lower and upper bound values, and determine whether or not the arithmetic
   //has overflowed in either case.
   bool lowOverflow;
   int32_t low  = subWithOverflow<int32_t>(getLow(), otherInt->getHigh(), lowOverflow);

   bool highOverflow;
   int32_t high = subWithOverflow<int32_t>(getHigh(), otherInt->getLow(), highOverflow);

   return getRange(low, high, lowOverflow, highOverflow, vp);
   }

TR_VPConstraint *TR_VPShortConstraint::getRange(int16_t low, int16_t high, bool lowCanOverflow, bool highCanOverflow, TR_ValuePropagation * vp)
   {
   //This function returns a VP constraint range, with the appropriate setting of the canOverflow bit, for either an addition or subtraction
   //operation. The function takes the results of the lower- and upper-bound calculations for the operation, both performed in the same precision
   //arithmetic as that of the underlying operation, along with two flags indicated whether or not the operations producing these two bounds
   //overflowed. From these values, a constraint can be created, with the overflow flags being set as conservatively as possible.
   //
   //Also see VPIntConstraint::getRange and VPLongConstraint::getRange

   if ( lowCanOverflow && highCanOverflow )
      {//Both sides overflow

      if ( ( high ^ low ) < 0 )
         // high and low have different signs, thus high has overflowed above range, low has overflowed below range, result is full range
         return NULL;

      //either both high and low have overflowed above range, or both have overflowed below range, result is guarunteed to overflow every time
      return TR_VPShortRange::create(vp, low, high, TR_yes);
      }

   else if ( lowCanOverflow || highCanOverflow )
      {
      //Only one sided has overflowed, generally a merged constraint

      if ( high >= low )
         //this would result in an overlapping merged constraint, or full range result
         return NULL;

      //disjoint merged constraint
      TR_VPConstraint* range1 = TR_VPShortRange::create(vp, TR::getMinSigned<TR::Int16>(), high, TR_yes);
      TR_VPConstraint* range2 = TR_VPShortRange::create(vp, low, TR::getMaxSigned<TR::Int16>(), TR_yes);
      return TR_VPMergedConstraints::create(vp, range1, range2);
      }

   //no overflow
   return TR_VPShortRange::create(vp, low, high, TR_no);
   }

TR_VPConstraint *TR_VPIntConstraint::getRange(int32_t low, int32_t high, bool lowCanOverflow, bool highCanOverflow, TR_ValuePropagation *vp)
   {
   //This function returns a VP constraint range, with the appropriate setting of the canOverflow bit, for either an addition or subtraction
   //operation. The function takes the results of the lower- and upper-bound calculations for the operation, both performed in the same precision
   //arithmetic as that of the underlying operation, along with two flags indicated whether or not the operations producing these two bounds
   //overflowed. From these values, a constraint can be created, with the overflow flags being set as conservatively as possible.
   //
   //Also see VPShortConstraint::getRange and VPLongConstraint::getRange

   if ( lowCanOverflow && highCanOverflow )
      {//Both sides overflow

      if ( ( high ^ low ) < 0 )
         // high and low have different signs, thus high has overflowed above range, low has overflowed below range, result is full range
         return NULL;

      //either both high and low have overflowed above range, or both have overflowed below range, result is guarunteed to overflow every time
      return TR_VPIntRange::create(vp, low, high, TR_yes);
      }

   else if ( lowCanOverflow || highCanOverflow )
      {
      //Only one sided has overflowed, generally a merged constraint

      if ( high >= low )
         //this would result in an overlapping merged constraint, or full range result
         return NULL;

      //disjoint merged constraint
      TR_VPConstraint* range1 = TR_VPIntRange::create(vp, TR::getMinSigned<TR::Int32>(), high, TR_yes);
      TR_VPConstraint* range2 = TR_VPIntRange::create(vp, low, TR::getMaxSigned<TR::Int32>(), TR_yes);
      return TR_VPMergedConstraints::create(vp, range1, range2);
      }

   //no overflow
   return TR_VPIntRange::create(vp, low, high, TR_no);
   }

TR_VPConstraint *TR_VPLongConstraint::add(TR_VPConstraint *other, TR::DataTypes dt, TR_ValuePropagation *vp)
   {
   // TODO - handle add and subtract for merged constraints
   //
   TR_VPLongConstraint *otherLong = other->asLongConstraint();
   if (!otherLong)
      return NULL;

   TR::DataType type(dt);
   if (!type.isInt64())
      return NULL;

   //Compute the lower and upper bound values, and determine whether or not the arithmetic
   //has overflowed in either case.
   bool lowOverflow;
   int64_t low  = addWithOverflow<int64_t>(getLow(), otherLong->getLow(), lowOverflow);

   bool highOverflow;
   int64_t high = addWithOverflow<int64_t>(getHigh(), otherLong->getHigh(), highOverflow);

   return getRange(low, high, lowOverflow, highOverflow, vp);
   }

TR_VPConstraint *TR_VPLongConstraint::subtract(TR_VPConstraint *other, TR::DataTypes dt, TR_ValuePropagation *vp)
   {
   TR_VPLongConstraint *otherLong = other->asLongConstraint();
   if (!otherLong)
      return NULL;

   TR::DataType type(dt);
   if (!type.isInt64())
      return NULL;

   //Compute the lower and upper bound values, and determine whether or not the arithmetic
   //has overflowed in either case.
   bool lowOverflow;
   int64_t low  = subWithOverflow<int64_t>(getLow(), otherLong->getHigh(), lowOverflow);

   bool highOverflow;
   int64_t high = subWithOverflow<int64_t>(getHigh(), otherLong->getLow(), highOverflow);

   return getRange(low, high, lowOverflow, highOverflow, vp);
   }

TR_VPConstraint *TR_VPLongConstraint::getRange(int64_t low, int64_t high, bool lowCanOverflow, bool highCanOverflow, TR_ValuePropagation *vp)
   {
   //This function returns a VP constraint range, with the appropriate setting of the canOverflow bit, for either an addition or subtraction
   //operation. The function takes the results of the lower- and upper-bound calculations for the operation, both performed in the same precision
   //arithmetic as that of the underlying operation, along with two flags indicated whether or not the operations producing these two bounds
   //overflowed. From these values, a constraint can be created, with the overflow flags being set as conservatively as possible.
   //
   //Also see VPShortConstraint::getRange and VPIntConstraint::getRange

   if ( lowCanOverflow && highCanOverflow )
      {//Both sides overflow

      if ( ( high ^ low ) < 0 )
         // high and low have different signs, thus high has overflowed above range, low has overflowed below range, result is full range
         return NULL;

      //either both high and low have overflowed above range, or both have overflowed below range, result is guarunteed to overflow every time
      return TR_VPLongRange::create(vp, low, high, TR_yes);
      }

   else if ( lowCanOverflow || highCanOverflow )
      {
      //Only one sided has overflowed, generally a merged constraint

      if ( high >= low )
         //this would result in an overlapping merged constraint, or full range result
         return NULL;

      //disjoint merged constraint
      TR_VPConstraint* range1 = TR_VPLongRange::create(vp, TR::getMinSigned<TR::Int64>(), high, TR_yes);
      TR_VPConstraint* range2 = TR_VPLongRange::create(vp, low, TR::getMaxSigned<TR::Int64>(), TR_yes);
      return TR_VPMergedConstraints::create(vp, range1, range2);
      }

   //no overflow
   return TR_VPLongRange::create(vp, low, high, TR_no);
   }

// ***************************************************************************
//
// COmparison operations on Value Propagation Constraints
//
// ***************************************************************************

bool TR_VPConstraint::mustBeEqual(TR_VPConstraint *other, TR_ValuePropagation *vp)
   {
   return false;
   }

bool TR_VPShortConst::mustBeEqual(TR_VPConstraint * other, TR_ValuePropagation *vp)
   {
   TR_VPShortConst * otherConst = other->asShortConst();
   if (isUnsigned() && otherConst && otherConst->isUnsigned())
       return ((uint16_t)otherConst->getShort() == (uint16_t)getShort());
   return otherConst && otherConst->getShort() == getShort();
   }

bool TR_VPIntConst::mustBeEqual(TR_VPConstraint *other, TR_ValuePropagation *vp)
   {
   TR_VPIntConst *otherConst = other->asIntConst();
   if (isUnsigned() && otherConst && otherConst->isUnsigned())
      return ((uint32_t)otherConst->getInt() == (uint32_t)getInt());
   return otherConst && otherConst->getInt() == getInt();
   }

bool TR_VPLongConst::mustBeEqual(TR_VPConstraint *other, TR_ValuePropagation *vp)
   {
   TR_VPLongConst *otherConst = other->asLongConst();
   return otherConst && otherConst->getLong() == getLong();
   }

bool TR_VPClass::mustBeEqual(TR_VPConstraint *other, TR_ValuePropagation *vp)
   {
   if (isNullObject() && other->isNullObject())
      return true;

   if (getKnownObject() && other->getKnownObject() && isNonNullObject() && other->isNonNullObject())
      return getKnownObject()->getIndex() == other->getKnownObject()->getIndex();

   TR_VPClass *otherClass = NULL;
   if (other)
      otherClass = other->asClass();
   if (!_preexistence &&
       !_arrayInfo && _type &&
       _type->isFixedClass() && isNonNullObject() &&
       other && otherClass &&
       !otherClass->getArrayInfo() &&
       !otherClass->isPreexistentObject() &&
       otherClass->getClassType() && otherClass->getClassType()->isFixedClass() && otherClass->isNonNullObject() &&
       (isClassObject() == TR_yes) &&
       (other->isClassObject() == TR_yes))
      {
      if (_type->asFixedClass()->getClass() == otherClass->getClassType()->asFixedClass()->getClass())
         return true;
      }

   return false;
   }

bool TR_VPNullObject::mustBeEqual(TR_VPConstraint *other, TR_ValuePropagation *vp)
   {
   return other->isNullObject();
   }

bool TR_VPConstString::mustBeEqual(TR_VPConstraint *other, TR_ValuePropagation *vp)
   {
   if (other->getKnownObject())
      return other->mustBeEqual(this, vp);
   else
      return false;
   }

bool TR_VPKnownObject::mustBeEqual(TR_VPConstraint *other, TR_ValuePropagation *vp)
   {
   // An expression with a Known Object constraint could be null.  Since there
   // are two possible values, we can't generally prove we mustBeEqual to
   // another expression unless we have isNonNullObject info too, and that is
   // handled by TR_VPClass, so there's little we can do here.
   //
   return Super::mustBeEqual(other, vp);
   }

bool TR_VPKnownObject::mustBeNotEqual(TR_VPConstraint *other, TR_ValuePropagation *vp)
   {
   // An expression with a Known Object constraint could be null.  To have any
   // hope of proving we're notEqual to some other expression, its constraint
   // must preclude null.
   //
   if (other->isNonNullObject())
      {
      if (other->getKnownObject() && getIndex() != other->getKnownObject()->getIndex())
         return true;
      }

   return Super::mustBeNotEqual(other, vp);
   }

bool TR_VPConstString::mustBeNotEqual(TR_VPConstraint *other, TR_ValuePropagation *vp)
   {
   if (other->getKnownObject())
      return other->getKnownObject()->mustBeNotEqual(this, vp);
   else
      return false;
   }

bool TR_VPConstraint::mustBeNotEqual(TR_VPConstraint *other, TR_ValuePropagation *vp)
   {
   if (isNullObject())
      {
      if (other->isNonNullObject())
         return true;
      }
   else if (isNonNullObject())
      {
      if (other->isNullObject())
         return true;
      }
   return false;
   }
bool TR_VPShortConstraint::mustBeNotEqual(TR_VPConstraint * other, TR_ValuePropagation *vp)
   {
   TR_VPShortConstraint *otherShort = other->asShortConstraint();
   if(otherShort)
      {
      if (isUnsigned() && otherShort->isUnsigned())
          return (((uint16_t)getHigh() < (uint16_t)otherShort->getLow()) ||
                  ((uint16_t)getLow()  > (uint16_t)otherShort->getHigh()));
      else
          return getHigh() < otherShort->getLow() || getLow() > otherShort->getHigh();
      }

   TR_VPMergedConstraints *otherList = other->asMergedShortConstraints();
   if (otherList)
      {
      ListIterator <TR_VPConstraint> iter(otherList->getList());
      for (TR_VPConstraint * c = iter.getFirst(); c; c=iter.getNext())
          {
          if(!mustBeNotEqual(c, vp))
             return false;
          }
      return true;
      }
   return false;
   }

bool TR_VPIntConstraint::mustBeNotEqual(TR_VPConstraint *other, TR_ValuePropagation *vp)
   {
   TR_VPIntConstraint *otherInt = other->asIntConstraint();
   if (otherInt)
      {
      if (isUnsigned() && otherInt->isUnsigned())
         return (((uint32_t)getHigh() < (uint32_t)otherInt->getLow()) ||
                  ((uint32_t)getLow() > (uint32_t)otherInt->getHigh()));
      else
         return getHigh() < otherInt->getLow() || getLow() > otherInt->getHigh();
      }
   TR_VPMergedConstraints *otherList = other->asMergedIntConstraints();
   if (otherList)
      {
      // Must be not equal to each item in the list to be not equal to the list
      //
      ListIterator<TR_VPConstraint> iter(otherList->getList());
      for (TR_VPConstraint *c = iter.getFirst(); c; c = iter.getNext())
         {
         if (!mustBeNotEqual(c, vp))
            return false;
         }
      return true;
      }
   return false;
   }

bool TR_VPLongConstraint::mustBeNotEqual(TR_VPConstraint *other, TR_ValuePropagation *vp)
   {
   TR_VPLongConstraint *otherLong = other->asLongConstraint();
   if (otherLong)
      return getHigh() < otherLong->getLow() || getLow() > otherLong->getHigh();
   TR_VPMergedConstraints *otherList = other->asMergedLongConstraints();
   if (otherList)
      {
      // Must be not equal to each item in the list to be not equal to the list
      //
      ListIterator<TR_VPConstraint> iter(otherList->getList());
      for (TR_VPConstraint *c = iter.getFirst(); c; c = iter.getNext())
         {
         if (!mustBeNotEqual(c, vp))
            return false;
         }
      return true;
      }
   return false;
   }

bool TR_VPMergedConstraints::mustBeNotEqual(TR_VPConstraint *other, TR_ValuePropagation *vp)
   {
   if (!other->asMergedConstraints())
      return other->mustBeNotEqual(this, vp);

   // Every item in this list must be not equal to the other list
   //
   ListIterator<TR_VPConstraint> iter(getList());
   for (TR_VPConstraint *c = iter.getFirst(); c; c = iter.getNext())
      {
      if (!c->mustBeNotEqual(other, vp))
         return false;
      }
   return true;
   }


//FIXME: this is too conservative, can do more for non-fixed objects
bool TR_VPClass::mustBeNotEqual(TR_VPConstraint *other, TR_ValuePropagation *vp)
   {
   if (isNullObject() && other->isNonNullObject())
      return true;
   if (isNonNullObject() && other->isNullObject())
      return true;

   if (getKnownObject() && other->getKnownObject() && isNonNullObject() && other->isNonNullObject())
      return getKnownObject()->getIndex() != other->getKnownObject()->getIndex();

   TR_VPClass *otherClass = NULL;
   if (other)
      otherClass = other->asClass();
  if (!_preexistence &&
       !_arrayInfo && _type &&
       _type->isFixedClass() && isNonNullObject() &&
       other && otherClass &&
       !otherClass->getArrayInfo() &&
       !otherClass->isPreexistentObject() &&
       otherClass->getClassType() && otherClass->getClassType()->isFixedClass() && otherClass->isNonNullObject() &&
       (isClassObject() == TR_yes) &&
       (other->isClassObject() == TR_yes))
      {
      if (_type->asFixedClass()->getClass() != otherClass->getClassType()->asFixedClass()->getClass())
         return true;
      }

   return false;
   }

bool TR_VPNullObject::mustBeNotEqual(TR_VPConstraint *other, TR_ValuePropagation *vp)
   {
   return other->isNonNullObject();
   }

bool TR_VPNonNullObject::mustBeNotEqual(TR_VPConstraint *other, TR_ValuePropagation *vp)
   {
   return other->isNullObject();
   }

bool TR_VPConstraint::mustBeLessThan(TR_VPConstraint *other, TR_ValuePropagation *vp)
   {
   return false;
   }

bool TR_VPShortConstraint::mustBeLessThan(TR_VPConstraint *other, TR_ValuePropagation *vp)
   {
   if (isUnsigned() && other->isUnsigned())
       return ((uint16_t)getHigh() < (uint16_t)other->getLowShort());
   return getHigh() < other->getLowShort();
   }

bool TR_VPIntConstraint::mustBeLessThan(TR_VPConstraint *other, TR_ValuePropagation *vp)
   {
   if (isUnsigned() && other->isUnsigned())
      return ((uint32_t)getHigh() < (uint32_t)other->getLowInt());
   return getHigh() < other->getLowInt();
   }

bool TR_VPLongConstraint::mustBeLessThan(TR_VPConstraint *other, TR_ValuePropagation *vp)
   {
   return getHigh() < other->getLowLong();
   }

bool TR_VPMergedConstraints::mustBeLessThan(TR_VPConstraint *other, TR_ValuePropagation *vp)
   {
   if (_type.isInt16())
      {
      if (_constraints.getLastElement()->getData()->isUnsigned())
          return ((uint16_t)getHighShort() < (uint16_t)other->getLowShort());
      return getHighShort() < other->getLowShort();
      }

   if (_type.isInt64())
      return getHighLong() < other->getLowLong();

   if (_constraints.getLastElement()->getData()->isUnsigned())
      return ((uint32_t)getHighInt() < (uint32_t)other->getLowInt());
   return getHighInt() < other->getLowInt();
   }

bool TR_VPConstraint::mustBeLessThanOrEqual(TR_VPConstraint *other, TR_ValuePropagation *vp)
   {
   return false;
   }

bool TR_VPShortConstraint::mustBeLessThanOrEqual(TR_VPConstraint * other, TR_ValuePropagation *vp)
   {
   if (isUnsigned() && other->isUnsigned())
        return ((uint16_t)getHigh() <= (uint16_t)other->getLowShort());
   return getHigh() <= other->getLowShort();
   }

bool TR_VPIntConstraint::mustBeLessThanOrEqual(TR_VPConstraint *other, TR_ValuePropagation *vp)
   {
   if (isUnsigned() && other->isUnsigned())
      return ((uint32_t)getHigh() <= (uint32_t)other->getLowInt());
   return getHigh() <= other->getLowInt();
   }

bool TR_VPLongConstraint::mustBeLessThanOrEqual(TR_VPConstraint *other, TR_ValuePropagation *vp)
   {
   return getHigh() <= other->getLowLong();
   }

bool TR_VPMergedConstraints::mustBeLessThanOrEqual(TR_VPConstraint *other, TR_ValuePropagation *vp)
   {
   if (_type.isInt64())
      return getHighLong() <= other->getLowLong();
   if (_constraints.getLastElement()->getData()->isUnsigned())
      return ((uint32_t)getHighInt() <= (uint32_t)other->getLowInt());
   return getHighInt() <= other->getLowInt();
   }

bool TR_VPConstraint::mustBeEqual()
   {
   return false;
   }

bool TR_VPEqual::mustBeEqual()
   {
   return increment() == 0;
   }

bool TR_VPConstraint::mustBeNotEqual()
   {
   return false;
   }

bool TR_VPLessThanOrEqual::mustBeNotEqual()
   {
   return increment() < 0;
   }

bool TR_VPGreaterThanOrEqual::mustBeNotEqual()
   {
   return increment() > 0;
   }

bool TR_VPEqual::mustBeNotEqual()
   {
   return increment() != 0;
   }

bool TR_VPNotEqual::mustBeNotEqual()
   {
   return increment() == 0;
   }

bool TR_VPConstraint::mustBeLessThan()
   {
   return false;
   }

bool TR_VPLessThanOrEqual::mustBeLessThan()
   {
   if ((increment() == 0) || hasArtificialIncrement())
      return (increment() < 0);
   return false;
   }

bool TR_VPEqual::mustBeLessThan()
   {
   return false; //increment() < 0;
   }

bool TR_VPConstraint::mustBeLessThanOrEqual()
   {
   return false;
   }

bool TR_VPLessThanOrEqual::mustBeLessThanOrEqual()
   {
   if ((increment() == 0) || hasArtificialIncrement())
      return (increment() <= 0);
   return false;
   }

bool TR_VPEqual::mustBeLessThanOrEqual()
   {
     return false; //increment() <= 0;
   }

bool TR_VPConstraint::mustBeGreaterThan()
   {
   return false;
   }

bool TR_VPGreaterThanOrEqual::mustBeGreaterThan()
   {
   if ((increment() == 0) || hasArtificialIncrement())
      return increment() > 0;
   return false;
   }

bool TR_VPEqual::mustBeGreaterThan()
   {
     return false; //increment() > 0;
   }

bool TR_VPConstraint::mustBeGreaterThanOrEqual()
   {
   return false;
   }

bool TR_VPGreaterThanOrEqual::mustBeGreaterThanOrEqual()
   {
   if ((increment() == 0) || hasArtificialIncrement())
      return (increment() >= 0);
   return false;
   }

bool TR_VPEqual::mustBeGreaterThanOrEqual()
   {
   return false; //increment() >= 0;
   }

// ***************************************************************************
//
// Propagation of absolute constraints to relative constraints and
// propagation of relative constraints.
//
// ***************************************************************************

TR_VPConstraint *TR_VPRelation::propagateAbsoluteConstraint(TR_VPConstraint *constraint, int32_t relative, TR_ValuePropagation *vp)
   {
   // Default propagation is to do nothing
   //
   return NULL;
   }

TR_VPConstraint *TR_VPLessThanOrEqual::propagateAbsoluteConstraint(TR_VPConstraint *constraint, int32_t relative, TR_ValuePropagation *vp)
   {
   // x <= y + I and x == (M to N)    ==> y == ((M-I) to TR::getMaxSigned<TR::Int32>())
   //
   if (vp->trace())
      {
      traceMsg(vp->comp(), "      Propagating V <= value %d %+d and V is ", relative, increment());
      constraint->print(vp->comp(), vp->comp()->getOutFile());
      }

   if (constraint->asLongConstraint())
      {
      int64_t oldBound = constraint->getLowLong();
      int64_t newBound = oldBound - increment();
      if (increment() < 0)
         return NULL;

      if (newBound > oldBound)
         return NULL;

      constraint = TR_VPLongRange::create(vp, newBound, TR::getMaxSigned<TR::Int64>()-increment());
      }
   else
      {
      int32_t oldBound = constraint->getLowInt();
      int32_t newBound = oldBound - increment();
      if (increment() < 0)
         return NULL;

      if (newBound > oldBound)
         return NULL;

      constraint = TR_VPIntRange::create(vp, newBound, TR::getMaxSigned<TR::Int32>()-increment());
      }
   if (vp->trace())
      {
      if (constraint)
         {
         traceMsg(vp->comp(), " ... value %d is ", relative);
         constraint->print(vp->comp(), vp->comp()->getOutFile());
         }
      traceMsg(vp->comp(), "\n");
      }

   return constraint;
   }

TR_VPConstraint *TR_VPGreaterThanOrEqual::propagateAbsoluteConstraint(TR_VPConstraint *constraint, int32_t relative, TR_ValuePropagation *vp)
   {
   // x >= y + I and x == (M to N)    ==> y == (TR::getMinSigned<TR::Int32>() to (M-I))
   //
   if (vp->trace())
      {
      traceMsg(vp->comp(), "      Propagating V >= value %d %+d and V is ", relative, increment());
      constraint->print(vp->comp(), vp->comp()->getOutFile());
      }


   if (constraint->asLongConstraint())
      {
      int64_t oldBound = constraint->getHighLong();
      int64_t newBound = oldBound - increment();
      if (increment() > 0)
         return NULL;

      if (newBound < oldBound)
         return NULL;

      constraint = TR_VPLongRange::create(vp, TR::getMinSigned<TR::Int64>() - increment(), newBound);
      }
   else
      {
      int32_t oldBound = constraint->getHighInt();
      int32_t newBound = oldBound - increment();
      if (increment() > 0)
         return NULL;

      if (newBound < oldBound)
         return NULL;

      constraint = TR_VPIntRange::create(vp, TR::getMinSigned<TR::Int32>() - increment(), newBound);
      }
   if (vp->trace())
      {
      if (constraint)
         {
         traceMsg(vp->comp(), " ... value %d is ", relative);
         constraint->print(vp->comp(), vp->comp()->getOutFile());
         }
      traceMsg(vp->comp(), "\n");
      }

   return constraint;
   }

TR_VPConstraint *TR_VPEqual::propagateAbsoluteConstraint(TR_VPConstraint *constraint, int32_t relative, TR_ValuePropagation *vp)
   {
   // x == y + I and x == (M to N)    ==> y == ((M-I) to N-I))
   //
   if (vp->trace())
      {
      traceMsg(vp->comp(), "      Propagating V == value %d %+d and V is ", relative, increment());
      constraint->print(vp->comp(), vp->comp()->getOutFile());
      }

   if (increment() != 0)
      {
      if (constraint->asLongConstraint())
         constraint = constraint->asLongConstraint()->subtract(TR_VPLongConst::create(vp, increment()), TR::Int64, vp);
      else if (constraint->asIntConstraint())
         {
         constraint = constraint->asIntConstraint()->subtract(TR_VPIntConst::create(vp, increment()), TR::Int32, vp);
         }
      else
         constraint = NULL;
      }
   if (vp->trace())
      {
      if (constraint)
         {
         traceMsg(vp->comp(), " ... value %d is ", relative);
         constraint->print(vp->comp(), vp->comp()->getOutFile());
         }
      traceMsg(vp->comp(), "\n");
      }

   return constraint;
   }

TR_VPConstraint *TR_VPNotEqual::propagateAbsoluteConstraint(TR_VPConstraint *constraint, int32_t relative, TR_ValuePropagation *vp)
   {
   // x != y + I and x == N           ==> y != (N-I)
   //
   if (vp->trace())
      {
      traceMsg(vp->comp(), "      Propagating V != value %d %+d and V is ", relative, increment());
      constraint->print(vp->comp(), vp->comp()->getOutFile());
      }

   TR_VPConstraint *newConstraint = NULL;
   if (constraint->asLongConst())
      {
      int64_t excludedValue = constraint->getLowLong() - increment();
      if (excludedValue != TR::getMinSigned<TR::Int64>())
         newConstraint = TR_VPLongRange::create(vp, TR::getMinSigned<TR::Int64>(), excludedValue-1);
      if (excludedValue != TR::getMaxSigned<TR::Int64>())
         {
         if (newConstraint)
            newConstraint = newConstraint->merge(TR_VPLongRange::create(vp, excludedValue+1, TR::getMaxSigned<TR::Int64>()), vp);
         else
            newConstraint = TR_VPLongRange::create(vp, excludedValue+1, TR::getMaxSigned<TR::Int64>());
         }
      }
   else if (constraint->asIntConst())
      {
      int32_t excludedValue = constraint->getLowInt() - increment();
      // if (constraint->isUnsigned())
//          {
//          bool isNegative = (excludedValue < TR::getMinUnsigned<TR::Int32>());
//          if (!isNegative && excludedValue != TR::getMinUnsigned<TR::Int32>())
//             {
//             newConstraint = TR_VPIntRange::create(vp, TR::getMinUnsigned<TR::Int32>(), excludedValue-1, true);
//             }
//          if (!isNegative && ((uint32_t)excludedValue != TR::getMaxUnsigned<TR::Int32>()))
//             {
//             if (newConstraint)
//                {
//                TR_VPConstraint *exConstraint = TR_VPIntRange::create(vp, excludedValue+1,
//                                                                      TR::getMaxUnsigned<TR::Int32>(), true);
//                newConstraint = newConstraint->merge(exConstraint, vp);
//                }
//             else
//                {
//                newConstraint = TR_VPIntRange::create(vp, excludedValue+1, TR::getMaxUnsigned<TR::Int32>(), true);
//                }
//             }
//          }
//       else
//         {
         if (excludedValue != TR::getMinSigned<TR::Int32>())
            newConstraint = TR_VPIntRange::create(vp, TR::getMinSigned<TR::Int32>(), excludedValue-1);
         if (excludedValue != TR::getMaxSigned<TR::Int32>())
            {
            if (newConstraint)
               newConstraint = newConstraint->merge(TR_VPIntRange::create(vp, excludedValue+1, TR::getMaxSigned<TR::Int32>()), vp);
            else
               newConstraint = TR_VPIntRange::create(vp, excludedValue+1, TR::getMaxSigned<TR::Int32>());
            }
//         }
      }

   if (vp->trace())
      {
      if (newConstraint)
         {
         traceMsg(vp->comp(), " ... value %d is ", relative);
         newConstraint->print(vp->comp(), vp->comp()->getOutFile());
         }
      traceMsg(vp->comp(), "\n");
      }

   return newConstraint;
   }

TR_VPConstraint *TR_VPLessThanOrEqual::propagateRelativeConstraint(TR_VPRelation *other, int32_t relative, int32_t otherRelative, TR_ValuePropagation *vp)
   {
   // x <= y + M and x >= z + N    ==> y >= z + (N-M)
   //
   if (!other->asGreaterThanOrEqual() && !other->asEqual())
      return NULL;

   TR_VPConstraint *constraint;
   int32_t newIncr = other->increment() - increment();

   // newIncr cannot be TR::getMinSigned<TR::Int32>(), because when we try to negate TR::getMinSigned<TR::Int32>(), we get TR::getMinSigned<TR::Int32>() again
   if (newIncr == TR::getMinSigned<TR::Int32>())
      return NULL;

   if (increment() >= 0)
      {
      if (newIncr > other->increment())
         return NULL;
      }
   else
      {
      if (newIncr < other->increment())
         return NULL;
      }
   constraint = TR_VPGreaterThanOrEqual::create(vp, newIncr);
   if (newIncr == other->increment())
      {
      if (other->hasArtificialIncrement())
        constraint->setHasArtificialIncrement();
      }
   else if (newIncr == -1*increment())
      {
      if (hasArtificialIncrement())
        constraint->setHasArtificialIncrement();
      }

   if (vp->trace())
      {
      traceMsg(vp->comp(), "      Propagating V <= value %d %+d and V >= value %d %+d", relative, increment(), otherRelative, other->increment());
      traceMsg(vp->comp(), " ... value %d >= value %d %+d\n", relative, otherRelative, newIncr);
      }
   return constraint;
   }

TR_VPConstraint *TR_VPGreaterThanOrEqual::propagateRelativeConstraint(TR_VPRelation *other, int32_t relative, int32_t otherRelative, TR_ValuePropagation *vp)
   {
   // x >= y + M and x <= z + N    ==> y <= z + (N-M)
   //
   if (!other->asLessThanOrEqual() && !other->asEqual())
      return NULL;

   TR_VPConstraint *constraint;
   int32_t newIncr = other->increment() - increment();

   // newIncr cannot be TR::getMinSigned<TR::Int32>(), because when we try to negate TR::getMinSigned<TR::Int32>(), we get TR::getMinSigned<TR::Int32>() again
   if (newIncr == TR::getMinSigned<TR::Int32>())
      return NULL;

   if (increment() >= 0)
      {
      if (newIncr > other->increment())
         return NULL;
      }
   else
      {
      if (newIncr < other->increment())
         return NULL;
      }
   constraint = TR_VPLessThanOrEqual::create(vp, newIncr);
   if (newIncr == other->increment())
      {
      if (other->hasArtificialIncrement())
        constraint->setHasArtificialIncrement();
      }
   else if (newIncr == -1*increment())
      {
      if (hasArtificialIncrement())
        constraint->setHasArtificialIncrement();
      }

   if (vp->trace())
      {
      traceMsg(vp->comp(), "      Propagating V >= value %d %+d and V <= value %d %+d", relative, increment(), otherRelative, other->increment());
      traceMsg(vp->comp(), " ... value %d <= value %d %+d\n", relative, otherRelative, newIncr);
      }
   return constraint;
   }

TR_VPConstraint *TR_VPEqual::propagateRelativeConstraint(TR_VPRelation *other, int32_t relative, int32_t otherRelative, TR_ValuePropagation *vp)
   {
   // x == y + M and x <= z + N    ==> y <= z + (N-M)
   // x == y + M and x >= z + N    ==> y >= z + (N-M)
   // x == y + M and x != z + N    ==> y != z + (N-M)
   // x == y + M and x == z + N    ==> y == z + (N-M)
   //
   TR_VPConstraint *constraint;
   int32_t newIncr = other->increment() - increment();

   // newIncr cannot be TR::getMinSigned<TR::Int32>(), because when we try to negate TR::getMinSigned<TR::Int32>(), we get TR::getMinSigned<TR::Int32>() again
   if (newIncr == TR::getMinSigned<TR::Int32>())
      return NULL;

   if (increment() >= 0)
      {
      if (newIncr > other->increment())
         return NULL;
      }
   else
      {
      if (newIncr < other->increment())
         return NULL;
      }
   if (other->asLessThanOrEqual())
      {
      constraint = TR_VPLessThanOrEqual::create(vp, newIncr);
      if (newIncr == other->increment())
         {
         if (other->hasArtificialIncrement())
            constraint->setHasArtificialIncrement();
         }
      else if (newIncr == -1*increment())
         {
         if (hasArtificialIncrement())
            constraint->setHasArtificialIncrement();
         }

      if (vp->trace())
         {
         traceMsg(vp->comp(), "      Propagating V == value %d %+d and V <= value %d %+d", relative, increment(), otherRelative, other->increment());
         traceMsg(vp->comp(), " ... value %d <= value %d %+d\n", relative, otherRelative, newIncr);
         }
      }
   else if (other->asGreaterThanOrEqual())
      {
      constraint = TR_VPGreaterThanOrEqual::create(vp, newIncr);
      if (newIncr == other->increment())
         {
         if (other->hasArtificialIncrement())
            constraint->setHasArtificialIncrement();
         }

      if (vp->trace())
         {
         traceMsg(vp->comp(), "      Propagating V == value %d %+d and V >= value %d %+d", relative, increment(), otherRelative, other->increment());
         traceMsg(vp->comp(), " ... value %d >= value %d %+d\n", relative, otherRelative, newIncr);
         }
      }
   else if (other->asNotEqual())
      {
      constraint = TR_VPNotEqual::create(vp, newIncr);
      if (vp->trace())
         {
         traceMsg(vp->comp(), "      Propagating V == value %d %+d and V != value %d %+d", relative, increment(), otherRelative, other->increment());
         traceMsg(vp->comp(), " ... value %d != value %d %+d\n", relative, otherRelative, newIncr);
         }
      }
   else
      {
      TR_ASSERT(other->asEqual(), "assertion failure");
      constraint = TR_VPEqual::create(vp, newIncr);
      if (vp->trace())
         {
         traceMsg(vp->comp(), "      Propagating V == value %d %+d and V == value %d %+d", relative, increment(), otherRelative, other->increment());
         traceMsg(vp->comp(), " ... value %d == value %d %+d\n", relative, otherRelative, newIncr);
         }
      }
   return constraint;
   }

TR_VPConstraint *TR_VPNotEqual::propagateRelativeConstraint(TR_VPRelation *other, int32_t relative, int32_t otherRelative, TR_ValuePropagation *vp)
   {
   // x != y + M and x == z + N    ==> y != z + (N-M)
   //
   if (!other->asEqual())
      return NULL;

   TR_VPConstraint *constraint;
   int32_t newIncr = other->increment() - increment();

   // newIncr cannot be TR::getMinSigned<TR::Int32>(), because when we try to negate TR::getMinSigned<TR::Int32>(), we get TR::getMinSigned<TR::Int32>() again
   if (newIncr == TR::getMinSigned<TR::Int32>())
      return NULL;

   if (increment() >= 0)
      {
      if (newIncr > other->increment())
         return NULL;
      }
   else
      {
      if (newIncr < other->increment())
         return NULL;
      }
   constraint = TR_VPNotEqual::create(vp, newIncr);
   if (vp->trace())
      {
      traceMsg(vp->comp(), "      Propagating V != value %d %+d and V == value %d %+d", relative, increment(), otherRelative, other->increment());
      traceMsg(vp->comp(), " ... value %d != value %d %+d\n", relative, otherRelative, newIncr);
      }
   return constraint;
   }



// ***************************************************************************
//
// Print methods for Value Propagation Constraints
//
// ***************************************************************************


void TR_VPConstraint::print(TR_ValuePropagation *vp)
   {
   print(vp->comp(), vp->comp()->getOutFile());
   }

void TR_VPConstraint::print(TR::Compilation *comp, TR::FILE *outFile)
   {
   if (outFile == NULL)
      return;
   trfprintf(outFile, "unknown absolute constraint");
   }

void TR_VPConstraint::print(TR::Compilation *comp, TR::FILE *outFile, int32_t relative)
   {
   if (outFile == NULL)
      return;
   trfprintf(outFile, "unknown constraint relative to value number %d", relative);
   }

void TR_VPShortConst::print(TR::Compilation * comp, TR::FILE *outFile)
   {
   if (outFile == NULL)
       return;
   if (isUnsigned())
      trfprintf(outFile, "%u US ", getLow());
   else
      trfprintf(outFile, "%d S ", getLow());

   }


void TR_VPShortRange::print(TR::Compilation * comp, TR::FILE *outFile)
   {
   if (outFile == NULL)
       return;

   if (isUnsigned())
      {
       if ((uint16_t)getLow() == TR::getMinUnsigned<TR::Int16>())
           trfprintf(outFile, "(TR::getMinUnsigned<TR::Int16>() ");
       else
           trfprintf(outFile, "(%u ", getLow());

       if ((uint16_t)getHigh() == TR::getMaxUnsigned<TR::Int16>())
           trfprintf(outFile, "to TR::getMaxUnsigned<TR::Int16>())US");
       else
           trfprintf(outFile, "to %u)US",getHigh());
      }
   else
      {
      if (getLow() == TR::getMinSigned<TR::Int16>())
         trfprintf(outFile, "(TR::getMinSigned<TR::Int16>() ");
      else
         trfprintf(outFile, "(%d ", getLow());
      if (getHigh() == TR::getMaxSigned<TR::Int16>())
         trfprintf(outFile, "to TR::getMaxSigned<TR::Int16>())S");
      else
         trfprintf(outFile, "to %d)S", getHigh());
      }
   }

void TR_VPIntConst::print(TR::Compilation *comp, TR::FILE *outFile)
   {
   if (outFile == NULL)
      return;
   if (isUnsigned())
      trfprintf(outFile, "%u UI ", getLow());
   else
      trfprintf(outFile, "%d I ", getLow());
   }

void TR_VPIntRange::print(TR::Compilation *comp, TR::FILE *outFile)
   {
   if (outFile == NULL)
      return;
   if (isUnsigned())
      {
      if ((uint32_t)getLow() == TR::getMinUnsigned<TR::Int32>())
         trfprintf(outFile, "(TR::getMinUnsigned<TR::Int32>() ");
      else
         trfprintf(outFile, "(%u ", getLow());
      if ((uint32_t)getHigh() == TR::getMaxUnsigned<TR::Int32>())
         trfprintf(outFile, "to TR::getMaxUnsigned<TR::Int32>())UI");
      else
         trfprintf(outFile, "to %u)UI", getHigh());
      }
   else
      {
      if (getLow() == TR::getMinSigned<TR::Int32>())
         trfprintf(outFile, "(TR::getMinSigned<TR::Int32>() ");
      else
         trfprintf(outFile, "(%d ", getLow());
      if (getHigh() == TR::getMaxSigned<TR::Int32>())
         trfprintf(outFile, "to TR::getMaxSigned<TR::Int32>())I");
      else
         trfprintf(outFile, "to %d)I", getHigh());
      }
   }

void TR_VPLongConst::print(TR::Compilation *comp, TR::FILE *outFile)
   {
   if (outFile == NULL)
      return;
   if (isUnsigned())
      trfprintf(outFile, UINT64_PRINTF_FORMAT " UL ", getUnsignedLong());
   else
      trfprintf(outFile, INT64_PRINTF_FORMAT " L ", getLong());
   }

void TR_VPLongRange::print(TR::Compilation *comp, TR::FILE *outFile)
   {
   if (outFile == NULL)
      return;
   if (isUnsigned())
      {
      if (getLow() == 0ULL)
         trfprintf(outFile, "(MIN_ULONG ");
      else
         trfprintf(outFile, "(" UINT64_PRINTF_FORMAT " ", (uint64_t)getLow());
      if (getHigh() == (uint64_t)(-1))
         trfprintf(outFile, "to MAX_ULONG)UL");
      else
         trfprintf(outFile, "to " UINT64_PRINTF_FORMAT ")UL", (uint64_t)getHigh());
      }
   else
      {
      if (getLow() == TR::getMinSigned<TR::Int64>())
         trfprintf(outFile, "(TR::getMinSigned<TR::Int64>() ");
      else
         trfprintf(outFile, "(" INT64_PRINTF_FORMAT " ", getLow());
      if (getHigh() == TR::getMaxSigned<TR::Int64>())
         trfprintf(outFile, "to TR::getMaxSigned<TR::Int64>())L");
      else
         trfprintf(outFile, "to " INT64_PRINTF_FORMAT ")L", getHigh());
      }

   }

void TR_VPClass::print(TR::Compilation *comp, TR::FILE *outFile)
   {
   if (outFile == NULL)
      return;
   if (_type)
      _type->print(comp, outFile);
   if (getKnownObject() && !isNonNullObject())
      trfprintf(outFile, " (maybe NULL)");
   if (_presence)
      _presence->print(comp, outFile);
   if (_arrayInfo)
      _arrayInfo->print(comp, outFile);
   if (_location)
      _location->print(comp, outFile);
   }

void TR_VPResolvedClass::print(TR::Compilation *comp, TR::FILE *outFile)
   {
   if (outFile == NULL)
      return;

   int32_t len = _len;
   const char *sig = _sig;
   if (isSpecialClass((uintptrj_t)_class))
      {
      sig = "<special>";
      len = strlen(sig);
      }

   trfprintf(outFile, "class %.*s", len, sig);
   }

void TR_VPFixedClass::print(TR::Compilation *comp, TR::FILE *outFile)
   {
   if (outFile == NULL)
      return;
   trfprintf(outFile, "fixed ");
   TR_VPResolvedClass::print(comp, outFile);
   }

static uint16_t format[] = {'C','S',':','"','%','.','*','s','"',0};
void TR_VPConstString::print(TR::Compilation * comp, TR::FILE *outFile)
   {
   if (outFile == NULL)
      return;

   trfprintf(outFile, "constant string: \"");
#ifdef J9_PROJECT_SPECIFIC
      {
      TR::VMAccessCriticalSection vpConstStringPrintCriticalSection(comp,
                                                                     TR::VMAccessCriticalSection::tryToAcquireVMAccess);
      if (vpConstStringPrintCriticalSection.hasVMAccess())
         {
         uintptrj_t string = *(uintptrj_t*) _symRef->getSymbol()->castToStaticSymbol()->getStaticAddress();
         int32_t len = comp->fej9()->getStringLength(string);
         for (int32_t i = 0; i < len; ++i)
            trfprintf(outFile, "%c", TR::Compiler->cls.getStringCharacter(comp, string, i));
         trfprintf(outFile, "\" ");
         }
      else
         trfprintf(outFile, " <could not print as no fe access> \" ");
      }
#endif
   }

void TR_VPKnownObject::print(TR::Compilation * comp, TR::FILE *outFile)
   {
   if (outFile == NULL)
      return;

   trfprintf(outFile, "known object obj%d ", _index);
   TR_VPFixedClass::print(comp, outFile);
   }

void TR_VPUnresolvedClass::print(TR::Compilation *comp, TR::FILE *outFile)
   {
   if (outFile == NULL)
      return;
   int32_t methodLen  = _method->nameLength();
   char   *methodName = _method->nameChars();
   trfprintf(outFile, "unresolved class %.*s in method %.*s", _len, _sig, methodLen, methodName);
   }

void TR_VPNullObject::print(TR::Compilation *comp, TR::FILE *outFile)
   {
   if (outFile == NULL)
      return;
   trfprintf(outFile, " (NULL)");
   }

void TR_VPNonNullObject::print(TR::Compilation *comp, TR::FILE *outFile)
   {
   if (outFile == NULL)
      return;
   trfprintf(outFile, " (non-NULL)");
   }

void TR_VPPreexistentObject::print(TR::Compilation *comp, TR::FILE *outFile)
   {
   if (outFile == NULL)
      return;
   trfprintf(outFile, " (pre-existent)");
   }

void TR_VPArrayInfo::print(TR::Compilation *comp, TR::FILE *outFile)
   {
   if (outFile == NULL)
      return;
   if (_lowBound > 0 || _highBound < TR::getMaxSigned<TR::Int32>())
      trfprintf(outFile, " (min bound %d, max bound %d)", _lowBound, _highBound);
   if (_elementSize > 0)
      trfprintf(outFile, " (array element size %d)", _elementSize);
   }

void TR_VPObjectLocation::print(TR::Compilation *comp, TR::FILE *outFile)
   {
   if (outFile == NULL)
      return;
   const int nkinds = 4;
   static TR_VPObjectLocationKind kinds[nkinds] = {  HeapObject ,  StackObject ,  JavaLangClassObject ,  J9ClassObject  };
   static const char * const      names[nkinds] = { "HeapObject", "StackObject", "JavaLangClassObject", "J9ClassObject" };
   trfprintf(outFile, " {");
   bool first = true;
   for (int i = 0; i < nkinds; i++)
      {
      if ((_kind & kinds[i]) != 0)
         {
         trfprintf(outFile, "%s%s", first ? "" : ",", names[i]);
         first = false;
         }
      }
   trfprintf(outFile, "}");
   }

void TR_VPMergedConstraints::print(TR::Compilation *comp, TR::FILE *outFile)
   {
   if (outFile == NULL)
      return;
   trfprintf(outFile, "{");
   ListElement<TR_VPConstraint> *p;
   for (p = _constraints.getListHead(); p; p = p->getNextElement())
      {
      p->getData()->print(comp, outFile);
      if (p->getNextElement())
         trfprintf(outFile, ", ");
      }
   trfprintf(outFile, "}");
   }

void TR_VPUnreachablePath::print(TR::Compilation *comp, TR::FILE *outFile)
   {
   if (outFile == NULL)
      return;
   trfprintf(outFile, "*** Unreachable Path ***");
   }

void TR_VPSync::print(TR::Compilation *comp, TR::FILE *outFile)
   {
   if (outFile == NULL)
      return;
   trfprintf(outFile, "sync has %s been emitted", (syncEmitted() == TR_yes) ? "" : "not");
   }

void TR_VPLessThanOrEqual::print(TR::Compilation *comp, TR::FILE *outFile)
   {
   if (outFile == NULL)
      return;
   trfprintf(outFile, "less than or equal to another value number");
   if (increment() > 0)
      trfprintf(outFile," + %d", increment());
   else if (increment() < 0)
      trfprintf(outFile," - %d", -increment());
   }

void TR_VPLessThanOrEqual::print(TR::Compilation *comp, TR::FILE *outFile, int32_t relative)
   {
   if (outFile == NULL)
      return;
   trfprintf(outFile, "less than or equal to value number %d", relative);
   if (increment() > 0)
      trfprintf(outFile," + %d", increment());
   else if (increment() < 0)
      trfprintf(outFile," - %d", -increment());
   }

void TR_VPGreaterThanOrEqual::print(TR::Compilation *comp, TR::FILE *outFile)
   {
   if (outFile == NULL)
      return;
   trfprintf(outFile, "greater than or equal to another value number");
   if (increment() > 0)
      trfprintf(outFile," + %d", increment());
   else if (increment() < 0)
      trfprintf(outFile," - %d", -increment());
   }

void TR_VPGreaterThanOrEqual::print(TR::Compilation *comp, TR::FILE *outFile, int32_t relative)
   {
   if (outFile == NULL)
      return;
   trfprintf(outFile, "greater than or equal to value number %d", relative);
   if (increment() > 0)
      trfprintf(outFile," + %d", increment());
   else if (increment() < 0)
      trfprintf(outFile," - %d", -increment());
   }

void TR_VPEqual::print(TR::Compilation *comp, TR::FILE *outFile)
   {
   if (outFile == NULL)
      return;
   trfprintf(outFile, "equal to another value number");
   if (increment() > 0)
      trfprintf(outFile," + %d", increment());
   else if (increment() < 0)
      trfprintf(outFile," - %d", -increment());
   }

void TR_VPEqual::print(TR::Compilation *comp, TR::FILE *outFile, int32_t relative)
   {
   if (outFile == NULL)
      return;
   trfprintf(outFile, "equal to value number %d", relative);
   if (increment() > 0)
      trfprintf(outFile," + %d", increment());
   else if (increment() < 0)
      trfprintf(outFile," - %d", -increment());
   }

void TR_VPNotEqual::print(TR::Compilation *comp, TR::FILE *outFile)
   {
   if (outFile == NULL)
      return;
   trfprintf(outFile, "not equal to another value number");
   if (increment() > 0)
      trfprintf(outFile," + %d", increment());
   else if (increment() < 0)
      trfprintf(outFile," - %d", -increment());
   }

void TR_VPNotEqual::print(TR::Compilation *comp, TR::FILE *outFile, int32_t relative)
   {
   if (outFile == NULL)
      return;
   trfprintf(outFile, "not equal to value number %d", relative);
   if (increment() > 0)
      trfprintf(outFile," + %d", increment());
   else if (increment() < 0)
      trfprintf(outFile," - %d", -increment());
   }

// ***************************************************************************
//
// Name methods for Value Propagation Constraints
//
// ***************************************************************************

const char *TR_VPShortConst::name()                 { return "ShortConst";           }
const char *TR_VPShortRange::name()                 { return "ShortRange";           }
const char *TR_VPIntConst::name()                   { return "IntConst";             }
const char *TR_VPIntRange::name()                   { return "IntRange";             }
const char *TR_VPLongConst::name()                  { return "LongConst";            }
const char *TR_VPLongRange::name()                  { return "LongRange";            }
const char *TR_VPClass::name()                      { return "Class";                }
const char *TR_VPResolvedClass::name()              { return "ResolvedClass";        }
const char *TR_VPFixedClass::name()                 { return "FixedClass";           }
const char *TR_VPConstString::name()                { return "ConstString";          }
const char *TR_VPUnresolvedClass::name()            { return "UnresolvedClass";      }
const char *TR_VPNullObject::name()                 { return "NullObject";           }
const char *TR_VPNonNullObject::name()              { return "NonNullObject";        }
const char *TR_VPPreexistentObject::name()          { return "PreexistentObject";    }
const char *TR_VPArrayInfo::name()                  { return "ArrayInfo";            }
const char *TR_VPObjectLocation::name()             { return "ObjectLocation";       }
const char *TR_VPKnownObject::name()                { return "KnownObject";          }
const char *TR_VPMergedConstraints::name()          { return "MergedConstraints";    }
const char *TR_VPUnreachablePath::name()            { return "UnreachablePath";      }
const char *TR_VPSync::name()                       { return "Sync";                 }
const char *TR_VPLessThanOrEqual::name()            { return "LessThanOrEqual";      }
const char *TR_VPGreaterThanOrEqual::name()         { return "GreaterThanOrEqual";   }
const char *TR_VPEqual::name()                      { return "Equal";                }
const char *TR_VPNotEqual::name()                   { return "NotEqual";             }

// ***************************************************************************
//
// Tracer
//
// ***************************************************************************

TR_VPConstraint::Tracer::Tracer(TR_ValuePropagation *vpArg, TR_VPConstraint *self, TR_VPConstraint *other, const char *name)
   :_vp(vpArg), _self(self), _other(other), _name(name)
   {
   if (comp()->getOption(TR_TraceVPConstraints))
      {
      traceMsg(comp(), "{{{ %s.%s\n", _self->name(), _name);
      traceMsg(comp(), "  self: ");
      _self->print(vp());
      traceMsg(comp(), "\n  other: ");
      _other->print(vp());
      traceMsg(comp(), "\n");
      }
   }

TR_VPConstraint::Tracer::~Tracer()
   {
   if (comp()->getOption(TR_TraceVPConstraints))
      {
      traceMsg(comp(), "%s.%s }}}\n", _self->name(), _name);
      }
   }

TR::Compilation *TR_VPConstraint::Tracer::comp()
   {
   return vp()->comp();
   }
