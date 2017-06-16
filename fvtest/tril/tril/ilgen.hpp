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

#ifndef ILGEN_HPP
#define ILGEN_HPP

#include "ilgen/TypeDictionary.hpp"
#include "ilgen/IlInjector.hpp"

#include "ast.h"

#include <map>
#include <string>

class TRLangBuilder : public TR::IlInjector {
    public:
        TRLangBuilder(ASTNode* trees, TR::TypeDictionary* d)
              : TR::IlInjector(d), _trees(trees) {}

        bool injectIL() override;

        TR::Node* toTRNode(const ASTNode* const tree);

        void cfgFor(const ASTNode* const tree);

    private:
        ASTNode* _trees;
        std::map<std::string, TR::SymbolReference*> _symRefMap;
        std::map<std::string, int> _blockMap;
};

#endif // ILGEN_HPP
