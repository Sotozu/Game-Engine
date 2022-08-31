#pragma once

#include "RTTI.h"
#include <gsl\gsl>

namespace FieaGameEngine
{
	class Game;

	class GameState;

	class InputComponent : public RTTI
	{
		RTTI_DECLARATIONS(InputComponent, RTTI)

	public:
		InputComponent() = default;
		InputComponent(Game& game);
		InputComponent(const InputComponent&) = default;
		InputComponent& operator=(const InputComponent&) = default;
		InputComponent(InputComponent&&) = default;
		InputComponent& operator=(InputComponent&&) = default;
		virtual ~InputComponent() = default;

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

