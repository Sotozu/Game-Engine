#pragma once
#include "IEventSubscriber.h"
#include "EventPublisher.h"
#include "Foo.h"
#include "Event.h"
using namespace FieaGameEngine;

namespace UnitTestLibraryDesktop
{

	class FooSubscriber final : public IEventSubscriber
	{
	public:
		FooSubscriber() = default;
		~FooSubscriber() override;

		FooSubscriber(FooSubscriber&& rhs) noexcept = default;
		FooSubscriber(FooSubscriber& rhs) noexcept = default;

		FooSubscriber& operator=(FooSubscriber&& rhs) = default;
		FooSubscriber& operator=(FooSubscriber & rhs) = default;

		bool operator==(EventPublisher& rhs);
		void Notify(const EventPublisher& ePublisher) override; //Paul refers to this in 4/18/2022 @10:42
	private:
		int _integerDataEventReceiver{0};
		std::string _stringDataEventReceiver;
		Foo _fooDataEventReceiver;
	};
}
