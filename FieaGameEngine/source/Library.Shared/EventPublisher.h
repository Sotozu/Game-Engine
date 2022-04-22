#pragma once
#include "RTTI.h"
#include "Vector.h"
#include "IEventSubscriber.h"
namespace FieaGameEngine
{
	class EventPublisher : public RTTI
	{
		RTTI_DECLARATIONS(EventPublisher, RTTI)
	public:

		EventPublisher() = delete;
		EventPublisher(EventPublisher&& rhs) = default;
		EventPublisher(const EventPublisher & rhs) = default;
		EventPublisher& operator=(EventPublisher && rhs) = default;
		EventPublisher& operator=(const EventPublisher & rhs) = default;
		virtual ~EventPublisher() override = default;

		/// <summary>
		/// Notify all subscribers of this event.
		/// </summary>
		void Deliver();
	protected:

		/// <summary>
		/// Constructor
		/// </summary>
		/// <param name="listofSubscribers">Derived Event will pass a refernce to its Vector of EventPublihserk</param>
		EventPublisher(Vector<std::reference_wrapper<IEventSubscriber>>& listOfSubscribers);

		/// <summary>
		/// Contains references to subscribers
		/// </summary>
		Vector<std::reference_wrapper<IEventSubscriber>>& _listOfSubscribers;

	};

}

