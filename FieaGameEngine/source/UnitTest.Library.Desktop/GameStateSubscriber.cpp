#include "pch.h"
#include "GameStateSubscriber.h"

using namespace FieaGameEngine;

namespace UnitTestLibraryDesktop
{
	GameStateSubscriber::GameStateSubscriber(GameState& gState) : _gState(gState)
	{}
	GameState& GameStateSubscriber::GetGameState() { return _gState; }

}

