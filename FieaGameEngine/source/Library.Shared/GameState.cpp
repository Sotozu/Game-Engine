#include "pch.h"
#include "GameState.h"
namespace FieaGameEngine
{
	void GameState::FinalizeUpdate()
	{
		DeleteActions();
		CreateActions();
	}

	GameTime& GameState::GetGameTime() { return _gTime; }

	GameClock& GameState::GetGameClock() { return _gClock; }

	EventQueue& GameState::GetEventQueue() { return _eQueue; }

	void GameState::CreateActions()
	{
		Vector<ActionCreateAction::PairType>& listOfActions = ActionCreateAction::GetActionsCreated();
		for (std::size_t index = 0; index < listOfActions.Size(); ++index)
		{
			auto [action, pScope] = listOfActions[index]; //ParentScope can either be a game object or a ActionList.
			pScope->Adopt("_actions", *action);
		}
		listOfActions.Clear();
		listOfActions.Resize(0);

	}

	void GameState::DeleteActions()
	{
		Vector<ActionDestroyAction::PairType>& listOfActions = ActionDestroyAction::GetActionsToDestroy();
		for (std::size_t index = 0; index < listOfActions.Size(); ++index)
		{
			auto [datumIndex, actionDatum] = listOfActions[index]; //ParentScope can either be a game object or a ActionList.
			delete &actionDatum->Get<Scope>(datumIndex);
		}
		listOfActions.Clear();
		listOfActions.Resize(0);
	}


	void GameState::Update() { _gClock.UpdateGameTime(_gTime); }

}