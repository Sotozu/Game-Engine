#pragma once
#include "ActionCreateAction.h"
#include "ActionDestroyAction.h"
#include "GameClock.h"
#include "GameTime.h"
#include "EventQueue.h"

namespace FieaGameEngine
{
	/// <summary>
	/// A singleton that will be able to perform methods on the game for cases where a global action is required. 
	/// </summary>
	class GameState final
	{
		friend GameClock;
		friend GameTime;
	public:
		/// <summary>
		/// After all updates are called in the game this function will be called to finalize any pending updates.
		/// Pending updates are by design meant to be "finalized" after all updates have been called in game.
		/// </summary>
		void FinalizeUpdate();

		void Update();

		GameTime& GetGameTime();
		GameClock& GetGameClock();
		EventQueue& GetEventQueue();

		GameClock _gClock;
		GameTime _gTime;
		EventQueue _eQueue;

	private:

		/// <summary>
		/// Accesses the static container that stores actions created within a recently executed update cycle.
		/// Links the created actions into their respective parents (GameObject/ActionList/etc).
		/// All linked actions will be callable in the next update cycle.
		/// </summary>
		void CreateActions();

		/// <summary>
		/// Accesses the static container that stores references and position to actions that will be deleted and stored for deletion in the recently executed update cycle.
		/// Access the references and positions of the actions to delete and deletes them.
		/// All deleted actions will not longer be callable in the next update cycle.
		/// </summary>
		void DeleteActions();

	};
}

#include "GameState.inl"
