/*******************************************************************************
 *
 * (c) Copyright IBM Corp. 2016, 2016
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
 *******************************************************************************/

#ifndef TEST_OPTIONSET_INCL
#define TEST_OPTIONSET_INCL

#include "LimitFileTest.hpp"

#include <map>
#include <string>

namespace TestCompiler {

typedef std::map<const std::string, int> MethodSets;

class OptionSetTest : public LimitFileTest
   {
   public:
   void applyOptionSets(const char *limitFile, const char *newFile, const TestCompiler::MethodSets &methods);

   private:
   std::string getMethodFromLine(const std::string &line);
   };

}

#endif // !defined(TEST_OPTIONSET_INCL)
