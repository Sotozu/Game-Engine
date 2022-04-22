#pragma once
#include "IJsonParseHelper.h"
#include "Vector.h"
#include "RTTI.h"


using namespace FieaGameEngine;

namespace UnitTestLibraryDesktop
{
	class JsonIntegerHelper final : public FieaGameEngine::IJsonParseHelper
	{
		RTTI_DECLARATIONS(JsonIntegerHelper, FieaGameEngine::IJsonParseHelper)
	public:

		/// <summary>
		/// valid Json string is de-serialized into a scope within this Data class. 
		/// </summary>
		class Data final : public FieaGameEngine::HelperData
		{
			RTTI_DECLARATIONS(Data, FieaGameEngine::HelperData)
		public:

			using pairType = std::pair<bool, std::string>;
			friend JsonIntegerHelper;

			
			//FieaGameEngine::Scope& GetData();
			const FieaGameEngine::Vector<std::uint32_t>& GetData() const;

			//virtual void Initialize() override;
			void Clear() override;

		protected:
			FieaGameEngine::Vector<std::uint32_t> _data;

			JsonIntegerHelper* _owner;
		private:

			Data(JsonIntegerHelper* owner);
			Data(Data& rhs) = default;
			Data(Data&& rhs) noexcept = default;
			Data& operator=(Data& rhs) = default;
			Data& operator=(Data&& rhs) noexcept = default;
			~Data() = default;

		};

	public:
		JsonIntegerHelper();
		~JsonIntegerHelper();

		JsonIntegerHelper(JsonIntegerHelper& rhs) = default;
		JsonIntegerHelper(JsonIntegerHelper&& rhs) noexcept = default;
		JsonIntegerHelper& operator=(JsonIntegerHelper& rhs) = default;
		JsonIntegerHelper& operator=(JsonIntegerHelper&& rhs) noexcept = default;


		void HandleIntegerDatum(const Json::Value& jsonValue);
		void HandleFloatDatum(const Json::Value& jsonValue);
		void HandleStringDatum(const Json::Value& jsonValue);
		void HandleScopeDatum(const Json::Value& jsonValue);

		/// <summary>
		/// Used for external use to view the DataClass which stores the de-serialized content.
		/// </summary>
		/// <returns></returns>
		const Data& GetDataClass() const override;

	private:
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
		/// Creates a heap allocated JsonIntegerHelper and returns its address.
		/// HelperData associated with this JsonIntegerHelper will be empty regardless if the calling JsonIntegerHelpers HelperData has de-serialized content.
		/// </summary>
		/// <returns>Returns the address of a newly created IJsonParseHelper.</returns>
		gsl::owner<JsonIntegerHelper*> Create() override;


		JsonIntegerHelper::Data* _helperData;
	};
}


