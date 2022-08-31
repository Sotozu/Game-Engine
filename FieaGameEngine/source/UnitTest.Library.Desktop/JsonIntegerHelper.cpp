#include "pch.h"
#include "JsonIntegerHelper.h"

namespace UnitTestLibraryDesktop
{
	RTTI_DEFINITIONS(JsonIntegerHelper)

		//Constructor
	JsonIntegerHelper::JsonIntegerHelper() : _helperData(new JsonIntegerHelper::Data(this))
	{}

	//Destructor
	JsonIntegerHelper::~JsonIntegerHelper()
	{
		delete _helperData;
	}

	inline JsonIntegerHelper::Data& JsonIntegerHelper::GetDataClass()
	{
		return *_helperData;
	}

	inline const JsonIntegerHelper::Data& JsonIntegerHelper::GetDataClass() const
	{
		return *_helperData;
	}


	void JsonIntegerHelper::Initialize()
	{
		_helperData->Clear();
	}

	/// <summary>
	/// The syntax for one data element this helper handles is as follows:
	/// 
	/// "Name" : 
	/// {
	///		"type" : "Integer",
	///		"value" : 100
	/// }
	/// 
	/// The syntax for an array this helper handles is as follows;
	/// 
	/// "Name" : 
	/// {
	///		"type" : "Integer",
	///		"value" : [100, 200, 300, 400, 500]
	/// }
	/// 
	/// </summary>
	/// <param name="helperData"></param>
	/// <param name="name"></param>
	/// <param name="value"></param>
	/// <param name="isArray"></param>
	/// <param name="isObject"></param>
	/// <returns></returns>
	bool JsonIntegerHelper::StartHandler(const std::string& key, const Json::Value& value)
	{
		key;
		value;
		return true;
	}


	bool JsonIntegerHelper::EndHandler(const std::string& key, const Json::Value& value)
	{
		key;
		value;
		return true;
	}


	gsl::owner<JsonIntegerHelper*> JsonIntegerHelper::Create()
	{
		return new JsonIntegerHelper();
	}

#pragma endregion

#pragma region JsonFloatHelper::SharedDat
	RTTI_DEFINITIONS(JsonIntegerHelper::Data)

	JsonIntegerHelper::Data::Data(JsonIntegerHelper* owner) : _owner(owner)
	{}

	void JsonIntegerHelper::Data::Clear()
	{
		_data.Clear();
	}

	//FieaGameEngine::Scope& JsonIntegerHelper::Data::GetData()
	//{
	//	return _data;
	//}
	const FieaGameEngine::Vector<std::uint32_t>& JsonIntegerHelper::Data::GetData() const
	{
		return _data;
	}


#pragma endregion
}

