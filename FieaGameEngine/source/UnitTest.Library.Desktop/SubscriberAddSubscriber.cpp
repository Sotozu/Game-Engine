#include "pch.h"
#include "SubscriberAddSubscriber.h"

using namespace FieaGameEngine;

namespace UnitTestLibraryDesktop
{
	void SubscriberAddSubscriber::Notify(const EventPublisher& publisher)
	{
		publisher;
		//++_count;
		////const Event<Event<int>>* e = publisher.As<Event<Event<int>>>(); //

		//if (e != nullptr)
		//{
		//	//Event<int> message = e->Message();
		//	std::shared_ptr<FooSubscriber> newFooSubscriber = std::make_shared<FooSubscriber>(message);
		//	//message.Subscribe(*newFooSubscriber);
		//}
	}
}