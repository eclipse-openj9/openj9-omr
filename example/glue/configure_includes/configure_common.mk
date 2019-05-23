###############################################################################
# Copyright (c) 2015, 2019 IBM Corp. and others
#
# This program and the accompanying materials are made available under
# the terms of the Eclipse Public License 2.0 which accompanies this
# distribution and is available at http://eclipse.org/legal/epl-2.0
# or the Apache License, Version 2.0 which accompanies this distribution
# and is available at https://www.apache.org/licenses/LICENSE-2.0.
#
# This Source Code may also be made available under the following Secondary
# Licenses when the conditions for such availability set forth in the
# Eclipse Public License, v. 2.0 are satisfied: GNU General Public License,
# version 2 with the GNU Classpath Exception [1] and GNU General Public
# License, version 2 with the OpenJDK Assembly Exception [2].
#
# [1] https://www.gnu.org/software/classpath/license.html
# [2] http://openjdk.java.net/legal/assembly-exception.html
#
# SPDX-License-Identifier: EPL-2.0 OR Apache-2.0 OR GPL-2.0 WITH Classpath-exception-2.0 OR LicenseRef-GPL-2.0 WITH Assembly-exception
###############################################################################

CONFIGURE_ARGS += \
  --enable-OMR_EXAMPLE \
  --enable-OMR_GC \
  --enable-OMR_PORT \
  --enable-OMR_THREAD \
  --enable-OMR_OMRSIG \
  --enable-fvtest \
  --enable-OMR_GC_SEGREGATED_HEAP \
  --enable-OMR_GC_MODRON_SCAVENGER \
  --enable-OMR_GC_MODRON_CONCURRENT_MARK \
  --enable-OMR_GC_VLHGC \
  --enable-OMR_GC_VLHGC_CONCURRENT_COPY_FORWARD \
  --enable-OMR_THR_CUSTOM_SPIN_OPTIONS \
  --enable-OMR_NOTIFY_POLICY_CONTROL

OMRGLUE_INCLUDES=example/
