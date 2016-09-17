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

#include <stddef.h>                   // for NULL
#include <stdint.h>                   // for int32_t, int64_t, uint32_t
#include <string.h>
#include "env/CompilerEnv.hpp"
#include "env/jittypes.h"             // for uintptrj_t, intptrj_t
#include "infra/Assert.hpp"           // for TR_ASSERT


char *
OMR::ClassEnv::classNameChars(TR::Compilation *comp, TR::SymbolReference *symRef, int32_t & len)
   {
   char *name = "<no class name>";
   len = strlen(name);
   return name;
   }
