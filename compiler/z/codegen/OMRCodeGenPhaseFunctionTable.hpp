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

/*
 * This file will be included within a table of function pointers.
 * Only valid static methods within CodeGenPhase class should be included.
 */


// The entries in this file must be kept in sync with compiler/z/codegen/OMRCodeGenPhaseEnum.hpp
#include "compiler/codegen/OMRCodeGenPhaseFunctionTable.hpp"

TR::CodeGenPhase::performMarkLoadAsZeroOrSignExtensionPhase,                              //markLoadAsZeroOrSignExtension
TR::CodeGenPhase::performSetBranchOnCountFlagPhase,

