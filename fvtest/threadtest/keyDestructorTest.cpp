/*******************************************************************************
 *
 * (c) Copyright IBM Corp. 1991, 2016
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

#include "omrTest.h"
#include "testHelper.hpp"

#if !defined(WIN32) && !defined(J9ZOS390)
#include <pthread.h>
#include <stdlib.h>
#endif /* !defined(WIN32) && !defined(J9ZOS390) */

extern ThreadTestEnvironment *omrTestEnv;

class KeyDestructorTest: public ::testing::Test {
public:
	static bool completed;
#if !defined(WIN32) && !defined(J9ZOS390)
	static pthread_key_t key;

	static void detachThread(void *p);
	static void *threadproc(void *p);
#endif /* !defined(WIN32) && !defined(J9ZOS390) */
protected:

	static void
	SetUpTestCase(void)
	{
	}
};

bool KeyDestructorTest::completed = false;

#if !defined(WIN32) && !defined(J9ZOS390)
pthread_key_t KeyDestructorTest::key;

void
KeyDestructorTest::detachThread(void *p)
{
	omrthread_t old_self = (omrthread_t)p;
	omrthread_t self = omrthread_self();

	/* omrthread_self relies on a tls key. Verify that it returns the right value. */
	if (self == old_self) {
		/* If they are equal detach the thread and mark the test as successful */
		omrthread_detach(self);
		KeyDestructorTest::completed = true;
	} else {
		/* Can not use DbgMsg as it relies on omrthread_self() */
		printf("Error old_self %p != omrthread_self() %p\n", old_self, self);
		fflush(stdout);
	}
}

void *
KeyDestructorTest::threadproc(void *p)
{
	omrthread_t self = NULL;
	intptr_t attachRC = 0;
	int rc = 0;

	attachRC = omrthread_attach_ex(&self, J9THREAD_ATTR_DEFAULT);
	if (0 == attachRC) {
		/* set the key value to the omrthread for this thread */
		rc = pthread_setspecific(KeyDestructorTest::key, (void *)self);
		if (rc != 0) {
			DbgMsg::println("Error setting key to self %d", rc);
		}
	} else {
		DbgMsg::println("Error attaching to omrthread library %d", attachRC);
	}

	/* pthread_exit to test the key destructor */
	pthread_exit(NULL);

	/* will never get here.  Just to stop compiler warning */
	return NULL;
}
#endif /* !defined(WIN32) && !defined(J9ZOS390) */

TEST_F(KeyDestructorTest, KeyDestructor)
{
#if defined(WIN32) || defined(J9ZOS390)
	completed = true;
#else
	pthread_t thread;
	int rc = 0;

	rc = pthread_key_create(&key, detachThread);
	ASSERT_EQ(0, rc);

	rc = pthread_create(&thread, NULL, threadproc, NULL);
	ASSERT_EQ(0, rc);

	pthread_join(thread, NULL);

	rc = pthread_key_delete(key);
	ASSERT_EQ(0, rc);
#endif /* !defined(WIN32) && !defined(J9ZOS390) */
	ASSERT_TRUE(completed);
}
