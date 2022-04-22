#include "pch.h"
#include "EventQueueSubscriber.h"

using namespace FieaGameEngine;

namespace UnitTestLibraryDesktop
{
	void EventQueueSubscriber::Notify(const EventPublisher& publisher)
	{
		++_count;
		const Event<GameStateSubscriber>* e = publisher.As<Event<GameStateSubscriber>>(); //

		if (e != nullptr)
		{
			GameStateSubscriber message = e->Message();
			std::shared_ptr<Event<GameStateSubscriber>> newEvent = std::make_shared<Event<GameStateSubscriber>>(message);

			message.GetGameState().GetEventQueue().Enqueue(*newEvent, message.GetGameState());
		}
	}
}