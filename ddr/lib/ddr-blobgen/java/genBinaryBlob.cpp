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
#if defined(J9ZOS390)
/* We need to define these for the limit macros to get defined in z/OS */
#define _ISOC99_SOURCE
#define __STDC_LIMIT_MACROS
#endif /* defined(J9ZOS390) */

#include "ddr/blobgen/java/genBinaryBlob.hpp"

#undef NDEBUG
#include <assert.h>
#include <fcntl.h>
#include <limits.h>
#include <sstream>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <vector>

using std::string;
using std::pair;
using std::make_pair;
using std::vector;
using std::stringstream;

void
JavaBlobGenerator::BuildBlobInfo::initBlobHeader()
{
	memset(&header, 0, sizeof(header));

	header.version = 1;
	header.sizeofBool = sizeof(bool); /* TODO what if cross-compiled? */
	header.sizeofUDATA = sizeof(UDATA); /* TODO what if cross-compiled? */
	initializeBitfieldFormat(&header.bitfieldFormat);
	header.structDataSize = 0;
	header.stringDataSize = 0;
	header.structureCount = 0;
}

DDR_RC
JavaBlobGenerator::BuildBlobInfo::initializeBitfieldFormat(uint8_t *bitfieldFormat)
{
	BitField bf;
	DDR_RC rc = DDR_RC_OK;
	uint32_t slot0;
	uint32_t slot1;

	memset(&bf, 0, sizeof(bf));

	bf.x1 = 1;			/* 1 */
	bf.x2 = 31768;		/* 111110000011000 */
	bf.x3 = 3;			/* 000011 */
	bf.x4 = 777;		/* 1100001001 */

	bf.x5 = 43;			/* 0101011 */
	bf.x6 = 298;		/* 100101010 */
	bf.x7 = 12;			/* 1100 */
	bf.x8 = 1654;		/* 011001110110 */

	slot0 = ((uint32_t *)&bf)[0];
	slot1 = ((uint32_t *)&bf)[1];
	if ((slot0 == 0xC243F831) && (slot1 == 0x676C952B)) {
		*bitfieldFormat = 1;
	} else if ((slot0 == 0xFC180F09) && (slot1 == 0x572AC676)) {
		*bitfieldFormat = 2;
	} else {
		ERRMSG("Unable to determine bitfield format from %08X %08X\n", slot0, slot1);
		rc = DDR_RC_ERROR;
	}

	return rc;
}

typedef struct StringTableEntry {
	const char *cString;
	uint32_t offset;
} StringTableEntry;

static uintptr_t
stringTableHash(void *key, void *userData)
{
	const char *s = ((StringTableEntry *)key)->cString;
	size_t length = strlen(s);
	uintptr_t hash = 0;
	uint8_t *data = (uint8_t *)s;
	const uint8_t *end = data + length;

	while (data < end) {
		uint16_t c;

		data += decodeUTF8Char(data, &c);
		hash = (hash << 5) - hash + c;
	}

	return hash;
}

static uintptr_t
stringTableEquals(void *leftKey, void *rightKey, void *userData)
{
	const char *left_s = ((StringTableEntry *)leftKey)->cString;
	const char *right_s = ((StringTableEntry *)rightKey)->cString;

	return (left_s == right_s) || (0 == strcmp(left_s, right_s));
}

void
JavaBlobGenerator::copyStringTable()
{
	uint8_t *stringData = _buildInfo.stringBuffer;
	J9HashTableState state;

	/* Iterate the table and copy each string to its already-assigned offset */

	StringTableEntry *entry = (StringTableEntry *)hashTableStartDo(_buildInfo.stringHash, &state);
	while (NULL != entry) {
		J9UTF8 *utf = (J9UTF8 *)(stringData + entry->offset);
		const char *cString = entry->cString;
		size_t len = strlen(cString);

		utf->length = (uint16_t)len;
		memcpy(utf->data, cString, len);
		free((char *)cString);

		entry = (StringTableEntry *)hashTableNextDo(&state);
	}

	/* TODO need to pad file length to 32-bits? */
#if 0
	/* Update cursor (stringDataSize is U_32 aligned already) */
	data->cursor = stringData + data->coreFileData->stringDataSize;
#endif
}

DDR_RC
JavaBlobGenerator::stringTableOffset(BlobHeader *blobHeader, J9HashTable *stringTable, const char *cString, uint32_t *offset)
{
	DDR_RC rc = DDR_RC_OK;
	StringTableEntry exemplar;
	StringTableEntry *entry;
	*offset = UINT32_MAX;

	exemplar.cString = cString;
	exemplar.offset = UINT32_MAX;

	/* Add will return an existing entry if one matches, NULL on allocation failure, or the new node */
	entry = (StringTableEntry *)hashTableAdd(stringTable, &exemplar);

	/* NULL means the string was new, but memory allocation failed */
	if (NULL == entry) {
		ERRMSG("Unable to allocate memory for string table entry: %s", cString);
		rc = DDR_RC_ERROR;
	} else {
		/* If the offset is UINT32_MAX, this indicates that a new entry was added */
		if (UINT32_MAX == entry->offset) {
			entry->offset = blobHeader->stringDataSize;

			/* For new string entries, allocate memory to hold the string */
			char *buffer = (char *)malloc(strlen(cString) + 1);
			if (NULL == buffer) {
				ERRMSG("Unable to allocate memory for string table entry: %s", cString);
				rc = DDR_RC_ERROR;
			} else {
				strcpy(buffer, cString);
				entry->cString = buffer;

				/* Reserve space for the uint16_t size, the string data, and the padding byte, if necessary */
				blobHeader->stringDataSize += (uint32_t)((sizeof(uint16_t) + strlen(cString) + 1) & ~(size_t)1);
				*offset = entry->offset;
			}
		} else {
			*offset = entry->offset;
		}
	}

	return rc;
}

DDR_RC
JavaBlobGenerator::genBinaryBlob(OMRPortLibrary *portLibrary, Symbol_IR *ir, const char *blobFile)
{
	DDR_RC rc = DDR_RC_OK;
	OMRPORT_ACCESS_FROM_OMRPORT(portLibrary);

	/* iterate ir:
	 *  - count structs - update blob header
	 *  - build string hash table
	 */
	rc = countStructsAndStrings(ir);

	if (DDR_RC_OK == rc) {
		/* compute offsets for each entry of string hash table
		 * compute size of string data - update blob header
		 */
		rc = buildBlobData(portLibrary, ir);
	}

	intptr_t fd = -1;
	if (DDR_RC_OK == rc) {
		/* open blob file */
		fd = omrfile_open(blobFile, EsOpenCreate | EsOpenWrite | EsOpenAppend | EsOpenTruncate, 0644);
		_buildInfo.fd = fd;
		if (-1 == fd) {
			ERRMSG("Failed to open file blob.dat for writing.\n");
			rc = DDR_RC_ERROR;
		}
	}

	if (DDR_RC_OK == rc) {
		/* write blob header */
		intptr_t wb = omrfile_write(fd, &_buildInfo.header, sizeof(_buildInfo.header));
		assert(wb == sizeof(_buildInfo.header));

		/* write structs */
		int f_ix = 0;
		int c_ix = 0;
		intptr_t amountWritten = 0;
		for (unsigned int s_ix = 0; s_ix < _buildInfo.header.structureCount; s_ix++) {
			wb = omrfile_write(fd, &_buildInfo.blobStructs[s_ix], sizeof(BlobStruct));
			amountWritten += wb;

			/* write fields */
			wb = omrfile_write(fd, &_buildInfo.blobFields[f_ix], sizeof(BlobField) * _buildInfo.blobStructs[s_ix].fieldCount);
			amountWritten += wb;
			f_ix += _buildInfo.blobStructs[s_ix].fieldCount;

			/* write constants */
			wb = omrfile_write(fd, &_buildInfo.blobConsts[c_ix], sizeof(BlobConstant) * _buildInfo.blobStructs[s_ix].constantCount);
			amountWritten += wb;
			c_ix += _buildInfo.blobStructs[s_ix].constantCount;
		}

		if ((unsigned long)amountWritten != _buildInfo.header.structDataSize) {
			ERRMSG("Written: %d fields, %d constants, %d structs.\nCounted: %d fields, %d constants, %d structures.\n",
				   f_ix, c_ix, (int)(_buildInfo.curBlobStruct - _buildInfo.blobStructs),
				   _buildInfo.fieldCount, _buildInfo.constCount, _buildInfo.header.structureCount);
			ERRMSG("Expected %d to be written, but %d was written.\n", (int)_buildInfo.header.structDataSize, (int)amountWritten);
		}
		/* write string data */
		wb = omrfile_write(fd, _buildInfo.stringBuffer, _buildInfo.header.stringDataSize);

		/* close blob file */
		omrfile_close(fd);

		free(_buildInfo.blobStructs);
		free(_buildInfo.blobFields);
		free(_buildInfo.blobConsts);
		free(_buildInfo.stringBuffer);
		hashTableFree(_buildInfo.stringHash);
	}

	return rc;
}

pair<string, unsigned long long> JavaBlobGenerator::cLimits[] = {
		make_pair("CHAR_MAX", CHAR_MAX),
		make_pair("CHAR_MIN", CHAR_MIN),
		make_pair("INT_MAX", INT_MAX),
		make_pair("INT_MIN", INT_MIN),
		make_pair("LONG_MAX", LONG_MAX),
		make_pair("LONG_MIN", LONG_MIN),
		make_pair("SCHAR_MAX", SCHAR_MAX),
		make_pair("SCHAR_MIN", SCHAR_MIN),
		make_pair("SHRT_MAX", SHRT_MAX),
		make_pair("SHRT_MIN", SHRT_MIN),
		make_pair("UCHAR_MAX", UCHAR_MAX),
		make_pair("UINT_MAX", UINT_MAX),
		make_pair("ULONG_MAX", ULONG_MAX),
		make_pair("USHRT_MAX", USHRT_MAX)
};

DDR_RC
JavaBlobGenerator::enumerateCLimits()
{
	DDR_RC rc = DDR_RC_OK;
	size_t fieldCount = 0;
	size_t constCount = sizeof(cLimits) / sizeof(cLimits[0]);
	size_t structDataSize = sizeof(BlobField) * fieldCount + sizeof(BlobConstant) * constCount + sizeof(BlobStruct);

	_buildInfo.header.structureCount += 1;
	_buildInfo.header.structDataSize += (uint32_t)structDataSize;
	_buildInfo.fieldCount += (uint32_t)fieldCount;
	_buildInfo.constCount += (uint32_t)constCount;
	return rc;
}

DDR_RC
JavaBlobGenerator::addCLimits()
{
	DDR_RC rc = DDR_RC_OK;
	size_t fieldCount = 0;
	size_t constCount = sizeof(cLimits) / sizeof(cLimits[0]);

	for (size_t i = 0; i < constCount; i += 1) {
		rc = addBlobConst(cLimits[i].first, cLimits[i].second, NULL);
		if (DDR_RC_OK != rc) {
			break;
		}
	}

	if (DDR_RC_OK == rc) {
		rc = addBlobStruct("CLimits", "", (uint32_t)constCount, (uint32_t)fieldCount, 0);
	}

	return rc;
}

/* iterate ir:
 *  - count structs - update blob header
 *  - build string hash table
 */
DDR_RC
JavaBlobGenerator::countStructsAndStrings(Symbol_IR *const ir)
{
	DDR_RC rc = DDR_RC_OK;
	/* just count everything */
	for (vector<Type *>::iterator v = (ir->_types).begin(); v != (ir->_types).end(); ++v) {
		rc = (*v)->enumerateType(this, false);
		if (DDR_RC_OK != rc) {
			break;
		}
	}

	/* Add CLimits to the count. */
	if (DDR_RC_OK == rc) {
		rc = enumerateCLimits();
	}
	return rc;
}

DDR_RC
JavaBlobGenerator::buildBlobData(OMRPortLibrary *portLibrary, Symbol_IR *const ir)
{
	DDR_RC rc = DDR_RC_OK;
	/* allocate hashtable */
	_buildInfo.stringHash =
		hashTableNew(portLibrary, OMR_GET_CALLSITE(), 0,
					 sizeof(StringTableEntry), 0, 0, OMRMEM_CATEGORY_UNKNOWN,
					 stringTableHash, stringTableEquals, NULL, NULL);

	/* allocate class, field, and const structures */
	_buildInfo.blobStructs = (BlobStruct *)malloc(_buildInfo.header.structureCount * sizeof(BlobStruct));
	_buildInfo.curBlobStruct = _buildInfo.blobStructs;
	_buildInfo.blobFields = (BlobField *)malloc(_buildInfo.fieldCount * sizeof(BlobField));
	_buildInfo.curBlobField = _buildInfo.blobFields;
	_buildInfo.blobConsts = (BlobConstant *)malloc(_buildInfo.constCount * sizeof(BlobConstant));
	_buildInfo.curBlobConst = _buildInfo.blobConsts;
	_buildInfo.curBlobStruct->constantCount = 0;
	_buildInfo.curBlobStruct->fieldCount = 0;

	for (vector<Type *>::iterator v = ir->_types.begin(); v != ir->_types.end(); ++v) {
		rc = (*v)->buildBlob(this, false, "");
		if (DDR_RC_OK != rc) {
			break;
		}
	}

	if (DDR_RC_OK == rc) {
		/* Add CLimits. */
		rc = addCLimits();
	}

	if (DDR_RC_OK == rc) {
		_buildInfo.stringBuffer = (uint8_t *)malloc(_buildInfo.header.stringDataSize);
		memset(_buildInfo.stringBuffer, 0, _buildInfo.header.stringDataSize);
		copyStringTable();
	}
	return rc;
}

DDR_RC
JavaBlobGenerator::dispatchBuildBlob(EnumUDT *e, bool addFieldsOnly, string prefix)
{
	DDR_RC rc = DDR_RC_OK;
	if (!e->_isDuplicate) {
		/* Do not add anonymous inner types as their own type. */
		if ((!e->isAnonymousType() || addFieldsOnly) && (e->_enumMembers.size() > 0)) {
			/* Format class name */
			string nameFormatted = e->_name;
			if (NULL != e->_outerNamespace) {
				nameFormatted = e->_outerNamespace->_name + "__" + nameFormatted;
			}

			uint32_t constCount = 0;
			for (vector<EnumMember *>::iterator m = (e->_enumMembers).begin(); m != (e->_enumMembers).end(); ++m) {
				rc = addBlobConst((*m)->_name, (*m)->_value, &constCount);
				if (DDR_RC_OK != rc) {
					break;
				}
			}

			if (DDR_RC_OK == rc) {
				if (addFieldsOnly) {
					_buildInfo.curBlobStruct->constantCount += constCount;
				} else {
					rc = addBlobStruct(nameFormatted, "", constCount, 0, 0);
				}
			}
		}
	}
	return rc;
}

DDR_RC
JavaBlobGenerator::dispatchBuildBlob(UnionUDT *u, bool addFieldsOnly, string prefix)
{
	DDR_RC rc = DDR_RC_OK;
	if (!u->_isDuplicate) {
		/* Do not add anonymous inner types as their own type. */
		if ((!u->isAnonymousType() || addFieldsOnly) && (u->_fieldMembers.size() > 0)) {
			/* Format class name */
			string nameFormatted = u->_name;
			if (NULL != u->_outerNamespace) {
				nameFormatted = u->_outerNamespace->_name + "__" + nameFormatted;
			}

			uint32_t fieldCount = 0;
			uint32_t constCount = 0;
			for (vector<Field *>::iterator m = (u->_fieldMembers).begin(); m != (u->_fieldMembers).end(); ++m) {
				rc = addBlobField(*m, &fieldCount, &constCount, prefix);
				if (DDR_RC_OK != rc) {
					break;
				}
			}

			if (DDR_RC_OK == rc) {
				for (vector<EnumMember *>::iterator m = (u->_enumMembers).begin(); m != (u->_enumMembers).end(); ++m) {
					rc = addBlobConst((*m)->_name, (*m)->_value, &constCount);
					if (DDR_RC_OK != rc) {
						break;
					}
				}
			}

			if (DDR_RC_OK == rc) {
				for (vector<Macro>::iterator m = (u->_macros).begin(); m != (u->_macros).end(); ++m) {
					long long value = 0;
					if (DDR_RC_OK == m->getNumeric(&value)) {
						rc = addBlobConst(m->_name, value, &constCount);
						if (DDR_RC_OK != rc) {
							break;
						}
					}
				}
			}

			if (DDR_RC_OK == rc) {
				if (addFieldsOnly) {
					_buildInfo.curBlobStruct->constantCount += constCount;
					_buildInfo.curBlobStruct->fieldCount += fieldCount;
				} else {
					rc = addBlobStruct(nameFormatted, "", constCount, fieldCount, (uint32_t)u->_sizeOf);
				}
			}
		}
	}
	if ((DDR_RC_OK == rc) && !addFieldsOnly) {
		for (vector<UDT *>::iterator v = u->_subUDTs.begin(); v != u->_subUDTs.end(); ++v) {
			rc = (*v)->buildBlob(this, false, "");
			if (DDR_RC_OK != rc) {
				break;
			}
		}
	}
	return rc;
}

DDR_RC
JavaBlobGenerator::dispatchBuildBlob(ClassUDT *cu, bool addFieldsOnly, string prefix)
{
	DDR_RC rc = DDR_RC_OK;
	if (!cu->_isDuplicate) {
	/* Do not add anonymous inner types as their own type. */
		if ((!cu->isAnonymousType() || addFieldsOnly) && (cu->_fieldMembers.size() > 0)){
			/* Format class name */
			string nameFormatted = cu->_name;
			if (NULL != cu->_outerNamespace) {
				nameFormatted = cu->_outerNamespace->_name + "__" + nameFormatted;
			}

			/* Find super class name offset */
			string superName;
			if (NULL != cu->_superClass) {
				superName = cu->_superClass->_name;
				if (NULL != cu->_superClass->_outerNamespace) {
					superName = cu->_superClass->_outerNamespace->_name + "__" + superName;
				}
			}

			uint32_t fieldCount = 0;
			uint32_t constCount = 0;
			for (vector<Field *>::iterator m = (cu->_fieldMembers).begin(); m != (cu->_fieldMembers).end(); ++m) {
				rc = addBlobField(*m, &fieldCount, &constCount, prefix);
				if (DDR_RC_OK != rc) {
					break;
				}
			}

			if (DDR_RC_OK == rc) {
				for (vector<EnumMember *>::iterator m = (cu->_enumMembers).begin(); m != (cu->_enumMembers).end(); ++m) {
					rc = addBlobConst((*m)->_name, (*m)->_value, &constCount);
					if (DDR_RC_OK != rc) {
						break;
					}
				}
			}

			if (DDR_RC_OK == rc) {
				for (vector<Macro>::iterator m = (cu->_macros).begin(); m != (cu->_macros).end(); ++m) {
					long long value = 0;
					if (DDR_RC_OK == m->getNumeric(&value)) {
						rc = addBlobConst(m->_name, value, &constCount);
						if (DDR_RC_OK != rc) {
							break;
						}
					}
				}
			}

			if (DDR_RC_OK == rc) {
				if (addFieldsOnly) {
					_buildInfo.curBlobStruct->constantCount += constCount;
					_buildInfo.curBlobStruct->fieldCount += fieldCount;
				} else {
					rc = addBlobStruct(nameFormatted, superName, constCount, fieldCount, (uint32_t)cu->_sizeOf);
				}
			}
		}
	}
	if ((DDR_RC_OK == rc) && !addFieldsOnly) {
		for (vector<UDT *>::iterator v = cu->_subUDTs.begin(); v != cu->_subUDTs.end(); ++v) {
			rc = (*v)->buildBlob(this, false, "");
			if (DDR_RC_OK != rc) {
				break;
			}
		}
	}
	return rc;
}

DDR_RC
JavaBlobGenerator::dispatchBuildBlob(NamespaceUDT *ns, bool addFieldsOnly, string prefix)
{
	DDR_RC rc = DDR_RC_OK;
	if (!ns->_isDuplicate) {
		uint32_t constCount = 0;
		for (vector<Macro>::iterator m = (ns->_macros).begin(); m != (ns->_macros).end(); ++m) {
			long long value = 0;
			if (DDR_RC_OK == m->getNumeric(&value)) {
				rc = addBlobConst(m->_name, value, &constCount);
				if (DDR_RC_OK != rc) {
					break;
				}
			}
		}

		if (DDR_RC_OK == rc) {
			if (addFieldsOnly) {
				_buildInfo.curBlobStruct->constantCount += constCount;
			} else {
				rc = addBlobStruct(ns->_name, "", constCount, 0, 0);
			}
		}

		if ((DDR_RC_OK == rc) && !addFieldsOnly) {
			for (vector<UDT *>::iterator v = ns->_subUDTs.begin(); v != ns->_subUDTs.end(); ++v) {
				rc = (*v)->buildBlob(this, false, "");
				if (DDR_RC_OK != rc) {
					break;
				}
			}
		}
	}
	return rc;
}

DDR_RC
JavaBlobGenerator::addBlobField(Field *f, uint32_t *fieldCount, uint32_t *constCount)
{
	return addBlobField(f, fieldCount, constCount, "");
}

DDR_RC
JavaBlobGenerator::addBlobField(Field *f, uint32_t *fieldCount, uint32_t *constCount, string prefix)
{
	DDR_RC rc = DDR_RC_OK;

	Type *type = f->_fieldType;
	if (NULL == type) {
		ERRMSG("Field %s has NULL type", f->_name.c_str());
		rc = DDR_RC_ERROR;
	} else if (!f->_isStatic) {
		/* Anonymous classes, structs, and unions add their fields directly to the outer type. */
		if (type->isAnonymousType()) {
			type->buildBlob(this, true, prefix + f->_name + ".");
		} else {
			*fieldCount += 1;
			string typeName;
			rc = formatFieldType(f, &typeName);

			if (DDR_RC_OK == rc) {
				string fieldName = prefix + f->_name;
				if (DDR_RC_OK == rc) {
					uint32_t offset = UINT32_MAX;
					rc = stringTableOffset(&_buildInfo.header, _buildInfo.stringHash, fieldName.c_str(), &offset);

					if (DDR_RC_OK == rc) {
						_buildInfo.curBlobField->nameOffset = offset;

						offset = UINT32_MAX;
						rc = stringTableOffset(&_buildInfo.header, _buildInfo.stringHash, typeName.c_str(), &offset);

						if (DDR_RC_OK == rc) {
							_buildInfo.curBlobField->typeOffset = offset;

							_buildInfo.curBlobField->offset = (uint32_t)f->_offset;
							_buildInfo.curBlobField += 1;
						}
					}
				}
			}
		}
	}

	return rc;
}

DDR_RC
JavaBlobGenerator::addBlobConst(string name, long long value, uint32_t *constCount)
{
	DDR_RC rc = DDR_RC_OK;
	uint32_t offset = UINT32_MAX;

	rc = stringTableOffset(&_buildInfo.header, _buildInfo.stringHash, name.c_str(), &offset);
	if (DDR_RC_OK == rc) {
		_buildInfo.curBlobConst->nameOffset = offset;

		memset(_buildInfo.curBlobConst->value, 0, sizeof(_buildInfo.curBlobConst->value));
		memcpy(_buildInfo.curBlobConst->value, &value, sizeof(value));
		_buildInfo.curBlobConst += 1;
		if (NULL != constCount) {
			*constCount += 1;
		}
	}

	return rc;
}

DDR_RC
JavaBlobGenerator::addBlobStruct(string name, string superName, uint32_t constCount, uint32_t fieldCount, uint32_t size)
{
	DDR_RC rc = DDR_RC_OK;
	uint32_t offset = UINT32_MAX;

	rc = stringTableOffset(&_buildInfo.header, _buildInfo.stringHash, name.c_str(), &offset);

	if (DDR_RC_OK == rc) {
		_buildInfo.curBlobStruct->nameOffset = offset;
		if (superName.empty()) {
			_buildInfo.curBlobStruct->superOffset = 0xffffffff;
		} else {
			offset = UINT32_MAX;
			rc = stringTableOffset(&_buildInfo.header, _buildInfo.stringHash, superName.c_str(), &offset);

			if (DDR_RC_OK == rc) {
				_buildInfo.curBlobStruct->superOffset = offset;
			}
		}

		_buildInfo.curBlobStruct->constantCount += constCount;
		_buildInfo.curBlobStruct->fieldCount += fieldCount;
		_buildInfo.curBlobStruct->structSize = size;
		if ((uintptr_t)(_buildInfo.curBlobStruct - _buildInfo.blobStructs) < (uintptr_t)(_buildInfo.header.structureCount - 1)) {
			_buildInfo.curBlobStruct += 1;
			_buildInfo.curBlobStruct->constantCount = 0;
			_buildInfo.curBlobStruct->fieldCount = 0;
		}
	}

	return rc;
}

DDR_RC
JavaBlobGenerator::formatFieldType(Field *f, string *fieldType)
{
	DDR_RC rc = DDR_RC_OK;
	*fieldType = "";

	if (0 != (f->_modifiers._modifierFlags & ~Modifiers::MODIFIER_FLAGS)) {
		ERRMSG("Unhandled field modifer flags: %d", f->_modifiers._modifierFlags);
		rc = DDR_RC_ERROR;
	} else {
		*fieldType =  f->_modifiers.getModifierNames();
	}

	SymbolKind st;
	rc = f->getBaseSymbolKind(&st);

	if (DDR_RC_OK == rc) {
		switch (st) {
		case CLASS:
			break;
		case STRUCT:
			*fieldType += "struct ";
			break;
		case UNION:
			*fieldType += "union ";
			break;
		case ENUM:
			*fieldType += "enum ";
			break;
		case BASE:
			*fieldType += "";
			break;
		case TYPEDEF:
			*fieldType += "";
			break;
		case NAMESPACE:
			*fieldType += "namespace ";
			break;
		default:
			ERRMSG("Unhandled fieldType: %d", st);
			rc = DDR_RC_ERROR;
		}
	}

	if (DDR_RC_OK == rc) {
		if (BASE == st) {
			TypedefUDT *td = dynamic_cast<TypedefUDT *>(f->_fieldType);
			if (NULL != td) {
				/* If typedef is void*, or otherwise known as a function pointer, return name as void*. */
				if ((NULL != td->_aliasedType) && ("void" == td->_aliasedType->_name) && (1 == td->_modifiers._pointerCount)) {
					*fieldType += "void*";
				} else {
					*fieldType += f->_fieldType->_name;
				}
			} else {
				*fieldType += f->_fieldType->_name;
			}
		} else {
			UDT *udt = (UDT *)f->_fieldType;
			if (NULL != udt) {
				if (NULL != udt->_outerNamespace) {
					*fieldType += udt->_outerNamespace->_name + "::";
				}
				*fieldType += udt->_name;
			} else {
				ERRMSG("NULL field type");
				rc = DDR_RC_ERROR;
			}
		}

		for (int i = 0; i < f->_modifiers._pointerCount; i++) {
			*fieldType += "*";
		}

		if (f->_modifiers.isArray()) {
			stringstream stream;
			for (unsigned int i = 0; i < f->_modifiers.getArrayDimensions(); i++) {
				stream << "[";
				if (f->_modifiers.getArrayLength(i) > 0) {
					stream << f->_modifiers.getArrayLength(i);
				}
				stream << "]";
			}
			*fieldType += stream.str();
		}
	}

	return rc;
}

DDR_RC
JavaBlobGenerator::addFieldAndConstCount(bool addFieldsOnly, size_t fieldCount, size_t constCount)
{
	DDR_RC rc = DDR_RC_OK;
	size_t structDataSize = sizeof(BlobField) * fieldCount + sizeof(BlobConstant) * constCount;
	if (!addFieldsOnly) {
		structDataSize += sizeof(BlobStruct);
	}

	if (structDataSize < ((uint32_t)-1)) {
		if (!addFieldsOnly) {
			_buildInfo.header.structureCount += 1;
		}
		_buildInfo.header.structDataSize += (uint32_t)structDataSize;
		_buildInfo.fieldCount += (uint32_t)fieldCount;
		_buildInfo.constCount += (uint32_t)constCount;
	} else {
		ERRMSG("structDataSize is too large to fit in header.structDataSize");
		rc = DDR_RC_ERROR;
	}
	return rc;
}

DDR_RC
JavaBlobGenerator::dispatchEnumerateType(Type *type, bool addFieldsOnly)
{
	/* No op: base types are not added to the blob at this time. */
	return DDR_RC_OK;
}

DDR_RC
JavaBlobGenerator::dispatchEnumerateType(NamespaceUDT *type, bool addFieldsOnly)
{
	DDR_RC rc = DDR_RC_OK;
	if (!type->_isDuplicate) {
		size_t constCount = 0;
		/* Add only integer constants to the blob. */
		for (vector<Macro>::iterator it = type->_macros.begin(); it != type->_macros.end(); it += 1) {
			if (DDR_RC_OK == it->getNumeric(NULL)) {
				constCount += 1;
			}
		}

		if ((DDR_RC_OK == rc) && (!type->isAnonymousType() || addFieldsOnly)) {
			rc = addFieldAndConstCount(addFieldsOnly, 0, constCount);
		}
	}
	/* When adding the fields from a field with an anonymous type, do not add subUDTs twice. */
	if (!addFieldsOnly) {
		for (vector<UDT *>::iterator v = (type->_subUDTs).begin(); v != (type->_subUDTs).end(); ++v) {
			rc = (*v)->enumerateType(this, false);
			if (DDR_RC_OK != rc) {
				break;
			}
		}
	}
	return rc;
}

DDR_RC
JavaBlobGenerator::dispatchEnumerateType(EnumUDT *type, bool addFieldsOnly)
{
	DDR_RC rc = DDR_RC_OK;
	if (!type->_isDuplicate) {
		if ((!type->isAnonymousType() || addFieldsOnly) && (type->_enumMembers.size() > 0)) {
			rc = addFieldAndConstCount(addFieldsOnly, 0, type->_enumMembers.size());
		}
	}
	return rc;
}

DDR_RC
JavaBlobGenerator::dispatchEnumerateType(TypedefUDT *type, bool addFieldsOnly)
{
	/* No op: typedefs are not added to the blob at this time. */
	return DDR_RC_OK;
}

DDR_RC
JavaBlobGenerator::dispatchEnumerateType(ClassUDT *type, bool addFieldsOnly)
{
	DDR_RC rc = DDR_RC_OK;
	if (!type->_isDuplicate) {
	size_t fieldCount = 0;
	size_t constCount = type->_enumMembers.size();

	if ((DDR_RC_OK == rc) && (!type->isAnonymousType() || addFieldsOnly) && (type->_fieldMembers.size() > 0)) {
		for (vector<Field *>::iterator v = type->_fieldMembers.begin(); v != type->_fieldMembers.end(); ++v) {
			if (!(*v)->_isStatic) {
				/* Anonymous type members are added to the struct and not counted as a field themselves. */
				if ((NULL != (*v)->_fieldType) && (*v)->_fieldType->isAnonymousType()) {
					rc = (*v)->_fieldType->enumerateType(this, true);
					if (DDR_RC_OK != rc) {
						break;
					}
				} else {
					fieldCount += 1;
				}
			}
		}
	}

	if (DDR_RC_OK == rc) {
		/* Add only integer constants to the blob. */
		for (vector<Macro>::iterator it = type->_macros.begin(); it != type->_macros.end(); it += 1) {
			if (DDR_RC_OK == it->getNumeric(NULL)) {
				constCount += 1;
			}
		}
	}
	if ((DDR_RC_OK == rc) && (!type->isAnonymousType() || addFieldsOnly) && (type->_fieldMembers.size() > 0)) {
		rc = addFieldAndConstCount(addFieldsOnly, fieldCount, constCount);
	}
	}
	if (DDR_RC_OK == rc) {
		/* When adding the fields from a field with an anonymous type, do not add subUDTs twice. */
		if (!addFieldsOnly) {
			for (vector<UDT *>::iterator v = type->_subUDTs.begin(); v != type->_subUDTs.end(); ++v) {
				rc = (*v)->enumerateType(this, false);
				if (DDR_RC_OK != rc) {
					break;
				}
			}
		}
	}
	return rc;
}

DDR_RC
JavaBlobGenerator::dispatchEnumerateType(UnionUDT *type, bool addFieldsOnly)
{
	DDR_RC rc = DDR_RC_OK;
	if (!type->_isDuplicate) {
		size_t fieldCount = 0;
		size_t constCount = type->_enumMembers.size();

		if ((DDR_RC_OK == rc) && (!type->isAnonymousType() || addFieldsOnly) && (type->_fieldMembers.size() > 0)) {
			for (vector<Field *>::iterator v = type->_fieldMembers.begin(); v != type->_fieldMembers.end(); ++v) {
				if (!(*v)->_isStatic) {
					/* Anonymous type members are added to the struct and not counted as a field themselves. */
					if ((NULL != (*v)->_fieldType) && (*v)->_fieldType->isAnonymousType()) {
						rc = (*v)->_fieldType->enumerateType(this, true);
						if (DDR_RC_OK != rc) {
							break;
						}
					} else {
						fieldCount += 1;
					}
				}
			}
		}

		if (DDR_RC_OK == rc) {
			/* Add only integer constants to the blob. */
			for (vector<Macro>::iterator it = type->_macros.begin(); it != type->_macros.end(); it += 1) {
				if (DDR_RC_OK == it->getNumeric(NULL)) {
					constCount += 1;
				}
			}
		}
		if ((DDR_RC_OK == rc) && (!type->isAnonymousType() || addFieldsOnly) && (type->_fieldMembers.size() > 0)) {
			rc = addFieldAndConstCount(addFieldsOnly, fieldCount, constCount);
		}
	}
	if (DDR_RC_OK == rc) {
		/* When adding the fields from a field with an anonymous type, do not add subUDTs twice. */
		if (!addFieldsOnly) {
			for (vector<UDT *>::iterator v = type->_subUDTs.begin(); v != type->_subUDTs.end(); ++v) {
				rc = (*v)->enumerateType(this, false);
				if (DDR_RC_OK != rc) {
					break;
				}
			}
		}
	}
	return rc;
}

DDR_RC
JavaBlobGenerator::dispatchBuildBlob(Type *type, bool addFieldsOnly, string prefix)
{
	/* No op: base types are not added to the blob at this time. */
	return DDR_RC_OK;
}

DDR_RC
JavaBlobGenerator::dispatchBuildBlob(TypedefUDT *type, bool addFieldsOnly, string prefix)
{
	/* No op: base types are not added to the blob at this time. */
	return DDR_RC_OK;
}
