#include "pch.h"
#include "Game.h"
#include "GameException.h"
#include "DrawableGameComponent.h"
#include "DirectXHelper.h"
#include "ContentTypeReaderManager.h"

#include "KeyboardComponent.h"
#include "MouseComponent.h"
#include "GamePadComponent.h"
#include "FpsComponent.h"
#include "Grid.h"

#include "FirstPersonCamera.h"
#include "ImGuiComponent.h"
#include "imgui_impl_dx11.h"
#include "UtilityWin32.h"

using namespace std;
using namespace gsl;
using namespace FieaGameEngine;
using namespace winrt;
using namespace DirectX;

namespace FieaGameEngine
{
	Game* Game::game = { nullptr };
	Game::Game(GameObject* rootGameObject, std::function<void* ()> getWindowCallback, std::function<void(SIZE&)> getRenderTargetSizeCallback, const std::string& name) :
		mRootGameObject(rootGameObject),
		mGetWindow(getWindowCallback),
		mGetRenderTargetSize(getRenderTargetSizeCallback),
		_name{name},
		mContentManager(*this),
		mRenderer(getWindowCallback, getRenderTargetSizeCallback)
	{
		assert(getWindowCallback != nullptr);
		assert(mGetRenderTargetSize != nullptr);

		mRenderer.CreateDeviceIndependentResources();
		mRenderer.CreateDeviceResources();
		mRenderer.UpdateRenderTargetSize();
		//Initialize();
	}

	//Game::Game(std::function<void* ()> getWindowCallback, std::function<void(SIZE&)> getRenderTargetSizeCallback, const std::string& name) :
	//	mRootGameObject{nullptr},
	//	mGetWindow(getWindowCallback), 
	//	mGetRenderTargetSize(getRenderTargetSizeCallback),
	//	_name{ name },
	//	mContentManager(*this),
	//	mRenderer(getWindowCallback, getRenderTargetSizeCallback)
	//{
	//	assert(getWindowCallback != nullptr);
	//	assert(mGetRenderTargetSize != nullptr);

	//	mRenderer.CreateDeviceIndependentResources();
	//	mRenderer.CreateDeviceResources();
	//	mRenderer.UpdateRenderTargetSize();
	//	Initialize();

	//}
	void Game::Initialize()
	{
		//Setup Input Components
		mKeyboard = make_shared<KeyboardComponent>(*this);
		mIComponents.push_back(mKeyboard);
		mServices.AddService(KeyboardComponent::TypeIdClass(), mKeyboard.get());

		mMouse = make_shared<MouseComponent>(*this, MouseModes::Absolute);
		mIComponents.push_back(mMouse);
		mServices.AddService(MouseComponent::TypeIdClass(), mMouse.get());

		mGamePad = make_shared<GamePadComponent>(*this);
		mIComponents.push_back(mGamePad);
		mServices.AddService(GamePadComponent::TypeIdClass(), mGamePad.get());


		//SetUp Scene Components
		auto camera = make_shared<FirstPersonCamera>(*this); //Convert this into a gameObject
		mComponents.push_back(camera);
		mServices.AddService(Camera::TypeIdClass(), camera.get());

		mGrid = make_shared<Grid>(*this, camera); //Convert this into a gameObject
		mComponents.push_back(mGrid);

		auto imGui = make_shared<ImGuiComponent>(*this); //Convert this into a gameObject
		mComponents.push_back(imGui);
		mServices.AddService(ImGuiComponent::TypeIdClass(), imGui.get());
		auto imGuiWndProcHandler = make_shared<UtilityWin32::WndProcHandler>(ImGui_ImplWin32_WndProcHandler);
		UtilityWin32::AddWndProcHandler(imGuiWndProcHandler);

		auto helpTextImGuiRenderBlock = make_shared<ImGuiComponent::RenderBlock>([this]()
			{
				ImGui::Begin("Controls");
				ImGui::SetNextWindowPos(ImVec2(10, 10));

				stringstream fpsLabel;
				fpsLabel << setprecision(4) << "Frame Rate: " << mFpsComponent->FrameRate() << "    Total Elapsed Time: " << mGameState._gTime.TotalGameTimeSeconds().count();
				ImGui::Text(fpsLabel.str().c_str());

				ImGui::Text("Camera (WASD + Left-Click-Mouse-Look)");

				stringstream gridVisibleLabel;
				gridVisibleLabel << "Toggle Grid (G): " << (mGrid->Visible() ? "Visible" : "Not Visible");
				ImGui::Text(gridVisibleLabel.str().c_str());
				ImGui::End();				
			});

		imGui->AddRenderBlock(helpTextImGuiRenderBlock);
		mFpsComponent = make_shared<FpsComponent>(*this);
		mFpsComponent->SetVisible(false);
		mComponents.push_back(mFpsComponent);


		ContentTypeReaderManager::Initialize(*this);
		mGameState.GetGameClock().Reset();

		//Initialize Game Components
		for (auto& component : mComponents)
		{
			component->Initialize();
		}


		InitializeRootGameObject();

		//Initialize Input Components
		for (auto& component : mIComponents)
		{
			component->Initialize();
		}

		camera->SetPosition(0.0f, 2.5f, 20.0f); // TO DO: Camera should be a GameObject as well. Need to update this later

	}

	void Game::Run()
	{
		mGameState.Update();

		UpdateInput(mGameState);
		UpdateStates(mGameState);
		UpdateDraw(mGameState);
	}

	void Game::Shutdown()
	{
		mGrid = nullptr;
		mFpsComponent = nullptr;

		for (auto& component : mComponents)
		{
			component->Shutdown();
		}

		mRenderer.Shutdown();
		mContentManager.Clear();
		ContentTypeReaderManager::Shutdown();

#if defined(DEBUG) || defined(_DEBUG)
		DumpD3DDebug();
#endif
	}

	void Game::UpdateStates(const GameState& gameState)
	{
		for (auto& component : mComponents)
		{
			if (component->Enabled())
			{
				component->Update(gameState);
			}
		}

		mRootGameObject->Update(gameState);
	}

	void Game::UpdateDraw(const GameState& gameState)
	{
		mRenderer.ClearScreen();

		mRootGameObject->Draw(gameState);

		for (auto& component : mComponents)
		{
			DrawableGameComponent* drawableGameComponent = component->As<DrawableGameComponent>();
			if (drawableGameComponent != nullptr && drawableGameComponent->Visible())
			{
				drawableGameComponent->Draw(gameState);
			}
		}

		HRESULT hr  = mRenderer.SwapChain()->Present(1, 0);;

		if (hr == DXGI_ERROR_DEVICE_REMOVED || hr == DXGI_ERROR_DEVICE_RESET)
		{
			mRenderer.HandleDeviceLost();
		}
		else
		{
			ThrowIfFailed(hr, "IDXGISwapChain::Present() failed.");
		}

	}


	void Game::UpdateInput(const GameState& gameState)
	{
		UNREFERENCED_PARAMETER(gameState);
		if (mKeyboard->WasKeyPressedThisFrame(Keys::Escape) || mGamePad->WasButtonPressedThisFrame(GamePadButtons::Back))
		{
			Exit();
		}

		if (mMouse->WasButtonPressedThisFrame(MouseButtons::Left))
		{
			mMouse->SetMode(MouseModes::Relative);
		}

		if (mMouse->WasButtonReleasedThisFrame(MouseButtons::Left))
		{
			mMouse->SetMode(MouseModes::Absolute);
		}
		if (mKeyboard->WasKeyPressedThisFrame(Keys::G))
		{
			mGrid->SetVisible(!mGrid->Visible());
		}

		//Update Input Components
		for (auto& component : mIComponents)
		{
			if (component->Enabled())
			{
				component->Update(gameState);
			}
		}
	}


	void Game::Exit()
	{
		PostQuitMessage(0);
	}

	void Game::SetRootGameObject(GameObject& gameObject)
	{
		mRootGameObject = &gameObject;
	}



	void Game::InitializeRootGameObject()
	{
		
		if (mRootGameObject == nullptr) { throw std::runtime_error("Root game object not set. Can't initialize"); }

		auto assignObjectGame = [this](GameObject& gObject)
		{
			gObject.Initialize();
		};

		auto assignActionGame = [this](Action& gAction)
		{
			gAction.Initialize();
		};
		mRootGameObject->WorkOnAllChildrenOfType<GameObject>(assignObjectGame);
		mRootGameObject->WorkOnAllChildrenOfType<Action>(assignActionGame);
	}

	const std::shared_ptr<FieaGameEngine::KeyboardComponent> Game::GetKeyBoard() const
	{
		return mKeyboard;
	}
	const std::shared_ptr<FieaGameEngine::MouseComponent> Game::GetMouse() const
	{
		return mMouse;
	}
	const std::shared_ptr<FieaGameEngine::GamePadComponent> Game::GetGamePad() const
	{
		return mGamePad;
	}



}