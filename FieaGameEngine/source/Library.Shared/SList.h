#pragma once
#include <cstddef>
#include <initializer_list>
#include <stdexcept>
namespace FieaGameEngine
{
	/// <summary>
	/// A template class that implements a singly-linked list.
	/// </summary>
	/// <typeparam name="T"></typeparam>
	template <typename T>
	class SList final
	{
	private:
		/// <summary>
		/// Defines what "one-link" of the list is comprised of. 
		/// It is a container for one item in the list of type T and also has a pointer that links it to the next Node in the list.
		/// </summary>
		struct Node final
		{
			T _data;
			Node* _next;
			template<typename... Args>
			Node(Args&&... args, Node* next = nullptr);

			Node(const T& data, Node* next = nullptr);

		};
	public:
		/// <summary>
		/// Used to traverse a list using an interface.
		/// Iterator points to a mutable item in the list it is assigned too. 
		/// </summary>
		class Iterator
		{
		public:
			/// <summary>
			/// Required for private Iterator constructor to be called
			/// </summary>
			friend SList;
			/// <summary>
			/// User declared AND compiler defined default constructor
			/// </summary>
			Iterator() = default;
			/// <summary>
			/// User declared AND compiler defined destructor
			/// </summary>
			~Iterator() = default;
			/// <summary>
			/// User declared AND compiler defined copy constructor
			/// </summary>
			Iterator(const Iterator&) = default;
			/// <summary>
			/// User declared AND compiler defined copy assignment
			/// </summary>
			Iterator& operator=(const Iterator&) = default;
			/// <summary>
			/// User declared AND compiler defined move constructor
			/// </summary>
			Iterator(Iterator&&) = default;
			/// <summary>
			/// User declared AND compiler defined move assignment
			/// </summary>
			Iterator& operator=(Iterator&&) = default;
			/// <summary>
			/// Operator Overload Is Equal
			/// Called by a Iterator to compare against another Iterator.
			/// Are equal when Iterators refer to the same list AND point to the same node
			/// </summary>
			/// <returns> A bool that indicates to the user if the Iterators are equal.</returns>
			bool operator==(const Iterator& right) const;
			/// <summary>
			/// Operator Overload Is Not Equal
			/// Called by a Iterator to compare against another Iterator.
			/// Are NOT equal when Iterators don't refer to the same list
			/// Are NOT equal when Iterators refer the same list AND point to different nodes
			/// </summary>
			/// <returns> A bool that indicates to the user if the Iterators are NOT equal.</returns>
			bool operator!=(const Iterator& right) const;
			/// <summary>
			/// Operator Overload Pre Increment.
			/// Used to increment the Iterator to traverse the list.
			/// </summary>
			/// <returns>A Iterator& reference to itself after the increment.</returns>
			/// <exception cref="Cannot traverse unassigned container.">Thrown when the Iterator is not assigned a container.</exception>
			Iterator& operator++();
			/// <summary>
			/// Operator Overload Post Increment.
			/// Used to increment the Iterator to traverse the list.
			/// </summary>
			/// <returns>A copy of itself before the increment.</returns>
			/// <exception cref="Cannot traverse unassigned container.">Thrown when the Iterator is not assigned a container.</exception>
			Iterator operator++(int);
			/// <summary>
			/// Operator Overload Dereference
			/// Returns a mutable reference to the item the Iterator is pointing to.
			/// </summary>
			/// <returns>A mutable reference to the data pointed at by the Iterator</returns>
			/// <exception cref="Cannot dereference unassigned container.">Thrown when the Iterator is not assigned a container.</exception>
			/// <exception cref="Cannot dereference end of container.">Thrown when the Iterator is pointing to the end of the SList.</exception>
			T& operator*() const;
		private:
			Iterator(const SList& owner, Node* node = nullptr);
			Node* _node{ nullptr };
			const SList* _owner{ nullptr };
		};
		/// <summary>
		/// Used to traverse a list using an interface.
		/// ConstIterator points to an immutable item in the list it is assigned.
		/// Though we can still update the iterator.
		/// (i.e., the iterator can be incremented or decremented but the item it points to can not be changed).
		/// </summary>
		class ConstIterator
		{
		public:
			/// <summary>
			/// Required for private ConstIterator constructor to be called
			/// </summary>
			friend SList;
			/// <summary>
			/// User declared AND compiler defined default constructor
			/// </summary>
			ConstIterator() = default;
			/// <summary>
			/// User declared AND compiler defined destructor
			/// </summary>
			~ConstIterator() = default;
			/// <summary>
			/// User declared AND compiler defined copy constructor
			/// </summary>
			ConstIterator(const ConstIterator&) = default;
			/// <summary>
			/// User declared AND compiler defined copy assignment
			/// </summary>
			ConstIterator& operator=(const ConstIterator&) = default;
			/// <summary>
			/// User declared AND compiler defined move constructor
			/// </summary>
			ConstIterator(ConstIterator&&) = default;
			/// <summary>
			/// User declared AND compiler defined move assignment
			/// </summary>
			ConstIterator& operator=(ConstIterator&&) = default;						
			/// <summary>
			/// Typecast Constructor
			/// Takes in an Iterator type and casts it into a ConstIterator.
			/// The owner and node will be the same.
			/// </summary>
			/// <param name="right">An Iterator that is to be cast to a ConstIterator</param>
			ConstIterator(const Iterator& right);
			/// <summary>
			/// Operator Overload Is Not Equal
			/// Called by a ConstIterator to compare against another ConstIterator.
			/// Are equal when ConstIterators refer to the same list AND node
			/// </summary>
			/// <param name="right">A ConstIterator and the subject from which contents will be copied to the invoking ConstIterator</param>
			/// <returns>A bool that indicates the user if the ConstIterator are equal.</returns>
			bool operator==(const ConstIterator& right) const;
			/// <summary>
			/// Operator Overload Is Not Equal
			/// Called by a ConstIterator to compare against another ConstIterator.
			/// Are NOT equal when ConstIterators don't refer to the same list
			/// Are NOT equal when ConstIterators refer the same list AND different nodes
			/// </summary>
			/// <param name="right">A ConstIterator whose contents will be compared against the invoking ConstIterator</param>
			/// <returns>A bool that indicates the user if the ConstIterator are NOT equal.</returns>
			bool operator!=(const ConstIterator& right) const;
			/// <summary>
			/// Operator Overload Pre Increment.
			/// Used to increment the ConstIterator to traverse the list.
			/// </summary>
			/// <returns>A ConstIterator& reference to itself after increment.</returns>
			/// <exception cref="Cannot traverse unassigned container.">Thrown when the Iterator is not assigned a container.</exception>
			ConstIterator& operator++();
			/// <summary>
			/// Operator Overload Post Increment.
			/// Used to increment the ConstIterator to traverse the list.
			/// </summary>
			/// <returns>A ConstIterator copy of of itself before increment.</returns>
			/// <exception cref="Cannot traverse unassigned container.">Thrown when the Iterator is not assigned a container.</exception>
			ConstIterator operator++(int);
			/// <summary>
			/// Operator Overload Dereference
			/// Used to access the contents the ConstIterator points to in the container it's assigned.
			/// <returns>A const reference to the data pointed at by the ConstIterator</returns>
			/// <exception cref="Cannot dereference unassigned container.">Thrown when the Iterator is not assigned a container.</exception>
			/// <exception cref="Cannot dereference end of container.">Thrown when the Iterator is pointing to the end of the SList.</exception>
			const T& operator*() const;
		private:

			ConstIterator(const SList& owner, const Node* node = nullptr);

			const Node* _node{ nullptr };
			const SList* _owner{ nullptr };
		};
#pragma region SLIST FUNCTION DECLARATIONS
		/// <summary>
		/// Finds the first value in the list that is passed into the function and returns a Iterator to it.
		/// Can be passed a Functor when called to use unique equivalence determinants for abstract data types.
		/// </summary>
		/// <template param name="EqualityFunctor">A functor used to equate between to items in the list</param>
		/// <param name="value">The value that we are searching for in the list</param>
		/// <\returns>A Iterator that points to the data found in the list</returns>
		template<typename EqualityFunctor = std::equal_to<T>>
		Iterator Find(const T& value);
		/// <summary>
		/// Finds the first value in the list that is passed into the function and returns a ConstIterator to it.
		/// Can be passed a Functor when called to use unique equivalence determinants for abstract data types.
		/// </summary>
		/// <template param name="EqualityFunctor">A functor used to equate between to items in the list</param>
		/// <param name="value">The value that we are searching for in the list</param>
		/// <\returns>A Iterator that points to the data found in the list</returns>
		template<typename EqualityFunctor = std::equal_to<T>>
		ConstIterator Find(const T& value) const;
		/// <summary>
		/// Inserts a value into a list. 
		/// The value will placed in the position after the iterators position in the list.
		/// </summary>
		/// <param name="value">The value to be placed in the list.</param>
		/// <param name="it">The iterators pointed value will be used as the position from which to insert the value after.</param>
		/// <returns>void</returns>				
		/// <exception cref="Cannot insert value into unassigned container.">Thrown when the Iterator passed into the parameter of the function is not assigned a container.</exception>
		Iterator InsertAfter(const T& value, const Iterator& it);
		/// <summary>
		/// Removes the first value found in a list whose value is equal to that of the passed in parameter. 
		/// Starts search from the front of the list.
		/// If the item is found then the item is deleted and list is linked appropriately.
		/// If no item is found then the list is left unchanged
		/// </summary>
		/// <typeparam name="EqualityFunctor">Used to compare items in the list to match the value.</typeparam>
		/// <param name="value">The value to be deleted from the list.</param>
		/// <returns>
		/// A bool indicating if the value was removed.
		/// If return value is false that means the value was either not found.
		/// If return value is true that means the value was found and removed.
		/// </returns>
		template<typename EqualityFunctor = std::equal_to<T>>
		bool Remove(const T& value);
		/// <summary>
		/// Used when a non-const list would like to assign itself a ConsIterator.
		/// ConstIterator will be set to the front of the list.
		/// </summary>
		/// <returns>A ConstIterator that is set to the beginning of the list.</returns>
		ConstIterator cbegin() const;
		/// <summary>
		/// Used when a non-const list would like to assign itself a Iterator
		/// Iterator will be set to the front of the list.
		/// </summary>
		/// <returns>A Iterator that is set to the beginning of the list.</returns>
		Iterator begin();
		/// <summary>
		/// Used when a const list would like to assign itself a ConstIterator
		/// ConstIterator will be set to the front of the list.
		/// </summary>
		/// <returns>A ConstIterator that is set to the beginning of the list.</returns>
		ConstIterator begin() const;
		/// <summary>
		/// Used when a non-const list would like to assign itself a ConstIterator
		/// ConstIterator will be set to the end of the list.
		/// </summary>
		/// <returns>A ConstIterator that is set to the end of the list.</returns>
		ConstIterator cend() const;
		/// <summary>
		/// Used when a non-const list would like to assign itself a ConstIterator
		/// ConstIterator will be set to the end of the list.
		/// </summary>
		/// <returns>A Iterator that is set to the end of the list.</returns>
		Iterator end();
		/// <summary>
		/// Used when a const list would like to assign itself a ConstIterator
		/// ConstIterator will be set to the end of the list.
		/// </summary>
		/// <returns>A ConstIterator that is set to the end of the list.</returns>
		ConstIterator end() const;
		/// <summary>
		/// User declared AND compiler defined default constructor
		/// </summary>
		SList() = default;
		/// <summary>
		/// Constructor that can take an initializer list.
		/// </summary>
		/// <typeparam name="T,T,T,...">seperated T items by coma</typeparam>
		SList(std::initializer_list<T> list);
		/// <summary>
		/// Copy constructor that initializes the SList object with another SList object.
		/// Calls IsEmpty() on the passed in SList object to check if list is empty. If copying an empty SList then the function calls Clear().
		/// If copying an SList that is NOT empty then then the function traverses the passed SList objects contents. 
		/// The calling SList creates nodes for every Node in the passed in SList object and deep copies them into respective Node member variable Data.
		/// </summary>
		/// <param name="right">The SList object that is used to initialize the calling SList object.</param>
		SList(const SList& right);
		/// <summary>
		/// Returns the data found in the node of the list.
		/// </summary>
		/// <returns>A mutable reference to the first item in the list.</returns>
		/// <exception cref="Cannot access an empty list.">Thrown when calling this function from an empty SList</exception>
		T& Front();
		/// <summary>
		/// Returns the data found in the last node of the list.
		/// </summary>
		/// <returns>A mutable reference to the last item in the list.</returns>
		/// <exception cref="Cannot access an empty list.">Thrown when calling this function from an empty SList</exception>
		T& Back();
		/// <summary>
		/// Returns the data found in the node of the list.
		/// </summary>
		/// <returns>A const reference to the first item in the list.</returns>
		/// <exception cref="Cannot access an empty list.">Thrown when calling this function from an empty SList</exception>
		const T& Front() const;
		/// <summary>
		/// Returns the data found in the last node of the list.
		/// </summary>
		/// <returns>A const reference to the last item in the list.</returns>
		/// <exception cref="Cannot access an empty list.">Thrown when calling this function from an empty SList</exception>
		const T& Back() const;
		/// <summary>
		/// Returns the size of the list.
		/// </summary>
		/// <returns>Returns a const copy of the size of the list.</returns>
		const std::size_t Size() const;
		/// <summary>
		/// Pushes the value passed in to the front of the SList. 
		/// </summary>
		/// <param name="var">An object that is set as the first link of the list.</param>
		/// <returns>A Iterator that points to the data pushed into the SList.</returns>
		Iterator PushFront(const T& var);
		/// <summary>
		/// Pushes the value passed in to the back of the SList. 
		/// </summary>
		/// <param name="var">An object that is set as the last link of the list.</param>
		/// <returns>A Iterator that points to the data pushed into the SList.</returns>
		Iterator PushBack(const T& var);
		/// <summary>
		/// Removes the first element in the list.
		/// </summary>
		/// <returns>
		/// A bool that informs the user if an item was popped from the list.
		/// If false then the item was not popped. Check if list is empty before calling this function.
		/// If true then the item was popped.
		/// </returns>
		bool PopFront();
		/// <summary>
		/// Removes the last element in the list.
		/// </summary>
		/// <returns>
		/// A bool that informs the user if an item was popped from the list.
		/// If false then the item was not popped. Check if list is empty before calling this function.
		/// If true then the item was popped.
		/// </returns>
		bool PopBack();
		/// <summary>
		/// Clears all the Nodes in the list while properly clearing up memory. Leaves the list empty.
		/// </summary>
		/// <returns>void</returns>
		void Clear();
		/// <summary>
		/// Checks if the list is empty.
		/// </summary>
		/// <returns>A bool that if true means the list is empty. Else the list is not empty</returns>
		bool IsEmpty() const;

		/// <summary>
		/// Constructs a new element in the list by passing the elements parameter(s).
		/// </summary>
		/// <typeparam name="...Args">Template parameter pack.</typeparam>
		/// <param name="...args">Parameter elements to construct a new element on the stack.</param>
		template<typename... Args>
		SList<T>::Iterator EmplaceFront(Args&&... args);

		/// <summary>
		/// Operator Overload Assignment
		/// Copies the contents of one SList object to another.
		/// </summary>
		/// <param name="right">The SList object that data is being copied from.</param>
		/// <returns>A mutable reference to itself.</returns>
		SList& operator=(const SList& right);
		//Destructor
		~SList();
#pragma endregion
	private:
		std::size_t _size{ 0 };
		Node* _front{ nullptr };
		Node* _back{ nullptr };
	};
}

#include "SList.inl"