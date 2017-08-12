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

#ifndef AST_HPP
#define AST_HPP

#include "ast.h"

#include <cstring>

struct ASTValue {
    ASTValueType type;
    union {
        uint64_t int64;
        double f64;
        const char* str;
    } value;
    ASTValue* next;
};

struct ASTNodeArg {
    const char* name;
    ASTValue* value;
    ASTNodeArg* next;
};

struct ASTNode {
    const char* name;
    ASTNodeArg* args;
    ASTNode* children;
    ASTNode* next;
};

inline bool operator == (ASTValue lhs, ASTValue rhs) {
   if (lhs.type == rhs.type) {
      switch (lhs.type) {
         case Int64: return lhs.value.int64 == rhs.value.int64;
         case Double: return lhs.value.f64 == rhs.value.f64;
         case String: return std::strcmp(lhs.value.str, rhs.value.str) == 0;
      }
   }
   return false;
}

#endif // AST_HPP
