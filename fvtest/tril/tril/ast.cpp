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

#include "ast.hpp"
#include <stdlib.h>
#include <string.h>

ASTNode* createNode(const char * name, ASTNodeArg* args, ASTNode* children,  ASTNode* next) {
    ASTNode* n = (ASTNode*)malloc(sizeof(ASTNode));
    //printf("  node at %p\n", n);
    n->name = name;
    n->args = args;
    n->next = next;
    //printf("  children at %p\n", children);
    n->children = children;
    return n;
}

ASTNodeArg* createNodeArg(const char * name, ASTValue* value,  ASTNodeArg* next) {
    return new ASTNodeArg{name, value, next};
}

ASTValue* createInt64Value(uint64_t val) {
    return new ASTValue{val};
}

ASTValue* createDoubleValue(double val) {
    return new ASTValue{val};
}

ASTValue* createStrValue(const char* val) {
    return new ASTValue{val};
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

void appendSiblingValue(ASTValue* list, ASTValue* newValue) {
    ASTValue* v = list;
    while (v->next) { v = v->next; }
    v->next = newValue;
}

uint16_t countNodes(const ASTNode* n) {
    uint16_t count = 0;
    while (n) {
       ++count;
       n = n->next;
    }
    return count;
}

const ASTNodeArg* getArgByName(const ASTNode* node, const char* name) {
    const ASTNodeArg* arg = node->args;
    while (arg) {
        if (arg->getName() != NULL && strcmp(name, arg->getName()) == 0) { // arg need not have a name
            return arg;
        }
        arg = arg->next;
    }
    return NULL;
}

const ASTNode* findNodeByNameInList(const ASTNode* list, const char* name) {
    const ASTNode* node = list;
    while (node) {
        if (strcmp(name, node->name) == 0) {
            return node;
        }
        node = node->next;
    }
    return NULL;
}

const ASTNode* findNodeByNameInTree(const ASTNode* tree, const char* name) {
    const ASTNode* node = tree;
    while (node) {
        if (strcmp(name, node->name) == 0) {
            return node;
        }

        const ASTNode* child = findNodeByNameInTree(node, name);
        if (child != NULL) {
            return child;
        }

        node = node->next;
    }
    return NULL;
}

void printASTValueUnion(FILE* file, const ASTValue* value) {
    switch (value->getType()) {
        case Int64: fprintf(file, "%lu", value->get<ASTValue::Integer_t>()); break;
        case Double: fprintf(file, "%f", value->get<ASTValue::Double_t>()); break;
        case String: fprintf(file, "\"%s\"", value->get<ASTValue::String_t>()); break;
        default: fprintf(file, "{bad arg type %d}", value->getType());
    };
}

void printASTValue(FILE* file, const ASTValue* value) {
    const ASTValue* v = value;
    int isList = v->next != NULL;
    if (isList) {
        printf("[");
    }
    printASTValueUnion(file, v);
    while (v->next) {
        v = v->next;
        fprintf(file, ", ");
        printASTValueUnion(file, v);
    }
    if (isList) {
        fprintf(file, "]");
    }
}

void printASTArgs(FILE* file, ASTNodeArg* args) {
    ASTNodeArg* a = args;
    while (a) {
        printf(" ");
        if (a->getName() != NULL && strcmp("", a->getName()) != 0) {
            fprintf(file, "%s=", a->getName());
        }
        printASTValue(file, a->getValue());
        a = a->next;
    }
}

void printTrees(FILE* file, ASTNode* trees, int indent) {
    ASTNode* t = trees;
    while(t) {
        int indentVal = indent*2 + (int)strlen(t->name); // indent with two spaces
        fprintf(file, "(%p) %*s", t, indentVal, t->name);
        printASTArgs(file, t->args);
        fprintf(file, "\n");
        printTrees(file, t->children, indent + 1);
        t = t->next;
    }
}
