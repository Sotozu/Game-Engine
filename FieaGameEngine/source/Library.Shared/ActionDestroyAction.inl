#include "ActionDestroyAction.h"

namespace FieaGameEngine
{
	inline void ActionDestroyAction::Init(const std::string& name, const std::string& targetActionName)
	{
		Action::SetName(name);
		_targetActionName = targetActionName;
	}
	inline void ActionDestroyAction::Init(const std::string& targetActionName) { _targetActionName = targetActionName; }
	inline Vector<ActionDestroyAction::PairType>& ActionDestroyAction::GetActionsToDestroy() { return _actionsToDestroy; }
	inline void ActionDestroyAction::SetTargetAction(const std::string& targetActionName) { _targetActionName = targetActionName; }
	inline std::string& ActionDestroyAction::GetTargetActionName() { return _targetActionName; }
	inline const std::string& ActionDestroyAction::GetTargetActionName() const { return _targetActionName; }
	inline std::string ActionDestroyAction::ToString() const { return "ActionDestroyAction"s; }
	inline gsl::owner<ActionDestroyAction*> ActionDestroyAction::Clone() const { return new ActionDestroyAction(*this); }

}