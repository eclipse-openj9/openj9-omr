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
// A "Linkage Convention" is the abstraction of one group of similar calling conventions;
// actual interpretation is up to the Front End / Code Generator combination.
// For example, by default, TR_System is interpreted as:
//     X86-32 Windows and Linux: cdecl calling convention
//     X86-64 Windows:           Microsoft x64 calling convention
//     X86-64 Linux:             System V AMD64 ABI
//

enum TR_LinkageConventions
   {
   #include "codegen/LinkageConventions.enum"
   TR_NumLinkages,

   // Force size to be at least 32-bit, as it may be cast to/from integers
   TR_LinkageForceSize = 0x7fffffff
   };

#endif
