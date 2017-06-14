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

#ifndef METHODHANDLER_HPP
#define METHODHANDLER_HPP

#include "ilgen.hpp"

#include <string>
#include <vector>
#include <type_traits>

/**
 * @brief Class for handling Tril ASTs representing methods
 *
 * Given a Tril AST representing a method, this class takes care of figuring out
 * how to compile the method and invoke the method. As part of this, it also
 * takes care of generating the IL for the method represented in the AST.
 */
class MethodHandler {
    public:

        /**
         * @brief Constructs a Tril method handler
         * @param methodNode is the AST node representing the Tril method
         */
        MethodHandler(ASTNode* methodNode);

        /**
         * @brief Performs IL generation of the contained AST and compiles the result
         * @return the return code for the compilation
         */
        int32_t compile();

        /**
         * @brief Returns the entry point to the compiled body (null if not yet compiled)
         *
         * The template parameter is the function pointer type that the entry
         * point should casted to. Passing a non-function pointer type as template
         * argument results in a compilation error.
         */
        template <typename T>
        T getEntryPoint() {
            static_assert( std::is_pointer<T>::value &&
                           std::is_function<typename std::remove_pointer<T>::type>::value,
                          "Attempted to get entry point using a non-function-pointer type.");
            return reinterpret_cast<T>(_entry_point);
        }

        /**
         * @brief Gets the TR::DataTypes value from the data type's name
         * @param name is the name of the data type as a string
         * @return the TR::DataTypes value corresponding to the specified name
         */
        static TR::DataTypes getTRDataTypes(const std::string& name);

    private:
        ASTNode* _methodNode;
        TR::TypeDictionary _types;
        TRLangBuilder _ilInjector;
        TR::IlType* _returnType;
        std::vector<TR::IlType*> _argTypes;
        uint8_t* _entry_point;
};

#endif // METHODHANDLER_HPP
