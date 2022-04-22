#include "pch.h"
#include "EventPublisher.h"

namespace FieaGameEngine
{
	RTTI_DEFINITIONS(EventPublisher);

	EventPublisher::EventPublisher(Vector<std::reference_wrapper<IEventSubscriber>>& listOfSubscribers) :
		_listOfSubscribers{ listOfSubscribers }
	{}


	void EventPublisher::Deliver()
	{
		for (std::size_t index = 0; index < _listOfSubscribers.Size(); ++index)
		{
			_listOfSubscribers[index].get().Notify(*this);
		}
	}


}
