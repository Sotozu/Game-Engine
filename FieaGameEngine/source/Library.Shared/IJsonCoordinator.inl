#include "IJsonCoordinator.h"
#include "IJsonParseHelper.h"

namespace FieaGameEngine
{
	inline std::size_t IJsonCoordinator::NumberOfHelpers() { return _myHelpers.Size(); }

	template <typename HelperType>
	bool IJsonCoordinator::AddHelper()
	{
		auto [helperFound, index] = FindHelper<HelperType>(); //returns pointer to the helper and the index where it resides in the vector of helpers
		if (helperFound != nullptr) { return false; } //helper was found

		HelperType* myNewHelper = new HelperType;
		_myHelpers.PushBack(myNewHelper);
		return true;
	}

	template <typename HelperType>
	bool IJsonCoordinator::RemoveHelper()
	{
		auto[helperFound, index] = FindHelper<HelperType>(); //returns pointer to the helper and the index where it resides in the vector of helpers
		if (helperFound == nullptr) { return false; } //helper was not found

		_myHelpers.RemoveAt(index);
		delete helperFound;
		return true;
	}


	template <typename HelperType>
	IJsonCoordinator::pairType IJsonCoordinator::FindHelper()
	{
		pairType myPair(nullptr, -1); //Set up to be false;

		for (std::size_t index = 0; index < _myHelpers.Size(); ++index) //iterator through the helpers listed
		{
			if (_myHelpers[index]->TypeIdInstance() == HelperType::TypeIdClass()) //If a helper listed has the same RTTI information as the template parameter then we found it!
			{
				myPair.first = _myHelpers[index];
				myPair.second = static_cast<std::int32_t>(index);
				break;
			}
		}

		return myPair;
	}


	template <typename HelperTypeData>
	HelperTypeData* IJsonCoordinator::Parse(std::istream& inputStream)
	{
		if (LinkHelperData<HelperTypeData>() == false) { return nullptr; }; //early out if there is no HelperTypeData that can be used. Hasn't been added.

		Json::Value root;
		inputStream >> root;

		for (std::size_t index = 0; index < _myHelpers.Size(); ++index) //Clean all of the data within the helpers.
		{
			_myHelpers[index]->Initialize();
		}
		_myHelperData->IncrementDepth();
		HelperTypeData* ptrDeserializedData = ParseMembers<HelperTypeData>(root);
		_myHelperData->DecrementDepth();
		return ptrDeserializedData;

	}

	template <typename HelperTypeData>
	HelperTypeData* IJsonCoordinator::ParseFromString(const std::string& stringData)
	{

		std::stringstream inputStream;
		inputStream << stringData;
		return Parse<HelperTypeData>(inputStream);
	}

	template <typename HelperTypeData>
	HelperTypeData* IJsonCoordinator::ParseFromFile(const std::string& filename)
	{
		std::ifstream inputStream(filename, std::ifstream::binary);
		if (!inputStream.is_open()) { throw std::runtime_error("Error opening file."); }

		return Parse<HelperTypeData>(inputStream);
	}
	
	template<typename HelperTypeData>
	bool IJsonCoordinator::LinkHelperData()
	{
		bool isLinked = false;
		//Confirms that a helper exists that has a HelperTypeData that is equivalent to the one we passed in.
		for (std::size_t index = 0; index < _myHelpers.Size(); ++index) //iterate through helpers
		{
			if (HelperTypeData::TypeIdClass() == _myHelpers[index]->GetDataClass().TypeIdInstance()) //RTTI check
			{
				
				if (_myHelperData != nullptr) { _myHelperData->Clear(); }; //Clears out the data in the HelperTypeData that was previously set
				//_myHelpers[index]->Initialize(); 
				_myHelperData = &_myHelpers[index]->GetDataClass(); //links the found HelperTypeData to the coordinator.
				isLinked = true;
				break;
			}
		}
		return isLinked; //if not found then we return nullptr
	}


	template <typename HelperTypeData>
	HelperTypeData* IJsonCoordinator::Parse(const std::string& name, Json::Value& value)
	{

		if (value.isArray()) 
		{
			for (auto& helper : _myHelpers) {
				if (helper->StartHandler(name, value))
				{
					ParseArray<HelperTypeData>(name, value);
					helper->EndHandler(name, value);
					break;
				}
			}
		}
		else if (value.isObject()) 
		{
			for (auto& helper : _myHelpers) {
				if (helper->StartHandler(name, value))
				{
					_myHelperData->IncrementDepth();
					ParseMembers<HelperTypeData>(value);
					_myHelperData->DecrementDepth();
					helper->EndHandler(name, value);
					break;
				}
			}


		}
		else
		{
			for (auto& helper : _myHelpers) {
				if (helper->StartHandler(name, value))
				{
					helper->EndHandler(name, value);
					break;
				}
			}
		}

		return static_cast<HelperTypeData*>(_myHelperData);
	}

	template <typename HelperTypeData>
	void IJsonCoordinator::ParseArray(const std::string& name, Json::Value& value)
	{


		for (std::uint32_t index = 0; index < value.size(); ++index)
		{
			if (value[index].isObject())
			{
				for (auto& helper : _myHelpers) {
					if (helper->StartHandler(name, value[index]))
					{
						ParseMembers<HelperTypeData>(value[index]);
						helper->EndHandler(name, value[index]);
						break;
					}
				}

			}
			else
			{
				for (auto& helper : _myHelpers) {
					if (helper->StartHandler(name, value[index]))
					{
						helper->EndHandler(name, value[index]);
						break;
					}
				}

			}
		}

	}

	template <typename HelperTypeData>
	HelperTypeData* IJsonCoordinator::ParseMembers(Json::Value& root)
	{
		const auto& memberNames = root.getMemberNames();

		for (auto& memberName : memberNames)
		{
			const std::string& key = memberName;
			auto& value = root[memberName];
			Parse<HelperTypeData>(key, value);

		}
		return static_cast<HelperTypeData*>(_myHelperData);
	}

}