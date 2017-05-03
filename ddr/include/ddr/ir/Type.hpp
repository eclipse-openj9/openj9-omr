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

#include "ddr/config.hpp"
#include "ddr/blobgen/genBlob.hpp"
#include "ddr/scanner/Scanner.hpp"

using std::set;
using std::string;

enum SymbolType {
	CLASS,
	STRUCT,
	ENUM,
	UNION,
	BASE,
	TYPEDEF,
	NAMESPACE
};

class Type
{
protected:
	SymbolType _symbolType;

public:
	std::string _name;
	size_t _sizeOf; /* Size of type in bytes */

	Type(SymbolType symbolType, size_t size);
	virtual ~Type();

	SymbolType getSymbolType();
	virtual bool isAnonymousType();
	friend bool operator==(Type const& lhs, Type const& rhs);
	virtual bool equal(Type const& type, set<Type const*> *checked) const;
	virtual void replaceType(Type *typeToReplace, Type *replaceWith);

	virtual string getFullName();
	virtual string getSymbolTypeName();

	/* Visitor pattern functions to allow the scanner/generator to dispatch functionality based on type. */
	virtual DDR_RC scanChildInfo(Scanner *scanner, void *data);
	virtual DDR_RC enumerateType(BlobGenerator *blobGenerator, bool addFieldsOnly);
	virtual DDR_RC buildBlob(BlobGenerator *blobGenerator, bool addFieldsOnly, string prefix);
	virtual DDR_RC printToSuperset(SupersetGenerator *supersetGenerator, bool addFieldsOnly, string prefix);
};

#endif /* TYPE_HPP */
