#pragma once
#include <cstddef>
#include <functional>
#include <initializer_list>
#include <stdexcept>
#include <glm/glm.hpp>
#include "UnorderedMap.h"

#pragma warning (push)
#pragma warning (disable : 4201)
#include "glm/gtx/string_cast.hpp"
#pragma warning (pop)

namespace FieaGameEngine
{
	/// <summary>
	/// Forward declaration used for RTTI* types referenced in the header
	/// </summary>
	class RTTI;
	/// <summary>
	/// Forward declaration used for scope reference in the header
	/// </summary>
	class Scope;
	
	/// <summary>
	/// Used for O(1) access to type check Datum
	/// </summary>
	enum class DatumTypes
	{
		Unknown,
		Integer,
		Boolean,
		Float,
		String,
		Vector4,
		Matrix,
		Pointer,
		Table,

		Begin = Integer,
		End = Table

	};

	/// <summary>
	/// Runtime polymorphic structure
	/// </summary>
	class Datum final
	{
	public:
		friend class Attributed;
		friend Scope;
		Datum() = default;

		/// <summary>
		/// Copy Constructor
		/// </summary>
		/// <param name="rhs">A reference to another datum that data will be copied from</param>
		Datum(const Datum& rhs);

		/// <summary>
		/// Copy Assignment Operator
		/// </summary>
		/// <param name="rhs">A reference to another datum that data will be copied from</param>
		/// <returns>A reference to itself</returns>
		Datum& operator=(const Datum& rhs);
		
		template<typename T>
		Datum(std::initializer_list<T> list);

#pragma region OO Assignment Overloads
		/// <summary>
		/// Operator Overload Assignment - Scaler
		/// Scalar assignment to Datum.
		/// </summary>
		/// <param name="value">Will assign Datum to have only the value passed in</param>
		/// <returns>A reference to itself</returns>
		Datum& operator=(const int32_t& value);
		/// <summary>
		/// Operator Overload Assignment - Scaler
		/// Scalar assignment to Datum.
		/// </summary>
		/// <param name="value">Will assign Datum to have only the value passed in</param>
		/// <returns>A reference to itself</returns>
		Datum& operator=(const bool value);
		/// <summary>
		/// Operator Overload Assignment - Scaler
		/// Scalar assignment to Datum.
		/// </summary>
		/// <param name="value">Will assign Datum to have only the value passed in</param>
		/// <returns>A reference to itself</returns>
		Datum& operator=(const float& value);
		/// <summary>
		/// Operator Overload Assignment - Scaler
		/// Scalar assignment to Datum.
		/// </summary>
		/// <param name="value">Will assign Datum to have only the value passed in</param>
		/// <returns>A reference to itself</returns>
		Datum& operator=(const std::string& value);
		/// <summary>
		/// Operator Overload Assignment - Scaler
		/// Scalar assignment to Datum.
		/// </summary>
		/// <param name="value">Will assign Datum to have only the value passed in</param>
		/// <returns>A reference to itself</returns>
		Datum& operator=(const glm::vec4& value);
		/// <summary>
		/// Operator Overload Assignment - Scaler
		/// Scalar assignment to Datum.
		/// </summary>
		/// <param name="value">Will assign Datum to have only the value passed in</param>
		/// <returns>A reference to itself</returns>
		Datum& operator=(const glm::mat4& value);
		/// <summary>
		/// Operator Overload Assignment - Scaler
		/// Scalar assignment to Datum.
		/// </summary>
		/// <param name="value">Will assign Datum to have only the value passed in</param>
		/// <returns>A reference to itself</returns>
		Datum& operator=(RTTI*& value);

		/// <summary>
		/// Operator Overload Assignment - Scaler
		/// Scalar assignment to Datum.
		/// </summary>
		/// <param name="value">Will assign Datum to have only the value passed in</param>
		/// <returns>A reference to itself</returns>
		Datum& operator=(const Scope& value);
#pragma endregion

		/// <summary>
		/// Move Constructor
		/// </summary>
		/// <param name="rhs">Datum that will be converted to r-value and whose contents will be moved to callilng Datum</param>
		Datum(Datum&& rhs) noexcept;

		/// <summary>
		/// Operator Overload Assignment - Datum
		/// </summary>
		/// <param name="rhs">Datum that will be converted to r-value and whose contents will be moved to callilng Datum</param>
		/// <returns>A reference to itself</returns>
		Datum& operator=(Datum&& rhs) noexcept;

		~Datum();

		/// <summary>
		/// Returns the type that the Datum is assigned.
		/// </summary>
		/// <returns>Returns the type that the Datum is assigned.</returns>
		DatumTypes Type() const;

		/// <summary>
		/// Sets the type that the Datum will be. Can only be called on a Datum that has Unknown type.
		/// </summary>
		/// <param name="type">Type to set the Datum to</param>
		/// <returns>
		/// True if the type was set.
		/// False if the type was not set.
		/// </returns>
		bool SetType(const DatumTypes& type);

		/// <summary>
		/// Returns the current number of elements in the Datum
		/// </summary>
		/// <returns>Returns the current number of elements in the Datum</returns>
		size_t Size() const;

		/// <summary>
		/// Can shrink or grow a Datum.
		/// If growing the values will be default constructed.
		/// If shrinking the value will be deleted/removed.
		/// </summary>
		/// <param name="size">The size that the Datum will take.</param>
		/// <returns>
		/// True if the datum was resized.
		/// False if the datum was not resized.
		/// </returns>
		bool Resize(const size_t& size);

		/// <summary>
		/// Removes all elements from the Datum. Does not effect capacity.
		/// </summary>
		void Clear();

#pragma region SetStorage Overloads

		void SetStorage(DatumTypes type, void* ext_array, std::size_t size); //REVIEW

		/// <summary>
		/// Datum will have access but not control to the array passed in.
		/// Will not be able to shrink or grow the array.
		/// </summary>
		/// <param name="ext_array">The array which the Datum will have access to</param>
		/// <param name="size">The number of elements that the Datum will have access to in the array passed in</param>
		void SetStorage(int32_t* ext_array, std::size_t size);
		/// <summary>
		/// Datum will have access but not control to the array passed in.
		/// Will not be able to shrink or grow the array.
		/// </summary>
		/// <param name="ext_array">The array which the Datum will have access to</param>
		/// <param name="size">The number of elements that the Datum will have access to in the array passed in</param>
		void SetStorage(bool* ext_array, std::size_t size);
		/// <summary>
		/// Datum will have access but not control to the array passed in.
		/// Will not be able to shrink or grow the array.
		/// </summary>
		/// <param name="ext_array">The array which the Datum will have access to</param>
		/// <param name="size">The number of elements that the Datum will have access to in the array passed in</param>
		void SetStorage(float*, std::size_t size);
		/// <summary>
		/// Datum will have access but not control to the array passed in.
		/// Will not be able to shrink or grow the array.
		/// </summary>
		/// <param name="ext_array">The array which the Datum will have access to</param>
		/// <param name="size">The number of elements that the Datum will have access to in the array passed in</param>
		void SetStorage(std::string* ext_array, std::size_t size);
		/// <summary>
		/// Datum will have access but not control to the array passed in.
		/// Will not be able to shrink or grow the array.
		/// </summary>
		/// <param name="ext_array">The array which the Datum will have access to</param>
		/// <param name="size">The number of elements that the Datum will have access to in the array passed in</param>
		void SetStorage(glm::vec4* ext_array, std::size_t size);
		/// <summary>
		/// Datum will have access but not control to the array passed in.
		/// Will not be able to shrink or grow the array.
		/// </summary>
		/// <param name="ext_array">The array which the Datum will have access to</param>
		/// <param name="size">The number of elements that the Datum will have access to in the array passed in</param>
		void SetStorage(glm::mat4* ext_array, std::size_t size);
		/// <summary>
		/// Datum will have access but not control to the array passed in.
		/// Will not be able to shrink or grow the array.
		/// </summary>
		/// <param name="ext_array">The array which the Datum will have access to</param>
		/// <param name="size">The number of elements that the Datum will have access to in the array passed in</param>
		void SetStorage(RTTI** ext_array, std::size_t size);

		/// <summary>
		/// Datum will have access but not control to the array passed in.
		/// Will not be able to shrink or grow the array.
		/// </summary>
		/// <param name="ext_array">The array which the Datum will have access to</param>
		/// <param name="size">The number of elements that the Datum will have access to in the array passed in</param>
		//void SetStorage(Scope** ext_array, std::size_t size);

#pragma endregion

		void Reserve(std::size_t capacity);

		bool operator==(const Datum& rhs) const;
		bool operator!=(const Datum& rhs) const;		

#pragma region Set Overloads
		/// <summary>
		/// Sets an index of the datum to the value passed in.
		/// </summary>
		/// <param name="value">The value that you wish to set at the datum index</param>
		/// <param name="index">The position in the datum you wish to change</param>
		void Set( const int32_t& value, const std::size_t index = 0);

		/// <summary>
		/// Sets an index of the datum to the value passed in.
		/// </summary>
		/// <param name="value">The value that you wish to set at the datum index</param>
		/// <param name="index">The position in the datum you wish to change</param>
		void Set(const bool& value, const std::size_t index = 0);

		/// <summary>
		/// Sets an index of the datum to the value passed in.
		/// </summary>
		/// <param name="value">The value that you wish to set at the datum index</param>
		/// <param name="index">The position in the datum you wish to change</param>
		void Set( const float& value, const std::size_t index = 0);

		/// <summary>
		/// Sets an index of the datum to the value passed in.
		/// </summary>
		/// <param name="value">The value that you wish to set at the datum index</param>
		/// <param name="index">The position in the datum you wish to change</param>
		void Set( const std::string& value, const std::size_t index = 0);

		/// <summary>
		/// Sets an index of the datum to the value passed in.
		/// </summary>
		/// <param name="value">The value that you wish to set at the datum index</param>
		/// <param name="index">The position in the datum you wish to change</param>
		void Set( const glm::vec4& value, const std::size_t index = 0);

		/// <summary>
		/// Sets an index of the datum to the value passed in.
		/// </summary>
		/// <param name="value">The value that you wish to set at the datum index</param>
		/// <param name="index">The position in the datum you wish to change</param>
		void Set( const glm::mat4& value, const std::size_t index = 0);

		/// <summary>
		/// Sets an index of the datum to the value passed in.
		/// </summary>
		/// <param name="value">The value that you wish to set at the datum index</param>
		/// <param name="index">The position in the datum you wish to change</param>
		void Set(RTTI*& value, const std::size_t index = 0);
#pragma endregion

		/// <summary>
		/// Returns a reference to the value found at the passed in index.
		/// </summary>
		/// <typeparam name="T">Any of the valid DatumTypes that our Datum class can be</typeparam>
		/// <param name="index">The index in the Datum we wish to access</param>
		/// <returns>A reference of type which the Datum is set to.</returns>
		template<typename T>
		T& Get(const std::size_t index = 0);

		/// <summary>
		/// Returns a const reference to the value found at the passed in index.
		/// </summary>
		/// <typeparam name="T">Any of the valid DatumTypes that our Datum class can be</typeparam>
		/// <param name="index">The index in the Datum we wish to access</param>
		/// <returns>A const reference of type which the Datum is set to.</returns>
		template<typename T>
		const T& Get(std::size_t index = 0) const;

		/// <summary>
		/// Passed in string value is parsed and converted to type 
		/// </summary>
		/// <param name="str"></param>
		/// <param name="index"></param>
		void SetFromString(const std::string& str, std::size_t index);

		/// <summary>
		/// Passed in string value is parsed and converted to type 
		/// </summary>
		/// <param name="str">string to be converted to the type we wish</param>
		void PushBackFromString(const std::string& str);

		/// <summary>
		/// Returns a string version of the data within Datum
		/// </summary>
		/// <param name="index">The index of the datum pointing to a value we want to return as a string.</param>
		/// <returns>A string representation of the value</returns>
		std::string ToString(std::size_t index = 0) const;

#pragma region PushBack Overloads
		/// <summary>
		/// Passed in value will be set to the end of the datum.
		/// </summary>
		/// <param name="value">value that is set to the end of the Datum</param>
		void PushBack(const int32_t& value);

		/// <summary>
		/// Passed in value will be set to the end of the datum.
		/// </summary>
		/// <param name="value">value that is set to the end of the Datum</param>
		void PushBack(const bool& value);

		/// <summary>
		/// Passed in value will be set to the end of the datum.
		/// </summary>
		/// <param name="value">value that is set to the end of the Datum</param>
		void PushBack(const float& value);

		/// <summary>
		/// Passed in value will be set to the end of the datum.
		/// </summary>
		/// <param name="value">value that is set to the end of the Datum</param>
		void PushBack(const std::string& value);

		/// <summary>
		/// Passed in value will be set to the end of the datum.
		/// </summary>
		/// <param name="value">value that is set to the end of the Datum</param>
		void PushBack(const glm::vec4& value);

		/// <summary>
		/// Passed in value will be set to the end of the datum.
		/// </summary>
		/// <param name="value">value that is set to the end of the Datum</param>
		void PushBack(const glm::mat4& value);

		/// <summary>
		/// Passed in value will be set to the end of the datum.
		/// </summary>
		/// <param name="value">value that is set to the end of the Datum</param>
		void PushBack(RTTI* value);

#pragma endregion

		/// <summary>
		/// removes the last element in the Datum.
		/// </summary>
		void PopBack();

		/// <summary>
		/// Returns a reference to the first element in the Datum.
		/// </summary>
		/// <typeparam name="T">Type that the datum is set to. One of the valid DatumTypes.</typeparam>
		/// <returns>A reference to the first element in the Datum.</returns>
		template<typename T>
		T& Front();

		/// <summary>
		/// Returns a const reference to the first element in the Datum.
		/// </summary>
		/// <typeparam name="T">Type that the datum is set to. One of the valid DatumTypes.</typeparam>
		/// <returns>A const reference to the first element in the Datum.</returns>
		template<typename T>
		const T& Front() const;

		/// <summary>
		/// Returns a reference to the last element in the Datum.
		/// </summary>
		/// <typeparam name="T">Type that the datum is set to. One of the valid DatumTypes.</typeparam>
		/// <returns>A reference to the last element in the Datum.</returns>
		template<typename T>
		T& Back();


		/// <summary>
		/// Returns a const reference to the last element in the Datum.
		/// </summary>
		/// <typeparam name="T">Type that the datum is set to. One of the valid DatumTypes.</typeparam>
		/// <returns>A const reference to the last element in the Datum.</returns>
		template<typename T>
		const T& Back() const;

#pragma region Remove Overloads
		/// <summary>
		/// Find the first element that matched the type passed in and removes it from Datum.
		/// </summary>
		/// <param name="value">Value to search for and remove from the Datum.</param>
		bool Remove(const int32_t& value);
		/// <summary>
		/// Find the first element that matched the type passed in and removes it from Datum.
		/// </summary>
		/// <param name="value">Value to search for and remove from the Datum.</param>
		bool Remove(const bool& value);
		/// <summary>
		/// Find the first element that matched the type passed in and removes it from Datum.
		/// </summary>
		/// <param name="value">Value to search for and remove from the Datum.</param>
		bool Remove(const float& value);
		/// <summary>
		/// Find the first element that matched the type passed in and removes it from Datum.
		/// </summary>
		/// <param name="value">Value to search for and remove from the Datum.</param>
		bool Remove(const std::string& value);
		/// <summary>
		/// Find the first element that matched the type passed in and removes it from Datum.
		/// </summary>
		/// <param name="value">Value to search for and remove from the Datum.</param>
		bool Remove(const glm::vec4& value);
		/// <summary>
		/// Find the first element that matched the type passed in and removes it from Datum.
		/// </summary>
		/// <param name="value">Value to search for and remove from the Datum.</param>
		bool Remove(const glm::mat4& value);
		/// <summary>
		/// Find the first element that matched the type passed in and removes it from Datum.
		/// </summary>
		/// <param name="value">Value to search for and remove from the Datum.</param>
		bool Remove(RTTI*& value);
		/// <summary>
		/// Find the first element that matched the type passed in and removes it from Datum.
		/// </summary>
		/// <param name="value">Value to search for and remove from the Datum.</param>
		bool Remove(Scope& value);
#pragma endregion

		/// <summary>
		/// Removes an element at a certain index in the Datum.
		/// </summary>
		/// <param name="index">index where element will be removed from</param>
		void RemoveAt(size_t index); 

#pragma region Find Overloads
		/// <summary>
		/// Finds the passed in value in the Datum and returns an index to the position found.
		/// If NOT found the returned value will be equal to SIZE.
		/// </summary>
		/// <param name="value">Valud to find in the Datum</param>
		/// <returns>
		/// A std::pair with a pointer to a mutable type found in the datum and the index where it was found.
		/// If NOT FOUND then the pointer is nullptr and the index is 0.
		/// </returns>
		std::pair<int32_t*, size_t> Find(int32_t value);

		/// <summary>
		/// Finds the passed in value in the Datum and returns an index to the position found.
		/// If NOT found the returned value will be equal to SIZE.
		/// </summary>
		/// <param name="value">Valud to find in the Datum</param>
		/// <returns>
		/// A std::pair with a pointer to a mutable type found in the datum and the index where it was found.
		/// If NOT FOUND then the pointer is nullptr and the index is 0.
		/// </returns>
		std::pair<bool*, size_t> Find(bool value);

		/// <summary>
		/// Finds the passed in value in the Datum and returns an index to the position found.
		/// If NOT found the returned value will be equal to SIZE.
		/// </summary>
		/// <param name="value">Valud to find in the Datum</param>
		/// <returns>
		/// A std::pair with a pointer to a mutable type found in the datum and the index where it was found.
		/// If NOT FOUND then the pointer is nullptr and the index is 0.
		/// </returns>
		std::pair<float*, size_t> Find(float value);

		/// <summary>
		/// Finds the passed in value in the Datum and returns an index to the position found.
		/// If NOT found the returned value will be equal to SIZE.
		/// </summary>
		/// <param name="value">Valud to find in the Datum</param>
		/// <returns>
		/// A std::pair with a pointer to a mutable type found in the datum and the index where it was found.
		/// If NOT FOUND then the pointer is nullptr and the index is 0.
		/// </returns>
		std::pair<std::string*, size_t> Find(const std::string& value);

		/// <summary>
		/// Finds the passed in value in the Datum and returns an index to the position found.
		/// If NOT found the returned value will be equal to SIZE.
		/// </summary>
		/// <param name="value">Valud to find in the Datum</param>
		/// <returns>
		/// A std::pair with a pointer to a mutable type found in the datum and the index where it was found.
		/// If NOT FOUND then the pointer is nullptr and the index is 0.
		/// </returns>
		std::pair<glm::vec4*, size_t> Find(const glm::vec4& value);

		/// <summary>
		/// Finds the passed in value in the Datum and returns an index to the position found.
		/// If NOT found the returned value will be equal to SIZE.
		/// </summary>
		/// <param name="value">Valud to find in the Datum</param>
		/// <returns>
		/// A std::pair with a pointer to a mutable type found in the datum and the index where it was found.
		/// If NOT FOUND then the pointer is nullptr and the index is 0.
		/// </returns>
		std::pair<glm::mat4*, size_t> Find(const glm::mat4& value);

		/// <summary>
		/// Finds the passed in value in the Datum and returns an index to the position found.
		/// If NOT found the returned value will be equal to SIZE.
		/// </summary>
		/// <param name="value">Valud to find in the Datum</param>
		/// <returns>
		/// A std::pair with a pointer to a mutable type found in the datum and the index where it was found.
		/// If NOT FOUND then the pointer is nullptr and the index is 0.
		/// </returns>
		std::pair<RTTI*, size_t> Find(const RTTI* value);

		/// <summary>
		/// Finds the passed in value in the Datum and returns an index to the position found.
		/// If NOT found the returned value will be equal to SIZE.
		/// </summary>
		/// <param name="value">Valud to find in the Datum</param>
		/// <returns>
		/// A std::pair with a pointer to a mutable type found in the datum and the index where it was found.
		/// If NOT FOUND then the pointer is nullptr and the index is 0.
		/// </returns>
		std::pair<Scope*, size_t> Find(const Scope& value);
#pragma endregion

#pragma region Find Overloads const
		/// <summary>
		/// Finds the passed in value in the Datum and returns an index to the position found.
		/// If NOT found the returned value will be equal to SIZE.
		/// </summary>
		/// <param name="value">Valud to find in the Datum</param>
		/// <returns>
		/// A std::pair with a pointer to a const type found in the datum and the index where it was found.
		/// If NOT FOUND then the pointer is nullptr and the index is 0.
		/// </returns>
		std::pair<const int32_t*, size_t> Find(int32_t value) const;

		/// <summary>
		/// Finds the passed in value in the Datum and returns an index to the position found.
		/// If NOT found the returned value will be equal to SIZE.
		/// </summary>
		/// <param name="value">Valud to find in the Datum</param>
		/// <returns>
		/// A std::pair with a pointer to a const type found in the datum and the index where it was found.
		/// If NOT FOUND then the pointer is nullptr and the index is 0.
		/// </returns>
		std::pair<const bool*, size_t> Find(bool value) const;

		/// <summary>
		/// Finds the passed in value in the Datum and returns an index to the position found.
		/// If NOT found the returned value will be equal to SIZE.
		/// </summary>
		/// <param name="value">Valud to find in the Datum</param>
		/// <returns>
		/// A std::pair with a pointer to a const type found in the datum and the index where it was found.
		/// If NOT FOUND then the pointer is nullptr and the index is 0.
		/// </returns>
		std::pair<const float*, size_t> Find(float value) const;

		/// <summary>
		/// Finds the passed in value in the Datum and returns an index to the position found.
		/// If NOT found the returned value will be equal to SIZE.
		/// </summary>
		/// <param name="value">Valud to find in the Datum</param>
		/// <returns>
		/// A std::pair with a pointer to a const type found in the datum and the index where it was found.
		/// If NOT FOUND then the pointer is nullptr and the index is 0.
		/// </returns>
		std::pair<const std::string*, size_t> Find(const std::string& value) const;

		/// <summary>
		/// Finds the passed in value in the Datum and returns an index to the position found.
		/// If NOT found the returned value will be equal to SIZE.
		/// </summary>
		/// <param name="value">Valud to find in the Datum</param>
		/// <returns>
		/// A std::pair with a pointer to a const type found in the datum and the index where it was found.
		/// If NOT FOUND then the pointer is nullptr and the index is 0.
		/// </returns>
		std::pair<const glm::vec4*, size_t> Find(const glm::vec4& value) const;
		/// <summary>
		/// Finds the passed in value in the Datum and returns an index to the position found.
		/// If NOT found the returned value will be equal to SIZE.
		/// </summary>
		/// <param name="value">Valud to find in the Datum</param>
		/// <returns>
		/// A std::pair with a pointer to a const type found in the datum and the index where it was found.
		/// If NOT FOUND then the pointer is nullptr and the index is 0.
		/// </returns>
		std::pair<const glm::mat4*, size_t> Find(const glm::mat4& value) const;
		/// <summary>
		/// Finds the passed in value in the Datum and returns an index to the position found.
		/// If NOT found the returned value will be equal to SIZE.
		/// </summary>
		/// <param name="value">Valud to find in the Datum</param>
		/// <returns>
		/// A std::pair with a pointer to a const type found in the datum and the index where it was found.
		/// If NOT FOUND then the pointer is nullptr and the index is 0.
		/// </returns>
		std::pair< const RTTI*, size_t> Find(const RTTI* value) const;

		/// <summary>
		/// Finds the passed in value in the Datum and returns an index to the position found.
		/// If NOT found the returned value will be equal to SIZE.
		/// </summary>
		/// <param name="value">Valud to find in the Datum</param>
		/// <returns>
		/// A std::pair with a pointer to a const type found in the datum and the index where it was found.
		/// If NOT FOUND then the pointer is nullptr and the index is 0.
		/// </returns>
		std::pair<const Scope*, size_t> Find(const Scope& value) const;
#pragma endregion

#pragma region OO Is Equal
		bool operator==(const std::int32_t value) const;
		bool operator==(const bool value) const;
		bool operator==(const float value) const;
		bool operator==(const std::string& value) const;
		bool operator==(const glm::vec4& value) const;
		bool operator==(const glm::mat4& value) const;
		bool operator==(const RTTI* value) const;
		bool operator==(const Scope& value) const;
#pragma endregion

#pragma region OTHER FUNCTIONS NOT REQUIRED BUT HELPFULL
		/// <summary>
		/// Returns the capacity of the Datum
		/// </summary>
		/// <returns>a size_t that refers the capacity of the Datum</returns>
		size_t Capacity() const;

		/// <summary>
		/// Removes excess capacity from a Datum and makes size and capacity equal.
		/// </summary>
		void ShrinkToFit();

		/// <summary>
		/// Checks if the datum is empty by checking size.
		/// </summary>
		/// <returns>
		/// True if the Datum is empty.
		/// False if the Datum is NOT empty.
		/// </returns>
		bool IsEmpty() const;

		/// <summary>
		/// Constructor -Type - Size
		/// Default constructs Size number of type values in the Datum.
		/// </summary>
		/// <param name="type">Type to set the Datum to.</param>
		/// <param name="size">Number of elements to default construct</param>
		Datum(DatumTypes type, std::size_t size = 0);

		/// <summary>
		/// Returns true if the data is external. 
		/// False if the data is internal.
		/// </summary>
		/// <returns>
		/// Returns true if the data is external. 
		/// False if the data is internal.
		/// </returns>
		bool IsExternal() const;

		/// <summary>
		/// Returns true if the data is NOT set to nullptr. 
		/// False if the data is pointing to nullptr.
		/// </summary>
		/// <returns>
		/// Returns true if the data is NOT set to nullptr. 
		/// False if the data is pointing to nullptr.
		/// </returns>
		bool IsDataSet() const;

		Scope& operator[](std::size_t index);
#pragma region Typecast Constructors
		/// <summary>
		/// Typecast constructor that takes in a value of type int32_t and sets size and capacity to 1.
		/// </summary>
		/// <param name="value">value to be first element in Datum</param>
		Datum(const int32_t& value);

		/// <summary>
		/// Typecast constructor that takes in a value of type int32_t and sets size and capacity to 1.
		/// </summary>
		/// <param name="value">value to be first element in Datum</param>
		Datum(const bool& value);

		/// <summary>
		/// Typecast constructor that takes in a value of type float and sets size and capacity to 1.
		/// </summary>
		/// <param name="value">value to be first element in Datum</param>
		Datum(const float& value);

		/// <summary>
		/// Typecast constructor that takes in a value of type std::string and sets size and capacity to 1.
		/// </summary>
		/// <param name="value">value to be first element in Datum</param>
		Datum(const std::string& value);

		/// <summary>
		/// Typecast constructor that takes in a value of type glm::vec4 and sets size and capacity to 1.
		/// </summary>
		/// <param name="value">value to be first element in Datum</param>
		Datum(const glm::vec4& value);

		/// <summary>
		/// Typecast constructor that takes in a value of type glm::mat4 and sets size and capacity to 1.
		/// </summary>
		/// <param name="value">value to be first element in Datum</param>
		Datum(const glm::mat4& value);

		/// <summary>
		/// Typecast constructor that takes in a value of type RTTI* and sets size and capacity to 1.
		/// </summary>
		/// <param name="value">value to be first element in Datum</param>
		Datum(RTTI* const value);

		/// <summary>
		/// Typecast constructor that takes in a value of type RTTI* and sets size and capacity to 1.
		/// </summary>
		/// <param name="value">value to be first element in Datum</param>f
		Datum(Scope& value);

#pragma endregion



#pragma endregion

		
		//NEED TO BE MADE PRIVATE AND TESTS CHANGED
		void PushBack(const Scope& value);
		void Set(const Scope& scope, const std::size_t index = 0);

		std::size_t SizeOfType() const;
	private:
		union DatumValues  
		{

			std::int32_t* i;
			bool* b;
			float* f;
			std::string* s;
			glm::vec4* v;
			glm::mat4* m;
			RTTI** r;
			Scope** t;

			void* vp { nullptr };
		};

		static const std::size_t DatumTypeSizes[static_cast<std::size_t>(DatumTypes::End) + 1];

		std::size_t _size{ 0 };
		std::size_t _capacity{ 0 };
		DatumTypes _type{ DatumTypes::Unknown };
		DatumValues _data;
		bool isExternal {0};
	};

	


}

#include "Datum.inl"