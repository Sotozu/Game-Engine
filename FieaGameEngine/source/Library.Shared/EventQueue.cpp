#include "pch.h"
#include "EventQueue.h"
#include "GameState.h"

namespace FieaGameEngine
{
#pragma region EventQueue
	void EventQueue::Update(GameState& gState) //Call this update method before the RootGameObject update method.
	{
		_isUpdating = true;
		DeliverAndRemoveExpiredEvents(gState);
		if (_pendClear) { _eventQueue.Clear(); }
		AddPendingEventsToEventQueue();
		_isUpdating = false;
	}

	void EventQueue::DeliverAndRemoveExpiredEvents(GameState& gState)
	{
		auto it = std::partition(_eventQueue.begin(), _eventQueue.end(), [&gState, this](QueuedEvent qEvent) {return  qEvent.IsNotExpired(gState.GetGameTime()); });
		auto cashedIt = it;
		for (it; it != _eventQueue.end(); ++it)
		{
			(*it)._eP->Deliver();
		}
		_eventQueue.Remove(cashedIt, _eventQueue.end());

	}
	void EventQueue::AddPendingEventsToEventQueue()
	{
		for (std::size_t index = 0; index < _pendingEventQueue.Size(); ++index)
		{
			_eventQueue.PushBack(_pendingEventQueue[index]);
		}
		_pendingEventQueue.Clear();
	}

	void EventQueue::Enqueue(EventPublisher& ePublisher, GameState& gState, const std::chrono::milliseconds& delayTime)
	{
		if (!_isUpdating) { _eventQueue.EmplaceBack(&ePublisher, gState.GetGameTime().CurrentTime() + delayTime); }
		else { _pendingEventQueue.EmplaceBack(&ePublisher, gState.GetGameTime().CurrentTime() + delayTime); }
	}
#pragma endregion

#pragma region QueuedEvent
	EventQueue::QueuedEvent::QueuedEvent(EventPublisher* eP, std::chrono::high_resolution_clock::time_point& expiration) :
		_eP(eP), 
		_expiration(expiration)
	{}


#pragma endregion
}