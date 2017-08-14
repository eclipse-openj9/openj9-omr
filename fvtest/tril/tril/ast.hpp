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

#include <type_traits>
#include <assert.h>
#include <cstring>

struct ASTValue {
    public:
    enum ASTType {Integer, FloatingPoint, String} ;

    private:
    ASTType _type;
    union {
        uint64_t integer;
        double floatingPoint;
        const char* str;
    } _value;

    public:
    ASTValue* next;

    using Integer_t = uint64_t;
    using FloatingPoint_t = double;
    using String_t = const char *;

    explicit ASTValue(Integer_t v) : _type{Integer}, next{nullptr} { _value.integer = v; }
    explicit ASTValue(FloatingPoint_t v) : _type{FloatingPoint}, next{nullptr} { _value.floatingPoint = v; }
    explicit ASTValue(String_t v) : _type{String}, next{nullptr} { _value.str = v; }

    template <typename T>
    typename std::enable_if<std::is_integral<T>::value, T>::type get() const {
        assert(Integer == _type);
        return static_cast<T>(_value.integer);
    }

    template <typename T>
    typename std::enable_if<std::is_floating_point<T>::value, T>::type get() const {
        assert(FloatingPoint == _type);
        return static_cast<T>(_value.floatingPoint);
    }

    template <typename T>
    typename std::enable_if<std::is_same<String_t, T>::value, T>::type get() const {
        assert(String == _type);
        return _value.str;
    }

    template <typename T>
    typename std::enable_if<std::is_integral<T>::value, bool>::type isCompatibleWith() const {
        return Integer == _type;
    }
    template <typename T>
    typename std::enable_if<std::is_floating_point<T>::value, bool>::type isCompatibleWith() const {
        return FloatingPoint == _type;
    }
    template <typename T>
    typename std::enable_if<std::is_same<String_t, T>::value, bool>::type isCompatibleWith() const {
        return String == _type;
    }

    ASTType getType() const { return _type; }
};

inline bool operator == (const ASTValue& lhs, const ASTValue& rhs) {
   if (lhs.getType() == rhs.getType()) {
      switch (lhs.getType()) {
         case ASTValue::Integer: return lhs.get<ASTValue::Integer_t>() == rhs.get<ASTValue::Integer_t>();
         case ASTValue::FloatingPoint: return lhs.get<ASTValue::FloatingPoint_t>() == rhs.get<ASTValue::FloatingPoint_t>();
         case ASTValue::String: return std::strcmp(lhs.get<ASTValue::String_t>(), rhs.get<ASTValue::String_t>()) == 0;
      }
   }
   return false;
}

struct ASTNodeArg {
    private:
    const char* _name;
    const ASTValue* _value;

    public:
    ASTNodeArg* next;

    ASTNodeArg(const char* name, ASTValue* value, ASTNodeArg* next = nullptr)
        : _name{name}, _value{value}, next{next} {}

    const char* getName() const { return _name; }
    const ASTValue* getValue() const { return _value; }
};

struct ASTNode {
    private:
    const char* _name;
    ASTNodeArg* _args;
    ASTNode* _children;

    public:
    ASTNode* next;

    ASTNode(const char* name, ASTNodeArg* args, ASTNode* children, ASTNode* next)
        : _name{name}, _args{args}, _children{children}, next{next} {}

    const char* getName() const { return _name; }
    const ASTNodeArg* getArgs() const { return _args; }
    const ASTNode* getChildren() const { return _children; }
    int getChildCount() const { return countNodes(_children); }
};

#endif // AST_HPP
