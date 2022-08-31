#include "pch.h"
#include "ActionCreateAction.h"

namespace FieaGameEngine
{
	RTTI_DEFINITIONS(ActionCreateAction);

	ActionCreateAction::ActionCreateAction() : Action(ActionCreateAction::TypeIdClass()),
		_actionToCreate{ "" },
		_targetKey{ "" }
	{}

	//ADDED 6/21/2022
	ActionCreateAction::ActionCreateAction(Game& game) : Action(game, ActionCreateAction::TypeIdClass()),
		_actionToCreate{ "" },
		_targetKey{ "" }
	{}

	ActionCreateAction::ActionCreateAction(std::size_t typeId) : Action(typeId),
		_actionToCreate{ "" },
		_targetKey{ "" }
	{}

	//ADDED 6/21/2022
	ActionCreateAction::ActionCreateAction(Game& game, std::size_t typeId) : Action(game, typeId),
		_actionToCreate{ "" },
		_targetKey{ "" }
	{}

	//Copy Constructor
	ActionCreateAction::ActionCreateAction(const ActionCreateAction& rhs) :
		_actionToCreate{ rhs._actionToCreate }, 
		_targetKey{rhs._targetKey}
	{}

	ActionCreateAction::ActionCreateAction(ActionCreateAction&& rhs) noexcept : 
		_actionToCreate{ rhs._actionToCreate },
		_targetKey{rhs._targetKey}
	{
		rhs._actionToCreate = "";
		rhs._targetKey = "";
	}

	ActionCreateAction& ActionCreateAction::operator=(const ActionCreateAction& rhs)
	{
		if (this != &rhs)
		{
			_actionToCreate = rhs._actionToCreate;
			_targetKey = rhs._targetKey;

		}
		return *this;
	}

	ActionCreateAction& ActionCreateAction::operator=(ActionCreateAction&& rhs) noexcept
	{
		if (this != &rhs) 
		{
			_actionToCreate = rhs._actionToCreate;
			_targetKey = rhs._targetKey;

			rhs._actionToCreate = "";
			rhs._targetKey = "";
		}

		return *this;
	}



	void ActionCreateAction::Update(const GameState& gameState)
	{
		UNREFERENCED_PARAMETER(gameState);
		if (_isEnabled)
		{
			if (!_actionToCreate.empty()) //If there is no action to create then simply skip
			{
				Action* action = Factory<Action>::Create(_actionToCreate);
				assert(action != nullptr);

				Scope* pScope;
				if (_targetKey.empty()) // DESIGN: When _targetKey is empty we simply default to setting it to the parent GameObject.
				{
					pScope = Scope::GetParent();
				}
				else
				{
					Datum* datum = Scope::SearchUp(_targetKey);
					pScope = &datum->Get<Scope>();
				}
				assert(pScope != nullptr);
				_actionsCreated.PushBack({ action , pScope });
			}
		}
	}

	const Vector<Signature> ActionCreateAction::Signatures()
	{
		return Vector<Signature>
		{
			{ "_actionToCreate", DatumTypes::String, 1, offsetof(ActionCreateAction, _actionToCreate) },
			{ "_targetKey", DatumTypes::String, 1, offsetof(ActionCreateAction, _targetKey) }
		};
	}

	bool ActionCreateAction::Equals(const RTTI* rhs) const
	{
		const ActionCreateAction* action = rhs->As<ActionCreateAction>();
		if (action == nullptr) { return false; }

		return _targetKey == action->_targetKey &&
			_actionToCreate == action->_actionToCreate &&
			_name == action->_name;
	}

}
