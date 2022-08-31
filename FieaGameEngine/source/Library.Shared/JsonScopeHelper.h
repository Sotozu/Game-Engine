#pragma once
#include "IJsonParseHelper.h"
#include "Stack.h"
#include "Scope.h"
#include "RTTI.h"
#include "Factory.h"
#include "UnorderedMap.h"
#include "GameObject.h"


using namespace FieaGameEngine;

namespace FieaGameEngine
{
	class JsonScopeHelper final : public FieaGameEngine::IJsonParseHelper
	{
		RTTI_DECLARATIONS(JsonScopeHelper, FieaGameEngine::IJsonParseHelper)
	public:

		/// <summary>
		/// valid Json string is de-serialized into a scope within this Data class. 
		/// </summary>
		class Data final : public FieaGameEngine::HelperData
		{
			RTTI_DECLARATIONS(Data, FieaGameEngine::HelperData)
		public:

			using pairType = std::pair<bool, std::string>;
			friend JsonScopeHelper;

			/// <summary>
			/// Retrieved a reference to the scope stored within the class
			/// </summary>
			/// <returns>A mutable pointer to a Scope Object</returns>
			FieaGameEngine::Scope* GetData() const;

			/// <summary>
			/// Clears the contents stored in the this Data class
			/// </summary>
			void Clear() override;

		protected:
			FieaGameEngine::Scope* _data;
			JsonScopeHelper* _owner;
		private:
			FieaGameEngine::Scope* _currentScope;
			Data(JsonScopeHelper* owner);
			Data(Data& rhs) = default;
			Data(Data && rhs) noexcept = default;
			Data& operator=(Data & rhs) = default;
			Data& operator=(Data && rhs) noexcept = default;
			~Data();

		};


	public:
		/// <summary>
		/// Default constructor.
		/// Binds a HelperData object that this helper can de-serialize content into.
		/// </summary>
		JsonScopeHelper();
		~JsonScopeHelper();

		JsonScopeHelper(JsonScopeHelper& rhs) = default;
		JsonScopeHelper(JsonScopeHelper&& rhs) noexcept = default;
		JsonScopeHelper& operator=(JsonScopeHelper& rhs) = default;
		JsonScopeHelper& operator=(JsonScopeHelper&& rhs) noexcept = default;

		/// <summary>
		/// De-serializes Json content into an Integer Datum for Internal use
		/// </summary>
		/// <param name="jsonValue">Json value to deserialize into an Integer Datum</param>
		void HandleIntegerDatum(const Json::Value& jsonValue);

		
		void HandleBooleanDatum(const Json::Value& jsonValue);

		/// <summary>
		/// De-serializes Json content into an Float Datum for Internal use
		/// </summary>
		/// <param name="jsonValue">Json value to deserialize into an Float Datum</param>
		void HandleFloatDatum(const Json::Value& jsonValue);

		/// <summary>
		/// De-serializes Json content into an String Datum for Internal use
		/// </summary>
		/// <param name="jsonValue">Json value to deserialize into an String Datum</param>
		void HandleStringDatum(const Json::Value& jsonValue);

		/// <summary>
		/// De-serializes Json content into an Vector4 Datum for Internal use
		/// </summary>
		/// <param name="jsonValue">Json value to deserialize into an Vector4 Datum</param>
		void HandleVectorDatum(const Json::Value& jsonValue);

		/// <summary>
		/// De-serializes Json content into an Matrix Datum for Internal use
		/// </summary>
		/// <param name="jsonValue">Json value to deserialize into an Matrix Datum</param>
		void HandleMatrixDatum(const Json::Value& jsonValue);

		/// <summary>
		/// De-serializes Json content into a Scope for Internal use
		/// </summary>
		/// <param name="jsonValue">Json value to deserialize into a Scope</param>
		void HandleScopeDatum(const Json::Value& jsonValue);

		/// <summary>
		/// Used for external use to view the DataClass which stores the de-serialized content.
		/// </summary>
		/// <returns></returns>
		const Data& GetDataClass() const override;
	private:
		


		/// <summary>
		/// Type of struct used
		/// When the handler confirms that the object being passed in is of type scope it then adds 
		/// </summary>
		class Context final
		{
		public:
			Context(const std::string& datumKey);

			const std::string& _datumKey;
			DatumTypes _type;
			bool _isArray;
			bool _isArrayElement;
			std::size_t _currentElementIndex;
			std::string _className;
			Scope* _context{ nullptr };
		};

		/// <summary>
		/// Used for internal use in the program to access the DataClass which stores the de-serialized content.
		/// </summary>
		Data& GetDataClass() override;

		/// <summary>
		/// Resets the contents of this helper
		/// </summary>
		void Initialize() override;

		/// <summary>
		/// Starts handling a Json key value pair for de-serialization.
		/// </summary>
		/// <param name="key">The "key" associated with the Json value</param>
		/// <param name="value">The "Json" value to be de-serialized</param>
		/// <returns>
		/// True if it can correctly start handling the de-serialization of the content passed
		/// False if it can't correctly start handling the de-serialization of the content passed
		/// </returns>
		bool StartHandler(const std::string& key, const Json::Value& value) override;

		/// <summary>
		/// Ends handling a Json key value pair for de-serialization.
		/// </summary>
		/// <param name="key">The "key" associated with the Json value</param>
		/// <param name="value">The "Json" value to be de-serialized</param>
		/// <returns>
		/// True if it can correctly end handling the de-serialization of the content passed
		/// False if it can't correctly end handling the de-serialization of the content passed
		/// </returns>
		bool EndHandler(const std::string& key, const Json::Value& value) override;

		/// <summary>
		/// Creates a heap allocated JsonScopeHelper and returns its address.
		/// HelperData associated with this JsonScopeHelper will be empty regardless if the calling JsonScopeHelpers HelperData has de-serialized content.
		/// </summary>
		/// <returns>Returns the address of a newly created IJsonParseHelper.</returns>
		gsl::owner<JsonScopeHelper*> Create() override;

		private:
		JsonScopeHelper::Data* _helperData;

		/// <summary>
		/// When the helper determines that the data in processing is of type scope we add it to the Stack.
		/// This will be used later to traverse which layer (context) of the scope we are working in.
		/// </summary>
		Stack<Context> _stackContext;

		inline static UnorderedMap<const std::string, const DatumTypes> _UMapDatumTypes = {
		{"unknown"s, DatumTypes::Unknown},
		{"integer"s, DatumTypes::Integer},
		{"bool"s, DatumTypes::Boolean},
		{"float"s, DatumTypes::Float},
		{"string"s, DatumTypes::String},
		{"vector"s, DatumTypes::Vector4},
		{"matrix"s, DatumTypes::Matrix},
		{"scope"s, DatumTypes::Table},
		};
	};
}


