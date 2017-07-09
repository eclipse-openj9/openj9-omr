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
#include <iterator>

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
 * @brief Given a set of input values, returns the cartesian product of the set with itself
 */
template <typename T>
std::vector<std::tuple<T, T>> make_value_pairs(T zero, T pos, T neg, T min, T max) {
    return make_value_pairs<T, T>(zero, pos, neg, min, max, zero, pos, neg, min, max);
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
