
#include "pch.h"
#include "ToStringSpecializations.h"
#include "Event.h"
#include "GameClock.h"
#include "EventPublisher.h"
#include "IEventSubscriber.h"
#include "EventQueue.h"
#include "Foo.h"
#include "FooSubscriber.h"
#include "GameState.h"
#include "EventQueueSubscriber.h"
#include "EventClearQueSubscriber.h"
#include "GameStateSubscriber.h"

using namespace std::chrono_literals;

using namespace FieaGameEngine;
using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTestLibraryDesktop
{

	TEST_CLASS(EventTest)
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
		TEST_METHOD(EventConstructors)
		{		
			Foo foo;
			Event<Foo> e(foo);
		}
		TEST_METHOD(SubscriberConstructor)
		{
			FooSubscriber fooSub;
		}
		TEST_METHOD(EventSubscribe)
		{
			Foo foo;
			Event<Foo> e(foo);

			//std::shared_ptr<IEventSubscriber> fooSub(new FooSubscriber);

			FooSubscriber fooSub1;

			Assert::AreEqual(0_z, Event<Foo>::NumberOfSubcribers());
			Event<Foo>::Subscribe(fooSub1); //Adding a new subscriber instance
			Assert::AreEqual(1_z, Event<Foo>::NumberOfSubcribers());

			unique_ptr<FooSubscriber> fooSub2(new FooSubscriber()); //Test with unique ptr

			Event<Foo>::Subscribe(*fooSub2); //Adding a new subscriber instance
			Assert::AreEqual(2_z, Event<Foo>::NumberOfSubcribers());

			Event<Foo>::Subscribe(fooSub1); //Attempting to a subscriber instance that already exists within the subscriber list
			Assert::AreEqual(2_z, Event<Foo>::NumberOfSubcribers());

		}
		TEST_METHOD(EventUnSubscribe)
		{
			Foo foo;
			Event<Foo> e(foo);

			//std::shared_ptr<IEventSubscriber> fooSub(new FooSubscriber);

			FooSubscriber fooSub1;
			FooSubscriber fooSub2;
			FooSubscriber fooSub3;

			Assert::AreEqual(0_z, Event<Foo>::NumberOfSubcribers());
			Event<Foo>::Subscribe(fooSub1); //Adding a new subscriber instance
			Event<Foo>::Subscribe(fooSub2); //Adding a new subscriber instance
			Event<Foo>::Subscribe(fooSub3); //Adding a new subscriber instance

			Assert::AreEqual(3_z, Event<Foo>::NumberOfSubcribers());

			Event<Foo>::UnSubscribe(fooSub1); //Removing new subscriber instance
			Assert::AreEqual(2_z, Event<Foo>::NumberOfSubcribers());

			Event<Foo>::UnSubscribe(fooSub2); //Removing new subscriber instance
			Assert::AreEqual(1_z, Event<Foo>::NumberOfSubcribers());

			Event<Foo>::UnSubscribe(fooSub3); //Removing new subscriber instance
			Assert::AreEqual(0_z, Event<Foo>::NumberOfSubcribers());

		}
		TEST_METHOD(EventUnSubscribeAll)
		{
			GameState gState;
			Foo foo;

			FooSubscriber sFoo0;
			FooSubscriber sFoo1;
			FooSubscriber sFoo2;

			Event<Foo> e(foo);

			Event<Foo>::Subscribe(sFoo0);
			Event<Foo>::Subscribe(sFoo1);
			Event<Foo>::Subscribe(sFoo2);

			Assert::AreEqual(3_z, Event<Foo>::NumberOfSubcribers());
			Event<Foo>::UnSubscribeAll();
			Assert::AreEqual(0_z, Event<Foo>::NumberOfSubcribers());

		}
		TEST_METHOD(EventMessage)
		{
			GameState gState;

			Foo foo;

			Event<Foo> e(foo);
			Assert::AreEqual(foo, e.Message());

			++e.Message().Data();
			Assert::AreEqual(1, e.Message().Data());

		}
		TEST_METHOD(EventQueEnqueue)
		{
			GameState gState;
			Foo foo;
			FooSubscriber sFoo;
			Event<Foo> e(foo);

			Event<Foo>::Subscribe(sFoo);

			EventQueue& eQueue = gState.GetEventQueue();
			
			Assert::AreEqual(0_z, eQueue.Size());
			eQueue.Enqueue(e, gState);
			Assert::AreEqual(1_z, eQueue.Size());

		}
		TEST_METHOD(EventQueClear)
		{
			GameState gState;
			Foo foo;
			FooSubscriber sFoo;

			Event<Foo> e0(foo);
			Event<Foo> e1(foo);
			Event<Foo> e2(foo);

			Event<Foo>::Subscribe(sFoo);

			EventQueue& eQueue = gState.GetEventQueue();

			Assert::AreEqual(0_z, eQueue.Size());

			eQueue.Enqueue(e0, gState);
			eQueue.Enqueue(e1, gState);
			eQueue.Enqueue(e2, gState);

			Assert::AreEqual(3_z, eQueue.Size());

			eQueue.Clear();

			Assert::AreEqual(0_z, eQueue.Size());

		}
		TEST_METHOD(EventQueUpdate)
		{

			//Immediate Update
			{
				GameState gState;
				gState.GetGameTime().SetCurrentTime(std::chrono::high_resolution_clock::time_point(0s));

				Foo foo;

				Event<Foo> e0(foo); //Our event now contains a message
				Event<Foo> e1(foo); //Our event now contains a message
				Event<Foo> e2(foo); //Our event now contains a message
				Event<Foo> e3(foo); //Our event now contains a message
				Event<Foo> e4(foo); //Our event now contains a message
				Event<Foo> e5(foo); //Our event now contains a message

				FooSubscriber sFoo; //This subscriber is interested in receiving messages from Event<Foo>

				Event<Foo>::Subscribe(sFoo); //Subscribe him to this Event<Foo>

				EventQueue& eQueue = gState.GetEventQueue();

				eQueue.Enqueue(e0, gState, 2s);
				eQueue.Enqueue(e1, gState, 2s);
				eQueue.Enqueue(e2, gState, 2s);

				eQueue.Enqueue(e3, gState, 4s);
				eQueue.Enqueue(e4, gState, 4s);
				eQueue.Enqueue(e5, gState, 4s);

				Assert::AreEqual(6_z, eQueue.Size());

				eQueue.Update(gState);
				Assert::AreEqual(6_z, eQueue.Size());


				gState.GetGameTime().SetCurrentTime(std::chrono::high_resolution_clock::time_point(1s));
				eQueue.Update(gState);
				Assert::AreEqual(6_z, eQueue.Size());

				gState.GetGameTime().SetCurrentTime(std::chrono::high_resolution_clock::time_point(2s));
				eQueue.Update(gState);
				Assert::AreEqual(6_z, eQueue.Size());

				gState.GetGameTime().SetCurrentTime(std::chrono::high_resolution_clock::time_point(3s));
				eQueue.Update(gState);
				Assert::AreEqual(3_z, eQueue.Size());

				gState.GetGameTime().SetCurrentTime(std::chrono::high_resolution_clock::time_point(4s));
				eQueue.Update(gState);
				Assert::AreEqual(3_z, eQueue.Size());

				gState.GetGameTime().SetCurrentTime(std::chrono::high_resolution_clock::time_point(5s));
				eQueue.Update(gState);
				Assert::AreEqual(0_z, eQueue.Size());

			}

			//Postponed Update

		}
		TEST_METHOD(SubscriberAddsEventWhenNotified)
		{
			GameState gState;
			EventClearQueSubscriber eQSub; //When notified it will clear the queue

			Foo foo;

			Event<Foo> e0(foo); //Our event now contains a message
			Event<Foo> e1(foo); //Our event now contains a message
			Event<Foo> e2(foo); //Our event now contains a message
			Event<Foo> e3(foo); //Our event now contains a message
			Event<Foo> e4(foo); //Our event now contains a message
			Event<Foo> e5(foo); //Our event now contains a message

			Event<GameStateSubscriber>::Subscribe(eQSub);
			GameStateSubscriber eEnqFoo(gState);
			Event<GameStateSubscriber> e(eEnqFoo);
			
			gState.GetGameTime().SetCurrentTime(std::chrono::high_resolution_clock::time_point(0s));

			gState.GetEventQueue().Enqueue(e, gState, 3s); //Once 3 seconds or more pass this event will expire and all events will be removed from the queue.

			gState.GetEventQueue().Enqueue(e0, gState, 1s); //Will be sent
			gState.GetEventQueue().Enqueue(e1, gState, 2s); //Will be sent
			gState.GetEventQueue().Enqueue(e2, gState, 3s); //Will be sent

			gState.GetEventQueue().Enqueue(e3, gState, 4s); //Wont be sent
			gState.GetEventQueue().Enqueue(e4, gState, 5s); //Wont be sent
			gState.GetEventQueue().Enqueue(e5, gState, 6s); //Wont be sent


			gState.GetGameTime().SetCurrentTime(std::chrono::high_resolution_clock::time_point(0s));
			gState.GetEventQueue().Update(gState); 
			Assert::AreEqual(7_z, gState.GetEventQueue().Size());

			gState.GetGameTime().SetCurrentTime(std::chrono::high_resolution_clock::time_point(1s));
			gState.GetEventQueue().Update(gState);
			Assert::AreEqual(7_z, gState.GetEventQueue().Size());

			gState.GetGameTime().SetCurrentTime(std::chrono::high_resolution_clock::time_point(2s));
			gState.GetEventQueue().Update(gState);
			Assert::AreEqual(6_z, gState.GetEventQueue().Size());

			gState.GetGameTime().SetCurrentTime(std::chrono::high_resolution_clock::time_point(3s));
			gState.GetEventQueue().Update(gState);
			Assert::AreEqual(5_z, gState.GetEventQueue().Size());

			gState.GetGameTime().SetCurrentTime(std::chrono::high_resolution_clock::time_point(4s));
			gState.GetEventQueue().Update(gState);
			Assert::AreEqual(0_z, gState.GetEventQueue().Size());


		}
		TEST_METHOD(SubscriberClearsEventsWhenNotified)
		{
			GameState gState;
			EventQueueSubscriber eQSub;

			Event<GameStateSubscriber>::Subscribe(eQSub);
			GameStateSubscriber eEnqFoo(gState);
			Event<GameStateSubscriber> e(eEnqFoo);

			gState.GetGameTime().SetCurrentTime(std::chrono::high_resolution_clock::time_point(0s));

			gState.GetEventQueue().Enqueue(e, gState);
			gState.GetGameTime().SetCurrentTime(std::chrono::high_resolution_clock::time_point(1s));


			Assert::AreEqual(1_z, gState.GetEventQueue().Size()); //The original one in the queue is here
			gState.GetEventQueue().Update(gState); //Original one has been removed. New one placed in.
			Assert::AreEqual(1_z, gState.GetEventQueue().Size()); //The new one is in here now. So size has gone from 1 to 0 to 1.

		}

		private:
			static _CrtMemState _startMemState; //static class members are shared in every instance of the class
	};
		
	_CrtMemState EventTest::_startMemState; //There is needed to separate declaration from definition. Changes in C++17
}