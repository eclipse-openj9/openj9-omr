/*******************************************************************************
 * Copyright (c) 2015, 2018 IBM Corp. and others
 *
 * This program and the accompanying materials are made available under
 * the terms of the Eclipse Public License 2.0 which accompanies this
 * distribution and is available at http://eclipse.org/legal/epl-2.0
 * or the Apache License, Version 2.0 which accompanies this distribution
 * and is available at https://www.apache.org/licenses/LICENSE-2.0.
 *
 * This Source Code may also be made available under the following Secondary
 * Licenses when the conditions for such availability set forth in the
 * Eclipse Public License, v. 2.0 are satisfied: GNU General Public License,
 * version 2 with the GNU Classpath Exception [1] and GNU General Public
 * License, version 2 with the OpenJDK Assembly Exception [2].
 *
 * [1] https://www.gnu.org/software/classpath/license.html
 * [2] http://openjdk.java.net/legal/assembly-exception.html
 *
 * SPDX-License-Identifier: EPL-2.0 OR Apache-2.0 OR GPL-2.0 WITH Classpath-exception-2.0 OR LicenseRef-GPL-2.0 WITH Assembly-exception
 *******************************************************************************/

#include "ddr/ir/Macro.hpp"

#include "ddr/std/sstream.hpp"
#include <ctype.h>
#include <string.h>
#include <stdint.h>
#include <algorithm>
#include <vector>

using std::string;
using std::stringstream;

enum Symbol {
	OR_OR,		/* || */
	AND_AND,	/* && */
	OR,			/* | */
	XOR,		/* ^ */
	AND,		/* & */
	EQ_EQ,		/* == */
	NOT_EQ,		/* != */
	GT,			/* > */
	LT,			/* < */
	GT_EQ,		/* >= */
	LT_EQ,		/* <= */
	GT_GT,		/* >> */
	LT_LT,		/* << */
	PLUS,		/* + */
	MINUS,		/* - */
	MULT,		/* * */
	DIV,		/* / */
	REM,		/* % */
	NOT,		/* ! */
	TILDE,		/* ~ */
	LPAREN,		/* ( */
	RPAREN,		/* ) */
	END			/* '\0' or null terminator */
};

class MacroScanner
{
private:
	char const *_cursor;

	static bool isadigit(char c, int base);
	static int digitvalue(char c);

public:
	MacroScanner(char const *cString) :
		_cursor(cString)
	{
	}

	bool validTypeCast(bool *isSigned, size_t *bitWidth);
	bool atSymbol(Symbol sym);
	bool readNumber(int64_t *ret);
};

class MacroParser
{
/* data members */
private:
	MacroScanner _scanner; /*handles reading characters from expression string*/

/* function members */
private:
	bool logicalOr(int64_t *ret);
	bool logicalAnd(int64_t *ret);
	bool bitwiseOr(int64_t *ret);
	bool bitwiseXor(int64_t *ret);
	bool bitwiseAnd(int64_t *ret);
	bool equalNeq(int64_t *ret);
	bool comparison(int64_t *ret);
	bool bitshift(int64_t *ret);
	bool addSub(int64_t *ret);
	bool multDivRem(int64_t *ret);
	bool unaryExpression(int64_t *ret);
	bool parentheses(int64_t *ret);
	bool validSingleTerm(int64_t *ret);

public:
	MacroParser(char const *cExpression) :
		_scanner(cExpression)
	{
	}

	bool evaluate(int64_t *ret);
};

/**
 * Helper function to read the value of a number from a string, this checks if
 * a character is a digit in a specified base
 *
 * @param[in] c: character being checked
 * @param[in] base: the base used to check if the char is valid
 *
 * @return: if the character is a valid digit in the specified base
 */
bool
MacroScanner::isadigit(char c, int base)
{
	if (base > 10) {
		return ((c >= '0') && (c <= '9')) || ((c >= 'a') && (c < ('a' + base - 10)))
				|| ((c >= 'A') && (c < ('A' + base - 10)));
	} else {
		return (c >= '0') && (c < ('0' + base));
	}
}

/**
 * Helper function to read the value of a number from a string, this gets the
 * value of a single digit, assuming digits go 0-9 then A-Z case-insensitive
 * (36 possible digits)
 *
 * @param[in] c: digit whose value is sought
 *
 * @return: integer value of the digit
 */
int
MacroScanner::digitvalue(char c)
{
	if ((c >= 'a') && (c <= 'z')) {
		return 10 + c - 'a';
	} else if ((c >= 'A') && (c <= 'Z')) {
		return 10 + c - 'A';
	} else {
		return c - '0';
	}
}

/**
 * Checks if the cursor is at a valid set of type cast expressions, after
 * encountering a left parenthesis. If it is, determine the signedness and bit
 * width of the type cast. If not, move the cursor back to the left parenthesis.
 *
 * @param[out] isSigned: the signedness of the resulting typecast
 * @param[out] bitWidth: the bit width of the resulting typecast
 *
 * @return: if the typecast expression is valid
 */
bool
MacroScanner::validTypeCast(bool *isSigned, size_t *bitWidth)
{
	static const string fixedWidthTypes[] = {"int8_t", "int16_t", "int32_t", "int64_t",
	 "uint8_t", "uint16_t", "uint32_t", "uint64_t",
	 "intptr_t", "uintptr_t"};

	if (!atSymbol(LPAREN)) {
		return false;
	}

	char const *typecastStart = _cursor;
	_cursor = strchr(_cursor, ')');
	if (NULL == _cursor) {
		_cursor = typecastStart - 1;
		return false;
	}

	string typeExpr(typecastStart, _cursor);
	stringstream ss(typeExpr);
	std::vector<string> types;
	string word;
	bool validTypes = false;

	while (ss >> word) {
		types.push_back(word);
	}
	if (ss.eof()) {
		if (1 == types.size()) {
			const string *found = std::find(fixedWidthTypes, fixedWidthTypes + 10, types.front());
			if ((fixedWidthTypes + 0) == found) {
				*bitWidth = 8;
				*isSigned = true;
				validTypes = true;
			} else if ((fixedWidthTypes + 1) == found) {
				*bitWidth = 16;
				*isSigned = true;
				validTypes = true;
			} else if ((fixedWidthTypes + 2) == found) {
				*bitWidth = 32;
				*isSigned = true;
				validTypes = true;
			} else if ((fixedWidthTypes + 3) == found) {
				*bitWidth = 64;
				*isSigned = true;
				validTypes = true;
			} else if ((fixedWidthTypes + 4) == found) {
				*bitWidth = 8;
				*isSigned = false;
				validTypes = true;
			} else if ((fixedWidthTypes + 5) == found) {
				*bitWidth = 16;
				*isSigned = false;
				validTypes = true;
			} else if ((fixedWidthTypes + 6) == found) {
				*bitWidth = 32;
				*isSigned = false;
				validTypes = true;
			} else if ((fixedWidthTypes + 7) == found) {
				*bitWidth = 64;
				*isSigned = false;
				validTypes = true;
			} else if ((fixedWidthTypes + 8) == found) {
				*bitWidth = sizeof(intptr_t);
				*isSigned = true;
				validTypes = true;
			} else if ((fixedWidthTypes + 9) == found) {
				*bitWidth = sizeof(uintptr_t);
				*isSigned = false;
				validTypes = true;
			}
			if (validTypes && atSymbol(RPAREN)) {
				return validTypes;
			}
		}

		/* C standard allows types and type modifiers in any order,
		 * so we count the number of each type and verify it is correct
		 */
		size_t numChar = std::count(types.begin(), types.end(), "char");
		size_t numShort = std::count(types.begin(), types.end(), "short");
		size_t numInt = std::count(types.begin(), types.end(), "int");
		size_t numLong = std::count(types.begin(), types.end(), "long");
		size_t numSigned = std::count(types.begin(), types.end(), "signed");
		size_t numUnsigned = std::count(types.begin(), types.end(), "unsigned");

		/* total count of valid types should equal the size of types vector */
		if (types.size() == (numChar + numShort + numInt + numLong + numSigned + numUnsigned)) {
			if (numUnsigned > 0) {
				*isSigned = false;
			} else {
				*isSigned = true;
			}
			/* (1 >= numSigned + numUnsigned) checks that there can only be one
			 *  of 'unsigned' or 'signed' in the types
			 */
			if ((1 == numChar) && (1 >= numSigned + numUnsigned) && (0 == numShort)
					&& (0 == numInt) && (0 == numLong)) {
				*bitWidth = sizeof(char) * 8;
				validTypes = true;
			} else if ((1 == numShort) && (1 >= numSigned + numUnsigned)
					&& (0 == numChar) && (1 >= numInt) && (0 == numLong)) {
				*bitWidth = sizeof(short) * 8;
				validTypes = true;
			} else if ((1 == numInt) && (1 >= numSigned + numUnsigned)
					&& (0 == numChar) && (0 == numShort) && (0 == numLong)) {
				*bitWidth = sizeof(int) * 8;
				validTypes = true;
			} else if ((1 == numLong) && (1 >= numSigned + numUnsigned)
					&& (0 == numChar) && (0 == numShort) && (1 >= numInt)) {
				*bitWidth = sizeof(long) * 8;
				validTypes = true;
			} else if ((2 == numLong) && (1 >= numSigned + numUnsigned)
					&& (0 == numChar) && (0 == numShort) && (1 >= numInt)) {
				*bitWidth = sizeof(long long) * 8;
				validTypes = true;
			} else if ((1 >= numSigned + numUnsigned) && (0 == numChar)
					&& (0 == numShort) && (0 == numInt) && (0 == numLong)) {
				*bitWidth = 0;
				validTypes = true;
			}
		}
	}
	if (!(validTypes && atSymbol(RPAREN))) {
		_cursor = typecastStart - 1;
	}
	return validTypes;
}

/**
 * Check if the cursor is at a specific symbol, and if it is, consume the
 * symbol and move the cursor forward.
 */
bool
MacroScanner::atSymbol(Symbol sym)
{
	while (isspace(_cursor[0])) {
		_cursor += 1;
	}

	switch(sym) {
	case OR_OR:
		if (0 == strncmp(_cursor, "||", 2)) {
			_cursor += 2;
			return true;
		}
		break;
	case AND_AND:
		if (0 == strncmp(_cursor, "&&", 2)) {
			_cursor += 2;
			return true;
		}
		break;
	case OR:
		if (('|' == _cursor[0]) && ('|' != _cursor[1]) && ('=' != _cursor[1])) {
			_cursor += 1;
			return true;
		}
		break;
	case XOR:
		if ('^' == _cursor[0] && ('=' != _cursor[1])) {
			_cursor += 1;
			return true;
		}
		break;
	case AND:
		if (('&' == _cursor[0]) && ('&' != _cursor[1]) && ('=' != _cursor[1])) {
			_cursor += 1;
			return true;
		}
		break;
	case EQ_EQ:
		if (0 == strncmp(_cursor, "==", 2)) {
			_cursor += 2;
			return true;
		}
		break;
	case NOT_EQ:
		if (0 == strncmp(_cursor, "!=", 2)) {
			_cursor += 2;
			return true;
		}
		break;
	case GT:
		if (('>' == _cursor[0]) && ('>' != _cursor[1]) && ('=' != _cursor[1])) {
			_cursor += 1;
			return true;
		}
		break;
	case LT:
		if (('<' == _cursor[0]) && ('<' != _cursor[1]) && ('=' != _cursor[1])) {
			_cursor += 1;
			return true;
		}
		break;
	case GT_EQ:
		if (0 == strncmp(_cursor, ">=", 2)) {
			_cursor += 2;
			return true;
		}
		break;
	case LT_EQ:
		if (0 == strncmp(_cursor, "<=", 2)) {
			_cursor += 2;
			return true;
		}
		break;
	case GT_GT:
		if (0 == strncmp(_cursor, ">>", 2) && ('=' != _cursor[2])) {
			_cursor += 2;
			return true;
		}
		break;
	case LT_LT:
		if (0 == strncmp(_cursor, "<<", 2) && ('=' != _cursor[2])) {
			_cursor += 2;
			return true;
		}
		break;
	case PLUS:
		if (('+' == _cursor[0]) && ('+' != _cursor[1]) && ('=' != _cursor[1])) {
			_cursor += 1;
			return true;
		}
		break;
	case MINUS:
		if (('-' == _cursor[0]) && ('-' != _cursor[1]) && ('=' != _cursor[1])) {
			_cursor += 1;
			return true;
		}
		break;
	case MULT:
		if (('*' == _cursor[0]) && ('=' != _cursor[1])) {
			_cursor += 1;
			return true;
		}
		break;
	case DIV:
		if (('/' == _cursor[0]) && ('=' != _cursor[1])) {
			_cursor += 1;
			return true;
		}
		break;
	case REM:
		if (('%' == _cursor[0]) && ('=' != _cursor[1])) {
			_cursor += 1;
			return true;
		}
		break;
	case NOT:
		if (('!' == _cursor[0]) && ('=' != _cursor[1])) {
			_cursor += 1;
			return true;
		}
		break;
	case TILDE:
		if ('~' == _cursor[0]) {
			_cursor += 1;
			return true;
		}
		break;
	case LPAREN:
		if ('(' == _cursor[0]) {
			_cursor += 1;
			return true;
		}
		break;
	case RPAREN:
		if (')' == _cursor[0]) {
			_cursor += 1;
			return true;
		}
		break;
	case END:
		return '\0' == _cursor[0];
	default:
		break;
	}
	return false;
}

/**
 * Reads an integer number starting at the cursor in the expression string.
 * Number is read as a 64bit integer.
 */
bool
MacroScanner::readNumber(int64_t *ret)
{
	uint64_t retval = 0;
	int base = 0;

	while (isspace(_cursor[0])) {
		_cursor += 1;
	}

	if (!(isdigit(_cursor[0]))) {
		return false;
	}

	if ('0' == _cursor[0]) {
		_cursor += 1;
		if (('b' == _cursor[0]) || ('B' == _cursor[0])) {
			base = 2;
			_cursor += 1;
		} else if (('x' == _cursor[0])  || ('X' == _cursor[0])) {
			base = 16;
			_cursor += 1;
		} else {
			base = 8;
		}
	} else {
		base = 10;
	}

	while (isadigit(_cursor[0], base)) {
		retval *= base;
		retval += digitvalue(_cursor[0]);
		_cursor += 1;
	}

	/* type suffixes do nothing since we treat each literal as an uint64_t.
	 * so we just verify the suffix is correct
	 */
	bool seenL = false;
	bool seenU = false;
	for (int i = 0; _cursor[0] && (i < 3); i++, _cursor++) {
		if (('l' == _cursor[0]) || ('L' == _cursor[0])) {
			if (seenL) {
				return false;
			}
			seenL = true;
			//case of 'L's must match
			if (_cursor[0] == _cursor[1]) {
				_cursor +=1;
			} else if (('l' == _cursor[1]) || ('L' == _cursor[1])) {
				return false;
			}
		} else if (('u' == _cursor[0]) || ('U' == _cursor[0])){
			if (seenU) {
				return false;
			}
			seenU = true;
		} else {
			break;
		}
	}

	*ret = retval;
	return true;
}

/**
 * Evaluates a constant expression found in a macro. The parser assumes that
 * all values have all been resolved into constants. Thus it does not handle
 * any of the operators that require an lvalue (i.e. increment).
 *
 * @param[out] ret: the integer value the expression evaluates to
 *
 * @return: whether the expression is valid or not
 */
bool
MacroParser::evaluate(int64_t *ret)
{
	return logicalOr(ret) && _scanner.atSymbol(END);
}

/**
 * Evaluates a logical or expression (a || b) by recursively calling the
 * functions for evaluating expressions with higher precedence.
 *
 * @param[out] ret: the integer value the logical or expression evaluates to
 *
 * @return: whether at the current location the expression can be evaluated
 */
bool
MacroParser::logicalOr(int64_t *ret)
{
	int64_t term2 = 0;
	if (logicalAnd(ret)) {
		for (;;) {
			if (_scanner.atSymbol(OR_OR)) {
				if (logicalAnd(&term2)) {
					*ret = *ret || term2;
				} else {
					return false;
				}
			} else {
				break;
			}
		}
		return true;
	}
	return false;
}

/**
 * Evaluates a logical and expression (a && b) by recursively calling the
 * functions for evaluating expressions with higher precedence.
 *
 * @param[out] ret: the integer value the logical and expression evaluates to
 *
 * @return: whether at the current location the expression can be evaluated
 */
bool
MacroParser::logicalAnd(int64_t *ret)
{
	int64_t term2 = 0;
	if (bitwiseOr(ret)) {
		for (;;) {
			if (_scanner.atSymbol(AND_AND)) {
				if (bitwiseOr(&term2)) {
					*ret = *ret && term2;
				} else {
					return false;
				}
			} else {
				break;
			}
		}
		return true;
	}
	return false;
}

/**
 * Evaluates a bitwise or expression (a | b) by recursively calling the
 * functions for evaluating expressions with higher precedence.
 *
 * @param[out] ret: the integer value the bitwise or expression evaluates to
 *
 * @return: whether at the current location the expression can be evaluated
 */
bool
MacroParser::bitwiseOr(int64_t *ret)
{
	int64_t term2 = 0;
	if (bitwiseXor(ret)) {
		for (;;) {
			if (_scanner.atSymbol(OR)) {
				if (bitwiseXor(&term2)) {
					*ret = *ret | term2;
				} else {
					return false;
				}
			} else {
				break;
			}
		}
		return true;
	}
	return false;
}

/**
 * Evaluates a bitwise xor expression (a ^ b) by recursively calling the
 * functions for evaluating expressions with higher precedence.
 *
 * @param[out] ret: the integer value the bitwise xor expression evaluates to
 *
 * @return: whether at the current location the expression can be evaluated
 */
bool
MacroParser::bitwiseXor(int64_t *ret)
{
	int64_t term2 = 0;
	if (bitwiseAnd(ret)) {
		for (;;) {
			if (_scanner.atSymbol(XOR)) {
				if (bitwiseAnd(&term2)) {
					*ret = *ret ^ term2;
				} else {
					return false;
				}
			} else {
				break;
			}
		}
		return true;
	}
	return false;
}

/**
 * Evaluates a bitwise and expression (a & b) by recursively calling the
 * functions for evaluating expressions with higher precedence.
 *
 * @param[out] ret: the integer value the bitwise and expression evaluates to
 *
 * @return: whether at the current location the expression can be evaluated
 */
bool
MacroParser::bitwiseAnd(int64_t *ret)
{
	int64_t term2 = 0;
	if (equalNeq(ret)) {
		for (;;) {
			if (_scanner.atSymbol(AND)) {
				if (equalNeq(&term2)) {
					*ret = *ret & term2;
				} else {
					return false;
				}
			} else {
				break;
			}
		}
		return true;
	}
	return false;
}

/**
 * Evaluates the 2 expressions: equality (a == b), and inequality (a != b),
 * which have the same operator precedence, by recursively calling the
 * functions for evaluating expressions with higher precedence.
 *
 * @param[out] ret: the integer value the logical and expression evaluates to
 *
 * @return: whether at the current location the expression can be evaluated
 */
bool
MacroParser::equalNeq(int64_t *ret)
{
	int64_t term2 = 0;
	if (comparison(ret)) {
		for (;;) {
			if (_scanner.atSymbol(EQ_EQ)) {
				if (comparison(&term2)) {
					*ret = *ret == term2;
				} else {
					return false;
				}
			} else if (_scanner.atSymbol(NOT_EQ)) {
				if (comparison(&term2)) {
					*ret = *ret != term2;
				} else {
					return false;
				}
			} else {
				break;
			}
		}
		return true;
	}
	return false;
}

/**
 * Evaluates the comparison expressions: greater than (a > b), less than (a < b),
 * greater than or equal to (a >= b), less than or equal to (a <= b),
 * which have the same operator precedence, by recursively calling the
 * functions for evaluating expressions with higher precedence.
 *
 * @param[out] ret: the integer value the logical and expression evaluates to
 *
 * @return: whether at the current location the expression can be evaluated
 */
bool
MacroParser::comparison(int64_t *ret)
{
	int64_t term2 = 0;
	if (bitshift(ret)) {
		for (;;) {
			if (_scanner.atSymbol(GT_EQ)) {
				if (bitshift(&term2)) {
					*ret = *ret >= term2;
				} else {
					return false;
				}
			} else if (_scanner.atSymbol(LT_EQ)) {
				if (bitshift(&term2)) {
					*ret = *ret <= term2;
				} else {
					return false;
				}
			} else if (_scanner.atSymbol(GT)) {
				if (bitshift(&term2)) {
					*ret = *ret > term2;
				} else {
					return false;
				}
			} else if (_scanner.atSymbol(LT)) {
				if (bitshift(&term2)) {
					*ret = *ret < term2;
				} else {
					return false;
				}
			} else {
				break;
			}
		}
		return true;
	}
	return false;
}

/**
 * Evaluates the bitshift expressions: shift left (a << b), and shift right
 * (a >> b), which have the same operator precedence, by recursively calling
 * the functions for evaluating expressions with higher precedence.
 *
 * @param[out] ret: the integer value the logical and expression evaluates to
 *
 * @return: whether at the current location the expression can be evaluated
 */
bool
MacroParser::bitshift(int64_t *ret)
{
	int64_t term2 = 0;
	if (addSub(ret)) {
		for (;;) {
			if (_scanner.atSymbol(GT_GT)) {
				if (addSub(&term2)) {
					*ret = *ret >> term2;
				} else {
					return false;
				}
			} else if (_scanner.atSymbol(LT_LT)) {
				if (addSub(&term2)) {
					*ret = *ret << term2;
				} else {
					return false;
				}
			} else {
				break;
			}
		}
		return true;
	}
	return false;
}

/**
 * Evaluates the addition expression (a + b), and the subtraction expression
 * (a - b), which have the same operator precedence, by recursively calling
 * the functions for evaluating expressions with higher precedence.
 *
 * @param[out] ret: the integer value the logical and expression evaluates to
 *
 * @return: whether at the current location the expression can be evaluated
 */
bool
MacroParser::addSub(int64_t *ret)
{
	int64_t term2 = 0;
	if (multDivRem(ret)) {
		for (;;) {
			if (_scanner.atSymbol(PLUS)) {
				if (multDivRem(&term2)) {
					*ret = *ret + term2;
				} else {
					return false;
				}
			} else if (_scanner.atSymbol(MINUS)) {
				if (multDivRem(&term2)) {
					*ret = *ret - term2;
				} else {
					return false;
				}
			} else {
				break;
			}
		}
		return true;
	}
	return false;
}

/**
 * Evaluates the multiplication (a * b), division (a / b), and remainder
 * (a % b) expressions, which have the same operator precedence, by recursively
 * calling the functions for evaluating expressions with higher precedence.
 * Since this is the highest precedence binary expression, it will attempt to
 * evaluate its operands as either unary expressions, parenthesized
 * expressions, or numbers.
 *
 * @param[out] ret: the integer value the logical and expression evaluates to
 *
 * @return: whether at the current location the expression can be evaluated
 */
bool
MacroParser::multDivRem(int64_t *ret)
{
	int64_t term2 = 0;
	if (validSingleTerm(ret)) {
		for (;;) {
			if (_scanner.atSymbol(MULT)) {
				if (validSingleTerm(&term2)) {
					*ret = *ret * term2;
				} else {
					return false;
				}
			} else if (_scanner.atSymbol(DIV)) {
				if (validSingleTerm(&term2)) {
					*ret = *ret / term2;
				} else {
					return false;
				}
			} else if (_scanner.atSymbol(REM)) {
				if (validSingleTerm(&term2)) {
					*ret = *ret % term2;
				} else {
					return false;
				}
			} else {
				break;
			}
		}
		return true;
	}
	return false;
}

/**
 * Evaluates a unary expression from the current index in the expression
 * string recursively.
 *
 * @param[out] ret: the integer value the unary expression evaluates to
 *
 * @return: whether at the current location can be evaluated as a unary
 * 			expression
 */
bool
MacroParser::unaryExpression(int64_t *ret)
{
	int64_t operand = 0;
	size_t bitWidth = 0;
	bool isSigned = true;
	if (_scanner.atSymbol(PLUS)) {
		if (validSingleTerm(&operand)) {
			*ret = +operand;
			return true;
		}
	} else if (_scanner.atSymbol(MINUS)) {
		if (validSingleTerm(&operand)) {
			*ret = -operand;
			return true;
		}
	} else if (_scanner.atSymbol(NOT)) {
		if (validSingleTerm(&operand)) {
			*ret = !operand;
			return true;
		}
	} else if (_scanner.atSymbol(TILDE)) {
		if (validSingleTerm(&operand)) {
			*ret = ~operand;
			return true;
		}
	} else if (_scanner.validTypeCast(&isSigned, &bitWidth) ) {
		if (validSingleTerm(&operand)) {
			switch(bitWidth) {
			case 0:
				*ret = (isSigned) ? (signed) operand : (unsigned) operand;
				break;
			case 8:
				*ret = (isSigned) ? (int8_t) operand : (uint8_t) operand;
				break;
			case 16:
				*ret = (isSigned) ? (int16_t) operand : (uint16_t) operand;
				break;
			case 32:
				*ret = (isSigned) ? (int32_t) operand : (uint32_t) operand;
				break;
			case 64:
				*ret = (isSigned) ? (int64_t) operand : (uint64_t) operand;
				break;
			default:
				return false;
			}
			return true;
		}
	}

	return false;
}

/**
 * Evaluates a parenthesized expression from the current index in the
 * expression string recursively.
 *
 * @param[out] ret: the integer value the expression evaluates to
 *
 * @return: whether at the current location can be evaluated as a parenthesized
 * 			expression
 */
bool
MacroParser::parentheses(int64_t *ret)
{
	int64_t innerValue = 0;
	if (_scanner.atSymbol(LPAREN)) {
		if (logicalOr(&innerValue)) {
			if (_scanner.atSymbol(RPAREN)) {
				*ret = innerValue;
				return true;
			}
		}
	}
	return false;
}

/**
 * Checks if the expression at the cursor is one of: unary expression,
 * parenthesized expression, or a number, then evaluates it.
 *
 * @param[out] ret: the value after evaluating the expression
 *
 * return: whether at the current location can be evaluated as one of the
 * expressions listed
 */
bool
MacroParser::validSingleTerm(int64_t *ret)
{
	return unaryExpression(ret) || parentheses(ret)	|| _scanner.readNumber(ret);
}

/*
 * evaluates the numeric value of a constant expression from a macro
 *
 * @param[out] ret: the integer value of the expression in an long long
 *
 * return: DDR return code indicating success or error
 */
DDR_RC
Macro::getNumeric(long long *ret) const
{
	DDR_RC rc = DDR_RC_ERROR;
	int64_t retVal = 0;
	MacroParser parser(_value.c_str());
	if ((NULL != ret) && parser.evaluate(&retVal)) {
		*ret = retVal;
		rc = DDR_RC_OK;
	}
	return rc;
}
