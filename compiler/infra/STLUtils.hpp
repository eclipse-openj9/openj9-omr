/*******************************************************************************
 *
 * (c) Copyright IBM Corp. 2017, 2017
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


/** 
 * Declares utility functions to be used with STL containers.
 */

#ifndef TR_STL_HPP
#define TR_STL_HPP

/**
 * @brief Compares two strings based on ASCII values of their characters.
 *        This function is intended to be used as a string comparator for
 *        STL containers.
 * @param s1 pointer to the first operand string
 * @param s2 pointer to the second operand string
 * @returns true if the string pointed to by s1 is 'less than' the one pointed to by s2
 */
bool str_comparator(const char *s1, const char *s2);

#endif
