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

#ifndef CONFIG_HPP
#define CONFIG_HPP

#include "omrcfg.h"

/* Enable standard limit macros on z/OS. */

#if defined(J9ZOS390)
/* We need to define these for the limit macros to get defined in z/OS */
#define _ISOC99_SOURCE 1
#define __STDC_LIMIT_MACROS 1
#endif /* defined(J9ZOS390) */

/* C++ TR1 support */

#if defined(AIXPPC) || defined(J9ZOS390)
#define __IBMCPP_TR1__ 1
#define OMR_HAVE_TR1 1
#endif /* !defined(AIXPPC) && !defined(J9ZOS390) */

/* Why is this disabled? */

#if defined(NDEBUG)
#undef NDEBUG
#endif

#endif /* CONFIG_HPP */
