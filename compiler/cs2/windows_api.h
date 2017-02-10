/*******************************************************************************
 *
 * (c) Copyright IBM Corp. 2000, 2016
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

#ifdef WINDOWS

#ifdef BOOLEAN
/* There is a collision between J9's definition of BOOLEAN and Windows headers */
#define BOOLEAN_COLLISION_DETECTED BOOLEAN
#undef BOOLEAN
#endif

#ifdef boolean
/* There is a collision between J9's definition of boolean and Windows headers */
#define boolean_COLLISION_DETECTED boolean
#undef boolean
#endif

#define WINDOWS_API_INCLUDED 1
#define NOMINMAX 1
#include <Windows.h>

#ifdef boolean_COLLISION_DETECTED
#define boolean bool
#undef boolean_COLLISION_DETECTED
#endif

#ifdef BOOLEAN_COLLISION_DETECTED
#define BOOLEAN UDATA
#undef BOOLEAN_COLLISION_DETECTED
#endif

#endif
