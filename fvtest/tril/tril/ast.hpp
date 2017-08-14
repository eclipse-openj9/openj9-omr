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

/**
 * @brief The ASTValue struct represents a "value" in the Tril AST
 *
 * The struct acts as a variant capable of holding values of multiple types. It
 * abstracts away the details of how a value is stored and provides an interface
 * for retrieving a value using an appropriate type.
 *
 * An ASTValue can have one of three types:
 *
 * Type name     | Used for              | Compatible with (C++ types)
 * --------------|-----------------------|--------------------------------------
 * Integer       | integral values       | any type satisfying `std::is_integral`
 * FloatingPoint | floating point values | any type satisfying `std::is_floating_point`
 * String        | character strings     | `const char*`
 *
 * Names for these types are defined in the ASTType enum.
 *
 * To aid usability, this struct provides the `get<T>()` template member function
 * that will return the stored value as type `T` if `T` is compatible with the
 * type of the stored value.
 *
 * For convenience Integer_t, FloatingPoint_t, and String_t are provided as
 * aliases for the underlying C++ types used to store values.
 */
struct ASTValue {
    public:

    // names for the different AST data types
    enum ASTType {Integer, FloatingPoint, String} ;

    // aliases for the underlying C++ types used to store the different AST types
    using Integer_t = uint64_t;
    using FloatingPoint_t = double;
    using String_t = const char *;

    // constructors
    explicit ASTValue(Integer_t v) : _type{Integer}, next{nullptr} { _value.integer = v; }
    explicit ASTValue(FloatingPoint_t v) : _type{FloatingPoint}, next{nullptr} { _value.floatingPoint = v; }
    explicit ASTValue(String_t v) : _type{String}, next{nullptr} { _value.str = v; }

    /**
     * @brief Return the contained value as the specified type
     * @tparam T is the type the contained value should be returned as
     *
     * This function returns the contained value, casted to the specified type.
     * A type check is performed to ensure the specified type is compatible with
     * the type of the stored value.
     *
     * Examples:
     *
     * ASTValue a{4};
     * a.get<int>();   // returns 4 as an int
     * a.get<long>();  // returns 4 as a long
     * a.get<float>(); // causes an assertion failure
     *
     * ASTValue b{4.5};
     * b.get<float>();  // returns 4.5 as a float
     * b.get<double>(); // returns 4.5 as a double
     * b.get<int>();    // causes an assertion failure
     */
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

    /**
     * @brief Checks whether the type of the contained value and the specified type are compatible
     * @tparam T is the type to be checked for compatibility
     *
     * Examples:
     *
     * ASTValue a{3};
     * a.isCompatibleWith<int>();    // returns true
     * a.isCompatibleWith<long>();   // returns true
     * a.isCompatibleWith<double>(); // returns false
     *
     * ASTValue b{4.5};
     * b.isCompatibleWith<float>();  // returns true
     * b.isCompatibleWith<double>(); // returns true
     * b.isCompatibleWith<int>();    // returns false
     */
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

    /**
     * @brief Returns the ASTType of the contained value
     * @return
     */
    ASTType getType() const { return _type; }

    private:

    // tag containing specifying the type of the contained value
    ASTType _type;

    // union holding the contained value
    union {
        Integer_t integer;
        FloatingPoint_t floatingPoint;
        String_t str;
    } _value;

    public:
    ASTValue* next;
};

// overloaded operators for ASTValue

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

/**
 * @brief A struct representing arguments of nodes in the Tril AST
 */
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

/**
 * @brief A struct representing nodes in the Tril AST
 */
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
