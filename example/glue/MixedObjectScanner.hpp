/*******************************************************************************
 *
 * (c) Copyright IBM Corp. 2016
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

#ifndef MIXEDOBJECTSCANNER_HPP_
#define MIXEDOBJECTSCANNER_HPP_

#include "ObjectScanner.hpp"
#include "GCExtensionsBase.hpp"
#include "ObjectModel.hpp"

class GC_MixedObjectScanner : public GC_ObjectScanner
{
	/* Data Members */
private:
	fomrobject_t * const _endPtr;	/**< end scan pointer */
	fomrobject_t *_mapPtr;			/**< pointer to first slot in current scan segment */

protected:

public:

	/* Member Functions */
private:

protected:
	/**
	 * @param[in] env The scanning thread environment
	 * @param[in] objectPtr the object to be processed
	 * @param[in] flags Scanning context flags
	 */
	MMINLINE GC_MixedObjectScanner(MM_EnvironmentBase *env, omrobjectptr_t objectPtr, uintptr_t flags)
		: GC_ObjectScanner(env, objectPtr, (fomrobject_t *)objectPtr + 1, 0, flags, 0)
		, _endPtr((fomrobject_t *)((uint8_t*)objectPtr + MM_GCExtensionsBase::getExtensions(env->getOmrVM())->objectModel.getConsumedSizeInBytesWithHeader(objectPtr)))
		, _mapPtr(_scanPtr)
	{
		_typeId = __FUNCTION__;
	}

	/**
	 * Subclasses must call this method to set up the instance description bits and description pointer.
	 * @param[in] env The scanning thread environment
	 */
	MMINLINE void
	initialize(MM_EnvironmentBase *env)
	{
		GC_ObjectScanner::initialize(env);

		intptr_t slotCount = _endPtr - _scanPtr;

		/* Initialize the slot map assuming all slots are reference slots or NULL */
		if (slotCount < _bitsPerScanMap) {
			_scanMap = (((uintptr_t)1) << slotCount) - 1;
			setNoMoreSlots();
		} else {
			_scanMap = ~((uintptr_t)0);
			if (slotCount == _bitsPerScanMap) {
				setNoMoreSlots();
			}
		}
	}

public:
	/**
	 * In-place instantiation and initialization for mixed obect scanner.
	 * @param[in] env The scanning thread environment
	 * @param[in] objectPtr The object to scan
	 * @param[in] allocSpace Pointer to space for in-place instantiation (at least sizeof(GC_MixedObjectScanner) bytes)
	 * @param[in] flags Scanning context flags
	 * @return Pointer to GC_MixedObjectScanner instance in allocSpace
	 */
	MMINLINE static GC_MixedObjectScanner *
	newInstance(MM_EnvironmentBase *env, omrobjectptr_t objectPtr, void *allocSpace, uintptr_t flags)
	{
		GC_MixedObjectScanner *objectScanner = NULL;
		if (NULL != allocSpace) {
			new(allocSpace) GC_MixedObjectScanner(env, objectPtr, flags);
			objectScanner = (GC_MixedObjectScanner *)allocSpace;
			objectScanner->initialize(env);
		}
		return objectScanner;
	}

	MMINLINE uintptr_t getBytesRemaining() { return sizeof(fomrobject_t) * (_endPtr - _scanPtr); }

	/**
	 * @see GC_ObjectScanner::getNextSlotMap()
	 */
	virtual fomrobject_t *
	getNextSlotMap(uintptr_t &slotMap, bool &hasNextSlotMap)
	{
		intptr_t slotCount = _endPtr - _scanPtr;

		/* Initialize the slot map assuming all slots are reference slots or NULL */
		if (slotCount < _bitsPerScanMap) {
			slotMap = (((uintptr_t)1) << slotCount) - 1;
			hasNextSlotMap = false;
		} else {
			slotMap = ~((uintptr_t)0);
			hasNextSlotMap = slotCount > _bitsPerScanMap;
		}

		_mapPtr += _bitsPerScanMap;
		return _mapPtr;
	}

#if defined(OMR_GC_LEAF_BITS)
	/**
	 * @see GC_ObjectScanner::getNextSlotMap(uintptr_t&, uintptr_t&, bool&)
	 */
	virtual fomrobject_t *
	getNextSlotMap(uintptr_t &slotMap, uintptr_t &leafMap, bool &hasNextSlotMap)
	{
		leafMap = 0;
		return getNextSlotMap(slotMap, hasNextSlotMap);
	}
#endif /* OMR_GC_LEAF_BITS */
};

#endif /* MIXEDOBJECTSCANNER_HPP_ */
