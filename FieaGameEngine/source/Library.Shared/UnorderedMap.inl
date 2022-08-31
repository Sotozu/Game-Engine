#include "UnorderedMap.h"


/*
For non fully specialized function templates, i.e.ones that carry at least one unknown type, you can omit inline, and not receive errors.
But still they are not inline.
For full specializations, i.e.ones that use only known types, you cannot omit it.
*/
namespace FieaGameEngine
{
#pragma region UNORDERED MAP FUNCTIONS
	template<typename TKey, typename TData> 		//TO DO . ADDED 04/06/2022

UnorderedMap<TKey, TData>::UnorderedMap(std::initializer_list<pair_type> list, const size_type num_buckets, std::function<size_type(const TKey&)> hashFunctor, std::function<bool(const TKey&, const TKey&)> equalityFunctor) :
_hashFunctor(hashFunctor),
_equalityFunctor(equalityFunctor)
{
	_buckets.Resize(num_buckets);
	for (const auto& value : list)
	{
		Insert(value);
	}
}


//Constructor Defaulted Parameters
template<typename TKey, typename TData>
UnorderedMap<TKey, TData>::UnorderedMap(const size_type num_buckets, std::function<size_t(const TKey&)> hashFunctor, std::function<bool(const TKey&, const TKey&)> equalityFunctor) :
	_hashFunctor(hashFunctor),
	_equalityFunctor(equalityFunctor)
{
	_buckets.Resize(num_buckets);
}

//Constructor Equality Functor Specification
template<typename TKey, typename TData>
UnorderedMap<TKey, TData>::UnorderedMap(std::function<bool(const TKey&, const TKey&)> equalityFunctor, const size_t num_buckets, std::function<size_t(const TKey&)> hashFunctor) :
	_hashFunctor(hashFunctor),
	_equalityFunctor(equalityFunctor)
{
	_buckets.Resize(num_buckets);
}


template<typename TKey, typename TData>
typename UnorderedMap<TKey, TData>::size_type UnorderedMap<TKey, TData>::Size() const
{
	return _size;
}

template<typename TKey, typename TData>
typename UnorderedMap<TKey, TData>::size_type UnorderedMap<TKey, TData>::NumBuckets() const
{
	return _buckets.Size();
}

template<typename TKey, typename TData>
typename UnorderedMap<TKey, TData>::it_bool_pair UnorderedMap<TKey, TData>::Insert(const pair_type& pair)
{
	TKey key = pair.first;
	const size_t bucket = BucketPlacement(key);
	it_bool_pair itBoolPair = Find(key);

	if (itBoolPair.second == false)
	{
		(itBoolPair.first).SetInnerIterator(bucket, _buckets[bucket].PushBack(pair));
		++_size;
	}
	
	return itBoolPair;
}

template<typename TKey, typename TData>
typename UnorderedMap<TKey, TData>::it_bool_pair UnorderedMap<TKey, TData>::Find(const TKey& key)
{
	size_type bucket = BucketPlacement(key);
	chain_iterator chainIt = _buckets[bucket].begin();

	Iterator it = end();
	bool isFound = false;
	while (chainIt != _buckets[bucket].end())
	{
		if (_equalityFunctor((*chainIt).first, key))
		{
			it.SetInnerIterator(bucket, chainIt);
			isFound = true;
			break;
		}
		++chainIt;
	}

	it_bool_pair itBoolPair(it, isFound);

	return itBoolPair;

}

template<typename TKey, typename TData>
typename UnorderedMap<TKey, TData>::const_it_bool_pair UnorderedMap<TKey, TData>::Find(const TKey& key) const
{

	size_type bucket = BucketPlacement(key);
	const_chain_iterator constChainIt = _buckets[bucket].begin();

	ConstIterator it = end();
	bool isFound = false;
	while (constChainIt != _buckets[bucket].end())
	{
		if (_equalityFunctor((*constChainIt).first, key))
		{
			it.SetInnerIterator(bucket, constChainIt);
			isFound = true;
			break;
		}
		++constChainIt;
	}

	const_it_bool_pair itBoolPair(it, isFound);

	return itBoolPair;
}

template<typename TKey, typename TData>
typename UnorderedMap<TKey, TData>::Iterator UnorderedMap<TKey, TData>::begin()
{
	Iterator it(*this, 0);
	chain_iterator chainIt = _buckets[0].begin();
	size_type bucket_index = 0;
	
	for (bucket_index; bucket_index < _buckets.Size(); ++bucket_index)
	{
		chainIt = _buckets[bucket_index].begin();
		if (chainIt != _buckets[bucket_index].end())
		{
			break;
		}
	}
	it.SetInnerIterator(bucket_index, chainIt);
	return it;
}

template<typename TKey, typename TData>
typename UnorderedMap<TKey, TData>::ConstIterator UnorderedMap<TKey, TData>::begin() const
{
	ConstIterator constIt(*this, 0);
	const_chain_iterator constChainIt = _buckets[0].begin();
	size_type bucket_index = 0;

	for (bucket_index; bucket_index < _buckets.Size(); ++bucket_index)
	{
		constChainIt = _buckets[bucket_index].begin();
		if (constChainIt != _buckets[bucket_index].end())
		{
			break;
		}
	}
	constIt.SetInnerIterator(bucket_index, constChainIt);
	return constIt;
}

template<typename TKey, typename TData>
typename UnorderedMap<TKey, TData>::ConstIterator UnorderedMap<TKey, TData>::cbegin() const
{
	ConstIterator it(*this, 0);
	chain_iterator chainIt = _buckets[0].begin();
	size_type bucket_index = 0;

	for (bucket_index; bucket_index < _buckets.Size(); ++bucket_index)
	{
		chainIt = _buckets[bucket_index].begin();
		if (chainIt != _buckets[bucket_index].end())
		{
			break;
		}
	}
	it.SetInnerIterator(bucket_index, chainIt);
	return it;
}

template<typename TKey, typename TData>
typename UnorderedMap<TKey, TData>::Iterator UnorderedMap<TKey, TData>::end()
{
	return(Iterator(*this, _buckets.Size()));
}

template<typename TKey, typename TData>
typename UnorderedMap<TKey, TData>::ConstIterator UnorderedMap<TKey, TData>::end() const
{
	size_type size = _buckets.Size();
	ConstIterator it(*this, size);
	return(it);
}

template<typename TKey, typename TData>
typename UnorderedMap<TKey, TData>::ConstIterator UnorderedMap<TKey, TData>::cend() const
{
	return(ConstIterator(*this, _buckets.Size()));
}

template<typename TKey, typename TData>
typename const UnorderedMap<TKey, TData>::size_type UnorderedMap<TKey, TData>::BucketPlacement(const TKey& key) const
{
	size_type hash = _hashFunctor(key);
	return  hash % _buckets.Size();
}



template<typename TKey, typename TData>
TData& UnorderedMap<TKey, TData>::operator[](const TKey& key)
{
	it_bool_pair itBoolPair = Find(key);

	if (itBoolPair.first == end()) //NOT FOUND AND WE NEED TO INSERT THIS VALUE
	{
		TData data; //how do you default constructor int, char?

		const size_type bucket = BucketPlacement(key);
		//pushes back the pair into the chain and returns an chain_iterator type set to where the pair was placed in the chain
		std::pair<TKey, TData> pair(key, data);
		chain_iterator chain_it = _buckets[bucket].PushBack(pair);
		++_size;
		return (*chain_it).second;
	}
	else
	{
		return ((*itBoolPair.first).second); //this syntax is evil but this how we get to the TData
	}

}


template<typename TKey, typename TData>
const TData& UnorderedMap<TKey, TData>::operator[](const TKey& key) const
{
	const_it_bool_pair constItBoolPair = Find(key);

	if (constItBoolPair.second == false) //NOT FOUND
	{
		throw std::runtime_error("Key not found in the map.");
	}
	else
	{
		return ((*constItBoolPair.first).second); //this syntax is evil but this how we get to the TData
	}

}

template<typename TKey, typename TData>
void UnorderedMap<TKey, TData>::Remove(const TKey& key)
{
	size_type bucket = BucketPlacement(key);
	it_bool_pair itBoolPair = Find(key);

	bucket;

	if (itBoolPair.first != end()) //Key element was found;
	{

		Iterator map_it = itBoolPair.first;
		_buckets[bucket].Remove(*map_it);
	}

}


template<typename TKey, typename TData>
void UnorderedMap<TKey, TData>::Clear()
{
	for (size_type i = 0; i < _buckets.Size(); ++i)
	{
		_buckets[i].Clear();
	}
	_size = 0;
}

template<typename TKey, typename TData>
bool UnorderedMap<TKey, TData>::ContainsKey(const TKey& key)
{
	it_bool_pair itBoolPair = Find(key);

	return itBoolPair.second;
}

template<typename TKey, typename TData>
TData& UnorderedMap<TKey, TData>::At(const TKey& key)
{
	it_bool_pair itBoolPair = Find(key);

	if (itBoolPair.second == false)
	{
		throw std::runtime_error("Key not found in the map.");
	}
	return operator[](key);
}

template<typename TKey, typename TData>
const TData& UnorderedMap<TKey, TData>::At(const TKey& key) const
{
	const_it_bool_pair constItBoolPair = Find(key);

	if (constItBoolPair.first == end()) //NOT FOUND AND WE NEED TO INSERT THIS VALUE
	{
		throw std::runtime_error("Key not found in the map.");
	}
	return operator[](key);
}

template<typename TKey, typename TData>
float UnorderedMap<TKey, TData>::LoadFactorPercentage() const
{
	return  static_cast<float>(_size) / static_cast<float>(_buckets.Size());
}

template<typename TKey, typename TData>
void UnorderedMap<TKey, TData>::ReHashMap(const size_type new_size)
{
	UnorderedMap newMap(new_size, _hashFunctor, _equalityFunctor);
	Iterator It = begin();
	while (It != end())
	{
		newMap.Insert(*It);
		++It;
	}

	Clear();
	_buckets.Resize(new_size);
	this->operator=(std::move(newMap));
	
}


#pragma endregion
#pragma region ITERATOR FUNCTIONS
//Operator Overload Equals
template<typename TKey, typename TData>
bool UnorderedMap<TKey, TData>::Iterator::operator==(const Iterator& right) const
{
	return !(operator!=(right));
}

//Operator Overload Not Equals
template<typename TKey, typename TData>
bool UnorderedMap<TKey, TData>::Iterator::operator!=(const Iterator& right) const
{
	if (_owner != right._owner || _chain_it != right._chain_it || _bucket_index != right._bucket_index)
	{
		return true;
	}
	return false;
}

//Operator Overload Post Increment
template<typename TKey, typename TData>
typename UnorderedMap<TKey, TData>::Iterator UnorderedMap<TKey, TData>::Iterator::operator++(int)
{
	Iterator temp = *this;
	++* this;
	return temp;
}

//Operator Overload Pre Increment
template<typename TKey, typename TData>
typename UnorderedMap<TKey, TData>::Iterator& UnorderedMap<TKey, TData>::Iterator::operator++()
{
	//iterator is not assigned to a container.
	if (_owner == nullptr) { throw std::runtime_error("Cannot traverse an unassociated container."); }

	//attempting to dereference an empty container. Should return this. Which should be end()
	if (_owner->_size == 0)
	{
		return *this;
	}

	++_chain_it; //move up a link in the chain
	while (_chain_it == _owner->_buckets[_bucket_index].end()) // if link in chain is end...not valid then move on to the next bucket and check its link
	{
		
		++_bucket_index;

		if (_bucket_index == _owner->_buckets.Size()) //check if we have reached the end of the map
		{
			_chain_it = _owner->_buckets[_bucket_index - 1].end();

			this->SetInnerIterator(_bucket_index, _chain_it);
			break;
		}

		_chain_it = _owner->_buckets[_bucket_index].begin();

	}
	return *this;
}

//Operator Overload Dereference
template<typename TKey, typename TData>
typename UnorderedMap<TKey, TData>::pair_type& UnorderedMap<TKey, TData>::Iterator::operator*()
{
	//attempting to dereference an unassociated container.
	if (_owner == nullptr) { throw std::runtime_error("Cannot dereference unassigned container."); }

	//attempting to dereference an empty container. 
	if (_owner->_size == 0) { throw std::runtime_error("Cannot dereference end of container."); }

	return *_chain_it;
	//return *(It).second;	
}

//private constructor
template<typename TKey, typename TData>
UnorderedMap<TKey, TData>::Iterator::Iterator(UnorderedMap& owner, const size_t bucket_index) :
	_owner(&owner),
	_bucket_index(bucket_index), //needs to know which bucket it's traversing. Sets it to the first bucket or last bucket depending on begin() or end() call
	_chain_it( bucket_index < owner._buckets.Size() ? owner._buckets[bucket_index].begin() : owner._buckets[bucket_index - 1].end()) //if bucket_index is less than the number of buckets set the iterator to beginning of that bucket. Otherwise set it to end of the last bucket.
{

}

template<typename TKey, typename TData>
typename UnorderedMap<TKey, TData>::Iterator& UnorderedMap<TKey, TData>::Iterator::SetInnerIterator(const size_t bucket_index, const chain_iterator& chain_it)
{
	_chain_it = chain_it;
	_bucket_index = bucket_index;
	return *this;
}
#pragma endregion
#pragma region CONST-ITERATOR FUNCTIONS
template<typename TKey, typename TData>
UnorderedMap<TKey, TData>::ConstIterator::ConstIterator(const Iterator &right) :
	_owner(right._owner),
	_bucket_index(right._bucket_index),
	_chain_it(right._chain_it)
{

}

//Operator Overload Equals
template<typename TKey, typename TData>
bool UnorderedMap<TKey, TData>::ConstIterator::operator==(const ConstIterator& right) const
{
	return !(operator!=(right));
}

//Operator Overload Not Equals
template<typename TKey, typename TData>
bool UnorderedMap<TKey, TData>::ConstIterator::operator!=(const ConstIterator& right) const
{
	if (_owner != right._owner || _chain_it != right._chain_it || _bucket_index != right._bucket_index)
	{
		return true;
	}
	return false;
}

//Operator Overload Post Increment
template<typename TKey, typename TData>
typename UnorderedMap<TKey, TData>::ConstIterator UnorderedMap<TKey, TData>::ConstIterator::operator++(int)
{
	ConstIterator temp = *this;
	++* this;
	return temp;
}

//Operator Overload Pre Increment
template<typename TKey, typename TData>
typename UnorderedMap<TKey, TData>::ConstIterator& UnorderedMap<TKey, TData>::ConstIterator::operator++()
{
	//iterator is not assigned to a container.
	if (_owner == nullptr) { throw std::runtime_error("Cannot traverse an unassociated container."); }

	//attempting to dereference an empty container. Should return this. Which should be end()
	if (_owner->_size == 0)
	{
		return *this;
	}

	++_chain_it; //move up a link in the chain
	while (_chain_it == _owner->_buckets[_bucket_index].end()) // if link in chain is end...not valid then move on to the next bucket and check its link
	{
		
		_chain_it = _owner->_buckets[++_bucket_index].begin();

		if (_bucket_index == _owner->_buckets.Size()) //check if we have reached the end of the map
		{
			this->operator=(end());
			break;
		}

	}
	return *this;
}

//Operator Overload Dereference
template<typename TKey, typename TData>
typename const UnorderedMap<TKey, TData>::pair_type& UnorderedMap<TKey, TData>::ConstIterator::operator*() const
{
	//attempting to dereference an unassociated container.
	if (_owner == nullptr) { throw std::runtime_error("Cannot dereference unassigned container."); }

	//attempting to dereference an empty container. 
	if (_owner->_size == 0) { throw std::runtime_error("Cannot dereference end of container."); }

	return *_chain_it;
	//return *(It).second;	
}

//private constructor
template<typename TKey, typename TData>
UnorderedMap<TKey, TData>::ConstIterator::ConstIterator(const UnorderedMap& owner, const size_t bucket_index) :
	_owner(&owner),
	_bucket_index(bucket_index), //needs to know which bucket it's traversing. Sets it to the first bucket or last bucket depending on begin() or end() call
	_chain_it(bucket_index < owner._buckets.Size() ? owner._buckets[bucket_index].begin() : owner._buckets[bucket_index - 1].end()) //if bucket_index is less than the number of buckets set the iterator to beginning of that bucket. Otherwise set it to end of the last bucket.
{}

//Sets the chain_iterator of the map_iterator
template<typename TKey, typename TData>
typename UnorderedMap<TKey, TData>::ConstIterator& UnorderedMap<TKey, TData>::ConstIterator::SetInnerIterator(const size_t bucket_index, const const_chain_iterator& chain_it)
{
	_chain_it = chain_it;
	_bucket_index = bucket_index;
	return *this;
}
#pragma endregion
template<typename TKey, typename TData>
struct DefaultPairEqualityFunctor
{
public:
	bool operator()(const std::pair<TKey, TData>& left, const  std::pair<TKey, TData>& right) const
	{
		if (left.first != right.first || left.second != right.second) { return false; }
		return true;
	}
};
}
