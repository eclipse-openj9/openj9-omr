/*******************************************************************************
 *
 * (c) Copyright IBM Corp. 2016
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
#include "testHeader.h"
#include "map_to_type/map_to_type.hpp"

/*
 * @ddr_namespace: default
 */

/* These should be included: */
#define dolphin			11
#define fish ((unsigned int)(4))/* Test comment block. */
#define whale (7) // Test comment.
#define octopus dolphin

/* These should be not included: */
#define test_function_macro(x) x
#define test_function_macro2 (x) x

