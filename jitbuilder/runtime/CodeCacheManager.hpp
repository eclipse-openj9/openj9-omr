/*******************************************************************************
 *
 * (c) Copyright IBM Corp. 2014, 2016
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

#ifndef TR_CODECACHEMANAGER_INCL
#define TR_CODECACHEMANAGER_INCL

#include "runtime/JBCodeCacheManager.hpp"

/*
 * These #ifndef's and classes must appear before including the OMR version of this file
 */
namespace TR
{

   class CodeCacheManager : public JitBuilder::CodeCacheManager
      {
      public:
      CodeCacheManager(TR_FrontEnd *fe) : JitBuilder::CodeCacheManager(fe) { }
      };

} // namespace TR

#endif // defined(TR_CODECACHEMANAGER_INCL)
