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

#include "ast.h"
#include <stdlib.h>
#include <string.h>

ASTNode* createNode(char * name, ASTNodeArg* args, ASTNode* children,  ASTNode* next) {
    ASTNode* n = (ASTNode*)malloc(sizeof(ASTNode));
    //printf("  node at %p\n", n);
    n->name = name;
    n->args = args;
    n->next = next;
    //printf("  children at %p\n", children);
    n->children = children;
    return n;
}

ASTNodeArg* createNodeArg(const char * name, ASTValue value,  ASTNodeArg* next) {
    ASTNodeArg* a = (ASTNodeArg*)malloc(sizeof(ASTNodeArg));
    a->name = name;
    a->value = value;
    a->next = next;
    return a;
}

ASTValue createInt64Value(uint64_t val) {
    ASTValue v;
    v.type = Int64;
    v.value.int64 = val;
    return v;
}

ASTValue createDoubleValue(double val) {
    ASTValue v;
    v.type = Double;
    v.value.f64 = val;
    return v;
}

ASTValue createStrValue(const char* val) {
   ASTValue v;
   v.type = String;
   v.value.str = val;
   return v;
}

void appendSiblingNode(ASTNode* list, ASTNode* newNode) {
    ASTNode* n = list;
    while (n->next) { n = n->next; }
    n->next = newNode;
}

void appendSiblingArg(ASTNodeArg* list, ASTNodeArg* newArg) {
   ASTNodeArg* a = list;
   while (a->next) { a = a->next; }
   a->next = newArg;
}

uint16_t countNodes(const ASTNode* n) {
    uint16_t count = 0;
    while (n) {
       ++count;
       n = n->next;
    }
    return count;
}

void printTrees(ASTNode* trees, int indent) {
    ASTNode* t = trees;
    while(t) {
        int indentVal = indent*2 + (int)strlen(t->name); // indent with two spaces
        printf("(%p) %*s", t, indentVal, t->name);
        ASTNodeArg* a = t->args;
        while (a) {
            printf(" ");
            if (a->name != NULL && strcmp("", a->name) != 0) {
                printf("%s=", a->name);
            }
            switch (a->value.type) {
                case Int64: printf("%lu", a->value.value.int64); break;
                case Double: printf("%f", a->value.value.f64); break;
                case String: printf("\"%s\"", a->value.value.str); break;
                default: printf(" [bad arg]");
            }
            a = a->next;
        }
        printf("\n");
        printTrees(t->children, indent + 1);
        t = t->next;
    }
}
