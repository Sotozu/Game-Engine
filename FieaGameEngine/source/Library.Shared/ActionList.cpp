#include "pch.h"
#include "ActionList.h"

namespace FieaGameEngine
{
	RTTI_DEFINITIONS(ActionList);


	ActionList::ActionList() : Action(ActionList::TypeIdClass()),
		_actionListDatum{ nullptr }

	{}

	//ADDED 6/21/2022
	ActionList::ActionList(Game& game) : Action(game, ActionList::TypeIdClass()),
		_actionListDatum{ nullptr }

	{}

	ActionList::ActionList(std::size_t typeId) : Action(typeId),
		_actionListDatum{nullptr}
	{}

	//ADDED 6/21/2022
	ActionList::ActionList(Game& game, std::size_t typeId) : Action(game, typeId),
		_actionListDatum{ nullptr }
	{}

	ActionList::~ActionList()
	{
		Clear();
	}


	void ActionList::Update(const GameState& gameState)
	{
		if (_isEnabled)
		{

			if (_actionListDatum == nullptr) { _actionListDatum = Find("_actions"); }
			assert(_actionListDatum != nullptr);

			for (std::size_t index = 0; index < _actionListDatum->Size(); ++index)
			{
				Action* _action = _actionListDatum->Get<Scope>(index).As<Action>(); //Downcast so that we get a game object.
				assert(_action != nullptr);
				_action->Update(gameState);
			}
		}
	}

	const Vector<Signature> ActionList::Signatures()
	{
		return Vector<Signature>
		{
			{ "_actions", DatumTypes::Table, 0, 0},
		};
	}


	bool ActionList::Equals(const RTTI* rhs) const
	{
		const ActionList* action = rhs->As<ActionList>();
		if (action == nullptr) { return false; }
		if (_actionListDatum->Size() != action->_actionListDatum->Size()) { return false; } // number of actions present in action lists is not equal

		for (std::size_t index = 0; index < _actionListDatum->Size(); ++index)
		{
			Action* _actionLHS = _actionListDatum->Get<Scope>(index).As<Action>();
			Action* _actionRHS = action->_actionListDatum->Get<Scope>(index).As<Action>();
			assert(_actionLHS != nullptr);
			assert(_actionRHS != nullptr);

			if (!_actionLHS->Equals(_actionRHS)) { return false; }
		}
		return true;
	}

	void ActionList::Clear()
	{
		if (_actionListDatum != nullptr)
		{
			for (std::size_t index = 0; index < _actionListDatum->Size(); ++index)
			{
				Scope* _action = &_actionListDatum->Get<Scope>(index); //Downcast so that we get a game object.
				assert(_action != nullptr);

				delete _action;			
			}
		}
	}


}