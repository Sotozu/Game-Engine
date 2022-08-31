#include "pch.h"
#include "ToStringSpecializations.h"
#include "Vector.h"
#include "Foo.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace FieaGameEngine;
using namespace std;

namespace UnitTestLibraryDesktop
{
	TEST_CLASS(VectorTests)
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
#pragma region VECTOR FUNCTION TESTS
		TEST_METHOD(Partition) //TO DO: Added 04/20/2022
		{
			Vector<int32_t> vec;
			for (int32_t index = 0; index < 10; ++index)
			{
				vec.PushBack(index);
			}
			auto it = std::partition(vec.begin(), vec.end(), [](int32_t isEven) {return isEven % 2 == 0; });


		}
		TEST_METHOD(VectorBiderectionalIterator) //TO DO: Added 04/20/2022
		{

			//Testing a unassigned iterator
			{
				Vector<Foo>::Iterator It;
				auto Expected = [&It]() {It++; };
				Assert::ExpectException<std::runtime_error>(Expected);
			}

			//Testing an assigned iterator
			{
				Foo foo1(1);
				Foo foo2(2);
				Foo foo3(3);

				Vector<Foo> myVec{ foo1, foo2, foo3 };

				Vector<Foo>::Iterator It = myVec.begin();

				Assert::AreEqual(foo1, *It);
				It++;
				Assert::AreEqual(foo2, *It);
				It++;
				Assert::AreEqual(foo3, *It);

				It--;
				Assert::AreEqual(foo2, *It);

				It--;
				Assert::AreEqual(foo1, *It);
			}

		}
		TEST_METHOD(EmplaceBack) //TO DO: Added 04/19/2022
		{
			Vector<Foo> myVec;
			myVec.EmplaceBack(1);
		}
		TEST_METHOD(VectorConstructorCapacity)
		{

			//Testing construction with no parameters. This should initialize the member variables to what is declared in the header file.
			{
				Vector<Foo> myVec;
				Assert::AreEqual(static_cast<size_t>(0), myVec.Size());
				Assert::AreEqual(static_cast<size_t>(0), myVec.Capacity());
			}

			//Testing construction and passing a size for capacity
			{
				Vector<Foo> myVec(20);
				Assert::AreEqual(static_cast<size_t>(0), myVec.Size());
				Assert::AreEqual(static_cast<size_t>(20), myVec.Capacity());
			}

		}
		
		TEST_METHOD(VectorInitializerConstructor)
		{
			Foo foo1(1);
			Foo foo2(2);
			Foo foo3(3);

			Vector<Foo> myVec{ foo1,foo2,foo3 };

			Assert::AreEqual(static_cast<size_t>(3), myVec.Size());
			Assert::AreEqual(static_cast<size_t>(3), myVec.Capacity());

		}
		TEST_METHOD(VectorCopyConstructor)
		{
			const Foo foo1(1);
			const Foo foo2(2);
			const Foo foo3(3);

			Vector<Foo> myVec{ foo1,foo2,foo3 };

			Vector<Foo> copyVec(myVec);

			Assert::AreEqual(myVec.Front(), copyVec.Front());
			Assert::AreEqual(myVec.Back(), copyVec.Back());
			Assert::AreEqual(myVec[1], copyVec[1]);
			Assert::AreEqual(myVec.Size(), copyVec.Size());
			Assert::AreEqual(myVec.Capacity(), copyVec.Capacity());
		}
		TEST_METHOD(VectorMoveConstructor)
		{
			const Foo foo1(1);
			const Foo foo2(2);
			const Foo foo3(3);

			Vector<Foo> myVec{ foo1,foo2,foo3 };

			Vector<Foo> moveVec(static_cast<Vector<Foo>&&>(myVec));

			Assert::AreEqual(foo1, moveVec[0]);
			Assert::AreEqual(foo2, moveVec[1]);
			Assert::AreEqual(foo3, moveVec[2]);


		}
		TEST_METHOD(VectroPopBack)
		{
			Foo foo1(1);
			Foo foo2(2);
			Foo foo3(3);

			Vector<Foo> myVec{ foo1,foo2,foo3 };

			myVec.PopBack();
			Assert::AreEqual(foo2, myVec.Back());

			myVec.PopBack();
			Assert::AreEqual(foo1, myVec.Back());

		}
		TEST_METHOD(VectorAt)
		{
			const Foo foo1(1);
			const Foo foo2(2);
			const Foo foo3(3);

			Vector<Foo> myVec{ foo1,foo2,foo3 };

			Foo fooValue = myVec.At(1);
			Assert::AreEqual(foo2, fooValue);
		}
		TEST_METHOD(VectorAtConst)
		{
			const Foo foo1(1);
			const Foo foo2(2);
			const Foo foo3(3);

			const Vector<Foo> myVec{ foo1,foo2,foo3 };

			const Foo fooValue = myVec.At(1);
			Assert::AreEqual(foo2, fooValue);
		}
		TEST_METHOD(VectorOOBracketBracket)
		{
			const Foo foo1(1);
			const Foo foo2(2);
			const Foo foo3(3);

			Vector<Foo> myVec{ foo1,foo2,foo3 };
			Foo value = myVec[1];
			Assert::AreEqual(foo2, value);

			//Accessing beyond size
			auto Expected = [&myVec]() {myVec[10]; };
			Assert::ExpectException<std::runtime_error>(Expected);
		}
		TEST_METHOD(VectorOOBracketBracketConst)
		{
			//Testing access to data
			{
				const Foo foo1(1);
				const Foo foo2(2);
				const Foo foo3(3);

				const Vector<Foo> myVec{ foo1,foo2,foo3 };

				const Foo fooValue = myVec[1];
				Assert::AreEqual(foo2, fooValue);
			}
			
			//Accessing beyond size
			{
				const Foo foo1(1);
				const Foo foo2(2);
				const Foo foo3(3);

				Vector<Foo> myVec{ foo1,foo2,foo3 };

				auto Expected = [&myVec]() {myVec[10]; };
				Assert::ExpectException<std::runtime_error>(Expected);
			}
		}
		TEST_METHOD(VectorIsEmpty)
		{
			//Testing vector without items
			{
				Vector<Foo> myVec;
				Assert::IsTrue(myVec.IsEmpty());
			}

			//Testing vector with an item
			{
				const Foo foo1(1);
				Vector<Foo> myVec{ foo1 };
				Assert::IsFalse(myVec.IsEmpty());
			}

			//Testing vector with an item and then cleared
			{
				const Foo foo1(1);
				Vector<Foo> myVec{ foo1 };
				myVec.Clear();
				Assert::IsTrue(myVec.IsEmpty());
			}
		}
		TEST_METHOD(VectorFront)
		{
			//Testing return value
			{
				const Foo foo1(1);
				const Foo foo2(2);
				const Foo foo3(3);

				Vector<Foo> myVec{ foo1,foo2,foo3 };

				Assert::AreEqual(foo1, myVec.Front());
			}
			
			//Testing exception when accessing empty vector
			{
				Vector<Foo> myVec;

				auto Expected = [&myVec]() {myVec.Front(); };
				Assert::ExpectException<std::runtime_error>(Expected);
			}
		}
		TEST_METHOD(VectorBack)
		{
			//Testing return value
			{
				const Foo foo1(1);
				const Foo foo2(2);
				const Foo foo3(3);

				Vector<Foo> myVec{ foo1,foo2,foo3 };

				Assert::AreEqual(foo3, myVec.Back());
			}

			//Testing exception when accessing empty vector
			{
				Vector<Foo> myVec;

				auto Expected = [&myVec]() {myVec.Back(); };
				Assert::ExpectException<std::runtime_error>(Expected);
			}
		}
		TEST_METHOD(VectorFrontConst)
		{
			//Testing return value
			{
				const Foo foo1(1);
				const Foo foo2(2);
				const Foo foo3(3);

				const Vector<Foo> myVec{ foo1,foo2,foo3 };

				Assert::AreEqual(foo1, myVec.Front());
			}

			//Testing exception when accessing empty vector
			{
				const Vector<Foo> myVec;

				auto Expected = [&myVec]() {myVec.Front(); };
				Assert::ExpectException<std::runtime_error>(Expected);
			}
		}
		TEST_METHOD(VectorBackConst)
		{
			//Testing return value
			{
				const Foo foo1(1);
				const Foo foo2(2);
				const Foo foo3(3);

				const Vector<Foo> myVec{ foo1,foo2,foo3 };

				Assert::AreEqual(foo3, myVec.Back());
			}

			//Testing exception when accessing empty vector
			{
				const Vector<Foo> myVec;

				auto Expected = [&myVec]() {myVec.Back(); };
				Assert::ExpectException<std::runtime_error>(Expected);
			}
		}
		TEST_METHOD(VectorSize)
		{
			const Foo foo1(1);
			const Foo foo2(2);
			const Foo foo3(3);

			Vector<Foo> myVec{ foo1,foo2,foo3 };

			Assert::AreEqual(static_cast<size_t>(3), myVec.Size());
			
			myVec.PushBack(4);
			myVec.PushBack(5);
			myVec.PushBack(6);

			Assert::AreEqual(static_cast<size_t>(6), myVec.Size());
		}
		TEST_METHOD(VectorCapacity)
		{
			//Testing default capacity
			{
				Vector<Foo> myVec;
				Assert::AreEqual(static_cast<size_t>(0), myVec.Capacity());
			}

			//Testing capacity when constructor has values that initialize the vector
			{
				const Foo foo1(1);
				const Foo foo2(2);
				const Foo foo3(3);

				Vector<Foo> myVec{ foo1,foo2,foo3 };

				Assert::AreEqual(static_cast<size_t>(3), myVec.Capacity()); //capacity is double the size;
			}
		}
		TEST_METHOD(VectorPushBack)
		{
			const Foo foo1(1);
			const Foo foo2(2);
			const Foo foo3(3);
			const Foo foo4(4);
			const Foo foo5(5);
			const Foo foo6(6);

			const Foo foo7(7); // will cause capacity to double

			Vector<Foo> myVec{ foo1,foo2,foo3 }; // capacity = size*2 = 6

			Assert::AreEqual(foo3, myVec.Back());


			myVec.PushBack(foo4);
			Assert::AreEqual(foo4, myVec.Back());


			myVec.PushBack(foo5);
			Assert::AreEqual(foo5, myVec.Back());


			myVec.PushBack(foo6);
			Assert::AreEqual(foo6, myVec.Back());

			myVec.PushBack(foo7);
			Assert::AreEqual(foo7, myVec.Back());

			Assert::AreEqual(size_t(12), myVec.Capacity());
		}
		TEST_METHOD(VectorOOAssignment)
		{
			{
				const Foo foo1(1);
				const Foo foo2(2);
				const Foo foo3(3);

				Vector<Foo> myVec{ foo1,foo2,foo3 };

				const Foo foo11(11);
				const Foo foo12(12);
				const Foo foo13(13);

				Vector<Foo> copyVec{ foo11,foo12,foo13 };

				copyVec = myVec;

				Assert::AreEqual(myVec.Front(), copyVec.Front());
				Assert::AreEqual(myVec.Back(), copyVec.Back());
				Assert::AreEqual(myVec[1], copyVec[1]);
				Assert::AreEqual(myVec.Size(), copyVec.Size());
				Assert::AreEqual(myVec.Capacity(), copyVec.Capacity());
			}
	
			//Test self assignment
			{
				const Foo foo1(1);
				const Foo foo2(2);
				const Foo foo3(3);

				Vector<Foo> myVec{ foo1,foo2,foo3 };

				Vector<Foo>::Iterator It = myVec.begin();

				myVec = myVec;
				Assert::AreEqual(It, myVec.begin());
			}
		}
		TEST_METHOD(VectorMoveAssignment)
		{
			const Foo foo1(1);
			const Foo foo2(2);
			const Foo foo3(3);

			Vector<Foo> myVec{ foo1,foo2,foo3 };
			Vector<Foo> moveVec;
			moveVec = static_cast<Vector<Foo>&&>(myVec);

			Assert::AreEqual(foo1, moveVec[0]);
			Assert::AreEqual(foo2, moveVec[1]);
			Assert::AreEqual(foo3, moveVec[2]);


		}
		TEST_METHOD(VectorClear)
		{

			//Test clearing an initialized vector
			{
				const Foo foo1(1);
				const Foo foo2(2);
				const Foo foo3(3);

				Vector<Foo> myVec{ foo1,foo2,foo3 };

				myVec.Clear();

				Assert::AreEqual(static_cast<size_t>(0), myVec.Size());
				Assert::AreEqual(static_cast<size_t>(0), myVec.Capacity());
			}

			//Testing clearing an empty vector
			{
				Vector<Foo> myVec;
				myVec.Clear();
				Assert::AreEqual(static_cast<size_t>(0), myVec.Size());
				Assert::AreEqual(static_cast<size_t>(0), myVec.Capacity());
			}
		}
		TEST_METHOD(VectorReserve)
		{
			//Test if reserving amount less than capacity
			{
				const Foo foo1(1);
				const Foo foo2(2);
				const Foo foo3(3);

				Vector<Foo> myVec{ foo1,foo2,foo3 };

				Assert::IsFalse(myVec.Reserve(0));
			}

			//Test reserving amount larger than capacity
			{
				const Foo foo1(1);
				const Foo foo2(2);
				const Foo foo3(3);

				Vector<Foo> myVec{ foo1,foo2,foo3 };

				Assert::AreEqual(static_cast<size_t>(3), myVec.Capacity());

				myVec.Reserve(5);
				Assert::AreEqual(static_cast<size_t>(5), myVec.Capacity());

				const Foo foo4(4);
				myVec.PushBack(4);

				const Foo foo5(5);
				myVec.PushBack(5);

				Assert::AreEqual(static_cast<size_t>(5), myVec.Capacity());

			}
		}
		TEST_METHOD(VectorResize)
		{

			//Test Resizing an empty list
			{
				const Foo foo1(1);
				const Foo foo2(2);
				const Foo foo3(3);

				const Foo emptyFoo;

				Vector<Foo> myVec;
				myVec.Resize(3);
				Assert::AreEqual(size_t(3), myVec.Size());
				Assert::AreEqual(emptyFoo, myVec[0]);
				Assert::AreEqual(emptyFoo, myVec[1]);
				Assert::AreEqual(emptyFoo, myVec[2]);
			}

			//Test if size passed in is equal to current size
			{
				const Foo foo1(1);
				const Foo foo2(2);
				const Foo foo3(3);

				Vector<Foo> myVec{ foo1,foo2,foo3 };
				myVec.Resize(3);
				Assert::AreEqual(size_t(3), myVec.Size());
				Assert::AreEqual(foo1, myVec[0]);
				Assert::AreEqual(foo2, myVec[1]);
				Assert::AreEqual(foo3, myVec[2]);
			}

			//Test resizing to 0
			{
				const Foo foo1(1);
				const Foo foo2(2);
				const Foo foo3(3);

				Vector<Foo> myVec{ foo1,foo2,foo3 };

				myVec.Resize(0);
				Assert::IsTrue(myVec.IsEmpty());

			}

			//Testing removing the last item in the vector

			{
				const Foo foo1(1);
				const Foo foo2(2);
				const Foo foo3(3);

				Vector<Foo> myVec{ foo1,foo2,foo3 };

				myVec.Resize(2);
				Assert::AreEqual(size_t(2), myVec.Size());
				Assert::AreEqual(foo1, myVec[0]);
				Assert::AreEqual(foo2, myVec[1]);

			}

			//testing removing everything but one item in the vector
			{
				const Foo foo1(1);
				const Foo foo2(2);
				const Foo foo3(3);

				Vector<Foo> myVec{ foo1,foo2,foo3 };

				myVec.Resize(1);
				Assert::AreEqual(size_t(1), myVec.Size());
				Assert::AreEqual(foo1, myVec[0]);
			}

			//Testing setting size to capacity
			{
				const Foo foo1(1);
				const Foo foo2(2);
				const Foo foo3(3);
				const Foo dummyFoo;

				Vector<Foo> myVec{ foo1,foo2,foo3 };

				myVec.Resize(6);

				Assert::AreEqual(size_t(6), myVec.Size());
				Assert::AreEqual(size_t(6), myVec.Capacity());
				Assert::AreEqual(foo1, myVec[0]);
				Assert::AreEqual(foo2, myVec[1]);
				Assert::AreEqual(foo3, myVec[2]);
				Assert::AreEqual(dummyFoo, myVec[3]);
				Assert::AreEqual(dummyFoo, myVec[4]);
				Assert::AreEqual(dummyFoo, myVec[5]);
			}

		}

		TEST_METHOD(VectorRemove)
		{

			//Testing when end() is passed as the parameter
			{
				const Foo foo1(1);
				const Foo foo2(2);
				const Foo foo3(3);

				Vector<Foo> myVec{ foo1,foo2,foo3 };

				Vector<Foo>::Iterator ItRemove = myVec.end();

				Assert::IsFalse(myVec.Remove(ItRemove));
			}

			//removing the last item in a vector
			{
				const Foo foo1(1);
				const Foo foo2(2);
				const Foo foo3(3);

				Vector<Foo> myVec{ foo1,foo2,foo3 };
				Vector<Foo>::Iterator ItRemove = myVec.begin();

				for (int i = 0; i < 2; i++) //ItRemove will iterate to the last item in the vector
				{
					++ItRemove;
				}

				myVec.Remove(ItRemove); //once removed it is invalidated??? check this

				for (Vector<Foo>::Iterator It1 = myVec.begin(); It1 != myVec.end(); ++It1)
				{
					Foo fooValue = *It1;
					fooValue;
				}
				Assert::AreEqual(static_cast<size_t>(2), myVec.Size());
				Assert::AreEqual(static_cast<size_t>(3), myVec.Capacity());
				Assert::AreEqual(foo1, myVec[0]);
				Assert::AreEqual(foo2, myVec[1]);
			}

			//Removing the first item in a vector
			{
				const Foo foo1(1);
				const Foo foo2(2);
				const Foo foo3(3);

				Vector<Foo> myVec{ foo1,foo2,foo3 };
				Vector<Foo>::Iterator ItRemove = myVec.begin();
				myVec.Remove(ItRemove); //once removed it is invalidated??? check this

				for (Vector<Foo>::Iterator It1 = myVec.begin(); It1 != myVec.end(); ++It1)
				{
					Foo fooValue = *It1;
					fooValue;
				}
				Assert::AreEqual(static_cast<size_t>(2), myVec.Size());
				Assert::AreEqual(static_cast<size_t>(3), myVec.Capacity());
				Assert::AreEqual(foo2, myVec[0]);
				Assert::AreEqual(foo3, myVec[1]);
			}


			//removing an item in the middle of a vector
			{
				const Foo foo1(1);
				const Foo foo2(2);
				const Foo foo3(3);
				const Foo foo4(4);
				const Foo foo5(5);

				Vector<Foo> myVec{ foo1, foo2, foo3, foo4, foo5 };
				Vector<Foo>::Iterator ItRemove = myVec.begin();

				//moving the iterator to the third item in the vector
				for (int i = 0; i < 2; i++)
				{
					++ItRemove;
				}

				*ItRemove;

				myVec.Remove(ItRemove); //once removed it is invalidated??? check this

				for (Vector<Foo>::Iterator It1 = myVec.begin(); It1 != myVec.end(); ++It1)
				{
					Foo fooValue = *It1;
					fooValue;
				}

				Assert::AreEqual(static_cast<size_t>(4), myVec.Size());
				Assert::AreEqual(static_cast<size_t>(5), myVec.Capacity());
				Assert::AreEqual(foo1, myVec[0]);
				Assert::AreEqual(foo2, myVec[1]);
				Assert::AreEqual(foo4, myVec[2]);
				Assert::AreEqual(foo5, myVec[3]);
			}
		}
		TEST_METHOD(VectorRemoveAt) //REVIEW : ADDED 3.18.2022
		{
			Foo* myFoo1 = new Foo{ 1 };
			Foo* myFoo2 = new Foo{ 2 };
			Foo* myFoo3 = new Foo{ 3 };

			Vector<Foo*> myVec{ myFoo1, myFoo2, myFoo3 };

			Assert::AreEqual(std::size_t(3), myVec.Size());
			myVec.RemoveAt(1);
			Assert::AreEqual(std::size_t(2), myVec.Size());


			delete myFoo1;
			delete myFoo2;
			delete myFoo3;

		}
		TEST_METHOD(VectorRemoveRange)
		{

			//Testing when end() is passed as the parameter
			{
				const Foo foo1(1);
				const Foo foo2(2);
				const Foo foo3(3);

				Vector<Foo> myVec{ foo1,foo2,foo3 };

				Vector<Foo>::Iterator ItRemove1 = myVec.end();
				Vector<Foo>::Iterator ItRemove2 = myVec.end();

				Assert::IsFalse(myVec.Remove(ItRemove1, ItRemove2));
			}

			//Test when parameters are passed incorrectly from (least),(greatest) to (greatest),(least)
			{
				const Foo foo1(1);
				const Foo foo2(2);
				const Foo foo3(3);

				Vector<Foo> myVec{ foo1,foo2,foo3 };
				Vector<Foo>::Iterator ItRemove1 = myVec.begin();
				Vector<Foo>::Iterator ItRemove2 = myVec.begin();
				++ItRemove2;

				auto Expected = [&myVec, &ItRemove1, &ItRemove2]() {myVec.Remove(ItRemove2, ItRemove1); };
				Assert::ExpectException<std::runtime_error>(Expected);
			}

			//Test when both iterators refer to the same index
			{
				const Foo foo1(1);
				const Foo foo2(2);
				const Foo foo3(3);

				Vector<Foo> myVec{ foo1,foo2,foo3 };
				Vector<Foo>::Iterator ItRemove1 = myVec.begin();
				Vector<Foo>::Iterator ItRemove2 = myVec.begin();
			
				myVec.Remove(ItRemove1, ItRemove2);

				Assert::AreEqual(foo2, myVec[0]);
				Assert::AreEqual(foo3, myVec[1]);
			}

			//Removing the first 5 items in the vector
			{
				const Foo foo0(0);
				const Foo foo1(1);
				const Foo foo2(2);
				const Foo foo3(3);
				const Foo foo4(4);
				const Foo foo5(5);
				const Foo foo6(6);
				const Foo foo7(7);
				const Foo foo8(8);
				const Foo foo9(9);

				Vector<Foo> myVec{ foo0, foo1,foo2,foo3,foo4,foo5,foo6,foo7,foo8,foo9 };

				Vector<Foo>::Iterator ItRemove1 = myVec.begin();

				Vector<Foo>::Iterator ItRemove2 = myVec.begin();

				while (*ItRemove2 != foo4)
				{
					++ItRemove2;
				}

				myVec.Remove(ItRemove1, ItRemove2);

				for (Vector<Foo>::Iterator It1 = myVec.begin(); It1 != myVec.end(); ++It1)
				{
					Foo fooValue = *It1;
					fooValue;
				}
				
				Assert::AreEqual(static_cast<size_t>(5), myVec.Size());
				Assert::AreEqual(static_cast<size_t>(10), myVec.Capacity());
				Assert::AreEqual(foo5, myVec[0]);
				Assert::AreEqual(foo6, myVec[1]);
				Assert::AreEqual(foo7, myVec[2]);
				Assert::AreEqual(foo8, myVec[3]);
				Assert::AreEqual(foo9, myVec[4]);
			}

			//removing the last 5 items in the vector
			{
				const Foo foo0(0);
				const Foo foo1(1);
				const Foo foo2(2);
				const Foo foo3(3);
				const Foo foo4(4);
				const Foo foo5(5);
				const Foo foo6(6);
				const Foo foo7(7);
				const Foo foo8(8);
				const Foo foo9(9);

				Vector<Foo> myVec{ foo0, foo1,foo2,foo3,foo4,foo5,foo6,foo7,foo8,foo9 };
				Vector<Foo>::Iterator ItRemove1 = myVec.begin();
				Vector<Foo>::Iterator ItRemove2 = myVec.begin();

				while (*ItRemove1 != foo5)
				{
					++ItRemove1;
				}

				while (*ItRemove2 != foo9)
				{
					++ItRemove2;
				}

				myVec.Remove(ItRemove1, ItRemove2);

				for (Vector<Foo>::Iterator It1 = myVec.begin(); It1 != myVec.end(); ++It1)
				{
					Foo fooValue = *It1;
					fooValue;
				}

				Assert::AreEqual(static_cast<size_t>(5), myVec.Size());
				Assert::AreEqual(static_cast<size_t>(10), myVec.Capacity());
				Assert::AreEqual(foo0, myVec[0]);
				Assert::AreEqual(foo1, myVec[1]);
				Assert::AreEqual(foo2, myVec[2]);
				Assert::AreEqual(foo3, myVec[3]);
				Assert::AreEqual(foo4, myVec[4]);
			}

			//removing the middle 4 items in the vector
			{
				const Foo foo0(0);
				const Foo foo1(1);
				const Foo foo2(2);
				const Foo foo3(3);
				const Foo foo4(4);
				const Foo foo5(5);
				const Foo foo6(6);
				const Foo foo7(7);
				const Foo foo8(8);
				const Foo foo9(9);

				Vector<Foo> myVec{ foo0, foo1,foo2,foo3,foo4,foo5,foo6,foo7,foo8,foo9 };
				Vector<Foo>::Iterator ItRemove1 = myVec.begin();
				Vector<Foo>::Iterator ItRemove2 = myVec.begin();

				while (*ItRemove1 != foo3)
				{
					++ItRemove1;
				}

				while (*ItRemove2 != foo6)
				{
					++ItRemove2;
				}

				myVec.Remove(ItRemove1, ItRemove2);

				for (Vector<Foo>::Iterator It1 = myVec.begin(); It1 != myVec.end(); ++It1)
				{
					Foo fooValue = *It1;
					fooValue;
				}

				Assert::AreEqual(static_cast<size_t>(6), myVec.Size());
				Assert::AreEqual(static_cast<size_t>(10), myVec.Capacity());
				Assert::AreEqual(foo0, myVec[0]);
				Assert::AreEqual(foo1, myVec[1]);
				Assert::AreEqual(foo2, myVec[2]);
				Assert::AreEqual(foo7, myVec[3]);
				Assert::AreEqual(foo8, myVec[4]);
				Assert::AreEqual(foo9, myVec[5]);

			}

		}
		TEST_METHOD(VectorIteratorBegin)
		{
			const Foo foo1(1);
			const Foo foo2(2);
			const Foo foo3(3);

			Vector<Foo> myVec{ foo1,foo2,foo3 };
			Vector<Foo>::Iterator It = myVec.begin();

			Assert::AreEqual(foo1, *It);
			++It;
			Assert::AreEqual(foo2, *It);
			++It;
			Assert::AreEqual(foo3, *It);


		}
		TEST_METHOD(VectorConstIteratorBegin)
		{
			const Foo foo1(1);
			const Foo foo2(2);
			const Foo foo3(3);

			Vector<Foo> myVec{ foo1,foo2,foo3 };
			Vector<Foo>::ConstIterator It = myVec.begin();

			Assert::AreEqual(foo1, *It);
			++It;
			Assert::AreEqual(foo2, *It);
			++It;
			Assert::AreEqual(foo3, *It);
		}
		TEST_METHOD(VectorConstIteratorCBegin)
		{
			const Foo foo1(1);
			const Foo foo2(2);
			const Foo foo3(3);

			Vector<Foo> myVec{ foo1,foo2,foo3 };

			Vector<Foo>::ConstIterator It = myVec.cbegin();

			Assert::AreEqual(foo1, *It);
			++It;
			Assert::AreEqual(foo2, *It);
			++It;
			Assert::AreEqual(foo3, *It);
		}
		TEST_METHOD(VectorConstIteratorEnd)
		{
			const Foo foo1(1);
			const Foo foo2(2);
			const Foo foo3(3);
			const Vector<Foo> myVec{ foo1,foo2,foo3 };
			Vector<Foo>::ConstIterator It = myVec.end();

			auto Expected = [&It]() {*It; };
			Assert::ExpectException<std::runtime_error>(Expected);
		}
		TEST_METHOD(VectorIteratorEnd)
		{
			const Foo foo1(1);
			const Foo foo2(2);
			const Foo foo3(3);
			Vector<Foo> myVec{ foo1,foo2,foo3 };
			Vector<Foo>::Iterator It = myVec.end();

			auto Expected = [&It]() {*It; };
			Assert::ExpectException<std::runtime_error>(Expected);
		}
		TEST_METHOD(VectorConstIteratorCEnd)
		{

			const Foo foo1(1);
			const Foo foo2(2);
			const Foo foo3(3);

			Vector<Foo> myVec{ foo1,foo2,foo3 };
			Vector<Foo>::ConstIterator It = myVec.cend();

			auto Expected = [&It]() {*It; };
			Assert::ExpectException<std::runtime_error>(Expected);
		}

		TEST_METHOD(VectorFindIterator)
		{
			//item is in the vector
			{
				Foo foo1(1);
				Foo foo2(2);
				Foo foo3(3);

				Vector<Foo> myVec{ foo1,foo2,foo3 };

				Vector<Foo>::Iterator It;
				It = myVec.Find<FooEqualityFunctor>(foo3);

				Assert::AreEqual(foo3, *It);
			}

			//item is not in the vector
			{
				Foo foo1(1);
				Foo foo2(2);
				Foo foo3(3);

				Foo foo4(std::int32_t(4));

				Vector<Foo> myVec{ foo1,foo2,foo3 };

				Vector<Foo>::Iterator It;
				It = myVec.Find<FooEqualityFunctor>(foo4);

				Assert::AreEqual(It, myVec.end());
			}
		}
		TEST_METHOD(VectorFindConstIterator)
		{
			//item is in the vector
			{
				Foo foo1(1);
				Foo foo2(2);
				Foo foo3(3);

				const Vector<Foo> myVec{ foo1,foo2,foo3 };

				Vector<Foo>::ConstIterator It;
				It = myVec.Find<FooEqualityFunctor>(foo3);

				Assert::AreEqual(foo3, *It);

			}

			//item is not in the vector
			{
				Foo foo1(1);
				Foo foo2(2);
				Foo foo3(3);

				Foo foo4(4);

				const Vector<Foo> myVec{ foo1,foo2,foo3 };

				Vector<Foo>::ConstIterator It;
				It = myVec.Find<FooEqualityFunctor>(foo4);

				Assert::AreEqual(It, myVec.end());

			}
		}
#pragma endregion

#pragma region VECTOR ITERATOR FUNCTION TESTS
		TEST_METHOD(IteratorCopyConstructor)
		{
			Foo foo1(1);
			Foo foo2(2);
			Foo foo3(2);

			Vector<Foo> myVec{ foo1, foo2, foo3 };

			Vector<Foo>::Iterator It1 = myVec.begin();

			Vector<Foo>::Iterator It2(It1);
			Assert::AreEqual(It1, It2);

		}
		TEST_METHOD(IteratorOOEqual)
		{
			Foo foo1(1);
			Foo foo2(2);
			Foo foo3(3);

			Vector<Foo> myVec1{ foo1, foo2, foo3 };
			Vector<Foo> myVec2{ foo1, foo2, foo3 };

			Vector<Foo>::Iterator It1 = myVec1.begin();
			Vector<Foo>::Iterator It2 = myVec1.begin();

			Vector<Foo>::Iterator It3 = myVec2.begin();

			//Same owner, same index
			bool result = (It1 == It2);
			Assert::IsTrue(result);
			
			It1++;

			//Same owner,different index
			result = (It1 == It2);
			Assert::IsFalse(result);

			//different owner, same index
			result = (It2 == It3);
			Assert::IsFalse(result);

		}
		TEST_METHOD(IteratorOONotEqual)
		{
			Foo foo1(1);
			Foo foo2(2);
			Foo foo3(3);

			Vector<Foo> myVec1{ foo1, foo2, foo3 };
			Vector<Foo> myVec2{ foo1, foo2, foo3 };

			Vector<Foo>::Iterator It1 = myVec1.begin();
			Vector<Foo>::Iterator It2 = myVec1.begin();

			Vector<Foo>::Iterator It3 = myVec2.begin();

			//Same owner, same index
			bool result = (It1 != It2);
			Assert::IsFalse(result);

			It1++;

			//Same owner,different index
			result = (It1 != It2);
			Assert::IsTrue(result);

			//different owner, same index
			result = (It2 != It3);
			Assert::IsTrue(result);
		}
		TEST_METHOD(IteratorOOPostIncrement)
		{
			//Testing a unassigned iterator
			{
				Vector<Foo>::Iterator It;
				auto Expected = [&It]() {It++; };
				Assert::ExpectException<std::runtime_error>(Expected);
			}

			//Testing an assigned iterator
			{
				Foo foo1(1);
				Foo foo2(2);
				Foo foo3(3);

				Vector<Foo> myVec{ foo1, foo2, foo3 };

				Vector<Foo>::Iterator It = myVec.begin();

				Assert::AreEqual(foo1, *It);
				It++;
				Assert::AreEqual(foo2, *It);
				It++;
				Assert::AreEqual(foo3, *It);

			}

			//Testing post increment logic
			{
				{
					Foo foo1(1);
					Foo foo2(2);
					Foo foo3(3);

					Vector<Foo> myVec{ foo1, foo2, foo3 };

					Vector<Foo>::Iterator It = myVec.begin();
					Vector<Foo>::Iterator It1 = myVec.begin();

					It1 = It++;

					Assert::AreEqual(foo1, *It1);
					Assert::AreEqual(foo2, *It);
				}
			}
		}
		TEST_METHOD(IteratorOOPreIncrement)
		{

			//Testing a unassigned iterator
			{
				Vector<Foo>::Iterator It;
				auto Expected = [&It]() {++It; }; //increment an unassigned iterator
				Assert::ExpectException<std::runtime_error>(Expected);
			}

			//Testing an assigned iterator
			{
				Foo foo1(1);
				Foo foo2(2);
				Foo foo3(3);

				Vector<Foo> myVec{ foo1, foo2, foo3 };

				Vector<Foo>::Iterator It = myVec.begin();

				Assert::AreEqual(foo1, *It);
				++It;
				Assert::AreEqual(foo2, *It);
				++It;
				Assert::AreEqual(foo3, *It);

			}

			//Testing post increment logic
			{
				{
					Foo foo1(1);
					Foo foo2(2);
					Foo foo3(3);

					Vector<Foo> myVec{ foo1, foo2, foo3 };

					Vector<Foo>::Iterator It = myVec.begin();
					Vector<Foo>::Iterator It1 = myVec.begin();

					It1 = ++It;

					Assert::AreEqual(foo2, *It1);
				}
			}

		}
		TEST_METHOD(IteratorOODereference)
		{
			//Testing a unassigned iterator
			{
				Vector<Foo>::Iterator It;
				auto Expected = [&It]() {*It; }; //increment an unassigned iterator
				Assert::ExpectException<std::runtime_error>(Expected);
			}

			//Testing with size 0
			{
				Vector<Foo> myVec;
				Vector<Foo>::Iterator It = myVec.begin();
				auto Expected = [&It]() {*It; }; //increment an unassigned iterator
				Assert::ExpectException<std::runtime_error>(Expected);
			}

			//Testing with end
			{
				Foo foo1(1);
				Foo foo2(2);
				Foo foo3(3);

				Vector<Foo> myVec{ foo1, foo2, foo3 };
				Vector<Foo>::Iterator It = myVec.end();
				auto Expected = [&It]() {*It; }; //increment an unassigned iterator
				Assert::ExpectException<std::runtime_error>(Expected);
			}
			//Testing an assigned iterator
			{
				Foo foo1(1);
				Foo foo2(2);
				Foo foo3(3);

				Vector<Foo> myVec{ foo1, foo2, foo3 };

				Vector<Foo>::Iterator It = myVec.begin();

				Assert::AreEqual(foo1, *It);
				++It;
				Assert::AreEqual(foo2, *It);
				++It;
				Assert::AreEqual(foo3, *It);

			}
		}
		TEST_METHOD(IteratorPrivateConstructor)
		{
			Foo foo1(1);
			Foo foo2(2);
			Foo foo3(3);


			Vector<Foo> myVec{ foo1, foo2, foo3 };

			Vector<Foo>::Iterator It = myVec.begin();
			Assert::AreEqual(foo1, *It);
		}
#pragma endregion

#pragma region VECTOR CONST-ITERATOR FUNCTION TESTS
		TEST_METHOD(ConstIteratorCopyConstructor)
		{
			//const vector test. Testing ConstIterator constructing from ConstIterator
			{
				Foo foo1(1);
				Foo foo2(2);
				Foo foo3(2);

				const Vector<Foo> list1{ foo1, foo2, foo3 };

				Vector<Foo>::ConstIterator It1 = list1.begin();

				Vector<Foo>::ConstIterator It2(It1);

				Assert::AreEqual(It1, It2);
			}

			//non const vector test. Testing ConstIterator constructing from ConstIterator
			{
				Foo foo1(1);
				Foo foo2(2);
				Foo foo3(2);

				Vector<Foo> list1{ foo1, foo2, foo3 };
				Vector<Foo>::ConstIterator It1 = list1.cbegin();
				Vector<Foo>::ConstIterator It2(It1);
				Assert::AreEqual(It1, It2);
			}
		}
		TEST_METHOD(ConstIteratorTypecastConstructor)
		{
			Foo foo1(1);
			Foo foo2(2);
			Foo foo3(2);

			Vector<Foo> list1{ foo1, foo2, foo3 };
			Vector<Foo>::Iterator It1 = list1.begin();
			Vector<Foo>::ConstIterator It2(It1);
			Assert::AreEqual(*It1, *It2);
		}
		TEST_METHOD(ConstIteratorOOEqual)
		{
			Foo foo1(1);
			Foo foo2(2);
			Foo foo3(3);

			const Vector<Foo> myVec1{ foo1, foo2, foo3 };
			const Vector<Foo> myVec2{ foo1, foo2, foo3 };

			Vector<Foo>::ConstIterator It1 = myVec1.begin();
			Vector<Foo>::ConstIterator It2 = myVec1.begin();

			Vector<Foo>::ConstIterator It3 = myVec2.begin();

			//Same owner, same index
			bool result = (It1 == It2);
			Assert::IsTrue(result);

			It1++;

			//Same owner,different index
			result = (It1 == It2);
			Assert::IsFalse(result);

			//different owner, same index
			result = (It2 == It3);
			Assert::IsFalse(result);

		}
		TEST_METHOD(ConstIteratorOONotEqual)
		{
			Foo foo1(1);
			Foo foo2(2);
			Foo foo3(3);

			const Vector<Foo> myVec1{ foo1, foo2, foo3 };
			const Vector<Foo> myVec2{ foo1, foo2, foo3 };

			Vector<Foo>::ConstIterator It1 = myVec1.begin();
			Vector<Foo>::ConstIterator It2 = myVec1.begin();

			Vector<Foo>::ConstIterator It3 = myVec2.begin();

			//Same owner, same index
			bool result = (It1 != It2);
			Assert::IsFalse(result);

			It1++;

			//Same owner,different index
			result = (It1 != It2);
			Assert::IsTrue(result);

			//different owner, same index
			result = (It2 != It3);
			Assert::IsTrue(result);
		}
		TEST_METHOD(ConstIteratorOOPostIncrement)
		{
			//testing a unassigned ConstIterator
			{
				Vector<Foo>::ConstIterator It;
				auto Expected = [&It]() {It++; };
				Assert::ExpectException<std::runtime_error>(Expected);
			}

			//const vector testing an assigned ConstIterator
			{
				Foo foo1(1);
				Foo foo2(2);
				Foo foo3(3);

				const Vector<Foo> list1{ foo1, foo2, foo3 };

				Vector<Foo>::ConstIterator It = list1.begin();

				Assert::AreEqual(foo1, *It);
				It++;
				Assert::AreEqual(foo2, *It);
				It++;
				Assert::AreEqual(foo3, *It);
			}

			//non const vector testing an assigned ConstIterator
			{
				Foo foo1(1);
				Foo foo2(2);
				Foo foo3(3);

				Vector<Foo> list1{ foo1, foo2, foo3 };

				Vector<Foo>::ConstIterator It = list1.cbegin();

				Assert::AreEqual(foo1, *It);
				It++;
				Assert::AreEqual(foo2, *It);
				It++;
				Assert::AreEqual(foo3, *It);
			}

			//const vector testing post increment logic
			{

				Foo foo1(1);
				Foo foo2(2);
				Foo foo3(3);

				const Vector<Foo> list1{ foo1, foo2, foo3 };

				Vector<Foo>::ConstIterator It = list1.begin();
				Vector<Foo>::ConstIterator It1 = list1.begin();

				It1 = It++;

				Assert::AreEqual(foo1, *It1);
			}

			//non const vector testing post increment logic
			{

				Foo foo1(1);
				Foo foo2(2);
				Foo foo3(3);

				Vector<Foo> list1{ foo1, foo2, foo3 };

				Vector<Foo>::ConstIterator It = list1.cbegin();
				Vector<Foo>::ConstIterator It1 = list1.cbegin();

				It1 = It++;

				Assert::AreEqual(foo1, *It1);
			}
		}
		TEST_METHOD(ConstIteratorOOPreIncrement)
		{

			//testing a unassigned iterator
			{
				Vector<Foo>::Iterator It;
				auto Expected = [&It]() {++It; }; //increment an unassigned iterator
				Assert::ExpectException<std::runtime_error>(Expected);
			}

			//const vector testing an assigned iterator
			{
				Foo foo1(1);
				Foo foo2(2);
				Foo foo3(3);

				const Vector<Foo> list1{ foo1, foo2, foo3 };

				Vector<Foo>::ConstIterator It = list1.begin();

				Assert::AreEqual(foo1, *It);
				++It;
				Assert::AreEqual(foo2, *It);
				++It;
				Assert::AreEqual(foo3, *It);

			}

			//non const vector testing an assigned iterator
			{
				Foo foo1(1);
				Foo foo2(2);
				Foo foo3(3);

				Vector<Foo> list1{ foo1, foo2, foo3 };

				Vector<Foo>::ConstIterator It = list1.cbegin();

				Assert::AreEqual(foo1, *It);
				++It;
				Assert::AreEqual(foo2, *It);
				++It;
				Assert::AreEqual(foo3, *It);

			}

			//Testing post increment logic
			{
				{
					Foo foo1(1);
					Foo foo2(2);
					Foo foo3(3);

					Vector<Foo> list1{ foo1, foo2, foo3 };

					Vector<Foo>::Iterator It = list1.begin();
					Vector<Foo>::Iterator It1 = list1.begin();

					It1 = ++It;

					Assert::AreEqual(foo2, *It1);
				}
			}

		}
		TEST_METHOD(ConstIteratorOODereference)
		{
			//testing a unassigned iterator
			{
				Vector<Foo>::ConstIterator It;
				auto Expected = [&It]() {*It; }; //increment an unassigned iterator
				Assert::ExpectException<std::runtime_error>(Expected);
			}

			//Testing with size 0
			{
				const Vector<Foo> myVec;
				Vector<Foo>::ConstIterator It = myVec.begin();
				auto Expected = [&It]() {*It; }; //increment an unassigned iterator
				Assert::ExpectException<std::runtime_error>(Expected);
			}

			//const vector testing an assigned iterator
			{
				Foo foo1(1);
				Foo foo2(2);
				Foo foo3(3);

				const Vector<Foo> list1{ foo1, foo2, foo3 };

				Vector<Foo>::ConstIterator It = list1.begin();

				Assert::AreEqual(foo1, *It);
				++It;
				Assert::AreEqual(foo2, *It);
				++It;
				Assert::AreEqual(foo3, *It);

			}

			//non const vector testing an assigned iterator
			{
				Foo foo1(1);
				Foo foo2(2);
				Foo foo3(3);

				Vector<Foo> list1{ foo1, foo2, foo3 };

				Vector<Foo>::ConstIterator It = list1.cbegin();

				Assert::AreEqual(foo1, *It);
				++It;
				Assert::AreEqual(foo2, *It);
				++It;
				Assert::AreEqual(foo3, *It);

			}

		}
		TEST_METHOD(ConstIteratorPrivateConstructor)
		{
			//const vector
			{
				Foo foo1(1);
				Foo foo2(2);
				Foo foo3(3);

				const Vector<Foo> list1{ foo1, foo2, foo3 };

				Vector<Foo>::ConstIterator It = list1.begin();
				Assert::AreEqual(foo1, *It);
			}

			//non const vector
			{
				Foo foo1(1);
				Foo foo2(2);
				Foo foo3(3);

				Vector<Foo> list1{ foo1, foo2, foo3 };

				Vector<Foo>::ConstIterator It = list1.cbegin();
				Assert::AreEqual(foo1, *It);
			}
		}

#pragma endregion

		static _CrtMemState _startMemState; //static class members are shared in every instance of the class
	};
	_CrtMemState VectorTests::_startMemState; //There is needed to separate declaration from definition. Changes in C++17

}