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

#ifndef TEST_LOGFILE_INCL
#define TEST_LOGFILE_INCL

#include <vector>
#include <map>
#include "gtest/gtest.h"

namespace TestCompiler {

class LogFileTest : public ::testing::Test
   {
   public:
   LogFileTest();
   ~LogFileTest();

   void forkAndCompile(std::string logFile, const char *logType = "traceFull");
   void checkLogForKeywords(std::map<const char*, bool> keywords, const char *logFile);
   void runKeywordTests(std::map<const char*, std::map<const char*, bool>> logFileChecks);
   bool fileIsNotEmpty(std::string logFile);
   std::map<const char*, bool> buildKeywordMap(std::initializer_list <const char*> inputs);

   private:
   std::vector<std::string> _logFiles;

   bool fileExists(std::string name);
   void createLog(std::string logFile, const char *logType);
   void compileTests();
   };

}

#endif // !defined(TEST_LOGFILE_INCL)
