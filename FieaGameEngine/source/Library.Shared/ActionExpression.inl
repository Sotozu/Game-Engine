#include "ActionExpression.h"

namespace FieaGameEngine
{
	inline std::string ActionExpression::ToString() const { return "ActionExpression"s; }
	inline gsl::owner<ActionExpression*> ActionExpression::Clone() const { return new ActionExpression(*this); }

}