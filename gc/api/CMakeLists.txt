###############################################################################
# Copyright (c) 2017, 2020 IBM Corp. and others
#
# This program and the accompanying materials are made available under
# the terms of the Eclipse Public License 2.0 which accompanies this
# distribution and is available at https://www.eclipse.org/legal/epl-2.0/
# or the Apache License, Version 2.0 which accompanies this distribution and
# is available at https://www.apache.org/licenses/LICENSE-2.0.
#
# This Source Code may also be made available under the following
# Secondary Licenses when the conditions for such availability set
# forth in the Eclipse Public License, v. 2.0 are satisfied: GNU
# General Public License, version 2 with the GNU Classpath
# Exception [1] and GNU General Public License, version 2 with the
# OpenJDK Assembly Exception [2].
#
# [1] https://www.gnu.org/software/classpath/license.html
# [2] http://openjdk.java.net/legal/assembly-exception.html
#
# SPDX-License-Identifier: EPL-2.0 OR Apache-2.0 OR GPL-2.0 WITH Classpath-exception-2.0 OR LicenseRef-GPL-2.0 WITH Assembly-exception
###############################################################################

include(OmrAssert)

omr_assert(TEST OMR_GC_API)

###
### OMR GC
###

set(gc_include_directories
	glue/
	include/
)

set(gc_compile_features
	cxx_constexpr
	cxx_deleted_functions
	cxx_delegating_constructors
	cxx_noexcept
	cxx_nullptr
	cxx_override
	cxx_right_angle_brackets
	cxx_variadic_templates
	cxx_template_template_parameters
)

target_include_directories(omrgc
	PUBLIC
		${gc_include_directories}
)
target_compile_features(omrgc
	PUBLIC
		${gc_compile_features}
)

if(OMR_MIXED_REFERENCES_MODE_STATIC)
	target_include_directories(omrgc_full
		PUBLIC
			${gc_include_directories}
	)
	target_compile_features(omrgc_full
		PUBLIC
			${gc_compile_features}
	)
endif()
