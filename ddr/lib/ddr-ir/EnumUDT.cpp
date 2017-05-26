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
	return (NULL != _outerNamespace) && (_name.empty());
}

DDR_RC
EnumUDT::scanChildInfo(Scanner *scanner, void *data)
{
	return scanner->dispatchScanChildInfo(this, data);
}

string
EnumUDT::getSymbolKindName()
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
EnumUDT::acceptVisitor(TypeVisitor const &visitor)
{
	return visitor.visitType(this);
}
