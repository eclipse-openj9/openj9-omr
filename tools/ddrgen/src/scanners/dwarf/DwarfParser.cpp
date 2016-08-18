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

#include <tuple>


#include <iostream>
#include <stdexcept>
#include <stdio.h>
#include <string.h>
#include <sstream>
#include <sys/fcntl.h>
#include <unordered_map>
#include <vector>

#include "DwarfParser.hpp"

using std::get;
using std::make_pair;
using std::make_tuple;
using std::pair;
using std::runtime_error;
using std::string;
using std::stringstream;
using std::tuple;
using std::unordered_map;
using std::vector;

/* Statics to create: */
static unordered_map<Dwarf_Off, Dwarf_Die> refMap;
static vector<string> _fileList;
static Dwarf_CU_Context *_firstCU = NULL;
static Dwarf_CU_Context *_currentCU = NULL;

static void deleteDie(Dwarf_Die die);
static int parseDwarfInfo(char *line, Dwarf_Die *lastCreatedDie, Dwarf_Die *currentDie,
	size_t *lastIndent, unordered_map<Dwarf_Die *, Dwarf_Off> *refToPopulate, Dwarf_Error *error);
static int parseCompileUnit(char *line, size_t *lastIndent, Dwarf_Error *error);
static int parseDwarfDie(char *line, Dwarf_Die *lastCreatedDie,
	Dwarf_Die *currentDie, size_t *lastIndent, size_t spaces, Dwarf_Error *error);
static void parseTagString(char *string, size_t length, Dwarf_Half *tag);
static int parseAttribute(char *line, Dwarf_Die *lastCreatedDie,
	unordered_map<Dwarf_Die *, Dwarf_Off> *refToPopulate, Dwarf_Error *error);
static void parseAttrType(char *string, size_t length, Dwarf_Half *type, Dwarf_Half *form);
static void setError(Dwarf_Error *error, Dwarf_Half num);

static const char *_errmsgs[] = {
	"No error (0)",
	"DW_DLE_ATTR_FORM_BAD (1)",
	"DW_DLE_BADOFF (2) Invalid offset",
	"DW_DLE_DIE_NULL (3)",
	"DW_DLE_FNO (4) file not open",
	"DW_DLE_IA (5) invalid argument",
	"DW_DLE_IOF (6) I/O failure",
	"DW_DLE_MAF (7) memory allocation failure",
	"DW_DLE_NOB (8) not an object file or dSYM bundle not found",
	"DW_DLE_VMM (9) dwarf format/library version mismatch",
};

int
dwarf_srcfiles(Dwarf_Die die, char ***srcfiles, Dwarf_Signed *filecount, Dwarf_Error *error)
{
	int ret = DW_DLV_OK;
	if (NULL == die) {
		ret = DW_DLV_ERROR;
		setError(error, DW_DLE_DIE_NULL);
	} else if ((NULL == filecount) || (NULL == srcfiles)) {
		ret = DW_DLV_ERROR;
		setError(error, DW_DLE_IA);
	} else {
		/* Copy the list of file names found during dwarf_init.
		 * This implementation maintains the entire list through
		 * all CU's, rather than just the current CU, for simplicity.
		 */
		*filecount = _fileList.size();
		*srcfiles = new char *[*filecount];
		if (NULL == *srcfiles) {
			ret = DW_DLV_ERROR;
			setError(error, DW_DLE_MAF);
		} else {
			size_t index = 0;
			for (vector<string>::iterator it = _fileList.begin(); it != _fileList.end(); it ++) {
				(*srcfiles)[index] = strdup(it->c_str());
				if (NULL == (*srcfiles)[index]) {
					ret = DW_DLV_ERROR;
					setError(error, DW_DLE_MAF);
					break;
				}
				index += 1;
			}
		}
	}
	return ret;
}

int
dwarf_attr(
	Dwarf_Die die,
	Dwarf_Half attr,
	Dwarf_Attribute *return_attr,
	Dwarf_Error *error)
{
	int ret = DW_DLV_NO_ENTRY;
	if (NULL == die) {
		setError(error, DW_DLE_DIE_NULL);
		ret = DW_DLV_ERROR;
	} else if (NULL == return_attr) {
		setError(error, DW_DLE_DIE_NULL);
		ret = DW_DLE_IA;
	} else {
		/* Iterate the Die's attributes to look for one with
		 * a type matching 'attr' and return it.
		 */
		*return_attr = NULL;
		Dwarf_Attribute attrCurrent = die->_attribute;
		while (NULL != attrCurrent) {
			if (attr == attrCurrent->_type) {
				*return_attr = attrCurrent;
				ret = DW_DLV_OK;
				break;
			}
			attrCurrent = attrCurrent->_nextAttr;
		}
	}
	return ret;
}

char *
dwarf_errmsg(Dwarf_Error error)
{
	return (char *)_errmsgs[error->_errno];
}

int
dwarf_formstring(Dwarf_Attribute attr, char **returned_string, Dwarf_Error *error)
{
	int ret = DW_DLV_OK;
	if ((NULL == attr) || (NULL == returned_string)) {
		ret = DW_DLV_ERROR;
		setError(error, DW_DLE_IA);
	} else if (DW_FORM_string != attr->_form) {
		ret = DW_DLV_ERROR;
		setError(error, DW_DLE_ATTR_FORM_BAD);
	} else {
		/* Return attribute data. */
		*returned_string = attr->_stringdata;
	}
	return ret;
}

void
dwarf_dealloc(Dwarf_Debug dbg, void *space, Dwarf_Unsigned type)
{
	/* Since only the dwarf_init() function allocates anything,
	 * all memory is freed in dwarf_finish(). Freeing any data
	 * in part could lead to not being able to free other data
	 * late and would prevent using the Die's that rely on the
	 * freed data. Just as in the dwarf library, dwarf_dealloc()
	 * is a no-op for Dies and Attributes.
	 */
	 if (DW_DLA_STRING == type) {
		 free((char *)space);
	 } else if (DW_DLA_LIST == type) {
		 delete [] ((char **)space);
	 } else if (DW_DLA_ERROR == type) {
		 delete((Dwarf_Error)space);
	 }
}

int
dwarf_hasattr(Dwarf_Die die, Dwarf_Half attr, Dwarf_Bool *returned_bool, Dwarf_Error *error)
{
	int ret = DW_DLV_OK;
	if (NULL == die) {
		ret = DW_DLV_ERROR;
		setError(error, DW_DLE_DIE_NULL);
	} if (NULL == returned_bool) {
		ret = DW_DLV_ERROR;
		setError(error, DW_DLE_IA);
	} else {
		/* Iterate the Die's attributes to check if it has an
		 * attribute of a given type.
		 */
		*returned_bool = false;
		Dwarf_Attribute attrCurrent = die->_attribute;
		while (NULL != attrCurrent) {
			if (attr == attrCurrent->_type) {
				*returned_bool = true;
				break;
			}
			attrCurrent = attrCurrent->_nextAttr;
		}
	}
	return ret;
}

int
dwarf_formudata(Dwarf_Attribute attr, Dwarf_Unsigned  *returned_val, Dwarf_Error *error)
{
	int ret = DW_DLV_OK;
	if ((NULL == attr) || (NULL == returned_val)) {
		ret = DW_DLV_ERROR;
		setError(error, DW_DLE_IA);
	} else if (DW_FORM_udata != attr->_form) {
		ret = DW_DLV_ERROR;
		setError(error, DW_DLE_ATTR_FORM_BAD);
	} else {
		/* Return attribute data. */
		*returned_val = attr->_udata;
	}
	return ret;
}

int
dwarf_diename(Dwarf_Die die, char **diename, Dwarf_Error *error)
{
	int ret = DW_DLV_OK;
	if (NULL == die) {
		ret = DW_DLV_ERROR;
		setError(error, DW_DLE_DIE_NULL);
	} else if (NULL == diename) {
		ret = DW_DLV_ERROR;
		setError(error, DW_DLE_IA);
	} else {
		/* Get the name attribute. */
		Dwarf_Attribute attr = NULL;
		ret = dwarf_attr(die, DW_AT_name, &attr, error);
		char *name = NULL;
		if (DW_DLV_OK == ret) {
			ret = dwarf_formstring(attr, &name, error);
		}
		/* If the Die has a name attribute, copy the name string. */
		if (DW_DLV_OK == ret) {
			*diename = strdup(name);
			if (NULL == *diename) {
				ret = DW_DLV_ERROR;
				setError(error, DW_DLE_MAF);
			}
		} else if (DW_DLV_NO_ENTRY == ret) {
			*diename = strdup("");
			if (NULL == *diename) {
				ret = DW_DLV_ERROR;
				setError(error, DW_DLE_MAF);
			}
		}
	}
	return ret;
}

int
dwarf_global_formref(Dwarf_Attribute attr, Dwarf_Off *return_offset, Dwarf_Error *error)
{
	int ret = DW_DLV_OK;
	if ((NULL == attr) || (NULL == return_offset)) {
		ret = DW_DLV_ERROR;
		setError(error, DW_DLE_IA);
	} else if ((DW_FORM_ref1 > attr->_form) || (DW_FORM_ref8 < attr->_form)) {
		ret = DW_DLV_ERROR;
		setError(error, DW_DLE_ATTR_FORM_BAD);
	} else {
		/* Return attribute data. */
		*return_offset = attr->_refdata;
	}
	return ret;
}

int
dwarf_offdie_b(Dwarf_Debug dbg,
	Dwarf_Off offset,
	Dwarf_Bool is_info,
	Dwarf_Die *return_die,
	Dwarf_Error *error)
{
	int ret = DW_DLV_OK;
	if (NULL == return_die) {
		ret = DW_DLV_ERROR;
		setError(error, DW_DLE_IA);
	} else if (refMap.end() != refMap.find(offset)) {
		/* Return any Die from its global offset. */
		*return_die = refMap[offset];
	} else {
		ret = DW_DLV_NO_ENTRY;
		setError(error, DW_DLE_BADOFF);
	}
	return ret;
}

int
dwarf_child(Dwarf_Die die, Dwarf_Die *return_childdie, Dwarf_Error *error)
{
	int ret = DW_DLV_OK;
	if (NULL == die) {
		ret = DW_DLV_ERROR;
		setError(error, DW_DLE_DIE_NULL);
	} else if (NULL == return_childdie) {
		ret = DW_DLE_IA;
		setError(error, DW_DLE_DIE_NULL);
	} else if (NULL == die->_child) {
		ret = DW_DLV_NO_ENTRY;
		*return_childdie = NULL;
	} else {
		/* Return the Die's first child. */
		*return_childdie = die->_child;
	}
	return ret;
}

int
dwarf_whatform(Dwarf_Attribute attr, Dwarf_Half *returned_final_form, Dwarf_Error *error)
{
	int ret = DW_DLV_OK;
	if ((NULL == attr) || (NULL == returned_final_form)) {
		ret = DW_DLV_ERROR;
		setError(error, DW_DLE_IA);
	} else {
		/* Get the data form of an attribute. */
		*returned_final_form = attr->_form;
	}
	return ret;
}

int
dwarf_tag(Dwarf_Die die, Dwarf_Half *return_tag, Dwarf_Error *error)
{
	int ret = DW_DLV_OK;
	if (NULL == die) {
		ret = DW_DLV_ERROR;
		setError(error, DW_DLE_DIE_NULL);
	} else if (NULL == return_tag) {
		ret = DW_DLV_ERROR;
		setError(error, DW_DLE_IA);
	} else {
		/* Get a Die's tag, indicating what type of Die it is. */
		*return_tag = die->_tag;
	}
	return ret;
}

int
dwarf_formsdata(Dwarf_Attribute attr, Dwarf_Signed *returned_val, Dwarf_Error *error)
{
	int ret = DW_DLV_OK;
	if ((NULL == attr) || (NULL == returned_val)) {
		ret = DW_DLV_ERROR;
		setError(error, DW_DLE_IA);
	} else if (DW_FORM_sdata != attr->_form) {
		ret = DW_DLV_ERROR;
		setError(error, DW_DLE_ATTR_FORM_BAD);
	} else {
		/* Return attribute data. */
		*returned_val = attr->_sdata;
	}
	return ret;
}

int
dwarf_next_cu_header(Dwarf_Debug dbg,
	Dwarf_Unsigned *cu_header_length,
	Dwarf_Half *version_stamp,
	Dwarf_Off *abbrev_offset,
	Dwarf_Half *address_size,
	Dwarf_Unsigned *next_cu_header_offset,
	Dwarf_Error *error)
{
	/* Advance the current CU and return its properties. */
	int ret = DW_DLV_OK;
	if (NULL == _currentCU) {
		_currentCU = _firstCU;
	} else if (NULL == _currentCU->_nextCU) {
		ret = DW_DLV_NO_ENTRY;
	} else {
		_currentCU = _currentCU->_nextCU;
	}
	if (DW_DLV_OK == ret) {
		*cu_header_length = _currentCU->_CUheaderLength;
		*version_stamp = _currentCU->_versionStamp;
		*abbrev_offset = _currentCU->_abbrevOffset;
		*address_size = _currentCU->_addressSize;
		*next_cu_header_offset = _currentCU->_nextCUheaderOffset;
	}
	return ret;
}

int
dwarf_siblingof(Dwarf_Debug dbg, Dwarf_Die die, Dwarf_Die *dieOut, Dwarf_Error *error)
{
	int ret = DW_DLV_OK;
	if (NULL == dieOut) {
		ret = DW_DLV_ERROR;
		setError(error, DW_DLE_IA);
	} else if (NULL == die) {
		*dieOut = _currentCU->_die;
	} else if (NULL == die->_sibling) {
		*dieOut = NULL;
		ret = DW_DLV_NO_ENTRY;
	} else {
		/* Get the next sibling of a Die. */
		*dieOut = die->_sibling;
	}
	return ret;
}

int
dwarf_finish(Dwarf_Debug dbg, Dwarf_Error *error)
{
	/* Free all Dies and Attributes created in dwarf_init(). */
	_currentCU = _firstCU;
	while (NULL != _currentCU) {
		Dwarf_CU_Context *nextCU = _currentCU->_nextCU;
		if (NULL != _currentCU->_die) {
			deleteDie(_currentCU->_die);
		}
		delete(_currentCU);
		_currentCU = nextCU;
	}
	_fileList.clear();
	_currentCU = NULL;
	_firstCU = NULL;
	return DW_DLV_OK;
}

int
dwarf_init(int fd,
	Dwarf_Unsigned access,
	Dwarf_Handler errhand,
	Dwarf_Ptr errarg,
	Dwarf_Debug *dbg,
	Dwarf_Error *error)
{
	int ret = DW_DLV_OK;

	/* Get the original file path of the opened file. */
	char filepath[PATH_MAX] = {0};
	if (0 > fcntl(fd, F_GETPATH, filepath)) {
		ret = DW_DLV_ERROR;
		setError(error, DW_DLE_FNO);
	}

	/* Call the dwarfdump command on the file's dSYM bundle and read its output.
	 * The bundle must first be created by using dsymutil on an object or executable.
	 */
	FILE *fp = NULL;
	if (DW_DLV_OK == ret) {
		stringstream ss;
		ss << "dwarfdump " << filepath << ".dSYM" << " 2>&1";
		fp = popen(ss.str().c_str(), "r");
		if (NULL == fp) {
			ret = DW_DLV_ERROR;
			setError(error, DW_DLE_IOF);
		}
	}

	/* Parse the output from dwarfdump to create the Die tree. */
	unordered_map<Dwarf_Die *, Dwarf_Off> refToPopulate;
	if (DW_DLV_OK == ret) {
		size_t len = 0;
		char *buffer = NULL;
		Dwarf_Die lastCreatedDie = NULL;
		Dwarf_Die currentDie = NULL;
		size_t lastIndent = 0;
		while ((-1 != getline(&buffer, &len, fp)) && (DW_DLV_OK == ret))
		{
			if (0 == strncmp(buffer, "error", 5)) {
				ret = DW_DLV_ERROR;
				setError(error, DW_DLE_NOB);
			} else {
				ret = parseDwarfInfo(buffer, &lastCreatedDie, &currentDie, &lastIndent, &refToPopulate, error);
			}
		}
		free(buffer);

	}
	if (DW_DLV_OK == ret) {
		for (unordered_map<Dwarf_Die *, Dwarf_Off>::iterator it = refToPopulate.begin(); it != refToPopulate.end(); it ++) {
			(*it->first) = refMap[it->second];
		}
		_currentCU = NULL;
	}

	if (NULL != fp) {
		pclose(fp);
	}

	return ret;
}

static void
deleteDie(Dwarf_Die die)
{
	/* Free the Die's attributes. */
	Dwarf_Attribute attr = die->_attribute;
	while (NULL != attr) {
		Dwarf_Attribute next = attr->_nextAttr;
		if (NULL != attr->_stringdata) {
			free(attr->_stringdata);
		}
		delete(attr);
		attr = next;
	}

	/* Free the Die's siblings and children. */
	if (NULL != die->_sibling) {
		deleteDie(die->_sibling);
	}
	if (NULL != die->_child) {
		deleteDie(die->_child);
	}
	delete(die);
}

static int
parseDwarfInfo(char *line, Dwarf_Die *lastCreatedDie, Dwarf_Die *currentDie,
	size_t *lastIndent, unordered_map<Dwarf_Die *, Dwarf_Off> *refToPopulate,
	Dwarf_Error *error)
{
	int ret = DW_DLV_OK;
	/* There are two possible formats for a line. Every second line
	 * is blank. Non-blank lines either begin with
	 * "0x[address]: [whitespace...] TAG_*" to represent a new Die org/legal/epl-v10
	 * "[whitespae...] AT_*( {[optional address]} ( [value] ) ) for a die attribute.
	 * The amount of whitespace varies to indicate parent-child relationships
	 * between Die's.
	 */
	if (0 == strncmp("0x", line, 2)) {
		/* Find the end of the address and number of spaces. Amount of indentation
		 * represents parent-child relationship between Dwarf_Die's.
		 */
		char *endOfAddress = NULL;
		Dwarf_Off address = strtoul(line, &endOfAddress, 16);
		size_t spaces = 0;
		if (NULL != endOfAddress) {
			endOfAddress += 1;
			spaces = strspn(endOfAddress, " ");
		}
		/* The string "TAG_[tag]" or "Compile Unit:" or "NULL" should be next. */
		if (0 == strncmp(endOfAddress + spaces, "Compile Unit: ", 14)) {
			ret = parseCompileUnit(endOfAddress + spaces + 14, lastIndent, error);
		} else if (0 == strncmp(endOfAddress + spaces, "TAG_", 4)) {
			ret = parseDwarfDie(endOfAddress + spaces + 4, lastCreatedDie, currentDie, lastIndent, spaces, error);
			if (DW_DLV_OK == ret) {
				refMap[address] = *lastCreatedDie;
			}
		} else if (0 == strncmp(endOfAddress + spaces, "NULL", 4)) {
			/* A "NULL" line indicates going up one level in the Die tree.
			 * Process it only if the current level contained a Die that was
			 * processed.
			 */
			if (spaces <= *lastIndent) {
				*lastCreatedDie = (*lastCreatedDie)->_parent;
			}
		} else {
			ret = DW_DLV_ERROR;
			setError(error, DW_DLE_VMM);
		}
	} else if ((0 == strncmp("  ", line, 2)) && (NULL != *currentDie)) {
		/* Die attribute lines begin with "[whitespace...] AT_". To belong
		 * to the last Die to be created, it must contain 12 spaces more
		 * than the last indentation (For the size of the Die offset text).
		 */
		size_t spaces = strspn(line, " ");
		if ((spaces == *lastIndent + 12) && (0 == strncmp(line + spaces, "AT_", 3))) {
			ret = parseAttribute(line + spaces + 3, lastCreatedDie, refToPopulate, error);
		}
	}
	return ret;
}

static int
parseCompileUnit(char *line, size_t *lastIndent, Dwarf_Error *error)
{
	/* Compile unit lines are of the format:
	 * 0x00000000: Compile Unit: length = 0x[offset]  version = 0x[version]  abbr_offset = 0x[offset]  addr_size = 0x[size]  (next CU at 0x[offset])
	 */
	int ret = DW_DLV_OK;
	Dwarf_Unsigned CUheaderLength = 0;
	Dwarf_Half versionStamp = 0;
	Dwarf_Off abbrevOffset = 0;
	Dwarf_Half addressSize = 0;
	Dwarf_Unsigned nextCUheaderOffset = 0;

	char *valueString = strstr(line, "length = ");
	if (NULL == valueString) {
		ret = DW_DLV_ERROR;
		setError(error, DW_DLE_VMM);
	} else {
		CUheaderLength = (Dwarf_Unsigned)strtoul(valueString + 9, &valueString, 16);
	}

	if (DW_DLV_OK == ret) {
		valueString = strstr(valueString, "version = ");
		if (NULL == valueString) {
			ret = DW_DLV_ERROR;
			setError(error, DW_DLE_VMM);
		} else {
			versionStamp = (Dwarf_Half)strtoul(valueString + 10, &valueString, 16);
		}
	}

	if (DW_DLV_OK == ret) {
		valueString = strstr(valueString, "abbr_offset = ");
		if (NULL == valueString) {
			ret = DW_DLV_ERROR;
			setError(error, DW_DLE_VMM);
		} else {
			abbrevOffset = (Dwarf_Off)strtoul(valueString + 14, &valueString, 16);
		}
	}

	if (DW_DLV_OK == ret) {
		valueString = strstr(valueString, "addr_size = ");
		if (NULL == valueString) {
			ret = DW_DLV_ERROR;
			setError(error, DW_DLE_VMM);
		} else {
			addressSize = (Dwarf_Half)strtoul(valueString + 12, &valueString, 16);
		}
	}

	if (DW_DLV_OK == ret) {
		valueString = strstr(valueString, "(next CU at ");
		if (NULL == valueString) {
			ret = DW_DLV_ERROR;
			setError(error, DW_DLE_VMM);
		} else {
			nextCUheaderOffset = (Dwarf_Unsigned)strtoul(valueString + 12, &valueString, 16);
		}
	}

	if (DW_DLV_OK == ret) {
		Dwarf_CU_Context *newCU = new Dwarf_CU_Context();
		if (NULL == newCU) {
			ret = DW_DLV_ERROR;
			setError(error, DW_DLE_MAF);
		} else {
			newCU->_CUheaderLength = CUheaderLength;
			newCU->_versionStamp = versionStamp;
			newCU->_abbrevOffset = abbrevOffset;
			newCU->_addressSize = addressSize;
			newCU->_nextCUheaderOffset = nextCUheaderOffset;
			newCU->_nextCU = NULL;
			newCU->_die = NULL;
			if (NULL == _firstCU) {
				_firstCU = newCU;
				_currentCU = _firstCU;
			} else {
				_currentCU->_nextCU = newCU;
				_currentCU = newCU;
			}
			*lastIndent = 0;
		}
	}
	return ret;
}

static int
parseDwarfDie(char *line, Dwarf_Die *lastCreatedDie,
	Dwarf_Die *currentDie, size_t *lastIndent, size_t spaces, Dwarf_Error *error)
{
	int ret = DW_DLV_OK;
	Dwarf_Half tag = DW_TAG_unknown;
	size_t span = strcspn(line, " ");
	parseTagString(line, span, &tag);

	if (DW_TAG_unknown == tag) {
		*currentDie = NULL;
	} else {
		Dwarf_Die newDie = new Dwarf_Die_s();
		if (NULL == newDie) {
			ret = DW_DLV_ERROR;
			setError(error, DW_DLE_MAF);
		} else {
			newDie->_tag = tag;
			newDie->_attribute = NULL;
			newDie->_context = _currentCU;
			newDie->_child = NULL;
			newDie->_sibling = NULL;
			if (0 == *lastIndent) {
				/* No last indent indicates that this Die is at the start of a CU. */
				newDie->_parent = NULL;
				_currentCU->_die = newDie;
			} else if (spaces > *lastIndent) {
				/* If the Die is indended farther than the last Die, it is
				 * a first child.
				 */
				newDie->_parent = *lastCreatedDie;
				(*lastCreatedDie)->_child = newDie;
			} else if (spaces <= *lastIndent) {
				/* If the Die is indented equally to the last Die, it is a
				 * sibling Die. If the indentation has decreased, the last
				 * Die would have been updated to one level up the tree when
				 * a "NULL" line indicated there were no more siblings.
				 */
				 newDie->_parent = (*lastCreatedDie)->_parent;
				 (*lastCreatedDie)->_sibling = newDie;
			}
			*lastCreatedDie = newDie;
			*currentDie = newDie;
			*lastIndent = spaces;
		}
	}
	return DW_DLV_OK;
}

static pair<const char *, Dwarf_Half> tagStrings[] = {
	make_pair("array_type", DW_TAG_array_type),
	make_pair("base_type", DW_TAG_base_type),
	make_pair("class_type", DW_TAG_class_type),
	make_pair("compile_unit", DW_TAG_compile_unit),
	make_pair("const_type", DW_TAG_const_type),
	make_pair("enumeration_type", DW_TAG_enumeration_type),
	make_pair("enumerator", DW_TAG_enumerator),
	make_pair("inheritance", DW_TAG_inheritance),
	make_pair("member", DW_TAG_member),
	make_pair("namespace", DW_TAG_namespace),
	make_pair("pointer_type", DW_TAG_pointer_type),
	make_pair("restrict_type", DW_TAG_restrict_type),
	make_pair("shared_type", DW_TAG_shared_type),
	make_pair("structure_type", DW_TAG_structure_type),
	make_pair("subprogram", DW_TAG_subprogram),
	make_pair("subrange_type", DW_TAG_subrange_type),
	make_pair("subroutine_type", DW_TAG_subroutine_type),
	make_pair("typedef", DW_TAG_typedef),
	make_pair("union_type", DW_TAG_union_type),
	make_pair("volatile_type", DW_TAG_volatile_type),
};

static void
parseTagString(char *string, size_t length, Dwarf_Half *tag)
{
	size_t options = sizeof(tagStrings) / sizeof(tagStrings[0]);
	for (size_t i = 0; i < options; i += 1) {
		if (0 == strncmp(string, tagStrings[i].first, length)) {
			*tag = tagStrings[i].second;
			break;
		}
	}
}

static int
parseAttribute(char *line, Dwarf_Die *lastCreatedDie,
	unordered_map<Dwarf_Die *, Dwarf_Off> *refToPopulate, Dwarf_Error *error)
{
	int ret = DW_DLV_OK;

	/* Get the type and form of the attribute. */
	Dwarf_Half type = DW_TAG_unknown;
	Dwarf_Half form = DW_FORM_unknown;
	size_t span = strcspn(line, "(");
	parseAttrType(line, span, &type, &form);

	if ((DW_AT_unknown != type) && (DW_FORM_unknown != form)) {
		Dwarf_Attribute newAttr = new Dwarf_Attribute_s();
		if (NULL == newAttr) {
			ret = DW_DLV_ERROR;
			setError(error, DW_DLE_MAF);
		} else {
			newAttr->_nextAttr = NULL;
			newAttr->_stringdata = NULL;
			newAttr->_type = type;
			newAttr->_form = form;
			newAttr->_ref = NULL;

			/* Parse the value of the attribute based on its form. */
			if (DW_AT_decl_file == type) {
				char *valueStart = line + span + 3;
				size_t valueLength = strchr(valueStart, '"') - valueStart;
				newAttr->_stringdata = strndup(valueStart, valueLength);
				if (NULL == newAttr->_stringdata) {
					ret = DW_DLV_ERROR;
					setError(error, DW_DLE_MAF);
					newAttr->_udata = 0;
				} else {
					_fileList.push_back(string(newAttr->_stringdata));
					newAttr->_udata = _fileList.size();
				}
			} else if (DW_FORM_string == form) {
				char *valueStart = line + span + 3;
				size_t valueLength = strchr(valueStart, '"') - valueStart;
				newAttr->_stringdata = strndup(valueStart, valueLength);
				if (NULL == newAttr->_stringdata) {
					ret = DW_DLV_ERROR;
					setError(error, DW_DLE_MAF);
				}
			} else if (DW_FORM_ref1 == form) {
				newAttr->_refdata = strtoul(line + span + 3, NULL, 16);
				refToPopulate->emplace(&newAttr->_ref, newAttr->_refdata);
			} else if (DW_FORM_udata == form) {
				newAttr->_udata = strtoul(line + span + 1, NULL, 0);
			} else if (DW_FORM_sdata == form) {
				newAttr->_udata = strtol(line + span + 1, NULL, 0);
			} else {
				ret = DW_DLV_ERROR;
				setError(error, DW_DLE_VMM);
			}
			if (DW_DLV_OK == ret) {
				/* Add the attribute to the most recently created Die. */
				if (NULL == (*lastCreatedDie)->_attribute) {
					(*lastCreatedDie)->_attribute = newAttr;
				} else {
					Dwarf_Attribute attr = (*lastCreatedDie)->_attribute;
					while (NULL != attr->_nextAttr) {
						attr = attr->_nextAttr;
					}
					attr->_nextAttr = newAttr;
				}
			} else {
				delete(newAttr);
			}
		}
	}
	return ret;
}

static tuple<const char *, Dwarf_Half, Dwarf_Half> attrStrings[] = {
	make_tuple("byte_size", DW_AT_byte_size, DW_FORM_udata),
	make_tuple("bit_size", DW_AT_bit_size, DW_FORM_udata),
	make_tuple("comp_dir", DW_AT_comp_dir, DW_FORM_string),
	make_tuple("const_value", DW_AT_const_value, DW_FORM_sdata),
	make_tuple("decl_file", DW_AT_decl_file, DW_FORM_udata),
	make_tuple("decl_line", DW_AT_decl_line, DW_FORM_udata),
	make_tuple("linkage_name", DW_AT_linkage_name, DW_FORM_string),
	make_tuple("name", DW_AT_name, DW_FORM_string),
	make_tuple("specification", DW_AT_specification, DW_FORM_ref1),
	make_tuple("type", DW_AT_type, DW_FORM_ref1),
	make_tuple("upper_bound", DW_AT_upper_bound, DW_FORM_udata),
};

static void
parseAttrType(char *string, size_t length, Dwarf_Half *type, Dwarf_Half *form)
{
	size_t options = sizeof(attrStrings) / sizeof(attrStrings[0]);
	for (size_t i = 0; i < options; i += 1) {
		if (0 == strncmp(string, get<0>(attrStrings[i]), length)) {
			*type = get<1>(attrStrings[i]);
			*form = get<2>(attrStrings[i]);
			break;
		}
	}
}

static void
setError(Dwarf_Error *error, Dwarf_Half num)
{
	if (NULL != error) {
		*error = new Dwarf_Error_s();
		if (NULL != *error) {
			(*error)->_errno = num;
		}
	}
}
