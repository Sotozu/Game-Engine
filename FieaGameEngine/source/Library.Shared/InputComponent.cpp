#include "pch.h"
#include "InputComponent.h"

namespace FieaGameEngine
{
	RTTI_DEFINITIONS(InputComponent)

		InputComponent::InputComponent(Game& game) :
		mGame(&game)
	{
	}

	Game* InputComponent::GetGame()
	{
		return mGame;
	}

	void InputComponent::SetGame(Game& game)
	{
		mGame = gsl::not_null<Game*>(&game);
	}

	bool InputComponent::Enabled() const
	{
		return mEnabled;
	}

	void InputComponent::SetEnabled(bool enabled)
	{
		mEnabled = enabled;
	}

	void InputComponent::Initialize()
	{
	}

	void InputComponent::Shutdown()
	{
	}

	void InputComponent::Update(const GameState& gameState)
	{
		UNREFERENCED_PARAMETER(gameState);
	}
}