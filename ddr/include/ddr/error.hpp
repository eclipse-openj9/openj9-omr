/*******************************************************************************
 *
 * (c) Copyright IBM Corp. 2015, 2017
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

 #if !defined(DDR_ERROR_HPP)
 #define DDR_ERROR_HPP

#include <ddr/config.hpp>
#include <stdio.h>

#define ERRMSG(...) { \
	fprintf(stderr, "Error: %s:%d %s - ", __FILE__, __LINE__, __FUNCTION__);\
	fprintf(stderr, __VA_ARGS__);\
	fprintf(stderr, "\n");\
}

#if 0
/* Enable debug printf output */
#define DEBUGPRINTF_ON
#endif

#if defined(DEBUGPRINTF_ON)
#define DEBUGPRINTF(...) { \
	printf("DEBUG: %s[%d]: %s: ", __FILE__, __LINE__, __FUNCTION__);\
	printf(__VA_ARGS__);\
	printf("\n");\
	fflush(stdout);\
}
#else
#define DEBUGPRINTF(...)
#endif /* defined(DEBUG) */

enum DDR_RC {
	DDR_RC_OK,
	DDR_RC_ERROR
};

#endif /* DDR_ERROR_HPP */