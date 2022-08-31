#include "Vector.h"
#include <assert.h>
/*
For non fully specialized function templates, i.e.ones that carry at least one unknown type, you can omit inline, and not receive errors.
But still they are not inline.
For full specializations, i.e.ones that use only known types, you cannot omit it.
*/
namespace FieaGameEngine
{

#pragma region VECTOR FUNCTIONS

	template<typename T>
	Vector<T>::Vector(size_t capacity) :
		_capacity(static_cast<size_t>(capacity)),
		_array( capacity > 0 ? reinterpret_cast<T*>(malloc(sizeof(T)* capacity)) : nullptr)
	{}

	template<typename T>
	Vector<T>::Vector(std::initializer_list<T> list) : 
		_capacity(list.size()),
		_array(reinterpret_cast<T*>(malloc(sizeof(T)* list.size())))
	{
		for (const auto& value : list)
		{
			PushBack(value);
		}
	}
	
	template<typename T>
	Vector<T>::Vector(const Vector& rhs) :
		_size(rhs._size),
		_capacity(rhs._capacity),
		_array(reinterpret_cast<T*>(malloc(sizeof(T)* _capacity)))
	{
		for (size_t i = 0; i < _size; ++i)
		{
			new(_array + i)T(rhs[i]);
		}
	}

	template<typename T>
	Vector<T>::Vector(Vector&& rhs) noexcept : _size(rhs._size), _capacity(rhs._capacity), _array(rhs._array)
	{
		rhs._size = 0;
		rhs._capacity = 0;
		rhs._array = nullptr;	
	}

	template<typename T>
	Vector<T>::~Vector()
	{
		for (size_t i = 0; i < _size; ++i)
		{
			(_array + i)->~T();
		}
		free(_array);
	}

	template<typename T>
	void Vector<T>::PopBack()
	{
		if (!IsEmpty())
		{
			(_array + _size - 1)->~T();
			_size--;
		}
	}


	template<typename T>
	T& Vector<T>::At(const int& index){return operator[](index);}

	template<typename T>
	const T& Vector<T>::At(const int& index) const{return operator[](index);}

	template<typename T>
	T& Vector<T>::operator[](const size_t& index)
	{
		if (index >= _size){throw std::runtime_error("Cannot access beyond vector size.");}
		return *(_array + index);
	}

	template<typename T>
	const T& Vector<T>::operator[](const size_t& index) const
	{
		if (index > _size){throw std::runtime_error("Cannot access beyond vector size.");}
		return *(_array + index);
	}

	template<typename T>
	const bool Vector<T>::IsEmpty() const{return (_size == 0);}

	template<typename T>
	T& Vector<T>::Front()
	{
		if (IsEmpty()){throw std::runtime_error("Cannot access an empty vector.");}
		return *_array;
	}

	template<typename T>
	T& Vector<T>::Back()
	{
		if (IsEmpty()){throw std::runtime_error("Cannot access an empty list.");}
		return *(_array + _size - 1);
	}

	template<typename T>
	const T& Vector<T>::Front() const
	{
		if (IsEmpty()){throw std::runtime_error("Cannot access an empty vector.");}
		return *_array;
	}

	template<typename T>
	const T& Vector<T>::Back() const
	{
		if (IsEmpty()){throw std::runtime_error("Cannot access an empty list.");}
		return *(_array + _size - 1);
	}

	template<typename T>
	const std::size_t Vector<T>::Size() const{return _size;}

	template<typename T>
	const std::size_t Vector<T>::Capacity() const{return _capacity;}

	template<typename T>
	void Vector<T>::PushBack(const T& var)
	{
		if (_size == _capacity) { Reserve(std::max(std::size_t(1), _size * 2)); } //What is _size == 0? Then what? Try... 		if (_size == _capacity) { Reserve(std::max(_capacity + 1, _capacity * 2)); }
		new(_array + _size)T(var);
		_size++;
	}

	template<typename T>
	void Vector<T>::PushBack(T&& var) noexcept
	{
		if (_size == _capacity) { Reserve(std::max(std::size_t(1), _size * 2)); } 
		new(_array + _size)T(std::move(var));
		_size++;
	}

	template<typename T>
	template<typename... Args>
	void Vector<T>::EmplaceBack(Args&&... args)
	{
		if (_size == _capacity) { Reserve(std::max(std::size_t(1), _size * 2)); } 
		new(_array + _size)T(args...);
		_size++;

	}

	template<typename T>
	Vector<T>& Vector<T>::operator=(const Vector& rhs)
	{
		if (this != &rhs) //Self assignment check
		{
			Clear();

			_size = rhs._size;
			_capacity = rhs._capacity;
			_array = reinterpret_cast<T*>(malloc(sizeof(T) * _capacity));

			for (size_t i = 0; i < _size; ++i)
			{
				new(_array + i)T(rhs[i]);
			}
		}
		return *this;
	}

	template<typename T>
	Vector<T>& Vector<T>::operator=(Vector&& rhs) noexcept
	{
		if (this != &rhs)
		{
			Clear();
			_size = rhs._size;
			_capacity = rhs._capacity;
			_array = rhs._array;


			rhs._size = 0;
			rhs._capacity = 0;
			rhs._array = nullptr;
		}

		return *this;
	}

	template<typename T>
	void Vector<T>::Clear()
	{
		while (!IsEmpty())
		{
			PopBack();
		}
		_size = 0;
		_capacity = 0;
		free(_array);
		_array = nullptr;
		
	}

	template<typename T>
	const bool Vector<T>::Reserve(const size_t& capacity)
	{
		bool didReserve = false;
		if (capacity > _capacity)
		{
			_capacity = capacity;
			T* temp = reinterpret_cast<T*>(realloc(_array, sizeof(T) * capacity));
			if (temp != nullptr) { _array = temp; }
			else { throw std::runtime_error("realloc returned invalid pointer (nullptr)."); }
			
			didReserve = true;

		}
		return didReserve;
	}


	template<typename T>
	const bool Vector<T>::Resize(const size_t& num)
	{		
		bool resized = false;
		if (num != _size) //If equal to size then skip this because nothing changes
		{
			if (num == 0) //If the number passed in is 0 then clear the whole vector.
			{
				Clear();
			}
			else if (num < _size)
			{
				int size_index = static_cast<int>(_size - 1);
				int num_index = static_cast<int>(num - 1);
				for (size_index; size_index > num_index; --size_index)
				{
					(_array + size_index)->~T(); //CHANGE THIS TO POPBACK AND DON'T NEED TO SET _size after
				}
				_size = num;
				Reserve(num);
			}
			else
			{
				Reserve(num);
				int size_index = static_cast<int>(_size);
				int num_index = static_cast<int>(num);
				for (size_index; size_index < num_index; ++size_index)
				{
					new(_array + size_index)T();
				}
				_size = num;
			}
			resized = true;
		}
		return resized;
	}

	template<typename T>
	const bool Vector<T>::Remove(const Iterator& it)
	{
		bool removedElement = false;
		if (it != end())
		{
			(_array + it._index)->~T();
			memmove(_array + it._index, _array + it._index + 1, sizeof(T) * (_size - it._index - 1));
			_size--;
			removedElement = true;
		}
		return removedElement;
	}

	template<typename T>
	const bool Vector<T>::Remove(const Iterator& it1, const Iterator& it2)
	{
		if (it1._index > it2._index) { throw std::runtime_error("Iterators passed into function must be placed into parameters as (lower index), (higher index)."); }

		bool removedElements = false;
		if (it1 != end())
		{
			size_t index = it1._index;

			size_t end_index;
			if (it2._index == _size) { end_index = _size - 1; } //end was passed in
			else { end_index = it2._index; }

			if (index != _size && end_index != _size)
			{
				for (index; index <= end_index; ++index) //Here we begin removing
				{
					(_array + index)->~T();
				}
				memmove(_array + it1._index, _array + end_index + 1, sizeof(T) * (_size - end_index - 1));
				_size -= (end_index - it1._index) + 1;
				removedElements = true;
			}

		}
		return removedElements;
	}

	template<typename T> //REVIEW : ADDED 3.18.2022
	void Vector<T>::RemoveAt(std::size_t index)
	{
		if (index >= _size ) { throw std::runtime_error("Accessing beyond bounds of the vector"); }

		(_array + index)->~T();
		memmove(_array + index, _array + index + 1, sizeof(T) * (_size - index - 1));
		_size--;

	}

	template<typename T>
	typename Vector<T>::Iterator Vector<T>::begin() { return (Iterator(*this, 0)); }

	template<typename T>
	typename Vector<T>::ConstIterator Vector<T>::begin() const { return (ConstIterator(*this, 0)); }

	template<typename T>
	typename Vector<T>::ConstIterator Vector<T>::cbegin() const { return (ConstIterator(*this, 0)); }

	template<typename T>
	typename Vector<T>::Iterator Vector<T>::end() { return (Iterator(*this, _size)); }

	template<typename T>
	typename Vector<T>::ConstIterator Vector<T>::end() const { return (ConstIterator(*this, _size)); }

	template<typename T>
	typename Vector<T>::ConstIterator Vector<T>::cend() const { return (ConstIterator(*this, _size)); }

	template<typename T>
	template<typename EqualityFunctor>
	typename Vector<T>::Iterator Vector<T>::Find(const T& value)
	{
		EqualityFunctor EquFunc{};
		Iterator It;

		for (It = begin(); It != end(); ++It)
		{
			if (EquFunc(*It, value))
			{
				return It;
				break;
			}
		}
		return end();
	}

	template<typename T>
	template<typename EqualityFunctor>
	typename Vector<T>::ConstIterator Vector<T>::Find(const T& value) const
	{
		EqualityFunctor EquFunc{};
		ConstIterator it;

		for (it = begin(); it != end(); ++it)
		{
			if (EquFunc(*it, value)){break;}
		}
		return it;
	}

	template<typename T>
	void Vector<T>::ShrinkToFit() {Resize(_size);}

#pragma endregion

#pragma region ITERATOR FUNCTIONS

	template<typename T>
	bool Vector<T>::Iterator::operator==(const Iterator& right) const
	{
		return !(operator!=(right));
	}

	template<typename T>
	bool Vector<T>::Iterator::operator!=(const Iterator& right) const
	{
		return (_owner != right._owner || _index != right._index);
	}

	template<typename T>
	typename Vector<T>::Iterator Vector<T>::Iterator::operator++(int)
	{
		Iterator temp = *this;
		++* this;
		return temp;
	}

	template<typename T>
	typename Vector<T>::Iterator& Vector<T>::Iterator::operator++()
	{
		if (_owner == nullptr) { throw std::runtime_error("Cannot traverse an unassociated container."); }
		if (_index < _owner->_size){_index++;}
		return *this;
	}

	//TO DO: Added 4/20/2022
	template<typename T>
	typename Vector<T>::Iterator Vector<T>::Iterator::operator--(int)
	{
		Iterator temp = *this;
		--* this;
		return temp;
	}

	//TO DO: Added 4/20/2022
	template<typename T>
	typename Vector<T>::Iterator& Vector<T>::Iterator::operator--()
	{
		if (_owner == nullptr) { throw std::runtime_error("Cannot traverse an unassociated container."); }
		if (_index > 0) { _index--; }
		return *this;
	}


	template<typename T>
	T& Vector<T>::Iterator::operator*() const
	{
		if (_owner == nullptr) { throw std::runtime_error("Cannot dereference unassigned container."); }
		if (_index >= _owner->_size) { throw std::runtime_error("Cannot dereference end of container."); }
		return _owner->operator[](_index);
	}

	template<typename T>
	Vector<T>::Iterator::Iterator(Vector& owner, const size_t index) :
		_owner(&owner),
		_index(index)
	{}
#pragma endregion

#pragma region  CONST-ITERATOR FUNCTIONS

	template<typename T>
	Vector<T>::ConstIterator::ConstIterator(const Iterator& right) :
		_owner(right._owner),
		_index(right._index)
	{}

	template<typename T>
	bool Vector<T>::ConstIterator::operator==(const ConstIterator& right) const
	{
		return !(*this != right);
	}

	template<typename T>
	bool Vector<T>::ConstIterator::operator!=(const ConstIterator& right) const
	{
		return (_owner != right._owner || _index != right._index);
	}
	
	template<typename T>
	typename Vector<T>::ConstIterator Vector<T>::ConstIterator::operator++(int)
	{
		ConstIterator temp = *this;
		++* this;
		return temp;
	}

	template<typename T>
	typename Vector<T>::ConstIterator& Vector<T>::ConstIterator::operator++()
	{
		if (_owner == nullptr) { throw std::runtime_error("Cannot traverse unassigned container."); }
		if (_index < _owner->_size) { _index++; }
		return *this;
	}

	//TO DO: 4/20/2022
	template<typename T>
	typename Vector<T>::ConstIterator Vector<T>::ConstIterator::operator--(int)
	{
		ConstIterator temp = *this;
		--* this;
		return temp;
	}

	//TO DO: 4/20/2022
	template<typename T>
	typename Vector<T>::ConstIterator& Vector<T>::ConstIterator::operator--()
	{
		if (_owner == nullptr) { throw std::runtime_error("Cannot traverse unassigned container."); }
		if (_index > 0) { _index--; }
		return *this;
	}


	template<typename T>
	const T& Vector<T>::ConstIterator::operator*() const
	{
		if (_owner == nullptr) { throw std::runtime_error("Cannot dereference unassigned container."); }
		if (_owner->_size == 0) { throw std::runtime_error("Cannot dereference empty container."); }
		if (_index >= _owner->_size) { throw std::runtime_error("Cannot dereference end of container."); }
		return *(_owner->_array + _index);
	}

	template<typename T>
	Vector<T>::ConstIterator::ConstIterator(const Vector& owner, const size_t index) :
		_owner(&owner),
		_index(index)
	{}

	
#pragma endregion
}



