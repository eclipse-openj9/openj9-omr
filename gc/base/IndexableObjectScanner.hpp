/*******************************************************************************
 * Copyright (c) 2015, 2019 IBM Corp. and others
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

#if !defined(INDEXABLEOBJECTSCANNER_HPP_)
#define INDEXABLEOBJECTSCANNER_HPP_

#include "ObjectScanner.hpp"

class GC_IndexableObjectScanner : public GC_ObjectScanner
{
	/* Data Members */
private:

protected:
	fomrobject_t *_endPtr;	/**< pointer to end of last array element in scan segment */
	fomrobject_t *_basePtr;	/**< pointer to base array element */
	fomrobject_t *_limitPtr;/**< pointer to end of last array element */

public:

	/* Member Functions */
private:

protected:
	/**
	 * @param env The scanning thread environment
	 * @param[in] arrayPtr pointer to the array to be processed
	 * @param[in] basePtr pointer to the first contiguous array cell
	 * @param[in] limitPtr pointer to end of last contiguous array cell
	 * @param[in] scanPtr pointer to the array cell where scanning will start
	 * @param[in] endPtr pointer to the array cell where scanning will stop
	 * @param[in] flags scanning context flags
	 */
	GC_IndexableObjectScanner(
		MM_EnvironmentBase *env
		, omrobjectptr_t arrayPtr
		, fomrobject_t *basePtr
		, fomrobject_t *limitPtr
		, fomrobject_t *scanPtr
		, fomrobject_t *endPtr
		, uintptr_t flags
	)
		: GC_ObjectScanner(
			env
			, arrayPtr
			, scanPtr
			, ((endPtr - scanPtr) < _bitsPerScanMap) ? (((uintptr_t)1 << (endPtr - scanPtr)) - 1) : UDATA_MAX
			, flags | GC_ObjectScanner::indexableObject
		)
		, _endPtr(endPtr)
		, _basePtr(basePtr)
		, _limitPtr(limitPtr)
	{
		_typeId = __FUNCTION__;
		if ((endPtr - scanPtr) <= _bitsPerScanMap) {
			setNoMoreSlots();
		}
	}

	/**
	 * Set up the scanner.
	 * @param[in] env current environment (per thread)
	 */
	MMINLINE void
	initialize(MM_EnvironmentBase *env)
	{
		Assert_MM_true(_basePtr <= _scanPtr);
		Assert_MM_true(_scanPtr <= _endPtr);
		Assert_MM_true(_endPtr <= _limitPtr);
		GC_ObjectScanner::initialize(env);
	}

public:
	/**
	 * Get the maximal index for the array. Array indices are assumed to be zero-based.
	 */
	MMINLINE uintptr_t getIndexableRange() { return _limitPtr - _basePtr; }

	/**
	 * Reset truncated end pointer to force scanning to limit pointer (scan to end of indexable object). This
	 * must be called if this scanner cannot be split to hive off the tail
	 */
	MMINLINE void scanToLimit() { _endPtr = _limitPtr; }

	/**
	 * Split this instance and set split scan/end pointers to indicate split scan range.
	 *
	 * @param env The scanning thread environment
	 * @param allocSpace Pointer to memory where split scanner will be instantiated (in-place)
	 * @param splitAmount The maximum number of array elements to include
	 * @return Pointer to split scanner in allocSpace
	 */
	virtual GC_IndexableObjectScanner *splitTo(MM_EnvironmentBase *env, void *allocSpace, uintptr_t splitAmount) = 0;
};

#endif /* INDEXABLEOBJECTSCANNER_HPP_ */
