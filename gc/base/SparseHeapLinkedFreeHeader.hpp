/*******************************************************************************
 * Copyright (c) 2022, 2022 IBM Corp. and others
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

#if !defined(SPARSEHEAPLINKEDFREEHEADER_HPP_)
#define SPARSEHEAPLINKEDFREEHEADER_HPP_

/**
 *
 *
 * @ingroup GC_Base_Core
 */

class MM_SparseHeapLinkedFreeHeader
{
/*
 * Data members
 */
private:
protected:
	uintptr_t _size;
	void *_address; /**< heap address associated with current node */
	MM_SparseHeapLinkedFreeHeader *_next;

public:

/*
 * Function members
 */
private:
protected:

public:

	MM_SparseHeapLinkedFreeHeader(uintptr_t size, void* address)
		: _size(size)
		, _address(address)
		, _next(NULL)
	{
	}

	/**
	 * Contract this entry by the specified number of bytes.
	 */
	MMINLINE bool contractSize(uintptr_t increment)
	{
		_size -= increment;
		return 0 == _size;
	}

	/**
	 * Expand this entry by the specified number of bytes.
	 */
	MMINLINE void expandSize(uintptr_t increment)
	{
		_size += increment;
	}

	/**
	 * Set the size of this entry.
	 */
	MMINLINE void setSize(uintptr_t size)
	{
		_size = size;
	}

	/**
	 * Set the address of this entry.
	 */
	MMINLINE void setAddress(void *address)
	{
		_address = address;
	}

	friend class MM_SparseAddressOrderedFixedSizeDataPool;

};

#endif /* SPARSEHEAPLINKEDFREEHEADER_HPP_ */
