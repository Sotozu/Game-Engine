#include "ActionCreateAction.h"

namespace FieaGameEngine
{
	inline void ActionCreateAction::Init(const std::string& name, const std::string& actionToCreate, const std::string& targetKey)
	{
		Action::SetName(name);
		_actionToCreate = actionToCreate;
		_targetKey = targetKey;
	}

	inline void ActionCreateAction::Init(const std::string& actionToCreate, const std::string& targetKey)
	{
		_actionToCreate = actionToCreate;
		_targetKey = targetKey;
	}

	inline Vector<ActionCreateAction::PairType>& ActionCreateAction::GetActionsCreated() { return _actionsCreated; }
	inline std::string ActionCreateAction::ToString() const { return "ActionCreateAction"s; }

	inline void ActionCreateAction::SetTarget(const std::string& targetKey) { _targetKey = targetKey; }
	inline void ActionCreateAction::SetActionToCreate(const std::string& actionToCreate) { _actionToCreate = actionToCreate; }

	inline std::string& ActionCreateAction::GetTarget() { return _targetKey; }
	inline const std::string& ActionCreateAction::GetTarget() const { return _targetKey; }
	inline std::string& ActionCreateAction::GetActionToCreate() { return _actionToCreate; }
	inline const std::string& ActionCreateAction::GetActionToCreate() const { return _actionToCreate; }

	inline gsl::owner<ActionCreateAction*> ActionCreateAction::Clone() const { return new ActionCreateAction(*this); }

}