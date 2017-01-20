/*******************************************************************************
 *
 * (c) Copyright IBM Corp. 2007, 2017
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

#include <errno.h>
#include "createTestHelper.h"

extern OMRPortLibrary *portLib;

void
port_printf(const char *str, ...)
{
	va_list vargs;
	OMRPORT_ACCESS_FROM_OMRPORT(portLib);
	va_start(vargs, str);
	omrtty_vprintf(str, vargs);
	va_end(vargs);
}
