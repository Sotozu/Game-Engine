#include "pch.h"
#include "ToStringSpecializations.h"
#include "Attributed.h"
#include "GameObject.h"
#include "AvatarFoo.h"
#include "IJsonCoordinator.h"
#include "JsonScopeHelper.h"
#include "AttributedFoo.h"
#include "DerivedAttributedFoo.h"
#include "SuperDerivedAttributedFoo.h"
#include "GameState.h"
using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTestLibraryDesktop
{

	TEST_CLASS(GameObjectTest)
	{
	public:
		TEST_METHOD_INITIALIZE(Initialize) //Before every test this is called.
		{
			TypeRegistry::RegisterType<GameObject>();
			TypeRegistry::RegisterType<AvatarFoo>();
			TypeRegistry::RegisterType<GameObject>();


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
#pragma region GameObject Tests
		TEST_METHOD(GameObjectConstructor)
		{
			GameObject myGameObject;
			glm::vec4 defaultVector{ 0.0f, 0.0f, 0.0f, 0.0f };

			Assert::AreEqual(""s, myGameObject._name);
			Assert::AreEqual(defaultVector, myGameObject.GetTransform()._position);
			Assert::AreEqual(defaultVector, myGameObject.GetTransform()._rotation);
			Assert::AreEqual(defaultVector, myGameObject.GetTransform()._scale);
		}
		TEST_METHOD(GameObjectConstructor_nameGetTransform())
		{
			glm::vec4 position(100.0f, 200.0f, 300.0f, 400.0f);
			glm::vec4 rotation(100.0f, 200.0f, 300.0f, 400.0f);
			glm::vec4 scale(100.0f, 200.0f, 300.0f, 400.0f);

			Transform myTransform(position, rotation, scale);


			GameObject myGameObject("Alex", myTransform);

			Assert::AreEqual("Alex"s, myGameObject._name);
			Assert::AreEqual(position, myGameObject.GetTransform()._position);
			Assert::AreEqual(rotation, myGameObject.GetTransform()._rotation);
			Assert::AreEqual(scale, myGameObject.GetTransform()._scale);
		}

		TEST_METHOD(GameObjectConstructor_name_position_rotation_scale)
		{
			glm::vec4 position(100.0f, 200.0f, 300.0f, 400.0f);
			glm::vec4 rotation(100.0f, 200.0f, 300.0f, 400.0f);
			glm::vec4 scale(100.0f, 200.0f, 300.0f, 400.0f);


			GameObject myGameObject("Alex", position, rotation, scale);

			Assert::AreEqual("Alex"s, myGameObject._name);
			Assert::AreEqual(position, myGameObject.GetTransform()._position);
			Assert::AreEqual(rotation, myGameObject.GetTransform()._rotation);
			Assert::AreEqual(scale, myGameObject.GetTransform()._scale);
		}
		TEST_METHOD(Clone)
		{
			GameObject myGameObject;

			myGameObject._name = "Alex"s;

			glm::vec4 position{ 1.0f, 2.0f, 3.0f, 4.0f };
			glm::vec4 rotation{ 1.1f, 2.1f, 3.1f, 4.1f };
			glm::vec4 scale{ 1.2f, 2.2f, 3.2f, 4.2f };
			myGameObject.GetTransform()._position = position;
			myGameObject.GetTransform()._rotation = rotation;
			myGameObject.GetTransform()._scale = scale;

			GameObject* myClone = myGameObject.Clone();

			Assert::AreEqual("Alex"s, myClone->_name);
			Assert::AreEqual(position, myClone->GetTransform()._position);
			Assert::AreEqual(rotation, myClone->GetTransform()._rotation);
			Assert::AreEqual(scale, myClone->GetTransform()._scale);

			delete myClone;
		}
		TEST_METHOD(GameObjectJsonParse)
		{
			IJsonCoordinator myCoordinator;
			myCoordinator.AddHelper<JsonScopeHelper>();
			AvatarFooFactory myAvatarFooFactory;

			const JsonScopeHelper::Data* _myDataClass = myCoordinator.ParseFromFile<JsonScopeHelper::Data>("Content/GameObject.json");
			_myDataClass;


			const Scope* myGameObjectScope = _myDataClass->GetData();
			const GameObject* myGameObject = myGameObjectScope->As<GameObject>();
			
			Assert::IsNotNull(myGameObject);

			Assert::AreEqual(static_cast<std::size_t>(7), myGameObjectScope->NumberOfItems());

			const Datum* gameObjectName = myGameObjectScope->Find("_name");
			Assert::IsNotNull(gameObjectName);
			Assert::AreEqual(DatumTypes::String, gameObjectName->Type());
			Assert::AreEqual(static_cast<std::size_t>(1), gameObjectName->Size());
			Assert::AreEqual("Alex"s, gameObjectName->Get<std::string>());;

			glm::vec4 _gameObjectVector(1.0f, 2.0f, 3.0f, 4.0f);

			const Datum* gameObjectPosition = myGameObjectScope->Find("_position");
			Assert::IsNotNull(gameObjectPosition);
			Assert::AreEqual(DatumTypes::Vector4, gameObjectPosition->Type());
			Assert::AreEqual(static_cast<std::size_t>(1), gameObjectPosition->Size());
			Assert::AreEqual(_gameObjectVector, gameObjectPosition->Get<glm::vec4>());;

			const Datum* gameObjectRotation = myGameObjectScope->Find("_rotation");
			Assert::IsNotNull(gameObjectRotation);
			Assert::AreEqual(DatumTypes::Vector4, gameObjectRotation->Type());
			Assert::AreEqual(static_cast<std::size_t>(1), gameObjectRotation->Size());
			Assert::AreEqual(_gameObjectVector, gameObjectRotation->Get<glm::vec4>());;

			const Datum* gameObjectScale = myGameObjectScope->Find("_scale");
			Assert::IsNotNull(gameObjectScale);
			Assert::AreEqual(DatumTypes::Vector4, gameObjectScale->Type());
			Assert::AreEqual(static_cast<std::size_t>(1), gameObjectScale->Size());
			Assert::AreEqual(_gameObjectVector, gameObjectScale->Get<glm::vec4>());;


			Assert::AreEqual("Alex"s, myGameObject->_name);
			Assert::AreEqual(_gameObjectVector, myGameObject->GetTransform()._position);
			Assert::AreEqual(_gameObjectVector, myGameObject->GetTransform()._rotation);
			Assert::AreEqual(_gameObjectVector, myGameObject->GetTransform()._scale);

			const Datum* myChildren = myGameObjectScope->Find("_children");
			Assert::IsNotNull(myChildren);
			Assert::AreEqual(DatumTypes::Table, myChildren->Type());
			Assert::AreEqual(static_cast<std::size_t>(1), myChildren->Size());
			
			const Scope& myGameObject0 = myChildren->Get<Scope>(0);

			Assert::IsNotNull(myGameObject0.As<GameObject>());
			Assert::IsNotNull(myGameObject0.As<AvatarFoo>());

			const AvatarFoo* myAvatarFoo = myGameObject0.As<AvatarFoo>();
			
			glm::vec4 _avatarFooVector(100.0f, 200.0f, 300.0f, 400.0f);

			Assert::AreEqual("MyAvatarFoo"s, myAvatarFoo->_name);
			Assert::AreEqual(500, myAvatarFoo->_hitPoints);
			Assert::AreEqual(_avatarFooVector, myAvatarFoo->GetTransform()._position);
			Assert::AreEqual(_avatarFooVector, myAvatarFoo->GetTransform()._rotation);
			Assert::AreEqual(_avatarFooVector, myAvatarFoo->GetTransform()._scale);


		}
		TEST_METHOD(GameObjectUpdate)
		{
			//GameTime time;
			GameState gameState;
			AvatarFoo myAvatarFoo;
			GameObject* myGameObject = &myAvatarFoo;
			Assert::AreEqual(100, myAvatarFoo._hitPoints);
			myGameObject->Update(gameState);
			Assert::AreEqual(101, myAvatarFoo._hitPoints);
		}
		TEST_METHOD(GameObjectUpdateChildren)
		{

			//GameTime myGameTime;
			GameState gameState;
			IJsonCoordinator myCoordinator;
			myCoordinator.AddHelper<JsonScopeHelper>();
			AvatarFooFactory myAvatarFooFactory;

			const JsonScopeHelper::Data* _myDataClass = myCoordinator.ParseFromFile<JsonScopeHelper::Data>("Content/GameObject.json");
			_myDataClass;


			Scope* myRootGameObjectScope = _myDataClass->GetData();

			GameObject* myRootGameObject = myRootGameObjectScope->As<GameObject>();

			Datum* myChildrenDatum = myRootGameObject->Find("_children");
			Assert::AreEqual(DatumTypes::Table, myChildrenDatum->Type());

			Scope& myScope1 = myChildrenDatum->Get<Scope>();

			AvatarFoo* myAvatarFoo1 = myScope1.As<AvatarFoo>();
			Assert::IsNotNull(myAvatarFoo1);


			myChildrenDatum = myAvatarFoo1->Find("_children");
			Assert::AreEqual(DatumTypes::Table, myChildrenDatum->Type());

			Scope& myScope2 = myChildrenDatum->Get<Scope>();

			AvatarFoo* myAvatarFoo2 = myScope2.As<AvatarFoo>();

			Assert::IsNotNull(myAvatarFoo2);

			Assert::AreEqual(500, myAvatarFoo1->_hitPoints);
			Assert::AreEqual(1000, myAvatarFoo2->_hitPoints);

			myRootGameObject->Update(gameState);

			Assert::AreEqual(501, myAvatarFoo1->_hitPoints);
			Assert::AreEqual(1001, myAvatarFoo2->_hitPoints);
		}
		TEST_METHOD(GameObjectEquals)
		{
			glm::vec4 position(100.0f, 200.0f, 300.0f, 400.0f);
			glm::vec4 rotation(100.0f, 200.0f, 300.0f, 400.0f);
			glm::vec4 scale(100.0f, 200.0f, 300.0f, 400.0f);

			//When equivalent
			{
				GameObject myGameObect("Alex", position, rotation, scale);

				GameObject myGameObectEquivalent("Alex", position, rotation, scale);

				Assert::IsTrue(myGameObect.Equals(&myGameObectEquivalent));
			}

			//When not equivalent
			{
				GameObject myGameObect("Alex", position, rotation, scale);

				GameObject myGameObectNotEquivalent("Alexander", position, rotation, scale);

				Assert::IsFalse(myGameObect.Equals(&myGameObectNotEquivalent));

			}
		}
		TEST_METHOD(RTTI_Test)
		{
			//GameObject
			{
				GameObject myGameObject;

				RTTI* myRTTIGameObject = &myGameObject;
				Assert::IsNotNull(myRTTIGameObject->As<Scope>());
				Assert::IsNotNull(myRTTIGameObject->As<Attributed>());
				Assert::IsNotNull(myRTTIGameObject->As<GameObject>());

				Assert::IsNull(myRTTIGameObject->As<AvatarFoo>());

			}

			//GameObject Derived Classes
			{
				AvatarFoo myAvatarFoo;

				RTTI* myRTTIAvatarFoo = &myAvatarFoo;
				Assert::IsNotNull(myRTTIAvatarFoo->As<Scope>());
				Assert::IsNotNull(myRTTIAvatarFoo->As<Attributed>());
				Assert::IsNotNull(myRTTIAvatarFoo->As<GameObject>());
				Assert::IsNotNull(myRTTIAvatarFoo->As<AvatarFoo>());

				Assert::IsNull(myRTTIAvatarFoo->As<AttributedFoo>());
				Assert::IsNull(myRTTIAvatarFoo->As<DerivedAttributedFoo>());
				Assert::IsNull(myRTTIAvatarFoo->As<SuperDerivedAttributedFoo>());

			}
		}
		TEST_METHOD(GameObjectToString)
		{
			GameObject myGameObject;

			RTTI* myRTTIGameObject = &myGameObject;
			Assert::AreEqual("GameObject"s, myRTTIGameObject->ToString());
		}
#pragma endregion

#pragma region Transform Tests
		TEST_METHOD(TransformConstructor)
		{
			Transform myTransform;

			glm::vec4 defaultVector{ 0.0f, 0.0f, 0.0f, 0.0f };

			Assert::AreEqual(defaultVector, myTransform._position);
			Assert::AreEqual(defaultVector, myTransform._rotation);
			Assert::AreEqual(defaultVector, myTransform._scale);

		}
		TEST_METHOD(TransformConstructor_position_rotation_scale)
		{

			glm::vec4 position(100.0f, 200.0f, 300.0f, 400.0f);
			glm::vec4 rotation(100.0f, 200.0f, 300.0f, 400.0f);
			glm::vec4 scale(100.0f, 200.0f, 300.0f, 400.0f);


			Transform myTransform( position, rotation, scale);


			Assert::AreEqual(scale, myTransform._position);
			Assert::AreEqual(rotation, myTransform._rotation);
			Assert::AreEqual(position, myTransform._scale);

		}

	private:
		static _CrtMemState _startMemState; //static class members are shared in every instance of the class
	};

	_CrtMemState GameObjectTest::_startMemState; //There is needed to separate declaration from definition. Changes in C++17
}