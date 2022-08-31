#include "pch.h"
#include "ActionIntegerIncrement.h"

namespace FieaGameEngine
{
	RTTI_DEFINITIONS(ActionIntegerIncrement);

	ActionIntegerIncrement::ActionIntegerIncrement() : Action(ActionIntegerIncrement::TypeIdClass()),
		_targetKey{ "" },
		_step{ 0 },
		_target{ nullptr }
	{}

	//ADDED 6/21/2022
	ActionIntegerIncrement::ActionIntegerIncrement(Game& game) : Action(game, ActionIntegerIncrement::TypeIdClass()),
		_targetKey{ "" },
		_step{ 0 },
		_target{ nullptr }
	{}

	ActionIntegerIncrement::ActionIntegerIncrement(std::size_t typeId) : Action(typeId),
		_targetKey{ "" },
		_step{ 0 },
		_target{ nullptr }
	{}

	//ADDED 6/21/2022
	ActionIntegerIncrement::ActionIntegerIncrement(Game& game, std::size_t typeId) : Action(game, typeId),
		_targetKey{ "" },
		_step{ 0 },
		_target{ nullptr }
	{}


	bool ActionIntegerIncrement::Equals(const RTTI* rhs) const
	{
		const ActionIntegerIncrement* action = rhs->As<ActionIntegerIncrement>();
		if (action == nullptr) { return false; }

		return _name == action->_name &&
			_step == action->_step &&
			*_target == *action->_target && //are both datums equivalent
			_targetKey == action->_targetKey;
	}


	void ActionIntegerIncrement::Update(const GameState& gameState)
	{
		UNREFERENCED_PARAMETER(gameState);
		if (_isEnabled)
		{

			if (_target == nullptr && !_targetKey.empty())
			{

				_target = SearchUp(_targetKey);
				assert(_target != nullptr);
				if (_target == nullptr) { return; }
				else if (_target->Type() != DatumTypes::Integer) { throw std::runtime_error("ActionIntegerIncrement can only reference integer datums."); }

			}

			_target->Get<int32_t>() += _step;
		}
	}


	const Vector<Signature> ActionIntegerIncrement::Signatures()
	{
		return Vector<Signature>
		{
			{ "_targetKey", DatumTypes::String, 1, offsetof(ActionIntegerIncrement, _targetKey) },
			{ "_step", DatumTypes::Integer, 1, offsetof(ActionIntegerIncrement, _step) },
		};
	}


}
