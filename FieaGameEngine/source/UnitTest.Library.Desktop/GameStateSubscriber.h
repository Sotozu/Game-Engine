#pragma once
#include "GameState.h"
using namespace FieaGameEngine;

namespace UnitTestLibraryDesktop
{
	
	class GameStateSubscriber
	{
	public:
		GameStateSubscriber(GameState& gState);
		GameState& GetGameState();
	private:
		GameState& _gState;
	};

}

