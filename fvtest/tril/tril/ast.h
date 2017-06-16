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

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <stdint.h>

typedef enum {Int64, Double, String} ASTValueType;

typedef struct {
    ASTValueType type;
    union {
        uint64_t int64;
        double f64;
        const char* str;
    } value;
} ASTValue;

struct ASTNodeArg;
typedef struct ASTNodeArg ASTNodeArg;
struct ASTNodeArg {
    const char* name;
    ASTValue value;
    ASTNodeArg* next;
};

struct ASTNode;
typedef struct ASTNode ASTNode;
struct ASTNode {
    char* name;
    ASTNodeArg* args;
    ASTNode* children;
    ASTNode* next;
};

ASTNode* createNode(char* name, ASTNodeArg* args, ASTNode* children,  ASTNode* next);

ASTNodeArg* createNodeArg(const char* name, ASTValue value,  ASTNodeArg* next);

ASTValue createInt64Value(uint64_t val);
ASTValue createDoubleValue(double val);
ASTValue createStrValue(const char* val);

void appendSiblingNode(ASTNode* list, ASTNode* newNode);
void appendSiblingArg(ASTNodeArg* list, ASTNodeArg* newArg);

void printTrees(ASTNode* trees, int indent);

ASTNode* genTrees(FILE* in);

#ifdef __cplusplus
}
#endif

#endif // AST_H
