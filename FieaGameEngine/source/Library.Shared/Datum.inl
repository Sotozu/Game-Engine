#pragma once
#include "Datum.h"
#include "Scope.h"
//The order in which these functions are declared in the .inl file is important
//If one is referenced before it is declared there will be errors.
namespace FieaGameEngine
{


#pragma region Get Template Specilizations
	template<>
	inline int32_t& Datum::Get<int32_t>(const std::size_t index)
	{
		if (_type != DatumTypes::Integer) { throw std::runtime_error("Cannot use Get method on Datum with type not set."); }
		if (index >= _size) { throw std::runtime_error("Cannot access empty datum or go beyond current size of datum."); }
		return _data.i[index];
	}
	template<>
	inline bool& Datum::Get<bool>(const std::size_t index)
	{
		if (_type != DatumTypes::Boolean) { throw std::runtime_error("Cannot use Get method on Datum with type not set."); }
		if (index >= _size) { throw std::runtime_error("Cannot access empty datum or go beyond current size of datum."); }
		return _data.b[index];
	}
	template<>
	inline float& Datum::Get<float>(const std::size_t index)
	{
		if (_type != DatumTypes::Float) { throw std::runtime_error("Cannot use Get method on Datum with type not set."); }
		if (index >= _size) { throw std::runtime_error("Cannot access empty datum or go beyond current size of datum."); }
		return _data.f[index];
	}
	template<>
	inline std::string& Datum::Get<std::string>(const std::size_t index)
	{
		if (_type != DatumTypes::String) { throw std::runtime_error("Cannot use Get method on Datum with type not set."); }
		if (index >= _size) { throw std::runtime_error("Cannot access empty datum or go beyond current size of datum."); }
		return _data.s[index];
	}
	template<>
	inline glm::vec4& Datum::Get<glm::vec4>(const std::size_t index)
	{
		if (_type != DatumTypes::Vector4) { throw std::runtime_error("Cannot use Get method on Datum with type not set."); }
		if (index >= _size) { throw std::runtime_error("Cannot access empty datum or go beyond current size of datum."); }
		return _data.v[index];
	}
	template<>
	inline glm::mat4& Datum::Get<glm::mat4>(const std::size_t index)
	{
		if (_type != DatumTypes::Matrix) { throw std::runtime_error("Cannot use Get method on Datum with type not set."); }
		if (index >= _size) { throw std::runtime_error("Cannot access empty datum or go beyond current size of datum."); }
		return _data.m[index];
	}
	template<>
	inline RTTI*& Datum::Get<RTTI*>(const std::size_t index)
	{
		if (_type != DatumTypes::Pointer) { throw std::runtime_error("Cannot use Get method on Datum with type not set."); }
		if (index >= _size) { throw std::runtime_error("Cannot access empty datum or go beyond current size of datum."); }
		return _data.r[index];
	}
	template<>
	inline Scope& Datum::Get<Scope>(const std::size_t index)
	{
		if (_type != DatumTypes::Table) { throw std::runtime_error("Cannot use Get method on Datum with type not set."); }
		if (index >= _size) { throw std::runtime_error("Cannot access empty datum or go beyond current size of datum."); }
		return *_data.t[index];
	}
#pragma endregion

#pragma region Get Const Template Specilizations
	template<>
	inline const int32_t& Datum::Get<int32_t>(const std::size_t index) const
	{
		if (_type != DatumTypes::Integer) { throw std::runtime_error("Cannot use Get method on Datum with type not set."); }
		if (index >= _size) { throw std::runtime_error("Cannot access empty datum or go beyond current size of datum."); }
		return _data.i[index];
	}
	template<>
	inline const float& Datum::Get<float>(const std::size_t index) const
	{
		if (_type != DatumTypes::Float) { throw std::runtime_error("Cannot use Get method on Datum with type not set."); }
		if (index >= _size) { throw std::runtime_error("Cannot access empty datum or go beyond current size of datum."); }
		return _data.f[index];
	}
	template<>
	inline const std::string& Datum::Get<std::string>(const std::size_t index) const
	{
		if (_type != DatumTypes::String) { throw std::runtime_error("Cannot use Get method on Datum with type not set."); }
		if (index >= _size) { throw std::runtime_error("Cannot access empty datum or go beyond current size of datum."); }
		return _data.s[index];
	}
	template<>
	inline const glm::vec4& Datum::Get<glm::vec4>(const std::size_t index) const
	{
		if (_type != DatumTypes::Vector4) { throw std::runtime_error("Cannot use Get method on Datum with type not set."); }
		if (index >= _size) { throw std::runtime_error("Cannot access empty datum or go beyond current size of datum."); }
		return _data.v[index];
	}
	template<>
	inline const glm::mat4& Datum::Get<glm::mat4>(const std::size_t index) const
	{
		if (_type != DatumTypes::Matrix) { throw std::runtime_error("Cannot use Get method on Datum with type not set."); }
		if (index >= _size) { throw std::runtime_error("Cannot access empty datum or go beyond current size of datum."); }
		return _data.m[index];
	}

	template<>
	inline RTTI* const& Datum::Get<RTTI*>(const std::size_t index) const
	{
		if (_type != DatumTypes::Pointer) { throw std::runtime_error("Cannot use Get method on Datum with type not set."); }
		if (index >= _size) { throw std::runtime_error("Cannot access empty datum or go beyond current size of datum."); }
		return _data.r[index];
	}
	template<>
	inline const Scope& Datum::Get<Scope>(const std::size_t index) const
	{
		if (_type != DatumTypes::Table) { throw std::runtime_error("Cannot use Get method on Datum with type not set."); }
		if (index >= _size) { throw std::runtime_error("Cannot access empty datum or go beyond current size of datum."); }
		return *_data.t[index];
	}
#pragma endregion

#pragma region Front Template Specializations
	template<>
	inline int32_t& Datum::Front<int32_t>()
	{
		if (_type != DatumTypes::Integer) { throw std::runtime_error("Template function is not compatible with Datum type."); }
		if (_size == 0) { throw std::runtime_error("Cannot access empty Datum."); }
		return _data.i[0]; //BUT WHAT IF SIZE IS 0?
	}

	template<>
	inline float& Datum::Front<float>()
	{
		if (_type != DatumTypes::Float) { throw std::runtime_error("Template function is not compatible with Datum type."); }
		if (_size == 0) { throw std::runtime_error("Cannot access empty Datum."); }
		return _data.f[0]; //BUT WHAT IF SIZE IS 0?
	}

	template<>
	inline std::string& Datum::Front<std::string>()
	{
		if (_type != DatumTypes::String) { throw std::runtime_error("Template function is not compatible with Datum type."); }
		if (_size == 0) { throw std::runtime_error("Cannot access empty Datum."); }
		return _data.s[0]; //BUT WHAT IF SIZE IS 0?
	}

	template<>
	inline glm::vec4& Datum::Front<glm::vec4>()
	{
		if (_type != DatumTypes::Vector4) { throw std::runtime_error("Template function is not compatible with Datum type."); }
		if (_size == 0) { throw std::runtime_error("Cannot access empty Datum."); }
		return _data.v[0]; //BUT WHAT IF SIZE IS 0?
	}

	template<>
	inline glm::mat4& Datum::Front<glm::mat4>()
	{
		if (_type != DatumTypes::Matrix) { throw std::runtime_error("Template function is not compatible with Datum type."); }
		if (_size == 0) { throw std::runtime_error("Cannot access empty Datum."); }
		return _data.m[0]; //BUT WHAT IF SIZE IS 0?
	}

	template<>
	inline RTTI*& Datum::Front<RTTI*>()
	{
		if (_type != DatumTypes::Pointer) { throw std::runtime_error("Template function is not compatible with Datum type."); }
		if (_size == 0) { throw std::runtime_error("Cannot access empty Datum."); }
		return _data.r[0]; //BUT WHAT IF SIZE IS 0?
	}

	template<>
	inline Scope& Datum::Front<Scope>()
	{
		if (_type != DatumTypes::Pointer) { throw std::runtime_error("Template function is not compatible with Datum type."); }
		if (_size == 0) { throw std::runtime_error("Cannot access empty Datum."); }
		return *_data.t[0]; //BUT WHAT IF SIZE IS 0?
	}

#pragma endregion

#pragma region Front Template Specializations Const
	template<>
	inline const int32_t& Datum::Front<int32_t>() const
	{
		if (_type != DatumTypes::Integer) { throw std::runtime_error("Template function is not compatible with Datum type."); }
		if (_size == 0) { throw std::runtime_error("Cannot access empty Datum."); }
		return _data.i[0]; //BUT WHAT IF SIZE IS 0?
	}

	template<>
	inline const float& Datum::Front<float>() const
	{
		if (_type != DatumTypes::Float) { throw std::runtime_error("Template function is not compatible with Datum type."); }
		if (_size == 0) { throw std::runtime_error("Cannot access empty Datum."); }
		return _data.f[0]; //BUT WHAT IF SIZE IS 0?
	}

	template<>
	inline const std::string& Datum::Front<std::string>() const
	{
		if (_type != DatumTypes::String) { throw std::runtime_error("Template function is not compatible with Datum type."); }
		if (_size == 0) { throw std::runtime_error("Cannot access empty Datum."); }
		return _data.s[0]; //BUT WHAT IF SIZE IS 0?
	}

	template<>
	inline const glm::vec4& Datum::Front<glm::vec4>() const
	{
		if (_type != DatumTypes::Vector4) { throw std::runtime_error("Template function is not compatible with Datum type."); }
		if (_size == 0) { throw std::runtime_error("Cannot access empty Datum."); }
		return _data.v[0]; //BUT WHAT IF SIZE IS 0?
	}

	template<>
	inline const glm::mat4& Datum::Front<glm::mat4>() const
	{
		if (_type != DatumTypes::Matrix) { throw std::runtime_error("Template function is not compatible with Datum type."); }
		if (_size == 0) { throw std::runtime_error("Cannot access empty Datum."); }
		return _data.m[0]; //BUT WHAT IF SIZE IS 0?
	}

	template<>
	inline RTTI* const& Datum::Front<RTTI*>() const
	{
		if (_type != DatumTypes::Pointer) { throw std::runtime_error("Template function is not compatible with Datum type."); }
		if (_size == 0) { throw std::runtime_error("Cannot access empty Datum."); }
		return _data.r[0]; //BUT WHAT IF SIZE IS 0?
	}
	template<>
	inline const Scope& Datum::Front<Scope>() const
	{
		if (_type != DatumTypes::Pointer) { throw std::runtime_error("Template function is not compatible with Datum type."); }
		if (_size == 0) { throw std::runtime_error("Cannot access empty Datum."); }
		return *_data.t[0]; //BUT WHAT IF SIZE IS 0?
	}
#pragma endregion

#pragma region Back Template Specializations
	template<>
	inline int32_t& Datum::Back<int32_t>()
	{
		if (_type != DatumTypes::Integer) { throw std::runtime_error("Template function is not compatible with Datum type."); }
		if (_size == 0) { throw std::runtime_error("Cannot access empty Datum."); }
		return _data.i[Size() - 1]; //BUT WHAT IF SIZE IS 0?
	}

	template<>
	inline float& Datum::Back<float>()
	{
		if (_type != DatumTypes::Float) { throw std::runtime_error("Template function is not compatible with Datum type."); }
		if (_size == 0) { throw std::runtime_error("Cannot access empty Datum."); }
		return _data.f[Size() - 1]; //BUT WHAT IF SIZE IS 0?
	}

	template<>
	inline std::string& Datum::Back<std::string>()
	{
		if (_type != DatumTypes::String) { throw std::runtime_error("Template function is not compatible with Datum type."); }
		if (_size == 0) { throw std::runtime_error("Cannot access empty Datum."); }
		return _data.s[Size() - 1]; //BUT WHAT IF SIZE IS 0?
	}

	template<>
	inline glm::vec4& Datum::Back<glm::vec4>()
	{
		if (_type != DatumTypes::Vector4) { throw std::runtime_error("Template function is not compatible with Datum type."); }
		if (_size == 0) { throw std::runtime_error("Cannot access empty Datum."); }
		return _data.v[Size() - 1]; //BUT WHAT IF SIZE IS 0?
	}

	template<>
	inline glm::mat4& Datum::Back<glm::mat4>()
	{
		if (_type != DatumTypes::Matrix) { throw std::runtime_error("Template function is not compatible with Datum type."); }
		if (_size == 0) { throw std::runtime_error("Cannot access empty Datum."); }
		return _data.m[Size() - 1]; //BUT WHAT IF SIZE IS 0?
	}

	template<>
	inline RTTI*& Datum::Back<RTTI*>()
	{
		if (_type != DatumTypes::Pointer) { throw std::runtime_error("Template function is not compatible with Datum type."); }
		if (_size == 0) { throw std::runtime_error("Cannot access empty Datum."); }
		return _data.r[Size() - 1]; //BUT WHAT IF SIZE IS 0?
	}

	template<>
	inline Scope& Datum::Back<Scope>()
	{
		if (_type != DatumTypes::Table) { throw std::runtime_error("Template function is not compatible with Datum type."); }
		if (_size == 0) { throw std::runtime_error("Cannot access empty Datum."); }
		return *_data.t[Size() - 1]; //BUT WHAT IF SIZE IS 0?
	}


#pragma endregion

#pragma region Back Template Specializations Const
	template<>
	inline const int32_t& Datum::Back<int32_t>() const
	{
		if (_type != DatumTypes::Integer) { throw std::runtime_error("Cannot access unassigned Datum."); }
		if (_size == 0) { throw std::runtime_error("Cannot access empty Datum."); }

		return _data.i[Size() - 1]; //BUT WHAT IF SIZE IS 0?
	}

	template<>
	inline const float& Datum::Back<float>() const
	{
		if (_type != DatumTypes::Float) { throw std::runtime_error("Cannot access unassigned Datum."); }

		if (_size == 0) { throw std::runtime_error("Cannot access empty Datum."); }
		return _data.f[Size() - 1]; //BUT WHAT IF SIZE IS 0?
	}

	template<>
	inline const std::string& Datum::Back<std::string>() const
	{
		if (_type != DatumTypes::String) { throw std::runtime_error("Cannot access unassigned Datum."); }

		if (_size == 0) { throw std::runtime_error("Cannot access empty Datum."); }
		return _data.s[Size() - 1]; //BUT WHAT IF SIZE IS 0?
	}

	template<>
	inline const glm::vec4& Datum::Back<glm::vec4>() const
	{
		if (_type != DatumTypes::Vector4) { throw std::runtime_error("Cannot access unassigned Datum."); }

		if (_size == 0) { throw std::runtime_error("Cannot access empty Datum."); }
		return _data.v[Size() - 1]; //BUT WHAT IF SIZE IS 0?
	}

	template<>
	inline const glm::mat4& Datum::Back<glm::mat4>() const
	{
		if (_type != DatumTypes::Matrix) { throw std::runtime_error("Cannot access unassigned Datum."); }
		if (_size == 0) { throw std::runtime_error("Cannot access empty Datum."); }
		return _data.m[Size() - 1]; //BUT WHAT IF SIZE IS 0?
	}

	template<>
	inline RTTI* const& Datum::Back<RTTI*>() const
	{
		if (_type != DatumTypes::Pointer) { throw std::runtime_error("Cannot access unassigned Datum."); }
		if (_size == 0) { throw std::runtime_error("Cannot access empty Datum."); }
		return _data.r[Size() - 1]; //BUT WHAT IF SIZE IS 0?
	}

	template<>
	inline const Scope& Datum::Back<Scope>() const
	{
		if (_type != DatumTypes::Table) { throw std::runtime_error("Template function is not compatible with Datum type."); }
		if (_size == 0) { throw std::runtime_error("Cannot access empty Datum."); }
		return *_data.t[Size() - 1]; //BUT WHAT IF SIZE IS 0?
	}
#pragma endregion

#pragma region Typecast Constructor Overloads
	inline Datum::Datum(const int32_t& value) { PushBack(value); }
	inline Datum::Datum(const bool& value) { PushBack(value); }
	inline Datum::Datum(const float& value) { PushBack(value); }
	inline Datum::Datum(const std::string& value) { PushBack(value); }
	inline Datum::Datum(const glm::vec4& value) { PushBack(value); }
	inline Datum::Datum(const glm::mat4& value) { PushBack(value); }
	inline Datum::Datum(RTTI* const value) { PushBack(value); }
	inline Datum::Datum(Scope& value) { PushBack(value); }
#pragma endregion

#pragma region Set Overloads
	inline void Datum::Set(const int32_t& value, const std::size_t index)
	{
		if (_type != DatumTypes::Integer) { throw std::runtime_error("Cannot call this overloaded set function on Datum type Datum::Integer."); }
		if (index >= _size) { throw std::runtime_error("Cannot access beyond current size of datum."); }
		_data.i[index] = value;
	}

	inline void Datum::Set(const bool& value, const std::size_t index)
	{
		if (_type != DatumTypes::Boolean) { throw std::runtime_error("Cannot call this overloaded set function on Datum type Datum::Boolean."); }
		if (index >= _size) { throw std::runtime_error("Cannot access beyond current size of datum."); }
		_data.b[index] = value;
	}

	inline void Datum::Set(const float& value, const std::size_t index)
	{
		if (_type != DatumTypes::Float) { throw std::runtime_error("Cannot call this overloaded set function on Datum type Datum::Float."); }
		if (index >= _size) { throw std::runtime_error("Cannot access beyond current size of datum."); }
		_data.f[index] = value;
	}

	inline void Datum::Set(const std::string& value, const std::size_t index)
	{
		if (_type != DatumTypes::String) { throw std::runtime_error("Cannot call this overloaded set function on Datum type Datum::String."); }
		if (index >= _size) { throw std::runtime_error("Cannot access beyond current size of datum."); }
		_data.s[index].~basic_string();
		_data.s[index] = value;
	}

	inline void Datum::Set(const glm::vec4& value, const std::size_t index)
	{
		if (_type != DatumTypes::Vector4) { throw std::runtime_error("Cannot call this overloaded set function on Datum type Datum::Vector."); }
		if (index >= _size) { throw std::runtime_error("Cannot access beyond current size of datum."); }
		_data.v[index] = value;
	}

	inline void Datum::Set(const glm::mat4& value, const std::size_t index)
	{
		if (_type != DatumTypes::Matrix) { throw std::runtime_error("Cannot call this overloaded set function on Datum type Datum::Matrix."); }
		if (index >= _size) { throw std::runtime_error("Cannot access beyond current size of datum."); }
		_data.m[index] = value;
	}

	inline void Datum::Set(RTTI*& value, const std::size_t index)
	{
		if (_type != DatumTypes::Pointer) { throw std::runtime_error("Cannot call this overloaded set function on Datum type Datum::Pointer."); }
		if (index >= _size) { throw std::runtime_error("Cannot access beyond current size of datum."); }
		_data.r[index] = value;
	}

	inline void Datum::Set(const Scope& value, const std::size_t index)
	{
		if (_type != DatumTypes::Table) { throw std::runtime_error("Cannot call this overloaded set function on Datum type Datum::Pointer."); }
		if (index >= _size) { throw std::runtime_error("Cannot access beyond current size of datum."); }
		_data.t[index] = const_cast<Scope*>(&value);
	}
#pragma endregion

#pragma region Remove Overloads
	inline bool Datum::Remove(const int32_t& value)
	{
		auto [ptr_value, index] = Find(value);
		if (ptr_value == nullptr) { return false; }
		RemoveAt(index);
		return true;
	}
	inline bool Datum::Remove(const bool& value)
	{
		auto [ptr_value, index] = Find(value);
		if (ptr_value == nullptr) { return false; }
		RemoveAt(index);
		return true;
	}
	inline bool Datum::Remove(const float& value)
	{
		auto [ptr_value, index] = Find(value);
		if (ptr_value == nullptr) { return false; }
		RemoveAt(index);
		return true;
	}
	inline bool Datum::Remove(const std::string& value)
	{
		auto [ptr_value, index] = Find(value);
		if (ptr_value == nullptr) { return false; }
		RemoveAt(index);
		return true;
	}
	inline bool Datum::Remove(const glm::vec4& value)
	{
		auto [ptr_value, index] = Find(value);
		if (ptr_value == nullptr) { return false; }
		RemoveAt(index);
		return true;
	}
	inline bool Datum::Remove(const glm::mat4& value)
	{
		auto [ptr_value, index] = Find(value);
		if (ptr_value == nullptr) { return false; }
		RemoveAt(index);
		return true;
	}
	inline bool Datum::Remove(RTTI*& value)
	{
		auto [ptr_value, index] = Find(value);
		if (ptr_value == nullptr) { return false; }
		RemoveAt(index);
		return true;
	}
	inline bool Datum::Remove(Scope& value)
	{
		auto [ptr_value, index] = Find(value);
		if (ptr_value == nullptr) { return false; }
		RemoveAt(index);
		return true;
	}

#pragma endregion

#pragma region OO Assignment Overloads
	inline Datum& Datum::operator=(const int32_t& value)
	{
		_type = DatumTypes::Integer;
		Clear(); //Will handle external datum type clear
		ShrinkToFit(); //If _size is 0 and shrink to fit is called it frees the memory
		PushBack(value);
		return *this;
	}
	inline Datum& Datum::operator=(const bool value)
	{
		_type = DatumTypes::Boolean;
		Clear(); //Will handle external datum type clear
		ShrinkToFit(); //If _size is 0 and shrink to fit is called it frees the memory
		PushBack(value);
		return *this;
	}
	inline Datum& Datum::operator=(const float& value)
	{
		_type = DatumTypes::Float;
		Clear(); //Will handle external datum type clear
		ShrinkToFit(); //If _size is 0 and shrink to fit is called it frees the memory
		PushBack(value);
		return *this;
	}
	inline Datum& Datum::operator=(const std::string& value)
	{
		_type = DatumTypes::String;
		Clear(); //Will handle external datum type clear
		ShrinkToFit(); //If _size is 0 and shrink to fit is called it frees the memory
		PushBack(value);
		return *this;
	}
	inline Datum& Datum::operator=(const glm::vec4& value)
	{
		_type = DatumTypes::Vector4;
		Clear(); //Will handle external datum type clear
		ShrinkToFit(); //If _size is 0 and shrink to fit is called it frees the memory
		PushBack(value);
		return *this;
	}
	inline Datum& Datum::operator=(const glm::mat4& value)
	{
		_type = DatumTypes::Matrix;
		Clear(); //Will handle external datum type clear
		ShrinkToFit(); //If _size is 0 and shrink to fit is called it frees the memory
		PushBack(value);
		return *this;
	}
	inline Datum& Datum::operator=(RTTI*& value)
	{
		_type = DatumTypes::Pointer;
		Clear(); //Will handle external datum type clear
		ShrinkToFit(); //If _size is 0 and shrink to fit is called it frees the memory
		PushBack(value);
		return *this;
	}
	inline Datum& Datum::operator=(const Scope& value)
	{
		_type = DatumTypes::Table;
		Clear(); //Will handle external datum type clear
		ShrinkToFit(); //If _size is 0 and shrink to fit is called it frees the memory
		PushBack(value);
		return *this;

	}
#pragma endregion

#pragma region OO Is Equal
	inline bool Datum::operator==(const std::int32_t value) const
	{
		if (_type != DatumTypes::Integer || _size != 1 || _data.i[0] != value) { return false; }
		return true;
	}
	inline bool Datum::operator==(const bool value) const
	{
		if (_type != DatumTypes::Boolean || _size != 1 || _data.b[0] != value) { return false; }
		return true;
	}
	inline bool Datum::operator==(const float value) const
	{
		if (_type != DatumTypes::Float || _size != 1 || _data.f[0] != value) { return false; }
		return true;
	}

	inline bool Datum::operator==(const std::string& value) const
	{
		if (_type != DatumTypes::String || _size != 1 || _data.s[0] != value) { return false; }
		return true;
	}

	inline bool Datum::operator==(const glm::vec4& value) const
	{
		if (_type != DatumTypes::Vector4 || _size != 1 || _data.v[0] != value) { return false; }
		return true;
	}

	inline bool Datum::operator==(const glm::mat4& value) const
	{
		if (_type != DatumTypes::Matrix || _size != 1 || _data.m[0] != value) { return false; }
		return true;
	}

	inline bool Datum::operator==(const RTTI* value) const
	{
		if (_type != DatumTypes::Pointer || _size != 1 || _data.r[0] != value) { return false; }
		return true;
	}

	inline bool Datum::operator==(const Scope& value) const //REVIEW THIS
	{
		if (_type != DatumTypes::Table || _size != 1 || (_data.t[0]) != &value) { return false; }
		return true;
	}
#pragma endregion

	inline DatumTypes Datum::Type() const {return _type;}

	inline size_t Datum::Size() const {return _size;}

	inline size_t Datum::Capacity() const { return _capacity; }

	inline bool Datum::IsEmpty() const {return (_size == 0);}

	inline bool Datum::IsExternal() const{return isExternal;}

	inline bool Datum::IsDataSet() const { return (_data.vp != nullptr); }

	inline Scope& Datum::operator[](std::size_t index) {return (Get<Scope>(index));}

	inline bool Datum::operator==(const Datum& rhs) const {return !(operator!=(rhs));}

	inline std::size_t Datum::SizeOfType() const {return DatumTypeSizes[static_cast<size_t>(_type)];}


	inline bool Datum::SetType(const DatumTypes& type)
	{
		if (type == DatumTypes::Unknown && _type != DatumTypes::Unknown) { throw std::runtime_error("Cannot set Datum to Unknown once type is already set."); }
		if (_type != DatumTypes::Unknown && type != _type) { throw std::runtime_error("Cannot reassign datum to another type once type is set."); }
		_type = type;
		return true;
	}

	inline void Datum::PopBack()
	{
		if (isExternal) { throw std::runtime_error("Cannot edit External Datum"); }
		if (_size == 0) { return; }
		if (_type == DatumTypes::String) { _data.s[_size - 1].~basic_string(); }
		_size--;
	}

}
