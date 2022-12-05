#pragma once

#include <windows.h>
#include <winrt\Windows.Foundation.h>
#include <cstdint>
#include <string>
#include <sstream>
#include <memory>
#include <functional>
#include <array>

#include "GameState.h"
#include "GameClock.h"
#include "GameTime.h"
#include "ServiceContainer.h"
#include "RenderTarget.h"
#include "ContentManager.h"

#include "RendererDirectX.h"
#include "GameObject.h"
#include "GameState.h"
#include "MatrixHelper.h"

#include "KeyboardComponent.h"
#include "MouseComponent.h"
#include "GamePadComponent.h"

namespace FieaGameEngine
{
	//class KeyboardComponent;
	//class MouseComponent;
	//class GamePadComponent;
	class FpsComponent;
	class Grid;
	class GameComponent;
	class InputComponent;
}

namespace FieaGameEngine
{

	class Game final
	{
	public:


		static void InitializeInstance(GameObject* rootGameObject = nullptr,

			std::function<void* ()> getWindowCallback = [&]() -> void* {void* i{nullptr};  return i; },

			std::function<void(SIZE&)> getRenderTargetSizeCallback = [](SIZE& r) {r;},

			const std::string& name = "Game")
		{
			static Game _game(rootGameObject, getWindowCallback, getRenderTargetSizeCallback, name);
			game = &_game;
		}

		static Game& GetInstance()
		{
			return *game;
		}

		//Game(std::function<void* ()> getWindowCallback, std::function<void(SIZE&)> getRenderTargetSizeCallback, const std::string& name = "Game");
		Game(const Game&) = delete;
		Game& operator=(const Game&) = delete;
		Game(Game&&) = delete;
		Game& operator=(Game&&) = delete;
		~Game() = default;

#pragma region Added these for now because everything in this framework uses it lol.
		gsl::not_null<ID3D11Device5*> Direct3DDevice() const;
		gsl::not_null<ID3D11DeviceContext4*> Direct3DDeviceContext() const;
		gsl::not_null<IDXGISwapChain1*> SwapChain() const;
		gsl::not_null<ID3D11RenderTargetView*> RenderTargetView() const;
		gsl::not_null<ID3D11DepthStencilView*> DepthStencilView() const;
#pragma endregion
		void Run();
		void Shutdown();

		bool IsFullScreen() const;

		std::function<void* ()> GetWindowCallback() const;

		ContentManager& GetContentManager();
		const ServiceContainer& Services() const;

		const std::shared_ptr<FieaGameEngine::KeyboardComponent> GetKeyBoard() const;
		const std::shared_ptr<FieaGameEngine::MouseComponent> GetMouse() const;
		const std::shared_ptr<FieaGameEngine::GamePadComponent> GetGamePad() const;

		void Initialize();
	private:
		static Game* game;
		Game(GameObject* rootGameObject = nullptr, std::function<void* ()> getWindowCallback = [&]() -> void* {void* i{nullptr}; return i; }, std::function<void(SIZE&)> getRenderTargetSizeCallback = [](SIZE& r) {r; }, const std::string& name = "Game");

		void SetRootGameObject(GameObject& gameObject);
		void InitializeRootGameObject();
		const std::string _name;
		//Make these private
		void UpdateInput(const GameState& gameState);
		void UpdateStates(const GameState& gameState);
		void UpdateDraw(const GameState& gameState);

		inline static const std::uint32_t DefaultFrameRate{ 60 };
		inline static const std::uint32_t DefaultMultiSamplingCount{ 4 };
		inline static const std::uint32_t DefaultBufferCount{ 2 };


		std::function<void* ()> mGetWindow;
		std::function<void(SIZE&)> mGetRenderTargetSize;



		bool mIsFullScreen{ false };

		/// <summary>
		/// Contains: Keyboard, Mouse and Gamepad
		/// To qualify as a service following criteria must be considered.
		/// Scope access must be very broad.
		/// Strong requirement to be a singleton, or very few instances.
		/// </summary>
		ServiceContainer mServices;
		ContentManager mContentManager;

		GameState mGameState;
		RendererDirectX mRenderer;
		GameObject* mRootGameObject;
		
		//In this implementation the GameComponent is used to house the following:
		std::vector<std::shared_ptr<GameComponent>> mComponents;

		std::vector<std::shared_ptr<InputComponent>> mIComponents;

		std::shared_ptr<FieaGameEngine::KeyboardComponent> mKeyboard;
		std::shared_ptr<FieaGameEngine::MouseComponent> mMouse;
		std::shared_ptr<FieaGameEngine::GamePadComponent> mGamePad;

		std::shared_ptr<FieaGameEngine::FpsComponent> mFpsComponent;
		std::shared_ptr<FieaGameEngine::Grid> mGrid;

		DirectX::XMFLOAT4X4 mWorldMatrix{ FieaGameEngine::MatrixHelper::Identity };

		void Exit();

		////This can use my map
		//static UnorderedMap<std::string, Game*> games;
	};
}

#include "Game.inl"