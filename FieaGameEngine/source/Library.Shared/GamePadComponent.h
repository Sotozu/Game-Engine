#pragma once

#include "InputComponent.h"
#include <DirectXTK\GamePad.h>
#include <memory>

namespace FieaGameEngine
{
	enum class GamePadButtons
	{
		A,
		B,
		X,
		Y,
		LeftStick,
		RightStick,
		LeftShoulder,
		RightShoulder,
		Back,
		Start,
		DPadUp,
		DPadDown,
		DPadLeft,
		DPadRight
	};

	class GamePadComponent final : public InputComponent
	{
		RTTI_DECLARATIONS(GamePadComponent, InputComponent)

	public:
		static DirectX::GamePad* GamePad();
		
		GamePadComponent(Game& game, int player = 0);

		int Player() const;
		const DirectX::GamePad::State& CurrentState() const;
		const DirectX::GamePad::State& LastState() const;

		virtual void Initialize() override;
		virtual void Update(const GameState& gameState) override;

		bool IsButtonUp(GamePadButtons button) const;
		bool IsButtonDown(GamePadButtons button) const;
		bool WasButtonUp(GamePadButtons button) const;
		bool WasButtonDown(GamePadButtons button) const;
		bool WasButtonPressedThisFrame(GamePadButtons button) const;
		bool WasButtonReleasedThisFrame(GamePadButtons button) const;
		bool IsButtonHeldDown(GamePadButtons button) const;

	private:
		static std::unique_ptr<DirectX::GamePad> sGamePad;

		bool GetButtonState(const DirectX::GamePad::State& state, GamePadButtons button) const;

		int mPlayer;
		DirectX::GamePad::State mCurrentState;
		DirectX::GamePad::State mLastState;		
	};
}