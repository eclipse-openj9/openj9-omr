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

namespace TRTest
{

/**
 * @brief A family of functions returning constants of the specified type
 */
template <typename T> T zero_value() { return static_cast<T>(0); }
template <typename T> T positive_value() { return static_cast<T>(3); }
template <typename T> T negative_value() { return static_cast<T>(-2); }

/**
 * @brief Convenience function returning possible test inputs of the specified type
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
 * @brief Convenience function returning pairs of possible test inputs of the specified types
 */
template <typename L, typename R>
std::vector<std::tuple<L,R>> const_value_pairs()
   {
   return TRTest::combine(const_values<L>(), const_values<R>());
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

//~ Opcode test fixtures ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

template <typename Ret, typename... Args>
class OpCodeTest : public JitTest, public ::testing::WithParamInterface<ParamType<Ret, Args...>> {};

template <typename T>
class BinaryOpTest : public JitTest, public ::testing::WithParamInterface<BinaryOpParamType<T,T,T>> {};

} // namespace CompTest

#endif // OPCODETEST_HPP
