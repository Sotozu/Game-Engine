#include "pch.h"
#include "Datum.h"

namespace FieaGameEngine 
{
	const std::size_t Datum::DatumTypeSizes[] =
	{
		0, //UNKNOWN
		sizeof(std::int32_t),
		sizeof(bool),
		sizeof(float),
		sizeof(std::string),
		sizeof(glm::vec4),
		sizeof(glm::mat4),
		sizeof(RTTI*),
		sizeof(Scope*)
	};

#pragma region Initializer List Template Specializations
	template<>
	Datum::Datum(std::initializer_list<int32_t> list) : _capacity(list.size()), _type(DatumTypes::Integer), isExternal(0)
	{
		void* data = malloc(_capacity * sizeof(int32_t));
		assert(data != nullptr);
		_data.vp = data;

		for (const auto& value : list)
		{
			PushBack(value);
		}
	}

	template<>
	Datum::Datum(std::initializer_list<bool> list) : _capacity(list.size()), _type(DatumTypes::Boolean), isExternal(0)
	{
		void* data = malloc(_capacity * sizeof(bool));
		assert(data != nullptr);
		_data.vp = data;

		for (const auto& value : list)
		{
			PushBack(value);
		}
	}


	template<>
	Datum::Datum(std::initializer_list<float> list) : _capacity(list.size()), _type(DatumTypes::Float), isExternal(0)
	{
		void* data = malloc(_capacity * sizeof(float));
		assert(data != nullptr);
		_data.vp = data;

		for (const auto& value : list)
		{
			PushBack(value);
		}
	}

	template<>
	Datum::Datum(std::initializer_list<std::string> list) : _capacity(list.size()), _type(DatumTypes::String), isExternal(0)
	{
		void* data = malloc(_capacity * sizeof(std::string));
		assert(data != nullptr);
		_data.vp = data;

		for (const auto& value : list)
		{
			PushBack(value);
		}
	}

	template<>
	Datum::Datum(std::initializer_list<glm::vec4> list) : _capacity(list.size()), _type(DatumTypes::Vector4), isExternal(0)
	{
		void* data = malloc(_capacity * sizeof(glm::vec4));
		assert(data != nullptr);
		_data.vp = data;

		for (const auto& value : list)
		{
			PushBack(value);
		}
	}

	template<>
	Datum::Datum(std::initializer_list<glm::mat4> list) : _capacity(list.size()), _type(DatumTypes::Matrix), isExternal(0)
	{
		void* data = malloc(_capacity * sizeof(glm::mat4));
		assert(data != nullptr);
		_data.vp = data;

		for (const auto& value : list)
		{
			PushBack(value);
		}

	}

	template<>
	Datum::Datum(std::initializer_list<RTTI*> list) : _capacity(list.size()), _type(DatumTypes::Pointer), isExternal(0)
	{
		void* data = malloc(_capacity * sizeof(RTTI*));
		assert(data != nullptr);
		_data.vp = data;

		for (const auto& value : list)
		{
			PushBack(value);
		}

	}
	
	template<>
	Datum::Datum(std::initializer_list<Scope*> list) : _capacity(list.size()), _type(DatumTypes::Table), isExternal(0)
	{
		void* data = malloc(_capacity * sizeof(Scope*));
		assert(data != nullptr);
		_data.vp = data;

		for (const auto& value : list)
		{
			PushBack(*value); //Make sure we are pushing back a scope reference and not a pointer. Otherwise RTTI version will be invoked.
		}

	}
#pragma endregion

	//Constructor(type,size)
	Datum::Datum(DatumTypes type, std::size_t size)
	{
		if(type == DatumTypes::Unknown) { throw std::runtime_error("This constructor cannot have type set to Unknown."); }
		_type = type;
		std::size_t size_of_type = DatumTypeSizes[static_cast<size_t>(_type)];
		if (size > 0)
		{
			void* data = malloc(size * size_of_type);
			assert(data != nullptr);
			_data.vp = data;
			Resize(size);

		}
	}

	//Copy Constructor
	Datum::Datum(const Datum& rhs) : _size(rhs._size), _capacity(rhs._capacity), _type(rhs._type), isExternal(rhs.isExternal)
	{
		if (!rhs.isExternal)
		{
			std::size_t size_of_type = DatumTypeSizes[static_cast<size_t>(_type)];

			if (rhs._size > 0)
			{
				void* data = malloc(rhs._capacity * size_of_type); //Malloc appropriate size
				assert(data != nullptr);
				_data.vp = data;

				if (_type == DatumTypes::String)
				{
					for (std::size_t index = 0; index < rhs._size; ++index)
					{
						new(_data.s + index)std::string(rhs._data.s[index]);
					}
				}
				else
				{
					memcpy(_data.vp, rhs._data.vp, size_of_type * rhs._size); //memcpy bytes into allocated space
				}
			}
		}
		else
		{
			_data = rhs._data;
		}


	}


	//OO Assignment
	Datum& Datum::operator=(const Datum& rhs)
	{
		if (this != &rhs)
		{
			if (!isExternal && rhs.isExternal) //INTERNAL COPYING EXTERNAL

			{
				Clear();
				ShrinkToFit();

				_data = rhs._data;
				_type = rhs._type;
				_size = rhs._size;
				_capacity = rhs._capacity;
				isExternal = rhs.isExternal;

			}
			else if (isExternal && rhs.isExternal) //EXTERNAL COPYING EXTERNAL
			{
				_data = rhs._data;
				_type = rhs._type;
				_size = rhs._size;
				_capacity = rhs._capacity;
				isExternal = rhs.isExternal;

			}
			else if (!isExternal && !rhs.isExternal) //INTERNAL COPYING INTERNAL 
			{
				Clear();
				ShrinkToFit();

				_type = rhs._type;
				_capacity = rhs._capacity;
				_size = rhs._size;
				isExternal = rhs.isExternal;
			

				std::size_t size_of_type = DatumTypeSizes[static_cast<size_t>(rhs._type)];

				if (rhs._size > 0)
				{
					void* data = malloc(rhs._capacity * size_of_type); //Malloc appropriate size
					assert(data != nullptr);
					_data.vp = data;

					if (_type == DatumTypes::String)
					{
						for (std::size_t index = 0; index < rhs._size; ++index)
						{
							new(_data.s + index)std::string(rhs._data.s[index]);
						}
					}
					else
					{
						memcpy(_data.vp, rhs._data.vp, size_of_type * rhs._size); //memcpy bytes into allocated space
					}
				}
			}
			else if (isExternal && !rhs.isExternal) //EXTERNAL COPYING INTERNAL
			{

				_data.vp = nullptr;
				_type = rhs._type;
				_capacity = rhs._capacity;
				_size = rhs._size;
				isExternal = rhs.isExternal;


				std::size_t size_of_type = DatumTypeSizes[static_cast<size_t>(rhs._type)];

				if (rhs._size > 0)
				{
					void* data = malloc(rhs._capacity * size_of_type); //Malloc appropriate size
					assert(data != nullptr);
					_data.vp = data;

					if (_type == DatumTypes::String)
					{
						for (std::size_t index = 0; index < rhs._size; ++index)
						{
							new(_data.s + index)std::string(rhs._data.s[index]);
						}
					}
					else
					{
						memcpy(_data.vp, rhs._data.vp, size_of_type * rhs._size); //memcpy bytes into allocated space
					}
				}
			}
		}
		return *this;
	}

	//Move Constructor
	Datum::Datum(Datum&& rhs) noexcept: _type(rhs._type), _size(rhs._size), _capacity(rhs._capacity), _data(rhs._data), isExternal(rhs.isExternal)
	{
		rhs._type = DatumTypes::Unknown;
		rhs._size = 0;
		rhs._capacity = 0;
		rhs._data.vp = nullptr;
		rhs.isExternal = false;
	}

	//Move Assignment
	Datum& Datum::operator=(Datum&& rhs) noexcept
	{
		if (isExternal)
		{
			_data.vp = nullptr;

			_type = rhs._type;
			_size = rhs._size;
			_capacity = rhs._capacity;
			_data = rhs._data;
			isExternal = rhs.isExternal;

			rhs._type = DatumTypes::Unknown;
			rhs._size = 0;
			rhs._capacity = 0;
			rhs._data.vp = nullptr;
			isExternal = false;

		}
		else
		{
			Clear();
			ShrinkToFit();

			_type = rhs._type;
			_size = rhs._size;
			_capacity = rhs._capacity;
			_data = rhs._data;
			isExternal = rhs.isExternal;

			rhs._type = DatumTypes::Unknown;
			rhs._size = 0;
			rhs._capacity = 0;
			rhs._data.vp = nullptr;
			isExternal = false;
		}
		return *this;
	}

	//Destructor
	Datum::~Datum()
	{
		Clear();
		free(_data.vp);
	}

	bool Datum::Resize(const size_t& size)
	{
		if (isExternal) { throw std::runtime_error("Cannot edit External Datum"); }
		
		bool resized = false;
		if (size == 0)
		{
			Clear();
			_capacity = 0;

		}
		else if (size < _size) //We need to shrink
		{
			size_t size_index = _size - 1;
			size_t new_size_index = size - 1;
			for (size_index; size_index > new_size_index; --size_index)
			{
				PopBack();
			}
			ShrinkToFit();
			resized = true;
		}
		else //We need to grow
		{
			Reserve(size);

			/*if (_size != 0) { size_index = _size - 1;} *///If size is not 0 do this. Otherwise don't subtract from 0.


			if (_type == DatumTypes::String) //Special case for strings all other union types won't create issues
			{
				for (size_t size_index = _size; size_index < size; ++size_index)
				{
					new(_data.s + size_index)std::string("");
				}
			}
			if (_type == DatumTypes::Table) //We have ownership of Scope memory. So we must create new
			{
				for (size_t size_index = _size; size_index < size; ++size_index)
				{
					new(_data.t + size_index)Scope*();
				}

			}
			_size = size;
			resized = true;
		}
		return resized;

		
	}

	void Datum::ShrinkToFit()
	{
		if (isExternal) { throw std::runtime_error("Cannot edit External Datum"); }
		std::size_t size_of_type = DatumTypeSizes[static_cast<size_t>(_type)];
		if (_size == 0) 
		{ 
			free(_data.vp); 
			_data.vp = nullptr;
			_capacity = 0; 
			
		}
		else if (_size < _capacity)
		{
			void* data = realloc(_data.vp, _size * size_of_type);
			assert(data != nullptr);
			_data.vp = data;
			_capacity = _size;
		}
	}

	void Datum::Reserve(std::size_t capacity)
	{
		if (_type == DatumTypes::Unknown) { throw std::runtime_error("Reserve function cannot be called on Datum with no type."); }

		if (isExternal) { throw std::runtime_error("Cannot edit External Datum"); }

		if (_capacity < capacity)
		{
			std::size_t size_of_type = DatumTypeSizes[static_cast<size_t>(_type)];
			assert(size_of_type != 0);
			void* data = realloc(_data.vp, capacity * size_of_type);
			assert(data != nullptr);

			_data.vp = data;
			_capacity = capacity;
		}
	}

	void Datum::Clear()
	{
		if (isExternal) 
		{
			_capacity = 0;
			_size = 0;
			_data.vp = nullptr;
		}
		else
		{
			if (_size == 0) { return; }
			else
			{
				if (_type == DatumTypes::String)
				{
					for (size_t index = 0; index < _size; ++index)
					{
						_data.s[index].~basic_string();

					}
				}
				/*else if (_type == DatumTypes::Table)
				{
					for (std::size_t index = 0; index < _size; ++index)
					{
						delete _data.t[index];
					}
				}*/
				_size = 0;
			}
			
		}
	}

	//WHAT IF TEH THE DATUM IS OF TYPE TABLE?
	//FIGURE OUT WHAT TO DO WHEN THIS IS THE CASE
	void Datum::RemoveAt(size_t index)
	{
		if (isExternal) { throw std::runtime_error("Cannot edit External Datum"); }
		if(index >= _size){ throw std::runtime_error("Cannot access beyond size of datum."); }
		std::size_t size_of_type = DatumTypeSizes[static_cast<size_t>(_type)];

		if (_type == DatumTypes::String)
		{
			_data.s[index].~basic_string();
		}

		memmove((reinterpret_cast<uint8_t*>(_data.vp) + (index*size_of_type)), (reinterpret_cast<uint8_t*>(_data.vp) + ((index + 1)*size_of_type)), size_of_type * (_size - index - 1));
		--_size;
		
	}

	bool Datum::operator!=(const Datum& rhs) const
	{
		int zeroIsMemoryEquivalent = 0;//set to memory IS EQUIVALENT (Effectively false for this method)
		if (_type != rhs._type || _size != rhs._size) { return true; }
		if (_type == DatumTypes::String) //special case to compare strings
		{
			//iterate and compare strings
			for (std::size_t index = 0; index < _size; ++index)
			{
				std::string lhs_string = _data.s[index];
				std::string rhs_string = rhs._data.s[index];
				if (lhs_string != rhs_string) { zeroIsMemoryEquivalent = 1; break; }
			}
		}
		else if (_type == DatumTypes::Pointer)
		{
			for (std::size_t index = 0; index < _size; ++index)
			{
				if (_data.r[index]->Equals(rhs._data.r[index]) == false) { zeroIsMemoryEquivalent = 1; break; };
			}
		}
		else if (_type == DatumTypes::Table)
		{
			for (std::size_t index = 0; index < _size; ++index)
			{
				if (_data.t[index]->Equals(rhs._data.t[index]) == false) { zeroIsMemoryEquivalent = 1; break; };
			}

		}
		else
		{
			zeroIsMemoryEquivalent = memcmp(_data.s, rhs._data.s, _size); //If return is 0 that means the memory is equivalent (Makes this method return false)
		}

		return (zeroIsMemoryEquivalent != 0); //We only want to return true when the memory is not equivalent

	}

	void Datum::SetFromString(const std::string& str, std::size_t index) //Should this function be able to call malloc and allocate space if an empty datum calls it?
	{
		if (_type == DatumTypes::Unknown) { throw std::runtime_error("Datum type must be set before SetFromString function can be called."); }
		if (index >= _size) { throw std::runtime_error("Cannot access beyond current size of datum. Or Datum is empty"); }

		switch (_type)
		{
		case (DatumTypes::Integer):
			int i;
			sscanf_s(str.c_str(), "%d", &i);
			_data.i[index] = i;
			break;

		case (DatumTypes::Float):
			float f;
			sscanf_s(str.c_str(), "%f", &f);
			_data.f[index] = f;
			break;

		case (DatumTypes::String):

			_data.s[index].~basic_string();
			_data.s[index] = str;
			break;

		case (DatumTypes::Vector4):
			glm::vec4 myVec;
			sscanf_s(str.c_str(), "vec4(%f, %f, %f, %f)", &myVec[0], &myVec[1], &myVec[2], &myVec[3]);
			_data.v[index] = myVec;
			break;

		case (DatumTypes::Matrix):
			float myFloat[16];
			;
			sscanf_s(str.c_str(), "mat4x4((%f, %f, %f, %f), (%f, %f, %f, %f), (%f, %f, %f, %f), (%f, %f, %f, %f))",
				&myFloat[0], &myFloat[1], &myFloat[2], &myFloat[3],
				&myFloat[4], &myFloat[5], &myFloat[6], &myFloat[7],
				&myFloat[8], &myFloat[9], &myFloat[10], &myFloat[11],
				&myFloat[12], &myFloat[13], &myFloat[14], &myFloat[15]);

			glm::mat4 myMat{ myFloat[0], myFloat[1], myFloat[2], myFloat[3],
			myFloat[4], myFloat[5], myFloat[6], myFloat[7],
			myFloat[8], myFloat[9], myFloat[10], myFloat[11],
			myFloat[12], myFloat[13], myFloat[14], myFloat[15] };

			_data.m[index] = myMat;
			break;
		default:
			throw std::runtime_error("ToString method cannot parse token.");

		}
	}

	//The type is determined before this method is called. This is determined by data describing it in json.
	void Datum::PushBackFromString(const std::string& str) //Should this function be able to call malloc and allocate space if an empty datum calls it?
	{
		std::string myString;
		size_t size;
		char token = str[0];

		switch (token)
		{
		case ('i'):
			if (_type != DatumTypes::Integer) { throw std::runtime_error("Datum type and string token type not compatible."); }
			int i;
			sscanf_s(str.c_str(), "i(%d)", &i);
			PushBack(i);
			break;

		case ('f'):
			if (_type != DatumTypes::Float) { throw std::runtime_error("Datum type and string token type not compatible."); }
			float f;
			sscanf_s(str.c_str(), "f(%f)", &f);
			PushBack(f);
			break;

		case ('s'):
			if (_type != DatumTypes::String) { throw std::runtime_error("Datum type and string token type not compatible."); }
			size = str.length();
			myString = str.substr(2, str.length() - 3);
			PushBack(myString);
			break;

		case ('v'):
			if (_type != DatumTypes::Vector4) { throw std::runtime_error("Datum type and string token type not compatible."); }
			glm::vec4 myVec;
			sscanf_s(str.c_str(), "vec4(%f, %f, %f, %f)", &myVec[0], &myVec[1], &myVec[2], &myVec[3]);
			PushBack(myVec);
			break;

		case ('m'):
			if (_type != DatumTypes::Matrix) { throw std::runtime_error("Datum type and string token type not compatible."); }
			float myFloat[16];
			sscanf_s(str.c_str(), "mat4x4((%f, %f, %f, %f), (%f, %f, %f, %f), (%f, %f, %f, %f), (%f, %f, %f, %f))",
				&myFloat[0], &myFloat[1], &myFloat[2], &myFloat[3],
				&myFloat[4], &myFloat[5], &myFloat[6], &myFloat[7],
				&myFloat[8], &myFloat[9], &myFloat[10], &myFloat[11],
				&myFloat[12], &myFloat[13], &myFloat[14], &myFloat[15]);

			glm::mat4 myMat{ myFloat[0], myFloat[1], myFloat[2], myFloat[3],
			myFloat[4], myFloat[5], myFloat[6], myFloat[7],
			myFloat[8], myFloat[9], myFloat[10], myFloat[11],
			myFloat[12], myFloat[13], myFloat[14], myFloat[15] };

			PushBack(myMat);
			break;
		default:
			throw std::runtime_error("ToString method cannot parse token.");

		}
	}

	std::string Datum::ToString(std::size_t index) const
	{
		if (index >= _size) { throw std::runtime_error("Cannot access beyond current size of datum."); }
		std::string returnString;
		switch (_type)
		{
		case (DatumTypes::Integer):
			returnString = "i(" + std::to_string(_data.i[index]) + ")";
			return returnString;
			break;

		case (DatumTypes::Float):
			returnString = "f(" + std::to_string(_data.f[index]) + ")";
			return returnString;
			break;

		case (DatumTypes::String):
			returnString = "s(" + _data.s[index] + ")";
			return returnString;
			break;

		case (DatumTypes::Vector4):
			return glm::to_string( _data.v[index]);
			break;

		case (DatumTypes::Matrix):
			return glm::to_string(_data.m[index]);
			break;

		case (DatumTypes::Pointer):
			return _data.r[index]->ToString();
			break;

		default:
			throw std::runtime_error("ToString method cannot be called on Datum with no type.");
		}
	}

#pragma region PushBack Overloads
	void Datum::PushBack(const int32_t& value)
	{
		if (isExternal) { throw std::runtime_error("Cannot edit External Datum"); }

		SetType(DatumTypes::Integer); //Checks if it's possible to set the type
		
		if (_data.vp == nullptr) //First item in defaulted datum
		{
			++_capacity;
			void* data = malloc(_capacity * sizeof(int32_t));
			assert(data != nullptr);
			_data.vp = data;
		}
		else if (_size == _capacity)
		{
			Reserve( _capacity * 2);
		}
		new(_data.i + _size)int32_t(value);
		++_size;
	}
	void Datum::PushBack(const bool& value)
	{
		if (isExternal) { throw std::runtime_error("Cannot edit External Datum"); }

		SetType(DatumTypes::Boolean); //Checks if it's possible to set the type

		if (_data.vp == nullptr) //First item in defaulted datum
		{
			++_capacity;
			void* data = malloc(_capacity * sizeof(bool));
			assert(data != nullptr);
			_data.vp = data;
		}
		else if (_size == _capacity)
		{
			Reserve(_capacity * 2);
		}
		new(_data.b + _size)bool(value);
		++_size;
	}
	void Datum::PushBack(const float& value)
	{
		if (isExternal) { throw std::runtime_error("Cannot edit External Datum"); }

		SetType(DatumTypes::Float);


		if (_data.vp == nullptr)
		{
			++_capacity;
			void* data = malloc(_capacity * sizeof(float));
			assert(data != nullptr);
			_data.vp = data;

		}
		else if (_size == _capacity)
		{
			Reserve(_capacity * 2);
		}
		new(_data.f + _size)float(value);
		++_size;
		
	}
	void Datum::PushBack(const std::string& value)
	{
		if (isExternal) { throw std::runtime_error("Cannot edit External Datum"); }

		SetType(DatumTypes::String);

		if (_data.vp == nullptr) //if calling on defaulted datum
		{
			++_capacity;
			void* data = malloc(_capacity * sizeof(std::string));
			assert(data != nullptr);
			_data.vp = data;
		}
		else if (_size == _capacity) //if reached capacity then grow
		{
			Reserve(_capacity * 2);
		}
		new(_data.s + _size)std::string(value);
		++_size;

	}
	void Datum::PushBack(const glm::vec4& value)
	{
		if (isExternal) { throw std::runtime_error("Cannot edit external External Datum"); }

		SetType(DatumTypes::Vector4);

		if (_data.vp == nullptr)
		{
			++_capacity;
			void* data = malloc(_capacity * sizeof(glm::vec4));
			assert(data != nullptr);
			_data.vp = data;
		}
		else if (_size == _capacity)
		{
			Reserve(_capacity * 2);
		}
		new(_data.v + _size)glm::vec4(value);
		++_size;


	}
	void Datum::PushBack(const glm::mat4& value)
	{
		if (isExternal) { throw std::runtime_error("Cannot edit external External Datum"); }

		SetType(DatumTypes::Matrix);

		if (_data.vp == nullptr)
		{
			++_capacity;
			void* data = malloc(_capacity * sizeof(glm::mat4));
			assert(data != nullptr);
			_data.vp = data;
		}
		else if (_size == _capacity)
		{
			Reserve(_capacity * 2);
		}
		new(_data.m + _size)glm::mat4(value);
		++_size;

	}
	void Datum::PushBack(RTTI* value)
	{
		if (isExternal) { throw std::runtime_error("Cannot edit external External Datum"); }

		SetType(DatumTypes::Pointer);

		if (_data.vp == nullptr)
		{
			++_capacity;
			void* data = malloc(_capacity * sizeof(RTTI*));
			assert(data != nullptr);
			_data.vp = data;
		}
		else if (_size == _capacity)
		{
			Reserve(_capacity * 2);
		}
		new(_data.r + _size)RTTI* (value);
		++_size;

	}

	void Datum::PushBack(const Scope& value)
	{
		if (isExternal) { throw std::runtime_error("Cannot edit external External Datum"); }

		SetType(DatumTypes::Table);

		if (_data.vp == nullptr)
		{
			++_capacity;
			void* data = malloc(_capacity * sizeof(Scope*));
			assert(data != nullptr);
			_data.vp = data;
		}
		else if (_size == _capacity)
		{
			Reserve(_capacity * 2);
		}
		new(_data.t + _size)Scope* (const_cast<Scope*>(&value));
		++_size;

	}

	
#pragma endregion

#pragma region Find Overloads
	std::pair<int32_t*, size_t> Datum::Find(int32_t value)
	{
		if (_type != DatumTypes::Integer) { throw std::runtime_error("Wrong Datum type call."); }
		int32_t* foundValue = nullptr;
		std::pair<int32_t*, size_t> myPair(foundValue, 0);

		for (size_t index = 0; index < _size; ++index)
		{
			if (value == _data.i[index])
			{
				foundValue = &_data.i[index];
				myPair.first = foundValue;
				myPair.second = index;
				return myPair;
			}
		}
		return myPair;
	}
	std::pair<bool*, size_t> Datum::Find(bool value)
	{
		if (_type != DatumTypes::Boolean) { throw std::runtime_error("Wrong Datum type call."); }
		bool* foundValue = nullptr;
		std::pair<bool*, size_t> myPair(foundValue, 0);

		for (size_t index = 0; index < _size; ++index)
		{
			if (value == _data.b[index])
			{
				foundValue = &_data.b[index];
				myPair.first = foundValue;
				myPair.second = index;
				return myPair;
			}
		}
		return myPair;
	}
	std::pair<float*, size_t> Datum::Find(float value)
	{
		if (_type != DatumTypes::Float) { throw std::runtime_error("Wrong Datum type call."); }

		float* foundValue = nullptr;
		std::pair<float*, size_t> myPair(foundValue, 0);
		for (size_t index = 0; index < _size; ++index)
		{
			if (value == _data.f[index])
			{
				foundValue = &_data.f[index];
				myPair.first = foundValue;
				myPair.second = index;
				return myPair;
			}
		}
		return myPair;
	}
	std::pair<std::string*, size_t> Datum::Find(const std::string& value)
	{
		if (_type != DatumTypes::String) { throw std::runtime_error("Wrong Datum type call."); }

		std::string* foundValue = nullptr;
		std::pair<std::string*, size_t> myPair(foundValue, 0);
		for (size_t index = 0; index < _size; ++index)
		{
			if (value == _data.s[index])
			{
				foundValue = &_data.s[index];
				myPair.first = foundValue;
				myPair.second = index;
				return myPair;
			}
		}
		return myPair;
	}

	std::pair<glm::vec4*, size_t> Datum::Find(const glm::vec4& value)
	{
		if (_type != DatumTypes::Vector4) { throw std::runtime_error("Wrong Datum type call."); }

		glm::vec4* foundValue = nullptr;
		std::pair<glm::vec4*, size_t> myPair(foundValue, 0);
		for (size_t index = 0; index < _size; ++index)
		{
			if (value == _data.v[index])
			{
				foundValue = &_data.v[index];
				myPair.first = foundValue;
				myPair.second = index;
				return myPair;
			}
		}
		return myPair;
	}

	std::pair<glm::mat4*, size_t> Datum::Find(const glm::mat4& value)
	{
		if (_type != DatumTypes::Matrix) { throw std::runtime_error("Wrong Datum type call."); }

		glm::mat4* foundValue = nullptr;
		std::pair<glm::mat4*, size_t> myPair(foundValue, 0);
		for (size_t index = 0; index < _size; ++index)
		{
			if (value == _data.m[index])
			{
				foundValue = &_data.m[index];
				myPair.first = foundValue;
				myPair.second = index;
				return myPair;
			}
		}
		return myPair;
	}

	std::pair<RTTI*, size_t> Datum::Find(const RTTI* value)
	{
		if (value == nullptr) { throw std::runtime_error("Trying to dereference nullptr."); }
		if (_type != DatumTypes::Pointer) { throw std::runtime_error("Wrong Datum type call."); }

		RTTI* foundValue = nullptr;
		std::pair<RTTI*, size_t> myPair(foundValue, 0);
		for (size_t index = 0; index < _size; ++index)
		{
			if (value == _data.r[index])
			{
				foundValue = _data.r[index];
				myPair.first = foundValue;
				myPair.second = index;
				return myPair;
			}
		}
		return myPair;

	}

	std::pair<Scope*, size_t> Datum::Find(const Scope& value)
	{
		//if (&value == nullptr) { throw std::runtime_error("Trying to dereference nullptr."); }
		if (_type != DatumTypes::Table) { throw std::runtime_error("Wrong Datum type call."); }
		Scope* foundValue = nullptr;
		std::pair<Scope*, size_t> myPair(foundValue, 0);
		for (size_t index = 0; index < _size; ++index)
		{
			if (&value == _data.t[index])
			{
				foundValue = _data.t[index];;
				myPair.first = foundValue;
				myPair.second = index;
				return myPair;
			}
		}
		return myPair;
	}
#pragma endregion

#pragma region Find Overloads Const
	std::pair<const int32_t*, size_t> Datum::Find(int32_t value) const
	{
		if (_type != DatumTypes::Integer) { throw std::runtime_error("Wrong Datum type call."); }
		int32_t* foundValue = nullptr;
		std::pair<const int32_t*, size_t> myPair(foundValue, 0);

		for (size_t index = 0; index < _size; ++index)
		{
			if (value == _data.i[index])
			{
				foundValue = &_data.i[index];
				myPair.first = foundValue;
				myPair.second = index;
				return myPair;
			}
		}
		return myPair;
	}
	std::pair<const bool*, size_t> Datum::Find(bool value) const
	{
		if (_type != DatumTypes::Boolean) { throw std::runtime_error("Wrong Datum type call."); }
		bool* foundValue = nullptr;
		std::pair<const bool*, size_t> myPair(foundValue, 0);

		for (size_t index = 0; index < _size; ++index)
		{
			if (value == _data.b[index])
			{
				foundValue = &_data.b[index];
				myPair.first = foundValue;
				myPair.second = index;
				return myPair;
			}
		}
		return myPair;
	}
	std::pair<const float*, size_t> Datum::Find(float value) const
	{
		if (_type != DatumTypes::Float) { throw std::runtime_error("Wrong Datum type call."); }

		float* foundValue = nullptr;
		std::pair<const float*, size_t> myPair(foundValue, 0);
		for (size_t index = 0; index < _size; ++index)
		{
			if (value == _data.f[index])
			{
				foundValue = &_data.f[index];
				myPair.first = foundValue;
				myPair.second = index;
				return myPair;
			}
		}
		return myPair;
	}
	std::pair<const std::string*, size_t> Datum::Find(const std::string& value) const
	{
		if (_type != DatumTypes::String) { throw std::runtime_error("Wrong Datum type call."); }

		std::string* foundValue = nullptr;
		std::pair<const std::string*, size_t> myPair(foundValue, 0);
		for (size_t index = 0; index < _size; ++index)
		{
			if (value == _data.s[index])
			{
				foundValue = &_data.s[index];
				myPair.first = foundValue;
				myPair.second = index;
				return myPair;
			}
		}
		return myPair;
	}

	std::pair<const glm::vec4*, size_t> Datum::Find(const glm::vec4& value) const
	{
		if (_type != DatumTypes::Vector4) { throw std::runtime_error("Wrong Datum type call."); }

		glm::vec4* foundValue = nullptr;
		std::pair<const glm::vec4*, size_t> myPair(foundValue, 0);
		for (size_t index = 0; index < _size; ++index)
		{
			if (value == _data.v[index])
			{
				foundValue = &_data.v[index];
				myPair.first = foundValue;
				myPair.second = index;
				return myPair;
			}
		}
		return myPair;
	}

	std::pair<const glm::mat4*, size_t> Datum::Find(const glm::mat4& value) const
	{
		if (_type != DatumTypes::Matrix) { throw std::runtime_error("Wrong Datum type call."); }

		glm::mat4* foundValue = nullptr;
		std::pair<const glm::mat4*, size_t> myPair(foundValue, 0);
		for (size_t index = 0; index < _size; ++index)
		{
			if (value == _data.m[index])
			{
				foundValue = &_data.m[index];
				myPair.first = foundValue;
				myPair.second = index;
				return myPair;
			}
		}
		return myPair;
	}

	std::pair<const RTTI*, size_t> Datum::Find(const RTTI* value) const
	{
		if (value == nullptr) { throw std::runtime_error("Trying to dereference nullptr."); }
		if (_type != DatumTypes::Pointer) { throw std::runtime_error("Wrong Datum type call."); }

		RTTI* foundValue = nullptr;
		std::pair<const RTTI*, size_t> myPair(foundValue, 0);
		for (size_t index = 0; index < _size; ++index)
		{
			if (value == _data.r[index])
			{
				foundValue = _data.r[index];
				myPair.first = foundValue;
				myPair.second = index;
				return myPair;
			}
		}
		return myPair;

	}

	std::pair<const Scope*, size_t> Datum::Find(const Scope& value) const
	{
		//if (&value == nullptr) { throw std::runtime_error("Trying to dereference nullptr."); }
		if (_type != DatumTypes::Table) { throw std::runtime_error("Wrong Datum type call."); }
		Scope* foundValue = nullptr;
		std::pair<const Scope*, size_t> myPair(foundValue, 0);
		for (size_t index = 0; index < _size; ++index)
		{
			if (value == *_data.t[index])
			{
				foundValue = _data.t[index];;
				myPair.first = foundValue;
				myPair.second = index;
				return myPair;

			}
		}
		return myPair;
	}
#pragma endregion

#pragma region SetStorage Overloads
	void Datum::SetStorage(DatumTypes type, void* ext_array, std::size_t size)
	{
		assert(ext_array != nullptr);
		assert(size != 0);
		if (!isExternal) { Clear(); free(_data.vp); } //If not external then we must clear and free the data
		else { Clear(); } //if it is external then call Clear. Clear has a conditions specific to Datums whose storage type is external
		isExternal = true;
		_type = type;
		_capacity = _size = size;
		_data.vp = ext_array;
	}
	void Datum::SetStorage(int32_t* ext_array, std::size_t size)
	{
		assert(ext_array != nullptr);
		assert(size != 0);

		if (!isExternal) { Clear(); free(_data.vp); } //If not external then we must clear and free the data
		else { Clear(); } //if it is external then call Clear. Clear has a conditions specific to Datums whose storage type is external
		isExternal = true;
		_type = DatumTypes::Integer;
		_capacity = _size = size;
		_data.i = ext_array;
	}

	void Datum::SetStorage(bool* ext_array, std::size_t size)
	{
		assert(ext_array != nullptr);
		assert(size != 0);

		if (!isExternal) { Clear(); free(_data.vp); } //If not external then we must clear and free the data
		else { Clear(); } //if it is external then call Clear. Clear has a conditions specific to Datums whose storage type is external
		isExternal = true;
		_type = DatumTypes::Boolean;
		_capacity = _size = size;
		_data.b = ext_array;
	}

	void Datum::SetStorage(float* ext_array, std::size_t size)
	{
		assert(ext_array != nullptr);
		assert(size != 0);

		if (!isExternal) { Clear(); free(_data.vp); } //If not external then we must clear and free the data
		else { Clear(); } //if it is external then call Clear. Clear has a conditions specific to Datums whose storage type is external
		isExternal = true;
		_type = DatumTypes::Float;
		_capacity = _size = size;
		_data.f = ext_array;
	}
	void Datum::SetStorage(std::string* ext_array, std::size_t size)
	{
		assert(ext_array != nullptr);
		assert(size != 0);

		if (!isExternal) { Clear(); free(_data.vp); } //If not external then we must clear and free the data
		else { Clear(); } //if it is external then call Clear. Clear has a conditions specific to Datums whose storage type is external
		isExternal = true;
		_type = DatumTypes::String;
		_capacity = _size = size;
		_data.s = ext_array;
	}
	void Datum::SetStorage(glm::vec4* ext_array, std::size_t size)
	{
		if (!isExternal) { Clear(); free(_data.vp); } //If not external then we must clear and free the data
		else { Clear(); } //if it is external then call Clear. Clear has a conditions specific to Datums whose storage type is external
		isExternal = true;
		_type = DatumTypes::Vector4;
		_capacity = _size = size;
		_data.v = ext_array;
	}
	void Datum::SetStorage(glm::mat4* ext_array, std::size_t size)
	{
		assert(ext_array != nullptr);
		assert(size != 0);

		if (!isExternal) { Clear(); free(_data.vp); } //If not external then we must clear and free the data
		else { Clear(); } //if it is external then call Clear. Clear has a conditions specific to Datums whose storage type is external
		isExternal = true;
		_type = DatumTypes::Matrix;
		_capacity = _size = size;
		_data.m = ext_array;
	}
	void Datum::SetStorage(RTTI** ext_array, std::size_t size)
	{
		assert(ext_array != nullptr);
		assert(size != 0);

		if (!isExternal) { Clear(); free(_data.vp); } //If not external then we must clear and free the data
		else { Clear(); } //if it is external then call Clear. Clear has a conditions specific to Datums whose storage type is external
		isExternal = true;
		_type = DatumTypes::Pointer;
		_capacity = _size = size;
		_data.r = ext_array;
	}

#pragma endregion


}
