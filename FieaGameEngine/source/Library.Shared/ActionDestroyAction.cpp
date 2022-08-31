#include "pch.h"
#include "ActionDestroyAction.h"

namespace FieaGameEngine
{
	RTTI_DEFINITIONS(ActionDestroyAction);

	//Default Constructor
	ActionDestroyAction::ActionDestroyAction() : Action(ActionDestroyAction::TypeIdClass()),
		_targetActionName{ "" }
	{}

	//ADDED 6/21/2022
	ActionDestroyAction::ActionDestroyAction(Game& game) : Action(game, ActionDestroyAction::TypeIdClass()),
		_targetActionName{ "" }
	{}

	//Protected Constructor
	ActionDestroyAction::ActionDestroyAction(std::size_t typeId) : Action(typeId),
		_targetActionName{ "" }
	{}

	//ADDED 6/21/2022
	ActionDestroyAction::ActionDestroyAction(Game& game, std::size_t typeId) : Action(game, typeId),
		_targetActionName{ "" }
	{}

	//Copy Constructor
	ActionDestroyAction::ActionDestroyAction(const ActionDestroyAction& rhs) :
		_targetActionName{rhs._targetActionName}
	{}

	//Move Constructor
	ActionDestroyAction::ActionDestroyAction(ActionDestroyAction&& rhs) noexcept:
		_targetActionName{ rhs._targetActionName }
	{
		rhs._targetActionName = "";
	}
	
	//Copy Assignment
	ActionDestroyAction& ActionDestroyAction::operator=(const ActionDestroyAction& rhs)
	{
		if (this != &rhs){_targetActionName = rhs._targetActionName;}
		return *this;
	}

	//Move Assignment
	ActionDestroyAction& ActionDestroyAction::operator=(ActionDestroyAction&& rhs) noexcept
	{
		if (this != &rhs)
		{
			if (this != &rhs) { _targetActionName = rhs._targetActionName; }
			rhs._targetActionName = "";
		}
		return *this;
	}

	bool ActionDestroyAction::Equals(const RTTI* rhs) const
	{
		const ActionDestroyAction* action = rhs->As<ActionDestroyAction>();
		if (action == nullptr) { return false; }

		return _targetActionName == action->_targetActionName &&
			_name == action->_name;

	}

	void ActionDestroyAction::Update(const GameState& gameState)
	{
		UNREFERENCED_PARAMETER(gameState);

		if (_isEnabled)
		{

			if (!_targetActionName.empty()) //if the target is not set then don't update
			{
				Datum* datumActions = SearchUp("_actions");
				assert(datumActions != nullptr);
				for (std::size_t index = 0; index < datumActions->Size(); ++index)
				{
					Action* action = datumActions->Get<Scope>(index).As<Action>();
					assert(action != nullptr);
					if (_targetActionName == action->GetName())
					{
						_actionsToDestroy.PushBack({ index, datumActions });
						_targetActionName.clear(); //Once found and set for destruction we null out the target so that we don't continuously ask to delete once already delete.
						break;
					}
				}
			}
		}
	}


	const Vector<Signature> ActionDestroyAction::Signatures()
	{
		return Vector<Signature>
		{
			{ "_targetActionName", DatumTypes::String, 1, offsetof(ActionDestroyAction, _targetActionName) },
		};
	}
}
