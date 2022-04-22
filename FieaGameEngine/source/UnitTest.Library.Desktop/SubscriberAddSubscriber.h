#pragma once
#include "IEventSubscriber.h"
#include "EventPublisher.h"
#include "Foo.h"
#include "Event.h"
#include "GameStateSubscriber.h"
#include "FooSubscriber.h"

using namespace FieaGameEngine;

namespace UnitTestLibraryDesktop
{
	class SubscriberAddSubscriber final : public IEventSubscriber
	{
		std::size_t _count = 0;
		void Notify(const EventPublisher& publisher);
	};

}