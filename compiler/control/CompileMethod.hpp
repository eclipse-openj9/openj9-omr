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

#include <stdint.h>                      // for int32_t, uint8_t
#include "compile/CompilationTypes.hpp"  // for TR_Hotness
#include "env/ConcreteFE.hpp"            // for FrontEnd

struct OMR_VMThread;
class TR_ResolvedMethod;
namespace TR { class IlGeneratorMethodDetails; }
namespace TR { class JitConfig; }

int32_t init_options(TR::JitConfig *jitConfig, char * cmdLineOptions);
int32_t commonJitInit(OMR::FrontEnd &fe, char * cmdLineOptions);
uint8_t *compileMethod(OMR_VMThread *omrVMThread, TR_ResolvedMethod &compilee, TR_Hotness hotness, int32_t &rc);
uint8_t *compileMethodFromDetails(OMR_VMThread *omrVMThread, TR::IlGeneratorMethodDetails &details, TR_Hotness hotness, int32_t &rc);
