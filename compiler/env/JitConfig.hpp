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

#ifndef JITCONFIG_HPP_BThFwv
#define JITCONFIG_HPP_BThFwv

#include <stddef.h>
#include <stdint.h>
#include "env/IO.hpp"

namespace TR
{

class JitConfig
   {
   protected:
   JitConfig();

   public:

   static JitConfig *instance();

   // possibly temporary place for options to be stored?
   struct
      {
      int32_t       codeCacheKB;
      char        * vLogFileName;
      TR::FILE    * vLogFile;
      uint64_t      verboseFlags;
      } options;

   void *getProcessorInfo() { return _processorInfo; }
   void setProcessorInfo(void *buf) { _processorInfo = buf; }

   bool isCodeCacheFull() { return _codeCacheFull; }

   void setInterpreterTOC(size_t interpreterTOC) { _interpreterTOC = interpreterTOC; }
   size_t getInterpreterTOC()                    { return _interpreterTOC; }

   void *getPseudoTOC()               { return _pseudoTOC; }
   void setPseudoTOC(void *pseudoTOC) { _pseudoTOC = pseudoTOC; }

   private:
   char                        _eyecatcher[8];

   void                      * _processorInfo;

   bool                        _codeCacheFull;

   size_t                      _interpreterTOC;

   void                      * _pseudoTOC; // only used on POWER, otherwise should be NULL
   };

} /* namespace TR */

#endif /* JITCONFIG_HPP_BThFwv */
