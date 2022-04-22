#include "pch.h"
#include "ActionCreateAction.h"

namespace FieaGameEngine
{
	RTTI_DEFINITIONS(ActionCreateAction);

	ActionCreateAction::ActionCreateAction() : Action(ActionCreateAction::TypeIdClass()),
		_actionToCreate{ "" },
		_targetKey{""}
	{}

	ActionCreateAction::ActionCreateAction(std::size_t typeId) : Action(typeId),
		_actionToCreate{ "" },
		_targetKey{ "" }
	{}

	std::string ActionCreateAction::ToString() const { return "ActionCreateAction"s; }

	void ActionCreateAction::Update(const GameTime&)
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
		assert(pScope->As<GameObject>() != nullptr || pScope->As<ActionList>() != nullptr);//Test to see if the parentScope belongs to a GameObject or an ActionList
		_actionsCreated.PushBack({ *action , *pScope });

	}

	void ActionCreateAction::SetTarget(const std::string& targetKey){ _targetKey = targetKey; }

	const Vector<Signature> ActionCreateAction::Signatures()
	{
		return Vector<Signature>
		{
			{ "_actionToCreate", DatumTypes::String, 1, offsetof(ActionCreateAction, _actionToCreate) },
			{ "_targetKey", DatumTypes::String, 1, offsetof(ActionCreateAction, _targetKey) }
		};
	}

	void ActionCreateAction::Init(const std::string& name, const std::string& actionToCreate, const std::string& targetKey)
	{
		_name = name;
		_actionToCreate = actionToCreate;
		_targetKey = targetKey;
	}

	void ActionCreateAction::Init(const std::string& actionToCreate, const std::string& targetKey)
	{
		_actionToCreate = actionToCreate;
		_targetKey = targetKey;
	}

	Vector<ActionCreateAction::PairType>& ActionCreateAction::GetActionsCreated() { return _actionsCreated; }

}
