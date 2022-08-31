#include "pch.h"
#include "ToStringSpecializations.h"
#include "SList.h"
#include "Foo.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace FieaGameEngine;

namespace UnitTestLibraryDesktop
{


	TEST_CLASS(SListTests)
	{
	public:

#define INITALIZED_SLIST_FOO_3											\
	const Foo foo1_macro(std::int32_t(1));								\
	const Foo foo2_macro(std::int32_t(2));								\
	const Foo foo3_macro(std::int32_t(3));								\
	SList<Foo> list_foo_3_macro{ foo1_macro, foo2_macro, foo3_macro };	\


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
#pragma region SLIST FUNCTION TESTS
		TEST_METHOD(SListDefaultConstructor)
		{
			SList<Foo> list1;
			Assert::AreEqual(size_t(0), list1.Size());
		}
		TEST_METHOD(SListCopyConstructor)
		{
			//Test with empty list
			{
				SList<Foo> list1;
				SList<Foo> list2(list1);

				auto Expected1 = [&list1]() {list1.Front(); };

				Assert::ExpectException<std::runtime_error>(Expected1);

				auto Expected2 = [&list2]() {list2.Front(); };

				Assert::ExpectException<std::runtime_error>(Expected2);

			}
			
			//Test with initialized list
			{
				SList<Foo> list1;
				const Foo foo1(std::int32_t(1));
				const Foo foo2(std::int32_t(2));
				const Foo foo3(std::int32_t(3));

				list1.PushFront(foo1);
				list1.PushFront(foo2);
				list1.PushFront(foo3);

				SList<Foo> list2(list1);

				while (!list1.IsEmpty())
				{
					Assert::AreEqual(list1.Front(), list2.Front());
					Assert::AreEqual(list1.Size(), list2.Size());

					list1.PopFront();
					list2.PopFront();
				}

			}
		}
		TEST_METHOD(SListConstructorInitializerList)
		{

			INITALIZED_SLIST_FOO_3
			/*const Foo foo1(std::int32_t(1));
			const Foo foo2(std::int32_t(2));
			const Foo foo3(std::int32_t(3));

			SList<Foo> list1{ foo1, foo2, foo3 };*/

			Assert::AreEqual(foo1_macro, list_foo_3_macro.Front());
			Assert::AreEqual(foo3_macro, list_foo_3_macro.Back());

			list_foo_3_macro.PopBack();
			Assert::AreEqual(foo2_macro, list_foo_3_macro.Back());

			list_foo_3_macro.PopBack();
			Assert::AreEqual(foo1_macro, list_foo_3_macro.Back());

		}
		TEST_METHOD(SListFront)
		{
			//Test empty list
			{
				const Foo foo1(std::int32_t(1));
				SList<Foo> list1;

				auto Expected = [&list1]() {list1.Front(); };
				Assert::ExpectException<std::runtime_error>(Expected);
			}

			//Test initialized list
			{
				const Foo foo1(std::int32_t(1));
				const Foo foo2(std::int32_t(2));
				const Foo foo3(std::int32_t(3));

				SList<Foo> list1;

				list1.PushFront(foo1);
				Assert::AreEqual(foo1, list1.Front());

				list1.PushFront(foo2);
				Assert::AreEqual(foo2, list1.Front());

				list1.PushFront(foo3);
				Assert::AreEqual(foo3, list1.Front());

			}
			
		}
		TEST_METHOD(SListBack)
		{

			//Test empty list
			{
				const Foo foo1(std::int32_t(1));
				SList<Foo> list1;

				auto Expected = [&list1]() {list1.Back(); };
				Assert::ExpectException<std::runtime_error>(Expected);
			}

			//Test Initialized List
			{
				const Foo foo1(std::int32_t(1));
				const Foo foo2(std::int32_t(2));
				const Foo foo3(std::int32_t(3));
				SList<Foo> list1;
				list1.PushBack(foo1);
				Assert::AreEqual(foo1, list1.Back());

				list1.PushBack(foo2);
				Assert::AreEqual(foo2, list1.Back());

				list1.PushBack(foo3);
				Assert::AreEqual(foo3, list1.Back());
			}
		}
		TEST_METHOD(SListFrontConst)
		{
			//Test empty list
			{
				const Foo foo1(std::int32_t(1));
				const SList<Foo> list1;

				auto Expected = [&list1]() {list1.Front(); };

				Assert::ExpectException<std::runtime_error>(Expected);
			}

			//Test initialized list
			{
				const Foo foo1(std::int32_t(1));
				const Foo foo2(std::int32_t(2));
				const Foo foo3(std::int32_t(3));

				const SList<Foo> list1{ foo1,foo2,foo3 };

				Assert::AreEqual(foo1, list1.Front());
			}

		}
		TEST_METHOD(SListBackConst)
		{

			//Test empty list
			{
				const Foo foo1(std::int32_t(1));
				const SList<Foo> list1;

				auto Expected = [&list1]() {list1.Back(); };

				Assert::ExpectException<std::runtime_error>(Expected);
			}

			//Testing initialized list
			{
				const Foo foo1(std::int32_t(1));
				const Foo foo2(std::int32_t(2));
				const Foo foo3(std::int32_t(3));
				const SList<Foo> list1{ foo1, foo2, foo3 };
				Assert::AreEqual(foo3, list1.Back());
			}
		}
		TEST_METHOD(SListSize)
		{
			//Test with empty list
			{
				SList<Foo> list1;
				Assert::AreEqual(size_t(0), list1.Size());
			}

			//Testing size while adding items to the list
			{
				const Foo foo1(std::int32_t(1));
				const Foo foo2(std::int32_t(2));
				const Foo foo3(std::int32_t(3));
				SList<Foo> list1;
				list1.PushFront(foo1);
				Assert::AreEqual(size_t(1), list1.Size());

				list1.PushFront(foo2);
				Assert::AreEqual(size_t(2), list1.Size());
				
				list1.PushFront(foo3);
				Assert::AreEqual(size_t(3), list1.Size());
			}
			
			//Testing size while removing items from the list
			{
				const Foo foo1(std::int32_t(1));
				const Foo foo2(std::int32_t(2));
				const Foo foo3(std::int32_t(3));
				SList<Foo> list1;
				list1.PushFront(foo1);
				list1.PushFront(foo2);
				list1.PushFront(foo3);

				list1.PopFront();
				Assert::AreEqual(size_t(2), list1.Size());
				list1.PopFront();
				Assert::AreEqual(size_t(1), list1.Size());
				list1.PopFront();
				Assert::AreEqual(size_t(0), list1.Size());
			}
		}
		TEST_METHOD(SListPushFront)
		{
			
			const Foo foo1(std::int32_t(1));
			const Foo foo2(std::int32_t(2));
			const Foo foo3(std::int32_t(3));

			SList<Foo> list1;

			list1.PushFront(foo1);
			Assert::AreEqual(foo1, list1.Front());

			list1.PushFront(foo2);
			Assert::AreEqual(foo2, list1.Front());

			list1.PushFront(foo3);
			Assert::AreEqual(foo3, list1.Front());
			
			
		}
		TEST_METHOD(SListPushBack)
		{
			const Foo foo1(std::int32_t(1));
			const Foo foo2(std::int32_t(2));
			const Foo foo3(std::int32_t(3));

			SList<Foo> list1;

			list1.PushBack(foo1);
			Assert::AreEqual(foo1, list1.Back());

			list1.PushBack(foo2);
			Assert::AreEqual(foo2, list1.Back());

			list1.PushBack(foo3);
			Assert::AreEqual(foo3, list1.Back());
		}
		TEST_METHOD(SListPopFront)
		{
			const Foo foo1(std::int32_t(1));
			const Foo foo2(std::int32_t(2));
			const Foo foo3(std::int32_t(3));

			SList<Foo> list1;

			list1.PushFront(foo1);
			list1.PushFront(foo2);
			list1.PushFront(foo3);

			Assert::AreEqual(foo3, list1.Front());
			
			list1.PopFront();
			Assert::AreEqual(foo2, list1.Front());
			
			list1.PopFront();
			Assert::AreEqual(foo1, list1.Front());
		}
		TEST_METHOD(SListPopBack)
		{
			//Testing empty list
			{
				SList<Foo> list1;

				Assert::IsFalse(list1.PopBack());
			}

			//Testing Initialized list
			{
				const Foo foo1(std::int32_t(1));
				const Foo foo2(std::int32_t(2));
				const Foo foo3(std::int32_t(3));

				SList<Foo> list1;

				list1.PushFront(foo1);
				list1.PushFront(foo2);
				list1.PushFront(foo3);

				Assert::AreEqual(foo1, list1.Back());

				list1.PopBack();
				Assert::AreEqual(foo2, list1.Back());

				list1.PopBack();
				Assert::AreEqual(foo3, list1.Back());

			}
		}
		TEST_METHOD(SListClear)
		{
			//Testing empty list
			{
				SList<Foo> list1;
				list1.Clear();

				Assert::AreEqual(size_t(0), list1.Size());
			}

			//Testing an initialized list
			{
				const Foo foo1(std::int32_t(1));
				const Foo foo2(std::int32_t(2));
				const Foo foo3(std::int32_t(3));
				SList<Foo> list1;
				list1.PushFront(foo1);
				list1.PushFront(foo2);
				list1.PushFront(foo3);

				list1.Clear();

				Assert::AreEqual(size_t(0), list1.Size());

			}
		}
		TEST_METHOD(SListIsEmpty)
		{
			//Testing an empty list
			{
				SList<Foo> list1;
				Assert::AreEqual(true, list1.IsEmpty());
			}

			//Testing an initialized list
			{
				const Foo foo1(std::int32_t(1));
				const Foo foo2(std::int32_t(2));
				const Foo foo3(std::int32_t(3));
				SList<Foo> list1;
				list1.PushFront(foo1);
				list1.PushFront(foo2);
				list1.PushFront(foo3);

				Assert::AreEqual(false, list1.IsEmpty());

				list1.Clear();

				Assert::AreEqual(true, list1.IsEmpty());
			}
		}
		TEST_METHOD(SListOOAssignment) //Operator Overload Assignment
		{
			//Testing empty list
			{
				SList<Foo> list1;
				SList<Foo> list2;
				list2 = list1;
				Assert::AreEqual(list1.Size(), list2.Size());

			}

			//Testing initialized list
			{
				const Foo foo1(std::int32_t(1));
				const Foo foo2(std::int32_t(2));
				const Foo foo3(std::int32_t(3));

				SList<Foo> list1;

				list1.PushBack(foo1);
				list1.PushBack(foo2);
				list1.PushBack(foo3);

				SList<Foo> list2;

				list2 = list1;

				while (!list2.IsEmpty()) {
					Assert::AreEqual(list1.Size(), list2.Size());
					Assert::AreEqual(list1.Front(), list2.Front());
					Assert::AreEqual(list1.Back(), list2.Back());

					list1.PopFront();
					list2.PopFront();
				}

			}

			//Testing if assigned to self
			{
				const Foo foo1(std::int32_t(1));
				const Foo foo2(std::int32_t(2));
				const Foo foo3(std::int32_t(3));

				SList<Foo> list1;

				list1.PushBack(foo1);
				list1.PushBack(foo2);
				list1.PushBack(foo3);

				SList<Foo>& referenctList = list1;

				list1 = referenctList;

			}

		}
		TEST_METHOD(SListInitializerListConstructor) //Review
		{

			const Foo foo1(std::int32_t(1));
			const Foo foo2(std::int32_t(2));
			const Foo foo3(std::int32_t(3));

			SList<Foo> list{ foo1, foo2, foo3 };
			SList<Foo>::Iterator It = list.begin();
		
			Assert::AreEqual(foo1, *It);
			It++;
			Assert::AreEqual(foo2, *It);
			It++;
			Assert::AreEqual(foo3, *It);

		}
		TEST_METHOD(SListIteratorBegin)
		{
			const Foo foo1(std::int32_t(1));
			const Foo foo2(std::int32_t(2));
			const Foo foo3(std::int32_t(3));
			SList<Foo> list{ foo1,foo2,foo3};
			SList<Foo>::Iterator It;
			It = list.begin();
			Assert::AreEqual(foo1, *It);
		}
		TEST_METHOD(SListConstIteratorBegin)
		{
			const Foo foo1(std::int32_t(1));
			const Foo foo2(std::int32_t(2));
			const Foo foo3(std::int32_t(3));
			const SList<Foo> list{ foo1,foo2,foo3 };
			SList<Foo>::ConstIterator It = list.begin();

			Assert::AreEqual(foo1, *It);
			++It;
			Assert::AreEqual(foo2, *It);
			++It;
			Assert::AreEqual(foo3, *It);
		}
		TEST_METHOD(SListConstIteratorCBegin)
		{
			const Foo foo1(std::int32_t(1));
			const Foo foo2(std::int32_t(2));
			const Foo foo3(std::int32_t(3));
			SList<Foo> list{ foo1,foo2,foo3 };
			SList<Foo>::ConstIterator It = list.cbegin();

			Assert::AreEqual(foo1, *It);
			++It;
			Assert::AreEqual(foo2, *It);
			++It;
			Assert::AreEqual(foo3, *It);
		}
		TEST_METHOD(SListIteratorEnd)
		{
			const Foo foo1(std::int32_t(1));
			const Foo foo2(std::int32_t(2));
			const Foo foo3(std::int32_t(3));
			SList<Foo> list{ foo1,foo2,foo3 };
			SList<Foo>::Iterator It = list.end();
			
			auto Expected = [&It]() {*It; };
			Assert::ExpectException<std::runtime_error>(Expected);

		}
		TEST_METHOD(SListConstIteratorEnd) //REVIEW
		{
			const Foo foo1(std::int32_t(1));
			const Foo foo2(std::int32_t(2));
			const Foo foo3(std::int32_t(3));
			const SList<Foo> list{ foo1,foo2,foo3 };
			SList<Foo>::ConstIterator It = list.end();

			auto Expected = [&It]() {*It; };
			Assert::ExpectException<std::runtime_error>(Expected);

		}
		TEST_METHOD(SListConstIteratorCEnd) //REVIEW
		{
			const Foo foo1(std::int32_t(1));
			const Foo foo2(std::int32_t(2));
			const Foo foo3(std::int32_t(3));
			SList<Foo> list{ foo1,foo2,foo3 };
			SList<Foo>::ConstIterator It = list.cend();

			auto Expected = [&It]() {*It; };
			Assert::ExpectException<std::runtime_error>(Expected);

		}
		TEST_METHOD(SListFindIterator)
		{
			//testing empty list
			{
				const Foo foo1(std::int32_t(1));
				SList<Foo> list;
				Assert::AreEqual(list.end(), list.Find<FooEqualityFunctor>(foo1));
			}
			//item is in the list
			{
				const Foo foo1(std::int32_t(1));
				const Foo foo2(std::int32_t(2));
				const Foo foo3(std::int32_t(3));

				SList<Foo> list{ foo1,foo2,foo3 };

				SList<Foo>::Iterator It;
				It = list.Find<FooEqualityFunctor>(foo3);

				Assert::AreEqual(foo3, *It);
			}

			//item is not in the list
			{
				const Foo foo1(std::int32_t(1));
				const Foo foo2(std::int32_t(2));
				const Foo foo3(std::int32_t(3));

				const Foo foo4(std::int32_t(4));

				SList<Foo> list{ foo1,foo2,foo3 };

				SList<Foo>::Iterator It;
				It = list.Find<FooEqualityFunctor>(foo4);

				Assert::AreEqual(It, list.end());
			}
		}
		TEST_METHOD(SListFindConstIterator)
		{

			//testing empty list
			{
				const Foo foo1(std::int32_t(1));
				const SList<Foo> list;
				Assert::AreEqual(list.end(), list.Find<FooEqualityFunctor>(foo1));
			}

			//item is in the list
			{
				const Foo foo1(std::int32_t(1));
				const Foo foo2(std::int32_t(2));
				const Foo foo3(std::int32_t(3));

				const SList<Foo> list{ foo1,foo2,foo3 };

				SList<Foo>::ConstIterator It;
				It = list.Find<FooEqualityFunctor>(foo3);

				Assert::AreEqual(foo3, *It);

			}

			//item is not in the list
			{
				const Foo foo1(std::int32_t(1));
				const Foo foo2(std::int32_t(2));
				const Foo foo3(std::int32_t(3));

				const Foo foo4(std::int32_t(4));

				const SList<Foo> list{ foo1,foo2,foo3 };

				SList<Foo>::ConstIterator It;
				It = list.Find<FooEqualityFunctor>(foo4);

				Assert::AreEqual(It, list.end());

			}
			
			
		}
		TEST_METHOD(SListInsertAfter)
		{

			//Inserting into initialized list
			{
				const Foo foo1(std::int32_t(1));
				const Foo foo2(std::int32_t(2));
				const Foo foo3(std::int32_t(3));

				const Foo foo4(std::int32_t(4));

				SList<Foo> list{ foo1,foo2,foo3 };

				SList<Foo>::Iterator It = list.begin();
				++It;

				list.InsertAfter(foo4, It);

				It = list.begin();
				++It;
				++It;
				Assert::AreEqual(foo4, *It);
			}
			//Insert into empty container
			{
				const Foo foo1(std::int32_t(1));
				SList<Foo> list;
				SList<Foo>::Iterator It = list.begin();
				list.InsertAfter(foo1, It);
				It = list.begin();
				Assert::AreEqual(foo1, *It);
			}
			//Insert into the end of an initialized list
			{
				
				const Foo foo1(std::int32_t(1));
				const Foo foo2(std::int32_t(2));
				const Foo foo3(std::int32_t(3));
				const Foo foo4(std::int32_t(4));

				SList<Foo> list{ foo1,foo2,foo3 };

				SList<Foo>::Iterator It = list.end();
				
				list.InsertAfter(foo4, It);

				It = list.begin();

				It++;
				It++;
				It++;

				Assert::AreEqual(foo4, *It);

			}
			//Inserting into unassigned iterator
			{
				const Foo foo1(std::int32_t(1));
				const Foo foo2(std::int32_t(2));
				const Foo foo3(std::int32_t(3));

				SList<Foo> list{ foo1,foo2,foo3 };


				auto Expected = [&list]()
				{
					SList<Foo>::Iterator It;
					const Foo foo4(std::int32_t(4));
					list.InsertAfter(foo4, It);
				};

				Assert::ExpectException<std::runtime_error>(Expected);
			}
		}
		TEST_METHOD(SListRemove)
		{
			//Removing a value from an initialized list
			{
				const Foo foo1(std::int32_t(1));
				const Foo foo2(std::int32_t(2));
				const Foo foo3(std::int32_t(3));


				SList<Foo> list{ foo1, foo2, foo3 };

				list.Remove(foo1);

				SList<Foo>::Iterator It = list.begin();
				Assert::AreEqual(foo2, *It);
			}
			
			//Removing from an empty list
			{
				const Foo foo1(std::int32_t(1));

				SList<Foo> list;

				auto Expected1 = [&list]() {list.Front(); };
				Assert::ExpectException<std::runtime_error>(Expected1);

				list.Remove(foo1);

				auto Expected2 = [&list]() {list.Front(); };
				Assert::ExpectException<std::runtime_error>(Expected2);

			}

			//Removing from a list with one item
			{
				const Foo foo1(std::int32_t(1));

				SList<Foo> list{ foo1 };

				list.Remove(foo1);

				Assert::IsTrue(list.IsEmpty());
			}
			//Removing the last item on the list
			{
				const Foo foo1(std::int32_t(1));
				const Foo foo2(std::int32_t(2));
				const Foo foo3(std::int32_t(3));

				SList<Foo> list{ foo1, foo2, foo3 };
				SList<Foo>::Iterator It = list.begin();
				list.Remove(foo3);

				Assert::AreEqual(foo2, list.Back() );
				

			}
		}
#pragma endregion

#pragma region SLIST ITERTATOR FUNCTION TESTS
		TEST_METHOD(IteratorCopyConstructor)
		{
			const Foo foo1(std::int32_t(1));
			const Foo foo2(std::int32_t(2));
			const Foo foo3(std::int32_t(2));

			SList<Foo> list1 {foo1, foo2, foo3 };

			SList<Foo>::Iterator It1 = list1.begin();

			SList<Foo>::Iterator It2(It1);
			Assert::AreEqual(It1, It2);

		}
		TEST_METHOD(IteratorOOEqual)
		{

			const Foo foo1(std::int32_t(1));
			const Foo foo2(std::int32_t(2));
			const Foo foo3(std::int32_t(3));

			SList<Foo> list{ foo1, foo2, foo3 };

			SList<Foo>::Iterator It = list.begin();
			SList<Foo>::Iterator It2 = list.begin();

			Assert::AreEqual(It, It2);

		}
		TEST_METHOD(IteratorOONotEqual)
		{

			const Foo foo1(std::int32_t(1));
			const Foo foo2(std::int32_t(2));
			const Foo foo3(std::int32_t(3));

			SList<Foo> list1 { foo1, foo2, foo3 };
			SList<Foo> list2 { foo1, foo2, foo3 };

			SList<Foo>::Iterator It = list1.begin();
			SList<Foo>::Iterator It2 = list2.begin();

			Assert::AreNotEqual(It, It2);

		}
		TEST_METHOD(IteratorOOPostIncrement)
		{
			//Testing a unassigned iterator
			{
				SList<Foo>::Iterator it;
				auto Expected = [&it]() {it++; };
				Assert::ExpectException<std::runtime_error>(Expected);
			}

			//Testing an assigned iterator
			{
				const Foo foo1(std::int32_t(1));
				const Foo foo2(std::int32_t(2));
				const Foo foo3(std::int32_t(3));

				SList<Foo> list1{ foo1, foo2, foo3 };

				SList<Foo>::Iterator It = list1.begin();

				Assert::AreEqual(foo1, *It);
				It++;
				Assert::AreEqual(foo2, *It);
				It++;
				Assert::AreEqual(foo3, *It);

			}

			//Testing post increment logic
			{
				{
					const Foo foo1(std::int32_t(1));
					const Foo foo2(std::int32_t(2));
					const Foo foo3(std::int32_t(3));

					SList<Foo> list1{ foo1, foo2, foo3 };

					SList<Foo>::Iterator It = list1.begin();
					SList<Foo>::Iterator It1 = list1.begin();

					It1 = It++;

					Assert::AreEqual(foo1, *It1);
				}
			}
		}
		TEST_METHOD(IteratorOOPreIncrement)
		{

			//Testing a unassigned iterator
			{
				SList<Foo>::Iterator it;
				auto Expected = [&it]() {++it; }; //increment an unassigned iterator
				Assert::ExpectException<std::runtime_error>(Expected);
			}
			
			//Testing an assigned iterator
			{
				const Foo foo1(std::int32_t(1));
				const Foo foo2(std::int32_t(2));
				const Foo foo3(std::int32_t(3));

				SList<Foo> list1{ foo1, foo2, foo3 };

				SList<Foo>::Iterator It = list1.begin();

				Assert::AreEqual(foo1, *It);
				++It;
				Assert::AreEqual(foo2, *It);
				++It;
				Assert::AreEqual(foo3, *It);

			}

			//Testing post increment logic
			{
				{
					const Foo foo1(std::int32_t(1));
					const Foo foo2(std::int32_t(2));
					const Foo foo3(std::int32_t(3));

					SList<Foo> list1{ foo1, foo2, foo3 };

					SList<Foo>::Iterator It = list1.begin();
					SList<Foo>::Iterator It1 = list1.begin();

					It1 = ++It;

					Assert::AreEqual(foo2, *It1);
				}
			}

		}
		TEST_METHOD(IteratorOODereference)
		{
			//Testing a unassigned iterator
			{
				SList<Foo>::Iterator it;
				auto Expected = [&it]() {*it; }; //increment an unassigned iterator
				Assert::ExpectException<std::runtime_error>(Expected);
			}

			//Testing an empty list
			{
				SList<Foo> list;
				SList<Foo>::Iterator It = list.begin();
				auto Expected = [&It]() {*It; }; //increment an unassigned iterator
				Assert::ExpectException<std::runtime_error>(Expected);
			}


			//Testing an assigned iterator
			{
				const Foo foo1(std::int32_t(1));
				const Foo foo2(std::int32_t(2));
				const Foo foo3(std::int32_t(3));

				SList<Foo> list1{ foo1, foo2, foo3 };

				SList<Foo>::Iterator It = list1.begin();

				Assert::AreEqual(foo1, *It);
				++It;
				Assert::AreEqual(foo2, *It);
				++It;
				Assert::AreEqual(foo3, *It);

			}
			
		}
		TEST_METHOD(IteratorPrivateConstructor)
		{
			const Foo foo1(std::int32_t(1));
			const Foo foo2(std::int32_t(2));
			const Foo foo3(std::int32_t(3));


			SList<Foo> list1{ foo1, foo2, foo3 };

			SList<Foo>::Iterator It = list1.begin();
			Assert::AreEqual(foo1, *It);
		}
#pragma endregion

#pragma region SLIST CONST-ITERATOR FUNCTION TESTS
		TEST_METHOD(ConstIteratorCopyConstructor)
		{
			//const list test. Testing ConstIterator constructing from ConstIterator
			{
				const Foo foo1(std::int32_t(1));
				const Foo foo2(std::int32_t(2));
				const Foo foo3(std::int32_t(2));

				const SList<Foo> list1{ foo1, foo2, foo3 };

				SList<Foo>::ConstIterator It1 = list1.begin();

				SList<Foo>::ConstIterator It2(It1);

				Assert::AreEqual(It1, It2);
			}
			
			//non const list test. Testing ConstIterator constructing from ConstIterator
			{
				const Foo foo1(std::int32_t(1));
				const Foo foo2(std::int32_t(2));
				const Foo foo3(std::int32_t(2));

				SList<Foo> list1{ foo1, foo2, foo3 };

				SList<Foo>::ConstIterator It1 = list1.cbegin();

				SList<Foo>::ConstIterator It2(It1);

				Assert::AreEqual(It1, It2);
			}
			
			//type-casting Iterator to ConstIterator
			{
				const Foo foo1(std::int32_t(1));
				const Foo foo2(std::int32_t(2));
				const Foo foo3(std::int32_t(2));

				SList<Foo> list1{ foo1, foo2, foo3 };

				SList<Foo>::Iterator It1 = list1.begin();


				SList<Foo>::ConstIterator It2(It1); //REVIEW


			
				//Assert::AreEqual(It1, It2);
			}
		}
		TEST_METHOD(ConstIteratorOOEqual)
		{
			//const list
			{
				const Foo foo1(std::int32_t(1));
				const Foo foo2(std::int32_t(2));
				const Foo foo3(std::int32_t(3));

				const SList<Foo> list{ foo1, foo2, foo3 };

				SList<Foo>::ConstIterator It = list.begin();
				SList<Foo>::ConstIterator It2 = list.begin();

				Assert::AreEqual(It, It2);
			}

			//non const list
			{
				const Foo foo1(std::int32_t(1));
				const Foo foo2(std::int32_t(2));
				const Foo foo3(std::int32_t(3));

				SList<Foo> list{ foo1, foo2, foo3 };

				SList<Foo>::ConstIterator It = list.cbegin();
				SList<Foo>::ConstIterator It2 = list.cbegin();

				Assert::AreEqual(It, It2);
			}

		}
		TEST_METHOD(ConstIteratorOONotEqual)
		{
			//const list
			{
				const Foo foo1(std::int32_t(1));
				const Foo foo2(std::int32_t(2));
				const Foo foo3(std::int32_t(3));

				const SList<Foo> list1{ foo1, foo2, foo3 };
				const SList<Foo> list2{ foo1, foo2, foo3 };

				SList<Foo>::ConstIterator It = list1.begin();
				SList<Foo>::ConstIterator It2 = list2.begin();

				Assert::AreNotEqual(It, It2);
			}

			//non const list
			{
				const Foo foo1(std::int32_t(1));
				const Foo foo2(std::int32_t(2));
				const Foo foo3(std::int32_t(3));

				SList<Foo> list1{ foo1, foo2, foo3 };
				SList<Foo> list2{ foo1, foo2, foo3 };

				SList<Foo>::ConstIterator It = list1.cbegin();
				SList<Foo>::ConstIterator It2 = list2.cbegin();

				Assert::AreNotEqual(It, It2);
			}
		}
		TEST_METHOD(ConstIteratorOOPostIncrement)
		{
			//testing a unassigned ConstIterator
			{
				SList<Foo>::ConstIterator It;
				auto Expected = [&It]() {It++; };
				Assert::ExpectException<std::runtime_error>(Expected);
			}

			//const list testing an assigned ConstIterator
			{
				const Foo foo1(std::int32_t(1));
				const Foo foo2(std::int32_t(2));
				const Foo foo3(std::int32_t(3));

				const SList<Foo> list1{ foo1, foo2, foo3 };

				SList<Foo>::ConstIterator It = list1.begin();

				Assert::AreEqual(foo1, *It);
				It++;
				Assert::AreEqual(foo2, *It);
				It++;
				Assert::AreEqual(foo3, *It);
			}

			//non const list testing an assigned ConstIterator
			{
				const Foo foo1(std::int32_t(1));
				const Foo foo2(std::int32_t(2));
				const Foo foo3(std::int32_t(3));

				SList<Foo> list1{ foo1, foo2, foo3 };

				SList<Foo>::ConstIterator It = list1.cbegin();

				Assert::AreEqual(foo1, *It);
				It++;
				Assert::AreEqual(foo2, *It);
				It++;
				Assert::AreEqual(foo3, *It);
			}

			//const list testing post increment logic
			{
				
				const Foo foo1(std::int32_t(1));
				const Foo foo2(std::int32_t(2));
				const Foo foo3(std::int32_t(3));

				const SList<Foo> list1{ foo1, foo2, foo3 };

				SList<Foo>::ConstIterator It = list1.begin();
				SList<Foo>::ConstIterator It1 = list1.begin();

				It1 = It++;

				Assert::AreEqual(foo1, *It1);	
			}

			//non const list testing post increment logic
			{
				
				const Foo foo1(std::int32_t(1));
				const Foo foo2(std::int32_t(2));
				const Foo foo3(std::int32_t(3));

				SList<Foo> list1{ foo1, foo2, foo3 };

				SList<Foo>::ConstIterator It = list1.cbegin();
				SList<Foo>::ConstIterator It1 = list1.cbegin();

				It1 = It++;

				Assert::AreEqual(foo1, *It1);
			}
		}
		TEST_METHOD(ConstIteratorOOPreIncrement)
		{

			//testing a unassigned iterator
			{
				SList<Foo>::Iterator It;
				auto Expected = [&It]() {++It; }; //increment an unassigned iterator
				Assert::ExpectException<std::runtime_error>(Expected);
			}

			//const list testing an assigned iterator
			{
				const Foo foo1(std::int32_t(1));
				const Foo foo2(std::int32_t(2));
				const Foo foo3(std::int32_t(3));

				const SList<Foo> list1{ foo1, foo2, foo3 };

				SList<Foo>::ConstIterator It = list1.begin();

				Assert::AreEqual(foo1, *It);
				++It;
				Assert::AreEqual(foo2, *It);
				++It;
				Assert::AreEqual(foo3, *It);

			}
			
			//non const list testing an assigned iterator
			{
				const Foo foo1(std::int32_t(1));
				const Foo foo2(std::int32_t(2));
				const Foo foo3(std::int32_t(3));

				SList<Foo> list1{ foo1, foo2, foo3 };

				SList<Foo>::ConstIterator It = list1.cbegin();

				Assert::AreEqual(foo1, *It);
				++It;
				Assert::AreEqual(foo2, *It);
				++It;
				Assert::AreEqual(foo3, *It);

			}

			//Testing post increment logic
			{
				{
					const Foo foo1(std::int32_t(1));
					const Foo foo2(std::int32_t(2));
					const Foo foo3(std::int32_t(3));

					SList<Foo> list1{ foo1, foo2, foo3 };

					SList<Foo>::ConstIterator It = list1.begin();
					SList<Foo>::ConstIterator It1 = list1.begin();

					It1 = ++It;

					Assert::AreEqual(foo2, *It1);
				}
			}

		}
		TEST_METHOD(ConstIteratorOODereference)
		{
			//testing a unassigned iterator
			{
				SList<Foo>::ConstIterator It;
				auto Expected = [&It]() {*It; }; //increment an unassigned iterator
				Assert::ExpectException<std::runtime_error>(Expected);
			}

			//Testing an empty list
			{
				const SList<Foo> list;
				SList<Foo>::ConstIterator It = list.begin();
				auto Expected = [&It]() {*It; }; //increment an unassigned iterator
				Assert::ExpectException<std::runtime_error>(Expected);
			}

			//const list testing an assigned iterator
			{
				const Foo foo1(std::int32_t(1));
				const Foo foo2(std::int32_t(2));
				const Foo foo3(std::int32_t(3));

				const SList<Foo> list1{ foo1, foo2, foo3 };

				SList<Foo>::ConstIterator It = list1.begin();

				Assert::AreEqual(foo1, *It);
				++It;
				Assert::AreEqual(foo2, *It);
				++It;
				Assert::AreEqual(foo3, *It);

			}

			//non const list testing an assigned iterator
			{
				const Foo foo1(std::int32_t(1));
				const Foo foo2(std::int32_t(2));
				const Foo foo3(std::int32_t(3));

				SList<Foo> list1{ foo1, foo2, foo3 };

				SList<Foo>::ConstIterator It = list1.cbegin();

				Assert::AreEqual(foo1, *It);
				++It;
				Assert::AreEqual(foo2, *It);
				++It;
				Assert::AreEqual(foo3, *It);

			}

		}
		TEST_METHOD(ConstIteratorPrivateConstructor)
		{
			//const list
			{
				const Foo foo1(std::int32_t(1));
				const Foo foo2(std::int32_t(2));
				const Foo foo3(std::int32_t(3));


				const SList<Foo> list1{ foo1, foo2, foo3 };

				SList<Foo>::ConstIterator It = list1.begin();
				Assert::AreEqual(foo1, *It);

			}
			
			//non const list
			{
				const Foo foo1(std::int32_t(1));
				const Foo foo2(std::int32_t(2));
				const Foo foo3(std::int32_t(3));


				SList<Foo> list1{ foo1, foo2, foo3 };

				SList<Foo>::ConstIterator It = list1.cbegin();
				Assert::AreEqual(foo1, *It);

			}
		}
#pragma endregion
		static _CrtMemState _startMemState; //static class members are shared in every instance of the class
	};

	_CrtMemState SListTests::_startMemState; //There is needed to separate declaration from definition. Changes in C++17
}
