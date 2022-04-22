#pragma once
#include <json/json.h>

#include <gsl/pointers>

#include "RTTI.h"


namespace FieaGameEngine
{
	/// <summary>
	/// Provides polymorphic interface for classes that contains de-serialized data.
	/// Helpers each have their unique binded derived HelperData class. Each of those inherit from this Base HelperData class.
	/// </summary>
	class HelperData : public RTTI
	{
		RTTI_DECLARATIONS(Data, RTTI)
	public:

		/// <summary>
		/// Will clear the HelperData class of all de-serialized data.
		/// </summary>
		virtual void Clear() = 0;

		/// <summary>
		/// Increments the Depth of the HelperData to accurately track it's traversal within Json structures.
		/// </summary>
		void IncrementDepth();

		/// <summary>
		/// Decrements the Depth of the HelperData to accurately track it's traversal within Json structures.
		/// </summary>
		void DecrementDepth();
		/// <summary>
		/// Returns a value representing where the HelperData currently is in respect to it's traversal in a Json structure.
		/// </summary>
		/// <returns></returns>
		std::size_t Depth();

		/// <summary>
		/// Value that represents the depth at which the HelperData is currently in within it's traversal in a Json structure.
		/// </summary>
		std::size_t _depth;

		/// <summary>
		/// Defaulted Constructor
		/// </summary>
		HelperData() = default;
		/// <summary>
		/// Virtual destructor.
		/// </summary>
		virtual ~HelperData() = default;
		/// <summary>
		/// Move Constructor
		/// </summary>
		HelperData(HelperData&&) noexcept = default;
		/// <summary>
		/// Move operator
		/// </summary>
		HelperData& operator=(HelperData&&) noexcept = default;
		/// <summary>
		/// Copy Constructor
		/// </summary>
		HelperData(HelperData&) = default;
		/// <summary>
		/// Copy Operator
		/// </summary>
		HelperData& operator=(HelperData&) = default;
	};

	/// <summary>
	/// Provides polymorphic interface for helpers managed by the IJsonCoordinator.
	/// The IJsonCoordinator has vector of derived ParseHelpers. 
	/// We want to call methods polymorphically from those ParseHelpers so we have them derive from this base IJsonParseHelper
	/// </summary>
	class IJsonParseHelper : public RTTI
	{
		RTTI_DECLARATIONS(IJsonParseHelper, RTTI)


	public:
		/// <summary>
		/// Returns a pointer to a const HelperData class that contains the de-serialized data.
		/// This can be called by the user.
		/// </summary>
	virtual const HelperData& GetDataClass() const = 0;

	/// <summary>
	/// Returns a pointer to a mutable HelperData class that contains the de-serialized data.
	/// This is used for internal methods.
	/// </summary>
	virtual HelperData& GetDataClass() = 0;

		/// <summary>
		/// Clears the contents of the data in the helper.
		/// </summary>
		virtual void Initialize() = 0;

		/// <summary>
		/// Called by a derived ParseHelper and will attempt to start handling the key and value and may de-serialize it depending on helper implementation.
		/// </summary>
		virtual bool StartHandler(const std::string& key, const Json::Value& value) = 0;

		/// <summary>
		/// Called by a derived ParseHelper and will attempt to end the handling of the key and value and may de-serialize it depending on helper implementation.
		/// </summary>
		virtual bool EndHandler(const std::string& key, const Json::Value& value) = 0;

		/// <summary>
		/// Used internally
		/// </summary>
		virtual gsl::owner<IJsonParseHelper*> Create() = 0; 

		/// <summary>
		/// Defaulted Destructor
		/// Helpers are derived from this class and thus the destructor must be marked virtual.
		/// </summary>
		virtual ~IJsonParseHelper() = default;

		/// <summary>
		/// Defaulted Constructor
		/// </summary>
		IJsonParseHelper() = default;

		/// <summary>
		/// Defaulted Move Constructor
		/// </summary>
		IJsonParseHelper(IJsonParseHelper&&) noexcept = default;

		/// <summary>
		/// Defaulted Move Assignment
		/// </summary>
		IJsonParseHelper& operator=(IJsonParseHelper&&) noexcept = default;

		/// <summary>
		/// Defaulted Copy Constructor
		/// </summary>
		IJsonParseHelper(IJsonParseHelper&) = default;

		/// <summary>
		/// Defaulted Copy Assignment
		/// </summary>
		IJsonParseHelper& operator=(IJsonParseHelper&) = default;
		
	};
}


