#include "pch.h"
#include "ToStringSpecializations.h"
#include "Factory.h"
#include "Foo.h"
#include "RTTI.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTestLibraryDesktop
{

	TEST_CLASS(FactoryTest)
	{
	public:

		TEST_METHOD_INITIALIZE(Initialize) //Before every test this is called.
		{
			


#if defined(_DEBUG) //Only runs in Debug mode.
			_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF);
			_CrtMemCheckpoint(&_startMemState); //Takes a snapshot of heap allocated data.
#endif

		}
		TEST_METHOD_CLEANUP(Cleanup) //After every test this is called.
		{
#if defined(_DEBUG)
			_CrtMemState endMemState, diffMemState;
			_CrtMemCheckpoint(&endMemState); //Takes a snapshot of heap allocated data.
			if (_CrtMemDifference(&diffMemState, &_startMemState, &endMemState)) //Compares heap allocated data before and after the test ran.
			{
				_CrtMemDumpStatistics(&diffMemState);
				Assert::Fail(L"Memory Leaks!");
			}
#endif
		}
		TEST_METHOD(NoRegisterdFactories)
		{
			
			RTTI* isFoo = Factory<RTTI>::Create("Foo"s);
			Assert::IsNull(isFoo);
			
		}
		TEST_METHOD(FactoryFind)
		{
			FooFactory myFooFactory;
			RTTI* isFoo = Factory<RTTI>::Create("Foo"s);
			Assert::IsNotNull(isFoo);
			Foo* myFoo = isFoo->As<Foo>();
			Assert::IsNotNull(myFoo);
			delete myFoo;
		}
	private:
		static _CrtMemState _startMemState; //static class members are shared in every instance of the class
	};

	_CrtMemState FactoryTest::_startMemState; //There is needed to separate declaration from definition. Changes in C++17
}