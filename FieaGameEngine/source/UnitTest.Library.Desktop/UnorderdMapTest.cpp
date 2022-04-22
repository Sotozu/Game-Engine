#include "pch.h"
#include "ToStringSpecializations.h"

#include "UnorderedMap.h"
#include "Foo.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace FieaGameEngine;


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
	TEST_CLASS(UnorderedMapTests)
	{

		using UMap = UnorderedMap<const Foo, std::string>;
		using it_bool_pair = std::pair<UMap::Iterator, bool>; //used to return  an iterator and bool in situations where the user wants to know if the value already exists in the map
		using const_it_bool_pair = std::pair<UMap::ConstIterator, bool>; //used to return  an iterator and bool in situations where the user wants to know if the value already exists in the map
	
		using size_type = std::size_t;
		using pair_type = std::pair<const Foo, std::string>;
		using chain_type = SList<pair_type>;
		using chain_iterator = typename chain_type::Iterator; //HOLY SHIT I HAD TO TRY SO MANY WAYS TO MAKE THIS SYNTAX WORK FUCK ME
		using const_chain_iterator = typename chain_type::ConstIterator; //HOLY SHIT I HAD TO TRY SO MANY WAYS TO MAKE THIS SYNTAX WORK FUCK ME
		using bucket_type = Vector<chain_type>;




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
#pragma region UNORDERED MAP FUNCTION TESTS
		TEST_METHOD(UMapConstructor)
		{
			//Defaulted Size, Hash Functor, and Equality Functor parameters
			{
				UMap myUMap;

				size_type size = myUMap.Size();
				size;

				size_type numBuckets = myUMap.NumBuckets();
				numBuckets;

				Assert::AreEqual(size, myUMap.Size());
				Assert::AreEqual(numBuckets, myUMap.NumBuckets());

			}

			//User specified Size, Hash Functor, and Equality Functor parameters
			{

				//FooEqualityFunctor FooEquFunc{};
				//DefaultHash<const Foo> FooHashFunc{};

				UMap myUMap;

				//UMap myUMap1(20, FooHashFunc, FooEquFunc); ??NOT WORKING??

				size_type size = myUMap.Size();
				size;

				size_type numBuckets = myUMap.NumBuckets();
				numBuckets;

				Assert::AreEqual(static_cast<size_type>(0), myUMap.Size());
				Assert::AreEqual(static_cast<size_type>(11), myUMap.NumBuckets());

			}
		}
		TEST_METHOD(UMapCopyConstructorDefaulted)
		{
			//Defaulted Size, Hash Functor, and Equality Functor parameters
			//Empty map
			{
				UMap myUMap;

				size_type size = myUMap.Size();
				size;


				UMap myUMapCopy(myUMap);

				Assert::AreEqual(myUMapCopy.Size(), myUMap.Size());
				Assert::AreEqual(myUMapCopy.NumBuckets(), myUMap.NumBuckets());

			}

			//map with items
			{
				const Foo fooKey1(1);
				const Foo fooKey2(2);
				const Foo fooKey3(3);

				std::string storedValue1 = "storedValue1";
				std::string storedValue2 = "storedValue2";
				std::string storedValue3 = "storedValue3";

				pair_type mypair1(fooKey1, storedValue1);
				pair_type mypair2(fooKey2, storedValue2);
				pair_type mypair3(fooKey3, storedValue3);

				UMap myUMap;

				myUMap.Insert(mypair1);
				myUMap.Insert(mypair2);
				myUMap.Insert(mypair3);

				UMap myUMapCopy(myUMap);

				Assert::AreEqual(myUMapCopy.Size(), myUMap.Size());
				Assert::AreEqual(myUMapCopy.NumBuckets(), myUMap.NumBuckets());

				UMap::Iterator It1 = myUMap.begin();
				UMap::Iterator It2 = myUMapCopy.begin();


				Assert::AreEqual(*It1, *It2);
				++It1;
				++It2;
				Assert::AreEqual(*It1, *It2);
				++It1;
				++It2;
				Assert::AreEqual(*It1, *It2);

			}
		}
		TEST_METHOD(UMapAssignmentDefaulted)
		{
			//Empty map
			{
				UMap myUMap;

				size_type size = myUMap.Size();
				size;

				UMap myUMapCopy;

				myUMapCopy = myUMap;

				Assert::AreEqual(myUMapCopy.Size(), myUMap.Size());
				Assert::AreEqual(myUMapCopy.NumBuckets(), myUMap.NumBuckets());

			}

			//map with items
			{
				const Foo fooKey1(1);
				const Foo fooKey2(2);
				const Foo fooKey3(3);

				std::string storedValue1 = "storedValue1";
				std::string storedValue2 = "storedValue2";
				std::string storedValue3 = "storedValue3";

				pair_type mypair1(fooKey1, storedValue1);
				pair_type mypair2(fooKey2, storedValue2);
				pair_type mypair3(fooKey3, storedValue3);

				UMap myUMap;

				myUMap.Insert(mypair1);
				myUMap.Insert(mypair2);
				myUMap.Insert(mypair3);

				UMap myUMapCopy;

				myUMapCopy = myUMap;


				Assert::AreEqual(myUMapCopy.Size(), myUMap.Size());
				Assert::AreEqual(myUMapCopy.NumBuckets(), myUMap.NumBuckets());

				UMap::Iterator It1 = myUMap.begin();
				UMap::Iterator It2 = myUMapCopy.begin();

				Assert::AreEqual(*It1, *It2);
				++It1;
				++It2;
				Assert::AreEqual(*It1, *It2);
				++It1;
				++It2;
				Assert::AreEqual(*It1, *It2);


			}

			//Self Assignment
			{
				const Foo fooKey1(1);
				const Foo fooKey2(2);
				const Foo fooKey3(3);

				std::string storedValue1 = "storedValue1";
				std::string storedValue2 = "storedValue2";
				std::string storedValue3 = "storedValue3";

				pair_type mypair1(fooKey1, storedValue1);
				pair_type mypair2(fooKey2, storedValue2);
				pair_type mypair3(fooKey3, storedValue3);

				UMap myUMap;

				myUMap.Insert(mypair1);
				myUMap.Insert(mypair2);
				myUMap.Insert(mypair3);

				UMap& myUMapReference = myUMap;
				

				myUMap = myUMapReference;


				Assert::AreEqual(myUMapReference.Size(), myUMap.Size());
				Assert::AreEqual(myUMapReference.NumBuckets(), myUMap.NumBuckets());

				UMap::Iterator It1 = myUMap.begin();

			}
		}
		TEST_METHOD(UMapMoveConstructorDefaulted)
		{
			const Foo fooKey1(1);
			const Foo fooKey2(2);
			const Foo fooKey3(3);

			std::string storedValue1 = "storedValue1";
			std::string storedValue2 = "storedValue2";
			std::string storedValue3 = "storedValue3";

			pair_type mypair1(fooKey1, storedValue1);
			pair_type mypair2(fooKey2, storedValue2);
			pair_type mypair3(fooKey3, storedValue3);

			UMap myUMap;

			myUMap.Insert(mypair1);
			myUMap.Insert(mypair2);
			myUMap.Insert(mypair3);

			UMap myUMapMoved(std::move(myUMap));

			it_bool_pair itBoolPair =  myUMapMoved.Find(fooKey1);
			Assert::AreEqual(true, itBoolPair.second);
			itBoolPair = myUMapMoved.Find(fooKey2);
			Assert::AreEqual(true, itBoolPair.second);
			itBoolPair = myUMapMoved.Find(fooKey3);
			Assert::AreEqual(true, itBoolPair.second);
		}
		TEST_METHOD(UMapMoveAssignmentDefaulted)
		{
			const Foo fooKey1(1);
			const Foo fooKey2(2);
			const Foo fooKey3(3);

			std::string storedValue1 = "storedValue1";
			std::string storedValue2 = "storedValue2";
			std::string storedValue3 = "storedValue3";

			pair_type mypair1(fooKey1, storedValue1);
			pair_type mypair2(fooKey2, storedValue2);
			pair_type mypair3(fooKey3, storedValue3);

			UMap myUMap;

			myUMap.Insert(mypair1);
			myUMap.Insert(mypair2);
			myUMap.Insert(mypair3);

			UMap myUMapMoved;
			myUMapMoved = (std::move(myUMap));

			it_bool_pair itBoolPair = myUMapMoved.Find(fooKey1);
			Assert::AreEqual(true, itBoolPair.second);
			itBoolPair = myUMapMoved.Find(fooKey2);
			Assert::AreEqual(true, itBoolPair.second);
			itBoolPair = myUMapMoved.Find(fooKey3);
			Assert::AreEqual(true, itBoolPair.second);

		}
		TEST_METHOD(UMapInsert)
		{
			{
				//Inserting when item is already in the map
				const Foo fooKey1(1);
				const Foo fooKey2(2);
				const Foo fooKey3(3);

				std::string storedValue1 = "storedValue1";
				std::string storedValue2 = "storedValue2";
				std::string storedValue3 = "storedValue3";

				pair_type mypair1(fooKey1, storedValue1);
				pair_type mypair2(fooKey2, storedValue2);
				pair_type mypair3(fooKey3, storedValue3);

				UMap myUMap;

				myUMap.Insert(mypair1);
				myUMap.Insert(mypair2);
				myUMap.Insert(mypair3);

				//inserting a pair that already exists in the map
				UMap::it_bool_pair itBoolPair = myUMap.Insert(mypair3);

				Assert::AreEqual(*(itBoolPair.first), mypair3);
				Assert::IsTrue(itBoolPair.second);

			}
			//Inserting when item is not in the map

			{
				const Foo fooKey1(1);
				const Foo fooKey2(2);
				const Foo fooKey3(3);

				const Foo fooKey4(4);

				std::string storedValue1 = "storedValue1";
				std::string storedValue2 = "storedValue2";
				std::string storedValue3 = "storedValue3";
				std::string storedValue4 = "storedValue4";

				pair_type mypair1(fooKey1, storedValue1);
				pair_type mypair2(fooKey2, storedValue2);
				pair_type mypair3(fooKey3, storedValue3);
				pair_type mypair4(fooKey4, storedValue4);

				UMap myUMap;

				myUMap.Insert(mypair1);
				myUMap.Insert(mypair2);
				myUMap.Insert(mypair3);

				//inserting a pair that does not exist in the map
				UMap::it_bool_pair itBoolPair = myUMap.Insert(mypair4);

				Assert::AreEqual(*(itBoolPair.first), mypair4);
				Assert::IsFalse(itBoolPair.second); //not found

			}
		}
		TEST_METHOD(UMapFind)
		{
			//Key exists in the map
			{
				const Foo fooKey1(1);
				const Foo fooKey2(2);
				const Foo fooKey3(3);

				const Foo fooKey4(4);

				std::string storedValue1 = "storedValue1";
				std::string storedValue2 = "storedValue2";
				std::string storedValue3 = "storedValue3";

				pair_type mypair1(fooKey1, storedValue1);
				pair_type mypair2(fooKey2, storedValue2);
				pair_type mypair3(fooKey3, storedValue3);

				FooEqualityFunctor func;
				UMap myUMap(func);

				myUMap.Insert(mypair1);
				myUMap.Insert(mypair2);
				myUMap.Insert(mypair3);

				UMap::it_bool_pair itBoolPair = myUMap.Find(fooKey1);

				Assert::AreEqual(mypair1, *(itBoolPair.first));
				Assert::IsTrue(itBoolPair.second);

				
			}

			//Key does NOT exists in the map
			{
				const Foo fooKey1(1);
				const Foo fooKey2(2);
				const Foo fooKey3(3);

				const Foo fooKey4(4);

				std::string storedValue1 = "storedValue1";
				std::string storedValue2 = "storedValue2";
				std::string storedValue3 = "storedValue3";

				pair_type mypair1(fooKey1, storedValue1);
				pair_type mypair2(fooKey2, storedValue2);
				pair_type mypair3(fooKey3, storedValue3);

				UMap myUMap;

				myUMap.Insert(mypair1);
				myUMap.Insert(mypair2);

				UMap::it_bool_pair itBoolPair = myUMap.Find(fooKey4);

				Assert::AreEqual(itBoolPair.first, myUMap.end());
				Assert::IsFalse(itBoolPair.second);
			}

		}
		TEST_METHOD(UMapOOBracketBracket)
		{
			//Key exists in the map
			{
				const Foo fooKey1(1);
				const Foo fooKey2(2);
				const Foo fooKey3(3);

				std::string storedValue1 = "storedValue1";
				std::string storedValue2 = "storedValue2";
				std::string storedValue3 = "storedValue3";

				pair_type mypair1(fooKey1, storedValue1);
				pair_type mypair2(fooKey2, storedValue2);
				pair_type mypair3(fooKey3, storedValue3);

				UMap myUMap;

				myUMap.Insert(mypair1);
				myUMap.Insert(mypair2);
				myUMap.Insert(mypair3);

				const std::string myString1 = myUMap[fooKey1];
				Assert::AreEqual(storedValue1, myString1);
			}

			//Key does NOT exist in the map
			{
				const Foo fooKey1(1);
				const Foo fooKey2(2);
				const Foo fooKey3(3);
				const Foo fooKey4(4);

				std::string storedValue1 = "storedValue1";
				std::string storedValue2 = "storedValue2";
				std::string storedValue3 = "storedValue3";
				std::string storedValue4 = "storedValue4";


				pair_type mypair1(fooKey1, storedValue1);
				pair_type mypair2(fooKey2, storedValue2);
				pair_type mypair3(fooKey3, storedValue3);

				UMap myUMap;

				myUMap.Insert(mypair1);
				myUMap.Insert(mypair2);
				myUMap.Insert(mypair3);

				const std::string myString1 = myUMap[fooKey1];
				Assert::AreEqual(storedValue1, myString1);

				const std::string myString2 = myUMap[fooKey2];
				Assert::AreEqual(storedValue2, myString2);

				const std::string myString3 = myUMap[fooKey3];
				Assert::AreEqual(storedValue3, myString3);

				//REVIEW

				//const std::string myString3 = myUMap[fooKey4];
				//Assert::AreEqual(storedValue3, myString3);

			}

		}
		TEST_METHOD(UMapOOBracketBracketConst)
		{
			//Key exists in the map
			{
				const Foo fooKey1(1);
				const Foo fooKey2(2);
				const Foo fooKey3(3);


				std::string storedValue1 = "storedValue1";
				std::string storedValue2 = "storedValue2";
				std::string storedValue3 = "storedValue3";


				pair_type mypair1(fooKey1, storedValue1);
				pair_type mypair2(fooKey2, storedValue2);
				pair_type mypair3(fooKey3, storedValue3);

				UMap myUMap;

				myUMap.Insert(mypair1);
				myUMap.Insert(mypair2);
				myUMap.Insert(mypair3);

				const UMap myUMapConst(myUMap);

				const std::string myString1 = myUMapConst[fooKey1];
				const std::string myString2 = myUMapConst[fooKey2];
				const std::string myString3 = myUMapConst[fooKey3];

				Assert::AreEqual(storedValue1, myString1);
				Assert::AreEqual(storedValue2, myString2);
				Assert::AreEqual(storedValue3, myString3);
			}

			//Key does NOT exist in the map
			{
				const Foo fooKey1(1);
				const Foo fooKey2(2);
				const Foo fooKey3(3);


				std::string storedValue1 = "storedValue1";
				std::string storedValue2 = "storedValue2";
				std::string storedValue3 = "storedValue3";


				pair_type mypair1(fooKey1, storedValue1);
				pair_type mypair2(fooKey2, storedValue2);
				pair_type mypair3(fooKey3, storedValue3);

				UMap myUMap;

				myUMap.Insert(mypair1);
				myUMap.Insert(mypair2);
				myUMap.Insert(mypair3);

				//REVIEW

				//auto Expected = [&myUMap, ]() {	const Foo fooKey4(4); myUMap.At(fooKey4); };
				//Assert::ExpectException<std::runtime_error>(Expected);

			}
		}
		TEST_METHOD(UMapRemove)
		{
			//Removing an item that exists in the map
			{
				const Foo fooKey1(1);
				const Foo fooKey2(2);
				const Foo fooKey3(3);

				std::string storedValue1 = "storedValue1";
				std::string storedValue2 = "storedValue2";
				std::string storedValue3 = "storedValue3";


				pair_type mypair1(fooKey1, storedValue1);
				pair_type mypair2(fooKey2, storedValue2);
				pair_type mypair3(fooKey3, storedValue3);

				UMap myUMap;

				myUMap.Insert(mypair1);
				myUMap.Insert(mypair2);
				myUMap.Insert(mypair3);

				UMap::it_bool_pair itBoolPair = myUMap.Find(fooKey1);
				Assert::IsTrue(itBoolPair.second);


				myUMap.Remove(fooKey1);

				itBoolPair = myUMap.Find(fooKey1);
				Assert::IsFalse(itBoolPair.second);

			}

			//Removing an item that does not exist in the map
			{
				const Foo fooKey1(1);
				const Foo fooKey2(2);
				const Foo fooKey3(3);

				const Foo fooKey4(4);

				std::string storedValue1 = "storedValue1";
				std::string storedValue2 = "storedValue2";
				std::string storedValue3 = "storedValue3";

				pair_type mypair1(fooKey1, storedValue1);
				pair_type mypair2(fooKey2, storedValue2);
				pair_type mypair3(fooKey3, storedValue3);

				UMap myUMap;

				myUMap.Insert(mypair1);
				myUMap.Insert(mypair2);
				myUMap.Insert(mypair3);

				Assert::AreEqual(static_cast<size_type>(3), myUMap.Size());
				myUMap.Remove(fooKey4);
				Assert::AreEqual(static_cast<size_type>(3), myUMap.Size());

			}
		}
		TEST_METHOD(UMapClear)
		{
			//Clearing a map with items
			{
				const Foo fooKey1(1);
				const Foo fooKey2(2);
				const Foo fooKey3(3);

				std::string storedValue1 = "storedValue1";
				std::string storedValue2 = "storedValue2";
				std::string storedValue3 = "storedValue3";

				pair_type mypair1(fooKey1, storedValue1);
				pair_type mypair2(fooKey2, storedValue2);
				pair_type mypair3(fooKey3, storedValue3);

				UMap myUMap;

				myUMap.Insert(mypair1);
				myUMap.Insert(mypair2);
				myUMap.Insert(mypair3);

				UMap::it_bool_pair itBoolPair = myUMap.Find(fooKey1);

				Assert::AreEqual(static_cast<size_type>(3), myUMap.Size());
				Assert::IsTrue(itBoolPair.second);

				myUMap.Clear();
				itBoolPair = myUMap.Find(fooKey1);

				Assert::AreEqual(static_cast<size_type>(0), myUMap.Size());
				Assert::IsFalse(itBoolPair.second);
			}
		}
		TEST_METHOD(UMapSize)
		{
			//Clearing a map with items
			{
				const Foo fooKey1(1);
				const Foo fooKey2(2);
				const Foo fooKey3(3);

				std::string storedValue1 = "storedValue1";
				std::string storedValue2 = "storedValue2";
				std::string storedValue3 = "storedValue3";

				pair_type mypair1(fooKey1, storedValue1);
				pair_type mypair2(fooKey2, storedValue2);
				pair_type mypair3(fooKey3, storedValue3);

				UMap myUMap;

				Assert::AreEqual(static_cast<size_type>(0), myUMap.Size());

				myUMap.Insert(mypair1);
				myUMap.Insert(mypair2);
				myUMap.Insert(mypair3);

				Assert::AreEqual(static_cast<size_type>(3), myUMap.Size());

				myUMap.Clear();

				Assert::AreEqual(static_cast<size_type>(0), myUMap.Size());

				myUMap.Insert(mypair1);
				myUMap.Insert(mypair2);
				myUMap.Insert(mypair3);

				Assert::AreEqual(static_cast<size_type>(3), myUMap.Size());

			}

			//Clearing an empty map
			{
				UMap myUMap;

				Assert::AreEqual(static_cast<size_type>(0), myUMap.Size());
			}

		}
		TEST_METHOD(UMapContainsKey)
		{
			//Testing when key exists in the map
			{
				const Foo fooKey1(1);
				const Foo fooKey2(2);
				const Foo fooKey3(3);

				std::string storedValue1 = "storedValue1";
				std::string storedValue2 = "storedValue2";
				std::string storedValue3 = "storedValue3";

				pair_type mypair1(fooKey1, storedValue1);
				pair_type mypair2(fooKey2, storedValue2);
				pair_type mypair3(fooKey3, storedValue3);

				UMap myUMap;

				myUMap.Insert(mypair1);
				myUMap.Insert(mypair2);
				myUMap.Insert(mypair3);

				Assert::AreEqual(true, myUMap.ContainsKey(fooKey1));
				Assert::AreEqual(true, myUMap.ContainsKey(fooKey2));
				Assert::AreEqual(true, myUMap.ContainsKey(fooKey3));
			}

			//Testing when key DOES NOT exist in the map
			{
				const Foo fooKey1(1);
				const Foo fooKey2(2);
				const Foo fooKey3(3);

				const Foo fooKey4(4);

				std::string storedValue1 = "storedValue1";
				std::string storedValue2 = "storedValue2";
				std::string storedValue3 = "storedValue3";


				pair_type mypair1(fooKey1, storedValue1);
				pair_type mypair2(fooKey2, storedValue2);
				pair_type mypair3(fooKey3, storedValue3);

				UMap myUMap;

				myUMap.Insert(mypair1);
				myUMap.Insert(mypair2);
				myUMap.Insert(mypair3);


				Assert::IsFalse(myUMap.ContainsKey(fooKey4));

			}

		}
		TEST_METHOD(UMapAt)
		{
			//At when the key exists in the map
			{
				const Foo fooKey1(1);
				const Foo fooKey2(2);
				const Foo fooKey3(3);

				std::string storedValue1 = "storedValue1";
				std::string storedValue2 = "storedValue2";
				std::string storedValue3 = "storedValue3";

				pair_type mypair1(fooKey1, storedValue1);
				pair_type mypair2(fooKey2, storedValue2);
				pair_type mypair3(fooKey3, storedValue3);

				UMap myUMap;

				myUMap.Insert(mypair1);
				myUMap.Insert(mypair2);
				myUMap.Insert(mypair3);
				
				Assert::AreEqual(storedValue1, myUMap.At(fooKey1));
			}

			//At when the key does not exist in the map
			{
				const Foo fooKey1(1);
				const Foo fooKey2(2);
				const Foo fooKey3(3);

				std::string storedValue1 = "storedValue1";
				std::string storedValue2 = "storedValue2";
				std::string storedValue3 = "storedValue3";

				pair_type mypair1(fooKey1, storedValue1);
				pair_type mypair2(fooKey2, storedValue2);
				pair_type mypair3(fooKey3, storedValue3);

				UMap myUMap;

				myUMap.Insert(mypair1);
				myUMap.Insert(mypair2);
				myUMap.Insert(mypair3);

				auto Expected = [&myUMap]() {const Foo fooKey4(4); myUMap.At(fooKey4);};
				Assert::ExpectException<std::runtime_error>(Expected);
			}


		}
		TEST_METHOD(UMapAtConst)
		{
			//At when the key exists in the map
			{
				const Foo fooKey1(1);
				const Foo fooKey2(2);
				const Foo fooKey3(3);

				std::string storedValue1 = "storedValue1";
				std::string storedValue2 = "storedValue2";
				std::string storedValue3 = "storedValue3";

				pair_type mypair1(fooKey1, storedValue1);
				pair_type mypair2(fooKey2, storedValue2);
				pair_type mypair3(fooKey3, storedValue3);

				UMap myUMap1;

				myUMap1.Insert(mypair1);
				myUMap1.Insert(mypair2);
				myUMap1.Insert(mypair3);

				const UMap myUMap2 = myUMap1;

				Assert::AreEqual(storedValue1, myUMap2.At(fooKey1));

			}

			//At when the key does not exist in the map
			{
				const Foo fooKey1(1);
				const Foo fooKey2(2);
				const Foo fooKey3(3);
				
				std::string storedValue1 = "storedValue1";
				std::string storedValue2 = "storedValue2";
				std::string storedValue3 = "storedValue3";

				pair_type mypair1(fooKey1, storedValue1);
				pair_type mypair2(fooKey2, storedValue2);
				pair_type mypair3(fooKey3, storedValue3);

				UMap myUMap1;

				myUMap1.Insert(mypair1);
				myUMap1.Insert(mypair2);
				myUMap1.Insert(mypair3);

				const UMap myUMap2 = myUMap1;

				auto Expected = [&myUMap2]() {const Foo fooKey4(4); myUMap2.At(fooKey4);};
				Assert::ExpectException<std::runtime_error>(Expected);
			}
		}
		TEST_METHOD(UMapLoadFactorPercentage)
		{

			//load factor of map with three items
			{
				const Foo fooKey1(1);
				const Foo fooKey2(2);
				const Foo fooKey3(3);

				std::string storedValue1 = "storedValue1";
				std::string storedValue2 = "storedValue2";
				std::string storedValue3 = "storedValue3";

				pair_type mypair1(fooKey1, storedValue1);
				pair_type mypair2(fooKey2, storedValue2);
				pair_type mypair3(fooKey3, storedValue3);

				UMap myUMap1;

				myUMap1.Insert(mypair1);
				myUMap1.Insert(mypair2);
				myUMap1.Insert(mypair3);

				float loadFactor = myUMap1.Size() / static_cast<float>(myUMap1.NumBuckets());

				Assert::AreEqual(loadFactor, myUMap1.LoadFactorPercentage());
			}


			//load factor of empty map
			{
				UMap myUMap1;
				Assert::AreEqual(0.f, myUMap1.LoadFactorPercentage());
			}
		}
		TEST_METHOD(UMapReHashMap)
		{
			const Foo fooKey1(1);
			const Foo fooKey2(2);
			const Foo fooKey3(3);

			std::string storedValue1 = "storedValue1";
			std::string storedValue2 = "storedValue2";
			std::string storedValue3 = "storedValue3";

			pair_type mypair1(fooKey1, storedValue1);
			pair_type mypair2(fooKey2, storedValue2);
			pair_type mypair3(fooKey3, storedValue3);

			UMap myUMap1;

			myUMap1.Insert(mypair1);
			myUMap1.Insert(mypair2);
			myUMap1.Insert(mypair3);

			myUMap1.ReHashMap(100);


			it_bool_pair itBoolPair = myUMap1.Find(fooKey1);
			Assert::AreEqual(mypair1, *(itBoolPair.first));

			itBoolPair = myUMap1.Find(fooKey2);
			Assert::AreEqual(mypair2, *(itBoolPair.first));

			itBoolPair = myUMap1.Find(fooKey3);
			Assert::AreEqual(mypair3, *(itBoolPair.first));

		}


#pragma endregion

#pragma region ITERATOR FUCTION TESTS
		TEST_METHOD(IteratorPrivateConstructor)
		{

			//unassigned iterator catching exceptions
			{
				const Foo fooKey1(1);
				const Foo fooKey2(2);
				const Foo fooKey3(3);

				std::string storedValue1 = "storedValue1";
				std::string storedValue2 = "storedValue2";
				std::string storedValue3 = "storedValue3";

				pair_type mypair1(fooKey1, storedValue1);
				pair_type mypair2(fooKey2, storedValue2);
				pair_type mypair3(fooKey3, storedValue3);

				UMap myUMap1;

				myUMap1.Insert(mypair1);
				myUMap1.Insert(mypair2);
				myUMap1.Insert(mypair3);


				UMap::Iterator It;

				auto Expected = [&It]() {++It; };
				Assert::ExpectException<std::runtime_error>(Expected);
			}

			//assigning iterator to begin
			{

				const Foo fooKey1(1);
				const Foo fooKey2(2);
				const Foo fooKey3(3);

				std::string storedValue1 = "storedValue1";
				std::string storedValue2 = "storedValue2";
				std::string storedValue3 = "storedValue3";

				pair_type mypair1(fooKey1, storedValue1);
				pair_type mypair2(fooKey2, storedValue2);
				pair_type mypair3(fooKey3, storedValue3);

				UMap myUMap1;

				myUMap1.Insert(mypair1);
				myUMap1.Insert(mypair2);
				myUMap1.Insert(mypair3);

				UMap::Iterator It = myUMap1.begin();

			}
			
			//assigning iterator to end
			{

				const Foo fooKey1(1);
				const Foo fooKey2(2);
				const Foo fooKey3(3);

				std::string storedValue1 = "storedValue1";
				std::string storedValue2 = "storedValue2";
				std::string storedValue3 = "storedValue3";

				pair_type mypair1(fooKey1, storedValue1);
				pair_type mypair2(fooKey2, storedValue2);
				pair_type mypair3(fooKey3, storedValue3);

				UMap myUMap1;

				myUMap1.Insert(mypair1);
				myUMap1.Insert(mypair2);
				myUMap1.Insert(mypair3);

				UMap::Iterator It = myUMap1.end();

			}

		}

#pragma endregion

#pragma region CONST-ITERATOR FUNCTION TESTS

#pragma endregion
		static _CrtMemState _startMemState; //static class members are shared in every instance of the class
	};
	_CrtMemState UnorderedMapTests::_startMemState; //There is needed to separate declaration from definition. Changes in C++17

}