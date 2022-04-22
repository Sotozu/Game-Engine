#include "Event.h"
namespace FieaGameEngine
{
	template<typename T>
	RTTI_DEFINITIONS(Event<T>); //We have a red squiggly sometimes...

	template<typename T>
	Event<T>::Event(T& message) : EventPublisher(_listOfSubscribers), 
		_message(message)
	{}

	template<typename T>
	Event<T>::~Event() 
	{
		_listOfSubscribers.Clear();
	}

	template<typename T>
	void Event<T>::Subscribe(std::reference_wrapper<IEventSubscriber> subscriber)
	{

		//Current issues is taht when the Vector calls find it tries to compare std::reference_wrapper objects.
		//The std::equal_to method has no compatabilty with this.

		//Possible solution with creating a unique equality functor for each subscriber but will need to confer with Paul about this.
		bool foundInstance{ false };
		for (std::size_t index = 0; index < _listOfSubscribers.Size(); ++index)
		{
			if (&_listOfSubscribers[index].get() == &subscriber.get()) { foundInstance = true; break; }//Same instance?
		}

		if (!foundInstance) { _listOfSubscribers.PushBack(subscriber); } //if no instance is found then add.

	}

	template<typename T>
	void Event<T>::UnSubscribe(std::reference_wrapper<IEventSubscriber> subscriber)
	{
		for (std::size_t index = 0; index < _listOfSubscribers.Size(); ++index)
		{
			if (&_listOfSubscribers[index].get() == &subscriber.get()) { _listOfSubscribers.RemoveAt(index); break; }//Same instance?
		}
	}

	template<typename T>
	void Event<T>::UnSubscribeAll()
	{
		_listOfSubscribers.Clear();
	}

	template<typename T>
	T& Event<T>::Message()
	{
		return _message; //so it will compile
	}

	template<typename T>

	std::size_t Event<T>::NumberOfSubcribers() { return _listOfSubscribers.Size(); }

	template<typename T>
	inline const T& Event<T>::Message() const
	{
		return _message;
	}

}