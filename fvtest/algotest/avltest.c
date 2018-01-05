/*******************************************************************************
 * Copyright (c) 1991, 2015 IBM Corp. and others
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

#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#include "omrport.h"
#include "avl_api.h"


/* we need isspace on ce platforms */
#if !defined(isspace)
#define isspace(x) ( ((x)==' ')||((x)=='\t')||((x)=='\r')||((x)=='\n') )
#endif

typedef struct OMRAVLTestNode {
	J9WSRP leftChild;
	J9WSRP rightChild;
	int datum;
} OMRAVLTestNode;

static BOOLEAN avlTestInsert(OMRPortLibrary *portlib, J9AVLTree *tree, uintptr_t val);
static intptr_t testInsertionComparator(J9AVLTree *tree, OMRAVLTestNode *insertNode, OMRAVLTestNode *walkNode);
static uintptr_t get_node_string(OMRPortLibrary *portlib, J9AVLTree *tree, OMRAVLTestNode *walk, char *buffer, uintptr_t bufSize);
static void avlSetup(J9AVLTree *tree);
static uintptr_t get_datum_string(OMRPortLibrary *portlib, OMRAVLTestNode *walk, char *buffer, uintptr_t bufSize);
static void avl_get_string(OMRPortLibrary *portlib, J9AVLTree *tree, char *buffer, uintptr_t bufSize);
static intptr_t testSearchComparator(J9AVLTree *tree, intptr_t search, OMRAVLTestNode *walkNode);
static void freeAVLTree(OMRPortLibrary *portlib, J9AVLTreeNode *currentNode);

int32_t
buildAndVerifyAVLTree(OMRPortLibrary *portLib, const char *success, const char *testData)
{
	J9AVLTree tree;
	char buffer[1024];
	const char *actionStr, *actionStrEnd;
	int32_t action;
	int remove;
	int value;
	int32_t result = -1;

	OMRPORT_ACCESS_FROM_OMRPORT(portLib);

	avlSetup(&tree);

	actionStr = testData;

	for (;;) {
		/* advance past the whitespace, so we can accurately determine failure */
		while (isspace(*actionStr)) {
			actionStr++;
		}

		/* walk past the number */
		actionStrEnd = actionStr;
		if (*actionStrEnd == '-') {
			remove = 1;
			actionStrEnd++;
		} else {
			remove = 0;
		}

		action = 0;

		/* a big wad of ugly ebcidic safe code */
		for (;;) {
			switch (*actionStrEnd) {
			case '0':
				value = 0;
				break;
			case '1':
				value = 1;
				break;
			case '2':
				value = 2;
				break;
			case '3':
				value = 3;
				break;
			case '4':
				value = 4;
				break;
			case '5':
				value = 5;
				break;
			case '6':
				value = 6;
				break;
			case '7':
				value = 7;
				break;
			case '8':
				value = 8;
				break;
			case '9':
				value = 9;
				break;
			default:
				value = -1;
				break;
			}
			if (value < 0) {
				break;
			}
			action = action * 10 + value;
			actionStrEnd++;
		}

		if (action == 0) {
			/* -0 -garbage or garbage */
			if ((remove == 1) || (actionStrEnd == actionStr)) {
				goto fail;
			}
			while (isspace(*actionStrEnd)) {
				actionStrEnd++;
			}
			if (*actionStrEnd != 0) {
				goto fail;
			}

			avl_get_string(portLib, &tree, buffer, 1024);

			if (0 != strcmp(buffer, success)) {
				goto fail;
			}
			break;
		}

		actionStr = actionStrEnd;
		while (isspace(*actionStr)) {
			actionStr++;
		}
		if (*actionStr != ',') {
			goto fail;
		}
		actionStr++;

		if (remove == 1) {
			J9AVLTreeNode *n = avl_search(&tree, action);
			if (n) {
				avl_delete(&tree, n);
				omrmem_free_memory(n);
			}
		} else {
			if (avlTestInsert(portLib, &tree, action) == FALSE) {
				goto fail;
			}
		}
	}

	result = 0;
fail:
	freeAVLTree(portLib, tree.rootNode);
	return result;
}


static uintptr_t
get_datum_string(OMRPortLibrary *portlib, OMRAVLTestNode *walk, char *buffer, uintptr_t bufSize)
{
	OMRPORT_ACCESS_FROM_OMRPORT(portlib);
	uintptr_t out = omrstr_printf(buffer, bufSize, "%i", walk->datum);
	if (out >= bufSize) {
		return bufSize;
	}
	return out;
}

static void avlSetup(J9AVLTree *tree)
{
	memset(tree, 0, sizeof(J9AVLTree));
	tree->insertionComparator = (intptr_t ( *)(struct J9AVLTree *, J9AVLTreeNode *, J9AVLTreeNode *))testInsertionComparator;
	tree->searchComparator = (intptr_t ( *)(struct J9AVLTree *, uintptr_t, J9AVLTreeNode *))testSearchComparator;
}

static void freeAVLTree(OMRPortLibrary *portlib, J9AVLTreeNode *currentNode)
{
	OMRPORT_ACCESS_FROM_OMRPORT(portlib);
	if (currentNode != NULL) {
		J9AVLTreeNode *leftChild = J9AVLTREENODE_LEFTCHILD(currentNode);
		J9AVLTreeNode *rightChild = J9AVLTREENODE_RIGHTCHILD(currentNode);

		freeAVLTree(portlib, leftChild);
		freeAVLTree(portlib, rightChild);
		omrmem_free_memory(currentNode);
	}
}


static BOOLEAN avlTestInsert(OMRPortLibrary *portlib, J9AVLTree *tree, uintptr_t val)
{
	OMRAVLTestNode *node = NULL;
	OMRAVLTestNode *t1;
	OMRAVLTestNode *t2;

	OMRPORT_ACCESS_FROM_OMRPORT(portlib);

	node = omrmem_allocate_memory(sizeof(OMRAVLTestNode), OMRMEM_CATEGORY_VM);
	if (!node) {
		omrtty_printf("out of memory");
		return FALSE;
	}

	memset(node, 0, sizeof(OMRAVLTestNode));
	node->datum = (int)val;

	t1 = (OMRAVLTestNode *)avl_search(tree, val);

	t2 = (OMRAVLTestNode *)avl_insert(tree, (J9AVLTreeNode *)node);

	/* Node exist in AVLTree */
	if (t1 != NULL) {
		if (t2 == NULL) {
			omrtty_printf("insert failed\n");
			omrmem_free_memory(node);
			return FALSE;
		} else if (t2 == t1) {
			omrmem_free_memory(node);
			return TRUE;
		} else if (t2 != t1) {
			omrtty_printf("insert added, when it should have found\n");
			/* Might not be a good idea to free the memory since it is in AVLTree now */
			return FALSE;
		}
	} else {
		if (t2 == NULL) {
			omrtty_printf("insert failed\n");
			omrmem_free_memory(node);
			return FALSE;
		} else if (t2 != node) {
			omrtty_printf("insert found, when it should have added\n");
			omrmem_free_memory(node);
		}
	}
	return TRUE;
}


static intptr_t testSearchComparator(J9AVLTree *tree, intptr_t search, OMRAVLTestNode *walkNode)
{
	return search - walkNode->datum;
}

static intptr_t testInsertionComparator(J9AVLTree *tree, OMRAVLTestNode *insertNode, OMRAVLTestNode *walkNode)
{
	return insertNode->datum - walkNode->datum;
}

static void avl_get_string(OMRPortLibrary *portlib, J9AVLTree *tree, char *buffer, uintptr_t bufSize)
{
	uintptr_t len = get_node_string(portlib, tree, (OMRAVLTestNode *)tree->rootNode, buffer, bufSize);
	if (len >= bufSize) {
		buffer[bufSize - 1] = 0;
	} else {
		buffer[len] = 0;
	}
}

static uintptr_t get_node_string(OMRPortLibrary *portlib, J9AVLTree *tree, OMRAVLTestNode *walk, char *buffer, uintptr_t bufSize)
{
	uintptr_t len;

	if (bufSize < 3) {
		return 0;
	}

	if (walk == 0) {
		buffer[0] = '@';
		return 1;
	}

	buffer[0] = '{';
	if (AVL_GETBALANCE(walk) == AVL_BALANCED) {
		buffer[1] = '=';
	} else if (AVL_GETBALANCE(walk) == AVL_LEFTHEAVY) {
		buffer[1] = 'L';
	} else {
		buffer[1] = 'R';
	}

	len = 2;
	len += get_datum_string(portlib, walk, &buffer[len], bufSize - len);
	len += get_node_string(portlib, tree, (OMRAVLTestNode *)AVL_SRP_GETNODE(walk->leftChild), &buffer[len], bufSize - len);
	len += get_node_string(portlib, tree, (OMRAVLTestNode *)AVL_SRP_GETNODE(walk->rightChild), &buffer[len], bufSize - len);

	if ((bufSize - len) > 1) {
		buffer[len++] = '}';
	}
	return len;
}
