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

#include "ddr/ir/UnionUDT.hpp"

UnionUDT::UnionUDT(size_t size, unsigned int lineNumber)
	: ClassType(UNION, size, lineNumber)
{
}

UnionUDT::~UnionUDT() {};

DDR_RC
UnionUDT::scanChildInfo(Scanner *scanner, void *data)
{
	return scanner->dispatchScanChildInfo(this, data);
}

DDR_RC
UnionUDT::enumerateType(BlobGenerator *blobGenerator, bool addFieldsOnly)
{
	return blobGenerator->dispatchEnumerateType(this, addFieldsOnly);
}

DDR_RC
UnionUDT::buildBlob(BlobGenerator *blobGenerator, bool addFieldsOnly, string prefix)
{
	return blobGenerator->dispatchBuildBlob(this, addFieldsOnly, prefix);
}

DDR_RC
UnionUDT::printToSuperset(SupersetGenerator *supersetGenerator, bool addFieldsOnly, string prefix = "")
{
	return supersetGenerator->dispatchPrintToSuperset(this, addFieldsOnly, prefix);
}

DDR_RC
UnionUDT::checkDuplicate(Symbol_IR *ir)
{
	DDR_RC rc = DDR_RC_OK;
	if ((!this->isAnonymousType()) && (this->_fieldMembers.size() > 0)) {
		string fullName = ir->getUDTname(this);
		if (ir->_fullTypeNames.end() != ir->_fullTypeNames.find(fullName)) {
			this->_isDuplicate = true;
		} else {
			ir->_fullTypeNames.insert(fullName);
		}
	}

	/* Still check the sub UDTs. This is because a previous duplicate of this UDT could have
	 * contained an empty version of the same sub UDT as this one, which was skipped over due
	 * to its emptiness.
	 */
	for (vector<UDT *>::iterator v = this->_subUDTs.begin(); v != this->_subUDTs.end(); ++v) {
		rc = (*v)->checkDuplicate(ir);
		if (DDR_RC_OK != rc) {
			break;
		}
	}
	return rc;
}
