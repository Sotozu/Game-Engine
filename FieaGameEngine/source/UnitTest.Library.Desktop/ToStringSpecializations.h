#pragma once
#include "Foo.h"
#include "SList.h"
#include "Vector.h"
#include "UnorderedMap.h"
#include "CppUnitTest.h"
#include "Datum.h"
#include "Scope.h"
#include "AttributedFoo.h"
#include "JsonScopeHelper.h"
#include "Light.h"

using namespace UnitTestLibraryDesktop;
using namespace FieaGameEngine;
using namespace std;
using namespace std::string_literals;

//SList
namespace Microsoft::VisualStudio::CppUnitTestFramework
{
	template<typename T> std::wstring ToString(const SList<T>& t) { RETURN_WIDE_STRING(t.Size()); }
	template<typename T> std::wstring ToString(const SList<T>* t) { RETURN_WIDE_STRING(t); }
	template<typename T> std::wstring ToString(SList<T> t) { RETURN_WIDE_STRING(t); }
}

//Foo
namespace Microsoft::VisualStudio::CppUnitTestFramework
{
	template<>inline std::wstring ToString<Foo>(const Foo& t) { RETURN_WIDE_STRING(t.Data()); }
	template<>inline std::wstring ToString<Foo>(const Foo* t) { RETURN_WIDE_STRING(t); }
	template<>inline std::wstring ToString<Foo>(Foo* t) { RETURN_WIDE_STRING(t); }
}

//pair<std::string, Foo>
namespace Microsoft::VisualStudio::CppUnitTestFramework
{
#pragma region pair<const std::string, Foo>
	template<>
	inline std::wstring ToString<pair<const std::string, Foo>>(const pair<const std::string, Foo>& t) 
	{ 
		std::wstring string_value = std::to_wstring(0) + std::to_wstring(t.second.Data()); //my first is a string figure out how to con
		RETURN_WIDE_STRING(string_value);
	}

	template<>
	inline std::wstring ToString<pair< const std::string, Foo>>(const pair<const std::string, Foo>* t)
	{
		std::wstring string_value = std::to_wstring(0) + std::to_wstring((*t).second.Data()); //my first is a string figure out how to con
		RETURN_WIDE_STRING(string_value);
	}

	template<>
	inline std::wstring ToString<pair<const std::string, Foo>>(pair<const std::string, Foo>* t)
	{
		std::wstring string_value = std::to_wstring(0) + std::to_wstring((*t).second.Data()); //my first is a string figure out how to con
		RETURN_WIDE_STRING(string_value);
	}
#pragma endregion
#pragma region pair<const Foo, std::string>
	template<>
	inline std::wstring ToString<pair<const Foo, std::string>>(const pair<const Foo, std::string>& t)
	{
		std::wstring string_value = std::to_wstring(t.first.Data()) + std::to_wstring(0); //my first is a string figure out how to con
		RETURN_WIDE_STRING(string_value);
	}

	template<>
	inline std::wstring ToString<pair<const Foo, std::string>>(const pair<const Foo, std::string>* t)
	{
		std::wstring string_value = std::to_wstring((*t).first.Data()) + std::to_wstring(0); //my first is a string figure out how to con
		RETURN_WIDE_STRING(string_value);
	}

	template<>
	inline std::wstring ToString<pair<const Foo, std::string>>(pair<const Foo, std::string>* t)
	{
		std::wstring string_value = std::to_wstring((*t).first.Data()) + std::to_wstring(0); //my first is a string figure out how to con
		RETURN_WIDE_STRING(string_value);
	}
#pragma endregion
}

//pair<Iterator, bool>
//pair<ConstIterator, bool>
namespace Microsoft::VisualStudio::CppUnitTestFramework
{
#pragma region pair<UnorderedMap<const std::string, Foo>::Iterator, bool>
	template<>
	inline std::wstring ToString<pair<UnorderedMap<const std::string, Foo>::Iterator, bool>>(const pair<UnorderedMap<const std::string, Foo>::Iterator, bool>& t)
	{
		std::wstring string_value = std::to_wstring(0) + std::to_wstring(t.second); //my first is a string figure out how to con
		RETURN_WIDE_STRING(string_value);
	}

	template<>
	inline std::wstring ToString<pair<UnorderedMap<const std::string, Foo>::Iterator, bool>>(const pair<UnorderedMap<const std::string, Foo>::Iterator, bool>* t)
	{
		std::wstring string_value = std::to_wstring(0) + std::to_wstring((*t).second); //my first is a string figure out how to con
		RETURN_WIDE_STRING(string_value);
	}

	template<>
	inline std::wstring ToString<pair<UnorderedMap<const std::string, Foo>::Iterator, bool>>(pair<UnorderedMap<const std::string, Foo>::Iterator, bool>* t)
	{
		std::wstring string_value = std::to_wstring(0) + std::to_wstring((*t).second); //my first is a string figure out how to con
		RETURN_WIDE_STRING(string_value);
	}
#pragma endregion
#pragma region pair<UnorderedMap<const std::string, Foo>::ConstIterator, bool>
	template<>
	inline std::wstring ToString<pair<UnorderedMap<const std::string, Foo>::ConstIterator, bool>>(const pair<UnorderedMap<const std::string, Foo>::ConstIterator, bool>& t)
	{
		std::wstring string_value = std::to_wstring(0) + std::to_wstring(t.second); //my first is a string figure out how to con
		RETURN_WIDE_STRING(string_value);
	}

	template<>
	inline std::wstring ToString<pair<UnorderedMap<const std::string, Foo>::ConstIterator, bool>>(const pair<UnorderedMap<const std::string, Foo>::ConstIterator, bool>* t)
	{
		std::wstring string_value = std::to_wstring(0) + std::to_wstring((*t).second); //my first is a string figure out how to con
		RETURN_WIDE_STRING(string_value);
	}

	template<>
	inline std::wstring ToString<pair<UnorderedMap<const std::string, Foo>::ConstIterator, bool>>(pair<UnorderedMap<const std::string, Foo>::ConstIterator, bool>* t)
	{
		std::wstring string_value = std::to_wstring(0) + std::to_wstring((*t).second); //my first is a string figure out how to con
		RETURN_WIDE_STRING(string_value);
	}

#pragma endregion

#pragma region pair<UnorderedMap<const std::string, Foo>::Iterator, bool>
	template<>
	inline std::wstring ToString<pair<UnorderedMap<const Foo, std::string>::Iterator, bool>>(const pair<UnorderedMap<const Foo, std::string>::Iterator, bool>& t)
	{
		std::wstring string_value = std::to_wstring(0) + std::to_wstring(t.second); //my first is a string figure out how to con
		RETURN_WIDE_STRING(string_value);
	}

	template<>
	inline std::wstring ToString<pair<UnorderedMap<const Foo, std::string>::Iterator, bool>>(const pair<UnorderedMap<const Foo, std::string>::Iterator, bool>* t)
	{
		std::wstring string_value = std::to_wstring(0) + std::to_wstring((*t).second); //my first is a string figure out how to con
		RETURN_WIDE_STRING(string_value);
	}

	template<>
	inline std::wstring ToString<pair<UnorderedMap<const Foo, std::string>::Iterator, bool>>(pair<UnorderedMap<const Foo, std::string>::Iterator, bool>* t)
	{
		std::wstring string_value = std::to_wstring(0) + std::to_wstring((*t).second); //my first is a string figure out how to con
		RETURN_WIDE_STRING(string_value);
	}
#pragma endregion
#pragma region pair<UnorderedMap<const std::string, Foo>::Iterator, bool>
	template<>
	inline std::wstring ToString<pair<UnorderedMap<const Foo, std::string>::ConstIterator, bool>>(const pair<UnorderedMap<const Foo, std::string>::ConstIterator, bool>& t)
	{
		std::wstring string_value = std::to_wstring(0) + std::to_wstring(t.second); //my first is a string figure out how to con
		RETURN_WIDE_STRING(string_value);
	}

	template<>
	inline std::wstring ToString<pair<UnorderedMap<const Foo, std::string>::ConstIterator, bool>>(const pair<UnorderedMap<const Foo, std::string>::ConstIterator, bool>* t)
	{
		std::wstring string_value = std::to_wstring(0) + std::to_wstring((*t).second); //my first is a string figure out how to con
		RETURN_WIDE_STRING(string_value);
	}

	template<>
	inline std::wstring ToString<pair<UnorderedMap<const Foo, std::string>::ConstIterator, bool>>(pair<UnorderedMap<const Foo, std::string>::ConstIterator, bool>* t)
	{
		std::wstring string_value = std::to_wstring(0) + std::to_wstring((*t).second); //my first is a string figure out how to con
		RETURN_WIDE_STRING(string_value);
	}

#pragma endregion
}

//UnorderedMap<const std::string, Foo>
namespace Microsoft::VisualStudio::CppUnitTestFramework
{
#pragma region UnorderedMap<const std::string, Foo>::Iterator
	template<>
	inline std::wstring ToString<UnorderedMap<const std::string, Foo>::Iterator>(const UnorderedMap<const std::string, Foo>::Iterator& t)
	{
		t;
		std::wstring string_value = std::to_wstring(0); //my first is a string figure out how to con
		RETURN_WIDE_STRING(string_value);
	}

	template<>
	inline std::wstring ToString<UnorderedMap<const std::string, Foo>::Iterator>(const UnorderedMap<const std::string, Foo>::Iterator* t)
	{
		t;
		std::wstring string_value = std::to_wstring(0); //my first is a string figure out how to con
		RETURN_WIDE_STRING(string_value);
	}

	template<>
	inline std::wstring ToString<UnorderedMap<const std::string, Foo>::Iterator>(UnorderedMap<const std::string, Foo>::Iterator* t)
	{
		t;
		std::wstring string_value = std::to_wstring(0); //my first is a string figure out how to con
		RETURN_WIDE_STRING(string_value);
	}
#pragma endregion
#pragma region UnorderedMap<const std::string, Foo>::ConstIterator

	template<>
	inline std::wstring ToString<UnorderedMap<const std::string, Foo>::ConstIterator>(const UnorderedMap<const std::string, Foo>::ConstIterator& t)
	{
		t;
		std::wstring string_value = std::to_wstring(0); //my first is a string figure out how to con
		RETURN_WIDE_STRING(string_value);
	}

	template<>
	inline std::wstring ToString<UnorderedMap<const std::string, Foo>::ConstIterator>(const UnorderedMap<const std::string, Foo>::ConstIterator* t)
	{
		t;
		std::wstring string_value = std::to_wstring(0); //my first is a string figure out how to con
		RETURN_WIDE_STRING(string_value);
	}

	template<>
	inline std::wstring ToString<UnorderedMap<const std::string, Foo>::ConstIterator>(UnorderedMap<const std::string, Foo>::ConstIterator* t)
	{
		t;
		std::wstring string_value = std::to_wstring(0); //my first is a string figure out how to con
		RETURN_WIDE_STRING(string_value);
	}
#pragma endregion

#pragma region UnorderedMap<const Foo, std::string>::Iterator
	template<>
	inline std::wstring ToString<UnorderedMap<const Foo, std::string>::Iterator>(const UnorderedMap<const Foo, std::string>::Iterator& t)
	{
		t;
		std::wstring string_value = std::to_wstring(0); //my first is a string figure out how to con
		RETURN_WIDE_STRING(string_value);
	}

	template<>
	inline std::wstring ToString<UnorderedMap<const Foo, std::string>::Iterator>(const UnorderedMap<const Foo, std::string>::Iterator* t)
	{
		t;
		std::wstring string_value = std::to_wstring(0); //my first is a string figure out how to con
		RETURN_WIDE_STRING(string_value);
	}

	template<>
	inline std::wstring ToString<UnorderedMap<const Foo, std::string>::Iterator>(UnorderedMap<const Foo, std::string>::Iterator* t)
	{
		t;
		std::wstring string_value = std::to_wstring(0); //my first is a string figure out how to con
		RETURN_WIDE_STRING(string_value);
	}
#pragma endregion
#pragma region UnorderedMap<const Foo, std::string>::ConstIterator
	template<>
	inline std::wstring ToString<UnorderedMap<const Foo, std::string>::ConstIterator>(const UnorderedMap<const Foo, std::string>::ConstIterator& t)
	{
		t;
		std::wstring string_value = std::to_wstring(0); //my first is a string figure out how to con
		RETURN_WIDE_STRING(string_value);
	}

	template<>
	inline std::wstring ToString<UnorderedMap<const Foo, std::string>::ConstIterator>(const UnorderedMap<const Foo, std::string>::ConstIterator* t)
	{
		t;
		std::wstring string_value = std::to_wstring(0); //my first is a string figure out how to con
		RETURN_WIDE_STRING(string_value);
	}

	template<>
	inline std::wstring ToString<UnorderedMap<const Foo, std::string>::ConstIterator>(UnorderedMap<const Foo, std::string>::ConstIterator* t)
	{
		t;
		std::wstring string_value = std::to_wstring(0); //my first is a string figure out how to con
		RETURN_WIDE_STRING(string_value);
	}
#pragma endregion
}

//SList<Foo>::Iterator
namespace Microsoft::VisualStudio::CppUnitTestFramework
{
	
#pragma region SList<Foo>
	template<> 
	inline std::wstring ToString<SList<Foo>::Iterator>(const SList<Foo>::Iterator& q)
	{
		try 
		{
			return ToString(*q);
		}
		catch (const std::runtime_error&)
		{
			return L"end()"s;
		}
	}

	template<>
	inline std::wstring ToString<SList<Foo>::Iterator>(const SList<Foo>::Iterator* q)
	{
		return ToString(q);
	}

	template<>
	inline std::wstring ToString<SList<Foo>::ConstIterator>(const SList<Foo>::ConstIterator& q)
	{
		try
		{
			return ToString(*q);
		}
		catch (const std::runtime_error&)
		{
			return L"end()"s;
		}
	}

	template<>
	inline std::wstring ToString<SList<Foo>::ConstIterator>(const SList<Foo>::ConstIterator* q)
	{
		return ToString(q);
	}
#pragma endregion

}

//Vector<Foo>::Iterator
namespace Microsoft::VisualStudio::CppUnitTestFramework
{
	template<>
	inline std::wstring ToString<Vector<Foo>::Iterator>(const Vector<Foo>::Iterator& q)
	{
		try
		{
			return ToString(*q);
		}
		catch (const std::runtime_error&)
		{
			return L"end()"s;
		}
	}

	template<>
	inline std::wstring ToString<Vector<Foo>::Iterator>(const Vector<Foo>::Iterator* q)
	{
		return ToString(q);
	}

	template<>
	inline std::wstring ToString<Vector<Foo>::ConstIterator>(const Vector<Foo>::ConstIterator& q)
	{
		try
		{
			return ToString(*q);
		}
		catch (const std::runtime_error&)
		{
			return L"end()"s;
		}
	}

	template<>
	inline std::wstring ToString<Vector<Foo>::ConstIterator>(const Vector<Foo>::ConstIterator* q)
	{
		return ToString(q);
	}
}

//FieaGameEngine::Datum
namespace Microsoft::VisualStudio::CppUnitTestFramework
{
	template<>
	inline std::wstring ToString<Datum>(const Datum& q)
	{
		std::wstring string_value = std::to_wstring(q.Size()); //my first is a string figure out how to con
		RETURN_WIDE_STRING(string_value);
	}

	template<>
	inline std::wstring ToString<Datum>(const Datum* q)
	{
		std::wstring string_value = std::to_wstring(q->Size()); //my first is a string figure out how to con
		RETURN_WIDE_STRING(string_value);
	}

	template<>
	inline std::wstring ToString<Datum>(Datum* q)
	{
		std::wstring string_value = std::to_wstring(q->Size()); //my first is a string figure out how to con
		RETURN_WIDE_STRING(string_value);
	}

}

//FieaGameEngine::RTTI*
//Special case here because RTTI* can be set to null.
namespace Microsoft::VisualStudio::CppUnitTestFramework
{
	template<>
	inline std::wstring ToString<RTTI>(const RTTI& q)
	{
		std::wstring string_value = std::to_wstring(q.TypeIdInstance()); //my first is a string figure out how to con
		RETURN_WIDE_STRING(string_value);
	}

	template<>
	inline std::wstring ToString<RTTI>(const RTTI* q)
	{
		std::wstring string_value;
		if (q == nullptr)
		{
			string_value =  L"nullptr";
		}
		else
		{
			string_value = std::to_wstring(q->TypeIdInstance()); //my first is a string figure out how to con
		}
		 
		RETURN_WIDE_STRING(string_value);
	}

	template<>
	inline std::wstring ToString<RTTI>(RTTI* q)
	{
		std::wstring string_value;
		if (q == nullptr)
		{
			string_value = L"nullptr";
		}
		else
		{
			string_value = std::to_wstring(q->TypeIdInstance()); //my first is a string figure out how to con
		}
		RETURN_WIDE_STRING(string_value);
	}
}

//FieaGameEngine::DatumTypes
namespace Microsoft::VisualStudio::CppUnitTestFramework
{
	template<>
	inline std::wstring ToString<FieaGameEngine::DatumTypes>(const FieaGameEngine::DatumTypes& q)
	{
		std::wstring string_value = std::to_wstring(static_cast<size_t>(q)); //my first is a string figure out how to con
		RETURN_WIDE_STRING(string_value);
	}

	template<>
	inline std::wstring ToString<FieaGameEngine::DatumTypes>(const FieaGameEngine::DatumTypes* q)
	{
		std::wstring string_value = std::to_wstring(static_cast<size_t>(*q)); //my first is a string figure out how to con
		RETURN_WIDE_STRING(string_value);
	}

	template<>
	inline std::wstring ToString<FieaGameEngine::DatumTypes>(FieaGameEngine::DatumTypes* q)
	{
		std::wstring string_value = std::to_wstring(static_cast<size_t>(*q)); //my first is a string figure out how to con
		RETURN_WIDE_STRING(string_value);
	}
}

//glm::vec4
namespace Microsoft::VisualStudio::CppUnitTestFramework
{
	template<>
	inline std::wstring ToString<glm::vec4>(const glm::vec4& q)
	{
		std::wstring string_value = std::to_wstring(q.length()); //my first is a string figure out how to con
		RETURN_WIDE_STRING(string_value);
	}

	template<>
	inline std::wstring ToString<glm::vec4>(const glm::vec4* q)
	{
		std::wstring string_value = std::to_wstring(q->length()); //my first is a string figure out how to con
		RETURN_WIDE_STRING(string_value);
	}

	template<>
	inline std::wstring ToString<glm::vec4>(glm::vec4* q)
	{
		std::wstring string_value = std::to_wstring(q->length()); //my first is a string figure out how to con
		RETURN_WIDE_STRING(string_value);
	}
}

//glm::mat4
namespace Microsoft::VisualStudio::CppUnitTestFramework
{
	template<>
	inline std::wstring ToString<glm::mat4>(const glm::mat4& q)
	{
		std::wstring string_value = std::to_wstring(q.length()); //my first is a string figure out how to con
		RETURN_WIDE_STRING(string_value);
	}

	template<>
	inline std::wstring ToString<glm::mat4>(const glm::mat4* q)
	{
		std::wstring string_value = std::to_wstring(q->length()); //my first is a string figure out how to con
		RETURN_WIDE_STRING(string_value);
	}

	template<>
	inline std::wstring ToString<glm::mat4>(glm::mat4* q)
	{
		std::wstring string_value = std::to_wstring(q->length()); //my first is a string figure out how to con
		RETURN_WIDE_STRING(string_value);
	}
}

//Scope
namespace Microsoft::VisualStudio::CppUnitTestFramework
{
	template<>
	inline std::wstring ToString<Scope>(const Scope& q)
	{
		std::wstring string_value = std::to_wstring(q.NumberOfItems()); //my first is a string figure out how to con
		RETURN_WIDE_STRING(string_value);
	}

	template<>
	inline std::wstring ToString<Scope>(const Scope* q)
	{
		std::wstring string_value = std::to_wstring(q->NumberOfItems()); //my first is a string figure out how to con
		RETURN_WIDE_STRING(string_value);
	}

	template<>
	inline std::wstring ToString<Scope>(Scope* q)
	{
		std::wstring string_value = std::to_wstring(q->NumberOfItems()); //my first is a string figure out how to con
		RETURN_WIDE_STRING(string_value);
	}

}

//AttributedFoo
namespace Microsoft::VisualStudio::CppUnitTestFramework
{
	template<>
	inline std::wstring ToString<AttributedFoo>(const AttributedFoo& q)
	{
		std::wstring string_value = std::to_wstring(q.NumberOfItems()); //my first is a string figure out how to con
		RETURN_WIDE_STRING(string_value);
	}

	template<>
	inline std::wstring ToString<AttributedFoo>(const AttributedFoo* q)
	{
		std::wstring string_value = std::to_wstring(q->NumberOfItems()); //my first is a string figure out how to con
		RETURN_WIDE_STRING(string_value);
	}

	template<>
	inline std::wstring ToString<AttributedFoo>(AttributedFoo* q)
	{
		std::wstring string_value = std::to_wstring(q->NumberOfItems()); //my first is a string figure out how to con
		RETURN_WIDE_STRING(string_value);
	}

}

//JsonScopeHelper
namespace Microsoft::VisualStudio::CppUnitTestFramework
{
	template<>
	inline std::wstring ToString<JsonScopeHelper>(const JsonScopeHelper& q)
	{
		std::wstring string_value = std::to_wstring(q.TypeIdInstance()); //my first is a string figure out how to con
		RETURN_WIDE_STRING(string_value);
	}

	template<>
	inline std::wstring ToString<JsonScopeHelper>(const JsonScopeHelper* q)
	{
		std::wstring string_value = std::to_wstring(q->TypeIdInstance()); //my first is a string figure out how to con
		RETURN_WIDE_STRING(string_value);
	}

	template<>
	inline std::wstring ToString<JsonScopeHelper>(JsonScopeHelper* q)
	{
		std::wstring string_value = std::to_wstring(q->TypeIdInstance()); //my first is a string figure out how to con
		RETURN_WIDE_STRING(string_value);
	}

}

//Light
namespace Microsoft::VisualStudio::CppUnitTestFramework
{
	template<>
	inline std::wstring ToString<Light>(const Light& q)
	{
		std::wstring x = std::to_wstring(q.Color().x); 
		std::wstring y = std::to_wstring(q.Color().y); 
		std::wstring z = std::to_wstring(q.Color().z); 
		std::wstring w = std::to_wstring(q.Color().w); 
		RETURN_WIDE_STRING(x + y + z + w);
	}

	template<>
	inline std::wstring ToString<Light>(const Light* q)
	{
		std::wstring x = std::to_wstring(q->Color().x); 
		std::wstring y = std::to_wstring(q->Color().y); 
		std::wstring z = std::to_wstring(q->Color().z); 
		std::wstring w = std::to_wstring(q->Color().w); 
		RETURN_WIDE_STRING(x + y + z + w);
	}

	template<>
	inline std::wstring ToString<Light>(Light* q)
	{
		std::wstring x = std::to_wstring(q->Color().x);
		std::wstring y = std::to_wstring(q->Color().y);
		std::wstring z = std::to_wstring(q->Color().z);
		std::wstring w = std::to_wstring(q->Color().w);
		RETURN_WIDE_STRING(x + y + z + w);
	}

}
