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

#include "env/JitConfig.hpp"
#include <string.h> // for memcpy
#include "env/ConcreteFE.hpp"

TR::JitConfig::JitConfig()
   : _processorInfo(0), _codeCacheFull(false), _interpreterTOC(0), _pseudoTOC(0)
   {
   memcpy(_eyecatcher, "JITCONF" /* 7 bytes + null */, sizeof(this->_eyecatcher));
   }

TR::JitConfig *
TR::JitConfig::instance()
   {
   return OMR::FrontEnd::singleton().jitConfig();
   }
