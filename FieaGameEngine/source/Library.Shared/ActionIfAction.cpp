#include "pch.h"
#include "ActionIfAction.h"

namespace FieaGameEngine
{
	RTTI_DEFINITIONS(ActionIfAction);

	//Default constructor
	ActionIfAction::ActionIfAction() : Action(ActionIfAction::TypeIdClass()),
		_true{ nullptr },
		_false{ nullptr },
		_target{ nullptr }
	{}

	//ADDED 6/21/2022
	ActionIfAction::ActionIfAction(Game& game) : Action(game, ActionIfAction::TypeIdClass()),
		_true{ nullptr },
		_false{ nullptr },
		_target{ nullptr }
	{}


	//Protected constructor
	ActionIfAction::ActionIfAction(std::size_t typeId) : Action(typeId),
		_true{ nullptr },
		_false{ nullptr },
		_target{ nullptr }
	{}
	
	//ADDED 6/21/2022
	ActionIfAction::ActionIfAction(Game& game, std::size_t typeId) : Action(game, typeId),
		_true{ nullptr },
		_false{ nullptr },
		_target{ nullptr }
	{}

	ActionIfAction::~ActionIfAction()
	{
		Clear();
	}

	//Copy Constructor
	ActionIfAction::ActionIfAction(const ActionIfAction& rhs) :
		_true{ rhs._true != nullptr ? rhs._true->Clone() : nullptr },
		_false{ rhs._false != nullptr ? rhs._false->Clone() : nullptr},
		_target{ rhs._target != nullptr ? new Datum(*rhs._target) : nullptr},
		_targetKey{ rhs._targetKey }
	{}

	//Move Constructor
	ActionIfAction::ActionIfAction(ActionIfAction&& rhs) noexcept: 
		_true{ rhs._true != nullptr ? std::move(rhs._true) : nullptr}, 
		_false{ rhs._false != nullptr ? std::move(rhs._false) : nullptr },
		_target{ rhs._target != nullptr ? std::move(rhs._target) : nullptr },
		_targetKey{rhs._targetKey}
	{
		rhs._false = nullptr;
		rhs._true = nullptr;
		rhs._target = nullptr;
		_targetKey.clear();
	}
	
	//Copy Assignment
	ActionIfAction& ActionIfAction::operator=(const ActionIfAction& rhs)
	{
		if(this != &rhs) //Self Assignment Test
		{
			Clear();
			_true = rhs._true;
			_false = rhs._false;
			if (rhs._target != nullptr) { //external datum 
				_target = new Datum;
				_target = rhs._target;
			}
			_targetKey = rhs._targetKey;
		}

		return *this;
	}

	//Move Assignment
	ActionIfAction& ActionIfAction::operator=(ActionIfAction&& rhs) noexcept
	{
		if (this != &rhs) //Self Assignment Test
		{
			Clear();
			_true = std::move(rhs._true);
			_false = std::move(rhs._false);
			 _target = std::move(rhs._target);
			_targetKey = std::move(rhs._targetKey);
		}
		return *this;
	}

	bool ActionIfAction::CheckTrueFalseSettings()
	{
		if (_target == nullptr)
		{
			_target = new Datum;
			bool& myBool = SearchUp(_targetKey)->Get<bool>();
			_target->SetStorage(&myBool, 1);
		}
		if (_true == nullptr)
		{
			Datum* actionListTrue = Find("_true");
			if (actionListTrue != nullptr) { _true = actionListTrue->Get<Scope>().As<ActionList>(); }

		}
		if (_false == nullptr)
		{
			Datum* actionListFalse = Find("_false");
			if (actionListFalse != nullptr) { _false = actionListFalse->Get<Scope>().As<ActionList>(); }
			if (actionListFalse != nullptr) { _false = actionListFalse->Get<Scope>().As<ActionList>(); }
		}

		if (_true == nullptr || _false == nullptr || _target == nullptr) { return false; }
		else { return true; }
	}

	void ActionIfAction::Update(const GameState& gameState)
	{
		if (_isEnabled)
		{
			if (!CheckTrueFalseSettings()) { return; }

			if (_target->Get<bool>())
			{
				_true->Update(gameState);
			}
			else
			{
				_false->Update(gameState);
			}
		}
	}

	const Vector<Signature> ActionIfAction::Signatures()
	{
		return Vector<Signature>
		{
			{ "_targetKey", DatumTypes::String, 1, offsetof(ActionIfAction, _targetKey) }, //should be pointing to a bool
			{ "_true", DatumTypes::Table, 0, 0 },
			{ "_false", DatumTypes::Table, 0, 0 }

		};
	}

	void ActionIfAction::Init(const std::string& name, const bool& condition, ActionList& trueActions, ActionList& falseActions)
	{
		//if (Scope::GetParent() == nullptr) { throw std::runtime_error("ActionIfAction must have parent before setting conditions"); }
		Action::SetName(name);
		SetCondition(condition);
		SetTrueFalseActions(trueActions, falseActions);
	}

	void ActionIfAction::Init(const bool& condition, ActionList& trueActions, ActionList& falseActions)
	{
		//if (Scope::GetParent() == nullptr) { throw std::runtime_error("ActionIfAction must have parent before setting conditions"); }
		SetCondition(condition);
		SetTrueFalseActions(trueActions, falseActions);

	}

	void ActionIfAction::SetFalseActions(ActionList& falseActions)
	{
		//if (Scope::GetParent() == nullptr) { throw std::runtime_error("ActionIfAction must have parent before setting conditions"); }
		if (_false != nullptr) { delete _false; }
		Adopt("_false", falseActions);
		_false = &falseActions;

	}
	void ActionIfAction::SetTrueActions(ActionList& trueActions)
	{
		//if (Scope::GetParent() == nullptr) { throw std::runtime_error("ActionIfAction must have parent before setting conditions"); }
		if (_true != nullptr) { delete _true; }
		Adopt("_true", trueActions);
		_true = &trueActions;

	}

	void ActionIfAction::SetTrueFalseActions(ActionList& trueActions, ActionList& falseActions)
	{
		//if (Scope::GetParent() == nullptr) { throw std::runtime_error("ActionIfAction must have parent before setting conditions"); }
		if (_true != nullptr) {delete _true;}
		Adopt("_true", trueActions);
		_true = &trueActions;

		if (_false != nullptr){delete _false;}
		Adopt("_false", falseActions);
		_false = &falseActions;
	}

	void ActionIfAction::Clear()
	{
		_name = "";
		_targetKey = "";
		if (_target != nullptr) { delete _target; }
		if (_true != nullptr) { delete _true; }
		if (_false != nullptr) { delete _false; }
	}

	bool ActionIfAction::Equals(const RTTI* rhs) const
	{
		const ActionIfAction* action = rhs->As<ActionIfAction>();
		if (action == nullptr) { return false; }

		
		if (_name != action->_name || _targetKey != action->_targetKey) {return false;} //first check
		
		//TEST TRUE ACTION LIST
		if (_true != nullptr && action->_true != nullptr)
		{
			if (!_true->Equals(action->_true)) { return false; }
		}
		else if (_true == nullptr && action->_true == nullptr)
		{
			//Do nothing
		}
		else
		{
			return false;
		}

		//TEST FALSE ACTION LIST
		if (_false != nullptr && action->_false != nullptr)
		{
			_false->Equals(action->_false);
		}
		else if (_false == nullptr && action->_false == nullptr)
		{
			//DO nothing
		}
		else
		{
			return false;
		}

		//TEST _TARGET
		if (_target != nullptr && action->_target != nullptr)
		{
			if (*_target != *action->_target) { return false; }
		}
		else if (_target == nullptr && action->_target == nullptr)
		{
			//Do nothing
		}
		else
		{
			return false;
		}

		return true;

	}



}
