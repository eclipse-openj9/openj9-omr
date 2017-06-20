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

#ifndef METHOD_COMPILER_HPP
#define METHOD_COMPILER_HPP

#include "method_info.hpp"
#include "ilgen.hpp"

#include "ilgen/TypeDictionary.hpp"

class MethodCompiler {
    public:
        explicit MethodCompiler(const ASTNode* methodNode)
            : _method{methodNode},
              _entry_point{nullptr}
        {}

        virtual ~MethodCompiler() = default;

        virtual int32_t compile() = 0;

        template <typename T>
        T getEntryPoint() {
            static_assert( std::is_pointer<T>::value &&
                           std::is_function<typename std::remove_pointer<T>::type>::value,
                          "Attempted to get entry point using a non-function-pointer type.");
            return reinterpret_cast<T>(_entry_point);
        }

    protected:
        void setEntryPoint(void* entry_point) { _entry_point = entry_point; }

        MethodInfo* getMethodInfo() { return &_method; }

    private:
        MethodInfo _method;
        void* _entry_point;
};

#endif // METHOD_COMPILER_HPP
