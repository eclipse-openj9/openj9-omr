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

#ifndef UNIONUDT_HPP
#define UNIONUDT_HPP

#include "ddr/ir/ClassType.hpp"

class UnionUDT: public ClassType
{
public:
	UnionUDT(size_t size, unsigned int lineNumber = 0);

	virtual ~UnionUDT();

	virtual DDR_RC acceptVisitor(TypeVisitor const &visitor);
	virtual string getSymbolKindName();
};

#endif /* UNIONUDT_HPP */
