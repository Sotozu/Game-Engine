#include "ActionList.h"

namespace FieaGameEngine
{
	inline std::string ActionList::ToString() const { return "ActionList"; }
	inline gsl::owner<ActionList*> ActionList::Clone() const { return new ActionList{ *this }; }

}