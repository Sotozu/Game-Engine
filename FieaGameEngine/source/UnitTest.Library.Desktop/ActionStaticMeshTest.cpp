#include "pch.h"


#pragma region pch.h from Library pasted here.
// Windows
#include <winrt\Windows.Foundation.h>
#include <windows.h>

// Standard Library
#include <cstddef>
#include <cstdint>
#include <stdexcept>

#include <exception>
#include <cassert>
#include <string>
#include <iostream>
#include <sstream>
#include <fstream>
#include <memory>
#include <vector>
#include <map>
#include <stack>
#include <cstdint>
#include <iomanip>
#include <codecvt>
#include <algorithm>
#include <functional>
#include <limits>
#include <filesystem>

// Microsoft Guidelines Support Library
#include <gsl\gsl>




// DirectX
#include <d3d11_4.h>
#include <dxgi1_6.h>
#include <DirectXMath.h>
#include <DirectXPackedVector.h>
#include <DirectXColors.h>
#include <DirectXTK\DDSTextureLoader.h>
#include <DirectXTK\WICTextureLoader.h>
#include <DirectXTK\SpriteBatch.h>
#include <DirectXTK\SpriteFont.h>
#include <DirectXTK\GamePad.h>
#include <DirectXTK\Keyboard.h>
#include <DirectXTK\Mouse.h>

#ifndef UNREFEREMCED_PARAMETER
#define UNREFERENCED_PARAMETER(P) (P)
#endif
#pragma endregion

#include "ToStringSpecializations.h"
#include "Attributed.h"
#include "IJsonCoordinator.h"
#include "JsonScopeHelper.h"
#include "GameObject.h"
#include "AvatarFoo.h"
#include "Light.h"
#include "LightRegistry.h"
#include "PointLight.h"
#include "GameState.h"
#include "ActionMeshRenderer.h"
#include "ModelResource.h"
#include "BasicMaterial.h"
#include "TextureMaterial.h"
#include "Model.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTestLibraryDesktop
{

	TEST_CLASS(ActionStaticMeshTest)
	{
	public:
		TEST_METHOD_INITIALIZE(Initialize) //Before every test this is called.
		{

			TypeRegistry::RegisterType<Model>();
			TypeRegistry::RegisterType<ActionMeshRenderer>();
			TypeRegistry::RegisterType<Light>();
			TypeRegistry::RegisterType<PointLight>();
			TypeRegistry::RegisterType<TextureMaterial>();
			TypeRegistry::RegisterType<AvatarFoo>();

#if defined(_DEBUG) //Only runs in Debug mode.
			_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF);
			_CrtMemCheckpoint(&_startMemState); //Takes a snapshot of heap allocated data.
#endif

		}
		TEST_METHOD_CLEANUP(Cleanup) //After every test this is called.
		{
#if defined(_DEBUG)
			_CrtMemState endMemState, diffMemState;
			_CrtMemCheckpoint(&endMemState); //Takes a snapshot of heap allocated data.
			if (_CrtMemDifference(&diffMemState, &_startMemState, &endMemState)) //Compares heap allocated data before and after the test ran.
			{
				_CrtMemDumpStatistics(&diffMemState);
				Assert::Fail(L"Memory Leaks!");
			}
#endif
		}
		TEST_METHOD(DeserializeActionMeshRenderer)
		{
			
			GameState gameState;
			IJsonCoordinator coordinator;
			coordinator.AddHelper<JsonScopeHelper>();

			AvatarFooFactory myAvatarFooFactory;

			ActionMeshRendererFactory actionIncrementFactory;

			
			JsonScopeHelper::Data* dataClass = coordinator.ParseFromFile<JsonScopeHelper::Data>("Content/ActionMeshRenderer.json");
			GameObject* rootGameObject = dataClass->GetData()->As<GameObject>();
			Assert::IsNotNull(rootGameObject);


			AvatarFoo* avatarFoo = rootGameObject->Find("_children")->Get<Scope>().As<AvatarFoo>();

			ActionMeshRenderer* staticMesh = avatarFoo->Find("_actions")->Get<Scope>().As<ActionMeshRenderer>();

			Assert::AreEqual("GameObject_SM_1"s, staticMesh->_name);
		}
		TEST_METHOD(DeserializeLight)
		{
			GameState gameState;
			IJsonCoordinator coordinator;
			coordinator.AddHelper<JsonScopeHelper>();

			GameObjectFactory gameObjectFactory;
			LightFactory lightFactory;


			JsonScopeHelper::Data* dataClass = coordinator.ParseFromFile<JsonScopeHelper::Data>("Content/Light.json");
			GameObject* rootGameObject = dataClass->GetData()->As<GameObject>();
			Assert::IsNotNull(rootGameObject);

			Light* light = rootGameObject->Find("_children")->Get<Scope>().As<Light>();

			Assert::AreEqual("light_1"s, light->_name);

			Assert::AreEqual(0.1f, light->Color().x);
			Assert::AreEqual(0.2f, light->Color().y);
			Assert::AreEqual(0.3f, light->Color().z);
			Assert::AreEqual(0.4f, light->Color().w);

		}

		TEST_METHOD(DeserializePointLight)
		{
			GameState gameState;
			IJsonCoordinator coordinator;
			coordinator.AddHelper<JsonScopeHelper>();

			GameObjectFactory gameObjectFactory;
			PointLightFactory pointLightFactory;

			JsonScopeHelper::Data* dataClass = coordinator.ParseFromFile<JsonScopeHelper::Data>("Content/PointLight.json");
			GameObject* rootGameObject = dataClass->GetData()->As<GameObject>();
			Assert::IsNotNull(rootGameObject);

			PointLight* point_light = rootGameObject->Find("_children")->Get<Scope>().As<PointLight>();

			Assert::AreEqual("point_light_1"s, point_light->_name);

			Assert::AreEqual(0.1f, point_light->Color().x);
			Assert::AreEqual(0.2f, point_light->Color().y);
			Assert::AreEqual(0.3f, point_light->Color().z);
			Assert::AreEqual(0.4f, point_light->Color().w);

			Assert::AreEqual(100.0f, point_light->Radius());

		}
		TEST_METHOD(DeserializeModel)
		{
			GameState gameState;
			IJsonCoordinator coordinator;
			coordinator.AddHelper<JsonScopeHelper>();
			GameObjectFactory gameObjectFactory;
			ModelFactory modelFactory;

			JsonScopeHelper::Data* dataClass = coordinator.ParseFromFile<JsonScopeHelper::Data>("Content/Model.json");
			GameObject* rootGameObject = dataClass->GetData()->As<GameObject>();
			Assert::IsNotNull(rootGameObject);

			Model* model = rootGameObject->Find("_children")->Get<Scope>().As<Model>();

			Assert::AreEqual("model_1"s, model->_name);
			Assert::AreEqual("Models\\Sphere1.obj.bin"s, model->GetModelPath());


			//Assert::AreEqual(false, model->HasMeshes());

			//I can't initialize this because there needs to be an instance of the game and the model needs to be parented to the hierarchy for it to work.
			//model->Initialize();

			//Assert::AreEqual(true, model->HasMeshes());
		}

		TEST_METHOD(LightRegistryTest)
		{
			GameState gameState;
			IJsonCoordinator coordinator;
			coordinator.AddHelper<JsonScopeHelper>();

			GameObjectFactory gameObjectFactory;
			LightFactory lightFactory;


			JsonScopeHelper::Data* dataClass = coordinator.ParseFromFile<JsonScopeHelper::Data>("Content/Light.json");
			GameObject* rootGameObject = dataClass->GetData()->As<GameObject>();
			Assert::IsNotNull(rootGameObject);

			Light* light = rootGameObject->Find("_children")->Get<Scope>().As<Light>();

			Assert::AreEqual("light_1"s, light->_name);

			Assert::AreEqual(0.1f, light->Color().x);
			Assert::AreEqual(0.2f, light->Color().y);
			Assert::AreEqual(0.3f, light->Color().z);
			Assert::AreEqual(0.4f, light->Color().w);

			//Light& registered_light = LightRegistry::GetLight();

			//Assert::AreEqual(light, &registered_light);

		}
		TEST_METHOD(SceneTest)
		{
			GameState gameState;
			IJsonCoordinator coordinator;
			coordinator.AddHelper<JsonScopeHelper>();

			GameObjectFactory gof;
			ActionMeshRendererFactory amrf;
			ModelFactory modelf;
			TextureMaterialFactory tmf;


			JsonScopeHelper::Data* dataClass = coordinator.ParseFromFile<JsonScopeHelper::Data>("Content/PPPScene.json");
			GameObject* rootGameObject = dataClass->GetData()->As<GameObject>();
			Assert::IsNotNull(rootGameObject);

			GameObject* go1 = rootGameObject->Find("_children")->Get<Scope>(0).As<GameObject>();
			GameObject* go2 = rootGameObject->Find("_children")->Get<Scope>(1).As<GameObject>();
			GameObject* go3 = rootGameObject->Find("_children")->Get<Scope>(2).As<GameObject>();

			ActionMeshRenderer* amr1 = go1->Find("_actions")->Get<Scope>(0).As<ActionMeshRenderer>();
			ActionMeshRenderer* amr2 = go2->Find("_actions")->Get<Scope>(0).As<ActionMeshRenderer>();
			ActionMeshRenderer* amr3 = go3->Find("_actions")->Get<Scope>(0).As<ActionMeshRenderer>();
			
			Datum* d1 = amr1->Find("_model");
			Datum* d2 = amr2->Find("_model");
			Datum* d3 = amr3->Find("_model");
	
			Model* m1 = d1->Get<Scope>().As<Model>();
			Model* m2 = d2->Get<Scope>().As<Model>();
			Model* m3 = d3->Get<Scope>().As<Model>();

			m1;
			m2;
			m3;
		}


		
	private:
		static _CrtMemState _startMemState; //static class members are shared in every instance of the class
	};

	_CrtMemState ActionStaticMeshTest::_startMemState; //There is needed to separate declaration from definition. Changes in C++17
}