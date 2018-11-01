/*******************************************************************************
 * Copyright (c) 2017, 2017 IBM Corp. and others
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

#include "omrTest.h"
#include "JitTest.hpp"

extern "C" {
int omr_main_entry(int argc, char **argv, char **envp);
}

extern bool internal_initializeJit();
extern bool internal_initializeJitWithOptions(char *options);
extern int32_t internal_compileMethodBuilder(TR::MethodBuilder * methodBuilder, void ** entryPoint);
extern void internal_shutdownJit();

bool initializeJit() {
   auto ret = internal_initializeJit();
   return ret;
}

bool initializeJitWithOptions(char * options) {
auto ret = internal_initializeJitWithOptions(options);
return ret;
}

int32_t compileMethodBuilder(TR::MethodBuilder * methodBuilder, void ** entryPoint) {
   auto ret = internal_compileMethodBuilder(methodBuilder, entryPoint);
   return ret;
}

void shutdownJit() {
   internal_shutdownJit();
}

int omr_main_entry(int argc, char **argv, char **envp) {
   ::testing::InitGoogleTest(&argc, argv);
   OMREventListener::setDefaultTestListener();
   return RUN_ALL_TESTS();
}
