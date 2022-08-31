#include "pch.h"
#include "EventClearQueSubscriber.h"

using namespace FieaGameEngine;

namespace UnitTestLibraryDesktop
{
	void EventClearQueSubscriber::Notify(const EventPublisher& publisher)
	{
		++_count;
		const Event<GameStateSubscriber>* e = publisher.As<Event<GameStateSubscriber>>(); //

		if (e != nullptr)
		{
			GameStateSubscriber message = e->Message();

			message.GetGameState().GetEventQueue().Clear();
		}
	}
}