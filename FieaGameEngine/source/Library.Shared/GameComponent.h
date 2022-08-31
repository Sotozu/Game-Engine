#pragma once

#include "RTTI.h"
#include <gsl\gsl>

namespace FieaGameEngine
{
	class Game;

	class GameState;

	class GameComponent : public RTTI
	{
		RTTI_DECLARATIONS(GameComponent, RTTI)

	public:
		GameComponent() = default;
		GameComponent(Game& game);
		GameComponent(const GameComponent&) = default;
		GameComponent& operator=(const GameComponent&) = default;
		GameComponent(GameComponent&&) = default;
		GameComponent& operator=(GameComponent&&) = default;
		virtual ~GameComponent() = default;

		Game* GetGame();
		void SetGame(Game& game);
		bool Enabled() const;
		void SetEnabled(bool enabled);

		virtual void Initialize();
		virtual void Shutdown();
		virtual void Update(const GameState& gameState);

	protected:
		gsl::not_null<Game*> mGame;
		bool mEnabled{ true };
	};
}