#pragma once
namespace FieaGameEngine
{

	class EventPublisher;

	class IEventSubscriber
	{

	public:
		/// <summary>
		/// Pure virtual method that accepts the address of an EventPublisher (the actual argument will be the event itself). 
		/// Handlers will use the RTTI interface to verify the event type.
		/// </summary>
		/// <param name=""></param>
		virtual void Notify(const EventPublisher&) = 0; //Paul refers to this in 4/18/2022 @10:42
		
		IEventSubscriber() = default;
		virtual ~IEventSubscriber() = default;

		IEventSubscriber(IEventSubscriber&) = default;
		IEventSubscriber(IEventSubscriber&&) = default;

		IEventSubscriber& operator=(IEventSubscriber&) = default;
		IEventSubscriber& operator=(IEventSubscriber&&) = default;
	private:
	};

}

