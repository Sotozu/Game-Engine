#include "pch.h"
#include "Action.h"
#include "GameObject.h"
#include "Game.h" //ADDED 6/23/2022 for singleton use

namespace FieaGameEngine
{
	RTTI_DEFINITIONS(Action);

	//Default Constructor
	Action::Action() : Attributed(Action::TypeIdClass()),
		_name(""), _isEnabled{ true }, _game{nullptr}
	{
	}

	Action::Action(Game& game) : Attributed(Action::TypeIdClass()),
		_name(""), _isEnabled{ true }, _game{&game}
	{}

	//Protected Constructor
	Action::Action(std::size_t typeId) : Attributed(typeId),
		_name(""), _isEnabled{ true }, _game{nullptr}
	{
	}

	Action::Action(Game& game, std::size_t typeId) : Attributed(typeId),
		_name(""), _isEnabled{ true }, _game{ &game }
	{}



	const Vector<Signature> Action::Signatures()
	{
		return Vector<Signature> 
		{
			{"_name", DatumTypes::String, 1, offsetof(Action, _name)}		
		};

	}

	bool Action::Equals(const RTTI* rhs) const
	{
		const Action* action = rhs->As<Action>();
		if (action == nullptr) { return false; }

		return _name == action->_name;
	}

	//ADDED 6/21/2022
	//Actions must be associated with a GameObject.
	//Find game it's parent game object and use it's _game member variable to populate this objects _game member variable.
	void Action::Initialize()
	{
		_game  = &Game::GetInstance();
	}

	Game* Action::GetGame()
	{
		return _game;
	}

}
