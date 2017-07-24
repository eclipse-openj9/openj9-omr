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

#ifndef OPCODETEST_HPP
#define OPCODETEST_HPP

#include "JitTest.hpp"

#include <string>
#include <tuple>
#include <vector>
#include <initializer_list>
#include <iterator>
#include <limits>

#define INT32_POS 3
#define INT32_NEG -2
#define INT32_ZERO 0

/**
 * @brief A family of functions returning constants of the specified type
 */
template <typename T> T zero_value() { return static_cast<T>(0); }
template <typename T> T positive_value() { return static_cast<T>(3); }
template <typename T> T negative_value() { return static_cast<T>(-2); }

/**
 * @brief A convenience function returning possible test inputs of the specified type
 */
template <typename T>
std::vector<T> const_values()
   {
   return std::vector<T>{ zero_value<T>(),
                          positive_value<T>(),
                          negative_value<T>(),
                          std::numeric_limits<T>::min(),
                          std::numeric_limits<T>::max() };
   }

/**
 * @brief Type for holding argument to parameterized opcode tests
 * @tparam Ret the type returned by the opcode
 * @tparam Args the types of the arguments to the opcode
 *
 * This type is just a tuple that packages the different parts of a argument for
 * an opcode test. The first field is another tuple holding the input values to
 * be given to the opcode for testing. The second field is a two-tuple containing
 * the opcode's name as a string, and a pointer to an oracle function that
 * returns the expected return value of the opcode test when given the input
 * values from the first part of the outer tuple.
 */
template <typename Ret, typename... Args>
using ParamType = std::tuple<std::tuple<Args...>, std::tuple<std::string, Ret (*)(Args...)> >;

/**
 * @brief Type for holding argument to parameterized binary opcode tests
 */
template <typename Ret, typename Left, typename Right>
using BinaryOpParamType = ParamType<Ret, Left, Right>;

/**
 * @brief Struct equivalent to the BinaryOpParamType tuple
 *
 * Used for easier unpacking of test argument.
 */
template <typename Ret, typename Left, typename Right>
struct BinaryOpParamStruct {
        Left lhs;
        Right rhs;
        std::string opcode;
        Ret (*oracle)(Left, Right);
};

/**
 * @brief Given an instance of BinaryOpParamType, returns an equivalent instance
 *    of BinaryOpParamStruct
 */
template <typename Ret, typename Left, typename Right>
BinaryOpParamStruct<Ret, Left, Right> to_struct(BinaryOpParamType<Ret, Left, Right> param) {
    BinaryOpParamStruct<Ret, Left, Right> s;
    s.lhs = std::get<0>(std::get<0>(param));
    s.rhs = std::get<1>(std::get<0>(param));
    s.opcode = std::get<0>(std::get<1>(param));
    s.oracle = std::get<1>(std::get<1>(param));
    return s;
}

/**
 * @brief Given two lists input values, returns their cartesain product
 *
 * The values are specified as argument. The first five are treated as the first
 * set and the remaining five as the second set. The returned value is a vector
 * of two-tuples containing the cartesian product of the two sets. The intention
 * is to use this function to generate all possible combinations of input to a
 * function. There is convention that for each set: the first value is the
 * "zero" value, the second is some positive value, the third is some negative
 * value, the fourth is the minimum value, and the last is the maximum value.
 * Following this convention is neither required nor enforced.
 */
template <typename L, typename R>
std::vector<std::tuple<L, R>> make_value_pairs(L lzero, L lpos, L lneg, L lmin, L lmax,
                                               R rzero, R rpos, R rneg, R rmin, R rmax) {
    return std::vector<std::tuple<L, R>>{{
        std::make_tuple(lzero, rzero),
        std::make_tuple(lzero, rpos),
        std::make_tuple(lpos, rzero),
        std::make_tuple(lpos, rpos),
        std::make_tuple(lzero, rneg),
        std::make_tuple(lneg, rzero),
        std::make_tuple(lneg, rneg),
        std::make_tuple(lneg, rpos),
        std::make_tuple(lpos, rneg),
        std::make_tuple(lmax, rzero),
        std::make_tuple(lzero, rmax),
        std::make_tuple(lmin, rzero),
        std::make_tuple(lzero, rmin),
        std::make_tuple(lmax, rpos),
        std::make_tuple(lpos, rmax),
        std::make_tuple(lmin, rpos),
        std::make_tuple(lpos, rmin),
        std::make_tuple(lmax, rneg),
        std::make_tuple(lneg, rmax),
        std::make_tuple(lmin, rneg),
        std::make_tuple(lneg, rmin),
        std::make_tuple(lmax, rmax),
        std::make_tuple(lmin, rmin),
        std::make_tuple(lmax, rmin),
        std::make_tuple(lmin, rmax) }};
}

/**
 * @brief Given a set of input values, returns the Cartesian product of the set with itself
 */
template <typename T>
std::vector<std::tuple<T, T>> make_value_pairs(T zero, T pos, T neg, T min, T max) {
    return make_value_pairs<T, T>(zero, pos, neg, min, max, zero, pos, neg, min, max);
}

/**
 * @brief Returns the Cartesian product of two standard-conforming containers
 * @tparam L is the type of the first input standard-conforming container
 * @tparam R is the type of the second input standard-conforming container
 * @param l is the first input standard-conforming container
 * @param r is the second input standard-conforming container
 * @return the cartesion product of the input containers as a std::vector of std::tuple
 *
 * Example:
 *
 *    combine(std::vector<int>{1, 2, 3}, std::list<float>{4.0, 5.0, 6.0})
 */
template <typename L, typename R>
std::vector<std::tuple<typename L::value_type, typename R::value_type>> combine(L l, R r)
   {
   auto v = std::vector<std::tuple<typename L::value_type, typename R::value_type>>{};
   v.reserve((l.end() - l.begin())*(r.end() - r.begin()));
   for (auto i = l.begin(); i != l.end(); ++i)
      for (auto j = r.begin(); j != r.end(); ++j)
         v.push_back(std::make_tuple(*i, *j));
   return v;
   }

/**
 * @brief Returns the Cartesian product of two initializer lists
 * @tparam L is the type of elements in the first initializer list
 * @tparam R is the type of elements in the second initializer list
 * @param l is the first initializer list
 * @param r is the second initializer list
 * @return the Cartesian product of the two input lists
 *
 * Becuase of the rules surrounding type-deduction of initializer lists, the
 * types of the elements of the two lists must be explicitely specified when
 * calling this function.
 *
 * Example:
 *
 *    combine<int, float>({1, 2, 3}, {4.0, 5.0, 6.0})
 *
 */
template <typename L, typename R>
std::vector<std::tuple<L, R>> combine(std::initializer_list<L> l, std::initializer_list<R> r)
   {
   auto v = std::vector<std::tuple<L, R>>{};
   v.reserve((l.end() - l.begin())*(r.end() - r.begin()));
   for (auto i = l.begin(); i != l.end(); ++i)
      for (auto j = r.begin(); j != r.end(); ++j)
         v.push_back(std::make_tuple(*i, *j));
   return v;
   }

/**
 * @brief Given a vector and a predicate, returns a copy of the vector with the
 *    elements matching the predicate removed
 */
template <typename T, typename Predicate>
std::vector<T> filter(std::vector<T> range, Predicate pred) {
    auto end = std::remove_if(std::begin(range), std::end(range), pred);
    range.erase(end, std::end(range));
    return range;
}

//~ Opcode test fixtures ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

template <typename Ret, typename... Args>
class OpCodeTest : public JitTest, public ::testing::WithParamInterface<ParamType<Ret, Args...>> {};

template <typename T>
class BinaryOpTest : public JitTest, public ::testing::WithParamInterface<BinaryOpParamType<T,T,T>> {};

#endif // OPCODETEST_HPP
