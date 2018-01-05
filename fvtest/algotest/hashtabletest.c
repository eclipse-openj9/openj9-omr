/*******************************************************************************
 * Copyright (c) 2009, 2016 IBM Corp. and others
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

#include <stdlib.h>
#include <string.h>
#include "algorithm_test_internal.h"
#include "avl_api.h"
#include "hashtable_api.h"
#include "omrport.h"
/*
 * Testing the following functions of J9HashTable using the J9HASH_TABLE_ALLOW_SIZE_OPTIMIZATION flag:
 * 		hashTableAdd()
 * 		hashTableGetCount()
 * 		hashTableFind()
 * 		hashTableStartDo()
 * 		hashTableNextDo()
 * 		hashTableRemove()
 */

static uintptr_t hashFn(void *key, void *userData);
static uintptr_t hashEqualFn(void *leftKey, void *rightKey, void *userData);
static uintptr_t dataOffset(uintptr_t startOffset, uintptr_t dataLength, uintptr_t i);

extern const uint8_t RandomValues[256];

#define FORWARD 0
#define REVERSE -1

static uintptr_t
hashEqualFn(void *leftKey, void *rightKey, void *userData)
{
	return *(uintptr_t *)leftKey == *(uintptr_t *)rightKey;
}

static uintptr_t
hashFn(void *key, void *userData)
{
	BOOLEAN forceCollisions = (BOOLEAN)((uintptr_t)userData);
	if (forceCollisions) {
		return (*(uintptr_t *)key) & 0x1;
	} else {
		return *(uintptr_t *)key;
	}
}

static intptr_t
hashComparatorFn(struct J9AVLTree *tree, struct J9AVLTreeNode *leftNode, struct J9AVLTreeNode *rightNode)
{
	uintptr_t leftKey = *((uintptr_t *)(leftNode + 1));
	uintptr_t rightKey = *((uintptr_t *)(rightNode + 1));

	return (intptr_t)leftKey - (intptr_t)rightKey;
}

static uintptr_t dataOffset(uintptr_t startOffset, uintptr_t dataLength, uintptr_t i)
{
	uintptr_t result;
	if (startOffset == REVERSE) {
		return dataLength - (i + 1);
	}
	result = startOffset + i;
	if (result >= dataLength) {
		result -= dataLength;
	}
	return result;
}

static BOOLEAN
checkHashtableIntegrity(OMRPortLibrary *portLib, J9HashTable *table, const uintptr_t *data, uintptr_t dataLength, uintptr_t removeOffset, uintptr_t i)
{
	uintptr_t count, j;
	uintptr_t dup[256];
	J9HashTableState walkState;
	uintptr_t *next;

	count = hashTableGetCount(table);
	if (count != dataLength - (i + 1)) {
		return FALSE;
	}

	/* walk all the elements, checking for duplicates and ensuring the elements are valid */
	memset(dup, 0, sizeof(dup));
	count = 0;
	next = hashTableStartDo(table, &walkState);
	while (next != NULL) {
		uintptr_t entry;
		uintptr_t *node;
		BOOLEAN found = FALSE;
		count++;
		if (*next >= sizeof(dup) / sizeof(uintptr_t)) {
			return FALSE;
		}
		if (dup[*next]) {
			return FALSE;
		}
		dup[*next] = 1;
		for (j = i + 1; j < dataLength; j++) {
			if (*next == data[dataOffset(removeOffset, dataLength, j)]) {
				found = TRUE;
				break;
			}
		}
		if (!found) {
			return FALSE;
		}

		entry = *next;
		node = hashTableFind(table, &entry);
		if (node == NULL || *node != entry) {
			return FALSE;
		}

		next = hashTableNextDo(&walkState);
	}
	if (count != dataLength - (i + 1)) {
		return FALSE;
	}

	for (j = i + 1; j < dataLength; j++) {
		uintptr_t *node;
		uintptr_t entry = data[dataOffset(removeOffset, dataLength, j)];
		node = hashTableFind(table, &entry);
		if (node == NULL || *node != entry) {
			return FALSE;
		}
	}

	return TRUE;
}

static int32_t
runHashtableTests(OMRPortLibrary *portLib, J9HashTable *table, const uintptr_t *data, uintptr_t dataLength, uintptr_t removeOffset)
{
	uintptr_t i = 0;
	uintptr_t entry = 0;

	/* add all the data elements */
	for (i = 0; i < dataLength; i++) {
		entry = data[i];
		if (hashTableAdd(table, &entry) == NULL) {
			return -1;
		}
	}

	/* ensure the count is correct */
	if (hashTableGetCount(table) != dataLength) {
		return -2;
	}

	/* find all the elements */
	for (i = 0; i < dataLength; i++) {
		uintptr_t *node = NULL;
		entry = data[i];
		node = hashTableFind(table, &entry);
		if (node == NULL || *node != entry) {
			return -3;
		}
	}

	if (checkHashtableIntegrity(portLib, table, data, dataLength, 0, -1) == FALSE) {
		return -4;
	}

	/* remove all elements verifying the integrity */
	for (i = 0; i < dataLength; i++) {
		entry = data[dataOffset(removeOffset, dataLength, i)];
		if (hashTableRemove(table, &entry) != 0) {
			return -5;
		}
		if (checkHashtableIntegrity(portLib, table, data, dataLength, removeOffset, i) == FALSE) {
			return -6;
		}
	}

	return 0;
}

static J9HashTable *
allocateHashtable(OMRPortLibrary *portLib, HashtableInputData *inputData)
{
	J9HashTable *hashtable = NULL;
	const char *tableName = inputData->hashtableName;
	uint32_t tableSize = 17;
	uint32_t entrySize = sizeof(uintptr_t);
	uint32_t flags = 0;
	void *userData = (void *)(uintptr_t)inputData->forceCollisions;

	if (TRUE == inputData->collisionResistant) {
		hashtable = collisionResilientHashTableNew(portLib,
				tableName,
				tableSize,
				entrySize,
				flags,
				OMRMEM_CATEGORY_VM,
				inputData->listToTreeThreshold,
				hashFn,
				hashComparatorFn,
				NULL,
				userData);
	} else {
		hashtable = hashTableNew(portLib,
				tableName,
				tableSize,
				entrySize,
				sizeof(char *),
				flags | J9HASH_TABLE_ALLOW_SIZE_OPTIMIZATION,
				OMRMEM_CATEGORY_VM,
				hashFn,
				hashEqualFn,
				NULL,
				userData);
	}

	return hashtable;
}

int32_t
buildAndVerifyHashtable(OMRPortLibrary *portLib, HashtableInputData *inputData)
{
	J9HashTable *table = NULL;
	uintptr_t i = 0;
	int32_t result = 0;

	table = allocateHashtable(portLib, inputData);
	if (NULL == table) {
		result = -1;
		goto fail;
	}

	if (0 != runHashtableTests(portLib, table, inputData->data, inputData->dataLength, REVERSE)) {
		result = -2;
		goto fail;
	}
	for (i = 0; i < inputData->dataLength; i++) {
		if (0 != runHashtableTests(portLib, table, inputData->data, inputData->dataLength, i)) {
			result = -3;
			goto fail;
		}
	}
fail:
	hashTableFree(table);
	return result;
}
