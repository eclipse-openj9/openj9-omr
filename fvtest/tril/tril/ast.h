/*******************************************************************************
 *
 * (c) Copyright IBM Corp. 2017, 2017
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
 ******************************************************************************/

#ifndef AST_H
#define AST_H

#include <stdio.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/*
 * This file containes declarations that act as a C wrapper around the Tril AST
 * interface. This wrapper is mainly used by the Flex/Bison parser.
 */

/*
 * Forward declarations for the AST structs (classes)
 *
 * These allow C code to interact with pointers to the AST structures.
 */
typedef struct ASTValue ASTValue;
typedef struct ASTNodeArg ASTNodeArg;
typedef struct ASTNode ASTNode;

/*
 * The following functions are wrappers for the AST struct constructors.
 */

ASTNode* createNode(const char* name, ASTNodeArg* args, ASTNode* children,  ASTNode* next);

ASTNodeArg* createNodeArg(const char* name, ASTValue * value,  ASTNodeArg* next);

ASTValue* createIntegerValue(uint64_t val);
ASTValue* createFloatingPointValue(double val);
ASTValue* createStrValue(const char* val);

/**
 * @brief Adds an instance of ASTNode to the end of a (linked-) list
 * @param list is the linked-list that will be added to
 * @param newNode is the object that will be added to the list
 */
void appendSiblingNode(ASTNode* list, ASTNode* newNode);

/**
 * @brief Adds an instance of ASTNodeArg to the end of a (linked-) list
 * @param list is the linked-list that will be added to
 * @param newArg is the object that will be added to the list
 */
void appendSiblingArg(ASTNodeArg* list, ASTNodeArg* newArg);

/**
 * @brief Adds an instance of ASTValue to the end of a (linked-) list
 * @param list is the linked-list that will be added to
 * @param newValue is the object that will be added to the list
 */
void appendSiblingValue(ASTValue* list, ASTValue* newValue);

/**
 * @brief Returns the number of ASTNode instances in a (linked-) list
 * @param n is the start of the linked-list
 */
uint16_t countNodes(const ASTNode* n);

/**
 * @brief Searches a (linked-) list for an instance of ASTNode with a given name
 * @param list is the start of the linked-list to be searched
 * @param name is the name of the ASTNode instance being searched for
 * @return the instance of ASTNode if found, NULL otherwise
 */
const ASTNode* findNodeByNameInList(const ASTNode* list, const char* name);

/**
 * @brief Searches an AST sub-tree for an instance of ASTNode with a given name
 * @param tree is the root of the tree to be searched
 * @param name is the name of the ASTNode instance being searched for
 * @return the instance for ASTNode if found, NULL otherwise
 */
const ASTNode* findNodeByNameInTree(const ASTNode* tree, const char* name);

/*
 * Convenience functions for printing an AST sub-tree to a file handle.
 */

void printASTValueUnion(FILE* file, const ASTValue* value);
void printASTValue(FILE* file, const ASTValue* value);
void printASTArgs(FILE* file, const ASTNodeArg* args);
void printTrees(FILE* file, const ASTNode* trees, int indent);

/**
 * @brief Parse an input file containing Tril code
 * @param in is a handle pointing to the input file
 * @return a Tril AST representing the parsed code, or NULL if parsing failed
 */
ASTNode* parseFile(FILE* in);

/**
 * @brief Parse Tril code from an input string
 * @param in is the string to be parsed
 * @return a Tril AST representing the parsed code, or NULL if parsing failed
 */
ASTNode* parseString(const char* in);

#ifdef __cplusplus
}
#endif

#endif // AST_H
