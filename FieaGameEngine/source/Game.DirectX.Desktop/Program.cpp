#include "pch.h"
#include "GameException.h"
#include "Game.h"
#include "TypeRegistry.h"
#include "GameObject.h"
#include "Mesh.h"
#include "ModelResource.h"
#include "ActionMeshRenderer.h"
#include "UtilityWin32.h"
#include "IJsonCoordinator.h"
#include "JsonScopeHelper.h"
#include "BasicMaterial.h"
#include "PointLightMaterial.h"
#include "TextureMaterial.h"
#include "AvatarFoo.h"
#include "PointLight.h"

using namespace FieaGameEngine;
using namespace std;
using namespace std::string_literals;


int WINAPI WinMain(HINSTANCE instance, HINSTANCE, LPSTR, int showCommand)
{
#if defined(DEBUG) | defined(_DEBUG)
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif
	TypeRegistry::RegisterType<BasicMaterial>();
	TypeRegistry::RegisterType<PointLightMaterial>();
	TypeRegistry::RegisterType<Mesh>();
	TypeRegistry::RegisterType<Model>();
	TypeRegistry::RegisterType<GameObject>();
	TypeRegistry::RegisterType<ActionMeshRenderer>();
	TypeRegistry::RegisterType<TextureMaterial>();
	TypeRegistry::RegisterType<PointLight>();

	TypeRegistry::RegisterType<AvatarFoo>();
	

	IJsonCoordinator coordinator;
	coordinator.AddHelper<JsonScopeHelper>();
	BasicMaterialFactory bmf;
	PointLightMaterialFactory plmf;
	MeshFactory meshf;
	ModelFactory modelf;
	GameObjectFactory gof;
	ActionMeshRendererFactory amrf;
	TextureMaterialFactory tmf;
	AvatarFooFactory aff;
	PointLightFactory plf;
	
	auto dataClass = coordinator.ParseFromFile<JsonScopeHelper::Data>("Scene5.json");

	GameObject* rootGameObject = dataClass->GetData()->As<GameObject>();

#pragma region Window setup
	ThrowIfFailed(CoInitializeEx(nullptr, COINITBASE_MULTITHREADED), "Error initializing COM.");

	current_path(UtilityWin32::ExecutableDirectory());

	const wstring windowClassName = L"Game"s;
	const wstring windowTitle = L"Alex.DirectX.Desktop"s;

	const SIZE RenderTargetSize = { 1024, 768 };
	HWND windowHandle;
	WNDCLASSEX window;

	UtilityWin32::InitializeWindow(window, windowHandle, instance, windowClassName, windowTitle, RenderTargetSize, showCommand);

	auto getRenderTargetSize = [&RenderTargetSize](SIZE& renderTargetSize)
	{
		renderTargetSize = RenderTargetSize;
	};

	auto getWindow = [&]() -> void*
	{
		return reinterpret_cast<void*>(windowHandle);
	};
#pragma endregion

	Game::InitializeInstance(rootGameObject, getWindow, getRenderTargetSize, "Game");
	Game& game = Game::GetInstance();
	//Game game(rootGameObject, getWindow, getRenderTargetSize, "Game");

	//StaticMesh mStaticMesh(game);
	game.Initialize();
	MSG message{ 0 };

	try
	{
		while (message.message != WM_QUIT)
		{
			if (PeekMessage(&message, nullptr, 0, 0, PM_REMOVE))
			{
				TranslateMessage(&message);
				DispatchMessage(&message);
			}
			else
			{
				game.Run();
			}
		}
	}
	catch (GameException ex)
	{
		MessageBox(windowHandle, ex.whatw().c_str(), windowTitle.c_str(), MB_ABORTRETRYIGNORE);
	}

	game.Shutdown();
	UnregisterClass(windowClassName.c_str(), window.hInstance);
	CoUninitialize();

	return static_cast<int>(message.wParam);
}