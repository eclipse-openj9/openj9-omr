/*******************************************************************************
 *
 * (c) Copyright IBM Corp. 2015, 2016
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

#ifndef ENUMUDT_HPP
#define ENUMUDT_HPP

#include "ddr/ir/EnumMember.hpp"
#include "ddr/ir/UDT.hpp"

using std::vector;

class EnumUDT: public UDT
{
public:
	vector<EnumMember *> _enumMembers;

	EnumUDT(unsigned int lineNumber = 0);
	virtual ~EnumUDT();

	bool isAnonymousType();
	virtual bool equal(Type const& type, set<Type const*> *checked) const;
	virtual void replaceType(Type *typeToReplace, Type *replaceWith);
	virtual string getSymbolKindName();

	virtual DDR_RC scanChildInfo(Scanner *scanner, void *data);
	virtual DDR_RC enumerateType(BlobGenerator *blobGenerator, bool addFieldsOnly);
	virtual DDR_RC buildBlob(BlobGenerator *blobGenerator, bool addFieldsOnly, string prefix);
	virtual DDR_RC printToSuperset(SupersetGenerator *supersetGenerator, bool addFieldsOnly, string prefix);
	virtual DDR_RC checkDuplicate(Symbol_IR *ir);
};

#endif /* ENUMUDT_HPP */
