#pragma once
#include <cstddef>
#include <initializer_list>
#include <stdexcept>

namespace FieaGameEngine
{
	/// <summary>
	/// A template class that implements a vector.
	/// </summary>
	/// <typeparam name="T"></typeparam>
	template <typename T>
	class Vector final
	{
	public:
		/// <summary>
		/// Member Types
		/// </summary>
		using size_type = std::size_t;
		using difference_type = std::ptrdiff_t;
		using value_type = T;
		using reference = T;
		using pointer = T*;
		using iterator_category = std::bidirectional_iterator_tag;

		/// <summary>
		/// Used to traverse a vector using an interface.
		/// Iterator points to a mutable item in the vector it is assigned too. 
		/// </summary>
		class Iterator
		{
		public:

			using size_type = std::size_t;
			using difference_type = std::ptrdiff_t;
			using value_type = T;
			using reference = T;
			using pointer = T*;
			using iterator_category = std::bidirectional_iterator_tag;

			/// <summary>
			/// Required for private Iterator constructor to be called
			/// </summary>
			friend Vector;

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
			/// Are equal when Iterators refer to the same vector AND index
			/// </summary>
			/// <returns> A bool that indicates to the user if the Iterators are equal.</returns>
			bool operator==(const Iterator& right) const;
			/// <summary>
			/// Operator Overload Is Not Equal
			/// Called by a Iterator to compare against another Iterator.
			/// Are NOT equal when Iterators don't refer to the same vector
			/// Are NOT equal when Iterators refer the same vector AND point to different indices
			/// </summary>
			/// <returns> A bool that indicates to the user if the Iterators are NOT equal.</returns>
			bool operator!=(const Iterator& right) const;

			/// <summary>
			/// Operator Overload Pre Increment.
			/// Used to increment the Iterator to traverse the vector.
			/// </summary>
			/// <returns>Iterator&</returns>
			Iterator& operator++();

			/// <summary>
			/// Operator Overload Pre Decrement.
			/// Used to decrement the Iterator to traverse the vector.
			/// </summary>
			/// <returns>Iterator&</returns>
			Iterator& operator--();

			/// <summary>
			/// Operator Overload Post Increment.
			/// Used to increment the Iterator to traverse the vector.
			/// </summary>
			/// <returns>A copy of itself before the increment.</returns>
			/// <exception cref="Cannot traverse unassigned container.">Thrown when the Iterator is not assigned a container.</exception>
			Iterator operator++(int);

			/// <summary>
			/// Operator Overload Post Decrement.
			/// Used to decrement the Iterator to traverse the vector.
			/// </summary>
			/// <returns>A copy of itself before the decrement.</returns>
			/// <exception cref="Cannot traverse unassigned container.">Thrown when the Iterator is not assigned a container.</exception>
			Iterator operator--(int); //TO DO: Added 4/20/2022


			/// <summary>
			/// Operator Overload Dereference
			/// Returns a mutable reference to the item the Iterator is pointing to.
			/// </summary>
			/// <returns>A mutable reference to the data pointed at by the Iterator</returns>
			/// <exception cref="Cannot dereference unassigned container.">Thrown when the Iterator is not assigned a container.</exception>
			/// <exception cref="Cannot dereference end of container.">Thrown when the Iterator is pointing to the end of the SList.</exception>
			T& operator*() const;

		private:
			Iterator(Vector& owner, const size_t index = 0);
			size_t _index { 0 };
			Vector* _owner{ nullptr };

		};
		/// <summary>
		/// Used to traverse a vector using an interface.
		/// ConstIterator points to an immutable item in the vector it is assigned.
		/// Though we can still update the iterator.
		/// (i.e., the iterator can be incremented or decremented but the item it points to can not be changed).
		/// </summary>
		class ConstIterator
		{
		public:

			using size_type = std::size_t;
			using difference_type = std::ptrdiff_t;
			using value_type = T;
			using reference = T;
			using pointer = T*;
			using iterator_category = std::bidirectional_iterator_tag;

			/// <summary>
			/// Required for private ConstIterator constructor to be called
			/// </summary>
			friend Vector;

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
			/// Are equal when ConstIterators refer to the same vector AND index.
			/// </summary>
			/// <param name="right">A ConstIterator and the subject from which contents will be copied to the invoking ConstIterator</param>
			/// <returns>A bool that indicates the user if the ConstIterator are equal.</returns>
			bool operator==(const ConstIterator& right) const;

			/// <summary>
			/// Operator Overload Is Not Equal
			/// Called by a ConstIterator to compare against another ConstIterator.
			/// Are NOT equal when ConstIterators don't refer to the same vector.
			/// Are NOT equal when ConstIterators refer the same vector AND point to different indices.
			/// </summary>
			/// <returns>A bool that indicates the user if the ConstIterator are equal.</returns>
			bool operator!=(const ConstIterator& right) const;

			/// <summary>
			/// Operator Overload Pre Increment.
			/// Used to increment the ConstIterator to traverse the vector.
			/// </summary>
			/// <returns>A ConstIterator& reference to itself after increment.</returns>
			/// <exception cref="Cannot traverse unassigned container.">Thrown when the Iterator is not assigned a container.</exception>
			ConstIterator& operator++();

			/// <summary>
			/// Operator Overload Pre Decrement.
			/// Used to decrement the ConstIterator to traverse the vector.
			/// </summary>
			/// <returns>A ConstIterator& reference to itself after increment.</returns>
			/// <exception cref="Cannot traverse unassigned container.">Thrown when the Iterator is not assigned a container.</exception>
			ConstIterator& operator--();

			/// <summary>
			/// Operator Overload Post Decrement.
			/// Used to increment the ConstIterator to traverse the vector.
			/// </summary>
			/// <returns>ConstIterator</returns>
			ConstIterator operator++(int);

			/// <summary>
			/// Operator Overload Post Decrement.
			/// Used to decrement the ConstIterator to traverse the vector.
			/// </summary>
			/// <returns>A ConstIterator copy of of itself before increment.</returns>
			/// <exception cref="Cannot traverse unassigned container.">Thrown when the Iterator is not assigned a container.</exception>
			ConstIterator operator--(int); //TO DO: Added 4/20/2022

			/// <summary>
			/// Operator Overload Dereference
			/// Returns a NON mutable reference to the item the ConstIterator is pointing to.
			/// </summary>
			/// <returns>A mutable reference to the data pointed at by the Iterator</returns>
			/// <exception cref="Cannot dereference unassigned container.">Thrown when the ConstIterator is not assigned a container.</exception>
			/// <exception cref="Cannot dereference end of container.">Thrown when the ConstIterator is pointing to the end of the SList.</exception>
			const T& operator*() const;

		private:
			ConstIterator(const Vector& owner, const size_t _index = 0);
			size_t _index{ 0 };
			const Vector* _owner{ nullptr };
		};

#pragma region VECTOR FUNCTION DECLARATIONS
		/// <summary>
		/// Constructor that allows the user to pass in the capacity of the Vector at initialization
		/// </summary>
		/// <param name="capacity">User passed in capacity siz. Defaulted to 0 if not specified.</param>
		explicit Vector(size_t capacity = 0);

		/// <summary>
		/// Constructor that can take an initializer list.
		/// </summary>
		/// <typeparam name="T,T,T,...">seperated T items by coma</typeparam>
		Vector(std::initializer_list<T> list);

		/// <summary>
		/// Copy constructor that initializes the Vector<T> object with another Vector<T> object.
		/// Calls IsEmpty() on the passed in Vector<T> object to check if the vector is empty. If copying an empty Vector<T> then the function calls Clear().
		/// If copying a Vector<T> that is NOT empty then then the function traverses the passed Vector<T> objects contents. 
		/// The calling Vector<T> calls malloc to make space equal to the that of the passed in Vector<T>. Iterates through the passed in vector and copies it.
		/// </summary>
		/// <param name="right">The Vector<T> object that is used to initialize the calling Vector<T> object.</param>
		Vector(const Vector& right);

		/// <summary>
		/// Move Constructor
		/// Will take in a Vector<T> r-value and pass on it's contents to the calling Vector<T> object.
		/// Passes control of memory to calling object.
		/// Sets the passed in r-value Vector<T> pointers to nullptr and zeros out values.
		/// </summary>
		/// <param name="right"> and rvalue reference to Vector<T> object</param>
		Vector(Vector&& right) noexcept;

		~Vector();
		/// <summary>
		/// Removes the last item in the vector.
		/// </summary>
		/// <returns>void</returns>
		void PopBack();

		/// <summary>
		/// Returns the item in the vector at the passed in index.
		/// </summary>
		/// <param name="index"></param>
		/// <returns>A reference to item T in the vector</returns>
		T& At(const int& index);

		/// <summary>
		/// Returns the item in the vector at the passed in index.
		/// </summary>
		/// <param name="index"></param>
		/// <returns>A const reference to item T in the vector</returns>
		const T& At(const int& index) const;

		/// <summary>
		/// Operator Overload Bracket Bracket
		/// Returns the item in the vector at the passed in index.
		/// </summary>
		/// <param name="index"></param>
		/// <returns>A reference to item T in the vector</returns>
		T& operator[](const size_t& index);

		/// <summary>
		/// Operator Overload Bracket Bracket
		/// Returns the item in the vector at the passed in index.
		/// </summary>
		/// <param name="index"></param>
		/// <returns>A const reference to item T in the vector</returns>
		const T& operator[](const size_t& index) const;

		/// <summary>
		/// Returns true if the Vector<T> vector is empty. Otherwise returns false.
		/// </summary>
		/// <returns>bool</returns>
		const bool IsEmpty() const;

		/// <summary>
		/// Calls IsEmpty() to check if vector is currently empty.
		/// If empty then the function throws an exception.
		/// If NOT empty then the returns the item at the first index of the vector
		/// </summary>
		/// <returns>Returns a reference to the first item in the vector.</returns>
		T& Front();

		/// <summary>
		/// If empty then the function throws an exception.
		/// If NOT empty then the returns the item at the first index of the vector
		/// </summary>
		/// <returns>Returns a reference to the last item in the vector.</returns>
		T& Back();

		/// <summary>
		/// Calls IsEmpty() to check if vector is currently empty.
		/// If empty then the function throws an exception.
		/// If NOT empty then the returns the item at the first index of the vector
		/// </summary>
		/// <returns>Returns a const reference to the first item in the vector.</returns>
		const T& Front() const;

		/// <summary>
		/// If empty then the function throws an exception.
		/// If NOT empty then the returns the item at the first index of the vector
		/// </summary>
		/// <returns>Returns a const reference to the last item in the vector.</returns>
		const T& Back() const;

		/// <summary>
		/// Returns the size of the vector.
		/// </summary>
		/// <returns>Returns a copy of the size of the vector.</returns>
		const std::size_t Size() const;

		/// <summary>
		/// Returns the capacity of the vector.
		/// </summary>
		/// <returns>Returns a copy of the capacity of the vector.</returns>
		const std::size_t Capacity() const;

		/// <summary>
		/// Increases size by 1 and adds the item to the back of the vector.
		/// If size is equal to capacity then this calls Reserve which increases the capacity of the vector by a factor of 2.
		/// </summary>
		/// <param name="var">An object that is set as the last item in the vector.</param>
		void PushBack(const T& var);

		/// <summary>
		/// PushBack that uses move semantics.
		/// </summary>
		/// <param name="var">R value that will be pushedback using move semantics</param>
		/// <returns>void</returns>
		void PushBack(T&& var) noexcept;

		/// <summary>
		/// Constructs a new element in the list by passing the elements parameter(s).
		/// </summary>
		/// <typeparam name="...Args">Template parameter pack.</typeparam>
		/// <param name="...args">Parameter elements to construct a new element on the stack.</param>
		template<typename... Args>
		void EmplaceBack(Args&&... args);


		/// <summary>
		/// Overloading the assignment operator so that a Vector<T> object can be assigned to another Vector<T> object.
		/// </summary>
		/// <param name="right">The Vector<T> object that data is being copied from.</param>
		/// <returns>A reference to itself.</returns>
		Vector<T>& operator=(const Vector&);

		/// <summary>
		/// Move Assignment
		/// Will take in a Vector<T> r-value and pass on it's contents to the calling Vector<T> object.
		/// Passes control of memory to calling object.
		/// Sets the passed in r-value Vector<T> pointers to nullptr and zeros out values.
		/// </summary>
		/// <param name="rhs"> and rvalue reference to Vector<T> object</param>
		/// <returns>A reference to itself</returns>
		Vector<T>& operator=(Vector&& rhs) noexcept;

		/// <summary>
		/// Clears all the items in the vector while properly clearing up memory. Leaves the vector empty.
		/// </summary>
		void Clear();

		/// <summary>
		/// Set the capacity of the vector.
		/// throws an exception when passed in value is smaller than the current size of the vector.
		/// </summary>
		/// <param name="num">the new capacity to be set for the vector</param>
		/// <returns>bool</returns>
		const bool Reserve(const size_t& capacity);

		/// <summary>
		/// Resizes the
		/// </summary>
		/// <param name="num"></param>
		/// <returns></returns>
		const bool Resize(const size_t& capacity);

		/// <summary>
		/// Erases the specified elements from the container and thus the vector.
		/// </summary>
		/// <param name="It">The iterator whose contents will be removed from the vector</param>
		/// <returns>bool</returns>
		const bool Remove(const Iterator& it);

		/// Removes an element at a certain index in the Datum.
		/// </summary>
		/// <param name="index">index where element will be removed from</param>
		void RemoveAt(size_t index); //REVIEW : ADDED 3.18.2022


		/// <summary>
		/// Removes the elements in the range [first, last). This is inclusive.
		/// </summary>
		/// <param name="It1">The first item in the vector to be delted.</param>
		/// <param name="It2">The last item in the vector to be deleted.</param>
		/// <returns>bool</returns>
		const bool Remove(const Iterator& it1, const Iterator& it2);
		/// <summary>
		/// Removes any excess capacity the vector has and resizes it to the exact size of the items in the vector.
		/// </summary>
		/// <returns>void</returns>
		void ShrinkToFit();
		/// <summary>
		/// Returns a Iterator that contains the first item in a vector.
		/// Effectively the iterator will be empty but is assigned to a vector.
		/// </summary>
		/// <typeparam name="T"></typeparam>
		Iterator begin();

		/// <summary>
		/// Returns a ConstIterator that contains the first item in a vector.
		/// Effectively the iterator will be set to the front of the vector.
		/// </summary>
		/// <typeparam name="T"></typeparam>
		ConstIterator begin() const;

		/// <summary>
		/// Use case is when a non-const vector would like to assign itself a const iterator.
		/// Returns a ConstIterator that contains the first item in a vector.
		/// Effectively the iterator will be set to the front of the vector.
		/// </summary>
		/// <typeparam name="T"></typeparam>
		ConstIterator cbegin() const;

		/// <summary>
		/// Use case is when a non-const vector would like to assign itself a const iterator.
		/// Returns a ConstIterator that contains ONE PAST the last item in a vector.
		/// Effectively the iterator will be empty but is assigned to a vector.
		/// Used to traverse const lists
		/// </summary>
		/// <typeparam name="T"></typeparam>
		ConstIterator cend() const;

		/// <summary>
		/// Returns a Iterator that contains ONE PAST the last item in a vector.
		/// Effectively the iterator will be empty but is assigned to a vector.
		/// </summary>
		/// <typeparam name="T"></typeparam>
		Iterator end();

		/// <summary>
		/// Returns a ConstIterator that contains ONE PAST the last item in a vector.
		/// Effectively the iterator will be empty but is assigned to a vector.
		/// Used to traverse const lists
		/// </summary>
		/// <typeparam name="T"></typeparam>
		ConstIterator end() const;

		/// <summary>
		/// Finds the first value in the vector that is passed into the function and returns a Iterator to it.
		/// Can be passed a Functor when called to use unique equivalence determinants for abstract data types.
		/// </summary>
		/// <returns>Iterator</returns>
		template<typename EqualityFunctor = std::equal_to<T>>
		Iterator Find(const T& value);

		/// <summary>
		/// Finds the first value in the vector that is passed into the function and returns a ConstIterator to it.
		/// Can be passed a Functor when called to use unique equivalence determinants for abstract data types.
		/// </summary>
		/// <returns>ConstIterator</returns>
		template<typename EqualityFunctor = std::equal_to<T>>
		ConstIterator Find(const T& value) const;


		//template<class ForwardIt, class UnaryPredicate>
		//ForwardIt partition(ForwardIt first, ForwardIt last, UnaryPredicate p)
		//{
		//	first = std::find_if_not(first, last, p);
		//	if (first == last) return first;

		//	for (ForwardIt i = std::next(first); i != last; ++i) {
		//		if (p(*i)) {
		//			std::iter_swap(i, first);
		//			++first;
		//		}
		//	}
		//	return first;
		//}

#pragma endregion

	private:

		std::size_t _size{ 0 };
		std::size_t _capacity{ 0 };
		T* _array{ nullptr };

	};

}

#include "Vector.inl"