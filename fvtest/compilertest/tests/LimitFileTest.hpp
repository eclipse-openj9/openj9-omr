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

#ifndef TEST_LIMITFILE_INCL
#define TEST_LIMITFILE_INCL

#include <vector>
#include "gtest/gtest.h"

namespace TestCompiler {

class LimitFileTest : public ::testing::Test
   {
   public:
   LimitFileTest();
   ~LimitFileTest();

   void createVLog(const char *vlog, const char *limitFile = NULL);
   void createAndCheckVLog(const char *vlog, const char *limitFile = NULL, int *iNegLine = NULL);

   void checkVLogForMethod(const char *vlog, const char *method, bool compiled, int *foundOnLine = NULL);

   private:
   std::vector<const char *> _vlog;

   bool fileExists(const char *name);
   void generateVLog(const char *vlog, const char *limitFile = NULL);

   void compileTests();
   };

}

#endif // !defined(TEST_LIMITFILE_INCL)
