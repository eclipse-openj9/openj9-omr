/*******************************************************************************
 *
 * (c) Copyright IBM Corp. 1991, 2017
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


#include "threadTestLib.hpp"
#include "sanityTestHelper.hpp"

#include "omrTest.h"
#include "testHelper.hpp"

extern ThreadTestEnvironment *omrTestEnv;

class SanityTest: public ::testing::Test
{
public:
	static unsigned int runTime;
protected:

	static void
	SetUpTestCase(void)
	{
		/* parse the command line options */
		for (int32_t i = 1; i < omrTestEnv->_argc; i++) {
			if (0 == strncmp(omrTestEnv->_argv[i], "-runTime=", strlen("-runTime="))) {
				sscanf(&omrTestEnv->_argv[i][strlen("-runTime=")], "%u", &runTime);
			}
		}
	}
};
unsigned int SanityTest::runTime = 10; /* default test run time of 10 seconds */

TEST_F(SanityTest, simpleSanity)
{
	SimpleSanity();
	ASSERT_TRUE(SimpleSanity());
}

TEST_F(SanityTest, SanityTestSingleThread)
{
	SanityTestNThreads(1, runTime);
}

TEST_F(SanityTest, SanityTestTwoThreads)
{
	SanityTestNThreads(2, runTime);
}

TEST_F(SanityTest, SanityTestFourThreads)
{
	SanityTestNThreads(4, runTime);
}

TEST_F(SanityTest, QuickNDirtyPerformanceTest)
{
	QuickNDirtyPerformanceTest(runTime);
}

TEST_F(SanityTest, TestWaitNotify)
{
	TestWaitNotify(runTime);
}

TEST_F(SanityTest, TestBlockingQueue)
{
	omrthread_t self;
	omrthread_attach_ex(&self, J9THREAD_ATTR_DEFAULT);

	CThread cself(self);
	EXPECT_TRUE(TestBlockingQueue(cself, 1)) << "1 thread failed";
	EXPECT_TRUE(TestBlockingQueue(cself, 2)) << "2 threads failed";
	EXPECT_TRUE(TestBlockingQueue(cself, 3)) << "3 threads failed";
}
