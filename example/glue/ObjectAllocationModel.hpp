
/*******************************************************************************
 * Licensed Materials - Property of IBM
 * "Restricted Materials of IBM"
 *
 * (c) Copyright IBM Corp. 1991, 2014 All Rights Reserved
 *
 * US Government Users Restricted Rights - Use, duplication or disclosure
 * restricted by GSA ADP Schedule Contract with IBM Corp.
 *******************************************************************************/

#if !defined(OBJECTALLOCATIONMODEL_HPP_)
#define OBJECTALLOCATIONMODEL_HPP_

#include "AllocateInitialization.hpp"
#include "ObjectModel.hpp"

/**
 * Class definition for the Java object allocation model.
 */
class MM_ObjectAllocationModel : public MM_AllocateInitialization
{
	/*
	 * Member data and types
	 */
public:
	/**
	 * Define object allocation categories. These are represented in MM_AllocateInitialization
	 * objects and are used in GC_ObjectModel::initializeAllocation() to determine how to
	 * initialize the header of a newly allocated object.
	 */
	enum {
		allocation_category_example
	};

protected:
private:

	/*
	 * Member functions
	 */
private:
protected:
public:
	/**
	 * Initializer.
	 */
	MMINLINE omrobjectptr_t
	initializeObject(MM_EnvironmentBase *env, void *allocatedBytes)
	{
		omrobjectptr_t objectPtr = (omrobjectptr_t)allocatedBytes;

		if (NULL != objectPtr) {
			env->getExtensions()->objectModel.setObjectSize(objectPtr, getAllocateDescription()->getBytesRequested());
		}

		return objectPtr;
	}

	/**
	 * Constructor.
	 */
	MM_ObjectAllocationModel(MM_EnvironmentBase *env,  uintptr_t requiredSizeInBytes, uintptr_t allocateObjectFlags = 0)
		: MM_AllocateInitialization(env, allocation_category_example, requiredSizeInBytes, allocateObjectFlags)
	{}
};
#endif /* OBJECTALLOCATIONMODEL_HPP_ */
