#include "pch.h"
#include "FooSubscriber.h"
using namespace FieaGameEngine;

namespace UnitTestLibraryDesktop
{


	void FooSubscriber::Notify(const EventPublisher& ePublisher) //Paul refers to this in 4/18/2022 @10:42
	{
		//Need to check with RTTI if this should be a type of event

		const Event<Foo>* eFoo = ePublisher.As<Event<Foo>>();
		if (eFoo != nullptr) { Foo foo = eFoo->Message(); foo; }

		const Event<int>* eInt = ePublisher.As<Event<int>>();
		if (eInt != nullptr) { int i = eInt->Message(); i; }
		
		const Event<std::string>* eString = ePublisher.As<Event<std::string>>();
		if (eString != nullptr) { std::string s = eString->Message(); s; }
		
	}


	FooSubscriber::~FooSubscriber() {}


	bool FooSubscriber::operator==(EventPublisher& rhs)
	{
		rhs;
		return true;
	}

}