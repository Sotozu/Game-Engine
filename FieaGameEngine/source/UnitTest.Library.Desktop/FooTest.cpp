#include "pch.h"
#include "ToStringSpecializations.h"
#include "Foo.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTestLibraryDesktop
{

	TEST_CLASS(FooTests)
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
#pragma region FOO FUNCTION TESTS
		TEST_METHOD(DefaultConstructor)
		{
			Foo foo1;
			Assert::AreEqual(std::int32_t(0), foo1.Data());
		}
		TEST_METHOD(ConstructorParameter_int32_t)
		{
			Foo foo1(std::int32_t(1));
			Assert::AreEqual(std::int32_t(1), foo1.Data());
		}
		TEST_METHOD(CopyConstructor)
		{
			Foo foo1(std::int32_t(1));
			Foo foo2(foo1);
			Assert::AreEqual(foo1, foo2);
		}
		TEST_METHOD(Data)
		{
			std::int32_t var = 1;
			Foo foo1(var);
			Assert::AreEqual(var, foo1.Data());
		}
		TEST_METHOD(OOAssignment)
		{
			Foo foo1(std::int32_t(1));
			Foo foo2(std::int32_t(2));
			foo2 = foo1;
			Assert::AreEqual(foo1, foo2);
		}
		TEST_METHOD(OOIsEqual)
		{
			Foo foo1(std::int32_t(1));
			Foo foo2 = foo1;
			bool result = (foo2 == foo1);
			Assert::AreEqual(true, result);

			Foo& reffoo = foo1;
			foo1 = reffoo;
			bool result1 = (foo1 == foo1);
			Assert::AreEqual(true, result1);
			Assert::AreSame(foo1, foo1);
		}
		TEST_METHOD(OOIsNotEqual)
		{
			Foo foo1(std::int32_t(1));
			Foo foo2(std::int32_t(2));

			bool result = (foo2 != foo1);
			Assert::AreEqual(true, result);
		}
#pragma endregion
	private:
		static _CrtMemState _startMemState; //static class members are shared in every instance of the class
	};

	_CrtMemState FooTests::_startMemState; //There is needed to separate declaration from definition. Changes in C++17
}