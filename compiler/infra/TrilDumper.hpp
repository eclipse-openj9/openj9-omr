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

#ifndef TR_TRILDUMPER_HPP
#define TR_TRILDUMPER_HPP

#include "infra/ILTraverser.hpp"

namespace TR {

/**
 * @brief A class for dumping Testarossa IL as Tril
 *
 * This class is implemented as an observer of TR::ILTraverser. To print Tril,
 * an instance of this class must first be registered as an observer of an
 * instance of TR::ILTraverser. The Tril code will be printed when one of the
 * `traverse()` methods is called on the ILTraverser.
 *
 * Example usage:
 *      // dump Tril code for a method
 *      TR::TrilDumper dumper{stderr};
 *      TR::ILTraverser traverser(comp);
 *      traverser.registerObserver(&dumper);
 *      traverser.traverse(comp()->getJittedMethodSymbol());
 *      fprintf(stderr, "\n");
 *
 * Using this pattern allows the printing logic to be isolated from the IL
 * traversal logic.
 *
 * The output Tril code will all appear on a single line, with exactly one ' '
 * (space) separating each element (node, node argument, etc.).
 */
class TrilDumper : public TR::ILTraverser::Observer
   {
   public:

    /**
    * @brief Constructs an instance of TR::TrilDumper.
    * @param file is the file object to which the Tril code will be printed
    */
   explicit TrilDumper(FILE* file);

   void visitingMethod(TR::ResolvedMethodSymbol* method) override;
   void returnedToMethod(TR::ResolvedMethodSymbol* method) override;
   void visitingNode(TR::Node* node) override;
   void visitingCommonedChildNode(TR::Node* node) override;
   void visitedAllChildrenOfNode(TR::Node* node) override;

   private:
   FILE* _outputFile;
   };

} // namespace TR

#endif // TR_TRILDUMPER_HPP
