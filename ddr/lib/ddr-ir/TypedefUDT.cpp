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

#include "ddr/ir/TypedefUDT.hpp"

TypedefUDT::TypedefUDT(unsigned int lineNumber)
	: UDT(TYPEDEF, 0, lineNumber), _aliasedType(NULL)
{
}

TypedefUDT::~TypedefUDT()
{
}

bool
TypedefUDT::equal(Type const& type, set<Type const*> *checked) const
{
	bool ret = false;
	if (checked->find(this) != checked->end()) {
		ret = true;
	} else {
		checked->insert(this);
		TypedefUDT const *td = dynamic_cast<TypedefUDT const *>(&type);
		if (NULL != td) {
			ret = (UDT::equal(type, checked))
				&& (_aliasedType == td->_aliasedType || *_aliasedType == *td->_aliasedType)
				&& (_modifiers == td->_modifiers);
		}
	}
	return ret;
}

void
TypedefUDT::replaceType(Type *typeToReplace, Type *replaceWith)
{
	UDT::replaceType(typeToReplace, replaceWith);
	if (_aliasedType == typeToReplace) {
		_aliasedType = replaceWith;
	}
}

DDR_RC
TypedefUDT::scanChildInfo(Scanner *scanner, void *data)
{
	return scanner->dispatchScanChildInfo(this, data);
}

DDR_RC
TypedefUDT::enumerateType(BlobGenerator *blobGenerator, bool addFieldsOnly)
{
	return blobGenerator->dispatchEnumerateType(this, addFieldsOnly);
}

DDR_RC
TypedefUDT::buildBlob(BlobGenerator *blobGenerator, bool addFieldsOnly, string prefix)
{
	return blobGenerator->dispatchBuildBlob(this, addFieldsOnly, prefix);
}

DDR_RC
TypedefUDT::printToSuperset(SupersetGenerator *supersetGenerator, bool addFieldsOnly, string prefix)
{
	return supersetGenerator->dispatchPrintToSuperset(this, addFieldsOnly, prefix);
}

DDR_RC
TypedefUDT::checkDuplicate(Symbol_IR *ir)
{
	/* No-op: since TypedefUDTs aren't printed, there's no need to check if they're duplicates either */
	return DDR_RC_OK;
}
