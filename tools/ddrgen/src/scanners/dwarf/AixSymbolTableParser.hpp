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

#include <sys/types.h>
#include <sys/errno.h>
#include "config.hpp"
#include "DwarfFunctions.hpp"
#include <set>

typedef enum declFormat {
	NO_FORMAT, /* Default initial value */
	OLD_FORMAT, /* C format */
	NEW_FORMAT /* C++ format */
} declFormat;
typedef unordered_map<int, pair<Dwarf_Off, Dwarf_Die> > die_map;

/* PTR_SIZE varies for 32bit, 64bit */
const int PTR_SIZE = 8;

/* Number of built in types */
const int NUM_BUILT_IN_TYPES = 37;

const string BUILT_IN_TYPES[NUM_BUILT_IN_TYPES+1] = {"", "int", "char", "short int", "long int", "unsigned char",
"", "unsigned short int", "unsigned int", "", "long unsigned int", "void", 
"float", "double", "long double", "", "bool", "", "", "stringptr",
"", "", "", "", "", "", "", "",
"", "", "", "long long int", "long long unsigned int", "", "",
"intptr_t","uintptr_t", "sizetype"};
const string BUILT_IN_TYPES_THAT_ARE_TYPEDEFS[NUM_BUILT_IN_TYPES+1] = {"", "", "", "", "", "", "signed char",
"", "", "unsigned", "", "", "", "", "", "integer", "", "short real", "real", "", "character", "", "", "", "",
"", "", "integer*1", "integer*2", "integer*4", "wchar", "", "", "logical*8", "integer*8", "", "", ""
};
const int BUILT_IN_TYPE_SIZES[NUM_BUILT_IN_TYPES+1] = {0, 32, 8, 16, 32, 8, 8, 16, 32, 32,
32, 0, 32, 64, 128, 32, 32, 32, 64, PTR_SIZE, 8, 8, 16, 32, 32, 64, 128,
8, 16, 32, 16, 64, 64, 64, 64, 64, 64, 64};

const string START_OF_FILE[3] = {"debug", "3", "FILE"};
const string FILE_NAME = "a0";
const string DECL_FILE[3] = {"debug", "0", "decl"};

int extractFileID(const string data);

int extractTypeID(const string data, const int index = 1);

bool isCompilerGenerated (const string data);

void split(const string data, char delimeters, str_vect *elements);

str_vect split(const string data, char delimeters);

string strip(const string str, const char chr);

string stripLeading(const string str, const char chr);

string stripTrailing(const string str, const char chr);

double shiftDecimal(const double value);

double toDouble(const string value);

int toInt(const string value);

size_t toSize(const string size);

string toString(const int value);
