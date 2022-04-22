#pragma once
#include "EventQueue.h"
namespace FieaGameEngine
{
	

	inline void EventQueue::Clear() 
	{ 
		if (_isUpdating){_pendClear = true;}
		else{_eventQueue.Clear();}
	}

	inline bool EventQueue::IsEmpty() const { return _eventQueue.IsEmpty(); }
	inline std::size_t EventQueue::Size() const { return _eventQueue.Size(); }

	inline bool EventQueue::QueuedEvent::IsNotExpired(const GameTime& gTime) const { return gTime.CurrentTime() <= _expiration; }

}