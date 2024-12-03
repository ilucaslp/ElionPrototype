#ifndef __MU_LIST_H__
#define __MU_LIST_H__

#pragma once

template <class ContainerType>
class MULinkedListIteratorBase
{
public:
	explicit MULinkedListIteratorBase(ContainerType* FirstLink)
		: CurrentLink(FirstLink)
	{
	}

	/**
	* Advances the iterator to the next element.
	*/
	FORCEINLINE void Next()
	{
		CurrentLink = (ContainerType*)CurrentLink->GetNextLink();
	}

	FORCEINLINE MULinkedListIteratorBase& operator++()
	{
		Next();
		return *this;
	}

	FORCEINLINE MULinkedListIteratorBase operator++(mu_int32)
	{
		auto Tmp = *this;
		Next();
		return Tmp;
	}

	FORCEINLINE mu_boolean IsValid()
	{
		return CurrentLink != nullptr;
	}

protected:
	ContainerType* CurrentLink;

	FORCEINLINE friend mu_boolean operator==(const MULinkedListIteratorBase& Lhs, const MULinkedListIteratorBase& Rhs) { return Lhs.CurrentLink == Rhs.CurrentLink; }
	FORCEINLINE friend mu_boolean operator!=(const MULinkedListIteratorBase& Lhs, const MULinkedListIteratorBase& Rhs) { return Lhs.CurrentLink != Rhs.CurrentLink; }
};

template <class ContainerType, class ElementType>
class MULinkedListIterator : public MULinkedListIteratorBase<ContainerType>
{
	typedef MULinkedListIteratorBase<ContainerType> Super;

public:
	explicit MULinkedListIterator(ContainerType* FirstLink)
		: Super(FirstLink)
	{
	}

	// Accessors.
	FORCEINLINE ElementType& operator->() const
	{
		return **(this->CurrentLink);
	}

	FORCEINLINE ElementType& operator*() const
	{
		return **(this->CurrentLink);
	}
};

template <class ContainerType, class ElementType>
class MUIntrusiveLinkedListIterator : public MULinkedListIteratorBase<ElementType>
{
	typedef MULinkedListIteratorBase<ElementType> Super;

public:
	explicit MUIntrusiveLinkedListIterator(ElementType* FirstLink)
		: Super(FirstLink)
	{
	}

	// Accessors.
	FORCEINLINE ElementType& operator->() const
	{
		return *(this->CurrentLink);
	}

	FORCEINLINE ElementType& operator*() const
	{
		return *(this->CurrentLink);
	}
};


/**
* Base linked list class, used to implement methods shared by intrusive/non-intrusive linked lists
*/
template <class ContainerType, class ElementType, template<class, class> class IteratorType>
class MULinkedListBase
{
public:
	/**
	* Used to iterate over the elements of a linked list.
	*/
	typedef IteratorType<ContainerType, ElementType> Iterator;
	typedef IteratorType<ContainerType, const ElementType> TConstIterator;


	/**
	* Default constructor (empty list)
	*/
	MULinkedListBase()
		: NextLink(nullptr)
		, PrevLink(nullptr)
	{
	}

	/**
	* Removes this element from the list in constant time.
	*
	* This function is safe to call even if the element is not linked.
	*/
	FORCEINLINE void Unlink()
	{
		if (NextLink)
		{
			NextLink->PrevLink = PrevLink;
		}
		if (PrevLink)
		{
			*PrevLink = NextLink;
		}
		// Make it safe to call Unlink again.
		NextLink = nullptr;
		PrevLink = nullptr;
	}


	/**
	* Adds this element to a list, before the given element.
	*
	* @param Before	The link to insert this element before.
	*/
	FORCEINLINE void LinkBefore(ContainerType* Before)
	{
		PrevLink = Before->PrevLink;
		Before->PrevLink = &NextLink;

		NextLink = Before;

		if (PrevLink != nullptr)
		{
			*PrevLink = (ContainerType*)this;
		}
	}

	/**
	* Adds this element to the linked list, after the specified element
	*
	* @param After		The link to insert this element after.
	*/
	FORCEINLINE void LinkAfter(ContainerType* After)
	{
		PrevLink = &After->NextLink;
		NextLink = *PrevLink;
		*PrevLink = (ContainerType*)this;

		if (NextLink != nullptr)
		{
			NextLink->PrevLink = &NextLink;
		}
	}

	/**
	* Adds this element to the linked list, replacing the specified element.
	* This is equivalent to calling LinkBefore(Replace); Replace->Unlink();
	*
	* @param Replace	Pointer to the element to be replaced
	*/
	FORCEINLINE void LinkReplace(ContainerType* Replace)
	{
		ContainerType**& ReplacePrev = Replace->PrevLink;
		ContainerType*& ReplaceNext = Replace->NextLink;

		PrevLink = ReplacePrev;
		NextLink = ReplaceNext;

		if (PrevLink != nullptr)
		{
			*PrevLink = (ContainerType*)this;
		}

		if (NextLink != nullptr)
		{
			NextLink->PrevLink = &NextLink;
		}

		ReplacePrev = nullptr;
		ReplaceNext = nullptr;
	}


	/**
	* Adds this element as the head of the linked list, linking the input Head pointer to this element,
	* so that when the element is linked/unlinked, the Head linked list pointer will be correctly updated.
	*
	* If Head already has an element, this functions like LinkBefore.
	*
	* @param Head		Pointer to the head of the linked list - this pointer should be the main reference point for the linked list
	*/
	FORCEINLINE void LinkHead(ContainerType*& Head)
	{
		if (Head != nullptr)
		{
			Head->PrevLink = &NextLink;
		}

		NextLink = Head;
		PrevLink = &Head;
		Head = (ContainerType*)this;
	}


	/**
	* Returns whether element is currently linked.
	*
	* @return true if currently linked, false otherwise
	*/
	FORCEINLINE mu_boolean IsLinked()
	{
		return PrevLink != nullptr;
	}

	FORCEINLINE ContainerType** GetPrevLink() const
	{
		return PrevLink;
	}

	FORCEINLINE ContainerType* GetNextLink() const
	{
		return NextLink;
	}

	FORCEINLINE ContainerType* Next()
	{
		return NextLink;
	}

private:
	/** The next link in the linked list */
	ContainerType*  NextLink;

	/** Pointer to 'NextLink', within the previous link in the linked list */
	ContainerType** PrevLink;


	FORCEINLINE friend Iterator      begin(ContainerType& List) { return Iterator(&List); }
	FORCEINLINE friend TConstIterator begin(const ContainerType& List) { return TConstIterator(const_cast<ContainerType*>(&List)); }
	FORCEINLINE friend Iterator      end(ContainerType& List) { return Iterator(nullptr); }
	FORCEINLINE friend TConstIterator end(const ContainerType& List) { return TConstIterator(nullptr); }
};

/**
* Encapsulates a link in a single linked list with constant access time.
*
* This linked list is non-intrusive, i.e. it stores a copy of the element passed to it (typically a pointer)
*/
template <class ElementType>
class MULinkedList : public MULinkedListBase<MULinkedList<ElementType>, ElementType, MULinkedListIterator>
{
	typedef MULinkedListBase<MULinkedList<ElementType>, ElementType, MULinkedListIterator>		Super;

public:
	/** Default constructor (empty list). */
	MULinkedList()
		: Super()
	{
	}

	/**
	* Creates a new linked list with a single element.
	*
	* @param InElement The element to add to the list.
	*/
	explicit MULinkedList(const ElementType& InElement)
		: Super()
		, Element(InElement)
	{
	}


	// Accessors.
	FORCEINLINE ElementType* operator->()
	{
		return &Element;
	}
	FORCEINLINE const ElementType* operator->() const
	{
		return &Element;
	}
	FORCEINLINE ElementType& operator*()
	{
		return Element;
	}
	FORCEINLINE const ElementType& operator*() const
	{
		return Element;
	}


private:
	ElementType   Element;
};

/**
* Encapsulates a link in a single linked list with constant access time.
* Structs/classes must inherit this, to use it, e.g: struct FMyStruct : public MUIntrusiveLinkedList<FMyStruct>
*
* This linked list is intrusive, i.e. the element is a subclass of this link, so that each link IS the element.
*
* Never reference MUIntrusiveLinkedList outside of the above class/struct inheritance, only ever refer to the struct, e.g:
*	FMyStruct* MyLinkedList = nullptr;
*
*	FMyStruct* StructLink = new FMyStruct();
*	StructLink->LinkHead(MyLinkedList);
*
*	for (FMyStruct::TIterator It(MyLinkedList); It; It.Next())
*	{
*		...
*	}
*/
template <class ElementType>
class MUIntrusiveLinkedList : public MULinkedListBase<ElementType, ElementType, MUIntrusiveLinkedListIterator>
{
	typedef MULinkedListBase<ElementType, ElementType, MUIntrusiveLinkedListIterator>		Super;

public:
	/** Default constructor (empty list). */
	MUIntrusiveLinkedList()
		: Super()
	{
	}
};


template <class NodeType, class ElementType>
class MUDoubleLinkedListIterator
{
public:

	explicit MUDoubleLinkedListIterator(NodeType* StartingNode)
		: CurrentNode(StartingNode)
	{
	}

	mu_boolean IsValid()
	{
		return CurrentNode != nullptr;
	}

	MUDoubleLinkedListIterator& operator++()
	{
		CurrentNode = CurrentNode->GetNextNode();
		return *this;
	}

	MUDoubleLinkedListIterator operator++(mu_int32)
	{
		auto Tmp = *this;
		++(*this);
		return Tmp;
	}

	MUDoubleLinkedListIterator& operator--()
	{
		CurrentNode = CurrentNode->GetPrevNode();
		return *this;
	}

	MUDoubleLinkedListIterator operator--(mu_int32)
	{
		auto Tmp = *this;
		--(*this);
		return Tmp;
	}

	// Accessors.
	ElementType& operator->() const
	{
		return CurrentNode->GetValue();
	}

	ElementType& operator*() const
	{
		return CurrentNode->GetValue();
	}

	NodeType* GetNode() const
	{
		return CurrentNode;
	}

private:
	NodeType* CurrentNode;

	friend mu_boolean operator==(const MUDoubleLinkedListIterator& Lhs, const MUDoubleLinkedListIterator& Rhs) { return Lhs.CurrentNode == Rhs.CurrentNode; }
	friend mu_boolean operator!=(const MUDoubleLinkedListIterator& Lhs, const MUDoubleLinkedListIterator& Rhs) { return Lhs.CurrentNode != Rhs.CurrentNode; }
};


/**
* Double linked list.
*/
template <class ElementType>
class MUDoubleLinkedList
{
public:
	class MUDoubleLinkedListNode
	{
	public:
		friend class MUDoubleLinkedList;

		/** Constructor */
		MUDoubleLinkedListNode(const ElementType& InValue)
			: Value(InValue), NextNode(nullptr), PrevNode(nullptr)
		{
		}

		const ElementType& GetValue() const
		{
			return Value;
		}

		ElementType& GetValue()
		{
			return Value;
		}

		MUDoubleLinkedListNode* GetNextNode()
		{
			return NextNode;
		}

		MUDoubleLinkedListNode* GetPrevNode()
		{
			return PrevNode;
		}

	protected:
		ElementType            Value;
		MUDoubleLinkedListNode* NextNode;
		MUDoubleLinkedListNode* PrevNode;
	};

	/**
	* Used to iterate over the elements of a linked list.
	*/
	typedef MUDoubleLinkedListIterator<MUDoubleLinkedListNode, ElementType> TIterator;
	typedef MUDoubleLinkedListIterator<MUDoubleLinkedListNode, const ElementType> TConstIterator;

	/** Constructors. */
	MUDoubleLinkedList()
		: HeadNode(nullptr)
		, TailNode(nullptr)
		, ListSize(0)
	{
	}

	/** Destructor */
	virtual ~MUDoubleLinkedList()
	{
		Empty();
	}

	// Adding/Removing methods

	/**
	* Add the specified value to the beginning of the list, making that value the new head of the list.
	*
	* @param	InElement	the value to add to the list.
	* @return	whether the node was successfully added into the list.
	* @see GetHead, InsertNode, RemoveNode
	*/
	mu_boolean AddHead(const ElementType& InElement)
	{
		return AddHead(new MUDoubleLinkedListNode(InElement));
	}

	mu_boolean AddHead(MUDoubleLinkedListNode* NewNode)
	{
		if (NewNode == nullptr)
		{
			return false;
		}

		// have an existing head node - change the head node to point to this one
		if (HeadNode != nullptr)
		{
			NewNode->NextNode = HeadNode;
			HeadNode->PrevNode = NewNode;
			HeadNode = NewNode;
		}
		else
		{
			HeadNode = TailNode = NewNode;
		}

		SetListSize(ListSize + 1);
		return true;
	}

	/**
	* Append the specified value to the end of the list
	*
	* @param	InElement	the value to add to the list.
	* @return	whether the node was successfully added into the list
	* @see GetTail, InsertNode, RemoveNode
	*/
	mu_boolean AddTail(const ElementType& InElement)
	{
		return AddTail(new MUDoubleLinkedListNode(InElement));
	}

	mu_boolean AddTail(MUDoubleLinkedListNode* NewNode)
	{
		if (NewNode == nullptr)
		{
			return false;
		}

		if (TailNode != nullptr)
		{
			TailNode->NextNode = NewNode;
			NewNode->PrevNode = TailNode;
			TailNode = NewNode;
		}
		else
		{
			HeadNode = TailNode = NewNode;
		}

		SetListSize(ListSize + 1);
		return true;
	}

	/**
	* Insert the specified value into the list at an arbitrary point.
	*
	* @param	InElement			the value to insert into the list
	* @param	NodeToInsertBefore	the new node will be inserted into the list at the current location of this node
	*								if nullptr, the new node will become the new head of the list
	* @return	whether the node was successfully added into the list
	* @see Empty, RemoveNode
	*/
	mu_boolean InsertNode(const ElementType& InElement, MUDoubleLinkedListNode* NodeToInsertBefore = nullptr)
	{
		return InsertNode(new MUDoubleLinkedListNode(InElement), NodeToInsertBefore);
	}

	mu_boolean InsertNode(MUDoubleLinkedListNode* NewNode, MUDoubleLinkedListNode* NodeToInsertBefore = nullptr)
	{
		if (NewNode == nullptr)
		{
			return false;
		}

		if (NodeToInsertBefore == nullptr || NodeToInsertBefore == HeadNode)
		{
			return AddHead(NewNode);
		}

		NewNode->PrevNode = NodeToInsertBefore->PrevNode;
		NewNode->NextNode = NodeToInsertBefore;

		NodeToInsertBefore->PrevNode->NextNode = NewNode;
		NodeToInsertBefore->PrevNode = NewNode;

		SetListSize(ListSize + 1);
		return true;
	}

	/**
	* Remove the node corresponding to InElement.
	*
	* @param InElement The value to remove from the list.
	* @see Empty, InsertNode
	*/
	void RemoveNode(const ElementType& InElement)
	{
		MUDoubleLinkedListNode* ExistingNode = FindNode(InElement);
		RemoveNode(ExistingNode);
	}

	/**
	* Removes the node specified.
	*
	* @param NodeToRemove The node to remove.
	* @see Empty, InsertNode
	*/
	void RemoveNode(MUDoubleLinkedListNode* NodeToRemove, mu_boolean bDeleteNode = true)
	{
		if (NodeToRemove != nullptr)
		{
			// if we only have one node, just call Clear() so that we don't have to do lots of extra checks in the code below
			if (Num() == 1)
			{
				if (bDeleteNode)
				{
					Empty();
				}
				else
				{
					NodeToRemove->NextNode = NodeToRemove->PrevNode = nullptr;
					HeadNode = TailNode = nullptr;
					SetListSize(0);
				}
				return;
			}

			if (NodeToRemove == HeadNode)
			{
				HeadNode = HeadNode->NextNode;
				HeadNode->PrevNode = nullptr;
			}

			else if (NodeToRemove == TailNode)
			{
				TailNode = TailNode->PrevNode;
				TailNode->NextNode = nullptr;
			}
			else
			{
				NodeToRemove->NextNode->PrevNode = NodeToRemove->PrevNode;
				NodeToRemove->PrevNode->NextNode = NodeToRemove->NextNode;
			}

			if (bDeleteNode)
			{
				delete NodeToRemove;
			}
			else
			{
				NodeToRemove->NextNode = NodeToRemove->PrevNode = nullptr;
			}
			SetListSize(ListSize - 1);
		}
	}

	/** Removes all nodes from the list. */
	void Empty()
	{
		MUDoubleLinkedListNode* Node;
		while (HeadNode != nullptr)
		{
			Node = HeadNode->NextNode;
			delete HeadNode;
			HeadNode = Node;
		}

		HeadNode = TailNode = nullptr;
		SetListSize(0);
	}

	// Accessors.

	/**
	* Returns the node at the head of the list.
	*
	* @return Pointer to the first node.
	* @see GetTail
	*/
	MUDoubleLinkedListNode* GetHead() const
	{
		return HeadNode;
	}

	/**
	* Returns the node at the end of the list.
	*
	* @return Pointer to the last node.
	* @see GetHead
	*/
	MUDoubleLinkedListNode* GetTail() const
	{
		return TailNode;
	}

	/**
	* Finds the node corresponding to the value specified
	*
	* @param	InElement	the value to find
	* @return	a pointer to the node that contains the value specified, or nullptr of the value couldn't be found
	*/
	MUDoubleLinkedListNode* FindNode(const ElementType& InElement)
	{
		MUDoubleLinkedListNode* Node = HeadNode;
		while (Node != nullptr)
		{
			if (Node->GetValue() == InElement)
			{
				break;
			}

			Node = Node->NextNode;
		}

		return Node;
	}

	mu_boolean Contains(const ElementType& InElement)
	{
		return (FindNode(InElement) != nullptr);
	}

	/**
	* Returns the number of items in the list.
	*
	* @return Item count.
	*/
	mu_int32 Num() const
	{
		return ListSize;
	}

protected:

	/**
	* Updates the size reported by Num().  Child classes can use this function to conveniently
	* hook into list additions/removals.
	*
	* @param	NewListSize		the new size for this list
	*/
	virtual void SetListSize(mu_int32 NewListSize)
	{
		ListSize = NewListSize;
	}

private:
	MUDoubleLinkedListNode* HeadNode;
	MUDoubleLinkedListNode* TailNode;
	mu_int32 ListSize;

	MUDoubleLinkedList(const MUDoubleLinkedList&);
	MUDoubleLinkedList& operator=(const MUDoubleLinkedList&);

	friend TIterator      begin(MUDoubleLinkedList& List) { return TIterator(List.GetHead()); }
	friend TConstIterator begin(const MUDoubleLinkedList& List) { return TConstIterator(List.GetHead()); }
	friend TIterator      end(MUDoubleLinkedList& List) { return TIterator(nullptr); }
	friend TConstIterator end(const MUDoubleLinkedList& List) { return TConstIterator(nullptr); }
};


/*----------------------------------------------------------------------------
MUList.
----------------------------------------------------------------------------*/

//
// Simple single-linked list template.
//
template <class ElementType> class MUList
{
public:

	ElementType			Element;
	MUList<ElementType>*	Next;

	// Constructor.

	MUList(const ElementType &InElement, MUList<ElementType>* InNext = nullptr)
	{
		Element = InElement;
		Next = InNext;
	}
};

#endif