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

#include "ddr/ir/EnumUDT.hpp"

#include "ddr/config.hpp"

EnumUDT::EnumUDT(unsigned int lineNumber)
	: UDT(ENUM, 4, lineNumber)
{
};

EnumUDT::~EnumUDT()
{
	for (size_t i = 0; i < _enumMembers.size(); i++) {
		if (_enumMembers[i] == NULL) {
			ERRMSG("Null member, cannot free");
		} else {
			delete(_enumMembers[i]);
		}
	}
	_enumMembers.clear();
}

bool
EnumUDT::isAnonymousType()
{
	return (NULL != _outerUDT) && (_name.empty());
}

bool
EnumUDT::equal(Type const& type, set<Type const*> *checked) const
{
	bool ret = false;
	if (checked->find(this) != checked->end()) {
		ret = true;
	} else {
		checked->insert(this);
		EnumUDT const *enumUDT = dynamic_cast<EnumUDT const *>(&type);
		if (NULL != enumUDT) {
			bool membersEqual = _enumMembers.size() == enumUDT->_enumMembers.size();
			if (membersEqual) {
				for (size_t i = 0; i < _enumMembers.size(); i += 1) {
					if ((_enumMembers[i]->_name != enumUDT->_enumMembers[i]->_name)
						|| (_enumMembers[i]->_value != enumUDT->_enumMembers[i]->_value)
					) {
						membersEqual = false;
						break;
					}
				}
			}
			ret = (UDT::equal(type, checked) && (membersEqual));
		}
	}
	return ret;
}

void
EnumUDT::replaceType(Type *typeToReplace, Type *replaceWith)
{
	UDT::replaceType(typeToReplace, replaceWith);
}

DDR_RC
EnumUDT::scanChildInfo(Scanner *scanner, void *data)
{
	return scanner->dispatchScanChildInfo(this, data);
}

string
EnumUDT::getSymbolTypeName()
{
	return "enum";
}

DDR_RC
EnumUDT::enumerateType(BlobGenerator *blobGenerator, bool addFieldsOnly)
{
	return blobGenerator->dispatchEnumerateType(this, addFieldsOnly);
}

DDR_RC
EnumUDT::buildBlob(BlobGenerator *blobGenerator, bool addFieldsOnly, string prefix)
{
	return blobGenerator->dispatchBuildBlob(this, addFieldsOnly, prefix);
}

DDR_RC
EnumUDT::printToSuperset(SupersetGenerator *supersetGenerator, bool addFieldsOnly, string prefix = "")
{
	return supersetGenerator->dispatchPrintToSuperset(this, addFieldsOnly, prefix);
}

DDR_RC
EnumUDT::checkDuplicate(Symbol_IR *ir)
{
		if ((!this->isAnonymousType()) && (this->_enumMembers.size() > 0)) {
		string fullName = ir->getUDTname(this);
		if (ir->_fullTypeNames.end() != ir->_fullTypeNames.find(fullName)) {
			this->_isDuplicate = true;
		} else {
			ir->_fullTypeNames.insert(fullName);
		}
	}
	return DDR_RC_OK;
}
