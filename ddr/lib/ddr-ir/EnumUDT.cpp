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
	: UDT(4, lineNumber)
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

string
EnumUDT::getSymbolKindName()
{
	return "enum";
}

DDR_RC
EnumUDT::acceptVisitor(TypeVisitor const &visitor)
{
	return visitor.visitType(this);
}

bool
EnumUDT::operator==(Type const & rhs) const
{
	return rhs.compareToEnum(*this);
}

bool
EnumUDT::compareToEnum(EnumUDT const &other) const
{
	bool enumMembersEqual = _enumMembers.size() == other._enumMembers.size();
	vector<EnumMember *>::const_iterator it2 = other._enumMembers.begin();
	for (vector<EnumMember *>::const_iterator it = _enumMembers.begin();
		it != _enumMembers.end() && it2 != other._enumMembers.end() && enumMembersEqual;
		++ it, ++ it2) {
		enumMembersEqual = ((*it)->_name == (*it2)->_name) && ((*it)->_value == (*it2)->_value);
	}
	return compareToUDT(other)
		&& enumMembersEqual;
}
