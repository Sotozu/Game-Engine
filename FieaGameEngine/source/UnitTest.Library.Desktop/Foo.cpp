#include "pch.h"
#include "Foo.h"

namespace UnitTestLibraryDesktop
{

	RTTI_DEFINITIONS(Foo)

#pragma region FOO FUNCTIONS
	//Default Constructor
	Foo::Foo() : _data(new std::int32_t(0)) {}

	//Copy Constructor
	Foo::Foo(const Foo& right) : _data(new std::int32_t(*(right._data))) {}

	Foo::Foo(std::int32_t data) : _data(new std::int32_t(data)) {}

	Foo::~Foo() { 
		int32_t data = *_data;
		data;
		delete _data; 
	}

	std::int32_t& Foo::Data() const { return *_data; }

	Foo& Foo::operator=(const Foo& right)
	{
		if (this != &right) { *_data = *right._data; }
		return *this;
	}

	bool Foo::operator==(const Foo& right) const
	{

		return (*_data == *(right._data));
	}

	bool Foo::operator!=(const Foo& right) const { return !operator==(right); }

	std::string Foo::ToString() const 
	{
		return "Foo";
	}



#pragma endregion
	bool FooEqualityFunctor::operator()(const Foo& left, const Foo& right) const 
	{
		return left.Data() == right.Data();
	}

}
