/*******************************************************************************
 * Copyright (c) 2014, 2018 IBM Corp. and others
 *
 * This program and the accompanying materials are made available under
 * the terms of the Eclipse Public License 2.0 which accompanies this
 * distribution and is available at https://www.eclipse.org/legal/epl-2.0/
 * or the Apache License, Version 2.0 which accompanies this distribution and
 * is available at https://www.apache.org/licenses/LICENSE-2.0.
 *
 * This Source Code may also be made available under the following
 * Secondary Licenses when the conditions for such availability set
 * forth in the Eclipse Public License, v. 2.0 are satisfied: GNU
 * General Public License, version 2 with the GNU Classpath
 * Exception [1] and GNU General Public License, version 2 with the
 * OpenJDK Assembly Exception [2].
 *
 * [1] https://www.gnu.org/software/classpath/license.html
 * [2] http://openjdk.java.net/legal/assembly-exception.html
 *
 * SPDX-License-Identifier: EPL-2.0 OR Apache-2.0 OR GPL-2.0 WITH Classpath-exception-2.0 OR LicenseRef-GPL-2.0 WITH Assembly-exception
 *******************************************************************************/

#ifndef OMRMODRONCORE_H_
#define OMRMODRONCORE_H_

/*
 * @ddr_namespace: map_to_type=J9ModroncoreConstants
 */

#include "omrcfg.h"
#include "omrcomp.h"

/**
 * Card state definitions
 * @note CARD_DIRTY 0x01 has been hardcoded in builder see J9VMObjectAccessBarrier>>#dirtyCard
 * @ingroup GC_Include
 * @{
 */
typedef U_8 Card;

/* base2 log of CARD_SIZE, used to change division into shift */
#define CARD_SIZE_SHIFT 9

/* size of a "card" of the heap - in a sense:  the granule of deferred mark map update */
#define CARD_SIZE 512

/*
 * The following definitions are duplicated in j9modron.h.
 * Both versions of each definition must match exactly.
 */

/* means that this card requires no special handling in the next collection as a result of previous collection or mutator activity */
#define CARD_CLEAN 0x00
/* means that this card has been changed by the mutator since the collector last processed it */
#define CARD_DIRTY 0x01
/* means that a PGC has viewed the card but the next GMP increment (or SYS/OOM fall-back) must also process the card (since it could have hidden objects) */
#define CARD_GMP_MUST_SCAN 0x02
/* means that a GMP has viewed the card but the next PGC increment must also process the card (since it might have unseen inter-region references - the GMP has a smaller live set so it wouldn't see them nor does it update the RSLists) */
#define CARD_PGC_MUST_SCAN 0x03
/* largely equivalent to CARD_PGC_MUST_SCAN but can only exist between RSCL flushing and the end of PGC mark and has the special connotation that the only objects, in this card, which refer to other regions have their remembered bit set */
#define CARD_REMEMBERED 0x04
/* largely equivalent to CARD_DIRTY but can only exist between RSCL flushing and the end of PGC mark and has the special connotation that the only objects, in this card, which refer to other regions have their remembered bit set */
#define CARD_REMEMBERED_AND_GMP_SCAN 0x05
/* used to state that this card contains objects which refer to other regions but all of those who do have their remembered bit set and are not yet in the region's RSCL - the compact treats this much like PGC_MUST_SCAN in that it may promote it to GMP_MUST_SCAN if a GMP is active */
#define CARD_MARK_COMPACT_TRANSITION 0x06
/* used when a value must be passed which is considered uninitialized or otherwise invalid */
#define CARD_INVALID 0xFF

/**
 * @}
 */

/**
 * #defines representing the vmState that should be set during various GC activities
 * @note J9VMSTATE_GC is the "major mask" representing a "GC" activity - we OR in a minor mask
 * representing the specific activity.
 * @ingroup GC_Include
 * @{
 */
#define J9VMSTATE_GC 0x20000

#define OMRVMSTATE_GC_THIS_STATE_CAN_BE_REUSED_0001 (J9VMSTATE_GC | 0x0001)
#define OMRVMSTATE_GC_MARK (J9VMSTATE_GC | 0x0002)
#define OMRVMSTATE_GC_SWEEP (J9VMSTATE_GC | 0x0003)
#define OMRVMSTATE_GC_COMPACT (J9VMSTATE_GC | 0x0004)
#define OMRVMSTATE_GC_THIS_STATE_CAN_BE_REUSED_0005 (J9VMSTATE_GC | 0x0005)
#define OMRVMSTATE_GC_CONCURRENT_MARK_PREPARE_CARD_TABLE (J9VMSTATE_GC | 0x0006)
#define OMRVMSTATE_GC_COMPACT_FIX_HEAP_FOR_WALK (J9VMSTATE_GC | 0x0007)
#define OMRVMSTATE_GC_CONCURRENT_MARK_CLEAR_NEW_MARKBITS (J9VMSTATE_GC | 0x0008)
#define OMRVMSTATE_GC_CONCURRENT_MARK_SCAN_REMEMBERED_SET (J9VMSTATE_GC | 0x0009)
#define OMRVMSTATE_GC_CONCURRENT_MARK_FINAL_CLEAN_CARDS (J9VMSTATE_GC | 0x000A)
#define OMRVMSTATE_GC_CONCURRENT_SWEEP_COMPLETE_SWEEP (J9VMSTATE_GC | 0x000B)
#define OMRVMSTATE_GC_CONCURRENT_SWEEP_FIND_MINIMUM_SIZE_FREE (J9VMSTATE_GC | 0x000C)
#define OMRVMSTATE_GC_PARALLEL_OBJECT_DO (J9VMSTATE_GC | 0x000D)
#define OMRVMSTATE_GC_THIS_STATE_CAN_BE_REUSED_000E (J9VMSTATE_GC | 0x000E)
#define OMRVMSTATE_GC_SCAVENGE (J9VMSTATE_GC | 0x000F)
#define OMRVMSTATE_GC_SCRUB_CARD_TABLE (J9VMSTATE_GC | 0x0010)
#define OMRVMSTATE_GC_CLEANING_METADATA (J9VMSTATE_GC | 0x0011)
#define OMRVMSTATE_GC_COLLECTOR_CONCURRENTGC (J9VMSTATE_GC | 0x0012)
#define OMRVMSTATE_GC_COLLECTOR_CONCURRENTSWEEPGC (J9VMSTATE_GC | 0x0013)
#define OMRVMSTATE_GC_COLLECTOR_GLOBALGC (J9VMSTATE_GC | 0x0014)
#define OMRVMSTATE_GC_COLLECTOR_SCAVENGER (J9VMSTATE_GC | 0x0015)
#define OMRVMSTATE_GC_CONCURRENT_MARK_TRACE (J9VMSTATE_GC | 0x0016)
#define OMRVMSTATE_GC_CONCURRENT_SWEEP (J9VMSTATE_GC | 0x0017)
#define OMRVMSTATE_GC_COLLECTOR_METRONOME (J9VMSTATE_GC | 0x0018)
#define OMRVMSTATE_GC_ALLOCATE_OBJECT (J9VMSTATE_GC | 0x0019)
#define OMRVMSTATE_GC_ALLOCATE_INDEXABLE_OBJECT (J9VMSTATE_GC | 0x001A)
#define OMRVMSTATE_GC_THIS_STATE_CAN_BE_REUSED_001B (J9VMSTATE_GC | 0x001B)
#define OMRVMSTATE_GC_THIS_STATE_CAN_BE_REUSED_001C (J9VMSTATE_GC | 0x001C)
#define OMRVMSTATE_GC_THIS_STATE_CAN_BE_REUSED_001D (J9VMSTATE_GC | 0x001D)
#define OMRVMSTATE_GC_CONCURRENT_MARK_COMPLETE_TRACING (J9VMSTATE_GC | 0x001E)
#define OMRVMSTATE_GC_THIS_STATE_CAN_BE_REUSED_001F (J9VMSTATE_GC | 0x001F)
#define OMRVMSTATE_GC_CHECK_RESIZE (J9VMSTATE_GC | 0x0020)
#define OMRVMSTATE_GC_PERFORM_RESIZE (J9VMSTATE_GC | 0x0021)
#define OMRVMSTATE_GC_CHECK_BEFORE_GC (J9VMSTATE_GC | 0x0022)
#define OMRVMSTATE_GC_CHECK_AFTER_GC (J9VMSTATE_GC | 0x0023)
#define OMRVMSTATE_GC_TGC (J9VMSTATE_GC | 0x0024)
#define OMRVMSTATE_GC_DISPATCHER_IDLE (J9VMSTATE_GC | 0x0025)
#define OMRVMSTATE_GC_CONCURRENT_SCAVENGER (J9VMSTATE_GC | 0x0026)

#define OMRVMSTATE_GC_CARD_CLEANER_FOR_MARKING (J9VMSTATE_GC | 0x0101)
#define OMRVMSTATE_GC_COPY_FORWARD_GMP_CARD_CLEANER (J9VMSTATE_GC | 0x0102)
#define OMRVMSTATE_GC_COPY_FORWARD_NO_GMP_CARD_CLEANER (J9VMSTATE_GC | 0x0103)
#define OMRVMSTATE_GC_GLOBAL_COLLECTION_CARD_CLEANER (J9VMSTATE_GC | 0x0104)
#define OMRVMSTATE_GC_GLOBAL_COLLECTION_NO_SCAN_CARD_CLEANER (J9VMSTATE_GC | 0x0105)
#define OMRVMSTATE_GC_GLOBAL_MARK_CARD_CLEANER (J9VMSTATE_GC | 0x0106)
#define OMRVMSTATE_GC_GLOBAL_MARK_CARD_SCRUBBER (J9VMSTATE_GC | 0x0107)
#define OMRVMSTATE_GC_GLOBAL_MARK_NO_SCAN_CARD_CLEANER (J9VMSTATE_GC | 0x0108)
#define OMRVMSTATE_GC_PARTIAL_MARK_GMP_CARD_CLEANER (J9VMSTATE_GC | 0x0109)
#define OMRVMSTATE_GC_PARTIAL_MARK_NO_GMP_CARD_CLEANER (J9VMSTATE_GC | 0x010A)
#define OMRVMSTATE_GC_WRITE_ONCE_FIXUP_CARD_CLEANER (J9VMSTATE_GC | 0x010B)

/**
 * @}
 */

#endif /* OMRMODRONCORE_H_ */
