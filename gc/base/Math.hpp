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

#if !defined(MATH_HPP_)
#define MATH_HPP_

#include "omrcfg.h"
#include "omrcomp.h"
#include "modronbase.h"
#include "omrmodroncore.h"

#include "Bits.hpp"

class MM_Math
{
public:
	static uintptr_t saturatingSubtract(uintptr_t minuend, uintptr_t subtrahend);
	
	static float weightedAverage(float currentAverage, float newValue, float weight);

	static double weightedAverage(double currentAverage, double newValue, double weight);

	/* Round value up */
	static MMINLINE uintptr_t roundToCeiling(uintptr_t granularity, uintptr_t number) {
		return number + ((number % granularity) ? (granularity - (number % granularity)) : 0);
	}
	
	/* Round value up */
	static MMINLINE uint64_t roundToCeilingU64(uint64_t granularity, uint64_t number) {
		return number + ((number % granularity) ? (granularity - (number % granularity)) : 0);
	}

	/* Round value down */
	static MMINLINE uintptr_t roundToFloor(uintptr_t granularity, uintptr_t number) {
		return number - (number % granularity);
	}
	
	/* Round value down */
	static MMINLINE uint64_t roundToFloorU64(uint64_t granularity, uint64_t number) {
		return number - (number % granularity);
	}

	/* Round value to nearlest uintptr_t aligned */
	static MMINLINE uintptr_t roundToSizeofUDATA(uintptr_t number) {
		return (number + (sizeof(uintptr_t) - 1)) & (~(sizeof(uintptr_t) - 1));
	}
	
	/* Round value to nearlest uint32_t aligned */
	static MMINLINE uintptr_t roundToSizeofU32(uintptr_t number) {
		return (number + (sizeof(uint32_t) - 1)) & (~(sizeof(uint32_t) - 1));
	}

	/* Round value up to Card aligned */
	static MMINLINE uintptr_t roundToCeilingCard(uintptr_t number) {
		uintptr_t alignmentMask = CARD_SIZE - 1;
		return (number + alignmentMask) & ~alignmentMask;
	}

	/* Round value down to Card aligned */
	static MMINLINE uintptr_t roundToFloorCard(uintptr_t number) {
		uintptr_t alignmentMask = CARD_SIZE - 1;
		return number & ~alignmentMask;
	}

	/* returns:	 0 for 0
	 * 			 0 for 1
	 * 			 1 for [2,4)
	 * 			 2 for [4, 8)
	 *			...
	 * 			30 for [U32_MAX/4, U32_MAX/2)
	 * 			31 for [U32_MAX/2, U32_MAX] 
	 * 			...
	 * 			62 for [U64_MAX/4, U64_MAX/2)
	 * 			63 for [U64_MAX/2, U64_MAX] 
	 */
	static uintptr_t floorLog2(uintptr_t n) {
		uintptr_t pos = 0;
#ifdef OMR_ENV_DATA64
		if (n >= ((uintptr_t)1 << 32)) {
			n >>=  32;
			pos +=  32;
		}
#endif
		if (n >= (1<< 16)) {
			n >>=  16;
			pos +=  16;
		}		
		if (n >= (1<< 8)) {
			n >>=  8;
			pos +=  8;
		}
		if (n >= (1<< 4)) {
			n >>=  4;
			pos +=  4;
		}
		if (n >= (1<< 2)) {
			n >>=  2;
			pos +=  2;
		}
		if (n >= (1<< 1)) {
			pos +=  1;
		}
		return pos;
	}

	/**< return absolute value of a signed integer */
	static MMINLINE intptr_t abs(intptr_t number) {
		if (number < 0) {
			number = -number;
		}

		return number;
	}

	/**< return absolute value of a float */
	static MMINLINE float abs(float number) {
		if (number < 0) {
			number = -number;
		}
		return number;
	}


	/**
	 * Helper function to convert size to index using integers.
	 * In general conversion looks like index = logBase(size).
	 * Using log2(), this conversion can be written as index = log2(size)*(1/log2(base)).
	 * This function is hard coded to use 4 as 1/log2(base). It is corresponds with base = 1.18920712.
	 * Such selected value allows to use approximated log2() * 4 value as an index.
	 * Calculation of log2() is approximated:
	 * - integer part of log2() is calculated as MSB position;
	 * - next two bits are used for fractional part calculation (0,0.25,0.5,0.75).
	 * MSB position is calculated as topBit position minus number of leading zeroes.
	 * Number of leading zeroes is returned by MM_Bits::trailingZeroes().
	 * Please note, there is limitation for input size >= 8. Caller sites should control this.
	 * Please note this function supports 64 and 32 bit platforms.
	 *
	 * @param size needs to be converted to the index.
	 * @return index correspondent to the size.
	 */
	MMINLINE static uintptr_t sizeToIndex(uintptr_t size)
	{
		uintptr_t msb = (J9BITS_BITS_IN_SLOT - 1) - MM_Bits::trailingZeroes(size);
		uintptr_t index = 4 * msb + ((size >> (msb - 2)) & 0x3);

		return index;
	}

	/**
	 * Helper function to convert index to minimum size value for the range.
	 * This calculation is reversal to sizeToIndex(), see comment there.
	 *
	 * @param index needs to be converted to the size.
	 * @return minimum size for the range correspondent to the index.
	 */
	MMINLINE static uintptr_t indexToSize(uintptr_t index)
	{
		uintptr_t size = 0;

		if (8 <= index) {
			/*
			 * Constructing size for index:
			 *  create bit pattern in low bits MSB plus reserved space for two bits below 100b = 0x4
			 *  two low bits are stored in two low bits of index (index & 0x3), add them to bit pattern (0x4 + (index & 0x3))
			 *  (index / 4) is MSB position. In our bit pattern MSB is shifted left by 2 already (set to 4),
			 *  so shift bit pattern left for remaining (index / 4 - 2) bits.
			 */
			size = (4 + (index & 3)) << (index / 4 - 2);
		} else {
			/*
			 * Approximate log(n) = n + 1 for small n
			 */
			size = index / 4 + 1;
		}
		return size;
	}

};

#endif /*MATH_HPP_*/
