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

#ifndef TEST_JITCONFIG_HPP
#define TEST_JITCONFIG_HPP

#include "env/TRMemory.hpp"
#include "env/FEBase.hpp"
#include "env/JitConfig.hpp"

namespace TestCompiler { class FrontEnd; }

// Singleton JitConfig. The only instance of this is TestCompiler::FrontEnd::_jitConfig
namespace TestCompiler
{
struct JitConfig : public TR::JitConfig
   {
   private:
   friend class TR::FEBase<FrontEnd>;
   JitConfig() : TR::JitConfig() {}
   };
} // namespace TestCompiler

#endif
