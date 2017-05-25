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

#include "ddr/ir/NamespaceUDT.hpp"

NamespaceUDT::NamespaceUDT(unsigned int lineNumber)
	: UDT(NAMESPACE, 0, lineNumber)
{
}

NamespaceUDT::~NamespaceUDT()
{
	for (size_t i = 0; i < _subUDTs.size(); i += 1) {
		delete(_subUDTs[i]);
	}
	_subUDTs.clear();
}

DDR_RC
NamespaceUDT::scanChildInfo(Scanner *scanner, void *data)
{
	return scanner->dispatchScanChildInfo(this, data);
}

DDR_RC
NamespaceUDT::enumerateType(BlobGenerator *blobGenerator, bool addFieldsOnly)
{
	return blobGenerator->dispatchEnumerateType(this, addFieldsOnly);
}

DDR_RC
NamespaceUDT::buildBlob(BlobGenerator *blobGenerator, bool addFieldsOnly, string prefix)
{
	return blobGenerator->dispatchBuildBlob(this, addFieldsOnly, prefix);
}

DDR_RC
NamespaceUDT::printToSuperset(SupersetGenerator *supersetGenerator, bool addFieldsOnly, string prefix = "")
{
	return supersetGenerator->dispatchPrintToSuperset(this, addFieldsOnly, prefix);
}

void
NamespaceUDT::checkDuplicate(Symbol_IR *ir)
{
	UDT::checkDuplicate(ir);

	/* Still check the sub UDTs. This is because a previous duplicate of this UDT could have
	 * contained an empty version of the same sub UDT as this one, which was skipped over due
	 * to its emptiness.
	 */
	for (vector<UDT *>::iterator v = this->_subUDTs.begin(); v != this->_subUDTs.end(); ++v) {
		(*v)->checkDuplicate(ir);
	}
}

string
NamespaceUDT::getSymbolKindName()
{
	return "namespace";
}

void
NamespaceUDT::computeFieldOffsets()
{
	for (vector<UDT *>::iterator it = _subUDTs.begin(); it != _subUDTs.end(); it += 1) {
		(*it)->computeFieldOffsets();
	}
}

void
NamespaceUDT::addMacro(Macro *macro)
{
	_macros.push_back(*macro);
}

std::vector<UDT *> *
NamespaceUDT::getSubUDTS()
{
	return &_subUDTs;
}

void
NamespaceUDT::renameFieldsAndMacros(FieldOverride fieldOverride, Type *replacementType)
{
	if (!fieldOverride.isTypeOverride) {
		for (vector<Macro>::iterator it = _macros.begin(); it != _macros.end(); it += 1) {
			if (it->_name == fieldOverride.fieldName) {
				it->_name = fieldOverride.overrideName;
			}
		}
	}
	for (vector<UDT *>::iterator it = _subUDTs.begin(); it != _subUDTs.end(); it += 1) {
		(*it)->renameFieldsAndMacros(fieldOverride, replacementType);
	}		
}
