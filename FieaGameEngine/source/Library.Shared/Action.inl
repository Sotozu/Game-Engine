#include "Action.h"

namespace FieaGameEngine
{
	inline const std::string& Action::GetName() const { return _name; }
	inline std::string& Action::GetName() { return _name; }
	inline void Action::SetName(const std::string& name) { _name = name; }
	inline std::string Action::ToString() const { return "Action"; }
}