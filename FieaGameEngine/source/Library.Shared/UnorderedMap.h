#pragma once
#include <initializer_list>
#include <functional>
#include <stdexcept>

#include "SList.h"
#include "Vector.h"
#include "DefaultHash.h"

namespace FieaGameEngine
{
	template <typename TKey, typename TData>
	class UnorderedMap final
	{
	public:
		/// <summary>
		/// Forward declaration so that using directive can correctly scope variable types
		/// </summary>
		class Iterator;

		/// <summary>
		/// Forward declaration so that using directive can correctly scope variable types
		/// </summary>
		class ConstIterator;

		/// <summary>
		/// Used as a return value to confirm that a a key/pair is found in the map.
		/// </summary>
		using it_bool_pair = std::pair<Iterator, bool>;

		/// <summary>
		/// Used as a return value to confirm that a a key/pair is found in the map.
		/// </summary>
		using const_it_bool_pair = std::pair<ConstIterator, bool>;

		/// <summary>
		/// Used to set the basic integer size type
		/// </summary>
		using size_type = std::size_t;

		/// <summary>
		/// The key and data that is stored as a pair in the map.
		/// </summary>
		using pair_type = std::pair<const TKey, TData>;

		/// <summary>
		/// Sets the chain type for the map.
		/// </summary>
		using chain_type = SList<pair_type>;

		/// <summary>
		/// Iterator for the chain
		/// </summary>
		using chain_iterator = typename chain_type::Iterator;

		/// <summary>
		/// ConstIterator for the chain
		/// </summary>
		using const_chain_iterator = typename chain_type::ConstIterator;

		/// <summary>
		/// Bucket type for the map
		/// </summary>
		using bucket_type = Vector<chain_type>;

		/// <summary>
		/// Used to traverse a map using an interface.
		/// Iterator points to a mutable item in the map it is assigned too. 
		/// </summary>
		class Iterator
		{
		public:

			/// <summary>
			/// Required for private Iterator constructor to be called
			/// </summary>
			friend UnorderedMap;

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
			/// Are equal when Iterators refer to the same map AND the same index
			/// </summary>
			/// <returns>bool</returns>
			bool operator==(const Iterator& right) const;

			/// <summary>
			/// Operator Overload Is Not Equal
			/// Called by a Iterator to compare against another Iterator.
			/// Are NOT equal when Iterators don't refer to the same map
			/// Are NOT equal when Iterators refer the same map AND point to different indices
			/// </summary>
			/// <returns>bool</returns>
			bool operator!=(const Iterator& right) const;

			/// <summary>
			/// Operator Overload Pre Decrement.
			/// Used to increment the Iterator to traverse the map.
			/// </summary>
			/// <returns>Iterator&</returns>
			/// <exception cref="Cannot traverse an unassociated container.">Thrown when the Iterator is not assigned a container.</exception>
			Iterator& operator++();

			/// <summary>
			/// Operator Overload Post Decrement.
			/// Used to increment the Iterator to traverse the map.
			/// </summary>
			/// <returns>Iterator</returns>
			/// <exception cref="Cannot traverse an unassociated container.">Thrown when the Iterator is not assigned a container.</exception>
			Iterator operator++(int);

			/// <summary>
			/// Operator Overload Dereference
			/// Returns a mutable reference to the item the Iterator is pointing to.
			/// </summary>
			/// <returns>const T&</returns>
			/// <exception cref="Cannot dereference an unassociated container.">Thrown when the Iterator is not assigned a container.</exception>
			/// <exception cref="Cannot dereference an empty container.">Thrown when container is empty.</exception>
			pair_type& operator*();

		private:
			Iterator&  SetInnerIterator(const size_t bucket_index, const chain_iterator& it);
			Iterator(UnorderedMap& owner, const size_t bucket_index = 0);		
			UnorderedMap* _owner{ nullptr };
			size_type _bucket_index{ 0 };
			chain_iterator _chain_it;
		};

		/// <summary>
		/// Used to traverse a map using an interface.
		/// ConstIterator points to a non-mutable item in the vector it is assigned too. 
		/// </summary>
		class ConstIterator
		{
		public:

			/// <summary>
			/// Required for private ConstIterator constructor to be called
			/// </summary>
			friend UnorderedMap;

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
			///The owner and index will be the same.
			/// </summary>
			/// <param name="right">An Iterator that is to be cast to a ConstIterator</param>
			ConstIterator(const Iterator& right);

			/// <summary>
			/// Operator Overload Is Equal
			/// Called by a ConstIterator to compare against another ConstIterator.
			/// Are equal when Iterators refer to the same map AND the same index
			/// </summary>
			/// <returns>A bool indicating if the ConstIterators are equal</returns>
			bool operator==(const ConstIterator& right) const;

			/// <summary>
			/// Operator Overload Is Not Equal
			/// Called by a ConstIterator to compare against another ConstIterator.
			/// Are NOT equal when Iterators don't refer to the same map
			/// Are NOT equal when Iterators refer the same map AND point to different indices
			/// </summary>
			/// <param name="right">A ConstIterator that is to be cast to a ConstIterator</param>
			/// <returns>bool</returns>
			bool operator!=(const ConstIterator& right) const;

			/// <summary>
			/// Operator Overload Pre Decrement.
			/// Used to increment the ConstIterator to traverse the map.
			/// </summary>
			/// <returns>ConstIterator&</returns>
			/// <exception cref="Cannot traverse an unassociated container.">Thrown when the ConstIterator is not assigned a container.</exception>
			ConstIterator& operator++();

			/// <summary>
			/// Operator Overload Post Decrement.
			/// Used to increment the ConstIterator to traverse the map.
			/// </summary>
			/// <returns>ConstIterator</returns>
			/// <exception cref="Cannot traverse an unassociated container.">Thrown when the ConstIterator is not assigned a container.</exception>
			ConstIterator operator++(int);

			/// <summary>
			/// Operator Overload Dereference
			/// Returns a mutable reference to the item the ConstIterator is pointing to.
			/// </summary>
			/// <returns>const T&</returns>
			/// <exception cref="Cannot dereference an unassociated container.">Thrown when the ConstIterator is not assigned a container.</exception>
			/// <exception cref="Cannot dereference an empty container.">Thrown when container is empty.</exception>

			const pair_type& operator*() const;


		private:
			ConstIterator& SetInnerIterator(const size_t bucket_index, const const_chain_iterator& it);
			ConstIterator(const UnorderedMap& owner, const size_t bucket_index = 0);
			const UnorderedMap* _owner{ nullptr };
			size_type _bucket_index{ 0 }; //Keeps track of which bucket the ConstIterator is going through
			const_chain_iterator _chain_it;
		};
		
		/// <summary>
		/// Constructor with initializer list
		/// </summary>
		/// <param name="list">Elements to initaly populate the UnorderedMap with.</param>
		/// <param name="num_buckets">The number of buckets the map will be set to have</param>
		/// <param name="hashFunctor">A functor that a user can pass in to specify a way to hash values for the map.</param>
		/// <param name="equalityFunctor">A functor that a user can pass in to specify a way to equate values in the mapS</param>
		UnorderedMap(std::initializer_list<pair_type> list, const size_type num_buckets = 11, std::function<size_type(const TKey&)> hashFunctor = DefaultHash<TKey>(), std::function<bool(const TKey&, const TKey&)> equalityFunctor = std::equal_to<TKey>());

		/// <summary>
		/// Constructor
		/// </summary>
		/// <param name="num_buckets">The number of buckets the map will be set to have</param>
		/// <param name="hashFunctor">A functor that a user can pass in to specify a way to hash values for the map.</param>
		/// <param name="equalityFunctor">A functor that a user can pass in to specify a way to equate values in the mapS</param>
		UnorderedMap(const size_type num_buckets = 11, std::function<size_type(const TKey&)> hashFunctor = DefaultHash<TKey>(), std::function<bool(const TKey&, const TKey&)> equalityFunctor = std::equal_to<TKey>());

		/// <summary>
		/// Constructor
		/// Used for the case where the user only wants to specify the equalityFunctor
		/// </summary>
		/// <param name="equalityFunctor">A functor that a user can pass in to specify a way to equate values in the mapS</param>
		/// <param name="num_buckets">The number of buckets the map will be set to have</param>
		/// <param name="hashFunctor">A functor that a user can pass in to specify a way to hash values for the map.</param>
		UnorderedMap(std::function<bool(const TKey&, const TKey&)> equalityFunctor, const size_t num_buckets = 11, std::function<size_type(const TKey&)> hashFunctor = DefaultHash<TKey>());

		/// <summary>
		/// User declared AND compiler defined copy constructor
		/// </summary>
		UnorderedMap(UnorderedMap&) = default;

		/// <summary>
		/// User declared AND compiler defined assignment constructor
		/// </summary>
		UnorderedMap& operator=(UnorderedMap&) = default;

		/// <summary>
		/// User declared AND compiler defined move constructor
		/// </summary>
		UnorderedMap(UnorderedMap&&) = default;

		/// <summary>
		/// User declared AND compiler defined move assignment
		/// </summary>
		UnorderedMap& operator=(UnorderedMap&&) = default;

		/// <summary>
		/// User declared AND compiler defined destructor
		/// </summary>
		~UnorderedMap() = default;

		/// <summary>
		/// Inserts a pair into the map (TKey, TData). Returns a pair that contains an iterator and bool.
		/// Iterator returned in the pair contains reference to the pair inserted into the map.
		/// The bool returns true if the pair inserted already exists in the map.
		/// The bool returns false if the pair inserted does not exists in the map.
		/// </summary>
		/// <returns>a pair that has an iteraor and bool</returns>
		it_bool_pair Insert(const pair_type&);

		/// <summary>
		/// Finds pair that contains the key passed into the function
		/// If pair is found then the Iterator returned contains a reference to the pair.
		/// If pair is NOT found then the Iterator returned is equivalent to end.
		/// The bool returns true if the key inserted finds a pair.
		/// The bool returns false if the key inserted doesn't finds a pair.
		/// </summary>
		/// <param name="key">TKey type wich is what is used to hash and place items into buckets in the map</param>
		/// <returns>a pair that has an Iterator and bool</returns>
		it_bool_pair Find(const TKey& key);

		/// <summary>
		/// Finds pair that contains the key passed into the function
		/// If pair is found then the ConstIterator returned contains a reference to the pair.
		/// If pair is NOT found then the ConstIterator returned is equivalent to end.
		/// The bool returns true if the key inserted finds a pair.
		/// The bool returns false if the key inserted doesn't finds a pair.
		/// </summary>
		/// <param name="key">TKey type wich is what is used to hash and place items into buckets in the map</param>
		/// <returns>a pair that has a ConsIterator and bool</returns>
		const_it_bool_pair Find(const TKey&) const;

		/// <summary>
		/// Hashes they key passed in and determines which bucket to place the key into.
		/// </summary>
		/// <param name="key">TKey type wich is what is used to hash and place items into buckets in the map</param>
		/// <returns>A size_type bucket index of the map</returns>
		const size_type BucketPlacement(const TKey&) const;

		/// <summary>
		/// Returns the number of buckets in the map
		/// </summary>
		/// <returns>A size_type bucket index of the map</returns>
		size_type NumBuckets() const;

		/// <summary>
		/// OOBracketBracket
		/// </summary>
		/// <param name="key">TKey type wich is what is used to search for a item in the map</param>
		/// <returns>A reference to the stored item that is paired with the key</returns>
		/// <exception cref="Key not found in the map.">Thrown when the key is not found in the map.</exception>
		TData& operator[](const TKey& key);

		/// <summary>
		/// OOBracketBracket
		/// </summary>
		/// <param name="key">TKey type wich is what is used to search for a item in the map</param>
		/// <returns>A const reference to the stored item that is paired with the key</returns>
		const TData& operator[](const TKey& key) const;

		/// <summary>
		/// Removes the pair associated with the key passed.
		/// </summary>
		/// <param name="key">TKey type wich is what is used to search for a item in the map</param>
		void Remove(const TKey& key);

		/// <summary>
		/// Removes all items from the map.
		/// </summary>
		void Clear();

		/// <summary>
		/// returns the number of elements in the map.
		/// </summary>
		/// <returns>size_type value that is the number of items in the map</returns>
		size_type Size() const;

		/// <summary>
		/// Returns true if a pair with they key passed in is found
		/// Returns false if a pair is not found with the key passed in.
		/// </summary>
		/// <param name="key">TKey type wich is what is used to search for a item in the map</param>
		/// <returns>A bool if the key was found</returns>
		bool ContainsKey(const TKey& key);

		/// <summary>
		/// Returns the value associated with the key passed in.
		/// If the key is not found in map then an exception is thrown.
		/// </summary>
		/// <param name="key">TKey type wich is what is used to search for a item in the map</param>
		/// <returns>Returns a reference to the value associated with the key passed in.</returns>
		/// <exception cref="Key not found in the map.">Thrown when the key is not found in the map.</exception>
		TData& At(const TKey& key);

		/// <summary>
		/// Returns the value associated with the key passed in.
		/// If the key is not found in map then an exception is thrown.
		/// </summary>
		/// <param name="key">TKey type wich is what is used to search for a item in the map</param>
		/// <returns>Returns a const reference to the value associated with the key passed in.</returns>
		/// <exception cref="Key not found in the map.">Thrown when the key is not found in the map.</exception>
		const TData& At(const TKey& key) const;

		/// <summary>
		/// Returns a percentage value that represents the load factor of the map
		/// </summary>
		/// <returns>Returns a float type that reperesnets the load factor of the map as a percentage.</returns>
		float LoadFactorPercentage() const;

		/// <summary>
		/// Rehashes the map.
		/// </summary>
		/// <param name="new_size">Sets the new size, new number of buckets, of the map</param>
		void ReHashMap(const size_type new_size);

		/// <summary>
		/// Returns a Iterator that contains the first item in a map.
		/// Effectively the iterator will be empty but is assigned to a map.
		/// </summary>
		/// <typeparam name="T"></typeparam>
		Iterator begin();

		/// <summary>
		/// Returns a ConstIterator that contains the first item in a map.
		/// Effectively the iterator will be set to the front of the map.
		/// </summary>
		/// <typeparam name="T"></typeparam>
		ConstIterator begin() const;

		/// <summary>
		/// Use case is when a non-const map would like to assign itself a const iterator.
		/// Returns a ConstIterator that contains the first item in a map.
		/// Effectively the iterator will be set to the front of the map.
		/// </summary>
		/// <typeparam name="T"></typeparam>
		ConstIterator cbegin() const;

		/// <summary>
		/// Returns a Iterator that contains ONE PAST the last item in a map.
		/// Effectively the iterator will be empty but is assigned to a map.
		/// </summary>
		/// <typeparam name="T"></typeparam>
		Iterator end();

		/// <summary>
		/// Returns a ConstIterator that contains ONE PAST the last item in a map.
		/// Effectively the iterator will be empty but is assigned to a map.
		/// Used to traverse const lists
		/// </summary>
		/// <typeparam name="T"></typeparam>
		ConstIterator end() const;

		/// <summary>
		/// Use case is when a non-const map would like to assign itself a const iterator.
		/// Returns a ConstIterator that contains ONE PAST the last item in a map.
		/// Effectively the iterator will be empty but is assigned to a map.
		/// Used to traverse const lists
		/// </summary>
		/// <typeparam name="T"></typeparam>
		ConstIterator cend() const;

	private:
		size_type _size{ 0 };
		bucket_type _buckets;
		std::function<size_type(const TKey&)> _hashFunctor;
		std::function<bool(const TKey&, const TKey&)> _equalityFunctor;
	};

}

#include "UnorderedMap.inl"