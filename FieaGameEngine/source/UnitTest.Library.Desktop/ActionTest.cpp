#include "pch.h"
#include "ToStringSpecializations.h"
#include "Attributed.h"
#include "IJsonCoordinator.h"
#include "JsonScopeHelper.h"
#include "GameObject.h"
#include "AvatarFoo.h"
#include "ActionIntegerIncrement.h"
#include "ActionList.h"
#include "GameState.h"
#include "ActionCreateAction.h"
#include "ActionDestroyAction.h"
#include "ActionIfAction.h"
#include "ActionExpression.h"
using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTestLibraryDesktop
{

	TEST_CLASS(ActionTest)
	{
	public:
		TEST_METHOD_INITIALIZE(Initialize) //Before every test this is called.
		{

			TypeRegistry::RegisterType<ActionCreateAction>();
			TypeRegistry::RegisterType<ActionDestroyAction>();
			TypeRegistry::RegisterType<ActionIntegerIncrement>();
			TypeRegistry::RegisterType<ActionList>();
			TypeRegistry::RegisterType<ActionIfAction>();
			TypeRegistry::RegisterType<ActionExpression>();

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
		TEST_METHOD(Action_ActionIncrement)
		{
			//GameTime myGameTime;
			GameState gameState;
			IJsonCoordinator myCoordinator;
			myCoordinator.AddHelper<JsonScopeHelper>();
			AvatarFooFactory myAvatarFooFactory;
			ActionIntegerIncrementFactory myActionIncrementFactory;

			const JsonScopeHelper::Data* _myDataClass = myCoordinator.ParseFromFile<JsonScopeHelper::Data>("Content/ActionIncrement.json");
			GameObject* myRootGameObject = _myDataClass->GetData()->As<GameObject>();
			Assert::IsNotNull(myRootGameObject);

			Datum* myChildrenScopeDatum = myRootGameObject->Find("_children");
			Assert::IsNotNull(myChildrenScopeDatum);

			Scope& myChildrenScope = myChildrenScopeDatum->Get<Scope>();

			AvatarFoo* myAvatar = myChildrenScope.As<AvatarFoo>();
			Assert::IsNotNull(myAvatar);


			Datum* myActionScopeDatum = myAvatar->Find("_actions");
			Assert::IsNotNull(myActionScopeDatum);

			Scope& myActionScope = myActionScopeDatum->Get<Scope>();

			ActionIntegerIncrement* myActionIncrement = myActionScope.As<ActionIntegerIncrement>();
			Assert::IsNotNull(myActionIncrement);

			Assert::AreEqual("Increment _hitPoints"s, myActionIncrement->GetName());
			Assert::AreEqual(1, myActionIncrement->_step);
			Assert::AreEqual("_hitPoints"s, myActionIncrement->_targetKey);

			Assert::AreEqual(500, myAvatar->_hitPoints);
			Assert::IsNull(myActionIncrement->GetTarget());
			myActionIncrement->Update(gameState);
			Assert::IsNotNull(myActionIncrement->GetTarget());
			Assert::AreEqual(501, myAvatar->_hitPoints);

		}
		TEST_METHOD(Action_ActionList)
		{
			//GameTime myGameTime;
			GameState gameState;
			IJsonCoordinator myCoordinator;
			myCoordinator.AddHelper<JsonScopeHelper>();
			AvatarFooFactory myAvatarFooFactory;
			ActionIntegerIncrementFactory myActionIncrementFactory;
			ActionListFactory myActionListFactory;

			const JsonScopeHelper::Data* _myDataClass = myCoordinator.ParseFromFile<JsonScopeHelper::Data>("Content/ActionList.json");
			GameObject* myRootGameObject = _myDataClass->GetData()->As<GameObject>();
			Assert::IsNotNull(myRootGameObject);

			Datum* myChildrenDatum = myRootGameObject->Find("_children");
			Assert::IsNotNull(myChildrenDatum);

			Scope& myAvatarFooScope = myChildrenDatum->Get<Scope>();

			AvatarFoo* myAvatarFoo = myAvatarFooScope.As<AvatarFoo>();
			Assert::IsNotNull(myAvatarFoo);

			Datum* myActions = myAvatarFoo->GetActions();
			myActions->Size();
			Assert::AreEqual(500, myAvatarFoo->_hitPoints);
			myRootGameObject->Update(gameState);
			Assert::AreEqual(506, myAvatarFoo->_hitPoints);


		}
		TEST_METHOD(GameObject_CreateAction)
		{
			GameTime myGameTime;
			GameState gameState;
			IJsonCoordinator myCoordinator;
			myCoordinator.AddHelper<JsonScopeHelper>();
			AvatarFooFactory myAvatarFooFactory;

			ActionIntegerIncrementFactory myActionIncrementFactory;
			ActionListFactory myActionListFactory;


			const JsonScopeHelper::Data* _myDataClass = myCoordinator.ParseFromFile<JsonScopeHelper::Data>("Content/GameObjectCreateAction.json");
			GameObject* myRootGameObject = _myDataClass->GetData()->As<GameObject>();
			Assert::IsNotNull(myRootGameObject);

			Datum* myChildrenScopeDatum = myRootGameObject->Find("_children");
			Assert::IsNotNull(myChildrenScopeDatum);

			Scope& myChildrenScope = myChildrenScopeDatum->Get<Scope>();

			AvatarFoo* myAvatarFoo = myChildrenScope.As<AvatarFoo>();
			Assert::IsNotNull(myAvatarFoo);
			Datum* myActions = myAvatarFoo->GetActions();


			//The AvatarFoo Update itself increases it's hit points by 1. 
			//There are an additional 5 ActionIntegerIncrement Actions that it "owns" that will increase it's health by 1 each.
			//Total increased health 6
			Assert::AreEqual(500, myAvatarFoo->_hitPoints);
			myRootGameObject->Update(gameState);
			Assert::AreEqual(506, myAvatarFoo->_hitPoints);


			//reset the hit points back to 500
			myAvatarFoo->_hitPoints = 500;

			Assert::AreEqual(static_cast<std::size_t>(3), myActions->Size());

			Action& myAction = myAvatarFoo->CreateAction("ActionIntegerIncrement", "My Created Integer Increment"); //Added an additional action which will increase the health
			ActionIntegerIncrement& myActionIntegerIncrement = *myAction.As<ActionIntegerIncrement>();

			myActionIntegerIncrement.Init(1, "_hitPoints");

			Assert::AreEqual(static_cast<std::size_t>(4), myActions->Size());

			//Added an additional ActionIntegerIncrement Actions which will now have the AvatarFoo Health increase by 7 
			Assert::AreEqual(500, myAvatarFoo->_hitPoints);
			myRootGameObject->Update(gameState);
			Assert::AreEqual(507, myAvatarFoo->_hitPoints);

		}
		TEST_METHOD(Action_ActionCreateAction)
		{
			//GameTime myGameTime;
			GameState gameState;
			IJsonCoordinator myCoordinator;
			myCoordinator.AddHelper<JsonScopeHelper>();

			AvatarFooFactory myAvatarFooFactory;

			ActionIntegerIncrementFactory myActionIncrementFactory;
			ActionListFactory myActionListFactory;
			ActionCreateActionFactory myActionCreateActionFactory;

			const JsonScopeHelper::Data* _myDataClass = myCoordinator.ParseFromFile<JsonScopeHelper::Data>("Content/ActionCreateAction.json");
			GameObject* myRootGameObject = _myDataClass->GetData()->As<GameObject>();
			Assert::IsNotNull(myRootGameObject);

			AvatarFoo* avatar = myRootGameObject->Find("_children")->Get<Scope>().As<AvatarFoo>();
			Assert::IsNotNull(avatar);
			Datum* actionsDatum = avatar->Find("_actions");

			Assert::AreEqual(500, avatar->_hitPoints);

			Assert::AreEqual(static_cast<std::size_t>(1), actionsDatum->Size());
			myRootGameObject->Update(gameState); //Creating an ActionIntegerIncrement Action. Also avatar runs it's own update method which increments its _hitPoints by 1
			Assert::AreEqual(static_cast<std::size_t>(1), actionsDatum->Size());
			gameState.FinalizeUpdate(); //Adopting that ActionIntegerIncrement Action to it's target.
			Assert::AreEqual(static_cast<std::size_t>(2), actionsDatum->Size());

			Assert::AreEqual(501, avatar->_hitPoints); //Avatars update method increased it's _hitPoints variable by 1

			ActionIntegerIncrement* actionII_1 = actionsDatum->Get<Scope>(actionsDatum->Size() - 1).As<ActionIntegerIncrement>();//Find created action
			actionII_1->Init(1, "_hitPoints"); //initialize it

			myRootGameObject->Update(gameState);
			gameState.FinalizeUpdate();
			Assert::AreEqual(503, avatar->_hitPoints); //+ 1 for AvatarFoo update and + 1(xNumberOf ActionIntegerIncrement Actions)


			ActionIntegerIncrement* actionII_2 = actionsDatum->Get<Scope>(actionsDatum->Size() - 1).As<ActionIntegerIncrement>();
			actionII_2->Init(1, "_hitPoints");

			myRootGameObject->Update(gameState);
			gameState.FinalizeUpdate();
			Assert::AreEqual(506, avatar->_hitPoints);

		}
		TEST_METHOD(Action_ActionDestroyAction)
		{
			//GameTime myGameTime;
			GameState gameState;
			IJsonCoordinator myCoordinator;
			myCoordinator.AddHelper<JsonScopeHelper>();

			AvatarFooFactory myAvatarFooFactory;

			ActionIntegerIncrementFactory myActionIncrementFactory;
			ActionListFactory myActionListFactory;
			ActionDestroyActionFactory myActionDestroyActionFactory;

			const JsonScopeHelper::Data* _myDataClass = myCoordinator.ParseFromFile<JsonScopeHelper::Data>("Content/ActionDestroyAction.json");
			GameObject* myRootGameObject = _myDataClass->GetData()->As<GameObject>();
			Assert::IsNotNull(myRootGameObject);

			AvatarFoo* avatar = myRootGameObject->Find("_children")->Get<Scope>().As<AvatarFoo>();
			Assert::IsNotNull(avatar);
			Assert::AreEqual(500, avatar->_hitPoints);

			Datum* actionsDatum = avatar->Find("_actions");
			Assert::AreEqual(static_cast<std::size_t>(2), actionsDatum->Size());

			myRootGameObject->Update(gameState); //Queuing the deletion of an ActionIntegerIncrement Action.
			gameState.FinalizeUpdate();
			Assert::AreEqual(static_cast<std::size_t>(1), actionsDatum->Size());

			Assert::AreEqual(502, avatar->_hitPoints); //The ActionIntegerIncrement Action +1 health and the avatar update method + 1 health. TOTAL health increase is 2.


			myRootGameObject->Update(gameState); //At this time the ActionIntegerIncrement Action has been removed.
			gameState.FinalizeUpdate();
			Assert::AreEqual(static_cast<std::size_t>(1), actionsDatum->Size());

			Assert::AreEqual(503, avatar->_hitPoints); //Avatar update method + 1 health. TOTAL health increase is 1.
	
		}
		TEST_METHOD(Action_ActionIfAction)
		{
			//GameTime myGameTime;
			GameState gameState;
			IJsonCoordinator myCoordinator;
			myCoordinator.AddHelper<JsonScopeHelper>();

			AvatarFooFactory myAvatarFooFactory;

			ActionIntegerIncrementFactory myActionIncrementFactory;

			ActionListFactory myActionListFactory;

			ActionIfActionFactory myActionIfActionFactory;

			const JsonScopeHelper::Data* _myDataClass = myCoordinator.ParseFromFile<JsonScopeHelper::Data>("Content/ActionIfAction.json");
			GameObject* myRootGameObject = _myDataClass->GetData()->As<GameObject>();
			Assert::IsNotNull(myRootGameObject);


			AvatarFoo* avatar = myRootGameObject->Find("_children")->Get<Scope>().As<AvatarFoo>();
			
			Assert::IsNotNull(avatar);
			Assert::AreEqual(500, avatar->_hitPoints);
			Assert::AreEqual(false, avatar->_isTakingDamage);

			myRootGameObject->Update(gameState);
			Assert::AreEqual(501, avatar->_hitPoints);
			avatar->_isTakingDamage = true;

			//The ActionIfAction block will have the avatar take -6 health points in damage when the bool _isTakingDamage is true

			myRootGameObject->Update(gameState);
			Assert::AreEqual(496, avatar->_hitPoints);
			
			avatar->_isTakingDamage = false;
			myRootGameObject->Update(gameState);
			Assert::AreEqual(497, avatar->_hitPoints);
			myRootGameObject->Update(gameState);
			Assert::AreEqual(498, avatar->_hitPoints);
			myRootGameObject->Update(gameState);
			Assert::AreEqual(499, avatar->_hitPoints);
			myRootGameObject->Update(gameState);
			Assert::AreEqual(500, avatar->_hitPoints);

			avatar->_isTakingDamage = true;
			myRootGameObject->Update(gameState);
			Assert::AreEqual(495, avatar->_hitPoints);

			ActionDestroyAction destroyAction;
			destroyAction.SetName("Destroy Action!");
			destroyAction.SetTargetAction("My If Action");

			avatar->_isTakingDamage = false;

			avatar->Adopt("_actions", destroyAction);

			myRootGameObject->Update(gameState);
			gameState.FinalizeUpdate();
			Assert::AreEqual(496, avatar->_hitPoints);

			avatar->_isTakingDamage = true; //Action If Action previously would lower our health by 6. But has been removed.
			myRootGameObject->Update(gameState);
			gameState.FinalizeUpdate();
			Assert::AreEqual(497, avatar->_hitPoints);


		}

		TEST_METHOD(Action_ActionExpression)
		{

			GameState gameState;

			ActionExpression myAction;

			AvatarFooFactory myAvatarFooFactory;

			ActionExpressionFactory myActionIncrementFactory;

			ActionListFactory myActionListFactory;

			IJsonCoordinator myCoordinator;
			myCoordinator.AddHelper<JsonScopeHelper>();


			JsonScopeHelper::Data* _myDataClass = myCoordinator.ParseFromFile<JsonScopeHelper::Data>("Content/ActionExpression.json");
			_myDataClass;

			GameObject* rootGameObject = _myDataClass->GetData()->As<GameObject>();
			AvatarFoo* myAvatarFoo = rootGameObject->Find("_children")->Get<Scope>().As<AvatarFoo>();
			
			ActionExpression* ae = myAvatarFoo->Find("_actions")->Get<Scope>().As<ActionExpression>();
			Datum* _expression = ae->Find("_expression");
			std::string exp = _expression->Get<std::string>(0);
			exp;
			Assert::AreEqual(500, myAvatarFoo->_hitPoints);

			myAvatarFoo->Update(gameState);

			Assert::AreEqual(100, myAvatarFoo->_hitPoints);

		}
	private:
		static _CrtMemState _startMemState; //static class members are shared in every instance of the class
	};

	_CrtMemState ActionTest::_startMemState; //There is needed to separate declaration from definition. Changes in C++17
}