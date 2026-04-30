/*******************************************************************************
 * Copyright IBM Corp. and others 1991
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
 * [2] https://openjdk.org/legal/assembly-exception.html
 *
 * SPDX-License-Identifier: EPL-2.0 OR Apache-2.0 OR GPL-2.0-only WITH Classpath-exception-2.0 OR GPL-2.0-only WITH OpenJDK-assembly-exception-1.0
 *******************************************************************************/

#ifndef OMRSYSINFO_HELPERS_H_
#define OMRSYSINFO_HELPERS_H_

#include "omrport.h"

#define J9BYTES_PER_PAGE            4096		/* Size of main storage frame/virtual storage page/auxiliary storage slot */

#if defined(__cplusplus)
extern "C" {
#endif /* __cplusplus */

/**
* End of portion extracted from the header "//'SYS1.SIEAHDR.H(IWMQVSH)'".
*/

/**
 * Retrieve z/Architecture facility bits.
 *
 * @param [in]  lastDoubleWord   Size of the bits array in number of uint64_t, minus 1
 * @param [out] bits             Caller-supplied uint64_t array that gets populated with the facility bits
 *
 * @return The index of the last valid uint64_t in the bits array
 */
extern int getstfle(int lastDoubleWord, uint64_t *bits);

/**
 * Function retrieves and populates memory usage statistics on a z/OS platform.
 * @param [in] portLibrary The Port Library Handle
 * @param[out] memInfo     Pointer to J9MemoryInfo struct which we populate with memory usage
 * @return                 0 on success; negative value on failure
 */
int32_t
retrieveZOSMemoryStats(struct OMRPortLibrary *portLibrary, struct J9MemoryInfo *memInfo);

/**
 * Function retrieves and populates processor usage statistics on a z/OS platform.
 *
 * @param [in] portLibrary The Port Library Handle
 * @param[out] procInfo    Pointer to J9ProcessorInfos struct that we populate with processor usage
 * @return                 0 on success; negative value on failure
 */
int32_t
retrieveZOSProcessorStats(struct OMRPortLibrary *portLibrary, struct J9ProcessorInfos *procInfo);

/**
 * Return the CPU load as reported by z/OS. If zIIPs are available, the zIIP
 * load is returned; otherwise, the general CP (GCP) load is returned.
 *
 * @param [in] portLibrary The Port Library Handle
 * @return CPU load in the [0.0, 1.0] interval
 */
double
retrieveZosCpuLoad(struct OMRPortLibrary *portLibrary);

/**
 * Calculate the combined CPU load across general CPs and zIIPs.
 *
 * @param [in] portLibrary The Port Library Handle
 * @return combined CPU load in the [0.0, 1.0] interval
 */
double
calculateCombinedZosCpuLoad(struct OMRPortLibrary *portLibrary);

/**
 * Indicate whether IIPHONORPRIORITY is enabled. When enabled,
 * zIIP-eligible work may be dispatched to general CPs when zIIPs are fully
 * utilized.
 *
 * @return TRUE if flag is set and FALSE otherwise
 */
BOOLEAN
isIipHonorPrioritySet(struct OMRPortLibrary *portLibrary);

/**
 * Indicate whether simultaneous multithreading (SMT) is enabled. When enabled,
 * each zIIP engine provides two logical processors.
 *
 * @return TRUE if multithreading is enabled and FALSE otherwise
 */
BOOLEAN
isSMTEnabled(void);

/**
 * Return the available zIIP capacity. For SMT-1, this matches the online zIIP
 * count. For SMT-2, the capacity is adjusted using SMT_2_GAIN_FACTOR to
 * estimate effective capacity.
 *
 * @return available zIIP capacity adjusted for SMT-2 gain
 */
double
retrieveZosZiipCapacity(void);

/**
 * Return the number of online zIIP CPUs as reported by z/OS.
 *
 * @return number of online zIIP CPUs
 */
int32_t
retrieveZosOnlineZiipCount(void);

/**
 * Return the number of online general CPs as reported by z/OS.
 *
 * @return number of online general CPs
 */
int32_t
retrieveZosOnlineGcpCount(void);

/**
 * Return the total number of online CPUs as reported by z/OS. This includes
 * general CPs, zCBP/zAAPs, and zIIPs.
 *
 * @return number of CPUs online
 */
int32_t
retrieveZosOnlineCpuCount(void);

/**
 * Return an adjusted CPU capacity estimate for z/OS. If zIIPs are unavailable,
 * the result reflects the online general CP count. When zIIPs are available,
 * the estimate combines adjusted zIIP capacity with a scaled contribution from
 * general CPs to reflect typical workload execution behavior.
 *
 * @param [in] portLibrary The Port Library Handle
 * @return total available CPUs adjusted for zIIPs and SMT-2
 */
double
retrieveZosCpuCapacity(struct OMRPortLibrary *portLibrary);

/**
 * Return the SVT zIIP normalization factor. This factor is used to convert
 * general CP equivalent time to zIIP equivalent time.
 *
 * @return SVT zIIP normalization factor
 */
double
retrieveSvtZiipNormalizationFactor(void);

/**
 * Calculate CPU utilization for the current process. The calculation follows the
 * same approach as omrsysinfo_get_process_utilization().
 *
 * @param [in] portLibrary The Port Library Handle
 * @param [out] usageStats pointer to CpuUsageStats struct to be populated
 *                         with CPU usage stats
 * @return 0 on success; negative value on failure
 */
int32_t
calculateZosProcessCpuUtilization(struct OMRPortLibrary *portLibrary, struct CpuUsageStats *usageStats);

/**
 * Retrieve aggregate CPU usage statistics. This includes:
 * - timestamps and CPU times used for utilization calculations
 * - average zIIP utilization
 * - average combined CP utilization
 * - SVT normalization factor
 * - online CPU counts and capacities
 * - IIPHONORPRIORITY and SMT status
 * - other relevant CPU usage metrics
 *
 * @param [in] portLibrary The Port Library Handle
 * @param [out] usageStats pointer to CpuUsageStats struct to be populated
 *                         with CPU usage stats
 * @return 0 on success; negative value on failure
 */
int32_t
retrieveZosCpuUsageStats(struct OMRPortLibrary *portLibrary, struct CpuUsageStats *usageStats);

#if defined(__cplusplus)
} /* extern "C" */
#endif

#endif /* OMRSYSINFO_HELPERS_H_ */
