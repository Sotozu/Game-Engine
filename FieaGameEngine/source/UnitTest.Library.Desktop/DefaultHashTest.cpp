#include "pch.h"
#include "DefaultHash.h"
#include "Foo.h"
#include "Scope.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace FieaGameEngine;
using namespace UnitTestLibraryDesktop;
using namespace std;

namespace FieaGameEngine
{
	
	/// <summary>
	/// This is a template specialization that returns a unique hash value specific to the type.
	/// This implementation is only used for this module.  This is because Foo is only used for testing.
	/// </summary>
	template<>
	class DefaultHash<Foo>
	{
	public:
		std::size_t operator()(const Foo& key) const
		{
			return static_cast<size_t>(abs(key.Data()));
		}
	};

	template<>
	class DefaultHash<const Foo>
	{
	public:
		std::size_t operator()(const Foo& key) const
		{
			return static_cast<size_t>(abs(key.Data()));
		}
	};
	
}


namespace UnitTestLibraryDesktop
{

	TEST_CLASS(DefaultHashTests)
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

		TEST_METHOD(FooHash)
		{
			size_t size = AdditiveHash(reinterpret_cast <const uint8_t*>("Hello"), 6);
			size;
			//non const Foo
			{
				Foo foo1(1);
				Foo foo2(2);
				Foo foo1Equivalent(1);

				DefaultHash<Foo> hashFunc;

				Assert::AreEqual(hashFunc(foo1), hashFunc(foo1));
				Assert::AreNotEqual(hashFunc(foo1), hashFunc(foo2));
				Assert::AreNotEqual(hashFunc(foo2), hashFunc(foo1Equivalent));
				Assert::AreEqual(hashFunc(foo1), hashFunc(foo1Equivalent));
			}

			//const Foo
			{
				const Foo foo1(1);
				const Foo foo2(2);
				const Foo foo1Equivalent(1);

				DefaultHash<const Foo> hashFunc;

				Assert::AreEqual(hashFunc(foo1), hashFunc(foo1));
				Assert::AreNotEqual(hashFunc(foo1), hashFunc(foo2));
				Assert::AreNotEqual(hashFunc(foo2), hashFunc(foo1Equivalent));
				Assert::AreEqual(hashFunc(foo1), hashFunc(foo1Equivalent));

			}

		}
		TEST_METHOD(StringHash)
		{

			//no const string
			{
				std::string a = "I'm Variable A"s;
				std::string b = "I'm Variable B"s;
				std::string aEquivalent = "I'm Variable A"s;

				DefaultHash<std::string> hashFunc;

				Assert::AreEqual(hashFunc(a), hashFunc(a));
				Assert::AreNotEqual(hashFunc(a), hashFunc(b));
				Assert::AreNotEqual(hashFunc(b), hashFunc(aEquivalent));
				Assert::AreEqual(hashFunc(a), hashFunc(aEquivalent));
			}

			//const string
			{
				const std::string a = "I'm Variable A"s;
				const std::string b = "I'm Variable B"s;
				const std::string aEquivalent = "I'm Variable A"s;

				DefaultHash<const std::string> hashFunc;

				Assert::AreEqual(hashFunc(a), hashFunc(a));
				Assert::AreNotEqual(hashFunc(a), hashFunc(b));
				Assert::AreNotEqual(hashFunc(b), hashFunc(aEquivalent));
				Assert::AreEqual(hashFunc(a), hashFunc(aEquivalent));
			}

		}
		TEST_METHOD(CharHash)
		{
			//non const char
			{
				char a = 'a';
				char b = 'b';
				char aEquivalent = 'a';

				DefaultHash<char> hashFunc;

				Assert::AreEqual(hashFunc(a), hashFunc(a));
				Assert::AreNotEqual(hashFunc(a), hashFunc(b));
				Assert::AreNotEqual(hashFunc(b), hashFunc(aEquivalent));
				Assert::AreEqual(hashFunc(a), hashFunc(aEquivalent));
			}
			//const char
			{
				const char a = 'a';
				const char b = 'b';
				const char aEquivalent = 'a';

				DefaultHash<const char> hashFunc;

				Assert::AreEqual(hashFunc(a), hashFunc(a));
				Assert::AreNotEqual(hashFunc(a), hashFunc(b));
				Assert::AreNotEqual(hashFunc(b), hashFunc(aEquivalent));
				Assert::AreEqual(hashFunc(a), hashFunc(aEquivalent));
			}

			//6/202/2022 NOT VALID TEST
			//non const char*
			/*{
				char* myStringA = "Hello";
				char* myStringB = "World";
				char* myStringEquivalentA = "Hello";

				DefaultHash<char*> hashFunc;

				Assert::AreEqual(hashFunc(myStringA), hashFunc(myStringA));
				Assert::AreNotEqual(hashFunc(myStringA), hashFunc(myStringB));
				Assert::AreNotEqual(hashFunc(myStringB), hashFunc(myStringEquivalentA));
				Assert::AreEqual(hashFunc(myStringA), hashFunc(myStringEquivalentA));
			}*/
		
			//const char*
			{
				const char* myStringA = "Hello";
				const char* myStringB = "World";
				const char* myStringEquivalentA = "Hello";

				DefaultHash<const char*> hashFunc;

				Assert::AreEqual(hashFunc(myStringA), hashFunc(myStringA));
				Assert::AreNotEqual(hashFunc(myStringA), hashFunc(myStringB));
				Assert::AreNotEqual(hashFunc(myStringB), hashFunc(myStringEquivalentA));
				Assert::AreEqual(hashFunc(myStringA), hashFunc(myStringEquivalentA));
			}
		}
		TEST_METHOD(IntHash)
		{
			//non const int
			{
				int val1 = 1;
				int val2 = 2;
				int val1Equivalent = 1;

				DefaultHash<int> hashFunc;

				Assert::AreEqual(hashFunc(val1), hashFunc(val1));
				Assert::AreNotEqual(hashFunc(val1), hashFunc(val2));
				Assert::AreNotEqual(hashFunc(val2), hashFunc(val1Equivalent));
				Assert::AreEqual(hashFunc(val1), hashFunc(val1Equivalent));
			}

			//const int
			{
				const int val1 = 1;
				const int val2 = 2;
				const int val1Equivalent = 1;

				DefaultHash<const int> hashFunc;

				Assert::AreEqual(hashFunc(val1), hashFunc(val1));
				Assert::AreNotEqual(hashFunc(val1), hashFunc(val2));
				Assert::AreNotEqual(hashFunc(val2), hashFunc(val1Equivalent));
				Assert::AreEqual(hashFunc(val1), hashFunc(val1Equivalent));

			}

			//non const int*
			{
				int* val1 = new int(1);
				int* val2 = new int(2);
				int* val1Equivalent = new int(1);

				DefaultHash<int*> hashFunc;

				Assert::AreEqual(hashFunc(val1), hashFunc(val1));
				Assert::AreNotEqual(hashFunc(val1), hashFunc(val2));
				Assert::AreNotEqual(hashFunc(val2), hashFunc(val1Equivalent));
				Assert::AreEqual(hashFunc(val1), hashFunc(val1Equivalent));

				delete val1;
				delete val2;
				delete val1Equivalent;

			}

			//const int*
			{
				const int* val1 = new int(1);
				const int* val2 = new int(2);
				const int* val1Equivalent = new int(1);

				DefaultHash<const int*> hashFunc;

				Assert::AreEqual(hashFunc(val1), hashFunc(val1));
				Assert::AreNotEqual(hashFunc(val1), hashFunc(val2));
				Assert::AreNotEqual(hashFunc(val2), hashFunc(val1Equivalent));
				Assert::AreEqual(hashFunc(val1), hashFunc(val1Equivalent));

				delete val1;
				delete val2;
				delete val1Equivalent;
			}
		}
		TEST_METHOD(ShortHash)
		{
			//non const short
			{
				short val1 = 1;
				short val2 = 2;
				short val1Equivalent = 1;

				DefaultHash<short> hashFunc;

				Assert::AreEqual(hashFunc(val1), hashFunc(val1));
				Assert::AreNotEqual(hashFunc(val1), hashFunc(val2));
				Assert::AreNotEqual(hashFunc(val2), hashFunc(val1Equivalent));
				Assert::AreEqual(hashFunc(val1), hashFunc(val1Equivalent));
			}

			//const short
			{
				const short val1 = 1;
				const short val2 = 2;
				const short val1Equivalent = 1;

				DefaultHash<const short> hashFunc;

				Assert::AreEqual(hashFunc(val1), hashFunc(val1));
				Assert::AreNotEqual(hashFunc(val1), hashFunc(val2));
				Assert::AreNotEqual(hashFunc(val2), hashFunc(val1Equivalent));
				Assert::AreEqual(hashFunc(val1), hashFunc(val1Equivalent));

			}

			//non const short*
			{
				short* val1 = new short(1);
				short* val2 = new short(2);
				short* val1Equivalent = new short(1);

				DefaultHash<short*> hashFunc;

				Assert::AreEqual(hashFunc(val1), hashFunc(val1));
				Assert::AreNotEqual(hashFunc(val1), hashFunc(val2));
				Assert::AreNotEqual(hashFunc(val2), hashFunc(val1Equivalent));
				Assert::AreEqual(hashFunc(val1), hashFunc(val1Equivalent));

				delete val1;
				delete val2;
				delete val1Equivalent;

			}

			//const short*
			{
				const short* val1 = new short(1);
				const short* val2 = new short(2);
				const short* val1Equivalent = new short(1);

				DefaultHash<const short*> hashFunc;

				Assert::AreEqual(hashFunc(val1), hashFunc(val1));
				Assert::AreNotEqual(hashFunc(val1), hashFunc(val2));
				Assert::AreNotEqual(hashFunc(val2), hashFunc(val1Equivalent));
				Assert::AreEqual(hashFunc(val1), hashFunc(val1Equivalent));

				delete val1;
				delete val2;
				delete val1Equivalent;
			}
		}
		TEST_METHOD(LongHash)
		{
			//non const long
			{
				long val1 = 1;
				long val2 = 2;
				long val1Equivalent = 1;

				DefaultHash<long> hashFunc;

				Assert::AreEqual(hashFunc(val1), hashFunc(val1));
				Assert::AreNotEqual(hashFunc(val1), hashFunc(val2));
				Assert::AreNotEqual(hashFunc(val2), hashFunc(val1Equivalent));
				Assert::AreEqual(hashFunc(val1), hashFunc(val1Equivalent));
			}

			//const long
			{
				const long val1 = 1;
				const long val2 = 2;
				const long val1Equivalent = 1;

				DefaultHash<const long> hashFunc;

				Assert::AreEqual(hashFunc(val1), hashFunc(val1));
				Assert::AreNotEqual(hashFunc(val1), hashFunc(val2));
				Assert::AreNotEqual(hashFunc(val2), hashFunc(val1Equivalent));
				Assert::AreEqual(hashFunc(val1), hashFunc(val1Equivalent));

			}

			//non const long*
			{
				long* val1 = new long(1);
				long* val2 = new long(2);
				long* val1Equivalent = new long(1);

				DefaultHash<long*> hashFunc;

				Assert::AreEqual(hashFunc(val1), hashFunc(val1));
				Assert::AreNotEqual(hashFunc(val1), hashFunc(val2));
				Assert::AreNotEqual(hashFunc(val2), hashFunc(val1Equivalent));
				Assert::AreEqual(hashFunc(val1), hashFunc(val1Equivalent));

				delete val1;
				delete val2;
				delete val1Equivalent;

			}

			//const long*
			{
				const long* val1 = new long(1);
				const long* val2 = new long(2);
				const long* val1Equivalent = new long(1);

				DefaultHash<const long*> hashFunc;

				Assert::AreEqual(hashFunc(val1), hashFunc(val1));
				Assert::AreNotEqual(hashFunc(val1), hashFunc(val2));
				Assert::AreNotEqual(hashFunc(val2), hashFunc(val1Equivalent));
				Assert::AreEqual(hashFunc(val1), hashFunc(val1Equivalent));

				delete val1;
				delete val2;
				delete val1Equivalent;
			}
		}
		TEST_METHOD(LongLongHash)
		{
			{
				//non const long
				{
					long long val1 = 1;
					long long val2 = 2;
					long long val1Equivalent = 1;

					DefaultHash<long long> hashFunc;

					Assert::AreEqual(hashFunc(val1), hashFunc(val1));
					Assert::AreNotEqual(hashFunc(val1), hashFunc(val2));
					Assert::AreNotEqual(hashFunc(val2), hashFunc(val1Equivalent));
					Assert::AreEqual(hashFunc(val1), hashFunc(val1Equivalent));
				}

				//const long long
				{
					const long long val1 = 1;
					const long long val2 = 2;
					const long long val1Equivalent = 1;

					DefaultHash<const long long> hashFunc;

					Assert::AreEqual(hashFunc(val1), hashFunc(val1));
					Assert::AreNotEqual(hashFunc(val1), hashFunc(val2));
					Assert::AreNotEqual(hashFunc(val2), hashFunc(val1Equivalent));
					Assert::AreEqual(hashFunc(val1), hashFunc(val1Equivalent));

				}

				//non const long long*
				{
					long long* val1 = new long long(1);
					long long* val2 = new long long(2);
					long long* val1Equivalent = new long long(1);

					DefaultHash<long long*> hashFunc;

					Assert::AreEqual(hashFunc(val1), hashFunc(val1));
					Assert::AreNotEqual(hashFunc(val1), hashFunc(val2));
					Assert::AreNotEqual(hashFunc(val2), hashFunc(val1Equivalent));
					Assert::AreEqual(hashFunc(val1), hashFunc(val1Equivalent));

					delete val1;
					delete val2;
					delete val1Equivalent;

				}

				//const long long*
				{
					const long long* val1 = new long long(1);
					const long long* val2 = new long long(2);
					const long long* val1Equivalent = new long long(1);

					DefaultHash<const long long*> hashFunc;

					Assert::AreEqual(hashFunc(val1), hashFunc(val1));
					Assert::AreNotEqual(hashFunc(val1), hashFunc(val2));
					Assert::AreNotEqual(hashFunc(val2), hashFunc(val1Equivalent));
					Assert::AreEqual(hashFunc(val1), hashFunc(val1Equivalent));

					delete val1;
					delete val2;
					delete val1Equivalent;
				}
			}

		}
		TEST_METHOD(FloatHash)
		{
			
			//non const float
			{
				float val1 = 1.0;
				float val2 = 2.0;
				float val1Equivalent = 1.0;

				DefaultHash<float> hashFunc;

				Assert::AreEqual(hashFunc(val1), hashFunc(val1));
				Assert::AreNotEqual(hashFunc(val1), hashFunc(val2));
				Assert::AreNotEqual(hashFunc(val2), hashFunc(val1Equivalent));
				Assert::AreEqual(hashFunc(val1), hashFunc(val1Equivalent));
			}

			//const float
			{
				const float val1 = 1.0;
				const float val2 = 2.0;
				const float val1Equivalent = 1.0;

				DefaultHash<const float> hashFunc;

				Assert::AreEqual(hashFunc(val1), hashFunc(val1));
				Assert::AreNotEqual(hashFunc(val1), hashFunc(val2));
				Assert::AreNotEqual(hashFunc(val2), hashFunc(val1Equivalent));
				Assert::AreEqual(hashFunc(val1), hashFunc(val1Equivalent));

			}

			//non const float*
			{
				float* val1 = new float(1.0);
				float* val2 = new float(2.0);
				float* val1Equivalent = new float(1.0);

				DefaultHash<float*> hashFunc;

				Assert::AreEqual(hashFunc(val1), hashFunc(val1));
				Assert::AreNotEqual(hashFunc(val1), hashFunc(val2));
				Assert::AreNotEqual(hashFunc(val2), hashFunc(val1Equivalent));
				Assert::AreEqual(hashFunc(val1), hashFunc(val1Equivalent));

				delete val1;
				delete val2;
				delete val1Equivalent;

			}

			//const float*
			{
				const float* val1 = new float(1.0);
				const float* val2 = new float(2.0);
				const float* val1Equivalent = new float(1.0);

				DefaultHash<const float*> hashFunc;

				Assert::AreEqual(hashFunc(val1), hashFunc(val1));
				Assert::AreNotEqual(hashFunc(val1), hashFunc(val2));
				Assert::AreNotEqual(hashFunc(val2), hashFunc(val1Equivalent));
				Assert::AreEqual(hashFunc(val1), hashFunc(val1Equivalent));

				delete val1;
				delete val2;
				delete val1Equivalent;
			}
			
		}
		TEST_METHOD(DoubleHash)
		{
			//non const double
			{
				double val1 = 1.0;
				double val2 = 2.0;
				double val1Equivalent = 1.0;

				DefaultHash<double> hashFunc;

				Assert::AreEqual(hashFunc(val1), hashFunc(val1));
				Assert::AreNotEqual(hashFunc(val1), hashFunc(val2));
				Assert::AreNotEqual(hashFunc(val2), hashFunc(val1Equivalent));
				Assert::AreEqual(hashFunc(val1), hashFunc(val1Equivalent));
			}

			//const double
			{
				const double val1 = 1.0;
				const double val2 = 2.0;
				const double val1Equivalent = 1.0;

				DefaultHash<const double> hashFunc;

				Assert::AreEqual(hashFunc(val1), hashFunc(val1));
				Assert::AreNotEqual(hashFunc(val1), hashFunc(val2));
				Assert::AreNotEqual(hashFunc(val2), hashFunc(val1Equivalent));
				Assert::AreEqual(hashFunc(val1), hashFunc(val1Equivalent));

			}

			//non const double*
			{
				double* val1 = new double(1.0);
				double* val2 = new double(2.0);
				double* val1Equivalent = new double(1.0);

				DefaultHash<double*> hashFunc;

				Assert::AreEqual(hashFunc(val1), hashFunc(val1));
				Assert::AreNotEqual(hashFunc(val1), hashFunc(val2));
				Assert::AreNotEqual(hashFunc(val2), hashFunc(val1Equivalent));
				Assert::AreEqual(hashFunc(val1), hashFunc(val1Equivalent));

				delete val1;
				delete val2;
				delete val1Equivalent;

			}

			//const double*
			{
				const double* val1 = new double(1.0);
				const double* val2 = new double(2.0);
				const double* val1Equivalent = new double(1.0);

				DefaultHash<const double*> hashFunc;

				Assert::AreEqual(hashFunc(val1), hashFunc(val1));
				Assert::AreNotEqual(hashFunc(val1), hashFunc(val2));
				Assert::AreNotEqual(hashFunc(val2), hashFunc(val1Equivalent));
				Assert::AreEqual(hashFunc(val1), hashFunc(val1Equivalent));

				delete val1;
				delete val2;
				delete val1Equivalent;
			}
			
		}
		TEST_METHOD(LongDoubleHash)
		{
			//non const long double
			{
				long double val1 = 1.0;
				long double val2 = 2.0;
				long double val1Equivalent = 1.0;

				DefaultHash<long double> hashFunc;

				Assert::AreEqual(hashFunc(val1), hashFunc(val1));
				Assert::AreNotEqual(hashFunc(val1), hashFunc(val2));
				Assert::AreNotEqual(hashFunc(val2), hashFunc(val1Equivalent));
				Assert::AreEqual(hashFunc(val1), hashFunc(val1Equivalent));
			}

			//const long double
			{
				const long double val1 = 1.0;
				const long double val2 = 2.0;
				const long double val1Equivalent = 1.0;

				DefaultHash<const long double> hashFunc;

				Assert::AreEqual(hashFunc(val1), hashFunc(val1));
				Assert::AreNotEqual(hashFunc(val1), hashFunc(val2));
				Assert::AreNotEqual(hashFunc(val2), hashFunc(val1Equivalent));
				Assert::AreEqual(hashFunc(val1), hashFunc(val1Equivalent));

			}

			//non const long double*
			{
				long double* val1 = new long double(1.0);
				long double* val2 = new long double(2.0);
				long double* val1Equivalent = new long double(1.0);

				DefaultHash<long double*> hashFunc;

				Assert::AreEqual(hashFunc(val1), hashFunc(val1));
				Assert::AreNotEqual(hashFunc(val1), hashFunc(val2));
				Assert::AreNotEqual(hashFunc(val2), hashFunc(val1Equivalent));
				Assert::AreEqual(hashFunc(val1), hashFunc(val1Equivalent));

				delete val1;
				delete val2;
				delete val1Equivalent;

			}

			//const long double*
			{
				const long double* val1 = new long double(1.0);
				const long double* val2 = new long double(2.0);
				const long double* val1Equivalent = new long double(1.0);

				DefaultHash<const long double*> hashFunc;

				Assert::AreEqual(hashFunc(val1), hashFunc(val1));
				Assert::AreNotEqual(hashFunc(val1), hashFunc(val2));
				Assert::AreNotEqual(hashFunc(val2), hashFunc(val1Equivalent));
				Assert::AreEqual(hashFunc(val1), hashFunc(val1Equivalent));

				delete val1;
				delete val2;
				delete val1Equivalent;
			}

		}
	private:
		static _CrtMemState _startMemState; //static class members are shared in every instance of the class
	};

	_CrtMemState DefaultHashTests::_startMemState; //There is needed to separate declaration from definition. Changes in C++17
}