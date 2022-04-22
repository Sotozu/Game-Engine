#include "pch.h"
#include "Action.h"

namespace FieaGameEngine
{
	RTTI_DEFINITIONS(Action);

	Action::Action() : Attributed(Action::TypeIdClass()),
		_name("")
	{}
	Action::Action(std::size_t typeId) : Attributed(typeId),
		_name("")
	{}


	const Vector<Signature> Action::Signatures()
	{
		return Vector<Signature> 
		{
			{"_name", DatumTypes::String, 1, offsetof(Action, _name)}		
		};

	}

	bool Action::Equals(const RTTI* rhs) const
	{
		const Action* action = rhs->As<Action>();
		if (action == nullptr) { return false; }

		return _name == action->_name;
	}
}
