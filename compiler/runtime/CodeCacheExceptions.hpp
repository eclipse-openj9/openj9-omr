/*******************************************************************************
 *
 * (c) Copyright IBM Corp. 2016, 2016
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

#ifndef CODECACHEEXCEPTIONS_HPP
#define CODECACHEEXCEPTIONS_HPP

#pragma once

#include <exception>
#include <new>

namespace TR {

// For:
//    COMPILATION_NULL_SUBSTITUTE_CODE_CACHE
//    COMPILATION_CODE_MEMORY_EXHAUSTED
struct CodeCacheError : public virtual std::bad_alloc
   {
   virtual const char* what() const throw() { return "Code Cache Error"; }
   };

// For:
//    COMPILATION_ALL_CODE_CACHES_RESERVED
//    COMPILATION_ILLEGAL_CODE_CACHE_SWITCH
struct RecoverableCodeCacheError : public virtual std::bad_alloc
   {
   virtual const char* what() const throw() { return "Recoverable Code Cache Error"; }
   };

}

#endif // CODECACHEEXCEPTIONS_HPP
