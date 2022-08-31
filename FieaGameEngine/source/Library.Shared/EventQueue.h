#pragma once
#include<chrono>
#include "EventPublisher.h"
#include "GameTime.h"

using namespace std::chrono_literals;

namespace FieaGameEngine
{
	class GameState;

	/// <summary>
	/// Right before the RootGameObject update method is called call the EventQueue update method.
	/// </summary>
	class EventQueue final
	{
	public:

		/// <summary>
		/// Delivers events in the event queue
		/// Removes delivered events from the event queue
		/// </summary>
		/// <param name="gState"></param>
		void Update(GameState& gState); //Call this update method before the RootGameObject update method.

		/// <summary>
		/// Places an event into the event queue.
		/// </summary>
		/// <param name="ePublisher">event to place into the event queue</param>
		/// <param name="gState">A reference to the game state</param>
		/// <param name="delayTime">The time to delay the event from being delivered</param>
		void Enqueue(EventPublisher& ePublisher, GameState& gState, const std::chrono::milliseconds& delayTime = 0ms);

		/// <summary>
		/// Removes all events from the event queue
		/// </summary>
		inline void Clear();

		/// <summary>
		/// True if the event queue is empty
		/// False if the event queue false
		/// </summary>
		/// <returns>
		/// True if the event queue is empty
		/// False if the event queue false
		/// </returns>
		inline bool IsEmpty() const;

		/// <summary>
		/// Returns the number of elements within the event queue
		/// </summary>
		/// <returns>number of elements within the event queue</returns>
		inline std::size_t Size() const;
	private:

		/// <summary>
		/// Will be used as a container to place an event and the time it was inserted into the event queue.
		/// </summary>
		struct QueuedEvent
		{
		public:
			/// <summary>
			/// Constructor
			/// </summary>
			/// <param name="eP">Pointer to an event publisher</param>
			/// <param name="expiration">experiration time</param>
			QueuedEvent(EventPublisher* eP, std::chrono::high_resolution_clock::time_point& expiration);

			/// <summary>
			/// Pointer to an event publisher
			/// </summary>
			EventPublisher* _eP;

			/// <summary>
			/// Time which this event will expire
			/// </summary>
			std::chrono::high_resolution_clock::time_point _expiration;

			inline bool IsNotExpired(const GameTime& gTime) const;
		};

		/// <summary>
		/// Flag used to communicate to events that attempt to edit the eventQueue during an update.
		/// </summary>
		bool _isUpdating{ false };

		/// <summary>
		/// To be set when the Clear method is called in the middle of an update.
		/// </summary>
		bool _pendClear{ false };

		/// <summary>
		/// Main event queue that stores events to be fired in the next update
		/// </summary>
		Vector<QueuedEvent> _eventQueue;

		/// <summary>
		/// Stores events that are generated during the update process to be appended once the main event queue finishes.
		/// </summary>
		Vector<QueuedEvent> _pendingEventQueue;
		
		/// <summary>
		/// Helper function
		/// Partitions the events based on if they are expired.
		/// Deliveries all expired events and then removes them them from the queue.
		/// </summary>
		void DeliverAndRemoveExpiredEvents(GameState& gState);
		
		/// <summary>
		/// Helper function
		/// Adds the pending events from the pending event queue into the main event queue
		/// </summary>
		void AddPendingEventsToEventQueue();		
	};

}

#include "EventQueue.inl"

