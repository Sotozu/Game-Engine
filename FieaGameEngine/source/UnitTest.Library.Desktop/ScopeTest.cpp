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
using namespace FieaGameEngine;

namespace UnitTestLibraryDesktop
{
	TEST_CLASS(ScopeTest)
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
		TEST_METHOD(ScopeDefaultConstructor)
		{
			Scope myScope;
			Assert::AreEqual(static_cast<std::size_t>(0), myScope.NumberOfItems());
		}
		TEST_METHOD(ScopeInitializerConstructor)
		{

			//Scope holding All Types
			{

				glm::vec4 myVec1(1.0f, 1.0f, 1.0f, 1.0f);
				glm::vec4 myVec2(2.0f, 2.0f, 2.0f, 2.0f);
				glm::vec4 myVec3(3.0f, 3.0f, 3.0f, 3.0f);

				glm::mat4 myMat1(1.0f);
				glm::mat4 myMat2(2.0f);
				glm::mat4 myMat3(3.0f);

				Foo myFoo1(1);
				Foo myFoo2(2);
				Foo myFoo3(3);
				RTTI* myRTTI1 = &myFoo1;
				RTTI* myRTTI2 = &myFoo2;
				RTTI* myRTTI3 = &myFoo3;

				Datum myIntDatum{ 1,2,3 };
				Datum myFloatDatum{ 1.0f,2.0f,3.0f };
				Datum myStringDatum{ "a"s,"b"s,"c"s };
				Datum myVectorDatum{ myVec1,myVec2,myVec3 };
				Datum myMatrixDatum{ myMat1,myMat2,myMat3 };
				Datum myRTTIDatum{ myRTTI1,myRTTI2, myRTTI3 };


				Scope::pt_string_datum pairInt("Integer Datum", myIntDatum);
				Scope::pt_string_datum pairFloat("Float Datum", myFloatDatum);
				Scope::pt_string_datum pairString("String Datum", myStringDatum);
				Scope::pt_string_datum pairVector("Vector Datum", myVectorDatum);
				Scope::pt_string_datum pairMatrix("Matrix Datum", myMatrixDatum);
				Scope::pt_string_datum pairRTTI("RTTI Datum", myRTTIDatum);

				Scope* myScope1 = new Scope{ &pairInt, &pairFloat };
				Scope* myScope2 = new Scope{ &pairString, &pairVector };
				Scope* myScope3 = new Scope{ &pairMatrix, &pairRTTI };

				Datum myScopeDatum{ myScope1,myScope2, myScope3 };
				Scope::pt_string_datum pairScope("Scope Datum", myScopeDatum);


				Scope myStackTestScope{ &pairInt, &pairFloat, &pairString, &pairVector, &pairMatrix, &pairRTTI, &pairScope };

				Assert::AreEqual(static_cast<std::size_t>(7), myStackTestScope.NumberOfItems());
				Assert::AreEqual(myStackTestScope[0], myIntDatum);
				Assert::AreEqual(myStackTestScope[1], myFloatDatum);
				Assert::AreEqual(myStackTestScope[2], myStringDatum);
				Assert::AreEqual(myStackTestScope[3], myVectorDatum);
				Assert::AreEqual(myStackTestScope[4], myMatrixDatum);
				Assert::AreEqual(myStackTestScope[5], myRTTIDatum);
				Assert::AreEqual(myStackTestScope[6], myScopeDatum);


			}

		
		}
		TEST_METHOD(ScopeCopyConstructor)
		{

			glm::vec4 myVec1(1.0f, 1.0f, 1.0f, 1.0f);
			glm::vec4 myVec2(2.0f, 2.0f, 2.0f, 2.0f);
			glm::vec4 myVec3(3.0f, 3.0f, 3.0f, 3.0f);

			glm::mat4 myMat1(1.0f);
			glm::mat4 myMat2(2.0f);
			glm::mat4 myMat3(3.0f);

			Foo myFoo1(1);
			Foo myFoo2(2);
			Foo myFoo3(3);
			RTTI* myRTTI1 = &myFoo1;
			RTTI* myRTTI2 = &myFoo2;
			RTTI* myRTTI3 = &myFoo3;

			Datum myIntDatum{ 1,2,3 };
			Datum myFloatDatum{ 1.0f,2.0f,3.0f };
			Datum myStringDatum{ "a"s,"b"s,"c"s };
			Datum myVectorDatum{ myVec1,myVec2,myVec3 };
			Datum myMatrixDatum{ myMat1,myMat2,myMat3 };
			Datum myRTTIDatum{ myRTTI1,myRTTI2, myRTTI3 };


			Scope::pt_string_datum pairInt("Integer Datum", myIntDatum);
			Scope::pt_string_datum pairFloat("Float Datum", myFloatDatum);
			Scope::pt_string_datum pairString("String Datum", myStringDatum);
			Scope::pt_string_datum pairVector("Vector Datum", myVectorDatum);
			Scope::pt_string_datum pairMatrix("Matrix Datum", myMatrixDatum);
			Scope::pt_string_datum pairRTTI("RTTI Datum", myRTTIDatum);

			Scope* myScope1 = new Scope{ &pairInt, &pairFloat };
			Scope* myScope2 = new Scope{ &pairString, &pairVector };
			Scope* myScope3 = new Scope{ &pairMatrix, &pairRTTI };

			Datum myScopeDatum{ myScope1,myScope2, myScope3 };
			Scope::pt_string_datum pairScope("Scope Datum", myScopeDatum);


			Scope myStackTestScope{ &pairInt, &pairFloat, &pairString, &pairVector, &pairMatrix, &pairRTTI, &pairScope };

			Assert::AreEqual(static_cast<std::size_t>(7), myStackTestScope.NumberOfItems());
			Assert::AreEqual(myStackTestScope[0], myIntDatum);
			Assert::AreEqual(myStackTestScope[1], myFloatDatum);
			Assert::AreEqual(myStackTestScope[2], myStringDatum);
			Assert::AreEqual(myStackTestScope[3], myVectorDatum);
			Assert::AreEqual(myStackTestScope[4], myMatrixDatum);
			Assert::AreEqual(myStackTestScope[5], myRTTIDatum);
			Assert::AreEqual(myStackTestScope[6], myScopeDatum);

			Scope myCopyScope(myStackTestScope);

			Assert::AreEqual(static_cast<std::size_t>(7), myCopyScope.NumberOfItems());
			Assert::AreEqual(myCopyScope[0], myIntDatum);
			Assert::AreEqual(myCopyScope[1], myFloatDatum);
			Assert::AreEqual(myCopyScope[2], myStringDatum);
			Assert::AreEqual(myCopyScope[3], myVectorDatum);
			Assert::AreEqual(myCopyScope[4], myMatrixDatum);
			Assert::AreEqual(myCopyScope[5], myRTTIDatum);
			Assert::AreEqual(myCopyScope[6], myScopeDatum);

		}
		TEST_METHOD(ScopeOOAssignment)
		{
			glm::vec4 myVec1(1.0f, 1.0f, 1.0f, 1.0f);
			glm::vec4 myVec2(2.0f, 2.0f, 2.0f, 2.0f);
			glm::vec4 myVec3(3.0f, 3.0f, 3.0f, 3.0f);

			glm::mat4 myMat1(1.0f);
			glm::mat4 myMat2(2.0f);
			glm::mat4 myMat3(3.0f);

			Foo myFoo1(1);
			Foo myFoo2(2);
			Foo myFoo3(3);
			RTTI* myRTTI1 = &myFoo1;
			RTTI* myRTTI2 = &myFoo2;
			RTTI* myRTTI3 = &myFoo3;

			Datum myIntDatum{ 1,2,3 };
			Datum myFloatDatum{ 1.0f,2.0f,3.0f };
			Datum myStringDatum{ "a"s,"b"s,"c"s };
			Datum myVectorDatum{ myVec1,myVec2,myVec3 };
			Datum myMatrixDatum{ myMat1,myMat2,myMat3 };
			Datum myRTTIDatum{ myRTTI1,myRTTI2, myRTTI3 };


			Scope::pt_string_datum pairInt("Integer Datum", myIntDatum);
			Scope::pt_string_datum pairFloat("Float Datum", myFloatDatum);
			Scope::pt_string_datum pairString("String Datum", myStringDatum);
			Scope::pt_string_datum pairVector("Vector Datum", myVectorDatum);
			Scope::pt_string_datum pairMatrix("Matrix Datum", myMatrixDatum);
			Scope::pt_string_datum pairRTTI("RTTI Datum", myRTTIDatum);

			Scope* myScope1 = new Scope{ &pairInt, &pairFloat };
			Scope* myScope2 = new Scope{ &pairString, &pairVector };
			Scope* myScope3 = new Scope{ &pairMatrix, &pairRTTI };

			Datum myScopeDatum{ myScope1,myScope2, myScope3 };
			Scope::pt_string_datum pairScope("Scope Datum", myScopeDatum);


			Scope myStackTestScope{ &pairInt, &pairFloat, &pairString, &pairVector, &pairMatrix, &pairRTTI, &pairScope };

			Assert::AreEqual(static_cast<std::size_t>(7), myStackTestScope.NumberOfItems());
			Assert::AreEqual(myStackTestScope[0], myIntDatum);
			Assert::AreEqual(myStackTestScope[1], myFloatDatum);
			Assert::AreEqual(myStackTestScope[2], myStringDatum);
			Assert::AreEqual(myStackTestScope[3], myVectorDatum);
			Assert::AreEqual(myStackTestScope[4], myMatrixDatum);
			Assert::AreEqual(myStackTestScope[5], myRTTIDatum);
			Assert::AreEqual(myStackTestScope[6], myScopeDatum);

			Scope myCopyScope;
			myCopyScope = myStackTestScope;

			Assert::AreEqual(static_cast<std::size_t>(7), myCopyScope.NumberOfItems());
			Assert::AreEqual(myCopyScope[0], myIntDatum);
			Assert::AreEqual(myCopyScope[1], myFloatDatum);
			Assert::AreEqual(myCopyScope[2], myStringDatum);
			Assert::AreEqual(myCopyScope[3], myVectorDatum);
			Assert::AreEqual(myCopyScope[4], myMatrixDatum);
			Assert::AreEqual(myCopyScope[5], myRTTIDatum);
			Assert::AreEqual(myCopyScope[6], myScopeDatum);
		}
		TEST_METHOD(ScopeMoveConstructor)
		{
			glm::vec4 myVec1(1.0f, 1.0f, 1.0f, 1.0f);
			glm::vec4 myVec2(2.0f, 2.0f, 2.0f, 2.0f);
			glm::vec4 myVec3(3.0f, 3.0f, 3.0f, 3.0f);

			glm::mat4 myMat1(1.0f);
			glm::mat4 myMat2(2.0f);
			glm::mat4 myMat3(3.0f);

			Foo myFoo1(1);
			Foo myFoo2(2);
			Foo myFoo3(3);
			RTTI* myRTTI1 = &myFoo1;
			RTTI* myRTTI2 = &myFoo2;
			RTTI* myRTTI3 = &myFoo3;

			Datum myIntDatum{ 1,2,3 };
			Datum myFloatDatum{ 1.0f,2.0f,3.0f };
			Datum myStringDatum{ "a"s,"b"s,"c"s };
			Datum myVectorDatum{ myVec1,myVec2,myVec3 };
			Datum myMatrixDatum{ myMat1,myMat2,myMat3 };
			Datum myRTTIDatum{ myRTTI1,myRTTI2, myRTTI3 };


			Scope::pt_string_datum pairInt("Integer Datum", myIntDatum);
			Scope::pt_string_datum pairFloat("Float Datum", myFloatDatum);
			Scope::pt_string_datum pairString("String Datum", myStringDatum);
			Scope::pt_string_datum pairVector("Vector Datum", myVectorDatum);
			Scope::pt_string_datum pairMatrix("Matrix Datum", myMatrixDatum);
			Scope::pt_string_datum pairRTTI("RTTI Datum", myRTTIDatum);

			Scope* myScope1 = new Scope{ &pairInt, &pairFloat };
			Scope* myScope2 = new Scope{ &pairString, &pairVector };
			Scope* myScope3 = new Scope{ &pairMatrix, &pairRTTI };

			Datum myScopeDatum{ myScope1,myScope2, myScope3 };
			Scope::pt_string_datum pairScope("Scope Datum", myScopeDatum);


			Scope myStackTestScope{ &pairInt, &pairFloat, &pairString, &pairVector, &pairMatrix, &pairRTTI, &pairScope };

			Assert::AreEqual(static_cast<std::size_t>(7), myStackTestScope.NumberOfItems());
			Assert::AreEqual(myStackTestScope[0], myIntDatum);
			Assert::AreEqual(myStackTestScope[1], myFloatDatum);
			Assert::AreEqual(myStackTestScope[2], myStringDatum);
			Assert::AreEqual(myStackTestScope[3], myVectorDatum);
			Assert::AreEqual(myStackTestScope[4], myMatrixDatum);
			Assert::AreEqual(myStackTestScope[5], myRTTIDatum);
			Assert::AreEqual(myStackTestScope[6], myScopeDatum);

			Scope myMoveScope(std::move(myStackTestScope));

			Assert::AreEqual(static_cast<std::size_t>(7), myMoveScope.NumberOfItems());
			Assert::AreEqual(myMoveScope[0], myIntDatum);
			Assert::AreEqual(myMoveScope[1], myFloatDatum);
			Assert::AreEqual(myMoveScope[2], myStringDatum);
			Assert::AreEqual(myMoveScope[3], myVectorDatum);
			Assert::AreEqual(myMoveScope[4], myMatrixDatum);
			Assert::AreEqual(myMoveScope[5], myRTTIDatum);
			Assert::AreEqual(myMoveScope[6], myScopeDatum);
		}
		TEST_METHOD(ScopeMoveAssignment)
		{
			glm::vec4 myVec1(1.0f, 1.0f, 1.0f, 1.0f);
			glm::vec4 myVec2(2.0f, 2.0f, 2.0f, 2.0f);
			glm::vec4 myVec3(3.0f, 3.0f, 3.0f, 3.0f);

			glm::mat4 myMat1(1.0f);
			glm::mat4 myMat2(2.0f);
			glm::mat4 myMat3(3.0f);

			Foo myFoo1(1);
			Foo myFoo2(2);
			Foo myFoo3(3);
			RTTI* myRTTI1 = &myFoo1;
			RTTI* myRTTI2 = &myFoo2;
			RTTI* myRTTI3 = &myFoo3;

			Datum myIntDatum{ 1,2,3 };
			Datum myFloatDatum{ 1.0f,2.0f,3.0f };
			Datum myStringDatum{ "a"s,"b"s,"c"s };
			Datum myVectorDatum{ myVec1,myVec2,myVec3 };
			Datum myMatrixDatum{ myMat1,myMat2,myMat3 };
			Datum myRTTIDatum{ myRTTI1,myRTTI2, myRTTI3 };


			Scope::pt_string_datum pairInt("Integer Datum", myIntDatum);
			Scope::pt_string_datum pairFloat("Float Datum", myFloatDatum);
			Scope::pt_string_datum pairString("String Datum", myStringDatum);
			Scope::pt_string_datum pairVector("Vector Datum", myVectorDatum);
			Scope::pt_string_datum pairMatrix("Matrix Datum", myMatrixDatum);
			Scope::pt_string_datum pairRTTI("RTTI Datum", myRTTIDatum);

			Scope* myScope1 = new Scope{ &pairInt, &pairFloat };
			Scope* myScope2 = new Scope{ &pairString, &pairVector };
			Scope* myScope3 = new Scope{ &pairMatrix, &pairRTTI };

			Datum myScopeDatum{ myScope1,myScope2, myScope3 };
			Scope::pt_string_datum pairScope("Scope Datum", myScopeDatum);


			Scope myStackTestScope{ &pairInt, &pairFloat, &pairString, &pairVector, &pairMatrix, &pairRTTI, &pairScope };

			Assert::AreEqual(static_cast<std::size_t>(7), myStackTestScope.NumberOfItems());
			Assert::AreEqual(myStackTestScope[0], myIntDatum);
			Assert::AreEqual(myStackTestScope[1], myFloatDatum);
			Assert::AreEqual(myStackTestScope[2], myStringDatum);
			Assert::AreEqual(myStackTestScope[3], myVectorDatum);
			Assert::AreEqual(myStackTestScope[4], myMatrixDatum);
			Assert::AreEqual(myStackTestScope[5], myRTTIDatum);
			Assert::AreEqual(myStackTestScope[6], myScopeDatum);

			Scope myMoveScope;
			myMoveScope = std::move(myStackTestScope);

			Assert::AreEqual(static_cast<std::size_t>(7), myMoveScope.NumberOfItems());
			Assert::AreEqual(myMoveScope[0], myIntDatum);
			Assert::AreEqual(myMoveScope[1], myFloatDatum);
			Assert::AreEqual(myMoveScope[2], myStringDatum);
			Assert::AreEqual(myMoveScope[3], myVectorDatum);
			Assert::AreEqual(myMoveScope[4], myMatrixDatum);
			Assert::AreEqual(myMoveScope[5], myRTTIDatum);
			Assert::AreEqual(myMoveScope[6], myScopeDatum);

		}
		TEST_METHOD(ScopeClear)
		{
			glm::vec4 myVec1(1.0f, 1.0f, 1.0f, 1.0f);
			glm::vec4 myVec2(2.0f, 2.0f, 2.0f, 2.0f);
			glm::vec4 myVec3(3.0f, 3.0f, 3.0f, 3.0f);

			glm::mat4 myMat1(1.0f);
			glm::mat4 myMat2(2.0f);
			glm::mat4 myMat3(3.0f);

			Foo myFoo1(1);
			Foo myFoo2(2);
			Foo myFoo3(3);
			RTTI* myRTTI1 = &myFoo1;
			RTTI* myRTTI2 = &myFoo2;
			RTTI* myRTTI3 = &myFoo3;

			Datum myIntDatum{ 1,2,3 };
			Datum myFloatDatum{ 1.0f,2.0f,3.0f };
			Datum myStringDatum{ "a"s,"b"s,"c"s };
			Datum myVectorDatum{ myVec1,myVec2,myVec3 };
			Datum myMatrixDatum{ myMat1,myMat2,myMat3 };
			Datum myRTTIDatum{ myRTTI1,myRTTI2, myRTTI3 };


			Scope::pt_string_datum pairInt("Integer Datum", myIntDatum);
			Scope::pt_string_datum pairFloat("Float Datum", myFloatDatum);
			Scope::pt_string_datum pairString("String Datum", myStringDatum);
			Scope::pt_string_datum pairVector("Vector Datum", myVectorDatum);
			Scope::pt_string_datum pairMatrix("Matrix Datum", myMatrixDatum);
			Scope::pt_string_datum pairRTTI("RTTI Datum", myRTTIDatum);

			Scope* myScope1 = new Scope{ &pairInt, &pairFloat };
			Scope* myScope2 = new Scope{ &pairString, &pairVector };
			Scope* myScope3 = new Scope{ &pairMatrix, &pairRTTI };

			Datum myScopeDatum{ myScope1,myScope2, myScope3 };
			Scope::pt_string_datum pairScope("Scope Datum", myScopeDatum);


			Scope myStackTestScope{ &pairInt, &pairFloat, &pairString, &pairVector, &pairMatrix, &pairRTTI, &pairScope };

			Assert::AreEqual(static_cast<std::size_t>(7), myStackTestScope.NumberOfItems());
			Assert::AreEqual(myStackTestScope[0], myIntDatum);
			Assert::AreEqual(myStackTestScope[1], myFloatDatum);
			Assert::AreEqual(myStackTestScope[2], myStringDatum);
			Assert::AreEqual(myStackTestScope[3], myVectorDatum);
			Assert::AreEqual(myStackTestScope[4], myMatrixDatum);
			Assert::AreEqual(myStackTestScope[5], myRTTIDatum);
			Assert::AreEqual(myStackTestScope[6], myScopeDatum);

			myStackTestScope.Clear();
			Assert::AreEqual(static_cast<std::size_t>(0), myStackTestScope.NumberOfItems());

		}
		TEST_METHOD(ScopeFind)
		{
			glm::vec4 myVec1(1.0f, 1.0f, 1.0f, 1.0f);
			glm::vec4 myVec2(2.0f, 2.0f, 2.0f, 2.0f);
			glm::vec4 myVec3(3.0f, 3.0f, 3.0f, 3.0f);

			glm::mat4 myMat1(1.0f);
			glm::mat4 myMat2(2.0f);
			glm::mat4 myMat3(3.0f);

			Foo myFoo1(1);
			Foo myFoo2(2);
			Foo myFoo3(3);
			RTTI* myRTTI1 = &myFoo1;
			RTTI* myRTTI2 = &myFoo2;
			RTTI* myRTTI3 = &myFoo3;

			Datum myIntDatum{ 1,2,3 };
			Datum myFloatDatum{ 1.0f,2.0f,3.0f };
			Datum myStringDatum{ "a"s,"b"s,"c"s };
			Datum myVectorDatum{ myVec1,myVec2,myVec3 };
			Datum myMatrixDatum{ myMat1,myMat2,myMat3 };
			Datum myRTTIDatum{ myRTTI1,myRTTI2, myRTTI3 };


			Scope::pt_string_datum pairInt("Integer Datum", myIntDatum);
			Scope::pt_string_datum pairFloat("Float Datum", myFloatDatum);
			Scope::pt_string_datum pairString("String Datum", myStringDatum);
			Scope::pt_string_datum pairVector("Vector Datum", myVectorDatum);
			Scope::pt_string_datum pairMatrix("Matrix Datum", myMatrixDatum);
			Scope::pt_string_datum pairRTTI("RTTI Datum", myRTTIDatum);

			Scope* myScope1 = new Scope{ &pairInt, &pairFloat };
			Scope* myScope2 = new Scope{ &pairString, &pairVector };
			Scope* myScope3 = new Scope{ &pairMatrix, &pairRTTI };

			Datum myScopeDatum{ myScope1,myScope2, myScope3 };
			Scope::pt_string_datum pairScope("Scope Datum", myScopeDatum);


			Scope myStackTestScope{ &pairInt, &pairFloat, &pairString, &pairVector, &pairMatrix, &pairRTTI, &pairScope };

			Assert::AreEqual(static_cast<std::size_t>(7), myStackTestScope.NumberOfItems());
			Assert::AreEqual(myStackTestScope[0], myIntDatum);
			Assert::AreEqual(myStackTestScope[1], myFloatDatum);
			Assert::AreEqual(myStackTestScope[2], myStringDatum);
			Assert::AreEqual(myStackTestScope[3], myVectorDatum);
			Assert::AreEqual(myStackTestScope[4], myMatrixDatum);
			Assert::AreEqual(myStackTestScope[5], myRTTIDatum);
			Assert::AreEqual(myStackTestScope[6], myScopeDatum);

			Datum* datum;

			datum = myStackTestScope.Find("Integer Datum");
			Assert::IsNotNull(datum); //found a valid datum associated with the name.

			datum = myStackTestScope.Find("Float Datum");
			Assert::IsNotNull(datum); //found a valid datum associated with the name.

			datum = myStackTestScope.Find("String Datum");
			Assert::IsNotNull(datum); //found a valid datum associated with the name.

			datum = myStackTestScope.Find("Vector Datum");
			Assert::IsNotNull(datum); //found a valid datum associated with the name.

			datum = myStackTestScope.Find("Matrix Datum");
			Assert::IsNotNull(datum); //found a valid datum associated with the name.

			datum = myStackTestScope.Find("RTTI Datum");
			Assert::IsNotNull(datum); //found a valid datum associated with the name.

			datum = myStackTestScope.Find("Scope Datum");
			Assert::IsNotNull(datum); //found a valid datum associated with the name.

			datum = myStackTestScope.Find("DATUM NOT HERE");
			Assert::IsNull(datum); //DID NOT FIND A DATUM WITH THE NAME

		}
		TEST_METHOD(ScopeFindConst)
		{
			glm::vec4 myVec1(1.0f, 1.0f, 1.0f, 1.0f);
			glm::vec4 myVec2(2.0f, 2.0f, 2.0f, 2.0f);
			glm::vec4 myVec3(3.0f, 3.0f, 3.0f, 3.0f);

			glm::mat4 myMat1(1.0f);
			glm::mat4 myMat2(2.0f);
			glm::mat4 myMat3(3.0f);

			Foo myFoo1(1);
			Foo myFoo2(2);
			Foo myFoo3(3);
			RTTI* myRTTI1 = &myFoo1;
			RTTI* myRTTI2 = &myFoo2;
			RTTI* myRTTI3 = &myFoo3;

			Datum myIntDatum{ 1,2,3 };
			Datum myFloatDatum{ 1.0f,2.0f,3.0f };
			Datum myStringDatum{ "a"s,"b"s,"c"s };
			Datum myVectorDatum{ myVec1,myVec2,myVec3 };
			Datum myMatrixDatum{ myMat1,myMat2,myMat3 };
			Datum myRTTIDatum{ myRTTI1,myRTTI2, myRTTI3 };


			Scope::pt_string_datum pairInt("Integer Datum", myIntDatum);
			Scope::pt_string_datum pairFloat("Float Datum", myFloatDatum);
			Scope::pt_string_datum pairString("String Datum", myStringDatum);
			Scope::pt_string_datum pairVector("Vector Datum", myVectorDatum);
			Scope::pt_string_datum pairMatrix("Matrix Datum", myMatrixDatum);
			Scope::pt_string_datum pairRTTI("RTTI Datum", myRTTIDatum);

			Scope* myScope1 = new Scope{ &pairInt, &pairFloat };
			Scope* myScope2 = new Scope{ &pairString, &pairVector };
			Scope* myScope3 = new Scope{ &pairMatrix, &pairRTTI };

			Datum myScopeDatum{ myScope1,myScope2, myScope3 };
			Scope::pt_string_datum pairScope("Scope Datum", myScopeDatum);


			const Scope myStackTestScope{ &pairInt, &pairFloat, &pairString, &pairVector, &pairMatrix, &pairRTTI, &pairScope };

			Assert::AreEqual(static_cast<std::size_t>(7), myStackTestScope.NumberOfItems());
			Assert::AreEqual(myStackTestScope[0], myIntDatum);
			Assert::AreEqual(myStackTestScope[1], myFloatDatum);
			Assert::AreEqual(myStackTestScope[2], myStringDatum);
			Assert::AreEqual(myStackTestScope[3], myVectorDatum);
			Assert::AreEqual(myStackTestScope[4], myMatrixDatum);
			Assert::AreEqual(myStackTestScope[5], myRTTIDatum);
			Assert::AreEqual(myStackTestScope[6], myScopeDatum);

			const Datum* datum;

			datum = myStackTestScope.Find("Integer Datum");
			Assert::IsNotNull(datum); //found a valid datum associated with the name.

			datum = myStackTestScope.Find("Float Datum");
			Assert::IsNotNull(datum); //found a valid datum associated with the name.

			datum = myStackTestScope.Find("String Datum");
			Assert::IsNotNull(datum); //found a valid datum associated with the name.

			datum = myStackTestScope.Find("Vector Datum");
			Assert::IsNotNull(datum); //found a valid datum associated with the name.

			datum = myStackTestScope.Find("Matrix Datum");
			Assert::IsNotNull(datum); //found a valid datum associated with the name.

			datum = myStackTestScope.Find("RTTI Datum");
			Assert::IsNotNull(datum); //found a valid datum associated with the name.

			datum = myStackTestScope.Find("Scope Datum");
			Assert::IsNotNull(datum); //found a valid datum associated with the name.

			datum = myStackTestScope.Find("DATUM NOT HERE");
			Assert::IsNull(datum); //DID NOT FIND A DATUM WITH THE NAME
		}
		TEST_METHOD(ScopeSearchDown_String_ScopePtrRef)
		{
			glm::vec4 myVec1(1.0f, 1.0f, 1.0f, 1.0f);
			glm::vec4 myVec2(2.0f, 2.0f, 2.0f, 2.0f);
			glm::vec4 myVec3(3.0f, 3.0f, 3.0f, 3.0f);

			glm::mat4 myMat1(1.0f);
			glm::mat4 myMat2(2.0f);
			glm::mat4 myMat3(3.0f);

			Foo myFoo1(1);
			Foo myFoo2(2);
			Foo myFoo3(3);
			RTTI* myRTTI1 = &myFoo1;
			RTTI* myRTTI2 = &myFoo2;
			RTTI* myRTTI3 = &myFoo3;

			Datum myIntDatum{ 1,2,3 };
			Datum myFloatDatum{ 1.0f,2.0f,3.0f };
			Datum myStringDatum{ "a"s,"b"s,"c"s };
			Datum myVectorDatum{ myVec1,myVec2,myVec3 };
			Datum myMatrixDatum{ myMat1,myMat2,myMat3 };
			Datum myRTTIDatum{ myRTTI1,myRTTI2, myRTTI3 };


			Scope::pt_string_datum pairInt("Integer Datum", myIntDatum);
			Scope::pt_string_datum pairFloat("Float Datum", myFloatDatum);
			Scope::pt_string_datum pairString("String Datum", myStringDatum);
			Scope::pt_string_datum pairVector("Vector Datum", myVectorDatum);
			Scope::pt_string_datum pairMatrix("Matrix Datum", myMatrixDatum);
			Scope::pt_string_datum pairRTTI("RTTI Datum", myRTTIDatum);

			Scope* myScope1 = new Scope{ &pairInt, &pairFloat };
			Scope* myScope2 = new Scope{ &pairString, &pairVector };
			Scope* myScope3 = new Scope{ &pairMatrix, &pairRTTI };


			myScope2->Adopt("SC3", *myScope3); //Scope 2 holds Scope3 in a datum within its hash map.
			myScope1->Adopt("SC2", *myScope2); //Scope1 holds Scope2 in a datum within its hash map.
			

			Scope myStackTestScope;
			myStackTestScope.Adopt("SC1", *myScope1); //myStackTestScope holds Scope1 in a datum within its hash map.

			Assert::AreEqual(static_cast<std::size_t>(1), myStackTestScope.NumberOfItems());
			Datum* foundDatum = nullptr;
			Scope* locatedScope = nullptr;

			foundDatum = myStackTestScope.SearchDown("Integer Datum", &locatedScope);
			Assert::IsNotNull(foundDatum);
			Assert::IsNotNull(locatedScope);

			Assert::AreEqual(myIntDatum, *foundDatum);
			Assert::AreSame(*myScope1, *locatedScope);

			foundDatum = myStackTestScope.SearchDown("Float Datum", &locatedScope);
			Assert::IsNotNull(foundDatum);
			Assert::IsNotNull(locatedScope);

			Assert::AreEqual(myFloatDatum, *foundDatum);
			Assert::AreSame(*myScope1, *locatedScope);


			foundDatum = myStackTestScope.SearchDown("String Datum", &locatedScope);
			Assert::IsNotNull(foundDatum);
			Assert::IsNotNull(locatedScope);

			Assert::AreEqual(myStringDatum, *foundDatum);
			Assert::AreSame(*myScope2, *locatedScope);

			foundDatum = myStackTestScope.SearchDown("Vector Datum", &locatedScope);
			Assert::IsNotNull(foundDatum);
			Assert::IsNotNull(locatedScope);

			Assert::AreEqual(myVectorDatum, *foundDatum);
			Assert::AreSame(*myScope2, *locatedScope);

			foundDatum = myStackTestScope.SearchDown("Matrix Datum", &locatedScope);
			Assert::IsNotNull(foundDatum);
			Assert::IsNotNull(locatedScope);

			Assert::AreEqual(myMatrixDatum, *foundDatum);
			Assert::AreSame(*myScope3, *locatedScope);

			foundDatum = myStackTestScope.SearchDown("RTTI Datum", &locatedScope);
			Assert::IsNotNull(foundDatum);
			Assert::IsNotNull(locatedScope);

			Assert::AreEqual(myRTTIDatum, *foundDatum);
			Assert::AreSame(*myScope3, *locatedScope);

			foundDatum = myStackTestScope.SearchDown("NOT HERE!", &locatedScope);
			Assert::IsNull(foundDatum);
			Assert::IsNull(locatedScope);

		}
		TEST_METHOD(ScopeSearchDownConst_String_ScopePtrRef)
		{
			glm::vec4 myVec1(1.0f, 1.0f, 1.0f, 1.0f);
			glm::vec4 myVec2(2.0f, 2.0f, 2.0f, 2.0f);
			glm::vec4 myVec3(3.0f, 3.0f, 3.0f, 3.0f);

			glm::mat4 myMat1(1.0f);
			glm::mat4 myMat2(2.0f);
			glm::mat4 myMat3(3.0f);

			Foo myFoo1(1);
			Foo myFoo2(2);
			Foo myFoo3(3);
			RTTI* myRTTI1 = &myFoo1;
			RTTI* myRTTI2 = &myFoo2;
			RTTI* myRTTI3 = &myFoo3;

			Datum myIntDatum{ 1,2,3 };
			Datum myFloatDatum{ 1.0f,2.0f,3.0f };
			Datum myStringDatum{ "a"s,"b"s,"c"s };
			Datum myVectorDatum{ myVec1,myVec2,myVec3 };
			Datum myMatrixDatum{ myMat1,myMat2,myMat3 };
			Datum myRTTIDatum{ myRTTI1,myRTTI2, myRTTI3 };


			Scope::pt_string_datum pairInt("Integer Datum", myIntDatum);
			Scope::pt_string_datum pairFloat("Float Datum", myFloatDatum);
			Scope::pt_string_datum pairString("String Datum", myStringDatum);
			Scope::pt_string_datum pairVector("Vector Datum", myVectorDatum);
			Scope::pt_string_datum pairMatrix("Matrix Datum", myMatrixDatum);
			Scope::pt_string_datum pairRTTI("RTTI Datum", myRTTIDatum);

			Scope* myScope1 = new Scope{ &pairInt, &pairFloat };
			Scope* myScope2 = new Scope{ &pairString, &pairVector };
			Scope* myScope3 = new Scope{ &pairMatrix, &pairRTTI };


			myScope2->Adopt("SC3", *myScope3); //Scope 2 holds Scope3 in a datum within its hash map.
			myScope1->Adopt("SC2", *myScope2); //Scope1 holds Scope2 in a datum within its hash map.


			Scope::pt_string_datum pairScope("SC1", *myScope1); //Make sure you are passing Scope reference and not pointer. Otherwise RTTI methods are invoked.

			const Scope myStackTestScope{ &pairScope };//myStackTestScope holds Scope1 in a datum within its hash map

			Assert::AreEqual(static_cast<std::size_t>(1), myStackTestScope.NumberOfItems());
			
			const Scope* locatedScope = nullptr;
			const Datum* foundDatum = nullptr;

			foundDatum = myStackTestScope.SearchDown("Integer Datum", &locatedScope);
			Assert::IsNotNull(foundDatum);
			Assert::IsNotNull(locatedScope);

			Assert::AreEqual(myIntDatum, *foundDatum);
			Assert::AreSame(*myScope1, *locatedScope);

			foundDatum = myStackTestScope.SearchDown("Float Datum", &locatedScope);
			Assert::IsNotNull(foundDatum);
			Assert::IsNotNull(locatedScope);

			Assert::AreEqual(myFloatDatum, *foundDatum);
			Assert::AreSame(*myScope1, *locatedScope);


			foundDatum = myStackTestScope.SearchDown("String Datum", &locatedScope);
			Assert::IsNotNull(foundDatum);
			Assert::IsNotNull(locatedScope);

			Assert::AreEqual(myStringDatum, *foundDatum);
			Assert::AreSame(*myScope2, *locatedScope);

			foundDatum = myStackTestScope.SearchDown("Vector Datum", &locatedScope);
			Assert::IsNotNull(foundDatum);
			Assert::IsNotNull(locatedScope);

			Assert::AreEqual(myVectorDatum, *foundDatum);
			Assert::AreSame(*myScope2, *locatedScope);

			foundDatum = myStackTestScope.SearchDown("Matrix Datum", &locatedScope);
			Assert::IsNotNull(foundDatum);
			Assert::IsNotNull(locatedScope);

			Assert::AreEqual(myMatrixDatum, *foundDatum);
			Assert::AreSame(*myScope3, *locatedScope);

			foundDatum = myStackTestScope.SearchDown("RTTI Datum", &locatedScope);
			Assert::IsNotNull(foundDatum);
			Assert::IsNotNull(locatedScope);

			Assert::AreEqual(myRTTIDatum, *foundDatum);
			Assert::AreSame(*myScope3, *locatedScope);

			foundDatum = myStackTestScope.SearchDown("NOT HERE!", &locatedScope);
			Assert::IsNull(foundDatum);
			Assert::IsNull(locatedScope);
		}
		TEST_METHOD(ScopeSearchDown_String)
		{
			glm::vec4 myVec1(1.0f, 1.0f, 1.0f, 1.0f);
			glm::vec4 myVec2(2.0f, 2.0f, 2.0f, 2.0f);
			glm::vec4 myVec3(3.0f, 3.0f, 3.0f, 3.0f);

			glm::mat4 myMat1(1.0f);
			glm::mat4 myMat2(2.0f);
			glm::mat4 myMat3(3.0f);

			Foo myFoo1(1);
			Foo myFoo2(2);
			Foo myFoo3(3);
			RTTI* myRTTI1 = &myFoo1;
			RTTI* myRTTI2 = &myFoo2;
			RTTI* myRTTI3 = &myFoo3;

			Datum myIntDatum{ 1,2,3 };
			Datum myFloatDatum{ 1.0f,2.0f,3.0f };
			Datum myStringDatum{ "a"s,"b"s,"c"s };
			Datum myVectorDatum{ myVec1,myVec2,myVec3 };
			Datum myMatrixDatum{ myMat1,myMat2,myMat3 };
			Datum myRTTIDatum{ myRTTI1,myRTTI2, myRTTI3 };


			Scope::pt_string_datum pairInt("Integer Datum", myIntDatum);
			Scope::pt_string_datum pairFloat("Float Datum", myFloatDatum);
			Scope::pt_string_datum pairString("String Datum", myStringDatum);
			Scope::pt_string_datum pairVector("Vector Datum", myVectorDatum);
			Scope::pt_string_datum pairMatrix("Matrix Datum", myMatrixDatum);
			Scope::pt_string_datum pairRTTI("RTTI Datum", myRTTIDatum);

			Scope* myScope1 = new Scope{ &pairInt, &pairFloat };
			Scope* myScope2 = new Scope{ &pairString, &pairVector };
			Scope* myScope3 = new Scope{ &pairMatrix, &pairRTTI };


			myScope2->Adopt("SC3", *myScope3); //Scope 2 holds Scope3 in a datum within its hash map.
			myScope1->Adopt("SC2", *myScope2); //Scope1 holds Scope2 in a datum within its hash map.


			Scope myStackTestScope;
			myStackTestScope.Adopt("SC1", *myScope1); //myStackTestScope holds Scope1 in a datum within its hash map.

			Assert::AreEqual(static_cast<std::size_t>(1), myStackTestScope.NumberOfItems());
			Datum* foundDatum = nullptr;

			foundDatum = myStackTestScope.SearchDown("Integer Datum");

			Assert::IsNotNull(foundDatum);
			Assert::AreEqual(myIntDatum, *foundDatum);

			foundDatum = myStackTestScope.SearchDown("Float Datum");
			Assert::IsNotNull(foundDatum);
			Assert::AreEqual(myFloatDatum, *foundDatum);


			foundDatum = myStackTestScope.SearchDown("String Datum");
			Assert::IsNotNull(foundDatum);
			Assert::AreEqual(myStringDatum, *foundDatum);


			foundDatum = myStackTestScope.SearchDown("Vector Datum");
			Assert::IsNotNull(foundDatum);
			Assert::AreEqual(myVectorDatum, *foundDatum);

			foundDatum = myStackTestScope.SearchDown("Matrix Datum");
			Assert::IsNotNull(foundDatum);
			Assert::AreEqual(myMatrixDatum, *foundDatum);

			foundDatum = myStackTestScope.SearchDown("RTTI Datum");
			Assert::IsNotNull(foundDatum);
			Assert::AreEqual(myRTTIDatum, *foundDatum);

			foundDatum = myStackTestScope.SearchDown("NOT HERE!");
			Assert::IsNull(foundDatum);



		}
		TEST_METHOD(ScopeSearchDownConst_String)
		{
			glm::vec4 myVec1(1.0f, 1.0f, 1.0f, 1.0f);
			glm::vec4 myVec2(2.0f, 2.0f, 2.0f, 2.0f);
			glm::vec4 myVec3(3.0f, 3.0f, 3.0f, 3.0f);

			glm::mat4 myMat1(1.0f);
			glm::mat4 myMat2(2.0f);
			glm::mat4 myMat3(3.0f);

			Foo myFoo1(1);
			Foo myFoo2(2);
			Foo myFoo3(3);
			RTTI* myRTTI1 = &myFoo1;
			RTTI* myRTTI2 = &myFoo2;
			RTTI* myRTTI3 = &myFoo3;

			Datum myIntDatum{ 1,2,3 };
			Datum myFloatDatum{ 1.0f,2.0f,3.0f };
			Datum myStringDatum{ "a"s,"b"s,"c"s };
			Datum myVectorDatum{ myVec1,myVec2,myVec3 };
			Datum myMatrixDatum{ myMat1,myMat2,myMat3 };
			Datum myRTTIDatum{ myRTTI1,myRTTI2, myRTTI3 };


			Scope::pt_string_datum pairInt("Integer Datum", myIntDatum);
			Scope::pt_string_datum pairFloat("Float Datum", myFloatDatum);
			Scope::pt_string_datum pairString("String Datum", myStringDatum);
			Scope::pt_string_datum pairVector("Vector Datum", myVectorDatum);
			Scope::pt_string_datum pairMatrix("Matrix Datum", myMatrixDatum);
			Scope::pt_string_datum pairRTTI("RTTI Datum", myRTTIDatum);

			Scope* myScope1 = new Scope{ &pairInt, &pairFloat };
			Scope* myScope2 = new Scope{ &pairString, &pairVector };
			Scope* myScope3 = new Scope{ &pairMatrix, &pairRTTI };


			myScope2->Adopt("SC3", *myScope3); //Scope 2 holds Scope3 in a datum within its hash map.
			myScope1->Adopt("SC2", *myScope2); //Scope1 holds Scope2 in a datum within its hash map.


			Scope::pt_string_datum pairScope("SC1", *myScope1); //Make sure we are placing a reference to a scope and not pointer. otherwise RTTI pointer method is invoked.

			const Scope myStackTestScope{ &pairScope };//myStackTestScope holds Scope1 in a datum within its hash map

			Assert::AreEqual(static_cast<std::size_t>(1), myStackTestScope.NumberOfItems());

			const Datum* foundDatum = nullptr;

			foundDatum = myStackTestScope.SearchDown("Integer Datum");

			Assert::IsNotNull(foundDatum);
			Assert::AreEqual(myIntDatum , *foundDatum);

			foundDatum = myStackTestScope.SearchDown("Float Datum");
			Assert::IsNotNull(foundDatum);
			Assert::AreEqual(myFloatDatum, *foundDatum);


			foundDatum = myStackTestScope.SearchDown("String Datum");
			Assert::IsNotNull(foundDatum);
			Assert::AreEqual(myStringDatum, *foundDatum);


			foundDatum = myStackTestScope.SearchDown("Vector Datum");
			Assert::IsNotNull(foundDatum);
			Assert::AreEqual(myVectorDatum, *foundDatum);

			foundDatum = myStackTestScope.SearchDown("Matrix Datum");
			Assert::IsNotNull(foundDatum);
			Assert::AreEqual(myMatrixDatum, *foundDatum);

			foundDatum = myStackTestScope.SearchDown("RTTI Datum");
			Assert::IsNotNull(foundDatum);
			Assert::AreEqual(myRTTIDatum, *foundDatum);

			foundDatum = myStackTestScope.SearchDown("NOT HERE!");
			Assert::IsNull(foundDatum);


		}
		TEST_METHOD(ScopeSearchUp_String_ScopePtrRef)
		{
			glm::vec4 myVec1(1.0f, 1.0f, 1.0f, 1.0f);
			glm::vec4 myVec2(2.0f, 2.0f, 2.0f, 2.0f);
			glm::vec4 myVec3(3.0f, 3.0f, 3.0f, 3.0f);

			glm::mat4 myMat1(1.0f);
			glm::mat4 myMat2(2.0f);
			glm::mat4 myMat3(3.0f);

			Foo myFoo1(1);
			Foo myFoo2(2);
			Foo myFoo3(3);
			RTTI* myRTTI1 = &myFoo1;
			RTTI* myRTTI2 = &myFoo2;
			RTTI* myRTTI3 = &myFoo3;

			Datum myIntDatum{ 1,2,3 };
			Datum myFloatDatum{ 1.0f,2.0f,3.0f };
			Datum myStringDatum{ "a"s,"b"s,"c"s };
			Datum myVectorDatum{ myVec1,myVec2,myVec3 };
			Datum myMatrixDatum{ myMat1,myMat2,myMat3 };
			Datum myRTTIDatum{ myRTTI1,myRTTI2, myRTTI3 };


			Scope::pt_string_datum pairInt("Integer Datum", myIntDatum);
			Scope::pt_string_datum pairFloat("Float Datum", myFloatDatum);
			Scope::pt_string_datum pairString("String Datum", myStringDatum);
			Scope::pt_string_datum pairVector("Vector Datum", myVectorDatum);
			Scope::pt_string_datum pairMatrix("Matrix Datum", myMatrixDatum);
			Scope::pt_string_datum pairRTTI("RTTI Datum", myRTTIDatum);

			Scope* myScope1 = new Scope{ &pairInt, &pairFloat };
			Scope* myScope2 = new Scope{ &pairString, &pairVector };
			Scope* myScope3 = new Scope{ &pairMatrix, &pairRTTI };


			myScope2->Adopt("SC3", *myScope3); //Scope 2 holds Scope3 in a datum within its hash map.
			myScope1->Adopt("SC2", *myScope2); //Scope1 holds Scope2 in a datum within its hash map.


			Scope myStackTestScope;
			myStackTestScope.Adopt("SC1", *myScope1); //myStackTestScope holds Scope1 in a datum within its hash map.

			Assert::AreEqual(static_cast<std::size_t>(1), myStackTestScope.NumberOfItems());
			Datum* foundDatum = nullptr;
			Scope* locatedScope = nullptr;

			//Searching from most nested scope. Third layer.
			{
				foundDatum = myScope3->SearchUp("NOT HERE!", &locatedScope);
				Assert::IsNull(foundDatum);
				Assert::IsNull(locatedScope);
			}


			//Searching from third layer to first layer. Most nested scope to most surface level.
			{

				foundDatum = myScope3->SearchUp("Integer Datum", &locatedScope); //Starting from the most nested scope to look for the highest scope
				Assert::IsNotNull(foundDatum);
				Assert::IsNotNull(locatedScope);

				Assert::AreEqual(myIntDatum, *foundDatum);
				Assert::AreSame(*myScope1, *locatedScope);


				foundDatum = myScope3->SearchUp("Float Datum", &locatedScope); //Starting from the most nested scope to look for the highest scope
				Assert::IsNotNull(foundDatum);
				Assert::IsNotNull(locatedScope);

				Assert::AreEqual(myFloatDatum, *foundDatum);
				Assert::AreSame(*myScope1, *locatedScope);
			}

			//Searching from second layer to first layer
			{

				foundDatum = myScope2->SearchUp("Integer Datum", &locatedScope);
				Assert::IsNotNull(foundDatum);
				Assert::IsNotNull(locatedScope);

				Assert::AreEqual(myIntDatum, *foundDatum);
				Assert::AreSame(*myScope1, *locatedScope);

				foundDatum = myScope2->SearchUp("Float Datum", &locatedScope);
				Assert::IsNotNull(foundDatum);
				Assert::IsNotNull(locatedScope);

				Assert::AreEqual(myFloatDatum, *foundDatum);
				Assert::AreSame(*myScope1, *locatedScope);
			}

			//Searching locally from first layer scope
			{

				foundDatum = myScope1->SearchUp("Integer Datum", &locatedScope);
				Assert::IsNotNull(foundDatum);
				Assert::IsNotNull(locatedScope);

				Assert::AreEqual(myIntDatum, *foundDatum);
				Assert::AreSame(*myScope1, *locatedScope);

				foundDatum = myScope1->SearchUp("Float Datum", &locatedScope);
				Assert::IsNotNull(foundDatum);
				Assert::IsNotNull(locatedScope);

				Assert::AreEqual(myFloatDatum, *foundDatum);
				Assert::AreSame(*myScope1, *locatedScope);
			}

		}
		TEST_METHOD(ScopeSearchUpConst_String_ScopePtrRef)
		{
			glm::vec4 myVec1(1.0f, 1.0f, 1.0f, 1.0f);
			glm::vec4 myVec2(2.0f, 2.0f, 2.0f, 2.0f);
			glm::vec4 myVec3(3.0f, 3.0f, 3.0f, 3.0f);

			glm::mat4 myMat1(1.0f);
			glm::mat4 myMat2(2.0f);
			glm::mat4 myMat3(3.0f);

			Foo myFoo1(1);
			Foo myFoo2(2);
			Foo myFoo3(3);
			RTTI* myRTTI1 = &myFoo1;
			RTTI* myRTTI2 = &myFoo2;
			RTTI* myRTTI3 = &myFoo3;

			Datum myIntDatum{ 1,2,3 };
			Datum myFloatDatum{ 1.0f,2.0f,3.0f };
			Datum myStringDatum{ "a"s,"b"s,"c"s };
			Datum myVectorDatum{ myVec1,myVec2,myVec3 };
			Datum myMatrixDatum{ myMat1,myMat2,myMat3 };
			Datum myRTTIDatum{ myRTTI1,myRTTI2, myRTTI3 };


			Scope::pt_string_datum pairInt("Integer Datum", myIntDatum);
			Scope::pt_string_datum pairFloat("Float Datum", myFloatDatum);
			Scope::pt_string_datum pairString("String Datum", myStringDatum);
			Scope::pt_string_datum pairVector("Vector Datum", myVectorDatum);
			Scope::pt_string_datum pairMatrix("Matrix Datum", myMatrixDatum);
			Scope::pt_string_datum pairRTTI("RTTI Datum", myRTTIDatum);

			Scope* myScope1 = new Scope{ &pairInt, &pairFloat };
			Scope* myScope2 = new Scope{ &pairString, &pairVector };
			Scope* myScope3 = new Scope{ &pairMatrix, &pairRTTI };


			myScope2->Adopt("SC3", *myScope3); //Scope 2 holds Scope3 in a datum within its hash map.
			myScope1->Adopt("SC2", *myScope2); //Scope1 holds Scope2 in a datum within its hash map.


			Scope::pt_string_datum pairScope("SC1", *myScope1); //Make sure you are passing Scope reference and not pointer. Otherwise RTTI methods are invoked.

			const Scope myStackTestScope{ &pairScope };//myStackTestScope holds Scope1 in a datum within its hash map

			Assert::AreEqual(static_cast<std::size_t>(1), myStackTestScope.NumberOfItems());
			const Datum* foundDatum = nullptr;
			const Scope* locatedScope = nullptr;

			//Searching from most nested scope. Third layer.
			{
				const Scope& myScope3Const = *myScope3;
				foundDatum = myScope3Const.SearchUp("NOT HERE!", &locatedScope);
				Assert::IsNull(foundDatum);
				Assert::IsNull(locatedScope);
			}


			//Searching from third layer to first layer. Most nested scope to most surface level.
			{
				const Scope& myScope3Const = *myScope3;
				foundDatum = myScope3Const.SearchUp("Integer Datum", &locatedScope); //Starting from the most nested scope to look for the highest scope
				Assert::IsNotNull(foundDatum);
				Assert::IsNotNull(locatedScope);

				Assert::AreEqual(myIntDatum, *foundDatum);
				Assert::AreSame(*myScope1, *locatedScope);


				foundDatum = myScope3Const.SearchUp("Float Datum", &locatedScope); //Starting from the most nested scope to look for the highest scope
				Assert::IsNotNull(foundDatum);
				Assert::IsNotNull(locatedScope);

				Assert::AreEqual(myFloatDatum, *foundDatum);
				Assert::AreSame(*myScope1, *locatedScope);
			}

			//Searching from second layer to first layer
			{
				const Scope& myScope2Const = *myScope2;

				foundDatum = myScope2Const.SearchUp("Integer Datum", &locatedScope);
				Assert::IsNotNull(foundDatum);
				Assert::IsNotNull(locatedScope);

				Assert::AreEqual(myIntDatum, *foundDatum);
				Assert::AreSame(*myScope1, *locatedScope);

				foundDatum = myScope2Const.SearchUp("Float Datum", &locatedScope);
				Assert::IsNotNull(foundDatum);
				Assert::IsNotNull(locatedScope);

				Assert::AreEqual(myFloatDatum, *foundDatum);
				Assert::AreSame(*myScope1, *locatedScope);
			}

			//Searching locally from first layer scope
			{
				const Scope& myScope1Const = *myScope1;

				foundDatum = myScope1Const.SearchUp("Integer Datum", &locatedScope);
				Assert::IsNotNull(foundDatum);
				Assert::IsNotNull(locatedScope);

				Assert::AreEqual(myIntDatum, *foundDatum);
				Assert::AreSame(*myScope1, *locatedScope);

				foundDatum = myScope1Const.SearchUp("Float Datum", &locatedScope);
				Assert::IsNotNull(foundDatum);
				Assert::IsNotNull(locatedScope);

				Assert::AreEqual(myFloatDatum, *foundDatum);
				Assert::AreSame(*myScope1, *locatedScope);
			}
		}
		TEST_METHOD(ScopeSearchUp_String)
		{
			glm::vec4 myVec1(1.0f, 1.0f, 1.0f, 1.0f);
			glm::vec4 myVec2(2.0f, 2.0f, 2.0f, 2.0f);
			glm::vec4 myVec3(3.0f, 3.0f, 3.0f, 3.0f);

			glm::mat4 myMat1(1.0f);
			glm::mat4 myMat2(2.0f);
			glm::mat4 myMat3(3.0f);

			Foo myFoo1(1);
			Foo myFoo2(2);
			Foo myFoo3(3);
			RTTI* myRTTI1 = &myFoo1;
			RTTI* myRTTI2 = &myFoo2;
			RTTI* myRTTI3 = &myFoo3;

			Datum myIntDatum{ 1,2,3 };
			Datum myFloatDatum{ 1.0f,2.0f,3.0f };
			Datum myStringDatum{ "a"s,"b"s,"c"s };
			Datum myVectorDatum{ myVec1,myVec2,myVec3 };
			Datum myMatrixDatum{ myMat1,myMat2,myMat3 };
			Datum myRTTIDatum{ myRTTI1,myRTTI2, myRTTI3 };


			Scope::pt_string_datum pairInt("Integer Datum", myIntDatum);
			Scope::pt_string_datum pairFloat("Float Datum", myFloatDatum);
			Scope::pt_string_datum pairString("String Datum", myStringDatum);
			Scope::pt_string_datum pairVector("Vector Datum", myVectorDatum);
			Scope::pt_string_datum pairMatrix("Matrix Datum", myMatrixDatum);
			Scope::pt_string_datum pairRTTI("RTTI Datum", myRTTIDatum);

			Scope* myScope1 = new Scope{ &pairInt, &pairFloat };
			Scope* myScope2 = new Scope{ &pairString, &pairVector };
			Scope* myScope3 = new Scope{ &pairMatrix, &pairRTTI };


			myScope2->Adopt("SC3", *myScope3); //Scope 2 holds Scope3 in a datum within its hash map.
			myScope1->Adopt("SC2", *myScope2); //Scope1 holds Scope2 in a datum within its hash map.


			Scope myStackTestScope;
			myStackTestScope.Adopt("SC1", *myScope1); //myStackTestScope holds Scope1 in a datum within its hash map.

			Assert::AreEqual(static_cast<std::size_t>(1), myStackTestScope.NumberOfItems());
			const Datum* foundDatum = nullptr;

			//Searching from most nested scope. Third layer.
			{
				const Scope& myScope3Const = *myScope3;
				foundDatum = myScope3Const.SearchUp("NOT HERE!");
				Assert::IsNull(foundDatum);
			}


			//Searching from third layer to first layer. Most nested scope to most surface level.
			{
				const Scope& myScope3Const = *myScope3;
				foundDatum = myScope3Const.SearchUp("Integer Datum"); //Starting from the most nested scope to look for the highest scope
				Assert::IsNotNull(foundDatum);
				Assert::AreEqual(myIntDatum, *foundDatum);

				foundDatum = myScope3Const.SearchUp("Float Datum"); //Starting from the most nested scope to look for the highest scope
				Assert::IsNotNull(foundDatum);
				Assert::AreEqual(myFloatDatum, *foundDatum);
			}

			//Searching from second layer to first layer
			{
				const Scope& myScope2Const = *myScope2;

				foundDatum = myScope2Const.SearchUp("Integer Datum");
				Assert::IsNotNull(foundDatum);
				Assert::AreEqual(myIntDatum, *foundDatum);

				foundDatum = myScope2Const.SearchUp("Float Datum");
				Assert::IsNotNull(foundDatum);
				Assert::AreEqual(myFloatDatum, *foundDatum);
			}

			//Searching locally from first layer scope
			{
				const Scope& myScope1Const = *myScope1;

				foundDatum = myScope1Const.SearchUp("Integer Datum");
				Assert::IsNotNull(foundDatum);
				Assert::AreEqual(myIntDatum, *foundDatum);

				foundDatum = myScope1Const.SearchUp("Float Datum");
				Assert::IsNotNull(foundDatum);
				Assert::AreEqual(myFloatDatum, *foundDatum);
			}


		}
		TEST_METHOD(ScopeSearchUpConst_String)
		{
			glm::vec4 myVec1(1.0f, 1.0f, 1.0f, 1.0f);
			glm::vec4 myVec2(2.0f, 2.0f, 2.0f, 2.0f);
			glm::vec4 myVec3(3.0f, 3.0f, 3.0f, 3.0f);

			glm::mat4 myMat1(1.0f);
			glm::mat4 myMat2(2.0f);
			glm::mat4 myMat3(3.0f);

			Foo myFoo1(1);
			Foo myFoo2(2);
			Foo myFoo3(3);
			RTTI* myRTTI1 = &myFoo1;
			RTTI* myRTTI2 = &myFoo2;
			RTTI* myRTTI3 = &myFoo3;

			Datum myIntDatum{ 1,2,3 };
			Datum myFloatDatum{ 1.0f,2.0f,3.0f };
			Datum myStringDatum{ "a"s,"b"s,"c"s };
			Datum myVectorDatum{ myVec1,myVec2,myVec3 };
			Datum myMatrixDatum{ myMat1,myMat2,myMat3 };
			Datum myRTTIDatum{ myRTTI1,myRTTI2, myRTTI3 };


			Scope::pt_string_datum pairInt("Integer Datum", myIntDatum);
			Scope::pt_string_datum pairFloat("Float Datum", myFloatDatum);
			Scope::pt_string_datum pairString("String Datum", myStringDatum);
			Scope::pt_string_datum pairVector("Vector Datum", myVectorDatum);
			Scope::pt_string_datum pairMatrix("Matrix Datum", myMatrixDatum);
			Scope::pt_string_datum pairRTTI("RTTI Datum", myRTTIDatum);

			Scope* myScope1 = new Scope{ &pairInt, &pairFloat };
			Scope* myScope2 = new Scope{ &pairString, &pairVector };
			Scope* myScope3 = new Scope{ &pairMatrix, &pairRTTI };


			myScope2->Adopt("SC3", *myScope3); //Scope 2 holds Scope3 in a datum within its hash map.
			myScope1->Adopt("SC2", *myScope2); //Scope1 holds Scope2 in a datum within its hash map.


			Scope::pt_string_datum pairScope("SC1", *myScope1); //Make sure you are passing Scope reference and not pointer. Otherwise RTTI methods are invoked.

			const Scope myStackTestScope{ &pairScope };//myStackTestScope holds Scope1 in a datum within its hash map

			Assert::AreEqual(static_cast<std::size_t>(1), myStackTestScope.NumberOfItems());
			Datum* foundDatum = nullptr;

			//Searching from most nested scope. Third layer.
			{
				foundDatum = myScope3->SearchUp("NOT HERE!");
				Assert::IsNull(foundDatum);
			}


			//Searching from third layer to first layer. Most nested scope to most surface level.
			{

				foundDatum = myScope3->SearchUp("Integer Datum"); //Starting from the most nested scope to look for the highest scope
				Assert::IsNotNull(foundDatum);
				Assert::AreEqual(myIntDatum, *foundDatum);

				foundDatum = myScope3->SearchUp("Float Datum"); //Starting from the most nested scope to look for the highest scope
				Assert::IsNotNull(foundDatum);
				Assert::AreEqual(myFloatDatum, *foundDatum);
			}

			//Searching from second layer to first layer
			{

				foundDatum = myScope2->SearchUp("Integer Datum");
				Assert::IsNotNull(foundDatum);
				Assert::AreEqual(myIntDatum, *foundDatum);

				foundDatum = myScope2->SearchUp("Float Datum");
				Assert::IsNotNull(foundDatum);
				Assert::AreEqual(myFloatDatum, *foundDatum);
			}

			//Searching locally from first layer scope
			{

				foundDatum = myScope1->SearchUp("Integer Datum");
				Assert::IsNotNull(foundDatum);
				Assert::AreEqual(myIntDatum, *foundDatum);

				foundDatum = myScope1->SearchUp("Float Datum");
				Assert::IsNotNull(foundDatum);
				Assert::AreEqual(myFloatDatum, *foundDatum);
			}

		}
		TEST_METHOD(ScopeAppend_String)
		{	
			Scope myScope;
			myScope.Append("First");
			myScope.Append("Second");
			myScope.Append("Third");

			Assert::AreEqual(static_cast<std::size_t>(3), myScope.NumberOfItems());
			Assert::AreEqual(myScope[0].Type(), DatumTypes::Unknown);
			Assert::AreEqual(myScope[1].Type(), DatumTypes::Unknown);
			Assert::AreEqual(myScope[2].Type(), DatumTypes::Unknown);

			myScope[0].SetType(DatumTypes::Table);
			myScope[1].SetType(DatumTypes::Pointer);
			myScope[2].SetType(DatumTypes::Matrix);

			Assert::AreEqual(myScope[0].Type(), DatumTypes::Table);
			Assert::AreEqual(myScope[1].Type(), DatumTypes::Pointer);
			Assert::AreEqual(myScope[2].Type(), DatumTypes::Matrix);
		}
		TEST_METHOD(ScopeAppend_String_DatumRef)
		{
			glm::vec4 myVec1(1.0f, 1.0f, 1.0f, 1.0f);
			glm::vec4 myVec2(2.0f, 2.0f, 2.0f, 2.0f);
			glm::vec4 myVec3(3.0f, 3.0f, 3.0f, 3.0f);

			glm::mat4 myMat1(1.0f);
			glm::mat4 myMat2(2.0f);
			glm::mat4 myMat3(3.0f);

			Foo myFoo1(1);
			Foo myFoo2(2);
			Foo myFoo3(3);
			RTTI* myRTTI1 = &myFoo1;
			RTTI* myRTTI2 = &myFoo2;
			RTTI* myRTTI3 = &myFoo3;

			Datum myIntDatum{ 1,2,3 };
			Datum myFloatDatum{ 1.0f,2.0f,3.0f };
			Datum myStringDatum{ "a"s,"b"s,"c"s };
			Datum myVectorDatum{ myVec1,myVec2,myVec3 };
			Datum myMatrixDatum{ myMat1,myMat2,myMat3 };
			Datum myRTTIDatum{ myRTTI1,myRTTI2, myRTTI3 };


			Scope::pt_string_datum pairInt("Integer Datum", myIntDatum);
			Scope::pt_string_datum pairFloat("Float Datum", myFloatDatum);
			Scope::pt_string_datum pairString("String Datum", myStringDatum);
			Scope::pt_string_datum pairVector("Vector Datum", myVectorDatum);
			Scope::pt_string_datum pairMatrix("Matrix Datum", myMatrixDatum);
			Scope::pt_string_datum pairRTTI("RTTI Datum", myRTTIDatum);

			Scope* myScope1 = new Scope{ &pairInt, &pairFloat };
			Scope* myScope2 = new Scope{ &pairString, &pairVector };
			Scope* myScope3 = new Scope{ &pairMatrix, &pairRTTI };

			Datum myScopeDatum{ myScope1,myScope2, myScope3 };
			Scope::pt_string_datum pairScope("Scope Datum", myScopeDatum);

			Scope myStackTestScope;

			myStackTestScope.Append("First",   myIntDatum);
			myStackTestScope.Append("Second",  myFloatDatum);
			myStackTestScope.Append("Third",   myStringDatum);
			myStackTestScope.Append("Fourth",  myVectorDatum);
			myStackTestScope.Append("Fifth",   myMatrixDatum);
			myStackTestScope.Append("Sixth",   myRTTIDatum);
			myStackTestScope.Append("Seventh", myScopeDatum);

			Assert::AreEqual(static_cast<std::size_t>(7), myStackTestScope.NumberOfItems());
			Assert::AreEqual(myStackTestScope[0].Type(), DatumTypes::Integer);
			Assert::AreEqual(myStackTestScope[1].Type(), DatumTypes::Float);
			Assert::AreEqual(myStackTestScope[2].Type(), DatumTypes::String);
			Assert::AreEqual(myStackTestScope[3].Type(), DatumTypes::Vector4);
			Assert::AreEqual(myStackTestScope[4].Type(), DatumTypes::Matrix);
			Assert::AreEqual(myStackTestScope[5].Type(), DatumTypes::Pointer);
			Assert::AreEqual(myStackTestScope[6].Type(), DatumTypes::Table);

			Assert::AreEqual(myStackTestScope[0], myIntDatum);
			Assert::AreEqual(myStackTestScope[1], myFloatDatum);
			Assert::AreEqual(myStackTestScope[2], myStringDatum);
			Assert::AreEqual(myStackTestScope[3], myVectorDatum);
			Assert::AreEqual(myStackTestScope[4], myMatrixDatum);
			Assert::AreEqual(myStackTestScope[5], myRTTIDatum);
			Assert::AreEqual(myStackTestScope[6], myScopeDatum);


		}
		TEST_METHOD(ScopeAppend_Pair_String_Datum)
		{
			glm::vec4 myVec1(1.0f, 1.0f, 1.0f, 1.0f);
			glm::vec4 myVec2(2.0f, 2.0f, 2.0f, 2.0f);
			glm::vec4 myVec3(3.0f, 3.0f, 3.0f, 3.0f);

			glm::mat4 myMat1(1.0f);
			glm::mat4 myMat2(2.0f);
			glm::mat4 myMat3(3.0f);

			Foo myFoo1(1);
			Foo myFoo2(2);
			Foo myFoo3(3);
			RTTI* myRTTI1 = &myFoo1;
			RTTI* myRTTI2 = &myFoo2;
			RTTI* myRTTI3 = &myFoo3;

			Datum myIntDatum{ 1,2,3 };
			Datum myFloatDatum{ 1.0f,2.0f,3.0f };
			Datum myStringDatum{ "a"s,"b"s,"c"s };
			Datum myVectorDatum{ myVec1,myVec2,myVec3 };
			Datum myMatrixDatum{ myMat1,myMat2,myMat3 };
			Datum myRTTIDatum{ myRTTI1,myRTTI2, myRTTI3 };


			Scope::pt_string_datum pairInt("Integer Datum", myIntDatum);
			Scope::pt_string_datum pairFloat("Float Datum", myFloatDatum);
			Scope::pt_string_datum pairString("String Datum", myStringDatum);
			Scope::pt_string_datum pairVector("Vector Datum", myVectorDatum);
			Scope::pt_string_datum pairMatrix("Matrix Datum", myMatrixDatum);
			Scope::pt_string_datum pairRTTI("RTTI Datum", myRTTIDatum);

			Scope* myScope1 = new Scope{ &pairInt, &pairFloat };
			Scope* myScope2 = new Scope{ &pairString, &pairVector };
			Scope* myScope3 = new Scope{ &pairMatrix, &pairRTTI };

			Datum myScopeDatum{ myScope1,myScope2, myScope3 };
			Scope::pt_string_datum pairScope("Scope Datum", myScopeDatum);


			Scope myStackTestScope;

			myStackTestScope.Append(pairInt);
			myStackTestScope.Append(pairFloat);
			myStackTestScope.Append(pairString);
			myStackTestScope.Append(pairVector);
			myStackTestScope.Append(pairMatrix);
			myStackTestScope.Append(pairRTTI);
			myStackTestScope.Append(pairScope);

			Assert::AreEqual(static_cast<std::size_t>(7), myStackTestScope.NumberOfItems());
			Assert::AreEqual(myStackTestScope[0], myIntDatum);
			Assert::AreEqual(myStackTestScope[1], myFloatDatum);
			Assert::AreEqual(myStackTestScope[2], myStringDatum);
			Assert::AreEqual(myStackTestScope[3], myVectorDatum);
			Assert::AreEqual(myStackTestScope[4], myMatrixDatum);
			Assert::AreEqual(myStackTestScope[5], myRTTIDatum);
			Assert::AreEqual(myStackTestScope[6], myScopeDatum);

			Scope myCopyScope;
			myCopyScope = myStackTestScope;

			Assert::AreEqual(static_cast<std::size_t>(7), myCopyScope.NumberOfItems());
			Assert::AreEqual(myCopyScope[0], myIntDatum);
			Assert::AreEqual(myCopyScope[1], myFloatDatum);
			Assert::AreEqual(myCopyScope[2], myStringDatum);
			Assert::AreEqual(myCopyScope[3], myVectorDatum);
			Assert::AreEqual(myCopyScope[4], myMatrixDatum);
			Assert::AreEqual(myCopyScope[5], myRTTIDatum);
			Assert::AreEqual(myCopyScope[6], myScopeDatum);

		}
		TEST_METHOD(ScopeAppendScope)
		{
			//Creating Unique Scopes with Unique Names
			{
				Scope myScope;
				myScope.AppendScope("First");
				myScope.AppendScope("Second");
				myScope.AppendScope("Third");

				Assert::AreEqual(static_cast<std::size_t>(3), myScope.NumberOfItems());
				Assert::AreEqual(myScope[0].Type(), DatumTypes::Table);
				Assert::AreEqual(myScope[1].Type(), DatumTypes::Table);
				Assert::AreEqual(myScope[2].Type(), DatumTypes::Table);

			}

			//Creating Unique Scopes with Name that already exists to another scope
			{
				Scope myScope;
				myScope.AppendScope("First");
				myScope.AppendScope("First");
				myScope.AppendScope("First");

				Assert::AreEqual(static_cast<std::size_t>(1), myScope.NumberOfItems());
				Assert::AreEqual(myScope[0].Type(), DatumTypes::Table);
			}

			//Appending Scope to a datum of type Unknown
			{
				Scope myScope;
				myScope.Append("First");

				myScope.AppendScope("First");
				myScope.AppendScope("First");
				myScope.AppendScope("First");

				Assert::AreEqual(static_cast<std::size_t>(1), myScope.NumberOfItems());
				Assert::AreEqual(myScope[0].Type(), DatumTypes::Table);

			}

			//Appending Scope to a datum of type NOT Unknown and different than Table
			{
				Datum myIntDatm{ 1,2,3 };
				Scope myScope;
				myScope.Append("First", myIntDatm);

				auto Expected = [&myScope]() {myScope.AppendScope("First");};
				Assert::ExpectException<std::runtime_error>(Expected);

			}

		}
		TEST_METHOD(ScopeAdopt)
		{
			glm::vec4 myVec1(1.0f, 1.0f, 1.0f, 1.0f);
			glm::vec4 myVec2(2.0f, 2.0f, 2.0f, 2.0f);
			glm::vec4 myVec3(3.0f, 3.0f, 3.0f, 3.0f);

			glm::mat4 myMat1(1.0f);
			glm::mat4 myMat2(2.0f);
			glm::mat4 myMat3(3.0f);

			Foo myFoo1(1);
			Foo myFoo2(2);
			Foo myFoo3(3);
			RTTI* myRTTI1 = &myFoo1;
			RTTI* myRTTI2 = &myFoo2;
			RTTI* myRTTI3 = &myFoo3;

			Datum myIntDatum{ 1,2,3 };
			Datum myFloatDatum{ 1.0f,2.0f,3.0f };
			Datum myStringDatum{ "a"s,"b"s,"c"s };
			Datum myVectorDatum{ myVec1,myVec2,myVec3 };
			Datum myMatrixDatum{ myMat1,myMat2,myMat3 };
			Datum myRTTIDatum{ myRTTI1,myRTTI2, myRTTI3 };


			Scope::pt_string_datum pairInt("Integer Datum", myIntDatum);
			Scope::pt_string_datum pairFloat("Float Datum", myFloatDatum);
			Scope::pt_string_datum pairString("String Datum", myStringDatum);
			Scope::pt_string_datum pairVector("Vector Datum", myVectorDatum);
			Scope::pt_string_datum pairMatrix("Matrix Datum", myMatrixDatum);
			Scope::pt_string_datum pairRTTI("RTTI Datum", myRTTIDatum);


			Scope myStackTestScope;

			Scope& myScopeLayer1 = myStackTestScope.AppendScope("Layer 1");
			Scope& myScopeLayer2 = myScopeLayer1.AppendScope("Layer 2");
			Scope& myScopeLayer3 = myScopeLayer2.AppendScope("Layer 3");

			myScopeLayer1.Append(pairInt);
			myScopeLayer1.Append(pairFloat);

			myScopeLayer2.Append(pairString);
			myScopeLayer2.Append(pairVector);

			myScopeLayer3.Append(pairMatrix);
			myScopeLayer3.Append(pairRTTI);


			Assert::AreEqual(&myScopeLayer2, myScopeLayer3.GetParent());
			Assert::AreEqual(&myScopeLayer1, myScopeLayer2.GetParent());
			Assert::AreEqual(&myStackTestScope, myScopeLayer1.GetParent());
			Assert::IsNull(myStackTestScope.GetParent());


			Scope& myOrphanedScopeLayer1 = myScopeLayer1.Orphan().first;

			Assert::AreEqual(&myScopeLayer2, myScopeLayer3.GetParent());
			Assert::AreEqual(&myScopeLayer1, myScopeLayer2.GetParent());
			Assert::IsNull(myScopeLayer1.GetParent());
			Assert::IsNull(myStackTestScope.GetParent());

			Scope& myOrphanedScopeLayer2 = myScopeLayer2.Orphan().first;

			Assert::AreEqual(&myScopeLayer2, myScopeLayer3.GetParent());
			Assert::IsNull(myScopeLayer2.GetParent());
			Assert::IsNull(myScopeLayer1.GetParent());
			Assert::IsNull(myStackTestScope.GetParent());

			Scope& myOrphanedScopeLayer3 = myScopeLayer3.Orphan().first;

			Assert::IsNull(myScopeLayer3.GetParent());
			Assert::IsNull(myScopeLayer2.GetParent());
			Assert::IsNull(myScopeLayer1.GetParent());
			Assert::IsNull(myStackTestScope.GetParent());

			myOrphanedScopeLayer2.Adopt("Layer 3", myOrphanedScopeLayer3);
			myOrphanedScopeLayer1.Adopt("Layer 2", myOrphanedScopeLayer2);
			myStackTestScope.Adopt("Layer 1", myOrphanedScopeLayer1);

			Assert::AreEqual(&myScopeLayer2, myScopeLayer3.GetParent());
			Assert::AreEqual(&myScopeLayer1, myScopeLayer2.GetParent());
			Assert::AreEqual(&myStackTestScope, myScopeLayer1.GetParent());
			Assert::IsNull(myStackTestScope.GetParent());

			//Adopting a scope that already has a parent
			{

				Scope::pt_datumPtr_index pair = myScopeLayer2.FindContainedScope(myScopeLayer3); //datumPtr in pair will contain valid ptr.
				Datum* foundDatum = pair.first;
				Assert::IsNotNull(foundDatum);


				myStackTestScope.Adopt("Layer 1", myScopeLayer3); //Taking the most nested scope and setting it to be adopted by a scope up the hierarchy.
																  //We have removed myScopeLayer3 from myScopeLayer2.

				pair = myScopeLayer2.FindContainedScope(myScopeLayer3); //datumPtr in pair will be nullptr because it was not found.
				foundDatum = pair.first;
				Assert::IsNull(foundDatum);

				Assert::AreEqual(&myStackTestScope, myScopeLayer3.GetParent()); //myScopeLayer3 is a direct child of myStackTestScope
				Assert::AreEqual(&myStackTestScope, myScopeLayer1.GetParent());

				Assert::AreEqual(&myScopeLayer1, myScopeLayer2.GetParent()); //myScopeLayer2 is currently the most nested Scope as it exists within myScopeLayer1
				Assert::IsNull(myStackTestScope.GetParent());




				Scope myOtherStackTestScope;
				myOtherStackTestScope.Adopt("Layer 1", myScopeLayer2); //Adopting a nested scope from another scope to completely different scope.
				
				Assert::AreEqual(&myOtherStackTestScope, myScopeLayer2.GetParent());
				Assert::IsNull(myOtherStackTestScope.GetParent());

				pair = myScopeLayer1.FindContainedScope(myScopeLayer2); //datumPtr in pair will be nullptr because it was not found.
				foundDatum = pair.first;
				Assert::IsNull(foundDatum);
			}

			//Attempting to adopt scope into datum whose set type is Unknown or Table
			{
				Scope myOtherStackTestScope;
				Datum& myDatum = myOtherStackTestScope.Append("I'm Int Type");
				myDatum.SetType(DatumTypes::Integer);

				auto Expected = [&myOtherStackTestScope, &myScopeLayer1]() {myOtherStackTestScope.Adopt("I'm Int Type", myScopeLayer1); };
				Assert::ExpectException<std::runtime_error>(Expected);
			}
		}
		TEST_METHOD(ScopeGetParent)
		{
			glm::vec4 myVec1(1.0f, 1.0f, 1.0f, 1.0f);
			glm::vec4 myVec2(2.0f, 2.0f, 2.0f, 2.0f);
			glm::vec4 myVec3(3.0f, 3.0f, 3.0f, 3.0f);

			glm::mat4 myMat1(1.0f);
			glm::mat4 myMat2(2.0f);
			glm::mat4 myMat3(3.0f);

			Foo myFoo1(1);
			Foo myFoo2(2);
			Foo myFoo3(3);
			RTTI* myRTTI1 = &myFoo1;
			RTTI* myRTTI2 = &myFoo2;
			RTTI* myRTTI3 = &myFoo3;

			Datum myIntDatum{ 1,2,3 };
			Datum myFloatDatum{ 1.0f,2.0f,3.0f };
			Datum myStringDatum{ "a"s,"b"s,"c"s };
			Datum myVectorDatum{ myVec1,myVec2,myVec3 };
			Datum myMatrixDatum{ myMat1,myMat2,myMat3 };
			Datum myRTTIDatum{ myRTTI1,myRTTI2, myRTTI3 };


			Scope::pt_string_datum pairInt("Integer Datum", myIntDatum);
			Scope::pt_string_datum pairFloat("Float Datum", myFloatDatum);
			Scope::pt_string_datum pairString("String Datum", myStringDatum);
			Scope::pt_string_datum pairVector("Vector Datum", myVectorDatum);
			Scope::pt_string_datum pairMatrix("Matrix Datum", myMatrixDatum);
			Scope::pt_string_datum pairRTTI("RTTI Datum", myRTTIDatum);

			Scope* myScope1 = new Scope{ &pairInt, &pairFloat };
			Scope* myScope2 = new Scope{ &pairString, &pairVector };
			Scope* myScope3 = new Scope{ &pairMatrix, &pairRTTI };


			myScope2->Adopt("SC3", *myScope3); //Scope 2 holds Scope3 in a datum within its hash map.
			myScope1->Adopt("SC2", *myScope2); //Scope1 holds Scope2 in a datum within its hash map.


			Scope myStackTestScope;
			myStackTestScope.Adopt("SC1", *myScope1); //myStackTestScope holds Scope1 in a datum within its hash map.

			Assert::AreSame(*myScope2, *myScope3->GetParent());
			Assert::AreSame(*myScope1, *myScope2->GetParent());
			Assert::AreSame(myStackTestScope, *myScope1->GetParent());
			Assert::IsNull(myStackTestScope.GetParent());
		}
		TEST_METHOD(ScopeOOBracketBracket_String)
		{
			Scope myScope;
			myScope["First"];
			myScope["Second"];
			myScope["Third"];

			Assert::AreEqual(static_cast<std::size_t>(3), myScope.NumberOfItems());
			Assert::AreEqual(myScope[0].Type(), DatumTypes::Unknown);
			Assert::AreEqual(myScope[1].Type(), DatumTypes::Unknown);
			Assert::AreEqual(myScope[2].Type(), DatumTypes::Unknown);

			myScope[0].SetType(DatumTypes::Table);
			myScope[1].SetType(DatumTypes::Pointer);
			myScope[2].SetType(DatumTypes::Matrix);

			Assert::AreEqual(myScope[0].Type(), DatumTypes::Table);
			Assert::AreEqual(myScope[1].Type(), DatumTypes::Pointer);
			Assert::AreEqual(myScope[2].Type(), DatumTypes::Matrix);

		}
		TEST_METHOD(ScopeOOBracketBracket_unint32_t)
		{
			glm::vec4 myVec1(1.0f, 1.0f, 1.0f, 1.0f);
			glm::vec4 myVec2(2.0f, 2.0f, 2.0f, 2.0f);
			glm::vec4 myVec3(3.0f, 3.0f, 3.0f, 3.0f);

			glm::mat4 myMat1(1.0f);
			glm::mat4 myMat2(2.0f);
			glm::mat4 myMat3(3.0f);

			Foo myFoo1(1);
			Foo myFoo2(2);
			Foo myFoo3(3);
			RTTI* myRTTI1 = &myFoo1;
			RTTI* myRTTI2 = &myFoo2;
			RTTI* myRTTI3 = &myFoo3;

			Datum myIntDatum{ 1,2,3 };
			Datum myFloatDatum{ 1.0f,2.0f,3.0f };
			Datum myStringDatum{ "a"s,"b"s,"c"s };
			Datum myVectorDatum{ myVec1,myVec2,myVec3 };
			Datum myMatrixDatum{ myMat1,myMat2,myMat3 };
			Datum myRTTIDatum{ myRTTI1,myRTTI2, myRTTI3 };


			Scope::pt_string_datum pairInt("Integer Datum", myIntDatum);
			Scope::pt_string_datum pairFloat("Float Datum", myFloatDatum);
			Scope::pt_string_datum pairString("String Datum", myStringDatum);
			Scope::pt_string_datum pairVector("Vector Datum", myVectorDatum);
			Scope::pt_string_datum pairMatrix("Matrix Datum", myMatrixDatum);
			Scope::pt_string_datum pairRTTI("RTTI Datum", myRTTIDatum);

			Scope* myScope1 = new Scope{ &pairInt, &pairFloat };
			Scope* myScope2 = new Scope{ &pairString, &pairVector };
			Scope* myScope3 = new Scope{ &pairMatrix, &pairRTTI };

			Datum myScopeDatum{ myScope1,myScope2, myScope3 };
			Scope::pt_string_datum pairScope("Scope Datum", myScopeDatum);

			Scope myStackTestScope;

			myStackTestScope.Append("First", myIntDatum);
			myStackTestScope.Append("Second", myFloatDatum);
			myStackTestScope.Append("Third", myStringDatum);
			myStackTestScope.Append("Fourth", myVectorDatum);
			myStackTestScope.Append("Fifth", myMatrixDatum);
			myStackTestScope.Append("Sixth", myRTTIDatum);
			myStackTestScope.Append("Seventh", myScopeDatum);

			Assert::AreEqual(static_cast<std::size_t>(7), myStackTestScope.NumberOfItems());
			Assert::AreEqual(myStackTestScope[0].Type(), DatumTypes::Integer);
			Assert::AreEqual(myStackTestScope[1].Type(), DatumTypes::Float);
			Assert::AreEqual(myStackTestScope[2].Type(), DatumTypes::String);
			Assert::AreEqual(myStackTestScope[3].Type(), DatumTypes::Vector4);
			Assert::AreEqual(myStackTestScope[4].Type(), DatumTypes::Matrix);
			Assert::AreEqual(myStackTestScope[5].Type(), DatumTypes::Pointer);
			Assert::AreEqual(myStackTestScope[6].Type(), DatumTypes::Table);

			Assert::AreEqual(myStackTestScope[0], myIntDatum);
			Assert::AreEqual(myStackTestScope[1], myFloatDatum);
			Assert::AreEqual(myStackTestScope[2], myStringDatum);
			Assert::AreEqual(myStackTestScope[3], myVectorDatum);
			Assert::AreEqual(myStackTestScope[4], myMatrixDatum);
			Assert::AreEqual(myStackTestScope[5], myRTTIDatum);
			Assert::AreEqual(myStackTestScope[6], myScopeDatum);

		}
		TEST_METHOD(ScopeOOBracketBracketConst_unint32_t)
		{
			glm::vec4 myVec1(1.0f, 1.0f, 1.0f, 1.0f);
			glm::vec4 myVec2(2.0f, 2.0f, 2.0f, 2.0f);
			glm::vec4 myVec3(3.0f, 3.0f, 3.0f, 3.0f);

			glm::mat4 myMat1(1.0f);
			glm::mat4 myMat2(2.0f);
			glm::mat4 myMat3(3.0f);

			Foo myFoo1(1);
			Foo myFoo2(2);
			Foo myFoo3(3);
			RTTI* myRTTI1 = &myFoo1;
			RTTI* myRTTI2 = &myFoo2;
			RTTI* myRTTI3 = &myFoo3;

			Datum myIntDatum{ 1,2,3 };
			Datum myFloatDatum{ 1.0f,2.0f,3.0f };
			Datum myStringDatum{ "a"s,"b"s,"c"s };
			Datum myVectorDatum{ myVec1,myVec2,myVec3 };
			Datum myMatrixDatum{ myMat1,myMat2,myMat3 };
			Datum myRTTIDatum{ myRTTI1,myRTTI2, myRTTI3 };


			Scope::pt_string_datum pairInt("Integer Datum", myIntDatum);
			Scope::pt_string_datum pairFloat("Float Datum", myFloatDatum);
			Scope::pt_string_datum pairString("String Datum", myStringDatum);
			Scope::pt_string_datum pairVector("Vector Datum", myVectorDatum);
			Scope::pt_string_datum pairMatrix("Matrix Datum", myMatrixDatum);
			Scope::pt_string_datum pairRTTI("RTTI Datum", myRTTIDatum);

			Scope* myScope1 = new Scope{ &pairInt, &pairFloat };
			Scope* myScope2 = new Scope{ &pairString, &pairVector };
			Scope* myScope3 = new Scope{ &pairMatrix, &pairRTTI };

			Datum myScopeDatum{ myScope1,myScope2, myScope3 };
			Scope::pt_string_datum pairScope("Scope Datum", myScopeDatum);

			const Scope myStackTestScope{ &pairInt, &pairFloat, &pairString, &pairVector, &pairMatrix, &pairRTTI, &pairScope };

			Assert::AreEqual(static_cast<std::size_t>(7), myStackTestScope.NumberOfItems());
			Assert::AreEqual(myStackTestScope[0].Type(), DatumTypes::Integer);
			Assert::AreEqual(myStackTestScope[1].Type(), DatumTypes::Float);
			Assert::AreEqual(myStackTestScope[2].Type(), DatumTypes::String);
			Assert::AreEqual(myStackTestScope[3].Type(), DatumTypes::Vector4);
			Assert::AreEqual(myStackTestScope[4].Type(), DatumTypes::Matrix);
			Assert::AreEqual(myStackTestScope[5].Type(), DatumTypes::Pointer);
			Assert::AreEqual(myStackTestScope[6].Type(), DatumTypes::Table);

			Assert::AreEqual(myStackTestScope[0], myIntDatum);
			Assert::AreEqual(myStackTestScope[1], myFloatDatum);
			Assert::AreEqual(myStackTestScope[2], myStringDatum);
			Assert::AreEqual(myStackTestScope[3], myVectorDatum);
			Assert::AreEqual(myStackTestScope[4], myMatrixDatum);
			Assert::AreEqual(myStackTestScope[5], myRTTIDatum);
			Assert::AreEqual(myStackTestScope[6], myScopeDatum);
		}
		TEST_METHOD(ScopeOOEqualsConst_ScopeRef)
		{

			//EQUAL WITH SAME CONTENT AND SAME ORDER
			{
				glm::vec4 myVec1(1.0f, 1.0f, 1.0f, 1.0f);
				glm::vec4 myVec2(2.0f, 2.0f, 2.0f, 2.0f);
				glm::vec4 myVec3(3.0f, 3.0f, 3.0f, 3.0f);

				glm::mat4 myMat1(1.0f);
				glm::mat4 myMat2(2.0f);
				glm::mat4 myMat3(3.0f);

				Foo myFoo1(1);
				Foo myFoo2(2);
				Foo myFoo3(3);
				RTTI* myRTTI1 = &myFoo1;
				RTTI* myRTTI2 = &myFoo2;
				RTTI* myRTTI3 = &myFoo3;

				Datum myIntDatum{ 1,2,3 };
				Datum myFloatDatum{ 1.0f,2.0f,3.0f };
				Datum myStringDatum{ "a"s,"b"s,"c"s };
				Datum myVectorDatum{ myVec1,myVec2,myVec3 };
				Datum myMatrixDatum{ myMat1,myMat2,myMat3 };
				Datum myRTTIDatum{ myRTTI1,myRTTI2, myRTTI3 };


				Scope::pt_string_datum pairInt("Integer Datum", myIntDatum);
				Scope::pt_string_datum pairFloat("Float Datum", myFloatDatum);
				Scope::pt_string_datum pairString("String Datum", myStringDatum);
				Scope::pt_string_datum pairVector("Vector Datum", myVectorDatum);
				Scope::pt_string_datum pairMatrix("Matrix Datum", myMatrixDatum);
				Scope::pt_string_datum pairRTTI("RTTI Datum", myRTTIDatum);

				Scope* myScope1 = new Scope{ &pairInt, &pairFloat };
				Scope* myScope2 = new Scope{ &pairString, &pairVector };
				Scope* myScope3 = new Scope{ &pairMatrix, &pairRTTI };

				Datum myScopeDatum{ myScope1,myScope2, myScope3 };
				Scope::pt_string_datum pairScope("Scope Datum", myScopeDatum);


				Scope myStackTestScope{ &pairInt, &pairFloat, &pairString, &pairVector, &pairMatrix, &pairRTTI, &pairScope };

				Assert::AreEqual(static_cast<std::size_t>(7), myStackTestScope.NumberOfItems());
				Assert::AreEqual(myStackTestScope[0], myIntDatum);
				Assert::AreEqual(myStackTestScope[1], myFloatDatum);
				Assert::AreEqual(myStackTestScope[2], myStringDatum);
				Assert::AreEqual(myStackTestScope[3], myVectorDatum);
				Assert::AreEqual(myStackTestScope[4], myMatrixDatum);
				Assert::AreEqual(myStackTestScope[5], myRTTIDatum);
				Assert::AreEqual(myStackTestScope[6], myScopeDatum);

				Scope myCopyScope(myStackTestScope);

				Assert::AreEqual(static_cast<std::size_t>(7), myCopyScope.NumberOfItems());
				Assert::AreEqual(myCopyScope[0], myIntDatum);
				Assert::AreEqual(myCopyScope[1], myFloatDatum);
				Assert::AreEqual(myCopyScope[2], myStringDatum);
				Assert::AreEqual(myCopyScope[3], myVectorDatum);
				Assert::AreEqual(myCopyScope[4], myMatrixDatum);
				Assert::AreEqual(myCopyScope[5], myRTTIDatum);
				Assert::AreEqual(myCopyScope[6], myScopeDatum);

				bool areEqual = (myStackTestScope == myCopyScope);
				Assert::IsTrue(areEqual);
			}

			//EQUAL WITH SAME CONTENT AND DIFFERENT ORDER
			{
				glm::vec4 myVec1(1.0f, 1.0f, 1.0f, 1.0f);
				glm::vec4 myVec2(2.0f, 2.0f, 2.0f, 2.0f);
				glm::vec4 myVec3(3.0f, 3.0f, 3.0f, 3.0f);

				glm::mat4 myMat1(1.0f);
				glm::mat4 myMat2(2.0f);
				glm::mat4 myMat3(3.0f);

				Foo myFoo1(1);
				Foo myFoo2(2);
				Foo myFoo3(3);
				RTTI* myRTTI1 = &myFoo1;
				RTTI* myRTTI2 = &myFoo2;
				RTTI* myRTTI3 = &myFoo3;

				Datum myIntDatum{ 1,2,3 };
				Datum myFloatDatum{ 1.0f,2.0f,3.0f };
				Datum myStringDatum{ "a"s,"b"s,"c"s };
				Datum myVectorDatum{ myVec1,myVec2,myVec3 };
				Datum myMatrixDatum{ myMat1,myMat2,myMat3 };
				Datum myRTTIDatum{ myRTTI1,myRTTI2, myRTTI3 };


				Scope::pt_string_datum pairInt("Integer Datum", myIntDatum);
				Scope::pt_string_datum pairFloat("Float Datum", myFloatDatum);
				Scope::pt_string_datum pairString("String Datum", myStringDatum);
				Scope::pt_string_datum pairVector("Vector Datum", myVectorDatum);
				Scope::pt_string_datum pairMatrix("Matrix Datum", myMatrixDatum);
				Scope::pt_string_datum pairRTTI("RTTI Datum", myRTTIDatum);

				Scope* myScope1 = new Scope{ &pairInt, &pairFloat };
				Scope* myScope2 = new Scope{ &pairString, &pairVector };
				Scope* myScope3 = new Scope{ &pairMatrix, &pairRTTI };
	
				Datum myScopeDatum{ myScope1,myScope2, myScope3 };
				Scope::pt_string_datum pairScope("Scope Datum", myScopeDatum);

				Scope* myScope1EquivalentDatumsDifferentOrder = new Scope{ &pairFloat, &pairInt };
				Scope* myScope2EquivalentDatumsDifferentOrder = new Scope{ &pairVector, &pairString };
				Scope* myScope3EquivalentDatumsDifferentOrder = new Scope{ &pairRTTI, &pairMatrix };


				Datum myScopeDatumEquivalentScopeDifferentOrder{ myScope1EquivalentDatumsDifferentOrder,myScope2EquivalentDatumsDifferentOrder,myScope3EquivalentDatumsDifferentOrder };
				Scope::pt_string_datum pairScopeEquivalentDifferentOrder("Scope Datum", myScopeDatumEquivalentScopeDifferentOrder);


				Scope myStackTestScope{ &pairInt, &pairFloat, &pairString, &pairVector, &pairMatrix, &pairRTTI, &pairScope };

				Assert::AreEqual(static_cast<std::size_t>(7), myStackTestScope.NumberOfItems());
				Assert::AreEqual(myStackTestScope[0], myIntDatum);
				Assert::AreEqual(myStackTestScope[1], myFloatDatum);
				Assert::AreEqual(myStackTestScope[2], myStringDatum);
				Assert::AreEqual(myStackTestScope[3], myVectorDatum);
				Assert::AreEqual(myStackTestScope[4], myMatrixDatum);
				Assert::AreEqual(myStackTestScope[5], myRTTIDatum);
				Assert::AreEqual(myStackTestScope[6], myScopeDatum);

				Scope myEqivalentScopeDifferentOrder{ &pairScopeEquivalentDifferentOrder, &pairRTTI, &pairFloat, &pairInt,  &pairVector, &pairString, &pairMatrix };

				Assert::AreEqual(static_cast<std::size_t>(7), myEqivalentScopeDifferentOrder.NumberOfItems());
				Assert::AreEqual(myEqivalentScopeDifferentOrder[0], myScopeDatum);
				Assert::AreEqual(myEqivalentScopeDifferentOrder[1], myRTTIDatum);
				Assert::AreEqual(myEqivalentScopeDifferentOrder[2], myFloatDatum);
				Assert::AreEqual(myEqivalentScopeDifferentOrder[3], myIntDatum);
				Assert::AreEqual(myEqivalentScopeDifferentOrder[4], myVectorDatum);
				Assert::AreEqual(myEqivalentScopeDifferentOrder[5], myStringDatum);
				Assert::AreEqual(myEqivalentScopeDifferentOrder[6], myMatrixDatum);

				bool areEqual = (myStackTestScope == myEqivalentScopeDifferentOrder);
				Assert::IsTrue(areEqual);

			}
		}
		TEST_METHOD(ScopeOONotEqualsConst_ScopeRef)
		{
			//NOT EQUAL WHEN A SCOPE HAS DIFFEREING NUMBER OF ITEMS IN FIRST LAYER
			{
				glm::vec4 myVec1(1.0f, 1.0f, 1.0f, 1.0f);
				glm::vec4 myVec2(2.0f, 2.0f, 2.0f, 2.0f);
				glm::vec4 myVec3(3.0f, 3.0f, 3.0f, 3.0f);

				glm::mat4 myMat1(1.0f);
				glm::mat4 myMat2(2.0f);
				glm::mat4 myMat3(3.0f);

				Foo myFoo1(1);
				Foo myFoo2(2);
				Foo myFoo3(3);
				RTTI* myRTTI1 = &myFoo1;
				RTTI* myRTTI2 = &myFoo2;
				RTTI* myRTTI3 = &myFoo3;

				Datum myIntDatum{ 1,2,3 };
				Datum myFloatDatum{ 1.0f,2.0f,3.0f };
				Datum myStringDatum{ "a"s,"b"s,"c"s };
				Datum myVectorDatum{ myVec1,myVec2,myVec3 };
				Datum myMatrixDatum{ myMat1,myMat2,myMat3 };
				Datum myRTTIDatum{ myRTTI1,myRTTI2, myRTTI3 };


				Scope::pt_string_datum pairInt("Integer Datum", myIntDatum);
				Scope::pt_string_datum pairFloat("Float Datum", myFloatDatum);
				Scope::pt_string_datum pairString("String Datum", myStringDatum);
				Scope::pt_string_datum pairVector("Vector Datum", myVectorDatum);
				Scope::pt_string_datum pairMatrix("Matrix Datum", myMatrixDatum);
				Scope::pt_string_datum pairRTTI("RTTI Datum", myRTTIDatum);


				Scope* myScope1 = new Scope{ &pairInt, &pairFloat };
				Scope* myScope2 = new Scope{ &pairString, &pairVector };
				Scope* myScope3 = new Scope{ &pairMatrix, &pairRTTI };

				Scope* myScopeSame1 = new Scope{ &pairInt, &pairFloat };
				Scope* myScopeSame2 = new Scope{ &pairString, &pairVector };
				Scope* myScopeSame3 = new Scope{ &pairMatrix, &pairRTTI };

				Datum myScopeDatum{ myScope1,myScope2, myScope3 };
				Scope::pt_string_datum pairScope("Scope Datum", myScopeDatum);

				Datum myScopeDatumSame{ myScopeSame1, myScopeSame2, myScopeSame3 };
				Scope::pt_string_datum pairScopeSame("Scope Same Datum", myScopeDatumSame);

				Scope myStackTestScope{ &pairInt, &pairFloat, &pairString, &pairVector, &pairMatrix, &pairRTTI, &pairScope };

				Scope myNotEqualScope{ &pairFloat, &pairString, &pairVector, &pairMatrix, &pairRTTI, &pairScopeSame }; //MISSING pairInt

				bool areEqual = (myStackTestScope == myNotEqualScope);
				Assert::IsFalse(areEqual);
			}

			//NOT EQUAL WHEN A SCOPE HAS DIFFEREING NUMBER OF ITEMS IN NESTED SCOPE
			{
				glm::vec4 myVec1(1.0f, 1.0f, 1.0f, 1.0f);
				glm::vec4 myVec2(2.0f, 2.0f, 2.0f, 2.0f);
				glm::vec4 myVec3(3.0f, 3.0f, 3.0f, 3.0f);

				glm::mat4 myMat1(1.0f);
				glm::mat4 myMat2(2.0f);
				glm::mat4 myMat3(3.0f);

				Foo myFoo1(1);
				Foo myFoo2(2);
				Foo myFoo3(3);
				RTTI* myRTTI1 = &myFoo1;
				RTTI* myRTTI2 = &myFoo2;
				RTTI* myRTTI3 = &myFoo3;

				Datum myIntDatum{ 1,2,3 };
				Datum myFloatDatum{ 1.0f,2.0f,3.0f };
				Datum myStringDatum{ "a"s,"b"s,"c"s };
				Datum myVectorDatum{ myVec1,myVec2,myVec3 };
				Datum myMatrixDatum{ myMat1,myMat2,myMat3 };
				Datum myRTTIDatum{ myRTTI1,myRTTI2, myRTTI3 };


				Scope::pt_string_datum pairInt("Integer Datum", myIntDatum);
				Scope::pt_string_datum pairFloat("Float Datum", myFloatDatum);
				Scope::pt_string_datum pairString("String Datum", myStringDatum);
				Scope::pt_string_datum pairVector("Vector Datum", myVectorDatum);
				Scope::pt_string_datum pairMatrix("Matrix Datum", myMatrixDatum);
				Scope::pt_string_datum pairRTTI("RTTI Datum", myRTTIDatum);

				Scope* myScope1 = new Scope{ &pairInt, &pairFloat };
				Scope* myScope2 = new Scope{ &pairString, &pairVector}; //Contains pairString and pairVector
				Scope* myScope3 = new Scope{ &pairMatrix, &pairRTTI };

				Scope* myScopeSame1 = new Scope{ &pairInt, &pairFloat };
				Scope* myScopDifferent2 = new Scope{ &pairString }; //MISSING pairVector
				Scope* myScopeSame3 = new Scope{ &pairMatrix, &pairRTTI };


				Datum myScopeDatum{ myScope1,myScope2, myScope3 };
				Scope::pt_string_datum pairScope("Scope Datum", myScopeDatum);

				Datum myScopeDatumWithMissingPair{ myScopeSame1, myScopDifferent2, myScopeSame3 };
				Scope::pt_string_datum pairScopeWithMissingDatum("Scope Datum", myScopeDatumWithMissingPair);

				Scope myStackTestScope{ &pairInt, &pairFloat, &pairString, &pairVector, &pairMatrix, &pairRTTI, &pairScope };

				Scope myNotEqualScope{ &pairFloat, &pairString, &pairVector, &pairMatrix, &pairRTTI, &pairScopeWithMissingDatum }; //MISSING pairVector in nested scope.

				bool areEqual = (myStackTestScope == myNotEqualScope);
				Assert::IsFalse(areEqual);
			}

			//NOT EQUAL WHEN A DATUM HAS A VALUE THAT IS DIFFERENT
			{
				glm::vec4 myVec1(1.0f, 1.0f, 1.0f, 1.0f);
				glm::vec4 myVec2(2.0f, 2.0f, 2.0f, 2.0f);
				glm::vec4 myVec3(3.0f, 3.0f, 3.0f, 3.0f);

				glm::mat4 myMat1(1.0f);
				glm::mat4 myMat2(2.0f);
				glm::mat4 myMat3(3.0f);

				Foo myFoo1(1);
				Foo myFoo2(2);
				Foo myFoo3(3);
				RTTI* myRTTI1 = &myFoo1;
				RTTI* myRTTI2 = &myFoo2;
				RTTI* myRTTI3 = &myFoo3;

				Datum myIntDatum{ 1,2,3 };
				Datum myFloatDatum{ 1.0f,2.0f,3.0f };
				Datum myStringDatum{ "a"s,"b"s,"c"s };
				Datum myStringDatumDIFFERENT{ "x"s,"y"s,"z"s }; //DIFFERENT CONTENTS IN THIS DATUM
				Datum myVectorDatum{ myVec1,myVec2,myVec3 };
				Datum myMatrixDatum{ myMat1,myMat2,myMat3 };
				Datum myRTTIDatum{ myRTTI1,myRTTI2, myRTTI3 };


				Scope::pt_string_datum pairInt("Integer Datum", myIntDatum);
				Scope::pt_string_datum pairFloat("Float Datum", myFloatDatum);
				Scope::pt_string_datum pairString("String Datum", myStringDatum);

				Scope::pt_string_datum pairStringDIFFERENT("String Datum", myStringDatumDIFFERENT); //CONTAINES THE STRING DATUM WITH DIFFERENT VALUES

				Scope::pt_string_datum pairVector("Vector Datum", myVectorDatum);
				Scope::pt_string_datum pairMatrix("Matrix Datum", myMatrixDatum);
				Scope::pt_string_datum pairRTTI("RTTI Datum", myRTTIDatum);

				Scope* myScope1 = new Scope{ &pairInt, &pairFloat };
				Scope* myScope2 = new Scope{ &pairString, &pairVector }; //Contains pairString and pairVector
				Scope* myScope3 = new Scope{ &pairMatrix, &pairRTTI };

				Scope* myScopeSame1 = new Scope{ &pairInt, &pairFloat };
				Scope* myScopDifferent2 = new Scope{ &pairStringDIFFERENT, &pairVector }; //CONTENTS IN pairString DIFFERENT
				Scope* myScopeSame3 = new Scope{ &pairMatrix, &pairRTTI };


				Datum myScopeDatum{ myScope1,myScope2, myScope3 };
				Scope::pt_string_datum pairScope("Scope Datum", myScopeDatum);

				Datum myScopeDatumDifferent{ myScopeSame1, myScopDifferent2, myScopeSame3 };
				Scope::pt_string_datum pairScopeWithDifferentDatumValues("Scope Datum", myScopeDatumDifferent);

				Scope myStackTestScope{ &pairInt, &pairFloat, &pairString, &pairVector, &pairMatrix, &pairRTTI, &pairScope };

				Scope myNotEqualScope{ &pairFloat, &pairString, &pairVector, &pairMatrix, &pairRTTI, &pairScopeWithDifferentDatumValues }; //myScopeDatumDifferent contains a string datum with different values.

				bool areEqual = (myStackTestScope == myNotEqualScope);
				Assert::IsFalse(areEqual);

			}

				

	
		}
		TEST_METHOD(ScopeEqualsConst_RTTI_Override)
		{

			//Scopes are Equivalent
			{
				glm::vec4 myVec1(1.0f, 1.0f, 1.0f, 1.0f);
				glm::vec4 myVec2(2.0f, 2.0f, 2.0f, 2.0f);
				glm::vec4 myVec3(3.0f, 3.0f, 3.0f, 3.0f);

				glm::mat4 myMat1(1.0f);
				glm::mat4 myMat2(2.0f);
				glm::mat4 myMat3(3.0f);

				Foo myFoo1(1);
				Foo myFoo2(2);
				Foo myFoo3(3);
				RTTI* myRTTI1 = &myFoo1;
				RTTI* myRTTI2 = &myFoo2;
				RTTI* myRTTI3 = &myFoo3;

				Datum myIntDatum{ 1,2,3 };
				Datum myFloatDatum{ 1.0f,2.0f,3.0f };
				Datum myStringDatum{ "a"s,"b"s,"c"s };
				Datum myVectorDatum{ myVec1,myVec2,myVec3 };
				Datum myMatrixDatum{ myMat1,myMat2,myMat3 };
				Datum myRTTIDatum{ myRTTI1,myRTTI2, myRTTI3 };


				Scope::pt_string_datum pairInt("Integer Datum", myIntDatum);
				Scope::pt_string_datum pairFloat("Float Datum", myFloatDatum);
				Scope::pt_string_datum pairString("String Datum", myStringDatum);
				Scope::pt_string_datum pairVector("Vector Datum", myVectorDatum);
				Scope::pt_string_datum pairMatrix("Matrix Datum", myMatrixDatum);
				Scope::pt_string_datum pairRTTI("RTTI Datum", myRTTIDatum);


				Scope myStackTestScope;

				Scope& myScopeLayer1 = myStackTestScope.AppendScope("Layer 1");
				Scope& myScopeLayer2 = myScopeLayer1.AppendScope("Layer 2");
				Scope& myScopeLayer3 = myScopeLayer2.AppendScope("Layer 3");

				myScopeLayer1.Append(pairInt);
				myScopeLayer1.Append(pairFloat);

				myScopeLayer2.Append(pairString);
				myScopeLayer2.Append(pairVector);

				myScopeLayer3.Append(pairMatrix);
				myScopeLayer3.Append(pairRTTI);

				Scope myStackTestScopeEqual(myStackTestScope); //Copy constructor

				bool isEqual = myStackTestScope.Equals(&myStackTestScopeEqual);
				Assert::IsTrue(isEqual);
			}

			//Scopes are NOT Equivalent
			{
				glm::vec4 myVec1(1.0f, 1.0f, 1.0f, 1.0f);
				glm::vec4 myVec2(2.0f, 2.0f, 2.0f, 2.0f);
				glm::vec4 myVec3(3.0f, 3.0f, 3.0f, 3.0f);

				glm::mat4 myMat1(1.0f);
				glm::mat4 myMat2(2.0f);
				glm::mat4 myMat3(3.0f);

				Foo myFoo1(1);
				Foo myFoo2(2);
				Foo myFoo3(3);
				RTTI* myRTTI1 = &myFoo1;
				RTTI* myRTTI2 = &myFoo2;
				RTTI* myRTTI3 = &myFoo3;

				Datum myIntDatum{ 1,2,3 };
				Datum myFloatDatum{ 1.0f,2.0f,3.0f };
				Datum myStringDatum{ "a"s,"b"s,"c"s };
				Datum myVectorDatum{ myVec1,myVec2,myVec3 };
				Datum myMatrixDatum{ myMat1,myMat2,myMat3 };
				Datum myRTTIDatum{ myRTTI1,myRTTI2, myRTTI3 };


				Scope::pt_string_datum pairInt("Integer Datum", myIntDatum);
				Scope::pt_string_datum pairFloat("Float Datum", myFloatDatum);
				Scope::pt_string_datum pairString("String Datum", myStringDatum);
				Scope::pt_string_datum pairVector("Vector Datum", myVectorDatum);
				Scope::pt_string_datum pairMatrix("Matrix Datum", myMatrixDatum);
				Scope::pt_string_datum pairRTTI("RTTI Datum", myRTTIDatum);


				Scope myStackTestScope;

				Scope& myScopeLayer1 = myStackTestScope.AppendScope("Layer 1");
				Scope& myScopeLayer2 = myScopeLayer1.AppendScope("Layer 2");
				Scope& myScopeLayer3 = myScopeLayer2.AppendScope("Layer 3");

				myScopeLayer1.Append(pairInt);
				myScopeLayer1.Append(pairFloat);

				myScopeLayer2.Append(pairString);
				myScopeLayer2.Append(pairVector);

				myScopeLayer3.Append(pairMatrix);
				myScopeLayer3.Append(pairRTTI); 

				Scope myStackTestScopeNotEqual;

				Scope& myScopeLayer1Different = myStackTestScopeNotEqual.AppendScope("Layer 1");
				Scope& myScopeLayer2Different = myStackTestScopeNotEqual.AppendScope("Layer 2");
				Scope& myScopeLayer3Different = myStackTestScopeNotEqual.AppendScope("Layer 3");

				
				myScopeLayer1Different.Append(pairMatrix);
				myScopeLayer1Different.Append(pairRTTI);

				myScopeLayer2Different.Append(pairInt);
				myScopeLayer2Different.Append(pairFloat);

				myScopeLayer3Different.Append(pairString);
				myScopeLayer3Different.Append(pairVector);


				bool isEqual = myStackTestScope.Equals(&myStackTestScopeNotEqual);
				Assert::IsFalse(isEqual);
			}
			
		}
		TEST_METHOD(ScopeOrphan)
		{
			glm::vec4 myVec1(1.0f, 1.0f, 1.0f, 1.0f);
			glm::vec4 myVec2(2.0f, 2.0f, 2.0f, 2.0f);
			glm::vec4 myVec3(3.0f, 3.0f, 3.0f, 3.0f);

			glm::mat4 myMat1(1.0f);
			glm::mat4 myMat2(2.0f);
			glm::mat4 myMat3(3.0f);

			Foo myFoo1(1);
			Foo myFoo2(2);
			Foo myFoo3(3);
			RTTI* myRTTI1 = &myFoo1;
			RTTI* myRTTI2 = &myFoo2;
			RTTI* myRTTI3 = &myFoo3;

			Datum myIntDatum{ 1,2,3 };
			Datum myFloatDatum{ 1.0f,2.0f,3.0f };
			Datum myStringDatum{ "a"s,"b"s,"c"s };
			Datum myVectorDatum{ myVec1,myVec2,myVec3 };
			Datum myMatrixDatum{ myMat1,myMat2,myMat3 };
			Datum myRTTIDatum{ myRTTI1,myRTTI2, myRTTI3 };


			Scope::pt_string_datum pairInt("Integer Datum", myIntDatum);
			Scope::pt_string_datum pairFloat("Float Datum", myFloatDatum);
			Scope::pt_string_datum pairString("String Datum", myStringDatum);
			Scope::pt_string_datum pairVector("Vector Datum", myVectorDatum);
			Scope::pt_string_datum pairMatrix("Matrix Datum", myMatrixDatum);
			Scope::pt_string_datum pairRTTI("RTTI Datum", myRTTIDatum);


			Scope myStackTestScope;

			Scope& myScopeLayer1 = myStackTestScope.AppendScope("Layer 1");
			Scope& myScopeLayer2 = myScopeLayer1.AppendScope("Layer 2");
			Scope& myScopeLayer3 = myScopeLayer2.AppendScope("Layer 3");

			myScopeLayer1.Append(pairInt);
			myScopeLayer1.Append(pairFloat);

			myScopeLayer2.Append(pairString);
			myScopeLayer2.Append(pairVector);

			myScopeLayer3.Append(pairMatrix);
			myScopeLayer3.Append(pairRTTI);


			Assert::AreEqual(&myScopeLayer2, myScopeLayer3.GetParent());
			Assert::AreEqual(&myScopeLayer1, myScopeLayer2.GetParent());
			Assert::AreEqual(&myStackTestScope, myScopeLayer1.GetParent());
			Assert::IsNull(myStackTestScope.GetParent());

			Scope& myOrphanedScopeLayer1 = myScopeLayer1.Orphan().first;

			Assert::AreEqual(&myScopeLayer2, myScopeLayer3.GetParent());
			Assert::AreEqual(&myScopeLayer1, myScopeLayer2.GetParent());
			Assert::IsNull(myScopeLayer1.GetParent());
			Assert::IsNull(myStackTestScope.GetParent());

			Scope& myOrphanedScopeLayer2 = myScopeLayer2.Orphan().first;

			Assert::AreEqual(&myScopeLayer2, myScopeLayer3.GetParent());
			Assert::IsNull(myScopeLayer2.GetParent());
			Assert::IsNull(myScopeLayer1.GetParent());
			Assert::IsNull(myStackTestScope.GetParent());

			Scope& myOrphanedScopeLayer3 = myScopeLayer3.Orphan().first;

			Assert::IsNull(myScopeLayer3.GetParent());
			Assert::IsNull(myScopeLayer2.GetParent());
			Assert::IsNull(myScopeLayer1.GetParent());
			Assert::IsNull(myStackTestScope.GetParent());


			//Once orphaned they must be deleted by the user unless it's adopted
			delete &myOrphanedScopeLayer1;
			delete &myOrphanedScopeLayer2;
			delete &myOrphanedScopeLayer3;
		}
		TEST_METHOD(ScopeFindContainedScope)
		{
			//When scope IS contained
			
			glm::vec4 myVec1(1.0f, 1.0f, 1.0f, 1.0f);
			glm::vec4 myVec2(2.0f, 2.0f, 2.0f, 2.0f);
			glm::vec4 myVec3(3.0f, 3.0f, 3.0f, 3.0f);

			glm::mat4 myMat1(1.0f);
			glm::mat4 myMat2(2.0f);
			glm::mat4 myMat3(3.0f);

			Foo myFoo1(1);
			Foo myFoo2(2);
			Foo myFoo3(3);
			RTTI* myRTTI1 = &myFoo1;
			RTTI* myRTTI2 = &myFoo2;
			RTTI* myRTTI3 = &myFoo3;

			Datum myIntDatum{ 1,2,3 };
			Datum myFloatDatum{ 1.0f,2.0f,3.0f };
			Datum myStringDatum{ "a"s,"b"s,"c"s };
			Datum myVectorDatum{ myVec1,myVec2,myVec3 };
			Datum myMatrixDatum{ myMat1,myMat2,myMat3 };
			Datum myRTTIDatum{ myRTTI1,myRTTI2, myRTTI3 };


			Scope::pt_string_datum pairInt("Integer Datum", myIntDatum);
			Scope::pt_string_datum pairFloat("Float Datum", myFloatDatum);
			Scope::pt_string_datum pairString("String Datum", myStringDatum);
			Scope::pt_string_datum pairVector("Vector Datum", myVectorDatum);
			Scope::pt_string_datum pairMatrix("Matrix Datum", myMatrixDatum);
			Scope::pt_string_datum pairRTTI("RTTI Datum", myRTTIDatum);


			Scope myStackTestScope;

			Scope& myScopeLayer1A = myStackTestScope.AppendScope("Layer 1");

			Scope& myScopeLayer1B = myStackTestScope.AppendScope("Layer 1");

			Scope& myScopeLayer2 = myScopeLayer1A.AppendScope("Layer 2");
			Scope& myScopeLayer3 = myScopeLayer2.AppendScope("Layer 3");

			myScopeLayer1A.Append(pairInt);
			myScopeLayer1A.Append(pairFloat);

			myScopeLayer2.Append(pairString);
			myScopeLayer2.Append(pairVector);

			myScopeLayer3.Append(pairMatrix);
			myScopeLayer3.Append(pairRTTI);


			Assert::AreEqual(&myScopeLayer2, myScopeLayer3.GetParent());
			Assert::AreEqual(&myScopeLayer1A, myScopeLayer2.GetParent());
			Assert::AreEqual(&myStackTestScope, myScopeLayer1A.GetParent());
			Assert::AreEqual(&myStackTestScope, myScopeLayer1B.GetParent());
			Assert::IsNull(myStackTestScope.GetParent());

			Scope::pt_datumPtr_index myPair = myStackTestScope.FindContainedScope(myScopeLayer1A);
			Datum* foundDatum = myPair.first;
			std::size_t index = myPair.second;
			Assert::AreEqual(myScopeLayer1A, foundDatum->Get<Scope>(index));

			myPair = myStackTestScope.FindContainedScope(myScopeLayer1B);
			foundDatum = myPair.first;
			index = myPair.second;
			Assert::AreEqual(myScopeLayer1B, foundDatum->Get<Scope>(index));

			myPair = myScopeLayer1A.FindContainedScope(myScopeLayer2);
			foundDatum = myPair.first;
			index = myPair.second;
			Assert::AreEqual(myScopeLayer2, foundDatum->Get<Scope>(index));

			myPair = myScopeLayer2.FindContainedScope(myScopeLayer3);
			foundDatum = myPair.first;
			index = myPair.second;
			Assert::AreEqual(myScopeLayer3, foundDatum->Get<Scope>(index));

			//When Scope is NOT contained
			{
				myPair = myScopeLayer3.FindContainedScope(myStackTestScope);
				foundDatum = myPair.first;
				index = myPair.second;
				Assert::IsNull(foundDatum);
			}

			//Placing self into method
			{
				myPair = myStackTestScope.FindContainedScope(myStackTestScope);
				foundDatum = myPair.first;
				index = myPair.second;
				Assert::IsNull(foundDatum);
			}
			
		}
		TEST_METHOD(ScopeFindContainedScopeConst)
		{
			glm::vec4 myVec1(1.0f, 1.0f, 1.0f, 1.0f);
			glm::vec4 myVec2(2.0f, 2.0f, 2.0f, 2.0f);
			glm::vec4 myVec3(3.0f, 3.0f, 3.0f, 3.0f);

			glm::mat4 myMat1(1.0f);
			glm::mat4 myMat2(2.0f);
			glm::mat4 myMat3(3.0f);

			Foo myFoo1(1);
			Foo myFoo2(2);
			Foo myFoo3(3);
			RTTI* myRTTI1 = &myFoo1;
			RTTI* myRTTI2 = &myFoo2;
			RTTI* myRTTI3 = &myFoo3;

			Datum myIntDatum{ 1,2,3 };
			Datum myFloatDatum{ 1.0f,2.0f,3.0f };
			Datum myStringDatum{ "a"s,"b"s,"c"s };
			Datum myVectorDatum{ myVec1,myVec2,myVec3 };
			Datum myMatrixDatum{ myMat1,myMat2,myMat3 };
			Datum myRTTIDatum{ myRTTI1,myRTTI2, myRTTI3 };


			Scope::pt_string_datum pairInt("Integer Datum", myIntDatum);
			Scope::pt_string_datum pairFloat("Float Datum", myFloatDatum);
			Scope::pt_string_datum pairString("String Datum", myStringDatum);
			Scope::pt_string_datum pairVector("Vector Datum", myVectorDatum);
			Scope::pt_string_datum pairMatrix("Matrix Datum", myMatrixDatum);
			Scope::pt_string_datum pairRTTI("RTTI Datum", myRTTIDatum);


			Scope myStackTestScope;

			Scope& myScopeLayer1A = myStackTestScope.AppendScope("Layer 1");

			Scope& myScopeLayer1B = myStackTestScope.AppendScope("Layer 1");

			Scope& myScopeLayer2 = myScopeLayer1A.AppendScope("Layer 2");
			Scope& myScopeLayer3 = myScopeLayer2.AppendScope("Layer 3");

			myScopeLayer1A.Append(pairInt);
			myScopeLayer1A.Append(pairFloat);

			myScopeLayer2.Append(pairString);
			myScopeLayer2.Append(pairVector);

			myScopeLayer3.Append(pairMatrix);
			myScopeLayer3.Append(pairRTTI);

			const Scope& myConstStackTestScope = myStackTestScope;

			Assert::AreEqual(&myScopeLayer2, myScopeLayer3.GetParent());
			Assert::AreEqual(&myScopeLayer1A, myScopeLayer2.GetParent());
			Assert::AreEqual(&myStackTestScope, myScopeLayer1A.GetParent());
			Assert::AreEqual(&myStackTestScope, myScopeLayer1B.GetParent());
			Assert::IsNull(myStackTestScope.GetParent());

			Scope::pt_constDatumPtr_index myPair = myConstStackTestScope.FindContainedScope(myScopeLayer1A);

			const Datum* foundDatum = myPair.first;
			std::size_t index = myPair.second;

			Assert::AreEqual(myScopeLayer1A, foundDatum->Get<Scope>(index));

			myPair = myConstStackTestScope.FindContainedScope(myScopeLayer1B);
			foundDatum = myPair.first;
			index = myPair.second;
			Assert::AreEqual(myScopeLayer1B, foundDatum->Get<Scope>(index));

			myPair = myScopeLayer1A.FindContainedScope(myScopeLayer2);
			foundDatum = myPair.first;
			index = myPair.second;
			Assert::AreEqual(myScopeLayer2, foundDatum->Get<Scope>(index));

			myPair = myScopeLayer2.FindContainedScope(myScopeLayer3);
			foundDatum = myPair.first;
			index = myPair.second;
			Assert::AreEqual(myScopeLayer3, foundDatum->Get<Scope>(index));

			//When Scope is NOT contained
			{
				myPair = myScopeLayer3.FindContainedScope(myStackTestScope);
				foundDatum = myPair.first;
				index = myPair.second;
				Assert::IsNull(foundDatum);
			}

			//Placing self into method
			{
				myPair = myStackTestScope.FindContainedScope(myStackTestScope);
				foundDatum = myPair.first;
				index = myPair.second;
				Assert::IsNull(foundDatum);
			}

		}
		TEST_METHOD(ScopeClone)
		{
			glm::vec4 myVec1(1.0f, 1.0f, 1.0f, 1.0f);
			glm::vec4 myVec2(2.0f, 2.0f, 2.0f, 2.0f);
			glm::vec4 myVec3(3.0f, 3.0f, 3.0f, 3.0f);

			glm::mat4 myMat1(1.0f);
			glm::mat4 myMat2(2.0f);
			glm::mat4 myMat3(3.0f);

			Foo myFoo1(1);
			Foo myFoo2(2);
			Foo myFoo3(3);
			RTTI* myRTTI1 = &myFoo1;
			RTTI* myRTTI2 = &myFoo2;
			RTTI* myRTTI3 = &myFoo3;

			Datum myIntDatum{ 1,2,3 };
			Datum myFloatDatum{ 1.0f,2.0f,3.0f };
			Datum myStringDatum{ "a"s,"b"s,"c"s };
			Datum myVectorDatum{ myVec1,myVec2,myVec3 };
			Datum myMatrixDatum{ myMat1,myMat2,myMat3 };
			Datum myRTTIDatum{ myRTTI1,myRTTI2, myRTTI3 };


			Scope::pt_string_datum pairInt("Integer Datum", myIntDatum);
			Scope::pt_string_datum pairFloat("Float Datum", myFloatDatum);
			Scope::pt_string_datum pairString("String Datum", myStringDatum);
			Scope::pt_string_datum pairVector("Vector Datum", myVectorDatum);
			Scope::pt_string_datum pairMatrix("Matrix Datum", myMatrixDatum);
			Scope::pt_string_datum pairRTTI("RTTI Datum", myRTTIDatum);


			Scope myStackTestScope;

			Scope& myScopeLayer1 = myStackTestScope.AppendScope("Layer 1");
			Scope& myScopeLayer2 = myScopeLayer1.AppendScope("Layer 2");
			Scope& myScopeLayer3 = myScopeLayer2.AppendScope("Layer 3");

			myScopeLayer1.Append(pairInt);
			myScopeLayer1.Append(pairFloat);

			myScopeLayer2.Append(pairString);
			myScopeLayer2.Append(pairVector);

			myScopeLayer3.Append(pairMatrix);
			myScopeLayer3.Append(pairRTTI);


			Assert::AreEqual(&myScopeLayer2, myScopeLayer3.GetParent());
			Assert::AreEqual(&myScopeLayer1, myScopeLayer2.GetParent());
			Assert::AreEqual(&myStackTestScope, myScopeLayer1.GetParent());
			Assert::IsNull(myStackTestScope.GetParent());

			Scope* myClonedScope = myStackTestScope.Clone(); //When we clone we heap allocate. So we need to call delete later.

			Assert::AreEqual(*myClonedScope, myStackTestScope);

			delete myClonedScope;
		}
		TEST_METHOD(ScopeNumberOfItemsConst)
		{
			glm::vec4 myVec1(1.0f, 1.0f, 1.0f, 1.0f);
			glm::vec4 myVec2(2.0f, 2.0f, 2.0f, 2.0f);
			glm::vec4 myVec3(3.0f, 3.0f, 3.0f, 3.0f);

			glm::mat4 myMat1(1.0f);
			glm::mat4 myMat2(2.0f);
			glm::mat4 myMat3(3.0f);

			Foo myFoo1(1);
			Foo myFoo2(2);
			Foo myFoo3(3);
			RTTI* myRTTI1 = &myFoo1;
			RTTI* myRTTI2 = &myFoo2;
			RTTI* myRTTI3 = &myFoo3;

			Datum myIntDatum{ 1,2,3 };
			Datum myFloatDatum{ 1.0f,2.0f,3.0f };
			Datum myStringDatum{ "a"s,"b"s,"c"s };
			Datum myVectorDatum{ myVec1,myVec2,myVec3 };
			Datum myMatrixDatum{ myMat1,myMat2,myMat3 };
			Datum myRTTIDatum{ myRTTI1,myRTTI2, myRTTI3 };


			Scope::pt_string_datum pairInt("Integer Datum", myIntDatum);
			Scope::pt_string_datum pairFloat("Float Datum", myFloatDatum);
			Scope::pt_string_datum pairString("String Datum", myStringDatum);
			Scope::pt_string_datum pairVector("Vector Datum", myVectorDatum);
			Scope::pt_string_datum pairMatrix("Matrix Datum", myMatrixDatum);
			Scope::pt_string_datum pairRTTI("RTTI Datum", myRTTIDatum);


			Scope myStackTestScope;

			Scope& myScopeLayer1 = myStackTestScope.AppendScope("Layer 1");
			Scope& myScopeLayer2 = myScopeLayer1.AppendScope("Layer 2");
			Scope& myScopeLayer3 = myScopeLayer2.AppendScope("Layer 3");

			myScopeLayer1.Append(pairInt);
			myScopeLayer1.Append(pairFloat);

			myScopeLayer2.Append(pairString);
			myScopeLayer2.Append(pairVector);

			myScopeLayer3.Append(pairMatrix);
			myScopeLayer3.Append(pairRTTI);


			Assert::AreEqual(&myScopeLayer2, myScopeLayer3.GetParent());
			Assert::AreEqual(&myScopeLayer1, myScopeLayer2.GetParent());
			Assert::AreEqual(&myStackTestScope, myScopeLayer1.GetParent());
			Assert::IsNull(myStackTestScope.GetParent());

			Assert::AreEqual(static_cast<std::size_t>(1), myStackTestScope.NumberOfItems()); //has Datum type Scope (myScopeLayer1)									      : TOTAL ITEMS 1
			Assert::AreEqual(static_cast<std::size_t>(3), myScopeLayer1.NumberOfItems());    //has Datum type Scope (myScopeLayer2), Datum type Integer, Datum type Float : TOTAL ITEMS 3
			Assert::AreEqual(static_cast<std::size_t>(3), myScopeLayer2.NumberOfItems());    //has Datum type Scope (myScopeLayer3), Datum type String, Datum type Vector : TOTAL ITEMS 3
			Assert::AreEqual(static_cast<std::size_t>(2), myScopeLayer3.NumberOfItems());    //Datum type Matrix, Datum type RTTI										  : TOTAL ITEMS 2

		}
		TEST_METHOD(ScopeReHashMap)
		{
			glm::vec4 myVec1(1.0f, 1.0f, 1.0f, 1.0f);
			glm::vec4 myVec2(2.0f, 2.0f, 2.0f, 2.0f);
			glm::vec4 myVec3(3.0f, 3.0f, 3.0f, 3.0f);

			glm::mat4 myMat1(1.0f);
			glm::mat4 myMat2(2.0f);
			glm::mat4 myMat3(3.0f);

			Foo myFoo1(1);
			Foo myFoo2(2);
			Foo myFoo3(3);
			RTTI* myRTTI1 = &myFoo1;
			RTTI* myRTTI2 = &myFoo2;
			RTTI* myRTTI3 = &myFoo3;

			Datum myIntDatum{ 1,2,3 };
			Datum myFloatDatum{ 1.0f,2.0f,3.0f };
			Datum myStringDatum{ "a"s,"b"s,"c"s };
			Datum myVectorDatum{ myVec1,myVec2,myVec3 };
			Datum myMatrixDatum{ myMat1,myMat2,myMat3 };
			Datum myRTTIDatum{ myRTTI1,myRTTI2, myRTTI3 };


			Scope::pt_string_datum pairInt("Integer Datum", myIntDatum);
			Scope::pt_string_datum pairFloat("Float Datum", myFloatDatum);
			Scope::pt_string_datum pairString("String Datum", myStringDatum);
			Scope::pt_string_datum pairVector("Vector Datum", myVectorDatum);
			Scope::pt_string_datum pairMatrix("Matrix Datum", myMatrixDatum);
			Scope::pt_string_datum pairRTTI("RTTI Datum", myRTTIDatum);


			Scope myStackTestScope;
			Scope& myLayer1Scope = myStackTestScope.AppendScope("Layer 1");
			Scope& myLayer2Scope = myLayer1Scope.AppendScope("Layer 2");
			Scope& myLayer3Scope = myLayer2Scope.AppendScope("Layer 3");

			myLayer3Scope;

			myStackTestScope.Append(pairInt);
			myStackTestScope.Append(pairFloat);
			myStackTestScope.Append(pairString);
			myStackTestScope.Append(pairVector);
			myStackTestScope.Append(pairMatrix);
			myStackTestScope.Append(pairRTTI);

			//Current Order of myStackTestScope _orderVector is 
			//1. Datum-Table-myLayer1Scope
			//2. Datum-Integer-pairInt
			//3. Datum-Float-pairFloat
			//4. Datum-String-pairString
			//5. Datum-Vector-pairVector
			//6. Datum-Matrix-pairMatrix
			//7. Datum-Pointer-pairRTTI

			myStackTestScope.ReHashMap(100); //Contents have been rehashed. ORDER MUST BE PRESERVED

			Scope::pt_datumPtr_index pair = myStackTestScope.FindContainedScope(myLayer1Scope);

			Datum& myTableDatum = *(pair.first);

			Assert::AreEqual(myStackTestScope[0], myTableDatum);
			Assert::AreEqual(myStackTestScope[1], myIntDatum);
			Assert::AreEqual(myStackTestScope[2], myFloatDatum);
			Assert::AreEqual(myStackTestScope[3], myStringDatum);
			Assert::AreEqual(myStackTestScope[4], myVectorDatum);
			Assert::AreEqual(myStackTestScope[5], myMatrixDatum);
			Assert::AreEqual(myStackTestScope[6], myRTTIDatum);

		}
		TEST_METHOD(ScopeLoadFactorPercentage)
		{
			
			glm::vec4 myVec1(1.0f, 1.0f, 1.0f, 1.0f);
			glm::vec4 myVec2(2.0f, 2.0f, 2.0f, 2.0f);
			glm::vec4 myVec3(3.0f, 3.0f, 3.0f, 3.0f);

			glm::mat4 myMat1(1.0f);
			glm::mat4 myMat2(2.0f);
			glm::mat4 myMat3(3.0f);

			Foo myFoo1(1);
			Foo myFoo2(2);
			Foo myFoo3(3);
			RTTI* myRTTI1 = &myFoo1;
			RTTI* myRTTI2 = &myFoo2;
			RTTI* myRTTI3 = &myFoo3;

			Datum myIntDatum{ 1,2,3 };
			Datum myFloatDatum{ 1.0f,2.0f,3.0f };
			Datum myStringDatum{ "a"s,"b"s,"c"s };
			Datum myVectorDatum{ myVec1,myVec2,myVec3 };
			Datum myMatrixDatum{ myMat1,myMat2,myMat3 };
			Datum myRTTIDatum{ myRTTI1,myRTTI2, myRTTI3 };


			Scope::pt_string_datum pairInt("Integer Datum", myIntDatum);
			Scope::pt_string_datum pairFloat("Float Datum", myFloatDatum);
			Scope::pt_string_datum pairString("String Datum", myStringDatum);
			Scope::pt_string_datum pairVector("Vector Datum", myVectorDatum);
			Scope::pt_string_datum pairMatrix("Matrix Datum", myMatrixDatum);
			Scope::pt_string_datum pairRTTI("RTTI Datum", myRTTIDatum);


			Scope myStackTestScope;
			Scope& myLayer1Scope = myStackTestScope.AppendScope("Layer 1");
			Scope& myLayer2Scope = myLayer1Scope.AppendScope("Layer 2");
			Scope& myLayer3Scope = myLayer2Scope.AppendScope("Layer 3");

			myLayer3Scope;

			myStackTestScope.Append(pairInt);
			myStackTestScope.Append(pairFloat);
			myStackTestScope.Append(pairString);
			myStackTestScope.Append(pairVector);
			myStackTestScope.Append(pairMatrix);
			myStackTestScope.Append(pairRTTI);

			//Current Order of myStackTestScope _orderVector is 
			//1. Datum-Table-myLayer1Scope
			//2. Datum-Integer-pairInt
			//3. Datum-Float-pairFloat
			//4. Datum-String-pairString
			//5. Datum-Vector-pairVector
			//6. Datum-Matrix-pairMatrix
			//7. Datum-Pointer-pairRTTI
			
			float expectedLoadFactor = static_cast<float>(myStackTestScope.NumberOfItems()) / myStackTestScope.NumberOfBuckets();
			Assert::AreEqual(expectedLoadFactor, myStackTestScope.LoadFactorPercentage());

			myStackTestScope.ReHashMap(100); //Contents have been rehashed. ORDER MUST BE PRESERVED

			expectedLoadFactor = static_cast<float>(myStackTestScope.NumberOfItems()) / myStackTestScope.NumberOfBuckets();
			Assert::AreEqual(expectedLoadFactor, myStackTestScope.LoadFactorPercentage());

		}
		TEST_METHOD(ScopeFindAllChildrenOfType)
		{
			IJsonCoordinator myCoordinator;
			myCoordinator.AddHelper<JsonScopeHelper>();
			AvatarFooFactory myAvatarFooFactory;

			JsonScopeHelper::Data* _myDataClass = myCoordinator.ParseFromFile<JsonScopeHelper::Data>("Content/GameObject.json");
			_myDataClass;


			Scope* myGameObjectScope = _myDataClass->GetData();
			GameObject* myGameObject = myGameObjectScope->As<GameObject>();

			Assert::IsNotNull(myGameObject);

			Vector<AvatarFoo*> vecAvatartFoo = myGameObject->FindAllChildrenOfType<UnitTestLibraryDesktop::AvatarFoo>();

			std::size_t _size = vecAvatartFoo.Size();
			Assert::AreEqual(static_cast<size_t>(2), _size);
			Assert::AreEqual(vecAvatartFoo[0]->_hitPoints, 500);
			Assert::AreEqual(vecAvatartFoo[1]->_hitPoints, 1000);
		}
		TEST_METHOD(ScopeWorkOnAllChildrenOfType)
		{
			IJsonCoordinator myCoordinator;
			myCoordinator.AddHelper<JsonScopeHelper>();
			AvatarFooFactory myAvatarFooFactory;

			JsonScopeHelper::Data* _myDataClass = myCoordinator.ParseFromFile<JsonScopeHelper::Data>("Content/GameObject.json");
			_myDataClass;


			Scope* myGameObjectScope = _myDataClass->GetData();
			GameObject* myGameObject = myGameObjectScope->As<GameObject>();

			Assert::IsNotNull(myGameObject);

			auto setHealth = [](AvatarFoo& _avatar)
			{
				_avatar._hitPoints = 5;
			};

			Vector<AvatarFoo*> vecAvatartFoo = myGameObject->FindAllChildrenOfType<AvatarFoo>();

			myGameObject->WorkOnAllChildrenOfType<AvatarFoo>(setHealth);


			Assert::AreEqual(vecAvatartFoo[0]->_hitPoints, 5);
			Assert::AreEqual(vecAvatartFoo[1]->_hitPoints, 5);
		}

	private:
		static _CrtMemState _startMemState; //static class members are shared in every instance of the class
	};

	_CrtMemState ScopeTest::_startMemState; //There is needed to separate declaration from definition. Changes in C++17
}