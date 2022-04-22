#pragma once
#include "EventPublisher.h"
namespace FieaGameEngine
{
	/// <summary>
	/// Template argument is the concrete message (payload) class.
	/// </summary>
	/// <typeparam name="T">concrete message (payload)</typeparam>
	template<typename T>
	class Event : public EventPublisher
	{
		RTTI_DECLARATIONS(Event<T>, EventPublisher)
	public:
		
		Event(T& message);
		virtual ~Event() override;
		/// <summary>
		/// Given the address of an EventSubscriber, add it to the list of subscribers for this event type.
		/// </summary>
		/// <param name="subscriber">The subscriber that will recieve notifications from this event</param>
		inline static void Subscribe(std::reference_wrapper<IEventSubscriber> subscriber);

		/// <summary>
		/// Given the address of an EventSubscriber, add it to the list of subscribers for this event type.
		/// </summary>
		/// <param name="subscriber">The subscriber that will NO LONGER recieve notifications from this event</param>
		inline static void UnSubscribe(std::reference_wrapper<IEventSubscriber> subscriber);

		/// <summary>
		/// Unsubscribe all subscribers to this event type.
		/// </summary>
		inline static void UnSubscribeAll();

		inline static std::size_t NumberOfSubcribers();

		T& Message();
		
		const T& Message() const;
	protected:
		inline static Vector<std::reference_wrapper<IEventSubscriber>> _listOfSubscribers; //use of smart pointers
	private:
		T _message;
	};

}

#include "Event.inl"
