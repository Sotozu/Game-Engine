#include "ActionIntegerIncrement.h"

namespace FieaGameEngine
{
	inline const Datum* const ActionIntegerIncrement::GetTarget() const { return _target; }
	inline Datum* ActionIntegerIncrement::GetTarget() { return _target; }
	inline std::string ActionIntegerIncrement::ToString() const { return "ActionIntegerIncrement"s; }
	inline gsl::owner<ActionIntegerIncrement*> ActionIntegerIncrement::Clone() const { return new ActionIntegerIncrement(*this); }


	inline void ActionIntegerIncrement::Init(const std::string& name, int32_t step, const std::string& targetKey)
	{
		_name = name;
		_step = step;
		_targetKey = targetKey;
	}

	inline void ActionIntegerIncrement::Init(int32_t step, const std::string& targetKey)
	{
		_step = step;
		_targetKey = targetKey;
	}

}