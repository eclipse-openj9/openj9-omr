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
 *******************************************************************************/

#ifndef TR_LINKAGECONVENTION_INCL
#define TR_LINKAGECONVENTION_INCL

// Linkage conventions
//
enum TR_LinkageConventions
   {
   TR_Private           = 0,
   TR_System            = 1,
   TR_AllRegister       = 2,
   TR_InterpretedStatic = 3,
   TR_Helper            = 4,
   TR_J9JNILinkage      = 5,
   TR_NumLinkages       = 6
   };

#endif
