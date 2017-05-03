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

#include "ddr/ir/Field.hpp"

#include <stdlib.h>

#include "ddr/config.hpp"
#include "ddr/ir/TypedefUDT.hpp"

Field::Field()
	: _fieldType(NULL),
	  _sizeOf(0),
	  _bitField(0),
	  _isStatic(false)
{
}

string
Field::getTypeName()
{
	return _fieldType->_name;
}

DDR_RC
Field::getBaseSymbolType(SymbolType *symbolType)
{
	DDR_RC rc = DDR_RC_OK;
	Type *type = _fieldType;
	if (NULL == type) {
		ERRMSG("fieldType is NULL");
		rc = DDR_RC_ERROR;
	} else {
		TypedefUDT *td = dynamic_cast<TypedefUDT *>(type);
		while (NULL != td) {
			type = td->_type;
			td = dynamic_cast<TypedefUDT *>(type);
		}
		if (NULL == type) {
			*symbolType = TYPEDEF;
		} else {
			*symbolType = type->getSymbolType();
		}
	}

	return rc;
}
