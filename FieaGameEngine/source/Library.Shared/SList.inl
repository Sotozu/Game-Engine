#include "SList.h"

namespace FieaGameEngine
{
#pragma region SLIST FUNCTIONS

	//Copy Constructor
	template<typename T>
	SList<T>::SList(const SList& right) : _size(right._size)
	{
		if (!right.IsEmpty()) //if passing in an empty SList to copy construct from then don't do anything
		{
			Node* firstNode = new Node(right._front->_data);
			_front = firstNode;
			Node* previousNode = firstNode;
			Node* rightTraversalNode = right._front;

			while (rightTraversalNode->_next != nullptr) {
				rightTraversalNode = rightTraversalNode->_next;
				Node* newNode = new Node(rightTraversalNode->_data);
				previousNode->_next = newNode;
				previousNode = newNode;
			}

			_back = previousNode;
			_back->_next = nullptr;
		}	
	}

	//Initializer List Constructor
	template<typename T>
	SList<T>::SList(std::initializer_list<T> list)
	{
		for (const auto& value : list)
		{
			PushBack(value);
		}
	}

	template<typename T>
	inline T& SList<T>::Front()
	{
		if (IsEmpty()) {throw std::runtime_error("Cannot access an empty list.");}
		return _front->_data;
	}

	template<typename T>
	inline T& SList<T>::Back()
	{
		if (IsEmpty()) {throw std::runtime_error("Cannot access an empty list.");}
		return _back->_data;
	}

	template<typename T>
	inline const T& SList<T>::Front() const 
	{
		if (IsEmpty()) {throw std::runtime_error("Cannot access an empty list.");}
		return _front->_data;
	}

	template<typename T>
	inline const T& SList<T>::Back() const 
	{
		if (IsEmpty()) {throw std::runtime_error("Cannot access an empty list.");}
		return _back->_data;
	}

	template<typename T>
	inline const std::size_t SList<T>::Size() const 
	{
		return _size;
	}

	template<typename T>
	typename SList<T>::Iterator SList<T>::PushFront(const T& var)
	{
		Node* newFrontNode = new Node(var); //Creates a new node and initializes it.
		newFrontNode->_next = _front;
		_front = newFrontNode; //Set the front of the linked list to the new node
		if (IsEmpty()) {_back = newFrontNode;} //If this is the first 
		_size++;
		return(Iterator(*this, newFrontNode));
	}

	template<typename T>
	typename SList<T>::Iterator SList<T>::PushBack(const T& var)
	{
		Node* newNode = new Node(var);
		if (IsEmpty()){_front = _back = newNode;}
		else {_back = _back->_next = newNode;}
		_size++;
		return(Iterator(*this, newNode));
	}

	template<typename T>
	bool SList<T>::PopFront() 
	{
		bool popped = false;
		if (!IsEmpty())
		{
			if (_size == 1)
			{
				delete _front;
				_front = nullptr;
				_back = nullptr;
			}
			else {
				Node* newFrontNode = _front->_next;
				delete _front;
				_front = newFrontNode;
			}
			_size--;
			popped = true;
		}
		return popped;
	}

	template<typename T>
	bool SList<T>::PopBack() 
	{
		bool popped = false;
		if (!IsEmpty()) {
			if (_size == 1)
			{
				delete _back;
				_front = nullptr;
				_back = nullptr;
			}
			else {
				Node* traversalNode = _front;
				Node* savedNode = traversalNode;

				while (traversalNode->_next != nullptr) {
					savedNode = traversalNode;
					traversalNode = traversalNode->_next;
				}

				delete traversalNode;
				_back = savedNode;
				_back->_next = nullptr;
			}
			_size--;
			popped = true;
		}
		return popped;
	}

	template<typename T>
	void SList<T>::Clear() 
	{
		if (!IsEmpty()) {
			Node* traversalNode = _front;

			while (traversalNode != nullptr) {
				Node* previousNode = traversalNode;
				traversalNode = traversalNode->_next;
				delete previousNode;
			}

		}
		_front = nullptr;
		_back = nullptr;
		_size = 0;
	}

	template<typename T>
	inline bool SList<T>::IsEmpty() const
	{
		return (_size == 0);
	}

	//Operator Overload Copy Assignment
	template<typename T>
	SList<T>& SList<T>::operator=(const SList& right) 
	{
		if (this != &right) { //self assignment test
			Clear(); //removes contents of current list
			if (!right.IsEmpty()) { //If the right list is NOT empty then copy
				Node* firstNode = new Node((right._front)->_data);
				_front = firstNode;
				Node* previousNode = firstNode;
				Node* rightTraversalNode = right._front;

				while (rightTraversalNode->_next != nullptr) {
					rightTraversalNode = rightTraversalNode->_next;
					Node* newNode = new Node(rightTraversalNode->_data);
					previousNode->_next = newNode;
					previousNode = newNode;
				}

				_back = previousNode;
				_back->_next = nullptr;
				_size = right._size;
			}		
		}
		return *this;
	}

	template<typename T>
	SList<T>::~SList() 
	{
		if (_front != nullptr) {
			Node* traversalNode = _front;
			Node* previousNode = _front;

			while (traversalNode->_next != nullptr)
			{
				traversalNode = traversalNode->_next;
				delete previousNode;
				previousNode = traversalNode;
			}

			delete previousNode;
		}		
	}

	template<typename T>
	typename SList<T>::ConstIterator SList<T>::cbegin() const
	{
		return (ConstIterator(*this, _front));
	}

	template<typename T>
	typename SList<T>::Iterator SList<T>::begin()
	{
		return (Iterator(*this, _front));
	}

	template<typename T>
	typename SList<T>::ConstIterator SList<T>::begin() const
	{
		return (ConstIterator(*this, _front));
	}

	template<typename T>
	typename SList<T>::ConstIterator SList<T>::cend() const
	{
		return (ConstIterator(*this));
	}

	template<typename T>
	typename SList<T>::Iterator SList<T>::end()
	{
		return (Iterator(*this));
	}

	template<typename T>
	typename SList<T>::ConstIterator SList<T>::end() const
	{
		return (ConstIterator(*this));
	}

	template<typename T>
	template<typename EqualityFunctor>
	typename SList<T>::Iterator SList<T>::Find(const T& value)
	{
		EqualityFunctor equFunc{};
		Iterator it;

		for (it = begin(); it != end(); ++it)
		{
			if (equFunc(*it, value)) { break; }
		}

		return it;
	}
	template<typename T>
	template<typename EqualityFunctor>
	typename SList<T>::ConstIterator SList<T>::Find(const T& value) const
	{
		EqualityFunctor equFunc{};
		ConstIterator it;

		for (it = begin(); it != end(); ++it)
		{
			if (equFunc(*it, value)) { break; }
		}

		return it;
	}

	template<typename T>
	typename SList<T>::Iterator SList<T>::InsertAfter( const T& value, const Iterator& it)
	{
		if (it._owner == nullptr) { throw std::runtime_error("Cannot insert value into unassigned container."); } // Case where you are passing an unassigned list into the parameter
		
		if (it._node == nullptr || it._node->_next == nullptr) // Case where the list is empty or we have reached the end of the container
		{
			return PushBack(value); //Pushes the value onto the list and returns an iterator.
		}
		else
		{
			Node* newNode = new Node(value, it._node->_next);
			it._node->_next = newNode;
			++_size;
			return Iterator(*this, newNode);
		}
	}

	template<typename T>
	template<typename EqualityFunctor>
	bool SList<T>::Remove(const T& value)
	{
		EqualityFunctor equFunc{};

		bool isRemoved = false;
		if (!IsEmpty())
		{
			Find(value);

			Node* traversalNode = _front;
			Node* previouseNode = _front;

			while (traversalNode != nullptr) //Initial search for value to be removed
			{
				if (equFunc(traversalNode->_data, value)) {break;}
				previouseNode = traversalNode;
				traversalNode = traversalNode->_next;
			}

			if (traversalNode != nullptr) //Once search is complete we check if we have found the value.
			{
				if (traversalNode == _front && traversalNode == _back) //Only one item in the list
				{
					_front = _back = nullptr;
				}
				else if ((traversalNode == _back || traversalNode == _front) && _size == 2) //Only two items on the list
				{
					_front = _back = previouseNode;
					previouseNode->_next = nullptr;
				}
				else if (traversalNode == _front) //first item on the list
				{
					_front = traversalNode->_next;
				}
				else if (traversalNode == _back) //last item on the list
				{
					previouseNode->_next = nullptr;
					_back = previouseNode;
				}
				else
				{
					previouseNode->_next = traversalNode->_next;
				}
				delete traversalNode;
				--_size;
				isRemoved = true;
			}
		}
		return isRemoved;
	}
#pragma endregion

#pragma region SLIST NODE FUNCTIONS
	template<typename T>
	SList<T>::Node::Node(const T& data, Node* next) :
		_data(data),
		_next(next)
	{}

	template<typename T>
	template<typename... Args>
	SList<T>::Node::Node(Args&&... args, Node* next) :
		_data(args),
		_next(next)
	{}

#pragma endregion

#pragma region SLIST CONST-ITERATOR FUNCTIONS
	//Private constructor
	template<typename T>
	SList<T>::ConstIterator::ConstIterator(const SList& owner, const Node* node) :
		_owner(&owner),
		_node(node)
	{}

	//Typecast constructor
	template<typename T>
	SList<T>::ConstIterator::ConstIterator(const Iterator& right) :
		_owner(right._owner),
		_node(right._node)
	{}

	//OO Equals
	template<typename T>
	bool SList<T>::ConstIterator::operator==(const ConstIterator& right) const
	{
		return !(*this != right);
	}

	//OO NOT Equals
	template<typename T>
	bool SList<T>::ConstIterator::operator!=(const ConstIterator& right) const
	{
		return (_owner != right._owner || _node != right._node);
	}

	//Post Increment Operator
	template<typename T>
	typename SList<T>::ConstIterator SList<T>::ConstIterator::operator++(int)
	{
		ConstIterator Temp = *this;
		++* this; //calls prefix operator
		return Temp;
	}

	//Pre Increment Operator
	template<typename T>
	typename SList<T>::ConstIterator& SList<T>::ConstIterator::operator++() //We want access to
	{
		if (_owner == nullptr) { throw std::runtime_error("Cannot traverse unassigned container."); } //iterator is not assigned to a container.
		if (_node != nullptr) { _node = _node->_next; } //reached the end of the list
		return *this;
	}

	//OO Dereference Operator
	template<typename T>
	const T& SList<T>::ConstIterator::operator*() const
	{
		if (_owner == nullptr) { throw std::runtime_error("Cannot dereference unassigned container."); } //attempting to dereference an unassociated container.
		if (_node == nullptr) { throw std::runtime_error("Cannot dereference end of container."); } //reached the end of the list
		return _node->_data;
	}

#pragma endregion

#pragma region SLIST ITERATOR METHODS
	//private constructor
	template<typename T>
	SList<T>::Iterator::Iterator(const SList& owner, Node* node) :
		_owner(&owner),
		_node(node)
	{}

	//Operator Overload Equals
	template<typename T>
	bool SList<T>::Iterator::operator==(const Iterator& right) const
	{
		return !(operator!=(right));
	}

	//Operator Overload Not Equals
	template<typename T>
	bool SList<T>::Iterator::operator!=(const Iterator& right) const
	{
		return (_owner != right._owner || _node != right._node);
	}

	//Operator Overload Post Increment
	template<typename T>
	typename SList<T>::Iterator SList<T>::Iterator::operator++(int)
	{
		Iterator Temp = *this;
		++* this;
		return Temp;
	}

	//Operator Overload Pre Increment
	template<typename T>
	typename SList<T>::Iterator& SList<T>::Iterator::operator++()
	{
		if (_owner == nullptr) { throw std::runtime_error("Cannot traverse an unassociated container."); } 	//iterator is not assigned to a container.
		if (_node != nullptr) { _node = _node->_next; } //reached the end of the list
		return *this;
	}

	//Operator Overload Dereference
	template<typename T>
	T& SList<T>::Iterator::operator*() const
	{
		if (_owner == nullptr) { throw std::runtime_error("Cannot dereference unassigned container."); } //attempting to dereference an unassociated container.
		if (_node == nullptr) { throw std::runtime_error("Cannot dereference empty container or end of container."); } //attempting to dereference an empty container
		return _node->_data;
	}

	template<typename T>
	template <typename... Args>
	inline typename SList<T>::Iterator SList<T>::EmplaceFront(Args&&... args)
	{

		Node* newFrontNode = new Node(std::forward<Args>(args)...); //Creates a new node and initializes it.
		newFrontNode->_next = _front;
		_front = newFrontNode; //Set the front of the linked list to the new node
		if (IsEmpty()) { _back = newFrontNode; } //If this is the first 
		_size++;
		return(Iterator(*this, newFrontNode));

	}


#pragma endregion
}



