/*******************************************************************************
 *
 * (c) Copyright IBM Corp. 2016
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

#ifndef TYPE_HPP
#define TYPE_HPP

#include <set>
#include <string>

#include "ddr/blobgen/genBlob.hpp"
#include "ddr/config.hpp"
#include "ddr/ir/Symbol_IR.hpp"
#include "ddr/ir/TypeVisitor.hpp"
#include "ddr/scanner/Scanner.hpp"

using std::set;
using std::string;

class ClassUDT;
class ClassType;
class EnumUDT;
class Macro;
class NamespaceUDT;
class TypedefUDT;
class UDT;
class UnionUDT;
struct FieldOverride;

class Type
{
public:
	std::string _name;
	size_t _sizeOf; /* Size of type in bytes */
	bool _isDuplicate;

	Type(size_t size);
	virtual ~Type();

	virtual bool isAnonymousType();

	virtual string getFullName();
	virtual string getSymbolKindName();

	/* Visitor pattern function to allow the scanner/generator/IR to dispatch functionality based on type. */
	virtual DDR_RC acceptVisitor(TypeVisitor const &visitor);

	virtual void checkDuplicate(Symbol_IR *ir);
	virtual NamespaceUDT *getNamespace();
	virtual size_t getPointerCount();
	virtual size_t getArrayDimensions();
	virtual void computeFieldOffsets();
	virtual void addMacro(Macro *macro);
	virtual std::vector<UDT *> *getSubUDTS();
	virtual void renameFieldsAndMacros(FieldOverride fieldOverride, Type *replacementType);
	virtual Type *getBaseType();

	bool operator==(Type const & rhs) const;
	virtual bool compareToClass(ClassUDT const &) const;
	virtual bool compareToClasstype(ClassType const &) const;
	virtual bool compareToEnum(EnumUDT const &) const;
	virtual bool compareToNamespace(NamespaceUDT const &) const;
	virtual bool compareToType(Type const &) const;
	virtual bool compareToTypedef(TypedefUDT const &) const;
	virtual bool compareToUDT(UDT const &) const;
	virtual bool compareToUnion(UnionUDT const &) const;
};

#endif /* TYPE_HPP */
