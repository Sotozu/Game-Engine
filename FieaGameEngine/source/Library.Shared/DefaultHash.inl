#pragma once
#include "DefaultHash.h"

/// <summary>
/// Will hash the bytes of the object.
/// Will do a shallow hash so it won't hash data that this object points to.
/// If objects have heap allocated data that is required to hash then this hash will not work.
/// </summary>
/// <typeparam name="T">The value passed into the functor for hashing</typeparam>

namespace FieaGameEngine
{
	template <typename T>
	struct DefaultHash
	{
	public:
		std::size_t operator()(const T& value)
		{
			return AdditiveHash(reinterpret_cast<const uint8_t*>(&value), sizeof(T)); //must cast to a CONST uint8_t*
		}
	};

#pragma region String Specializations
	template<>
	class DefaultHash<std::string>
	{
	public:
		std::size_t operator()(const std::string& value)
		{
			return AdditiveHash(reinterpret_cast<const uint8_t*>(value.c_str()), value.size());
		}
	};

	template<>
	class DefaultHash<const std::string>
	{
	public:
		std::size_t operator()(const std::string& value)
		{
			return AdditiveHash(reinterpret_cast<const uint8_t*>(value.c_str()), value.size());
		}
	};
#pragma endregion

#pragma region Char Specializations	
	template<>
	class DefaultHash<char>
	{
	public:
		std::size_t operator()(const char& value)
		{
			return value;
		}
	};

	template<>
	class DefaultHash<const char>
	{
	public:
		std::size_t operator()(const char& value)
		{
			return value;
		}
	};

	template<>
	class DefaultHash<char*>
	{
	public:
		std::size_t operator()(const char* value) //when i make this const char*& value it gives me error. Review with Paul
		{
			//return AdditiveHash(reinterpret_cast<const uint8_t*>(*value), size);

			size_t hash = 31; //seed the hash with a prime number.
			size_t index = 0;
			while (value[index] != '\0')
			{
				hash += value[index];
				++index;
			}

			return hash;
		}
	};

	template<>
	class DefaultHash<const char*>
	{
	public:
		std::size_t operator()(const char* value)
		{
			size_t hash = 31; //seed the hash with a prime number.
			size_t index = 0;
			while (value[index] != '\0')
			{
				hash += value[index];
				++index;
			}

			return hash;
		}
	};
#pragma endregion

#pragma region Int Specializations
	template<>
	class DefaultHash<int>
	{
	public:
		std::size_t operator()(const int& value)
		{
			return static_cast<size_t>(abs(value));
		}
	};

	template<>
	class DefaultHash<const int>
	{
	public:
		std::size_t operator()(const int& value)
		{
			return static_cast<size_t>(abs(value));
		}
	};

	//Hashes the value pointed at by the int*
	template<>
	class DefaultHash<int*>
	{
	public:
		std::size_t operator()(const int* const& value) //we need to cast this 
		{
			//size_t val = abs(*value);
			return static_cast<size_t>(abs(*value));
		}
	};

	//Hashes the value pointed at by the int*
	template<>
	class DefaultHash<const int*>
	{
	public:
		std::size_t operator()(const int* const& value)
		{
			return static_cast<size_t>(abs(*value));
		}
	};
#pragma endregion

#pragma region Short Specilizations
	template<>
	class DefaultHash<short>
	{
	public:
		std::size_t operator()(const short& value)
		{
			return static_cast<size_t>(abs(value));
		}
	};

	template<>
	class DefaultHash<const short>
	{
	public:
		std::size_t operator()(const short& value)
		{
			return static_cast<size_t>(abs(value));
		}
	};

	template<>
	class DefaultHash<short*>
	{
	public:
		std::size_t operator()(const short* const& value)
		{
			return static_cast<size_t>(abs(*value));
		}
	};

	template<>
	class DefaultHash<const short*>
	{
	public:
		std::size_t operator()(const short* const& value)
		{
			return static_cast<size_t>(abs(*value));
		}
	};

#pragma endregion

#pragma region Long Specilizations
	template<>
	class DefaultHash<long>
	{
	public:
		std::size_t operator()(const long& value)
		{
			return static_cast<size_t>(abs(value));
		}
	};

	template<>
	class DefaultHash<const long>
	{
	public:
		std::size_t operator()(const long& value)
		{
			return static_cast<size_t>(abs(value));
		}
	};

	template<>
	class DefaultHash<long*>
	{
	public:
		std::size_t operator()(const long* const& value)
		{
			return static_cast<size_t>(abs(*value));
		}
	};

	template<>
	class DefaultHash<const long*>
	{
	public:
		std::size_t operator()(const long* const& value)
		{
			return static_cast<size_t>(abs(*value));
		}
	};
#pragma endregion

#pragma region Long Long Specilizations
	template<>
	class DefaultHash<long long>
	{
	public:
		std::size_t operator()(const long long& value)
		{
			return static_cast<size_t>(abs(value));
		}
	};

	template<>
	class DefaultHash<const long long>
	{
	public:
		std::size_t operator()(const long long& value)
		{
			return static_cast<size_t>(abs(value));
		}
	};

	template<>
	class DefaultHash<long long*>
	{
	public:
		std::size_t operator()(const long long* const& value)
		{
			return static_cast<size_t>(abs(*value));
		}
	};

	template<>
	class DefaultHash<const long long*>
	{
	public:
		std::size_t operator()(const long long* const& value)
		{
			return static_cast<size_t>(abs(*value));
		}
	};

#pragma endregion

#pragma region Float Specilizations
	template<>
	class DefaultHash<float>
	{
	public:
		std::size_t operator()(const float& value)
		{
			return static_cast<size_t>(abs(value));
		}
	};

	template<>
	class DefaultHash<const float>
	{
	public:
		std::size_t operator()(const float& value)
		{
			return static_cast<size_t>(abs(value));
		}
	};

	template<>
	class DefaultHash<float*>
	{
	public:
		std::size_t operator()(const float* const& value)
		{
			return static_cast<size_t>(abs(*value));
		}
	};

	template<>
	class DefaultHash<const float*>
	{
	public:
		std::size_t operator()(const float* const& value)
		{
			return static_cast<size_t>(abs(*value));
		}
	};

#pragma endregion

#pragma region Double Specilizations
	template<>
	class DefaultHash<double>
	{
	public:
		std::size_t operator()(const double& value)
		{
			return static_cast<size_t>(abs(value));
		}
	};

	template<>
	class DefaultHash<const double>
	{
	public:
		std::size_t operator()(const double& value)
		{
			return static_cast<size_t>(abs(value));
		}
	};

	template<>
	class DefaultHash<double*>
	{
	public:
		std::size_t operator()(const double* const& value)
		{
			return static_cast<size_t>(abs(*value));
		}
	};

	template<>
	class DefaultHash<const double*>
	{
	public:
		std::size_t operator()(const double* const& value)
		{
			return static_cast<size_t>(abs(*value));
		}
	};

#pragma endregion

#pragma region Long Double Specilizations
	template<>
	class DefaultHash<long double>
	{
	public:
		std::size_t operator()(const long double& value)
		{
			return static_cast<size_t>(abs(value));
		}
	};

	template<>
	class DefaultHash<const long double>
	{
	public:
		std::size_t operator()(const long double& value)
		{
			return static_cast<size_t>(abs(value));
		}
	};

	template<>
	class DefaultHash<long double*>
	{
	public:
		std::size_t operator()(const long double* const& value)
		{
			return static_cast<size_t>(abs(*value));
		}
	};

	template<>
	class DefaultHash<const long double*>
	{
	public:
		std::size_t operator()(const long double* const& value)
		{
			return static_cast<size_t>(abs(*value));
		}
	};

#pragma endregion


}