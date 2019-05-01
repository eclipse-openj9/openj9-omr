/*******************************************************************************
 * Copyright (c) 2019, 2019 IBM Corp. and others
 *
 * This program and the accompanying materials are made available under
 * the terms of the Eclipse Public License 2.0 which accompanies this
 * distribution and is available at http://eclipse.org/legal/epl-2.0
 * or the Apache License, Version 2.0 which accompanies this distribution
 * and is available at https://www.apache.org/licenses/LICENSE-2.0.
 *
 * This Source Code may also be made available under the following Secondary
 * Licenses when the conditions for such availability set forth in the
 * Eclipse Public License, v. 2.0 are satisfied: GNU General Public License,
 * version 2 with the GNU Classpath Exception [1] and GNU General Public
 * License, version 2 with the OpenJDK Assembly Exception [2].
 *
 * [1] https://www.gnu.org/software/classpath/license.html
 * [2] http://openjdk.java.net/legal/assembly-exception.html
 *
 * SPDX-License-Identifier: EPL-2.0 OR Apache-2.0 OR GPL-2.0 WITH Classpath-exception-2.0 OR LicenseRef-GPL-2.0 WITH Assembly-exception
 *******************************************************************************/

// On zOS XLC linker can't handle files with same name at link time.
// This workaround with pragma is needed. What this does is essentially
// give a different name to the codesection (csect) for this file. So it
// doesn't conflict with another file with the same name.
#pragma csect(CODE,"OMRZCPUBase#C")
#pragma csect(STATIC,"OMRZCPUBase#S")
#pragma csect(TEST,"OMRZCPUBase#T")

#include "compiler/z/env/OMRCPU.hpp"

bool
OMR::Z::CPU::getS390SupportsFPE()
   {
   return _flags.testAny(S390SupportsFPE);
   }

bool
OMR::Z::CPU::getS390SupportsTM()
   {
   return _flags.testAny(S390SupportsTM);
   }

bool
OMR::Z::CPU::getS390SupportsRI()
   {
   return _flags.testAny(S390SupportsRI);
   }

bool
OMR::Z::CPU::getS390SupportsVectorFacility()
   {
   return _flags.testAny(S390SupportsVectorFacility);
   }

bool
OMR::Z::CPU::getS390SupportsVectorPackedDecimalFacility()
   {
   return _flags.testAny(S390SupportsVectorPackedDecimalFacility);
   }

bool
OMR::Z::CPU::getS390SupportsGuardedStorageFacility()
   {
   return (_flags.testAny(S390SupportsGuardedStorageFacility) &&
           _flags.testAny(S390SupportsSideEffectAccessFacility));
   }

bool
OMR::Z::CPU::getS390SupportsMIE3()
   {
   return _flags.testAny(S390SupportsMIE3);
   }

bool
OMR::Z::CPU::getS390SupportsVectorFacilityEnhancement2()
   {
   return _flags.testAny(S390SupportsVectorFacilityEnhancement2);
   }

bool
OMR::Z::CPU::getS390SupportsVectorPDEnhancement()
   {
   return _flags.testAny(S390SupportsVectorPDEnhancementFacility);
   }