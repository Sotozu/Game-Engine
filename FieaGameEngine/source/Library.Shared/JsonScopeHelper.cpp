#include "pch.h"
#include "JsonScopeHelper.h"

namespace FieaGameEngine
{


	RTTI_DEFINITIONS(JsonScopeHelper)

		//Constructor
		JsonScopeHelper::JsonScopeHelper() : _helperData(new JsonScopeHelper::Data(this))
	{
	
	}

	//Destructor
	JsonScopeHelper::~JsonScopeHelper()
	{
		delete _helperData;
	}

	inline JsonScopeHelper::Data& JsonScopeHelper::GetDataClass()
	{
		return *_helperData;
	}

	inline const JsonScopeHelper::Data& JsonScopeHelper::GetDataClass() const
	{
		return *_helperData;
	}


	void JsonScopeHelper::Initialize()
	{
		_helperData->Clear();
	}

	bool JsonScopeHelper::StartHandler(const std::string& name, const Json::Value& value)
	{
		if (_helperData->_data == nullptr) //to set the _data variable to gameObject before StartHandler is ever called. This is a temporary fix.
		{ 
			_helperData->_data = new GameObject;
			_helperData->_currentScope = _helperData->_data;
		} 

		if (name == "class")
		{
			_stackContext.Top()._className = value.asString();
		}
		else if (name == "type")
		{

			auto [it, found] = _UMapDatumTypes.Find(value.asString());
			_stackContext.Top()._type = (*it).second;
			std::string type = value.asString();
		}
		else if (name == "value")
		{
			if (value.isArray()) 
			{
				_stackContext.Top()._isArray = true;
			}

			switch (_stackContext.Top()._type)
			{
			case DatumTypes::Integer:
				HandleIntegerDatum(value);
				break;
			case DatumTypes::Boolean:
				HandleBooleanDatum(value);
				break;
			case DatumTypes::Float:
				HandleFloatDatum(value);
				break;
			case DatumTypes::String:
				HandleStringDatum(value);
				break;
			case DatumTypes::Vector4:
				HandleVectorDatum(value);
				break;
			case DatumTypes::Matrix:
				HandleMatrixDatum(value);
				break;
			case DatumTypes::Table:
				HandleScopeDatum(value);
				break;
			}
		}
		else
		{
			Context myContext(name);
			_stackContext.Push(std::move(myContext));
		}

		return true;
	}


	void JsonScopeHelper::HandleIntegerDatum(const Json::Value& jsonValue)
	{

		if (jsonValue.isArray()) //If the type passed in is an array then we want to set the size of the datum equal to that of the array.
		{
			Datum* foundDatum = _helperData->_currentScope->Find(_stackContext.Top()._datumKey);// search the current context for the key being passed in. 
			if (foundDatum == nullptr) //If it already exists then do nothing. This means it's Auxiliary.
			{
				Datum& newlyCreatedDatum = _helperData->_currentScope->Append(_stackContext.Top()._datumKey);
				newlyCreatedDatum.SetType(_stackContext.Top()._type);
				newlyCreatedDatum.Reserve(jsonValue.size());
			}

		}
		else
		{
			Datum* foundDatum = _helperData->_currentScope->Find(_stackContext.Top()._datumKey); //Search the current context for the datum


			if (foundDatum == nullptr) //if it doest not exist then append it. This means it's Axillary.
			{

				Datum& newlyCreatedDatum = _helperData->_currentScope->Append(_stackContext.Top()._datumKey);
				newlyCreatedDatum.PushBack(jsonValue.asInt());
			}
			else //If it does exist then we need to ask it if it's auxiliary
			{
				if (foundDatum->IsExternal())
				{
					foundDatum->Set(jsonValue.asInt(), _stackContext.Top()._currentElementIndex++);
				}
				else
				{
					foundDatum->PushBack(jsonValue.asInt());
				}

			}
		}

	}

	void JsonScopeHelper::HandleBooleanDatum(const Json::Value& jsonValue)
	{

		if (jsonValue.isArray()) //If the type passed in is an array then we want to set the size of the datum equal to that of the array.
		{
			Datum* foundDatum = _helperData->_currentScope->Find(_stackContext.Top()._datumKey);// search the current context for the key being passed in. 
			if (foundDatum == nullptr) //If it already exists then do nothing. This means it's Auxiliary.
			{
				Datum& newlyCreatedDatum = _helperData->_currentScope->Append(_stackContext.Top()._datumKey);
				newlyCreatedDatum.SetType(_stackContext.Top()._type);
				newlyCreatedDatum.Reserve(jsonValue.size());
			}

		}
		else
		{
			Datum* foundDatum = _helperData->_currentScope->Find(_stackContext.Top()._datumKey); //Search the current context for the datum


			if (foundDatum == nullptr) //if it doest not exist then append it. This means it's Axillary.
			{

				Datum& newlyCreatedDatum = _helperData->_currentScope->Append(_stackContext.Top()._datumKey);
				newlyCreatedDatum.PushBack(jsonValue.asBool());
			}
			else //If it does exist then we need to ask it if it's auxiliary
			{
				if (foundDatum->IsExternal())
				{
					foundDatum->Set(jsonValue.asBool(), _stackContext.Top()._currentElementIndex++);
				}
				else
				{
					foundDatum->PushBack(jsonValue.asBool());
				}

			}
		}
	}

	void JsonScopeHelper::HandleFloatDatum(const Json::Value& jsonValue)
	{
		if (jsonValue.isArray()) //If the type passed in is an array then we want to set the size of the datum equal to that of the array.
		{
			Datum* foundDatum = _helperData->_currentScope->Find(_stackContext.Top()._datumKey);
			if (foundDatum == nullptr)
			{
				Datum& newlyCreatedDatum = _helperData->_currentScope->Append(_stackContext.Top()._datumKey);
				newlyCreatedDatum.SetType(_stackContext.Top()._type);
				newlyCreatedDatum.Reserve(jsonValue.size());
			}

		}
		else
		{
			Datum* foundDatum = _helperData->_currentScope->Find(_stackContext.Top()._datumKey);
			if (foundDatum == nullptr)
			{
				Datum& newlyCreatedDatum = _helperData->_currentScope->Append(_stackContext.Top()._datumKey);
				newlyCreatedDatum.PushBack(jsonValue.asFloat());
			}
			else
			{
				if (foundDatum->IsExternal())
				{
					foundDatum->Set(jsonValue.asFloat(), _stackContext.Top()._currentElementIndex++);
				}
				else
				{
					foundDatum->PushBack(jsonValue.asFloat());
				}

			}
		}



	}
	void JsonScopeHelper::HandleStringDatum(const Json::Value& jsonValue)
	{
		if (jsonValue.isArray()) //If the type passed in is an array then we want to set the size of the datum equal to that of the array.
		{


			Datum* foundDatum = _helperData->_currentScope->Find(_stackContext.Top()._datumKey);
			if (foundDatum == nullptr)
			{
				Datum& newlyCreatedDatum = _helperData->_currentScope->Append(_stackContext.Top()._datumKey);
				newlyCreatedDatum.SetType(_stackContext.Top()._type);
				newlyCreatedDatum.Reserve(jsonValue.size());
			}

		}
		else
		{

			Datum* foundDatum = _helperData->_currentScope->Find(_stackContext.Top()._datumKey);
			if (foundDatum == nullptr)
			{
				Datum& newlyCreatedDatum = _helperData->_currentScope->Append(_stackContext.Top()._datumKey);
				newlyCreatedDatum.PushBack(jsonValue.asString());
			}
			else
			{
				if (foundDatum->IsExternal())
				{
					foundDatum->Set(jsonValue.asString(), _stackContext.Top()._currentElementIndex++);
				}
				else
				{
					foundDatum->PushBack(jsonValue.asString());
				}

			}
		}

	}
	void JsonScopeHelper::HandleVectorDatum(const Json::Value& jsonValue)
	{
		if (jsonValue.isArray()) //If the type passed in is an array then we want to set the size of the datum equal to that of the array.
		{


			Datum* foundDatum = _helperData->_currentScope->Find(_stackContext.Top()._datumKey);
			if (foundDatum == nullptr)
			{
				Datum& newlyCreatedDatum = _helperData->_currentScope->Append(_stackContext.Top()._datumKey);
				newlyCreatedDatum.SetType(_stackContext.Top()._type);
				newlyCreatedDatum.Reserve(jsonValue.size());
			}

		}
		else
		{

			Datum* foundDatum = _helperData->_currentScope->Find(_stackContext.Top()._datumKey);
			if (foundDatum == nullptr)
			{
				Datum& newlyCreatedDatum = _helperData->_currentScope->Append(_stackContext.Top()._datumKey);
				newlyCreatedDatum.SetType(DatumTypes::Vector4);
				newlyCreatedDatum.PushBackFromString(jsonValue.asString());
			}
			else
			{
				if (foundDatum->IsExternal())
				{
					foundDatum->SetFromString(jsonValue.asString(), _stackContext.Top()._currentElementIndex++);
				}
				else
				{
					foundDatum->PushBackFromString(jsonValue.asString());
				}
			}
		}

	}
	void JsonScopeHelper::HandleMatrixDatum(const Json::Value& jsonValue)
	{
		if (jsonValue.isArray()) //If the type passed in is an array then we want to set the size of the datum equal to that of the array.
		{


			Datum* foundDatum = _helperData->_currentScope->Find(_stackContext.Top()._datumKey);
			if (foundDatum == nullptr)
			{
				Datum& newlyCreatedDatum = _helperData->_currentScope->Append(_stackContext.Top()._datumKey);
				newlyCreatedDatum.SetType(_stackContext.Top()._type);
				newlyCreatedDatum.Reserve(jsonValue.size());
			}

		}
		else
		{

			Datum* foundDatum = _helperData->_currentScope->Find(_stackContext.Top()._datumKey);
			if (foundDatum == nullptr)
			{
				Datum& newlyCreatedDatum = _helperData->_currentScope->Append(_stackContext.Top()._datumKey);
				newlyCreatedDatum.SetType(DatumTypes::Matrix);
				newlyCreatedDatum.PushBackFromString(jsonValue.asString());
			}
			else
			{
				if (foundDatum->IsExternal())
				{
					foundDatum->SetFromString(jsonValue.asString(), _stackContext.Top()._currentElementIndex++);
				}
				else
				{
					foundDatum->PushBackFromString(jsonValue.asString());
				}

			}
		}
	}

	void JsonScopeHelper::HandleScopeDatum(const Json::Value& jsonValue)
	{
		Context myContext = _stackContext.Top();
		_stackContext.Top()._datumKey;
		_stackContext.Top()._className;

		if (!jsonValue.isArray())
		{

			Scope* scopeToTraverse = nullptr;

			if (myContext._datumKey == "_children")
			{
				scopeToTraverse = Factory<Scope>::Create(jsonValue["class"].asString()); //if this is an array of scopes

				assert(scopeToTraverse->As<GameObject>() != nullptr);

				_helperData->_currentScope->Adopt(myContext._datumKey, *scopeToTraverse);
				_helperData->_currentScope = scopeToTraverse;

			}
			else if (myContext._datumKey == "_actions")
			{
				scopeToTraverse = Factory<Action>::Create(jsonValue["class"].asString()); //if this is an array of scopes
				assert(scopeToTraverse->As<Action>() != nullptr);

				_helperData->_currentScope->Adopt(myContext._datumKey, *scopeToTraverse);
				_helperData->_currentScope = scopeToTraverse;

			}
			else
			{ //This needs to know how to distinguish between Actions and Scope

				scopeToTraverse = Factory<Action>::Create(jsonValue["class"].asString()); //if this is an array of scopes
				

				if (scopeToTraverse != nullptr) 
				{
				}
				else if ((scopeToTraverse = Factory<GameObject>::Create(jsonValue["class"].asString())) != nullptr)
				{

				}
				else if ((scopeToTraverse = Factory<Scope>::Create(jsonValue["class"].asString())) != nullptr)
				{
				}
				else
				{
					scopeToTraverse = new Scope;
				}

				_helperData->_currentScope->Adopt(myContext._datumKey, *scopeToTraverse);
				_helperData->_currentScope = scopeToTraverse;

			}
	
		}
		


	}


	bool JsonScopeHelper::EndHandler(const std::string& name, const Json::Value& value)
	{
		if (!_stackContext.IsEmpty()) //Will be empty when in the context is the root scope. 
		{
			Context& myContext = _stackContext.Top();

			if (&myContext._datumKey == &name) { //we want the address of the key that way they are the VERY SAME KEY

				if (myContext._type == DatumTypes::Table && !myContext._isArray) //If this name belongs to a scope that means we are done with it.
				{

					Scope* parentScope = _helperData->_currentScope->GetParent();
					//parentScope->Adopt(_stackContext.Top()._datumKey, *_helperData->_currentScope);
					//A object that a factory created and has been populated by the de serialization of this process
					//will now be complete! We need to have it adopted by the parent scope.
					_helperData->_currentScope = parentScope;
				}
				_stackContext.Pop();
			}
			else if (value.isObject() && myContext._type == DatumTypes::Table && myContext._isArray) { //This handles array of scopes
				Scope* parentScope = _helperData->_currentScope->GetParent();
				//parentScope->Adopt(_stackContext.Top()._datumKey, *_helperData->_currentScope);
				_helperData->_currentScope = parentScope;
			}

		}
		return true;
	}


	gsl::owner<JsonScopeHelper*> JsonScopeHelper::Create()
	{
		return new JsonScopeHelper();
	}

#pragma endregion

#pragma region JsonFloatHelper::Data
	RTTI_DEFINITIONS(JsonScopeHelper::Data)

		JsonScopeHelper::Data::Data(JsonScopeHelper* owner) : _owner{ owner },
		_data{ nullptr } // TO DO: CAN BE LEFT TO NULLPTR FOR OTHER BEHAVIOR. LOOK AT MESSAGES SENT TO PAUL ON TEAMS ON 04/06/2022
	{
		_currentScope = _data ; //TO DO: CAN DELETE THIS.
	}

	void JsonScopeHelper::Data::Clear()
	{
		if (_data != nullptr) {_data->Clear();}
	}

	FieaGameEngine::Scope* JsonScopeHelper::Data::GetData() const
	{
		return _data;
	}

	JsonScopeHelper::Data::~Data()
	{
		delete _data;
	}

#pragma endregion

#pragma region JsonFloatHelper::Context
	JsonScopeHelper::Context::Context(const std::string& datumKey) :
		_datumKey{datumKey},
		_type{},
		_isArray{false},
		_isArrayElement{false},
		_currentElementIndex{0},
		_className{""},
		_context{nullptr}

	{}
#pragma endregion
}

