/*
 * The Apache Software License, Version 1.1
 *
 *
 * Copyright (c) 2000 The Apache Software Foundation.  All rights 
 * reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer. 
 *
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 *
 * 3. The end-user documentation included with the redistribution,
 *    if any, must include the following acknowledgment:  
 *       "This product includes software developed by the
 *        Apache Software Foundation (http://www.apache.org/)."
 *    Alternately, this acknowledgment may appear in the software itself,
 *    if and wherever such third-party acknowledgments normally appear.
 *
 * 4. The names "Xalan" and "Apache Software Foundation" must
 *    not be used to endorse or promote products derived from this
 *    software without prior written permission. For written 
 *    permission, please contact apache@apache.org.
 *
 * 5. Products derived from this software may not be called "Apache",
 *    nor may "Apache" appear in their name, without prior written
 *    permission of the Apache Software Foundation.
 *
 * THIS SOFTWARE IS PROVIDED ``AS IS'' AND ANY EXPRESSED OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED.  IN NO EVENT SHALL THE APACHE SOFTWARE FOUNDATION OR
 * ITS CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
 * USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
 * OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 * ====================================================================
 *
 * This software consists of voluntary contributions made by many
 * individuals on behalf of the Apache Software Foundation and was
 * originally based on software copyright (c) 1999, International
 * Business Machines, Inc., http://www.ibm.com.  For more
 * information on the Apache Software Foundation, please see
 * <http://www.apache.org/>.
 */

#if !defined(REUSABLEARENABLOCK_INCLUDE_GUARD_1357924680)
#define REUSABLEARENABLOCK_INCLUDE_GUARD_1357924680



#include "ArenaBlock.hpp"



template<class ObjectType,
		 class DestroyFunctionType = ArenaBlockDestroy<ObjectType> >
class ReusableArenaBlockDestroy
{
public:

	ReusableArenaBlockDestroy() :
		m_freeListSet()
	{
	}

	void
	operator()(ObjectType&	theObject) const
	{
		if (isOnFreeList(&theObject) == false)
		{
			m_destroyFunction(theObject);
		}
	}

	bool
	isOnFreeList(ObjectType*	theObject) const
	{
		return m_freeListSet.find(theObject) == m_freeListSet.end() ? false : true;
	}

	void
	destroyObject(ObjectType*	theObject)
	{
		assert(theObject != 0);

		m_destroyFunction(*theObject);

		m_freeListSet.insert(theObject);
	}

	ObjectType*
	getNextFromFreeList()
	{
		assert(m_freeListSet.size() != 0);

		const FreeListSetType::const_iterator	i = m_freeListSet.begin();

		return *i;
	}

	bool
	removeFromFreeList(ObjectType*	theObject)
	{
		assert(m_freeListSet.size() != 0);

		const FreeListSetType::iterator		i =
			m_freeListSet.find(theObject);

		if (i == m_freeListSet.end())
		{
			return false;
		}
		else
		{
			m_freeListSet.erase(i);

			return true;
		}
	}

#if defined(XALAN_NO_NAMESPACES)
	typedef set<ObjectType*>		FreeListSetType;
#else
	typedef std::set<ObjectType*>	FreeListSetType;
#endif

	FreeListSetType::size_type
	freeListSize() const
	{
		return m_freeListSet.size();
	}

private:

	// Destroy function to call.
	DestroyFunctionType		m_destroyFunction;

	// Set which contains pointers to all objects
	// that should not be destroyed.
	FreeListSetType			m_freeListSet;
};



template<class ObjectType,
		 class DestroyFunctionType = ArenaBlockDestroy<ObjectType>,
#if defined(XALAN_NO_NAMESPACES)
		 class AllocatorType = allocator<ObjectType>,
#else
		 class AllocatorType = std::allocator<ObjectType>,
#endif
		 class ReusableDestroyFunctionType = ReusableArenaBlockDestroy<ObjectType,
																	   DestroyFunctionType> >
class ReusableArenaBlock : public ArenaBlock<ObjectType,
											 ReusableDestroyFunctionType,
											 AllocatorType>
{
public:

	typedef ArenaBlock<ObjectType,
					   ReusableDestroyFunctionType,
					   AllocatorType>				BaseClassType;

	ReusableArenaBlock(size_type	theBlockCount) :
		BaseClassType(theBlockCount)
	{
	}

	~ReusableArenaBlock()
	{
	}

	virtual ObjectType*
	allocateBlock()
	{
		if (m_destroyFunction.freeListSize() == 0)
		{
			return BaseClassType::allocateBlock();
		}
		else
		{
			return m_destroyFunction.getNextFromFreeList();
		}
	}

	// $$$ ToDo: How much error checking, etc. do we do here?  Is
	// it worth trying to throw exceptions when things are not
	// what they should be?
	virtual void
	commitAllocation(ObjectType*	theBlock)
	{
		assert(theBlock != 0);
		assert(m_destroyFunction.freeListSize() == 0 ||
			   theBlock == m_destroyFunction.getNextFromFreeList());

		if (m_destroyFunction.freeListSize() == 0)
		{
			BaseClassType::commitAllocation(theBlock);
		}
		else
		{
			m_destroyFunction.removeFromFreeList(theBlock);
		}
	}

	virtual bool
	blockAvailable() const
	{
		return m_destroyFunction.freeListSize() != 0 ? true : BaseClassType::blockAvailable();
	}

	void
	destroyObject(ObjectType*	theObject)
	{
		if (ownsObject(theObject) == true &&
			m_destroyFunction.isOnFreeList(theObject) == false)
		{
			m_destroyFunction.destroyObject(theObject);
		}
	}

private:

	// Cannot and should not be implemented...
	ReusableArenaBlock(const ReusableArenaBlock&);

	ReusableArenaBlock&
	operator=(const ReusableArenaBlock&);

	bool
	operator==(const ReusableArenaBlock&) const;
};



#endif	// !defined(REUSABLEARENABLOCK_INCLUDE_GUARD_1357924680)
