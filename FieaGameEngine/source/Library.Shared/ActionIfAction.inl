#include "ActionIfAction.h"

namespace FieaGameEngine
{
	void ActionIfAction::SetCondition(const bool& condition) { _target->SetStorage(reinterpret_cast<int32_t*>(condition), 1); }
	inline ActionList* ActionIfAction::GetTrueActionList() const { return _true; }
	inline ActionList* ActionIfAction::GetFalseActionList() const { return _false; }
	inline gsl::owner<ActionIfAction*> ActionIfAction::Clone() const { return new ActionIfAction(*this); }
	inline std::string ActionIfAction::ToString() const { return "ActionIfAction"s; }
}