/*******************************************************************************
 *
 * (c) Copyright IBM Corp. 2015, 2016
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

#if defined(AIXPPC) || defined(J9ZOS390)
#define __IBMCPP_TR1__ 1
#endif /* defined(AIXPPC) || defined(J9ZOS390) */

#include <stdio.h>

#include "ddr/blobgen/genBlob.hpp"
#include "ddr/blobgen/java/genBinaryBlob.hpp"
#include "ddr/blobgen/java/genSuperset.hpp"

DDR_RC
genBlob(struct OMRPortLibrary *portLibrary, Symbol_IR *const ir, const char *supersetFile, const char *blobFile)
{
	JavaSupersetGenerator supersetGenerator;
	DDR_RC rc = supersetGenerator.printSuperset(portLibrary, ir, supersetFile);

	if (DDR_RC_OK == rc) {
		printf("Superset written to file: %s\n", supersetFile);

		JavaBlobGenerator blobGenerator;
		rc = blobGenerator.genBinaryBlob(portLibrary, ir, blobFile);
	}

	if (DDR_RC_OK == rc) {
		printf("Blob written to file: %s\n", blobFile);
	}

	return rc;
}
