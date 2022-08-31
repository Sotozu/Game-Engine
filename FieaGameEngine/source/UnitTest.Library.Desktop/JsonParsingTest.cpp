#include "pch.h"
#include "ToStringSpecializations.h"
#include "IJsonCoordinator.h"
#include "JsonScopeHelper.h"
#include "JsonIntegerHelper.h"
#include "DerivedAttributedFoo.h"
#include "AttributedFoo.h"
#include "DerivedAttributedFoo.h"
#include "SuperDerivedAttributedFoo.h"
#include "TypeRegistry.h"
#include "GameObject.h"
#include "AvatarFoo.h"


using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTestLibraryDesktop
{

	TEST_CLASS(JsonParsingTest)
	{
	public:
		TEST_METHOD_INITIALIZE(Initialize) //Before every test this is called.
		{

			TypeRegistry::RegisterType<SuperDerivedAttributedFoo>();
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
		TEST_METHOD(IJsonCoordinatorDefaultConstructor)
		{
			IJsonCoordinator myCoordinator;
			Assert::AreEqual(static_cast<std::size_t>(0), myCoordinator.NumberOfHelpers());
		}
		TEST_METHOD(IJsonCoordinatorMoveConstructor)
		{

			//Empty coordinator
			{
				IJsonCoordinator myCoordinator;
				Assert::AreEqual(static_cast<std::size_t>(0), myCoordinator.NumberOfHelpers());

				IJsonCoordinator myMovedCoordinator(std::move(myCoordinator));
				Assert::AreEqual(static_cast<std::size_t>(0), myMovedCoordinator.NumberOfHelpers());

			}

			//Coordinator with helper
			{
				IJsonCoordinator myCoordinator;
				myCoordinator.AddHelper<JsonScopeHelper>();
				Assert::AreEqual(static_cast<std::size_t>(1), myCoordinator.NumberOfHelpers());


				const JsonScopeHelper& myScopeHelper1 = *static_cast<const JsonScopeHelper*>(myCoordinator.FindHelper<JsonScopeHelper>().first); //ew
				
				IJsonCoordinator myMovedCoordinator(std::move(myCoordinator));
				Assert::AreEqual(static_cast<std::size_t>(1), myMovedCoordinator.NumberOfHelpers());

				const JsonScopeHelper& myScopeHelper2 = *static_cast<const JsonScopeHelper*>(myMovedCoordinator.FindHelper<JsonScopeHelper>().first);

				
				Assert::AreSame(myScopeHelper1, myScopeHelper2);

				//myCoordinator should be empty
				Assert::AreEqual(static_cast<std::size_t>(0), myCoordinator.NumberOfHelpers());

			}

			//Coordinator with helper that has contents within its helper data. Test that helpers data are the same when moved
			{
				IJsonCoordinator myCoordinator;
				myCoordinator.AddHelper<JsonScopeHelper>();
				AttributedFooFactory myFactory;

				const JsonScopeHelper::Data* _myDataClass = myCoordinator.ParseFromFile<JsonScopeHelper::Data>("Content/AttributedFoo.json");
				
				Assert::AreEqual(_myDataClass->GetData()->NumberOfItems(), static_cast<std::size_t>(8));
				Assert::AreEqual(static_cast<std::size_t>(1), myCoordinator.NumberOfHelpers());
				
				const JsonScopeHelper& myScopeHelper1 = *static_cast<const JsonScopeHelper*>(myCoordinator.FindHelper<JsonScopeHelper>().first);

				IJsonCoordinator myMovedCoordinator(std::move(myCoordinator));
				Assert::AreEqual(static_cast<std::size_t>(1), myMovedCoordinator.NumberOfHelpers());

				const JsonScopeHelper& myScopeHelper2 = *static_cast<const JsonScopeHelper*>(myMovedCoordinator.FindHelper<JsonScopeHelper>().first);


				Assert::AreSame(myScopeHelper1, myScopeHelper2);

				const FieaGameEngine::Scope* myScope1 = myScopeHelper1.GetDataClass().GetData();
				const FieaGameEngine::Scope* myScope2 = myScopeHelper2.GetDataClass().GetData();

				Assert::AreSame(*myScope1, *myScope2); //Are they the same scope?

			}

		}
		TEST_METHOD(IJsonCoordiantorClone)
		{
			IJsonCoordinator myCoordinator;
			myCoordinator.AddHelper<JsonScopeHelper>();

			Assert::AreEqual(myCoordinator.NumberOfHelpers(), static_cast<std::size_t>(1));

			IJsonCoordinator* myClonedCoordinator = myCoordinator.Clone();
			myClonedCoordinator;
			Assert::AreEqual(myCoordinator.NumberOfHelpers(), static_cast<std::size_t>(1));

			delete myClonedCoordinator;
		}
		TEST_METHOD(IJsonCoordinatorAddHelper)
		{
			IJsonCoordinator myCoordinator;
			Assert::AreEqual(static_cast<std::size_t>(0), myCoordinator.NumberOfHelpers());

			//Adding a helper
			Assert::IsTrue(myCoordinator.AddHelper<JsonIntegerHelper>());
			Assert::AreEqual(static_cast<std::size_t>(1), myCoordinator.NumberOfHelpers());

			//Adding the same type of helper won't occur because that type of helper already exists and is managed by the coordinator.
			Assert::IsFalse(myCoordinator.AddHelper<JsonIntegerHelper>());
			Assert::AreEqual(static_cast<std::size_t>(1), myCoordinator.NumberOfHelpers());

			//Adding new type of helper will add a helper to the vector of helpers within the coordinator
			Assert::IsTrue(myCoordinator.AddHelper<JsonScopeHelper>());
			Assert::AreEqual(static_cast<std::size_t>(2), myCoordinator.NumberOfHelpers());
		}
		TEST_METHOD(IJsonCoordinatorRemoveHelper)
		{
			IJsonCoordinator myCoordinator;
			myCoordinator.AddHelper<JsonIntegerHelper>();
			myCoordinator.AddHelper<JsonScopeHelper>();

			Assert::AreEqual(static_cast<std::size_t>(2), myCoordinator.NumberOfHelpers());

			//Removing a helper
			Assert::IsTrue(myCoordinator.RemoveHelper<JsonIntegerHelper>());
			Assert::AreEqual(static_cast<std::size_t>(1), myCoordinator.NumberOfHelpers());

			//Removing a helper that does not exist in the helper vector
			Assert::IsFalse(myCoordinator.RemoveHelper<JsonIntegerHelper>());
			Assert::AreEqual(static_cast<std::size_t>(1), myCoordinator.NumberOfHelpers());

			Assert::IsTrue(myCoordinator.RemoveHelper<JsonScopeHelper>());
			Assert::AreEqual(static_cast<std::size_t>(0), myCoordinator.NumberOfHelpers());

		}
		TEST_METHOD(IJsonCoordinatorNumberOfHelpers)
		{
			//Adding the 
			IJsonCoordinator myCoordinator;
			Assert::AreEqual(static_cast<std::size_t>(0), myCoordinator.NumberOfHelpers());

			myCoordinator.AddHelper<JsonIntegerHelper>();
			Assert::AreEqual(static_cast<std::size_t>(1), myCoordinator.NumberOfHelpers());

			myCoordinator.AddHelper<JsonIntegerHelper>();
			Assert::AreEqual(static_cast<std::size_t>(1), myCoordinator.NumberOfHelpers());

			myCoordinator.AddHelper<JsonScopeHelper>();
			Assert::AreEqual(static_cast<std::size_t>(2), myCoordinator.NumberOfHelpers());

		}
		TEST_METHOD(IJsonCoordinatorParseFromFile)
		{
			
			IJsonCoordinator myCoordinator;
			myCoordinator.AddHelper<JsonScopeHelper>();
			AttributedFooFactory myFactory;

			JsonScopeHelper::Data* _myData = myCoordinator.ParseFromFile<JsonScopeHelper::Data>("Content/AttributedFoo1.json");

			const GameObject* rootGameObject = _myData->GetData()->As<GameObject>();
			Assert::AreEqual(static_cast<std::size_t>(8), rootGameObject->NumberOfItems());

			const AttributedFoo* myAttributedFoo = rootGameObject->Find("MyAttributedFoo")->Get<Scope>().As<AttributedFoo>();

			//myData.As
			Assert::IsNotNull(myAttributedFoo);
			Assert::AreEqual(100, myAttributedFoo->_externalInteger);
			Assert::AreEqual(1.1f, myAttributedFoo->_externalFloat);
			Assert::AreEqual("Alex"s, myAttributedFoo->_externalString);

			glm::vec4 A_myCompareVec(1.0f, 2.0f, 3.0f, 4.0f);
			Assert::AreEqual(A_myCompareVec, myAttributedFoo->_externalVector);

			glm::mat4 A_myCompareMat(1.0f, 2.0f, 3.0f, 4.0f, 1.0f, 2.0f, 3.0f, 4.0f, 1.0f, 2.0f, 3.0f, 4.0f, 1.0f, 2.0f, 3.0f, 4.0f);
			Assert::AreEqual(A_myCompareMat, myAttributedFoo->_externalMatrix);


			Assert::AreEqual(100, myAttributedFoo->_externalIntegerArray[0]);
			Assert::AreEqual(200, myAttributedFoo->_externalIntegerArray[1]);
			Assert::AreEqual(300, myAttributedFoo->_externalIntegerArray[2]);

			Assert::AreEqual(1.1f, myAttributedFoo->_externalFloatArray[0]);
			Assert::AreEqual(2.2f, myAttributedFoo->_externalFloatArray[1]);
			Assert::AreEqual(3.3f, myAttributedFoo->_externalFloatArray[2]);

			Assert::AreEqual("Alex"s, myAttributedFoo->_externalStringArray[0]);
			Assert::AreEqual("is"s, myAttributedFoo->_externalStringArray[1]);
			Assert::AreEqual("cool!"s, myAttributedFoo->_externalStringArray[2]);


			glm::vec4 A_myCompareVec0(1.0f, 2.0f, 3.0f, 4.0f);
			glm::vec4 A_myCompareVec1(10.0f, 20.0f, 30.0f, 40.0f);
			glm::vec4 A_myCompareVec2(100.0f, 200.0f, 300.0f, 400.0f);

			Assert::AreEqual(A_myCompareVec0, myAttributedFoo->_externalVectorArray[0]);
			Assert::AreEqual(A_myCompareVec1, myAttributedFoo->_externalVectorArray[1]);
			Assert::AreEqual(A_myCompareVec2, myAttributedFoo->_externalVectorArray[2]);

			glm::mat4 A_myCompareMat0(1.0f, 2.0f, 3.0f, 4.0f, 1.0f, 2.0f, 3.0f, 4.0f, 1.0f, 2.0f, 3.0f, 4.0f, 1.0f, 2.0f, 3.0f, 4.0f);
			glm::mat4 A_myCompareMat1(10.0f, 20.0f, 30.0f, 40.0f, 10.0f, 20.0f, 30.0f, 40.0f, 10.0f, 20.0f, 30.0f, 40.0f, 10.0f, 20.0f, 30.0f, 40.0f);
			glm::mat4 A_myCompareMat2(100.0f, 200.0f, 300.0f, 400.0f, 100.0f, 200.0f, 300.0f, 400.0f, 100.0f, 200.0f, 300.0f, 400.0f, 100.0f, 200.0f, 300.0f, 400.0f);

			Assert::AreEqual(A_myCompareMat0, myAttributedFoo->_externalMatrixArray[0]);
			Assert::AreEqual(A_myCompareMat1, myAttributedFoo->_externalMatrixArray[1]);
			Assert::AreEqual(A_myCompareMat2, myAttributedFoo->_externalMatrixArray[2]);

		}
		TEST_METHOD(IJsonParseCoordinatorParseFromString)
		{
//			std::string stringDatumIntegerScalar = 
//				R"({
//  "MyAttributedFoo": {
//    "type": "scope",
//    "value": {
//
//      "class": "AttributedFoo",
//
//      "_externalInteger": {
//        "type": "integer",
//        "value": 100
//      },
//      "_externalFloat": {
//        "type": "float",
//        "value": 1.1
//      },
//      "_externalString": {
//        "type": "string",
//        "value": "Alex"
//      }
//    }
//  }
//})";
//
//			IJsonCoordinator myCoordinator;
//			myCoordinator.AddHelper<JsonScopeHelper>();
//			AttributedFooFactory myFactory;
//
//			JsonScopeHelper::Data* _myData = myCoordinator.ParseFromString<JsonScopeHelper::Data>("stringDatumIntegerScalar");
//
//			const GameObject* rootGameObject = _myData->GetData()->As<GameObject>();
//			Assert::AreEqual(static_cast<std::size_t>(8), rootGameObject->NumberOfItems());
//
//			const AttributedFoo* myAttributedFoo = rootGameObject->Find("MyAttributedFoo")->Get<Scope>().As<AttributedFoo>();
//
//			Assert::IsNotNull(myAttributedFoo);
//			Assert::AreEqual(100, myAttributedFoo->_externalInteger);
//			Assert::AreEqual(1.1f, myAttributedFoo->_externalFloat);
//			Assert::AreEqual("Alex"s, myAttributedFoo->_externalString);

		}
		TEST_METHOD(IJsonParsingTestFindHelper)
		{
			IJsonCoordinator myCoordinator;

			IJsonCoordinator::pairType myPair = myCoordinator.FindHelper<JsonScopeHelper>();
			
			Assert::IsNull(myPair.first);
			Assert::AreEqual(static_cast<std::int32_t>(-1), myPair.second);

			myCoordinator.AddHelper<JsonScopeHelper>();

			myPair = myCoordinator.FindHelper<JsonScopeHelper>();
			Assert::IsNotNull(myPair.first);
			Assert::AreEqual(static_cast<std::int32_t>(0), myPair.second);

		}
		TEST_METHOD(ParseMultiLayeredScope)
		{
			IJsonCoordinator myCoordinator;
			myCoordinator.AddHelper<JsonScopeHelper>();

			JsonScopeHelper::Data* _myData = myCoordinator.ParseFromFile<JsonScopeHelper::Data>("Content/ScopeMultiLayer.json");
				
			Assert::IsNotNull(_myData);

			const GameObject& rootGameObject = *_myData->GetData()->As<GameObject>();

			Assert::AreEqual(static_cast<std::size_t>(8), rootGameObject.NumberOfItems());

			const Scope& myScope = rootGameObject.Find("NestedScopeLevel1")->Get<Scope>();

			const Datum* foundDatumInteger = myScope.Find("DatumIntegerScalar");
			Assert::IsNotNull(foundDatumInteger);
			Assert::AreEqual(DatumTypes::Integer, foundDatumInteger->Type());
			Assert::AreEqual(static_cast<std::size_t>(1), foundDatumInteger->Size());
			Assert::AreEqual(static_cast<std::int32_t>(100), foundDatumInteger->Get<std::int32_t>());

			const Datum* foundDatumFloat = myScope.Find("DatumFloatScalar");
			Assert::IsNotNull(foundDatumFloat);
			Assert::AreEqual(DatumTypes::Float, foundDatumFloat->Type());
			Assert::AreEqual(static_cast<std::size_t>(1), foundDatumFloat->Size());
			Assert::AreEqual(1.1f, foundDatumFloat->Get<float>());

			const Datum* foundDatumString = myScope.Find("DatumStringScalar");
			Assert::IsNotNull(foundDatumString);
			Assert::AreEqual(DatumTypes::String, foundDatumString->Type());
			Assert::AreEqual(static_cast<std::size_t>(1), foundDatumString->Size());
			Assert::AreEqual("Alex"s, foundDatumString->Get<std::string>());

			//Layer 2 Nested Scope
			const Datum* foundDatumScope2 = myScope.Find("NestedScopeLevel2");
			Assert::IsNotNull(foundDatumScope2);
			Assert::AreEqual(DatumTypes::Table, foundDatumScope2->Type());
			Assert::AreEqual(static_cast<std::size_t>(1), foundDatumScope2->Size());

			const Scope& ScopeNestedLevel2 = foundDatumScope2->Get<Scope>();
			Assert::AreEqual(static_cast<std::size_t>(4), ScopeNestedLevel2.NumberOfItems());

			foundDatumInteger = ScopeNestedLevel2.Find("DatumIntegerScalar");
			Assert::IsNotNull(foundDatumInteger);
			Assert::AreEqual(DatumTypes::Integer, foundDatumInteger->Type());
			Assert::AreEqual(static_cast<std::size_t>(1), foundDatumInteger->Size());
			Assert::AreEqual(static_cast<std::int32_t>(200), foundDatumInteger->Get<std::int32_t>());

			foundDatumFloat = ScopeNestedLevel2.Find("DatumFloatScalar");
			Assert::IsNotNull(foundDatumFloat);
			Assert::AreEqual(DatumTypes::Float, foundDatumFloat->Type());
			Assert::AreEqual(static_cast<std::size_t>(1), foundDatumFloat->Size());
			Assert::AreEqual(2.2f, foundDatumFloat->Get<float>());

			foundDatumString = ScopeNestedLevel2.Find("DatumStringScalar");
			Assert::IsNotNull(foundDatumString);
			Assert::AreEqual(DatumTypes::String, foundDatumString->Type());
			Assert::AreEqual(static_cast<std::size_t>(1), foundDatumString->Size());
			Assert::AreEqual("Soto"s, foundDatumString->Get<std::string>());

			//Layer 3 Nested Scope
			const Datum* foundDatumScope3 = ScopeNestedLevel2.Find("NestedScopeLevel3");
			Assert::IsNotNull(foundDatumScope2);
			Assert::AreEqual(DatumTypes::Table, foundDatumScope3->Type());
			Assert::AreEqual(static_cast<std::size_t>(1), foundDatumScope3->Size());

			const Scope& ScopeNestedLevel3 = foundDatumScope3->Get<Scope>();
			Assert::AreEqual(static_cast<std::size_t>(3), ScopeNestedLevel3.NumberOfItems());

			foundDatumInteger = ScopeNestedLevel3.Find("DatumIntegerArray");
			Assert::IsNotNull(foundDatumInteger);
			Assert::AreEqual(DatumTypes::Integer, foundDatumInteger->Type());
			Assert::AreEqual(static_cast<std::size_t>(3), foundDatumInteger->Size());
			Assert::AreEqual(static_cast<std::int32_t>(100), foundDatumInteger->Get<std::int32_t>(0));
			Assert::AreEqual(static_cast<std::int32_t>(200), foundDatumInteger->Get<std::int32_t>(1));
			Assert::AreEqual(static_cast<std::int32_t>(300), foundDatumInteger->Get<std::int32_t>(2));

			foundDatumFloat = ScopeNestedLevel3.Find("DatumFloatArray");
			Assert::IsNotNull(foundDatumFloat);
			Assert::AreEqual(DatumTypes::Float, foundDatumFloat->Type());
			Assert::AreEqual(static_cast<std::size_t>(3), foundDatumFloat->Size());
			Assert::AreEqual(1.1f, foundDatumFloat->Get<float>(0));
			Assert::AreEqual(2.2f, foundDatumFloat->Get<float>(1));
			Assert::AreEqual(3.3f, foundDatumFloat->Get<float>(2));

			foundDatumString = ScopeNestedLevel3.Find("DatumStringArray");
			Assert::IsNotNull(foundDatumString);
			Assert::AreEqual(DatumTypes::String, foundDatumString->Type());
			Assert::AreEqual(static_cast<std::size_t>(3), foundDatumString->Size());
			Assert::AreEqual("Alex"s, foundDatumString->Get<std::string>(0));
			Assert::AreEqual("is"s, foundDatumString->Get<std::string>(1));
			Assert::AreEqual("cool!"s, foundDatumString->Get<std::string>(2));

		}
		TEST_METHOD(ParseScopeArray) 
		{

			IJsonCoordinator myCoordinator;
			myCoordinator.AddHelper<JsonScopeHelper>();

			AttributedFooFactory myFactory;

			JsonScopeHelper::Data* _myData = myCoordinator.ParseFromFile<JsonScopeHelper::Data>("Content/ScopeArray.json");

			
			Assert::IsNotNull(_myData);
			const GameObject* rootGameObject = _myData->GetData()->As<GameObject>();
			Assert::AreEqual(static_cast<std::size_t>(8), rootGameObject->NumberOfItems());


			const AttributedFoo* myFoo = rootGameObject->Find("MyAttributedFoo")->Get<Scope>().As<AttributedFoo>();
			const Datum* foundDatumScopeArray = myFoo->Find("ScopeArray");
			Assert::IsNotNull(foundDatumScopeArray);
			Assert::AreEqual(static_cast<std::size_t>(2), foundDatumScopeArray->Size());

			const Scope& scopeArrayElement0 = foundDatumScopeArray->Get<Scope>(0);
			const Datum* foundDatumIntegerScalar = scopeArrayElement0.Find("DatumIntegerScalar");
			Assert::IsNotNull(foundDatumIntegerScalar);
			Assert::AreEqual(DatumTypes::Integer, foundDatumIntegerScalar->Type());
			Assert::AreEqual(static_cast<std::size_t>(1), foundDatumIntegerScalar->Size());
			Assert::AreEqual(static_cast<std::int32_t>(100), foundDatumIntegerScalar->Get<std::int32_t>(0));


			const Scope& scopeArrayElement1 = foundDatumScopeArray->Get<Scope>(1);
			const Datum* foundDatumFloatScalar = scopeArrayElement1.Find("DatumFloatScalar");
			Assert::IsNotNull(foundDatumFloatScalar);
			Assert::AreEqual(DatumTypes::Float, foundDatumFloatScalar->Type());

			Assert::AreEqual(static_cast<std::size_t>(1), foundDatumFloatScalar->Size());
			Assert::AreEqual(1.1f, foundDatumFloatScalar->Get<float>(0));

		}

		TEST_METHOD(ParseVectorAndMatrixFromString)
		{		 
			IJsonCoordinator myCoordinator;
			myCoordinator.AddHelper<JsonScopeHelper>();

			JsonScopeHelper::Data* _myData = myCoordinator.ParseFromFile<JsonScopeHelper::Data>("Content/VectorAndMatrixDatums.json");
			Assert::IsNotNull(_myData);

			GameObject* rootGameObject = _myData->GetData()->As<GameObject>();
			Scope& nestedScope = rootGameObject->Find("NestedScopeLevel1")->Get<Scope>();
			const Datum* foundDatumVectorScalar = nestedScope.Find("DatumVectorScalar");

			Assert::IsNotNull(foundDatumVectorScalar);
			Assert::AreEqual(DatumTypes::Vector4, foundDatumVectorScalar->Type());
			Assert::AreEqual(static_cast<std::size_t>(1), foundDatumVectorScalar->Size());

			glm::vec4 myCompareVec(1.0f, 2.0f, 3.0f, 4.0f);
			Assert::AreEqual(myCompareVec, foundDatumVectorScalar->Get<glm::vec4>(0));

			//MATRIX SCALAR
			const Datum* foundDatumMatrixScalar = nestedScope.Find("DatumMatrixScalar");
			Assert::IsNotNull(foundDatumMatrixScalar);
			Assert::AreEqual(DatumTypes::Matrix, foundDatumMatrixScalar->Type());
			Assert::AreEqual(static_cast<std::size_t>(1), foundDatumMatrixScalar->Size());

			glm::mat4 myCompareMat(1.0f, 2.0f, 3.0f, 4.0f, 1.0f, 2.0f, 3.0f, 4.0f, 1.0f, 2.0f, 3.0f, 4.0f, 1.0f, 2.0f, 3.0f, 4.0f);
			Assert::AreEqual(myCompareMat, foundDatumMatrixScalar->Get<glm::mat4>(0));

			//VECTOR ARRAY
			const Datum* foundDatumVectorArray = nestedScope.Find("DatumVectorArray");
			Assert::IsNotNull(foundDatumVectorArray);
			Assert::AreEqual(DatumTypes::Vector4, foundDatumVectorArray->Type());
			Assert::AreEqual(static_cast<std::size_t>(3), foundDatumVectorArray->Size());

			glm::vec4 myCompareVec0(1.0f, 2.0f, 3.0f, 4.0f);
			glm::vec4 myCompareVec1(10.0f, 20.0f, 30.0f, 40.0f);
			glm::vec4 myCompareVec2(100.0f, 200.0f, 300.0f, 400.0f);
			Assert::AreEqual(myCompareVec0, foundDatumVectorArray->Get<glm::vec4>(0));
			Assert::AreEqual(myCompareVec1, foundDatumVectorArray->Get<glm::vec4>(1));
			Assert::AreEqual(myCompareVec2, foundDatumVectorArray->Get<glm::vec4>(2));

			//MATRIX ARRAY
			const Datum* foundDatumMatrixArray = nestedScope.Find("DatumMatrixArray");
			Assert::IsNotNull(foundDatumMatrixArray);
			Assert::AreEqual(DatumTypes::Matrix, foundDatumMatrixArray->Type());
			Assert::AreEqual(static_cast<std::size_t>(3), foundDatumMatrixArray->Size());

			glm::mat4 myCompareMat0(1.0f, 2.0f, 3.0f, 4.0f, 1.0f, 2.0f, 3.0f, 4.0f, 1.0f, 2.0f, 3.0f, 4.0f, 1.0f, 2.0f, 3.0f, 4.0f);
			glm::mat4 myCompareMat1(10.0f, 20.0f, 30.0f, 40.0f, 10.0f, 20.0f, 30.0f, 40.0f, 10.0f, 20.0f, 30.0f, 40.0f, 10.0f, 20.0f, 30.0f, 40.0f);
			glm::mat4 myCompareMat2(100.0f, 200.0f, 300.0f, 400.0f, 100.0f, 200.0f, 300.0f, 400.0f, 100.0f, 200.0f, 300.0f, 400.0f, 100.0f, 200.0f, 300.0f, 400.0f);
			Assert::AreEqual(myCompareMat0, foundDatumMatrixArray->Get<glm::mat4>(0));
			Assert::AreEqual(myCompareMat1, foundDatumMatrixArray->Get<glm::mat4>(1));
			Assert::AreEqual(myCompareMat2, foundDatumMatrixArray->Get<glm::mat4>(2));

		}
		TEST_METHOD(ParseComplexScope)
		{
			IJsonCoordinator myCoordinator;
			myCoordinator.AddHelper<JsonScopeHelper>();

			JsonScopeHelper::Data* _myData = myCoordinator.ParseFromFile<JsonScopeHelper::Data>("Content/ScopeComplex.json");

			Assert::IsNotNull(_myData);
			const GameObject* rootGameObject = _myData->GetData()->As<GameObject>();
			Assert::AreEqual(static_cast<std::size_t>(8), rootGameObject->NumberOfItems());

			/************************************************************************/
			const Scope& myScope = rootGameObject->Find("Layer 1-1: A Scope: NOT Array")->Get<Scope>();

			//LAYER 1: FOUND DATUM INTEGER SCALAR
			const Datum* foundDatumIntegerScalar2_1 = myScope.Find("DatumIntegerScalar2_1");
			Assert::IsNotNull(foundDatumIntegerScalar2_1);
			Assert::AreEqual(DatumTypes::Integer, foundDatumIntegerScalar2_1->Type());
			Assert::AreEqual(static_cast<std::size_t>(1), foundDatumIntegerScalar2_1->Size());
			Assert::AreEqual(static_cast<std::int32_t>(100), foundDatumIntegerScalar2_1->Get<std::int32_t>(0));

			//LAYER 1: FOUND DATUM FLOAT SCALAR
			const Datum* foundDatumFloatScalar2_1 = myScope.Find("DatumFloatScalar2_1");
			Assert::IsNotNull(foundDatumFloatScalar2_1);
			Assert::AreEqual(DatumTypes::Float, foundDatumFloatScalar2_1->Type());
			Assert::AreEqual(static_cast<std::size_t>(1), foundDatumFloatScalar2_1->Size());
			Assert::AreEqual(static_cast<float>(1.1f), foundDatumFloatScalar2_1->Get<float>(0));


			//LAYER 1: FOUND DATUM STRING SCALAR
			const Datum* foundDatumStringScalar2_1 = myScope.Find("DatumStringScalar2_1");
			Assert::IsNotNull(foundDatumStringScalar2_1);
			Assert::AreEqual(DatumTypes::String, foundDatumStringScalar2_1->Type());
			Assert::AreEqual(static_cast<std::size_t>(1), foundDatumStringScalar2_1->Size());
			Assert::AreEqual("Alex"s, foundDatumStringScalar2_1->Get<std::string>(0));

			/************************************************************************/

			//LAYER 2: FOUND SCOPE ARRAY
			const Datum* foundScopeArray2_1 = myScope.Find("Layer 2-1: A Scope: Is Array");
			Assert::IsNotNull(foundScopeArray2_1);
			Assert::AreEqual(DatumTypes::Table, foundScopeArray2_1->Type());
			Assert::AreEqual(static_cast<std::size_t>(4), foundScopeArray2_1->Size());

			//LAYER 2: SCOPE ARRAY INDEX 0
			const Scope& myScopeArray2_1_0 = foundScopeArray2_1->Get<Scope>(0);
			Assert::AreEqual(static_cast<std::size_t>(1), myScopeArray2_1_0.NumberOfItems());
			const Datum* foundDatumIntegerArray3_1 = myScopeArray2_1_0.Find("DatumIntegerArray3_1");
			Assert::IsNotNull(foundDatumIntegerArray3_1);
			Assert::AreEqual(DatumTypes::Integer, foundDatumIntegerArray3_1->Type());
			Assert::AreEqual(static_cast<std::size_t>(3), foundDatumIntegerArray3_1->Size());

			Assert::AreEqual(static_cast<std::int32_t>(100), foundDatumIntegerArray3_1->Get<std::int32_t>(0));
			Assert::AreEqual(static_cast<std::int32_t>(200), foundDatumIntegerArray3_1->Get<std::int32_t>(1));
			Assert::AreEqual(static_cast<std::int32_t>(300), foundDatumIntegerArray3_1->Get<std::int32_t>(2));


			//LAYER 2: SCOPE ARRAY INDEX 1
			const Scope& myScopeArray2_1_1 = foundScopeArray2_1->Get<Scope>(1);
			Assert::AreEqual(static_cast<std::size_t>(1), myScopeArray2_1_1.NumberOfItems());
			const Datum* foundDatumFloatArray3_1 = myScopeArray2_1_1.Find("DatumFloatArray3_1");
			Assert::IsNotNull(foundDatumFloatArray3_1);
			Assert::AreEqual(DatumTypes::Float, foundDatumFloatArray3_1->Type());
			Assert::AreEqual(static_cast<std::size_t>(3), foundDatumFloatArray3_1->Size());

			Assert::AreEqual(1.1f, foundDatumFloatArray3_1->Get<float>(0));
			Assert::AreEqual(2.2f, foundDatumFloatArray3_1->Get<float>(1));
			Assert::AreEqual(3.3f, foundDatumFloatArray3_1->Get<float>(2));

			//LAYER 2: SCOPE ARRAY INDEX 2
			const Scope& myScopeArray2_1_2 = foundScopeArray2_1->Get<Scope>(2);
			Assert::AreEqual(static_cast<std::size_t>(1), myScopeArray2_1_2.NumberOfItems());
			const Datum* foundDatumStringArray3_1 = myScopeArray2_1_2.Find("DatumStringArray3_1");
			Assert::IsNotNull(foundDatumStringArray3_1);
			Assert::AreEqual(DatumTypes::String, foundDatumStringArray3_1->Type());
			Assert::AreEqual(static_cast<std::size_t>(3), foundDatumStringArray3_1->Size());

			Assert::AreEqual("Alex"s, foundDatumStringArray3_1->Get<std::string>(0));
			Assert::AreEqual("is"s, foundDatumStringArray3_1->Get<std::string>(1));
			Assert::AreEqual("cool!"s, foundDatumStringArray3_1->Get<std::string>(2));

			/************************************************************************/

			//LAYER 3: SCOPE ARRAY INDEX 3
			const Scope& myScopeArray2_1_3 = foundScopeArray2_1->Get<Scope>(3);
			Assert::AreEqual(static_cast<std::size_t>(1), myScopeArray2_1_3.NumberOfItems());
			const Datum* foundScopeArray3_1 = myScopeArray2_1_3.Find("Layer 3-1: A Scope:: Is Array");
			Assert::IsNotNull(foundScopeArray3_1);
			Assert::AreEqual(DatumTypes::Table, foundScopeArray3_1->Type());
			Assert::AreEqual(static_cast<std::size_t>(3), foundScopeArray3_1->Size());

			//LAYER 3: SCOPE ARRAY INDEX 0
			const Scope& myScopeArray3_1_0 = foundScopeArray3_1->Get<Scope>(0);
			Assert::AreEqual(static_cast<std::size_t>(1), myScopeArray3_1_0.NumberOfItems());
			const Datum* foundDatumIntegerArray4_1 = myScopeArray3_1_0.Find("DatumIntegerArray4_1");
			Assert::IsNotNull(foundDatumIntegerArray4_1);
			Assert::AreEqual(DatumTypes::Integer, foundDatumIntegerArray4_1->Type());
			Assert::AreEqual(static_cast<std::size_t>(3), foundDatumIntegerArray4_1->Size());

			Assert::AreEqual(static_cast<std::int32_t>(100), foundDatumIntegerArray4_1->Get<std::int32_t>(0));
			Assert::AreEqual(static_cast<std::int32_t>(200), foundDatumIntegerArray4_1->Get<std::int32_t>(1));
			Assert::AreEqual(static_cast<std::int32_t>(300), foundDatumIntegerArray4_1->Get<std::int32_t>(2));


			//LAYER 2: SCOPE ARRAY INDEX 1
			const Scope& myScopeArray3_1_1 = foundScopeArray3_1->Get<Scope>(1);
			Assert::AreEqual(static_cast<std::size_t>(1), myScopeArray3_1_1.NumberOfItems());
			const Datum* foundDatumFloatArray4_1 = myScopeArray3_1_1.Find("DatumFloatArray4_1");
			Assert::IsNotNull(foundDatumFloatArray4_1);
			Assert::AreEqual(DatumTypes::Float, foundDatumFloatArray4_1->Type());
			Assert::AreEqual(static_cast<std::size_t>(3), foundDatumFloatArray4_1->Size());

			Assert::AreEqual(1.1f, foundDatumFloatArray4_1->Get<float>(0));
			Assert::AreEqual(2.2f, foundDatumFloatArray4_1->Get<float>(1));
			Assert::AreEqual(3.3f, foundDatumFloatArray4_1->Get<float>(2));

			//LAYER 2: SCOPE ARRAY INDEX 2
			const Scope& myScopeArray3_1_2 = foundScopeArray3_1->Get<Scope>(2);
			Assert::AreEqual(static_cast<std::size_t>(1), myScopeArray3_1_2.NumberOfItems());
			const Datum* foundDatumStringArray4_1 = myScopeArray3_1_2.Find("DatumStringArray4_1");
			Assert::IsNotNull(foundDatumStringArray4_1);
			Assert::AreEqual(DatumTypes::String, foundDatumStringArray4_1->Type());
			Assert::AreEqual(static_cast<std::size_t>(3), foundDatumStringArray4_1->Size());

			Assert::AreEqual("Alex"s, foundDatumStringArray4_1->Get<std::string>(0));
			Assert::AreEqual("is"s, foundDatumStringArray4_1->Get<std::string>(1));
			Assert::AreEqual("cool!"s, foundDatumStringArray4_1->Get<std::string>(2));

			/************************************************************************/
			                                                                     
			//LAYER 2: FOUND SCOPE
			const Datum* foundScopeNotArray2_2 = myScope.Find("Layer 2-2: A Scope: NOT Array");
			Assert::IsNotNull(foundScopeNotArray2_2);
			Assert::AreEqual(DatumTypes::Table, foundScopeNotArray2_2->Type());
			Assert::AreEqual(static_cast<std::size_t>(1), foundScopeNotArray2_2->Size());

			const Scope& myScopeNotArray2_2 = foundScopeNotArray2_2->Get<Scope>();

			//LAYER 2: FOUND DATUM INTEGER SCALAR
			const Datum* foundDatumIntegerScalar3_2_1 = myScopeNotArray2_2.Find("DatumIntegerScalar3_2_1");
			Assert::IsNotNull(foundDatumIntegerScalar3_2_1);
			Assert::AreEqual(DatumTypes::Integer, foundDatumIntegerScalar3_2_1->Type());
			Assert::AreEqual(static_cast<std::size_t>(1), foundDatumIntegerScalar3_2_1->Size());
			Assert::AreEqual(static_cast<std::int32_t>(100), foundDatumIntegerScalar3_2_1->Get<std::int32_t>(0));

			//LAYER 2: FOUND DATUM FLOAT SCALAR
			const Datum* foundDatumFloatScalar3_2_1 = myScopeNotArray2_2.Find("DatumFloatArray3_2_1");
			Assert::IsNotNull(foundDatumFloatScalar3_2_1);
			Assert::AreEqual(DatumTypes::Float, foundDatumFloatScalar3_2_1->Type());
			Assert::AreEqual(static_cast<std::size_t>(1), foundDatumFloatScalar3_2_1->Size());
			Assert::AreEqual(1.1f, foundDatumFloatScalar3_2_1->Get<float>(0));

			//LAYER 2: FOUND DATUM STRING SCALAR
			const Datum* foundDatumStringScalar3_2_1 = myScopeNotArray2_2.Find("DatumStringScalar3_2_1");
			Assert::IsNotNull(foundDatumStringScalar3_2_1);
			Assert::AreEqual(DatumTypes::String, foundDatumStringScalar3_2_1->Type());
			Assert::AreEqual(static_cast<std::size_t>(1), foundDatumStringScalar3_2_1->Size());
			Assert::AreEqual("Alex"s, foundDatumStringScalar3_2_1->Get<std::string>(0));

			//LAYER 2: FOUND DATUM VECTOR SCALAR
			const Datum* foundDatumVectorScalar3_2_1 = myScopeNotArray2_2.Find("DatumVectorScalar3_2_1");
			Assert::IsNotNull(foundDatumVectorScalar3_2_1);
			Assert::AreEqual(DatumTypes::Vector4, foundDatumVectorScalar3_2_1->Type());
			Assert::AreEqual(static_cast<std::size_t>(1), foundDatumVectorScalar3_2_1->Size());

			glm::vec4 myCompareVec(1.0f, 2.0f, 3.0f, 4.0f);
			Assert::AreEqual(myCompareVec, foundDatumVectorScalar3_2_1->Get<glm::vec4>());

			//LAYER 2: FOUND DATUM MATRIX SCALAR
			const Datum* foundDatumMatrixScalar3_2_1 = myScopeNotArray2_2.Find("DatumMatrixScalar3_2_1");
			Assert::IsNotNull(foundDatumMatrixScalar3_2_1);
			Assert::AreEqual(DatumTypes::Matrix, foundDatumMatrixScalar3_2_1->Type());
			Assert::AreEqual(static_cast<std::size_t>(1), foundDatumMatrixScalar3_2_1->Size());

			glm::mat4 myCompareMat(1.0f, 2.0f, 3.0f, 4.0f, 1.0f, 2.0f, 3.0f, 4.0f, 1.0f, 2.0f, 3.0f, 4.0f, 1.0f, 2.0f, 3.0f, 4.0f);
			Assert::AreEqual(myCompareMat, foundDatumMatrixScalar3_2_1->Get<glm::mat4>());

			//LAYER 2: FOUND DATUM VECTOR ARRAY
			const Datum* foundDatumVectorArray3_2_1 = myScopeNotArray2_2.Find("DatumVectorArray3_2_1");
			Assert::IsNotNull(foundDatumVectorArray3_2_1);
			Assert::AreEqual(DatumTypes::Vector4, foundDatumVectorArray3_2_1->Type());
			Assert::AreEqual(static_cast<std::size_t>(3), foundDatumVectorArray3_2_1->Size());

			glm::vec4 myCompareVec0(1.0f, 2.0f, 3.0f, 4.0f);
			glm::vec4 myCompareVec1(10.0f, 20.0f, 30.0f, 40.0f);
			glm::vec4 myCompareVec2(100.0f, 200.0f, 300.0f, 400.0f);
			Assert::AreEqual(myCompareVec0, foundDatumVectorArray3_2_1->Get<glm::vec4>(0));
			Assert::AreEqual(myCompareVec1, foundDatumVectorArray3_2_1->Get<glm::vec4>(1));
			Assert::AreEqual(myCompareVec2, foundDatumVectorArray3_2_1->Get<glm::vec4>(2));

			//LAYER 2: FOUND DATUM MATRIX ARRAY
			const Datum* foundDatumMatrixArray3_2_1 = myScopeNotArray2_2.Find("DatumMatrixArray3_1");
			Assert::IsNotNull(foundDatumMatrixArray3_2_1);
			Assert::AreEqual(DatumTypes::Matrix, foundDatumMatrixArray3_2_1->Type());
			Assert::AreEqual(static_cast<std::size_t>(3), foundDatumMatrixArray3_2_1->Size());

			glm::mat4 myCompareMat0(1.0f, 2.0f, 3.0f, 4.0f, 1.0f, 2.0f, 3.0f, 4.0f, 1.0f, 2.0f, 3.0f, 4.0f, 1.0f, 2.0f, 3.0f, 4.0f);
			glm::mat4 myCompareMat1(10.0f, 20.0f, 30.0f, 40.0f, 10.0f, 20.0f, 30.0f, 40.0f, 10.0f, 20.0f, 30.0f, 40.0f, 10.0f, 20.0f, 30.0f, 40.0f);
			glm::mat4 myCompareMat2(100.0f, 200.0f, 300.0f, 400.0f, 100.0f, 200.0f, 300.0f, 400.0f, 100.0f, 200.0f, 300.0f, 400.0f, 100.0f, 200.0f, 300.0f, 400.0f);
			Assert::AreEqual(myCompareMat0, foundDatumMatrixArray3_2_1->Get<glm::mat4>(0));
			Assert::AreEqual(myCompareMat1, foundDatumMatrixArray3_2_1->Get<glm::mat4>(1));
			Assert::AreEqual(myCompareMat2, foundDatumMatrixArray3_2_1->Get<glm::mat4>(2));

		}
		TEST_METHOD(ParseAttributedClass)
		{

			{
				IJsonCoordinator myCoordinator;
				myCoordinator.AddHelper<JsonScopeHelper>();
				AttributedFooFactory myFactory;

				JsonScopeHelper::Data* _myData = myCoordinator.ParseFromFile<JsonScopeHelper::Data>("Content/AttributedFoo1.json");

				const Scope* myScopePtr = _myData->GetData();
				const GameObject& rootGameObject = *myScopePtr->As<GameObject>();
				const AttributedFoo* myAttributedFoo = rootGameObject.Find("MyAttributedFoo")->Get<Scope>().As<AttributedFoo>();

				Assert::AreEqual(static_cast<std::size_t>(8), rootGameObject.NumberOfItems());

				//myData.As
				Assert::IsNotNull(myAttributedFoo);
				Assert::AreEqual(100, myAttributedFoo->_externalInteger);
				Assert::AreEqual(1.1f, myAttributedFoo->_externalFloat);
				Assert::AreEqual("Alex"s, myAttributedFoo->_externalString);

				glm::vec4 A_myCompareVec(1.0f, 2.0f, 3.0f, 4.0f);
				Assert::AreEqual(A_myCompareVec, myAttributedFoo->_externalVector);

				glm::mat4 A_myCompareMat(1.0f, 2.0f, 3.0f, 4.0f, 1.0f, 2.0f, 3.0f, 4.0f, 1.0f, 2.0f, 3.0f, 4.0f, 1.0f, 2.0f, 3.0f, 4.0f);
				Assert::AreEqual(A_myCompareMat, myAttributedFoo->_externalMatrix);

				
				Assert::AreEqual(100, myAttributedFoo->_externalIntegerArray[0]);
				Assert::AreEqual(200, myAttributedFoo->_externalIntegerArray[1]);
				Assert::AreEqual(300, myAttributedFoo->_externalIntegerArray[2]);

				Assert::AreEqual(1.1f, myAttributedFoo->_externalFloatArray[0]);
				Assert::AreEqual(2.2f, myAttributedFoo->_externalFloatArray[1]);
				Assert::AreEqual(3.3f, myAttributedFoo->_externalFloatArray[2]);

				Assert::AreEqual("Alex"s, myAttributedFoo->_externalStringArray[0]);
				Assert::AreEqual("is"s, myAttributedFoo->_externalStringArray[1]);
				Assert::AreEqual("cool!"s, myAttributedFoo->_externalStringArray[2]);

				
				glm::vec4 A_myCompareVec0(1.0f, 2.0f, 3.0f, 4.0f);
				glm::vec4 A_myCompareVec1(10.0f, 20.0f, 30.0f, 40.0f);
				glm::vec4 A_myCompareVec2(100.0f, 200.0f, 300.0f, 400.0f);

				Assert::AreEqual(A_myCompareVec0, myAttributedFoo->_externalVectorArray[0]);
				Assert::AreEqual(A_myCompareVec1, myAttributedFoo->_externalVectorArray[1]);
				Assert::AreEqual(A_myCompareVec2, myAttributedFoo->_externalVectorArray[2]);

				glm::mat4 A_myCompareMat0(1.0f, 2.0f, 3.0f, 4.0f, 1.0f, 2.0f, 3.0f, 4.0f, 1.0f, 2.0f, 3.0f, 4.0f, 1.0f, 2.0f, 3.0f, 4.0f);
				glm::mat4 A_myCompareMat1(10.0f, 20.0f, 30.0f, 40.0f, 10.0f, 20.0f, 30.0f, 40.0f, 10.0f, 20.0f, 30.0f, 40.0f, 10.0f, 20.0f, 30.0f, 40.0f);
				glm::mat4 A_myCompareMat2(100.0f, 200.0f, 300.0f, 400.0f, 100.0f, 200.0f, 300.0f, 400.0f, 100.0f, 200.0f, 300.0f, 400.0f, 100.0f, 200.0f, 300.0f, 400.0f);

				Assert::AreEqual(A_myCompareMat0, myAttributedFoo->_externalMatrixArray[0]);
				Assert::AreEqual(A_myCompareMat1, myAttributedFoo->_externalMatrixArray[1]);
				Assert::AreEqual(A_myCompareMat2, myAttributedFoo->_externalMatrixArray[2]);


			}

			{
				IJsonCoordinator myCoordinator;
				myCoordinator.AddHelper<JsonScopeHelper>();
				AttributedFooFactory myFactory;

				JsonScopeHelper::Data* _myData = myCoordinator.ParseFromFile<JsonScopeHelper::Data>("Content/AttributedFooWithAuxiliaryAttributes.json");

				const Scope* myScopePtr = _myData->GetData();
				const GameObject& rootGameObject = *myScopePtr->As<GameObject>();
				const AttributedFoo* A_myAttributedFoo = rootGameObject.Find("A-AttributedFoo")->Get<Scope>().As<AttributedFoo>();

				Assert::AreEqual(static_cast<std::size_t>(8), rootGameObject.NumberOfItems());

				Assert::IsNotNull(A_myAttributedFoo);
				Assert::AreEqual(100, A_myAttributedFoo->_externalInteger);
				Assert::AreEqual(1.1f, A_myAttributedFoo->_externalFloat);
				Assert::AreEqual("Alex"s, A_myAttributedFoo->_externalString);

				glm::vec4 A_myCompareVec(1.0f, 2.0f, 3.0f, 4.0f);
				Assert::AreEqual(A_myCompareVec, A_myAttributedFoo->_externalVector);

				glm::mat4 A_myCompareMat(1.0f, 2.0f, 3.0f, 4.0f, 1.0f, 2.0f, 3.0f, 4.0f, 1.0f, 2.0f, 3.0f, 4.0f, 1.0f, 2.0f, 3.0f, 4.0f);
				Assert::AreEqual(A_myCompareMat, A_myAttributedFoo->_externalMatrix);


				Assert::AreEqual(100, A_myAttributedFoo->_externalIntegerArray[0]);
				Assert::AreEqual(200, A_myAttributedFoo->_externalIntegerArray[1]);
				Assert::AreEqual(300, A_myAttributedFoo->_externalIntegerArray[2]);

				Assert::AreEqual(1.1f, A_myAttributedFoo->_externalFloatArray[0]);
				Assert::AreEqual(2.2f, A_myAttributedFoo->_externalFloatArray[1]);
				Assert::AreEqual(3.3f, A_myAttributedFoo->_externalFloatArray[2]);

				Assert::AreEqual("Alex"s, A_myAttributedFoo->_externalStringArray[0]);
				Assert::AreEqual("is"s, A_myAttributedFoo->_externalStringArray[1]);
				Assert::AreEqual("cool!"s, A_myAttributedFoo->_externalStringArray[2]);


				glm::vec4 A_myCompareVec0(1.0f, 2.0f, 3.0f, 4.0f);
				glm::vec4 A_myCompareVec1(10.0f, 20.0f, 30.0f, 40.0f);
				glm::vec4 A_myCompareVec2(100.0f, 200.0f, 300.0f, 400.0f);

				Assert::AreEqual(A_myCompareVec0, A_myAttributedFoo->_externalVectorArray[0]);
				Assert::AreEqual(A_myCompareVec1, A_myAttributedFoo->_externalVectorArray[1]);
				Assert::AreEqual(A_myCompareVec2, A_myAttributedFoo->_externalVectorArray[2]);

				glm::mat4 A_myCompareMat0(1.0f, 2.0f, 3.0f, 4.0f, 1.0f, 2.0f, 3.0f, 4.0f, 1.0f, 2.0f, 3.0f, 4.0f, 1.0f, 2.0f, 3.0f, 4.0f);
				glm::mat4 A_myCompareMat1(10.0f, 20.0f, 30.0f, 40.0f, 10.0f, 20.0f, 30.0f, 40.0f, 10.0f, 20.0f, 30.0f, 40.0f, 10.0f, 20.0f, 30.0f, 40.0f);
				glm::mat4 A_myCompareMat2(100.0f, 200.0f, 300.0f, 400.0f, 100.0f, 200.0f, 300.0f, 400.0f, 100.0f, 200.0f, 300.0f, 400.0f, 100.0f, 200.0f, 300.0f, 400.0f);

				Assert::AreEqual(A_myCompareMat0, A_myAttributedFoo->_externalMatrixArray[0]);
				Assert::AreEqual(A_myCompareMat1, A_myAttributedFoo->_externalMatrixArray[1]);
				Assert::AreEqual(A_myCompareMat2, A_myAttributedFoo->_externalMatrixArray[2]);
				
				const AttributedFoo* B_myAttributedFoo = A_myAttributedFoo->Find("B-AttributedFoo")->Get<Scope>().As<AttributedFoo>();

				Assert::IsNotNull(B_myAttributedFoo);
				Assert::AreEqual(100, B_myAttributedFoo->_externalInteger);
				Assert::AreEqual(1.1f, B_myAttributedFoo->_externalFloat);
				Assert::AreEqual("Alex"s, B_myAttributedFoo->_externalString);

				glm::vec4 B_myCompareVec(1.0f, 2.0f, 3.0f, 4.0f);
				Assert::AreEqual(B_myCompareVec, B_myAttributedFoo->_externalVector);

				glm::mat4 B_myCompareMat(1.0f, 2.0f, 3.0f, 4.0f, 1.0f, 2.0f, 3.0f, 4.0f, 1.0f, 2.0f, 3.0f, 4.0f, 1.0f, 2.0f, 3.0f, 4.0f);
				Assert::AreEqual(B_myCompareMat, B_myAttributedFoo->_externalMatrix);


				Assert::AreEqual(100, B_myAttributedFoo->_externalIntegerArray[0]);
				Assert::AreEqual(200, B_myAttributedFoo->_externalIntegerArray[1]);
				Assert::AreEqual(300, B_myAttributedFoo->_externalIntegerArray[2]);

				Assert::AreEqual(1.1f, B_myAttributedFoo->_externalFloatArray[0]);
				Assert::AreEqual(2.2f, B_myAttributedFoo->_externalFloatArray[1]);
				Assert::AreEqual(3.3f, B_myAttributedFoo->_externalFloatArray[2]);

				Assert::AreEqual("Alex"s, B_myAttributedFoo->_externalStringArray[0]);
				Assert::AreEqual("is"s, B_myAttributedFoo->_externalStringArray[1]);
				Assert::AreEqual("cool!"s, B_myAttributedFoo->_externalStringArray[2]);


				glm::vec4 B_myCompareVec0(1.0f, 2.0f, 3.0f, 4.0f);
				glm::vec4 B_myCompareVec1(10.0f, 20.0f, 30.0f, 40.0f);
				glm::vec4 B_myCompareVec2(100.0f, 200.0f, 300.0f, 400.0f);

				Assert::AreEqual(B_myCompareVec0, B_myAttributedFoo->_externalVectorArray[0]);
				Assert::AreEqual(B_myCompareVec1, B_myAttributedFoo->_externalVectorArray[1]);
				Assert::AreEqual(B_myCompareVec2, B_myAttributedFoo->_externalVectorArray[2]);

				glm::mat4 B_myCompareMat0(1.0f, 2.0f, 3.0f, 4.0f, 1.0f, 2.0f, 3.0f, 4.0f, 1.0f, 2.0f, 3.0f, 4.0f, 1.0f, 2.0f, 3.0f, 4.0f);
				glm::mat4 B_myCompareMat1(10.0f, 20.0f, 30.0f, 40.0f, 10.0f, 20.0f, 30.0f, 40.0f, 10.0f, 20.0f, 30.0f, 40.0f, 10.0f, 20.0f, 30.0f, 40.0f);
				glm::mat4 B_myCompareMat2(100.0f, 200.0f, 300.0f, 400.0f, 100.0f, 200.0f, 300.0f, 400.0f, 100.0f, 200.0f, 300.0f, 400.0f, 100.0f, 200.0f, 300.0f, 400.0f);

				Assert::AreEqual(B_myCompareMat0, B_myAttributedFoo->_externalMatrixArray[0]);
				Assert::AreEqual(B_myCompareMat1, B_myAttributedFoo->_externalMatrixArray[1]);
				Assert::AreEqual(B_myCompareMat2, B_myAttributedFoo->_externalMatrixArray[2]);

			}

		}
		TEST_METHOD(DerivedAttributedFooParseTest)
		{
			{
				IJsonCoordinator myCoordinator;
				myCoordinator.AddHelper<JsonScopeHelper>();
				DerivedAttributedFooFactory myFactory;

				JsonScopeHelper::Data* _myData = myCoordinator.ParseFromFile<JsonScopeHelper::Data>("Content/DerivedAttributedFoo.json");

				const Scope* myScopePtr = _myData->GetData();
				const GameObject& rootGameObject = *myScopePtr->As<GameObject>();

				Assert::AreEqual(static_cast<std::size_t>(8), rootGameObject.NumberOfItems());

				//myData.As
				const DerivedAttributedFoo* myDerivedAttributedFoo = rootGameObject.Find("MyAttributedFoo")->Get<Scope>().As<DerivedAttributedFoo>();
				Assert::IsNotNull(myDerivedAttributedFoo);
				Assert::AreEqual(100, myDerivedAttributedFoo->_externalInteger);
				Assert::AreEqual(1.1f, myDerivedAttributedFoo->_externalFloat);
				Assert::AreEqual("Alex"s, myDerivedAttributedFoo->_externalString);

				glm::vec4 A_myCompareVec(1.0f, 2.0f, 3.0f, 4.0f);
				Assert::AreEqual(A_myCompareVec, myDerivedAttributedFoo->_externalVector);

				glm::mat4 A_myCompareMat(1.0f, 2.0f, 3.0f, 4.0f, 1.0f, 2.0f, 3.0f, 4.0f, 1.0f, 2.0f, 3.0f, 4.0f, 1.0f, 2.0f, 3.0f, 4.0f);
				Assert::AreEqual(A_myCompareMat, myDerivedAttributedFoo->_externalMatrix);


				Assert::AreEqual(100, myDerivedAttributedFoo->_externalIntegerArray[0]);
				Assert::AreEqual(200, myDerivedAttributedFoo->_externalIntegerArray[1]);
				Assert::AreEqual(300, myDerivedAttributedFoo->_externalIntegerArray[2]);

				Assert::AreEqual(1.1f, myDerivedAttributedFoo->_externalFloatArray[0]);
				Assert::AreEqual(2.2f, myDerivedAttributedFoo->_externalFloatArray[1]);
				Assert::AreEqual(3.3f, myDerivedAttributedFoo->_externalFloatArray[2]);

				Assert::AreEqual("Alex"s, myDerivedAttributedFoo->_externalStringArray[0]);
				Assert::AreEqual("is"s, myDerivedAttributedFoo->_externalStringArray[1]);
				Assert::AreEqual("cool!"s, myDerivedAttributedFoo->_externalStringArray[2]);


				glm::vec4 A_myCompareVec0(1.0f, 2.0f, 3.0f, 4.0f);
				glm::vec4 A_myCompareVec1(10.0f, 20.0f, 30.0f, 40.0f);
				glm::vec4 A_myCompareVec2(100.0f, 200.0f, 300.0f, 400.0f);

				Assert::AreEqual(A_myCompareVec0, myDerivedAttributedFoo->_externalVectorArray[0]);
				Assert::AreEqual(A_myCompareVec1, myDerivedAttributedFoo->_externalVectorArray[1]);
				Assert::AreEqual(A_myCompareVec2, myDerivedAttributedFoo->_externalVectorArray[2]);

				glm::mat4 A_myCompareMat0(1.0f, 2.0f, 3.0f, 4.0f, 1.0f, 2.0f, 3.0f, 4.0f, 1.0f, 2.0f, 3.0f, 4.0f, 1.0f, 2.0f, 3.0f, 4.0f);
				glm::mat4 A_myCompareMat1(10.0f, 20.0f, 30.0f, 40.0f, 10.0f, 20.0f, 30.0f, 40.0f, 10.0f, 20.0f, 30.0f, 40.0f, 10.0f, 20.0f, 30.0f, 40.0f);
				glm::mat4 A_myCompareMat2(100.0f, 200.0f, 300.0f, 400.0f, 100.0f, 200.0f, 300.0f, 400.0f, 100.0f, 200.0f, 300.0f, 400.0f, 100.0f, 200.0f, 300.0f, 400.0f);

				Assert::AreEqual(A_myCompareMat0, myDerivedAttributedFoo->_externalMatrixArray[0]);
				Assert::AreEqual(A_myCompareMat1, myDerivedAttributedFoo->_externalMatrixArray[1]);
				Assert::AreEqual(A_myCompareMat2, myDerivedAttributedFoo->_externalMatrixArray[2]);


			}

		}
		TEST_METHOD(SuperDerivedAttributedFooParseTest)
		{
			{
				IJsonCoordinator myCoordinator;
				myCoordinator.AddHelper<JsonScopeHelper>();
				SuperDerivedAttributedFooFactory myFactory;

				JsonScopeHelper::Data* _myData = myCoordinator.ParseFromFile<JsonScopeHelper::Data>("Content/SuperDerivedAttributedFoo.json");

				const Scope* myScopePtr = _myData->GetData();
				const GameObject& rootGameObject = *myScopePtr->As< GameObject>();
				Assert::AreEqual(static_cast<std::size_t>(8), rootGameObject.NumberOfItems());

				const SuperDerivedAttributedFoo* mySuperDerivedAttributedFoo = rootGameObject.Find("MyAttributedFoo")->Get<Scope>().As<SuperDerivedAttributedFoo>();
				
				Assert::IsNotNull(mySuperDerivedAttributedFoo);
				Assert::AreEqual(100, mySuperDerivedAttributedFoo->_externalInteger);
				Assert::AreEqual(1.1f, mySuperDerivedAttributedFoo->_externalFloat);
				Assert::AreEqual("Alex"s, mySuperDerivedAttributedFoo->_externalString);

				glm::vec4 A_myCompareVec(1.0f, 2.0f, 3.0f, 4.0f);
				Assert::AreEqual(A_myCompareVec, mySuperDerivedAttributedFoo->_externalVector);

				glm::mat4 A_myCompareMat(1.0f, 2.0f, 3.0f, 4.0f, 1.0f, 2.0f, 3.0f, 4.0f, 1.0f, 2.0f, 3.0f, 4.0f, 1.0f, 2.0f, 3.0f, 4.0f);
				Assert::AreEqual(A_myCompareMat, mySuperDerivedAttributedFoo->_externalMatrix);


				Assert::AreEqual(100, mySuperDerivedAttributedFoo->_externalIntegerArray[0]);
				Assert::AreEqual(200, mySuperDerivedAttributedFoo->_externalIntegerArray[1]);
				Assert::AreEqual(300, mySuperDerivedAttributedFoo->_externalIntegerArray[2]);

				Assert::AreEqual(1.1f, mySuperDerivedAttributedFoo->_externalFloatArray[0]);
				Assert::AreEqual(2.2f, mySuperDerivedAttributedFoo->_externalFloatArray[1]);
				Assert::AreEqual(3.3f, mySuperDerivedAttributedFoo->_externalFloatArray[2]);

				Assert::AreEqual("Alex"s, mySuperDerivedAttributedFoo->_externalStringArray[0]);
				Assert::AreEqual("is"s, mySuperDerivedAttributedFoo->_externalStringArray[1]);
				Assert::AreEqual("cool!"s, mySuperDerivedAttributedFoo->_externalStringArray[2]);


				glm::vec4 A_myCompareVec0(1.0f, 2.0f, 3.0f, 4.0f);
				glm::vec4 A_myCompareVec1(10.0f, 20.0f, 30.0f, 40.0f);
				glm::vec4 A_myCompareVec2(100.0f, 200.0f, 300.0f, 400.0f);

				Assert::AreEqual(A_myCompareVec0, mySuperDerivedAttributedFoo->_externalVectorArray[0]);
				Assert::AreEqual(A_myCompareVec1, mySuperDerivedAttributedFoo->_externalVectorArray[1]);
				Assert::AreEqual(A_myCompareVec2, mySuperDerivedAttributedFoo->_externalVectorArray[2]);

				glm::mat4 A_myCompareMat0(1.0f, 2.0f, 3.0f, 4.0f, 1.0f, 2.0f, 3.0f, 4.0f, 1.0f, 2.0f, 3.0f, 4.0f, 1.0f, 2.0f, 3.0f, 4.0f);
				glm::mat4 A_myCompareMat1(10.0f, 20.0f, 30.0f, 40.0f, 10.0f, 20.0f, 30.0f, 40.0f, 10.0f, 20.0f, 30.0f, 40.0f, 10.0f, 20.0f, 30.0f, 40.0f);
				glm::mat4 A_myCompareMat2(100.0f, 200.0f, 300.0f, 400.0f, 100.0f, 200.0f, 300.0f, 400.0f, 100.0f, 200.0f, 300.0f, 400.0f, 100.0f, 200.0f, 300.0f, 400.0f);

				Assert::AreEqual(A_myCompareMat0, mySuperDerivedAttributedFoo->_externalMatrixArray[0]);
				Assert::AreEqual(A_myCompareMat1, mySuperDerivedAttributedFoo->_externalMatrixArray[1]);
				Assert::AreEqual(A_myCompareMat2, mySuperDerivedAttributedFoo->_externalMatrixArray[2]);


			}


		}
		TEST_METHOD(ParseAvatarFooWithBooleans) //ADDED BOOLEANS 6/21/2022
		{

			IJsonCoordinator coordinator;
			coordinator.AddHelper<JsonScopeHelper>();
			AvatarFooFactory avatarFooFactory;

			JsonScopeHelper::Data* dataClass = coordinator.ParseFromFile<JsonScopeHelper::Data>("Content/AvatarFooBoolean.json");
			dataClass;

			Scope* gameObjectScope = dataClass->GetData();
			GameObject* gameObject = gameObjectScope->As<GameObject>();

			Vector<AvatarFoo*> vecAvatartFoo = gameObject->FindAllChildrenOfType<AvatarFoo>();

			Assert::AreEqual(vecAvatartFoo[0]->_isTakingDamage, true);
			Assert::AreEqual(vecAvatartFoo[1]->_isTakingDamage, false);

		}
	private:
		static _CrtMemState _startMemState; //static class members are shared in every instance of the class
	};

	_CrtMemState JsonParsingTest::_startMemState; //There is needed to separate declaration from definition. Changes in C++17
}