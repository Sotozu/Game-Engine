#include "pch.h"
#include "Datum.h"
#include "Scope.h"
#include "Foo.h"
#include "ToStringSpecializations.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace FieaGameEngine;

namespace UnitTestLibraryDesktop
{
	TEST_CLASS(DatumTest)
	{
	public:
		
		TEST_METHOD_INITIALIZE(Initialize) //Before every test this is called.
		{
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
		TEST_METHOD(DefaultConstructor)
		{	
			//Declaring a Datum
			{
				Datum myDatum;
				Assert::AreEqual(size_t(0), myDatum.Size());
				Assert::AreEqual(size_t(0), myDatum.Capacity());
				Assert::AreEqual(DatumTypes::Unknown, myDatum.Type());
			}
		}
		TEST_METHOD(DefaultConstructor_Type_Size)
		{
			{
				Datum myDatum(DatumTypes::String, 10);
				Assert::AreEqual(size_t(10), myDatum.Size());
				Assert::AreEqual(size_t(10), myDatum.Capacity());
				Assert::AreEqual(DatumTypes::String, myDatum.Type());
			}
		}
		TEST_METHOD(DatumCopyConstructor)
		{

			//COPY CONSTRUCTING A INTERNAL TYPE DATUM
			{
				//Assigning one empty datum to another empty datum
				{
					Datum myDatum1;
					Datum myDatum2(myDatum1);

					Assert::AreEqual(myDatum1.Type(), myDatum2.Type());
					Assert::AreEqual(myDatum1.Size(), myDatum2.Size());
					Assert::AreEqual(myDatum1.Capacity(), myDatum2.Capacity());

				}

				//Assigning empty datum with type set
				{
					Datum myDatum1;
					myDatum1.SetType(DatumTypes::Integer);
					Datum myDatum2(myDatum1);

					Assert::AreEqual(myDatum1.Type(), myDatum2.Type());
					Assert::AreEqual(myDatum1.Size(), myDatum2.Size());
					Assert::AreEqual(myDatum1.Capacity(), myDatum2.Capacity());
				}

				//Assigning initialized datum with type set to
				{
					int32_t i = 1;
					Datum myDatum1(i); //Sets type and places the value 1 into the first block of memory for the Datum
					Datum myDatum2(myDatum1);

					Assert::AreEqual(myDatum1.Type(), myDatum2.Type());
					Assert::AreEqual(myDatum1.Size(), myDatum2.Size());
					Assert::AreEqual(myDatum1.Capacity(), myDatum2.Capacity());
				}

				//Assigning initialized datum with type set to String
				{
					std::string s = "Hello!";
					Datum myDatum1(s); //Sets type and places the value 1 into the first block of memory for the Datum
					Datum myDatum2(myDatum1);

					Assert::AreEqual(myDatum1.Type(), myDatum2.Type());
					Assert::AreEqual(myDatum1.Size(), myDatum2.Size());
					Assert::AreEqual(myDatum1.Capacity(), myDatum2.Capacity());
				}

			}

			//COPY CONSTRUCTING A EXTERNAL TYPE DATUM
			{
				//Assigning an external datum to constructing datum - Integer.
				{
					const int SIZE = 10;
					int myIntArray[SIZE] = { 0,1,2,3,4,5,6,7,8,9};
					Datum myDatum1;

					myDatum1.SetStorage(myIntArray, SIZE);

					Datum myDatum2(myDatum1);

					Assert::AreEqual(myDatum1.Type(), myDatum2.Type());
					Assert::AreEqual(myDatum1.Size(), myDatum2.Size());
					Assert::AreEqual(myDatum1.Capacity(), myDatum2.Capacity());

				}

				//Assigning an external datum to constructing datum - String.
				{
					const int SIZE = 10;
					int myIntArray[SIZE] = { 0,1,2,3,4,5,6,7,8,9 };
					Datum myDatum1;

					myDatum1.SetStorage(myIntArray, SIZE);


					Datum myDatum2(myDatum1);

					Assert::AreEqual(myDatum1.Type(), myDatum2.Type());
					Assert::AreEqual(myDatum1.Size(), myDatum2.Size());
					Assert::AreEqual(myDatum1.Capacity(), myDatum2.Capacity());
				}

			}

		}
		TEST_METHOD(DatumCopyAssignment)
		{
			
			//Assigning a defaulted datum to default datum
			{
				Datum myDatum1;
				Datum myDatum2;

				myDatum1 = myDatum2;
				Assert::AreEqual(myDatum1.Type(), myDatum2.Type());
				Assert::AreEqual(myDatum1.Size(), myDatum2.Size());
				Assert::AreEqual(myDatum1.Capacity(), myDatum2.Capacity());

			}

			//Self Assignment Test
			{
				//Empty
				Datum myDatum1;
				Datum* myDatum1Reference = &myDatum1;
				myDatum1 = *myDatum1Reference;
				Assert::AreEqual(DatumTypes::Unknown, myDatum1.Type());
				Assert::AreEqual(static_cast<size_t>(0), myDatum1.Size());
				Assert::AreEqual(static_cast<size_t>(0), myDatum1.Capacity());

				//Initialized
				Datum myDatum2(DatumTypes::Integer, 10);
				Datum* myDatum2Reference = &myDatum2;
				myDatum2 = *myDatum2Reference;
				Assert::AreEqual(DatumTypes::Integer, myDatum2.Type());
				Assert::AreEqual(static_cast<size_t>(10), myDatum2.Size());
				Assert::AreEqual(static_cast<size_t>(10), myDatum2.Capacity());


			}

			//INTERNAL TO EXTERNAL DATUM ASSIGNMENT
			{
				Foo myFoo1(1);
				Foo myFoo2(2);
				Foo myFoo3(3);
				RTTI* myRTTI1 = &myFoo1;
				RTTI* myRTTI2 = &myFoo2;
				RTTI* myRTTI3 = &myFoo3;
				Datum* myDatumTypes = new Datum[static_cast<std::size_t>(DatumTypes::End)];


				const int SIZE = 3;

				int myIntArray[SIZE] = { 0,1,2 };
				float myFloatArray[SIZE] = { 0.f,1.f,2.f };
				std::string myStringArray[SIZE] = { "","","" };
				glm::vec4 myVecArray[SIZE] = { { 1.0f, 1.0f, 1.0f, 1.0f}, { 1.0f, 1.0f, 1.0f, 1.0f}, { 1.0f, 1.0f, 1.0f, 1.0f} };
				glm::mat4 myMatArray[SIZE] = { {1.0f, 1.0f, 1.0f, 1.0f,1.0f, 1.0f, 1.0f, 1.0f,1.0f, 1.0f, 1.0f, 1.0f,1.0f, 1.0f, 1.0f, 1.0f},
												{1.0f, 1.0f, 1.0f, 1.0f,1.0f, 1.0f, 1.0f, 1.0f,1.0f, 1.0f, 1.0f, 1.0f,1.0f, 1.0f, 1.0f, 1.0f},
												{1.0f, 1.0f, 1.0f, 1.0f,1.0f, 1.0f, 1.0f, 1.0f,1.0f, 1.0f, 1.0f, 1.0f,1.0f, 1.0f, 1.0f, 1.0f} };

				RTTI* myRTTIArray[SIZE] = { myRTTI1, myRTTI2, myRTTI3 };

				Datum myDatum1(1);
				Datum myDatum2(1.0f);
				Datum myDatum3(""s);
				Datum myDatum4(myVecArray[0]);
				Datum myDatum5(myMatArray[0]);
				Datum myDatum6(myRTTI1);

				myDatumTypes[0].SetStorage(myIntArray, SIZE);
				myDatumTypes[0] = myDatum1;

				Assert::AreEqual(myDatumTypes[0].Type(), myDatum1.Type());
				Assert::AreEqual(myDatumTypes[0].Size(), myDatum1.Size());
				Assert::AreEqual(myDatumTypes[0].Capacity(), myDatum1.Capacity());
				Assert::AreEqual(myDatumTypes[0].IsExternal(), myDatum1.IsExternal());

				myDatumTypes[1].SetStorage(myFloatArray, SIZE);
				myDatumTypes[1] = myDatum2;

				Assert::AreEqual(myDatumTypes[1].Type(), myDatum2.Type());
				Assert::AreEqual(myDatumTypes[1].Size(), myDatum2.Size());
				Assert::AreEqual(myDatumTypes[1].Capacity(), myDatum2.Capacity());
				Assert::AreEqual(myDatumTypes[1].IsExternal(), myDatum1.IsExternal());


				myDatumTypes[2].SetStorage(myStringArray, SIZE);
				myDatumTypes[2] = myDatum3;

				Assert::AreEqual(myDatumTypes[2].Type(), myDatum3.Type());
				Assert::AreEqual(myDatumTypes[2].Size(), myDatum3.Size());
				Assert::AreEqual(myDatumTypes[2].Capacity(), myDatum3.Capacity());
				Assert::AreEqual(myDatumTypes[2].IsExternal(), myDatum1.IsExternal());


				myDatumTypes[3].SetStorage(myVecArray, SIZE);
				myDatumTypes[3] = myDatum4;

				Assert::AreEqual(myDatumTypes[3].Type(), myDatum4.Type());
				Assert::AreEqual(myDatumTypes[3].Size(), myDatum4.Size());
				Assert::AreEqual(myDatumTypes[3].Capacity(), myDatum4.Capacity());
				Assert::AreEqual(myDatumTypes[3].IsExternal(), myDatum1.IsExternal());


				myDatumTypes[4].SetStorage(myMatArray, SIZE);
				myDatumTypes[4] = myDatum5;

				Assert::AreEqual(myDatumTypes[4].Type(), myDatum5.Type());
				Assert::AreEqual(myDatumTypes[4].Size(), myDatum5.Size());
				Assert::AreEqual(myDatumTypes[4].Capacity(), myDatum5.Capacity());
				Assert::AreEqual(myDatumTypes[4].IsExternal(), myDatum1.IsExternal());


				myDatumTypes[5].SetStorage(myRTTIArray, SIZE);
				myDatumTypes[5] = myDatum6;

				Assert::AreEqual(myDatumTypes[5].Type(), myDatum6.Type());
				Assert::AreEqual(myDatumTypes[5].Size(), myDatum6.Size());
				Assert::AreEqual(myDatumTypes[5].Capacity(), myDatum6.Capacity());
				Assert::AreEqual(myDatumTypes[5].IsExternal(), myDatum1.IsExternal());

				delete[] myDatumTypes;

			}

			//EXTERNAL TO EXTERNAL DATUM ASSIGNMENT
			{

				const int SIZE = 3;

				Foo myFoo1(1);
				Foo myFoo2(2);
				Foo myFoo3(3);
				RTTI* myRTTI1 = &myFoo1;
				RTTI* myRTTI2 = &myFoo2;
				RTTI* myRTTI3 = &myFoo3;
				Datum* myDatumTypes = new Datum[static_cast<std::size_t>(DatumTypes::End)];	

				int myIntArray[SIZE] = { 0,1,2 };
				float myFloatArray[SIZE] = { 0.f,1.f,2.f };
				std::string myStringArray[SIZE] = { "","","" };
				glm::vec4 myVecArray[SIZE] = { { 1.0f, 1.0f, 1.0f, 1.0f}, { 1.0f, 1.0f, 1.0f, 1.0f}, { 1.0f, 1.0f, 1.0f, 1.0f} };
				glm::mat4 myMatArray[SIZE] = { {1.0f, 1.0f, 1.0f, 1.0f,1.0f, 1.0f, 1.0f, 1.0f,1.0f, 1.0f, 1.0f, 1.0f,1.0f, 1.0f, 1.0f, 1.0f},
												{1.0f, 1.0f, 1.0f, 1.0f,1.0f, 1.0f, 1.0f, 1.0f,1.0f, 1.0f, 1.0f, 1.0f,1.0f, 1.0f, 1.0f, 1.0f},
												{1.0f, 1.0f, 1.0f, 1.0f,1.0f, 1.0f, 1.0f, 1.0f,1.0f, 1.0f, 1.0f, 1.0f,1.0f, 1.0f, 1.0f, 1.0f} };

				RTTI* myRTTIArray[SIZE] = { myRTTI1, myRTTI2, myRTTI3 };

				myDatumTypes[0].SetStorage(myIntArray, SIZE);
				myDatumTypes[1].SetStorage(myFloatArray, SIZE);
				myDatumTypes[2].SetStorage(myStringArray, SIZE);
				myDatumTypes[3].SetStorage(myVecArray, SIZE);
				myDatumTypes[4].SetStorage(myMatArray, SIZE);
				myDatumTypes[5].SetStorage(myRTTIArray, SIZE);
				/*****************/

				Foo myOtherFoo1(1);
				Foo myOtherFoo2(2);
				Foo myOtherFoo3(3);
				RTTI* myOtherRTTI1 = &myFoo1;
				RTTI* myOtherRTTI2 = &myFoo2;
				RTTI* myOtherRTTI3 = &myFoo3;
				Datum* myDatumTypesOther = new Datum[static_cast<std::size_t>(DatumTypes::End)];

				int myOtherIntArray[SIZE] = { 0,1,2 };
				float myOtherFloatArray[SIZE] = { 0.f,1.f,2.f };
				std::string myOtherStringArray[SIZE] = { "","","" };
				glm::vec4 myOtherVecArray[SIZE] = { { 1.0f, 1.0f, 1.0f, 1.0f}, { 1.0f, 1.0f, 1.0f, 1.0f}, { 1.0f, 1.0f, 1.0f, 1.0f} };
				glm::mat4 myOtherMatArray[SIZE] = { {1.0f, 1.0f, 1.0f, 1.0f,1.0f, 1.0f, 1.0f, 1.0f,1.0f, 1.0f, 1.0f, 1.0f,1.0f, 1.0f, 1.0f, 1.0f},
												{1.0f, 1.0f, 1.0f, 1.0f,1.0f, 1.0f, 1.0f, 1.0f,1.0f, 1.0f, 1.0f, 1.0f,1.0f, 1.0f, 1.0f, 1.0f},
												{1.0f, 1.0f, 1.0f, 1.0f,1.0f, 1.0f, 1.0f, 1.0f,1.0f, 1.0f, 1.0f, 1.0f,1.0f, 1.0f, 1.0f, 1.0f} };

				RTTI* myOtherRTTIArray[SIZE] = { myOtherRTTI1, myOtherRTTI2, myOtherRTTI3 };

				myDatumTypesOther[0].SetStorage(myOtherIntArray, SIZE);
				myDatumTypesOther[1].SetStorage(myOtherFloatArray, SIZE);
				myDatumTypesOther[2].SetStorage(myOtherStringArray, SIZE);
				myDatumTypesOther[3].SetStorage(myOtherVecArray, SIZE);
				myDatumTypesOther[4].SetStorage(myOtherMatArray, SIZE);
				myDatumTypesOther[5].SetStorage(myOtherRTTIArray, SIZE);
				/*****************/

				myDatumTypesOther[0] = myDatumTypes[0];

				Assert::AreEqual(myDatumTypes[0].Type(), myDatumTypesOther[0].Type());
				Assert::AreEqual(myDatumTypes[0].Size(), myDatumTypesOther[0].Size());
				Assert::AreEqual(myDatumTypes[0].Capacity(), myDatumTypesOther[0].Capacity());
				Assert::AreEqual(myDatumTypes[0].IsExternal(), myDatumTypesOther[0].IsExternal());


				myDatumTypesOther[1] = myDatumTypes[1];

				Assert::AreEqual(myDatumTypes[1].Type(), myDatumTypesOther[1].Type());
				Assert::AreEqual(myDatumTypes[1].Size(), myDatumTypesOther[1].Size());
				Assert::AreEqual(myDatumTypes[1].Capacity(), myDatumTypesOther[1].Capacity());
				Assert::AreEqual(myDatumTypes[1].IsExternal(), myDatumTypesOther[1].IsExternal());


				myDatumTypesOther[2] = myDatumTypes[2];

				Assert::AreEqual(myDatumTypes[2].Type(), myDatumTypesOther[2].Type());
				Assert::AreEqual(myDatumTypes[2].Size(), myDatumTypesOther[2].Size());
				Assert::AreEqual(myDatumTypes[2].Capacity(), myDatumTypesOther[2].Capacity());
				Assert::AreEqual(myDatumTypes[2].IsExternal(), myDatumTypesOther[2].IsExternal());


				myDatumTypesOther[3] = myDatumTypes[3];

				Assert::AreEqual(myDatumTypes[3].Type(), myDatumTypesOther[3].Type());
				Assert::AreEqual(myDatumTypes[3].Size(), myDatumTypesOther[3].Size());
				Assert::AreEqual(myDatumTypes[3].Capacity(), myDatumTypesOther[3].Capacity());
				Assert::AreEqual(myDatumTypes[3].IsExternal(), myDatumTypesOther[3].IsExternal());


				myDatumTypesOther[4] = myDatumTypes[4];

				Assert::AreEqual(myDatumTypes[4].Type(), myDatumTypesOther[4].Type());
				Assert::AreEqual(myDatumTypes[4].Size(), myDatumTypesOther[4].Size());
				Assert::AreEqual(myDatumTypes[4].Capacity(), myDatumTypesOther[4].Capacity());
				Assert::AreEqual(myDatumTypes[4].IsExternal(), myDatumTypesOther[4].IsExternal());


				myDatumTypesOther[5] = myDatumTypes[5];

				Assert::AreEqual(myDatumTypes[5].Type(), myDatumTypesOther[5].Type());
				Assert::AreEqual(myDatumTypes[5].Size(), myDatumTypesOther[5].Size());
				Assert::AreEqual(myDatumTypes[5].Capacity(), myDatumTypesOther[5].Capacity());
				Assert::AreEqual(myDatumTypes[5].IsExternal(), myDatumTypesOther[5].IsExternal());

				delete[] myDatumTypes;
				delete[] myDatumTypesOther;

			}

			//INTERNAL TO INTERNAL DATUM ASSIGNMENT
			{
				//Assigning a defaulted datum to a datum with _type and _data already set ALL TYPES
				{

					glm::vec4 myVec(1.0f, 1.0f, 1.0f, 1.0f);
					glm::mat4 myMat(1.0f);
					Foo myFoo(1);
					RTTI* myRTTI = &myFoo;
					Datum* myDatumTypes = new Datum[static_cast<std::size_t>(DatumTypes::End)]
					{ 1, 1.0f, ""s , myVec, myMat, myRTTI};

					for (size_t i = 0; i < static_cast<size_t>(DatumTypes::End); ++i) // iterator through
					{

						Datum myDatum;
						myDatum = myDatumTypes[i];

						Assert::AreEqual(myDatumTypes[i].Type(), myDatum.Type());
						Assert::AreEqual(myDatumTypes[i].Size(), myDatum.Size());
						Assert::AreEqual(myDatumTypes[i].Capacity(), myDatum.Capacity());
					}
					delete[] myDatumTypes;
				}
			}

			//EXTERNAL TO INTERNAL DATUM ASSIGNMENT
			{
				Foo myFoo1(1);
				Foo myFoo2(2);
				Foo myFoo3(3);
				RTTI* myRTTI1 = &myFoo1;
				RTTI* myRTTI2 = &myFoo2;
				RTTI* myRTTI3 = &myFoo3;
				Datum* myDatumTypes = new Datum[static_cast<std::size_t>(DatumTypes::End)];


				const int SIZE = 3;

				int myIntArray[SIZE] = { 0,1,2};
				float myFloatArray[SIZE] = { 0.f,1.f,2.f};
				std::string myStringArray[SIZE] = { "","",""};
				glm::vec4 myVecArray[SIZE] = { { 1.0f, 1.0f, 1.0f, 1.0f}, { 1.0f, 1.0f, 1.0f, 1.0f}, { 1.0f, 1.0f, 1.0f, 1.0f} };
				glm::mat4 myMatArray[SIZE] = {  {1.0f, 1.0f, 1.0f, 1.0f,1.0f, 1.0f, 1.0f, 1.0f,1.0f, 1.0f, 1.0f, 1.0f,1.0f, 1.0f, 1.0f, 1.0f},
												{1.0f, 1.0f, 1.0f, 1.0f,1.0f, 1.0f, 1.0f, 1.0f,1.0f, 1.0f, 1.0f, 1.0f,1.0f, 1.0f, 1.0f, 1.0f},
												{1.0f, 1.0f, 1.0f, 1.0f,1.0f, 1.0f, 1.0f, 1.0f,1.0f, 1.0f, 1.0f, 1.0f,1.0f, 1.0f, 1.0f, 1.0f}};

				RTTI* myRTTIArray[SIZE] = { myRTTI1, myRTTI2, myRTTI3 };
			
				Datum myDatum1;
				Datum myDatum2;
				Datum myDatum3;
				Datum myDatum4;
				Datum myDatum5;
				Datum myDatum6;

				myDatumTypes[0].SetStorage(myIntArray, SIZE);
				myDatum1 = myDatumTypes[0];

				Assert::AreEqual(myDatumTypes[0].Type(), myDatum1.Type());
				Assert::AreEqual(myDatumTypes[0].Size(), myDatum1.Size());
				Assert::AreEqual(myDatumTypes[0].Capacity(), myDatum1.Capacity());
				Assert::AreEqual(myDatumTypes[0].IsExternal(), myDatum1.IsExternal());

				
				myDatumTypes[1].SetStorage(myFloatArray, SIZE);
				myDatum2 = myDatumTypes[1];

				Assert::AreEqual(myDatumTypes[1].Type(), myDatum2.Type());
				Assert::AreEqual(myDatumTypes[1].Size(), myDatum2.Size());
				Assert::AreEqual(myDatumTypes[1].Capacity(), myDatum2.Capacity());
				Assert::AreEqual(myDatumTypes[1].IsExternal(), myDatum1.IsExternal());


				myDatumTypes[2].SetStorage(myStringArray, SIZE);
				myDatum3 = myDatumTypes[2];

				Assert::AreEqual(myDatumTypes[2].Type(), myDatum3.Type());
				Assert::AreEqual(myDatumTypes[2].Size(), myDatum3.Size());
				Assert::AreEqual(myDatumTypes[2].Capacity(), myDatum3.Capacity());
				Assert::AreEqual(myDatumTypes[2].IsExternal(), myDatum1.IsExternal());

				
				myDatumTypes[3].SetStorage(myVecArray, SIZE);
				myDatum4 = myDatumTypes[3];

				Assert::AreEqual(myDatumTypes[3].Type(), myDatum4.Type());
				Assert::AreEqual(myDatumTypes[3].Size(), myDatum4.Size());
				Assert::AreEqual(myDatumTypes[3].Capacity(), myDatum4.Capacity());
				Assert::AreEqual(myDatumTypes[3].IsExternal(), myDatum1.IsExternal());

				
				myDatumTypes[4].SetStorage(myMatArray, SIZE);
				myDatum5 = myDatumTypes[4];

				Assert::AreEqual(myDatumTypes[4].Type(), myDatum5.Type());
				Assert::AreEqual(myDatumTypes[4].Size(), myDatum5.Size());
				Assert::AreEqual(myDatumTypes[4].Capacity(), myDatum5.Capacity());
				Assert::AreEqual(myDatumTypes[4].IsExternal(), myDatum1.IsExternal());

				
				myDatumTypes[5].SetStorage(myRTTIArray, SIZE);
				myDatum6 = myDatumTypes[5];

				Assert::AreEqual(myDatumTypes[5].Type(), myDatum6.Type());
				Assert::AreEqual(myDatumTypes[5].Size(), myDatum6.Size());
				Assert::AreEqual(myDatumTypes[5].Capacity(), myDatum6.Capacity());
				Assert::AreEqual(myDatumTypes[5].IsExternal(), myDatum1.IsExternal());

				delete[] myDatumTypes;
				
			}
		}
		TEST_METHOD(DatumOOAssignment)
		{

			//Datum set with no value or type
			{
				int myInt = 1;
				bool myBool = true;
				float myFloat = 1.0f;
				std::string myString = "";
				glm::vec4 myVec(1.0f, 1.0f, 1.0f, 1.0f);
				glm::mat4 myMat(1.0f);
				Foo myFoo(1);
				RTTI* myRTTI = &myFoo;

				Datum* myDatumTypes = new Datum[static_cast<std::size_t>(DatumTypes::End)];
				
				myDatumTypes[0] = myInt;
				Assert::AreEqual(DatumTypes::Integer, myDatumTypes[0].Type());
				Assert::AreEqual(static_cast<size_t>(1), myDatumTypes[0].Size());
				Assert::AreEqual(static_cast<size_t>(1), myDatumTypes[0].Capacity());

				myDatumTypes[1] = myBool;
				Assert::AreEqual(DatumTypes::Boolean, myDatumTypes[1].Type());
				Assert::AreEqual(static_cast<size_t>(1), myDatumTypes[1].Size());
				Assert::AreEqual(static_cast<size_t>(1), myDatumTypes[1].Capacity());


				myDatumTypes[2] = myFloat;
				Assert::AreEqual(DatumTypes::Float, myDatumTypes[2].Type());
				Assert::AreEqual(static_cast<size_t>(1), myDatumTypes[2].Size());
				Assert::AreEqual(static_cast<size_t>(1), myDatumTypes[2].Capacity());

				myDatumTypes[3] = myString;
				Assert::AreEqual(DatumTypes::String, myDatumTypes[3].Type());
				Assert::AreEqual(static_cast<size_t>(1), myDatumTypes[3].Size());
				Assert::AreEqual(static_cast<size_t>(1), myDatumTypes[3].Capacity());

				myDatumTypes[4] = myVec;
				Assert::AreEqual(DatumTypes::Vector4, myDatumTypes[4].Type());
				Assert::AreEqual(static_cast<size_t>(1), myDatumTypes[4].Size());
				Assert::AreEqual(static_cast<size_t>(1), myDatumTypes[4].Capacity());

				myDatumTypes[5] = myMat;
				Assert::AreEqual(DatumTypes::Matrix, myDatumTypes[5].Type());
				Assert::AreEqual(static_cast<size_t>(1), myDatumTypes[5].Size());
				Assert::AreEqual(static_cast<size_t>(1), myDatumTypes[5].Capacity());

				myDatumTypes[6] = myRTTI;
				Assert::AreEqual(DatumTypes::Pointer, myDatumTypes[6].Type());
				Assert::AreEqual(static_cast<size_t>(1), myDatumTypes[6].Size());
				Assert::AreEqual(static_cast<size_t>(1), myDatumTypes[6].Capacity());


				delete[] myDatumTypes;
			}


			//Datum with _data initialized to more than one value
			{
				int myInt = 1;
				bool myBool = true;
				float myFloat = 1.0f;
				std::string myString = "";
				glm::vec4 myVec(1.0f, 1.0f, 1.0f, 1.0f);
				glm::mat4 myMat(1.0f);
				Foo myFoo(1);
				RTTI* myRTTI = &myFoo;

				Datum* myDatumTypes = new Datum[static_cast<std::size_t>(DatumTypes::End)];

				myDatumTypes[0].PushBack(1);
				myDatumTypes[0].PushBack(2);
				myDatumTypes[0].PushBack(3);
				myDatumTypes[0] = myInt;

				Assert::AreEqual(DatumTypes::Integer, myDatumTypes[0].Type());
				Assert::AreEqual(static_cast<size_t>(1), myDatumTypes[0].Size());
				Assert::AreEqual(static_cast<size_t>(1), myDatumTypes[0].Capacity());

				myDatumTypes[1].PushBack(false);
				myDatumTypes[1].PushBack(false);
				myDatumTypes[1].PushBack(false);
				myDatumTypes[1] = myBool;

				Assert::AreEqual(DatumTypes::Boolean, myDatumTypes[1].Type());
				Assert::AreEqual(static_cast<size_t>(1), myDatumTypes[1].Size());
				Assert::AreEqual(static_cast<size_t>(1), myDatumTypes[1].Capacity());


				myDatumTypes[2].PushBack(1.0f);
				myDatumTypes[2].PushBack(2.0f);
				myDatumTypes[2].PushBack(3.0f);
				myDatumTypes[2] = myFloat;


				Assert::AreEqual(DatumTypes::Float, myDatumTypes[2].Type());
				Assert::AreEqual(static_cast<size_t>(1), myDatumTypes[2].Size());
				Assert::AreEqual(static_cast<size_t>(1), myDatumTypes[2].Capacity());

				myDatumTypes[3].PushBack("Hello World"s);
				myDatumTypes[3].PushBack("Hello World"s);
				myDatumTypes[3].PushBack("Hello World"s);
				myDatumTypes[3] = myString;
				
				Assert::AreEqual(DatumTypes::String, myDatumTypes[3].Type());
				Assert::AreEqual(static_cast<size_t>(1), myDatumTypes[3].Size());
				Assert::AreEqual(static_cast<size_t>(1), myDatumTypes[3].Capacity());

				myDatumTypes[4].PushBack(myVec);
				myDatumTypes[4].PushBack(myVec);
				myDatumTypes[4].PushBack(myVec);
				myDatumTypes[4] = myVec;
				
				Assert::AreEqual(DatumTypes::Vector4, myDatumTypes[4].Type());
				Assert::AreEqual(static_cast<size_t>(1), myDatumTypes[4].Size());
				Assert::AreEqual(static_cast<size_t>(1), myDatumTypes[4].Capacity());

				myDatumTypes[5].PushBack(myMat);
				myDatumTypes[5].PushBack(myMat);
				myDatumTypes[5].PushBack(myMat);
				myDatumTypes[5] = myMat;

				Assert::AreEqual(DatumTypes::Matrix, myDatumTypes[5].Type());
				Assert::AreEqual(static_cast<size_t>(1), myDatumTypes[5].Size());
				Assert::AreEqual(static_cast<size_t>(1), myDatumTypes[5].Capacity());

				myDatumTypes[6].PushBack(myRTTI);
				myDatumTypes[6].PushBack(myRTTI);
				myDatumTypes[6].PushBack(myRTTI);
				myDatumTypes[6] = myRTTI;

				Assert::AreEqual(DatumTypes::Pointer, myDatumTypes[6].Type());
				Assert::AreEqual(static_cast<size_t>(1), myDatumTypes[6].Size());
				Assert::AreEqual(static_cast<size_t>(1), myDatumTypes[6].Capacity());

				delete[] myDatumTypes;
			}	
		}
		TEST_METHOD(DatumMoveConstructor)
		{

			{
				Datum myDatum1(1);
				myDatum1.PushBack(2);
				myDatum1.PushBack(3);

				Datum myDatum2(std::move(myDatum1));

				Assert::AreEqual(1, myDatum2.Get<int32_t>(0));
				Assert::AreEqual(2, myDatum2.Get<int32_t>(1));
				Assert::AreEqual(3, myDatum2.Get<int32_t>(2));

				Assert::AreEqual(static_cast<size_t>(3), myDatum2.Size());

				Assert::ExpectException<std::runtime_error>([&myDatum1]() {myDatum1.Get<int32_t>(0); });
			}

			{
				Datum myDatum1(1.0f);
				myDatum1.PushBack(2.0f);
				myDatum1.PushBack(3.0f);
				Datum myDatum2(std::move(myDatum1));

				Assert::AreEqual(1.0f, myDatum2.Get<float>(0));
				Assert::AreEqual(2.0f, myDatum2.Get<float>(1));
				Assert::AreEqual(3.0f, myDatum2.Get<float>(2));

				Assert::AreEqual(static_cast<size_t>(3), myDatum2.Size());

				Assert::ExpectException<std::runtime_error>([&myDatum1]() {myDatum1.Get<float>(0); });

			}

			{
				Datum myDatum1("Hello"s);
				myDatum1.PushBack("I'm"s);
				myDatum1.PushBack("Alex"s);
				Datum myDatum2(std::move(myDatum1));

				Assert::AreEqual("Hello"s, myDatum2.Get<std::string>(0));
				Assert::AreEqual("I'm"s, myDatum2.Get<std::string>(1));
				Assert::AreEqual("Alex"s, myDatum2.Get<std::string>(2));

				Assert::AreEqual(static_cast<size_t>(3), myDatum2.Size());

				Assert::ExpectException<std::runtime_error>([&myDatum1]() {myDatum1.Get<std::string>(0); });

			}

			{
				glm::vec4 myVec1(1.0f, 1.0f, 1.0f, 1.0f);
				glm::vec4 myVec2(2.0f, 2.0f, 2.0f, 2.0f);
				glm::vec4 myVec3(3.0f, 3.0f, 3.0f, 3.0f);
				Datum myDatum1(myVec1);
				myDatum1.PushBack(myVec2);
				myDatum1.PushBack(myVec3);
				Datum myDatum2(std::move(myDatum1));

				Assert::AreEqual(myVec1, myDatum2.Get<glm::vec4>(0));
				Assert::AreEqual(myVec2, myDatum2.Get<glm::vec4>(1));
				Assert::AreEqual(myVec3, myDatum2.Get<glm::vec4>(2));

				Assert::AreEqual(static_cast<size_t>(3), myDatum2.Size());

				Assert::ExpectException<std::runtime_error>([&myDatum1]() {myDatum1.Get<std::string>(0); });

			}

			{
				glm::mat4 myMat1(1.0f);
				glm::mat4 myMat2(2.0f);
				glm::mat4 myMat3(3.0f);
				Datum myDatum1(myMat1);
				myDatum1.PushBack(myMat2);
				myDatum1.PushBack(myMat3);
				Datum myDatum2(std::move(myDatum1));

				Assert::AreEqual(myMat1, myDatum2.Get<glm::mat4>(0));
				Assert::AreEqual(myMat2, myDatum2.Get<glm::mat4>(1));
				Assert::AreEqual(myMat3, myDatum2.Get<glm::mat4>(2));

				Assert::AreEqual(static_cast<size_t>(3), myDatum2.Size());

				Assert::ExpectException<std::runtime_error>([&myDatum1]() {myDatum1.Get<glm::mat4>(0); });
			}

			{
				Foo myFoo1(1);
				Foo myFoo2(2);
				Foo myFoo3(3);
				FieaGameEngine::RTTI* myRTTI1 = &myFoo1;
				FieaGameEngine::RTTI* myRTTI2 = &myFoo2;
				FieaGameEngine::RTTI* myRTTI3 = &myFoo3;
				

				Datum myDatum1(myRTTI1);
				myDatum1.PushBack(myRTTI2);
				myDatum1.PushBack(myRTTI3);
				Datum myDatum2(std::move(myDatum1));

				Assert::AreEqual(myRTTI1, myDatum2.Get<RTTI*>(0));
				Assert::AreEqual(myRTTI2, myDatum2.Get<RTTI*>(1));
				Assert::AreEqual(myRTTI3, myDatum2.Get<RTTI*>(2));

				Assert::AreEqual(static_cast<size_t>(3), myDatum2.Size());

				Assert::ExpectException<std::runtime_error>([&myDatum1]() {myDatum1.Get<RTTI*>(0); });
			}

		}
		TEST_METHOD(DatumMoveAssignment)
		{
			{
				Datum myDatum1(1);
				myDatum1.PushBack(2);
				myDatum1.PushBack(3);

				Datum myDatum2;
				myDatum2 = std::move(myDatum1);

				Assert::AreEqual(1, myDatum2.Get<int32_t>(0));
				Assert::AreEqual(2, myDatum2.Get<int32_t>(1));
				Assert::AreEqual(3, myDatum2.Get<int32_t>(2));

				Assert::AreEqual(static_cast<size_t>(3), myDatum2.Size());

				Assert::ExpectException<std::runtime_error>([&myDatum1]() {myDatum1.Get<int32_t>(0); });
			}

			{
				Datum myDatum1(1.0f);
				myDatum1.PushBack(2.0f);
				myDatum1.PushBack(3.0f);

				Datum myDatum2;
				myDatum2 = std::move(myDatum1);

				Assert::AreEqual(1.0f, myDatum2.Get<float>(0));
				Assert::AreEqual(2.0f, myDatum2.Get<float>(1));
				Assert::AreEqual(3.0f, myDatum2.Get<float>(2));

				Assert::AreEqual(static_cast<size_t>(3), myDatum2.Size());

				Assert::ExpectException<std::runtime_error>([&myDatum1]() {myDatum1.Get<float>(0); });

			}

			{
				Datum myDatum1("Hello"s);
				myDatum1.PushBack("I'm"s);
				myDatum1.PushBack("Alex"s);

				Datum myDatum2;
				myDatum2 = std::move(myDatum1);

				Assert::AreEqual("Hello"s, myDatum2.Get<std::string>(0));
				Assert::AreEqual("I'm"s, myDatum2.Get<std::string>(1));
				Assert::AreEqual("Alex"s, myDatum2.Get<std::string>(2));

				Assert::AreEqual(static_cast<size_t>(3), myDatum2.Size());

				Assert::ExpectException<std::runtime_error>([&myDatum1]() {myDatum1.Get<std::string>(0); });

			}

			{
				glm::vec4 myVec1(1.0f, 1.0f, 1.0f, 1.0f);
				glm::vec4 myVec2(2.0f, 2.0f, 2.0f, 2.0f);
				glm::vec4 myVec3(3.0f, 3.0f, 3.0f, 3.0f);
				Datum myDatum1(myVec1);
				myDatum1.PushBack(myVec2);
				myDatum1.PushBack(myVec3);
				
				Datum myDatum2;
				myDatum2 = std::move(myDatum1);

				Assert::AreEqual(myVec1, myDatum2.Get<glm::vec4>(0));
				Assert::AreEqual(myVec2, myDatum2.Get<glm::vec4>(1));
				Assert::AreEqual(myVec3, myDatum2.Get<glm::vec4>(2));

				Assert::AreEqual(static_cast<size_t>(3), myDatum2.Size());

				Assert::ExpectException<std::runtime_error>([&myDatum1]() {myDatum1.Get<std::string>(0); });

			}

			{
				glm::mat4 myMat1(1.0f);
				glm::mat4 myMat2(2.0f);
				glm::mat4 myMat3(3.0f);
				Datum myDatum1(myMat1);
				myDatum1.PushBack(myMat2);
				myDatum1.PushBack(myMat3);

				Datum myDatum2;
				myDatum2 = std::move(myDatum1);

				Assert::AreEqual(myMat1, myDatum2.Get<glm::mat4>(0));
				Assert::AreEqual(myMat2, myDatum2.Get<glm::mat4>(1));
				Assert::AreEqual(myMat3, myDatum2.Get<glm::mat4>(2));

				Assert::AreEqual(static_cast<size_t>(3), myDatum2.Size());

				Assert::ExpectException<std::runtime_error>([&myDatum1]() {myDatum1.Get<glm::mat4>(0); });
			}

			{
				Foo myFoo1(1);
				Foo myFoo2(2);
				Foo myFoo3(3);
				FieaGameEngine::RTTI* myRTTI1 = &myFoo1;
				FieaGameEngine::RTTI* myRTTI2 = &myFoo2;
				FieaGameEngine::RTTI* myRTTI3 = &myFoo3;


				Datum myDatum1(myRTTI1);
				myDatum1.PushBack(myRTTI2);
				myDatum1.PushBack(myRTTI3);

				Datum myDatum2;
				myDatum2 = std::move(myDatum1);

				Assert::AreEqual(myRTTI1, myDatum2.Get<RTTI*>(0));
				Assert::AreEqual(myRTTI2, myDatum2.Get<RTTI*>(1));
				Assert::AreEqual(myRTTI3, myDatum2.Get<RTTI*>(2));

				Assert::AreEqual(static_cast<size_t>(3), myDatum2.Size());

				Assert::ExpectException<std::runtime_error>([&myDatum1]() {myDatum1.Get<RTTI*>(0); });
			}

		}
		TEST_METHOD(DatumType)
		{
			//Default constructed Datum
			{
				Datum myDatum;
				Assert::AreEqual(DatumTypes::Unknown, myDatum.Type());
			}

			//All valid Datum types
			{

				glm::vec4 myVec(1.0f, 1.0f, 1.0, 1.0);
				glm::mat4 myMat(1.0f);
				Foo myFoo(1);
				RTTI* myRTTI = &myFoo;
				Datum* myDatumTypes = new Datum[static_cast<std::size_t>(DatumTypes::End)]
				{ 1, 1.0f, ""s, myVec, myMat, myRTTI };

				Assert::AreEqual(DatumTypes::Integer, myDatumTypes[0].Type());
				Assert::AreEqual(DatumTypes::Float, myDatumTypes[1].Type());
				Assert::AreEqual(DatumTypes::String, myDatumTypes[2].Type());
				Assert::AreEqual(DatumTypes::Vector4, myDatumTypes[3].Type());
				Assert::AreEqual(DatumTypes::Matrix, myDatumTypes[4].Type());
				Assert::AreEqual(DatumTypes::Pointer, myDatumTypes[5].Type());


				delete[] myDatumTypes;
			}

		}
		TEST_METHOD(DatumSetType)
		{
			{
				//Set type on defaulted Datum
				for (size_t i = static_cast<size_t>(DatumTypes::Begin); i <= static_cast<size_t>(DatumTypes::End); ++i) // iterator through
				{
					DatumTypes type = static_cast<DatumTypes>(i);
					Datum myDatum1;
					myDatum1.SetType(type);
					Assert::AreEqual(type, myDatum1.Type());
				}
			}

			{
				//Set type on Datum with type already set. including trying to set to Unknown
				for (size_t i = 0; i <= static_cast<size_t>(DatumTypes::End); ++i) // iterator through
				{
					DatumTypes type = static_cast<DatumTypes>(i);
					Datum myDatum1;

					myDatum1.SetType(DatumTypes::End);

					if (type == DatumTypes::End) { type = DatumTypes::Begin; } //When we reach the last type we need to change that.
					auto Expected1 = [&myDatum1, &type]() {myDatum1.SetType(type); };
					Assert::ExpectException<std::runtime_error>(Expected1);

				}
			}
		}
		TEST_METHOD(DatumSize)
		{
			Datum myDatum;
			myDatum.PushBack(1);
			myDatum.PushBack(2);
			myDatum.PushBack(3);

			Assert::AreEqual(static_cast<size_t>(3), myDatum.Size());
			myDatum.PopBack();
			Assert::AreEqual(static_cast<size_t>(2), myDatum.Size());
			myDatum.PopBack();
			Assert::AreEqual(static_cast<size_t>(1), myDatum.Size());
			myDatum.PopBack();
			Assert::AreEqual(static_cast<size_t>(0), myDatum.Size());

		}
		TEST_METHOD(DatumResize)
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

			Scope* myScope1 = new Scope;
			Scope* myScope2 = new Scope;
			Scope* myScope3 = new Scope;

			Datum myIntDatum{ 1,2,3 };
			Datum myFloatDatum{ 1.0f,2.0f,3.0f };
			Datum myStringDatum{ "a"s,"b"s,"c"s };
			Datum myVectorDatum{ myVec1,myVec2,myVec3 };
			Datum myMatrixDatum{ myMat1,myMat2,myMat3 };
			Datum myRTTIDatum{ myRTTI1,myRTTI2,myRTTI3 };
			Datum myScopeDatum{ myScope1, myScope2, myScope3 };

			//Datum Int
			Assert::AreEqual(static_cast<size_t>(3), myIntDatum.Size());
			Assert::AreEqual(static_cast<size_t>(3), myIntDatum.Capacity());

			myIntDatum.Resize(10);
			Assert::AreEqual(static_cast<size_t>(10), myIntDatum.Size());
			Assert::AreEqual(static_cast<size_t>(10), myIntDatum.Capacity());

			myIntDatum.Resize(5);
			Assert::AreEqual(static_cast<size_t>(5), myIntDatum.Size());
			Assert::AreEqual(static_cast<size_t>(5), myIntDatum.Capacity());

			myIntDatum.Resize(0);
			Assert::AreEqual(static_cast<size_t>(0), myIntDatum.Size());
			Assert::AreEqual(static_cast<size_t>(0), myIntDatum.Capacity());

			//Datum Float
			Assert::AreEqual(static_cast<size_t>(3), myFloatDatum.Size());
			Assert::AreEqual(static_cast<size_t>(3), myFloatDatum.Capacity());

			myFloatDatum.Resize(10);
			Assert::AreEqual(static_cast<size_t>(10), myFloatDatum.Size());
			Assert::AreEqual(static_cast<size_t>(10), myFloatDatum.Capacity());

			myFloatDatum.Resize(5);
			Assert::AreEqual(static_cast<size_t>(5), myFloatDatum.Size());
			Assert::AreEqual(static_cast<size_t>(5), myFloatDatum.Capacity());

			myFloatDatum.Resize(0);
			Assert::AreEqual(static_cast<size_t>(0), myFloatDatum.Size());
			Assert::AreEqual(static_cast<size_t>(0), myFloatDatum.Capacity());

			//Datum String
			Assert::AreEqual(static_cast<size_t>(3), myStringDatum.Size());
			Assert::AreEqual(static_cast<size_t>(3), myStringDatum.Capacity());

			myStringDatum.Resize(10);
			Assert::AreEqual(static_cast<size_t>(10), myStringDatum.Size());
			Assert::AreEqual(static_cast<size_t>(10), myStringDatum.Capacity());

			myStringDatum.Resize(5);
			Assert::AreEqual(static_cast<size_t>(5), myStringDatum.Size());
			Assert::AreEqual(static_cast<size_t>(5), myStringDatum.Capacity());

			myStringDatum.Resize(0);
			Assert::AreEqual(static_cast<size_t>(0), myStringDatum.Size());
			Assert::AreEqual(static_cast<size_t>(0), myStringDatum.Capacity());

			//Datum Vector
			Assert::AreEqual(static_cast<size_t>(3), myVectorDatum.Size());
			Assert::AreEqual(static_cast<size_t>(3), myVectorDatum.Capacity());

			myVectorDatum.Resize(10);
			Assert::AreEqual(static_cast<size_t>(10), myVectorDatum.Size());
			Assert::AreEqual(static_cast<size_t>(10), myVectorDatum.Capacity());

			myVectorDatum.Resize(5);
			Assert::AreEqual(static_cast<size_t>(5), myVectorDatum.Size());
			Assert::AreEqual(static_cast<size_t>(5), myVectorDatum.Capacity());

			myVectorDatum.Resize(0);
			Assert::AreEqual(static_cast<size_t>(0), myVectorDatum.Size());
			Assert::AreEqual(static_cast<size_t>(0), myVectorDatum.Capacity());

			//Datum Matrix
			Assert::AreEqual(static_cast<size_t>(3), myMatrixDatum.Size());
			Assert::AreEqual(static_cast<size_t>(3), myMatrixDatum.Capacity());

			myMatrixDatum.Resize(10);
			Assert::AreEqual(static_cast<size_t>(10), myMatrixDatum.Size());
			Assert::AreEqual(static_cast<size_t>(10), myMatrixDatum.Capacity());

			myMatrixDatum.Resize(5);
			Assert::AreEqual(static_cast<size_t>(5), myMatrixDatum.Size());
			Assert::AreEqual(static_cast<size_t>(5), myMatrixDatum.Capacity());

			myMatrixDatum.Resize(0);
			Assert::AreEqual(static_cast<size_t>(0), myMatrixDatum.Size());
			Assert::AreEqual(static_cast<size_t>(0), myMatrixDatum.Capacity());

			//Datum RTTI
			Assert::AreEqual(static_cast<size_t>(3), myRTTIDatum.Size());
			Assert::AreEqual(static_cast<size_t>(3), myRTTIDatum.Capacity());

			myRTTIDatum.Resize(10);
			Assert::AreEqual(static_cast<size_t>(10), myRTTIDatum.Size());
			Assert::AreEqual(static_cast<size_t>(10), myRTTIDatum.Capacity());

			myRTTIDatum.Resize(5);
			Assert::AreEqual(static_cast<size_t>(5), myRTTIDatum.Size());
			Assert::AreEqual(static_cast<size_t>(5), myRTTIDatum.Capacity());

			myRTTIDatum.Resize(0);
			Assert::AreEqual(static_cast<size_t>(0), myRTTIDatum.Size());
			Assert::AreEqual(static_cast<size_t>(0), myRTTIDatum.Capacity());

			//Datum Scope
			Assert::AreEqual(static_cast<size_t>(3), myScopeDatum.Size());
			Assert::AreEqual(static_cast<size_t>(3), myScopeDatum.Capacity());

			myScopeDatum.Resize(10);
			Assert::AreEqual(static_cast<size_t>(10), myScopeDatum.Size());
			Assert::AreEqual(static_cast<size_t>(10), myScopeDatum.Capacity());

			myScopeDatum.Resize(5);
			Assert::AreEqual(static_cast<size_t>(5), myScopeDatum.Size());
			Assert::AreEqual(static_cast<size_t>(5), myScopeDatum.Capacity());

			myScopeDatum.Resize(0);
			Assert::AreEqual(static_cast<size_t>(0), myScopeDatum.Size());
			Assert::AreEqual(static_cast<size_t>(0), myScopeDatum.Capacity());

			delete myScope1;
			delete myScope2;
			delete myScope3;
		}
		TEST_METHOD(DatumClear)
		{
			//Test on defaulted datum
			//Should fail silently. Nothing should happen.
			{
				Datum myDatum;
				myDatum.Clear();

				Assert::AreEqual(DatumTypes::Unknown, myDatum.Type());
				Assert::AreEqual(static_cast<size_t>(0), myDatum.Size());
				Assert::AreEqual(static_cast<size_t>(0), myDatum.Capacity());

			}
			//Test on all valid Datum Types
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

				Scope* myScope1 = new Scope;
				Scope* myScope2 = new Scope;
				Scope* myScope3 = new Scope;

				Datum myIntDatum{ 1,2,3 };
				Datum myFloatDatum{ 1.0f,2.0f,3.0f };
				Datum myStringDatum{ "a"s,"b"s,"c"s};
				Datum myVectorDatum{ myVec1,myVec2,myVec3 };
				Datum myMatrixDatum{ myMat1,myMat2,myMat3 };
				Datum myRTTIDatum{ myRTTI1,myRTTI2,myRTTI3 };
				Datum myScopeDatum{myScope1, myScope2, myScope3 };


				Assert::AreEqual(static_cast<std::size_t>(3), myIntDatum.Size());
				myIntDatum.Clear();
				Assert::AreEqual(static_cast<std::size_t>(0), myIntDatum.Size());
				
				Assert::AreEqual(static_cast<std::size_t>(3), myFloatDatum.Size());
				myFloatDatum.Clear();
				Assert::AreEqual(static_cast<std::size_t>(0), myFloatDatum.Size());
				
				Assert::AreEqual(static_cast<std::size_t>(3), myStringDatum.Size());
				myStringDatum.Clear();
				Assert::AreEqual(static_cast<std::size_t>(0), myStringDatum.Size());
				
				Assert::AreEqual(static_cast<std::size_t>(3), myVectorDatum.Size());
				myVectorDatum.Clear();
				Assert::AreEqual(static_cast<std::size_t>(0), myVectorDatum.Size());
				
				Assert::AreEqual(static_cast<std::size_t>(3), myMatrixDatum.Size());
				myMatrixDatum.Clear();
				Assert::AreEqual(static_cast<std::size_t>(0), myMatrixDatum.Size());
				
				Assert::AreEqual(static_cast<std::size_t>(3), myRTTIDatum.Size());
				myRTTIDatum.Clear();
				Assert::AreEqual(static_cast<std::size_t>(0), myRTTIDatum.Size());
				
				Assert::AreEqual(static_cast<std::size_t>(3), myScopeDatum.Size());
				myScopeDatum.Clear();
				Assert::AreEqual(static_cast<std::size_t>(0), myScopeDatum.Size());

				delete myScope1;
				delete myScope2;
				delete myScope3;
			}
		}
		TEST_METHOD(DatumSetStorage)
		{
			//Test on default constructed Datum.
			{
				glm::vec4 myVec(1.0f, 1.0f, 1.0, 1.0);
				glm::mat4 myMat(1.0f);
				Foo myFoo(1);
				RTTI* myRTTI = &myFoo;
				Datum* myDatumTypes = new Datum[static_cast<std::size_t>(DatumTypes::End)]
				{ 1, 1.0f, ""s, myVec, myMat, myRTTI };

				Datum myExternalDatum;

				myExternalDatum.SetStorage(&myDatumTypes[0].Get<int>(0), myDatumTypes[0].Size());
				Assert::AreSame(myExternalDatum.Get<int>(0), myDatumTypes[0].Get<int>(0));
				Assert::AreEqual(myExternalDatum.Size(), myDatumTypes[0].Size());
				Assert::AreEqual(myExternalDatum.Capacity(), myDatumTypes[0].Capacity());
				Assert::ExpectException<std::runtime_error>([&myExternalDatum]() {myExternalDatum.PopBack(); });

				myExternalDatum.SetStorage(&myDatumTypes[1].Get<float>(0), myDatumTypes[1].Size());
				Assert::AreSame(myExternalDatum.Get<float>(0), myDatumTypes[1].Get<float>(0));
				Assert::AreEqual(myExternalDatum.Size(), myDatumTypes[1].Size());
				Assert::AreEqual(myExternalDatum.Capacity(), myDatumTypes[1].Capacity());
				Assert::ExpectException<std::runtime_error>([&myExternalDatum]() {myExternalDatum.PopBack(); });

				myExternalDatum.SetStorage(&myDatumTypes[2].Get<std::string>(0), myDatumTypes[2].Size());
				Assert::AreSame(myExternalDatum.Get<std::string>(0), myDatumTypes[2].Get<std::string>(0));
				Assert::AreEqual(myExternalDatum.Size(), myDatumTypes[2].Size());
				Assert::AreEqual(myExternalDatum.Capacity(), myDatumTypes[2].Capacity());
				Assert::ExpectException<std::runtime_error>([&myExternalDatum]() {myExternalDatum.PopBack(); });

				myExternalDatum.SetStorage(&myDatumTypes[3].Get<glm::vec4>(0), myDatumTypes[3].Size());
				Assert::AreSame(myExternalDatum.Get<glm::vec4>(0), myDatumTypes[3].Get<glm::vec4>(0));
				Assert::AreEqual(myExternalDatum.Size(), myDatumTypes[3].Size());
				Assert::AreEqual(myExternalDatum.Capacity(), myDatumTypes[3].Capacity());
				Assert::ExpectException<std::runtime_error>([&myExternalDatum]() {myExternalDatum.PopBack(); });

				myExternalDatum.SetStorage(&myDatumTypes[4].Get<glm::mat4>(0), myDatumTypes[4].Size());
				Assert::AreSame(myExternalDatum.Get<glm::mat4>(0), myDatumTypes[4].Get<glm::mat4>(0));
				Assert::AreEqual(myExternalDatum.Size(), myDatumTypes[4].Size());
				Assert::AreEqual(myExternalDatum.Capacity(), myDatumTypes[4].Capacity());
				Assert::ExpectException<std::runtime_error>([&myExternalDatum]() {myExternalDatum.PopBack(); });

				myExternalDatum.SetStorage(&myDatumTypes[5].Get<RTTI*>(0), myDatumTypes[5].Size());
				Assert::AreSame(myExternalDatum.Get<RTTI*>(0), myDatumTypes[5].Get<RTTI*>(0));
				Assert::AreEqual(myExternalDatum.Size(), myDatumTypes[5].Size());
				Assert::AreEqual(myExternalDatum.Capacity(), myDatumTypes[5].Capacity());
				Assert::ExpectException<std::runtime_error>([&myExternalDatum]() {myExternalDatum.PopBack(); });

				delete[] myDatumTypes;
			}

			//Testing void* version 3-11-2022
			{
				//void* to integer
				{
					int32_t myIntArray[5] = { 1,2,3,4,5 };
					void* voidPtr = &myIntArray;
					Datum myExternalIntDatum;
					myExternalIntDatum.SetStorage(DatumTypes::Integer, voidPtr, 5);

					Assert::AreSame(myExternalIntDatum.Get<int32_t>(0), *myIntArray);
					Assert::AreEqual(myExternalIntDatum.Size(), static_cast<size_t>(5));
					Assert::AreEqual(myExternalIntDatum.Capacity(), static_cast<size_t>(5));
					Assert::AreEqual(myExternalIntDatum.Get<int32_t>(0), static_cast<int32_t>(1));
					Assert::AreEqual(myExternalIntDatum.Get<int32_t>(1), static_cast<int32_t>(2));
					Assert::AreEqual(myExternalIntDatum.Get<int32_t>(2), static_cast<int32_t>(3));
					Assert::AreEqual(myExternalIntDatum.Get<int32_t>(3), static_cast<int32_t>(4));
					Assert::AreEqual(myExternalIntDatum.Get<int32_t>(4), static_cast<int32_t>(5));

				}

				{
					std::string myStrArray[5] = { "Hello1","Hello2","Hello3","Hello4","Hello5" };
					void* voidPtr = &myStrArray;
					Datum myExternalStrDatum;
					myExternalStrDatum.SetStorage(DatumTypes::String, voidPtr, 5);

					Assert::AreSame(myExternalStrDatum.Get<std::string>(0), *myStrArray);
					Assert::AreEqual(myExternalStrDatum.Size(), static_cast<size_t>(5));
					Assert::AreEqual(myExternalStrDatum.Capacity(), static_cast<size_t>(5));
					Assert::AreEqual(myExternalStrDatum.Get<std::string>(0), "Hello1"s);
					Assert::AreEqual(myExternalStrDatum.Get<std::string>(1), "Hello2"s);
					Assert::AreEqual(myExternalStrDatum.Get<std::string>(2), "Hello3"s);
					Assert::AreEqual(myExternalStrDatum.Get<std::string>(3), "Hello4"s);
					Assert::AreEqual(myExternalStrDatum.Get<std::string>(4), "Hello5"s);
				}

			}
		}
		TEST_METHOD(DatumReserve)
		{
			//Test on default Datum
			{
				Datum myDatum;
				Assert::ExpectException<std::runtime_error>([&myDatum]() {myDatum.Reserve(10); });
			}

			//Test on all Datum types
			{

			}

		}
		TEST_METHOD(DatumOOEqual)
		{
			//Test default constructed Datum
			{
				Datum myDatum1;
				Datum myDatum2;

				bool areEqual = (myDatum1 == myDatum2);
				Assert::IsTrue(areEqual);
			}

			//Testing Datum of string type equivalent.
			{
				Datum myDatum1("Hello!"s);
				Datum myDatum2("Hello!"s);

				bool areEqual = (myDatum1 == myDatum2);
				Assert::IsTrue(areEqual);
			}

			//Testing Datum of string type non-equivalent.
			{
				Datum myDatum1("Hello!"s);
				Datum myDatum2("Hello! World!"s);

				bool areEqual = (myDatum1 == myDatum2);
				Assert::IsFalse(areEqual);
			}


			//Testing on all valid Datum types. When comparing equivalent datums and using == opearator. Same type, size and capacity.
			{
				bool areEqual;

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
#pragma region Datum Set 1 Control Group

				//Datum int
				Datum myIntDatum1 = { 1,2,3 };
				Datum myIntDatum2 = { 1,2,3 };

				//Datum float
				Datum myFloatDatum1 = { 1.0f, 2.0f, 3.0f };
				Datum myFloatDatum2 = { 1.0f, 2.0f, 3.0f };

				//Datum string
				Datum myStringDatum1 = { "a"s, "b"s,"c"s };
				Datum myStringDatum2 = { "a"s, "b"s,"c"s };

				//Datum Vector
				Datum myVecDatum1 = { myVec1, myVec2, myVec3 };
				Datum myVecDatum2 = { myVec1, myVec2, myVec3 };

				//Datum Matrix
				Datum myMatrixDatum1 = { myMat1, myMat2, myMat3 };
				Datum myMatrixDatum2 = { myMat1, myMat2, myMat3 };

				//Datum RTTI*
				Datum myRTTIDatum1 = { myRTTI1, myRTTI2, myRTTI3 };
				Datum myRTTIDatum2 = { myRTTI1, myRTTI2, myRTTI3 };

				Scope::pt_string_datum myScope1Apair1("SC1A: A Vector Datum 1", myVecDatum1);
				Scope::pt_string_datum myScope1Apair2("SC1A: A Matrix Datum 1", myMatrixDatum1);
				Scope::pt_string_datum myScope1Apair3("SC1A: A RTTI* Datum 1", myRTTIDatum1);

				Scope::pt_string_datum myScope1Bpair1("SC1B: A Vector Datum 2", myVecDatum2);
				Scope::pt_string_datum myScope1Bpair2("SC1B: A Matrix Datum 2", myMatrixDatum2);
				Scope::pt_string_datum myScope1Bpair3("SC1B: A RTTI* Datum 2", myRTTIDatum2);

				Scope myStackScope1ControlGroup;
				Scope& heapScope1AControlGroup = myStackScope1ControlGroup.AppendScope("S1A:");
				Scope& heapScope1BControlGroup = myStackScope1ControlGroup.AppendScope("S1B:");

				heapScope1AControlGroup.Append(myScope1Apair1);
				heapScope1AControlGroup.Append(myScope1Apair2);
				heapScope1AControlGroup.Append(myScope1Apair3);

				heapScope1BControlGroup.Append(myScope1Bpair1);
				heapScope1BControlGroup.Append(myScope1Bpair2);
				heapScope1BControlGroup.Append(myScope1Bpair3);

#pragma endregion

#pragma region Datum Set 2 DIFFERENT SIZE

				//Datum int
				Datum myIntDatum1DiffSize = { 1,2 };
				Datum myIntDatum2DiffSize = { 1,2 };

				//Datum float
				Datum myFloatDatum1DiffSize = { 1.0f, 2.0f };
				Datum myFloatDatum2DiffSize = { 1.0f, 2.0f };

				//Datum string
				Datum myStringDatum1DiffSize = { "a"s, "b"s };
				Datum myStringDatum2DiffSize = { "a"s, "b"s };

				//Datum Vector
				Datum myVecDatum1DiffSize = { myVec1, myVec2 };
				Datum myVecDatum2DiffSize = { myVec1, myVec2 };

				//Datum Matrix
				Datum myMatrixDatum1DiffSize = { myMat1, myMat2 };
				Datum myMatrixDatum2DiffSize = { myMat1, myMat2 };

				//Datum RTTI*
				Datum myRTTIDatum1DiffSize = { myRTTI1, myRTTI2 };
				Datum myRTTIDatum2DiffSize = { myRTTI1, myRTTI2 };

				Scope::pt_string_datum myScope2Apair1("SC2A: A Vector Datum Different Size", myVecDatum1DiffSize);
				Scope::pt_string_datum myScope2Apair2("SC2A: A Matrix Datum Different Size", myMatrixDatum1DiffSize);
				Scope::pt_string_datum myScope2Apair3("SC2A: A RTTI* Datum Different Size", myRTTIDatum1DiffSize);

				Scope::pt_string_datum myScope2Bpair1("SC2B: A Vector Datum Different Size", myVecDatum2DiffSize);
				Scope::pt_string_datum myScope2Bpair2("SC2B: A Matrix Datum Different Size", myMatrixDatum2DiffSize);
				Scope::pt_string_datum myScope2Bpair3("SC2B: A RTTI* Datum Different Size", myRTTIDatum2DiffSize);

				Scope myStackScope2DifferentSize; //WILL HOLD A DATUM OF TYPE TABLE THAT HOLDS TWO ITEMS

				Scope& heapScope2ADifferentSize = myStackScope2DifferentSize.AppendScope("S2A:");
				Scope& heapScope2BDifferentSize = myStackScope2DifferentSize.AppendScope("S2A:"); //WILL BE PUSHED BACK INTO DATUM MAKING DATUM OF SIZE 2

				heapScope2ADifferentSize.Append(myScope2Apair1);
				heapScope2ADifferentSize.Append(myScope2Apair2);
				heapScope2ADifferentSize.Append(myScope2Apair3);

				heapScope2BDifferentSize.Append(myScope2Bpair1);
				heapScope2BDifferentSize.Append(myScope2Bpair2);
				heapScope2BDifferentSize.Append(myScope2Bpair3);



#pragma endregion

//#pragma region Datum Set 3 DIFFERENT CAPACITY
//
//				//Datum int
//				Datum myIntDatum1DiffCapacity = { 1,2,3 };
//				Datum myIntDatum2DiffCapacity = { 1,2,3 };
//
//				myIntDatum1DiffCapacity.Reserve(4);
//				myIntDatum2DiffCapacity.Reserve(4);
//
//				//Datum float
//				Datum myFloatDatum1DiffCapacity = { 1.0f, 2.0f, 3.0f };
//				Datum myFloatDatum2DiffCapacity = { 1.0f, 2.0f, 3.0f };
//
//				myFloatDatum1DiffCapacity.Reserve(4);
//				myFloatDatum2DiffCapacity.Reserve(4);
//
//				//Datum string
//				Datum myStringDatum1DiffCapacity = { "a"s, "b"s,"c"s };
//				Datum myStringDatum2DiffCapacity = { "a"s, "b"s,"c"s };
//
//				myStringDatum1DiffCapacity.Reserve(4);
//				myStringDatum2DiffCapacity.Reserve(4);
//
//				//Datum Vector
//				Datum myVecDatum1DiffCapacity = { myVec1, myVec2, myVec3 };
//				Datum myVecDatum2DiffCapacity = { myVec1, myVec2, myVec3 };
//
//				myVecDatum1DiffCapacity.Reserve(4);
//				myVecDatum2DiffCapacity.Reserve(4);
//
//				//Datum Matrix
//				Datum myMatrixDatum1DiffCapacity = { myMat1, myMat2, myMat3 };
//				Datum myMatrixDatum2DiffCapacity = { myMat1, myMat2, myMat3 };
//
//				myMatrixDatum1DiffCapacity.Reserve(4);
//				myMatrixDatum2DiffCapacity.Reserve(4);
//
//				//Datum RTTI*
//				Datum myRTTIDatum1DiffCapacity = { myRTTI1, myRTTI2, myRTTI3 };
//				Datum myRTTIDatum2DiffCapacity = { myRTTI1, myRTTI2, myRTTI3 };
//
//				myRTTIDatum1DiffCapacity.Reserve(4);
//				myRTTIDatum2DiffCapacity.Reserve(4);
//
//				Scope::pt_string_datum myScope3Apair1("SC3A: A Vector Datum 5", myVecDatum1DiffCapacity);
//				Scope::pt_string_datum myScope3Apair2("SC3A: A Matrix Datum 5", myMatrixDatum1DiffCapacity);
//				Scope::pt_string_datum myScope3Apair3("SC3A: A RTTI* Datum 5", myRTTIDatum1DiffCapacity);
//
//				Scope::pt_string_datum myScope3Bpair1("SC3B: A Vector Datum 6", myVecDatum2DiffCapacity);
//				Scope::pt_string_datum myScope3Bpair2("SC3B: A Matrix Datum 6", myMatrixDatum2DiffCapacity);
//				Scope::pt_string_datum myScope3Bpair3("SC3B: A RTTI* Datum 6", myRTTIDatum2DiffCapacity);
//
//				Scope myStackScope3DifferentCapacity;
//				Scope& heapScope3ADifferentCapacity = myStackScope3DifferentCapacity.AppendScope("S3A:");
//				Scope& heapScope3BDifferentCapacity = myStackScope3DifferentCapacity.AppendScope("S3B:");
//
//				heapScope3ADifferentCapacity.Append(myScope3Apair1);
//				heapScope3ADifferentCapacity.Append(myScope3Apair2);
//				heapScope3ADifferentCapacity.Append(myScope3Apair3);
//
//				heapScope3BDifferentCapacity.Append(myScope3Bpair1);
//				heapScope3BDifferentCapacity.Append(myScope3Bpair2);
//				heapScope3BDifferentCapacity.Append(myScope3Bpair3);
//
//				Scope::pt_datumPtr_index pair = myStackScope3DifferentCapacity.FindContainedScope(heapScope3ADifferentCapacity);
//				Datum* datum = pair.first;
//				datum->Reserve(4);
//
//				pair = myStackScope3DifferentCapacity.FindContainedScope(heapScope3BDifferentCapacity);
//				datum = pair.first;
//				datum->Reserve(4);
//
//#pragma endregion

				//Testing on all valid Datum types. When comparing equivalent datums using == opearator. Same type, size and capacity.
				{
					//Datum int
					areEqual = (myIntDatum1 == myIntDatum2);
					Assert::IsTrue(areEqual);

					//Datum float
					areEqual = (myFloatDatum1 == myFloatDatum2);
					Assert::IsTrue(areEqual);

					//Datum string
					areEqual = (myStringDatum1 == myStringDatum2);
					Assert::IsTrue(areEqual);

					//Datum vector				
					areEqual = (myVecDatum1 == myVecDatum2);
					Assert::IsTrue(areEqual);

					//Datum Matrix
					areEqual = (myMatrixDatum1 == myMatrixDatum2);
					Assert::IsTrue(areEqual);

					//Datum RTTI*
					areEqual = (myRTTIDatum1 == myRTTIDatum2);
					Assert::IsTrue(areEqual);

					//Datum Scope*
					Scope::pt_datumPtr_index myPair = myStackScope1ControlGroup.FindContainedScope(heapScope1AControlGroup);

					Datum* foundDatum = myPair.first;
					std::size_t index = myPair.second;

					Scope& foundScope = foundDatum->Get<Scope>(index);

					Datum* foundRTTIDatum1Equivalent = foundScope.Find("SC1A: A RTTI* Datum 1");
					foundRTTIDatum1Equivalent;
					areEqual = ( *foundRTTIDatum1Equivalent == myRTTIDatum1);
					Assert::IsTrue(areEqual);

				}

				//Testing on all valid Datum types. When comparing non-equivalent datums using == opearator. Differemt type, same size and same capacity.
				{

					//int to float
					areEqual = (myIntDatum1 == myFloatDatum1);
					Assert::IsFalse(areEqual);

					//float to string
					areEqual = (myFloatDatum1 == myStringDatum1);
					Assert::IsFalse(areEqual);

					//string to vector
					areEqual = (myStringDatum1 == myVecDatum1);
					Assert::IsFalse(areEqual);

					//vector to matrix				
					areEqual = (myVecDatum1 == myMatrixDatum1);
					Assert::IsFalse(areEqual);

					//matrix to RTTI*
					areEqual = (myMatrixDatum1 == myRTTIDatum1);
					Assert::IsFalse(areEqual);


					Scope::pt_datumPtr_index myPair = myStackScope1ControlGroup.FindContainedScope(heapScope1AControlGroup);
					Datum* foundDatum = myPair.first;
					std::size_t index = myPair.second;
					Scope& foundScope = foundDatum->Get<Scope>(index);
					Datum* foundRTTIDatum1Equivalent = foundScope.Find("SC1A: A RTTI* Datum 1");

					//RTTI* to Scope*
					areEqual = (myRTTIDatum1 == *foundDatum); //foundDatum is Datum of type Table
					Assert::IsFalse(areEqual);

					//Scope* to int
					areEqual = (*foundRTTIDatum1Equivalent == myIntDatum1);
					Assert::IsFalse(areEqual);

				}

				//Testing on all valid Datum types. When comparing non-equivalent datums using == opearator. Same type, different size and same capacity.
				{
					//int datums of different sizes
					areEqual = (myIntDatum1 == myIntDatum1DiffSize);
					Assert::IsFalse(areEqual);

					//float datums of different sizes
					areEqual = (myFloatDatum1 == myFloatDatum1DiffSize);
					Assert::IsFalse(areEqual);

					//string datums of different sizes
					areEqual = (myStringDatum1 == myStringDatum1DiffSize);
					Assert::IsFalse(areEqual);

					//vector datums of different sizes				
					areEqual = (myVecDatum1 == myVecDatum1DiffSize);
					Assert::IsFalse(areEqual);

					//matrix datums of different sizes
					areEqual = (myMatrixDatum1 == myMatrixDatum1DiffSize);
					Assert::IsFalse(areEqual);

					//RTTI* datums of different sizes
					areEqual = (myRTTIDatum1 == myRTTIDatum1DiffSize);
					Assert::IsFalse(areEqual);

					Scope::pt_datumPtr_index myPair = myStackScope1ControlGroup.FindContainedScope(heapScope1AControlGroup);
					Datum* foundTableDatumControlGroup = myPair.first;

					myPair = myStackScope2DifferentSize.FindContainedScope(heapScope2ADifferentSize);
					Datum* foundTableDatumDifferentSize = myPair.first;

					//Scope* to int
					areEqual = (*foundTableDatumControlGroup == *foundTableDatumDifferentSize);
					Assert::IsFalse(areEqual);

				}																		 
																						 
				//Testing on all valid Datum types. When comparing non-equivalent datums using == opearator. Same type, same size and different capacity.
				//{
				//	//int datums of different sizes
				//	areEqual = (myIntDatum1 == myIntDatum2DiffCapacity);
				//	Assert::IsFalse(areEqual);

				//	//float datums of different sizes
				//	areEqual = (myFloatDatum1 == myFloatDatum1DiffCapacity);
				//	Assert::IsFalse(areEqual);

				//	//string datums of different sizes
				//	areEqual = (myStringDatum1 == myStringDatum1DiffCapacity);
				//	Assert::IsFalse(areEqual);

				//	//vector datums of different sizes				
				//	areEqual = (myVecDatum1 == myVecDatum1DiffCapacity);
				//	Assert::IsFalse(areEqual);

				//	//matrix datums of different sizes
				//	areEqual = (myMatrixDatum1 == myMatrixDatum1DiffCapacity);
				//	Assert::IsFalse(areEqual);

				//	//RTTI* datums of different sizes
				//	areEqual = (myRTTIDatum1 == myRTTIDatum1DiffCapacity);
				//	Assert::IsFalse(areEqual);

				//	Scope::pt_datumPtr_index myPair = myStackScope1ControlGroup.FindContainedScope(heapScope1AControlGroup);
				//	Datum* foundTableDatumControlGroup = myPair.first;

				//	myPair = myStackScope3DifferentCapacity.FindContainedScope(heapScope3ADifferentCapacity);
				//	Datum* foundTableDatumDifferentCapacity = myPair.first;

				//	//Scope* to int
				//	areEqual = (*foundTableDatumControlGroup == *foundTableDatumDifferentCapacity);
				//	Assert::IsFalse(areEqual);

				//}																		 

			}

		}
		TEST_METHOD(DatumOONotEqual)
		{
			
			//Test default constructed Datum
			{
				Datum myDatum1;
				Datum myDatum2;

				bool areEqual = (myDatum1 != myDatum2);
				Assert::IsFalse(areEqual);
			}

			//Testing Datum of string type equivalent.
			{
				Datum myDatum1("Hello!"s);
				Datum myDatum2("Hello!"s);

				bool areEqual = (myDatum1 != myDatum2);
				Assert::IsFalse(areEqual);
			}

			//Testing Datum of string type non-equivalent.
			{
				Datum myDatum1("Hello!"s);
				Datum myDatum2("Hello! World!"s);

				bool areEqual = (myDatum1 != myDatum2);
				Assert::IsTrue(areEqual);
			}

			//Testing on all valid Datum types. When comparing equivalent datums and using == opearator. Same type, size and capacity.
			{
				bool areEqual;

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
#pragma region Datum Set 1 Control Group

				//Datum int
				Datum myIntDatum1 = { 1,2,3 };
				Datum myIntDatum2 = { 1,2,3 };

				//Datum float
				Datum myFloatDatum1 = { 1.0f, 2.0f, 3.0f };
				Datum myFloatDatum2 = { 1.0f, 2.0f, 3.0f };

				//Datum string
				Datum myStringDatum1 = { "a"s, "b"s,"c"s };
				Datum myStringDatum2 = { "a"s, "b"s,"c"s };

				//Datum Vector
				Datum myVecDatum1 = { myVec1, myVec2, myVec3 };
				Datum myVecDatum2 = { myVec1, myVec2, myVec3 };

				//Datum Matrix
				Datum myMatrixDatum1 = { myMat1, myMat2, myMat3 };
				Datum myMatrixDatum2 = { myMat1, myMat2, myMat3 };

				//Datum RTTI*
				Datum myRTTIDatum1 = { myRTTI1, myRTTI2, myRTTI3 };
				Datum myRTTIDatum2 = { myRTTI1, myRTTI2, myRTTI3 };

				Scope::pt_string_datum myScope1Apair1("SC1A: A Vector Datum 1", myVecDatum1);
				Scope::pt_string_datum myScope1Apair2("SC1A: A Matrix Datum 1", myMatrixDatum1);
				Scope::pt_string_datum myScope1Apair3("SC1A: A RTTI* Datum 1", myRTTIDatum1);

				Scope::pt_string_datum myScope1Bpair1("SC1B: A Vector Datum 2", myVecDatum2);
				Scope::pt_string_datum myScope1Bpair2("SC1B: A Matrix Datum 2", myMatrixDatum2);
				Scope::pt_string_datum myScope1Bpair3("SC1B: A RTTI* Datum 2", myRTTIDatum2);

				Scope myStackScope1ControlGroup;
				Scope& heapScope1AControlGroup = myStackScope1ControlGroup.AppendScope("S1A:");
				Scope& heapScope1BControlGroup = myStackScope1ControlGroup.AppendScope("S1B:");

				heapScope1AControlGroup.Append(myScope1Apair1);
				heapScope1AControlGroup.Append(myScope1Apair2);
				heapScope1AControlGroup.Append(myScope1Apair3);

				heapScope1BControlGroup.Append(myScope1Bpair1);
				heapScope1BControlGroup.Append(myScope1Bpair2);
				heapScope1BControlGroup.Append(myScope1Bpair3);

#pragma endregion

#pragma region Datum Set 2 DIFFERENT SIZE

				//Datum int
				Datum myIntDatum1DiffSize = { 1,2 };
				Datum myIntDatum2DiffSize = { 1,2 };

				//Datum float
				Datum myFloatDatum1DiffSize = { 1.0f, 2.0f };
				Datum myFloatDatum2DiffSize = { 1.0f, 2.0f };

				//Datum string
				Datum myStringDatum1DiffSize = { "a"s, "b"s };
				Datum myStringDatum2DiffSize = { "a"s, "b"s };

				//Datum Vector
				Datum myVecDatum1DiffSize = { myVec1, myVec2 };
				Datum myVecDatum2DiffSize = { myVec1, myVec2 };

				//Datum Matrix
				Datum myMatrixDatum1DiffSize = { myMat1, myMat2 };
				Datum myMatrixDatum2DiffSize = { myMat1, myMat2 };

				//Datum RTTI*
				Datum myRTTIDatum1DiffSize = { myRTTI1, myRTTI2 };
				Datum myRTTIDatum2DiffSize = { myRTTI1, myRTTI2 };

				Scope::pt_string_datum myScope2Apair1("SC2A: A Vector Datum Different Size", myVecDatum1DiffSize);
				Scope::pt_string_datum myScope2Apair2("SC2A: A Matrix Datum Different Size", myMatrixDatum1DiffSize);
				Scope::pt_string_datum myScope2Apair3("SC2A: A RTTI* Datum Different Size", myRTTIDatum1DiffSize);

				Scope::pt_string_datum myScope2Bpair1("SC2B: A Vector Datum Different Size", myVecDatum2DiffSize);
				Scope::pt_string_datum myScope2Bpair2("SC2B: A Matrix Datum Different Size", myMatrixDatum2DiffSize);
				Scope::pt_string_datum myScope2Bpair3("SC2B: A RTTI* Datum Different Size", myRTTIDatum2DiffSize);

				Scope myStackScope2DifferentSize; //WILL HOLD A DATUM OF TYPE TABLE THAT HOLDS TWO ITEMS

				Scope& heapScope2ADifferentSize = myStackScope2DifferentSize.AppendScope("S2A:");
				Scope& heapScope2BDifferentSize = myStackScope2DifferentSize.AppendScope("S2A:"); //WILL BE PUSHED BACK INTO DATUM MAKING DATUM OF SIZE 2

				heapScope2ADifferentSize.Append(myScope2Apair1);
				heapScope2ADifferentSize.Append(myScope2Apair2);
				heapScope2ADifferentSize.Append(myScope2Apair3);

				heapScope2BDifferentSize.Append(myScope2Bpair1);
				heapScope2BDifferentSize.Append(myScope2Bpair2);
				heapScope2BDifferentSize.Append(myScope2Bpair3);



#pragma endregion

//#pragma region Datum Set 3 DIFFERENT CAPACITY
//
//				//Datum int
//				Datum myIntDatum1DiffCapacity = { 1,2,3 };
//				Datum myIntDatum2DiffCapacity = { 1,2,3 };
//
//				myIntDatum1DiffCapacity.Reserve(4);
//				myIntDatum2DiffCapacity.Reserve(4);
//
//				//Datum float
//				Datum myFloatDatum1DiffCapacity = { 1.0f, 2.0f, 3.0f };
//				Datum myFloatDatum2DiffCapacity = { 1.0f, 2.0f, 3.0f };
//
//				myFloatDatum1DiffCapacity.Reserve(4);
//				myFloatDatum2DiffCapacity.Reserve(4);
//
//				//Datum string
//				Datum myStringDatum1DiffCapacity = { "a"s, "b"s,"c"s };
//				Datum myStringDatum2DiffCapacity = { "a"s, "b"s,"c"s };
//
//				myStringDatum1DiffCapacity.Reserve(4);
//				myStringDatum2DiffCapacity.Reserve(4);
//
//				//Datum Vector
//				Datum myVecDatum1DiffCapacity = { myVec1, myVec2, myVec3 };
//				Datum myVecDatum2DiffCapacity = { myVec1, myVec2, myVec3 };
//
//				myVecDatum1DiffCapacity.Reserve(4);
//				myVecDatum2DiffCapacity.Reserve(4);
//
//				//Datum Matrix
//				Datum myMatrixDatum1DiffCapacity = { myMat1, myMat2, myMat3 };
//				Datum myMatrixDatum2DiffCapacity = { myMat1, myMat2, myMat3 };
//
//				myMatrixDatum1DiffCapacity.Reserve(4);
//				myMatrixDatum2DiffCapacity.Reserve(4);
//
//				//Datum RTTI*
//				Datum myRTTIDatum1DiffCapacity = { myRTTI1, myRTTI2, myRTTI3 };
//				Datum myRTTIDatum2DiffCapacity = { myRTTI1, myRTTI2, myRTTI3 };
//
//				myRTTIDatum1DiffCapacity.Reserve(4);
//				myRTTIDatum2DiffCapacity.Reserve(4);
//
//				Scope::pt_string_datum myScope3Apair1("SC3A: A Vector Datum 5", myVecDatum1DiffCapacity);
//				Scope::pt_string_datum myScope3Apair2("SC3A: A Matrix Datum 5", myMatrixDatum1DiffCapacity);
//				Scope::pt_string_datum myScope3Apair3("SC3A: A RTTI* Datum 5", myRTTIDatum1DiffCapacity);
//
//				Scope::pt_string_datum myScope3Bpair1("SC3B: A Vector Datum 6", myVecDatum2DiffCapacity);
//				Scope::pt_string_datum myScope3Bpair2("SC3B: A Matrix Datum 6", myMatrixDatum2DiffCapacity);
//				Scope::pt_string_datum myScope3Bpair3("SC3B: A RTTI* Datum 6", myRTTIDatum2DiffCapacity);
//
//				Scope myStackScope3DifferentCapacity;
//				Scope& heapScope3ADifferentCapacity = myStackScope3DifferentCapacity.AppendScope("S3A:");
//				Scope& heapScope3BDifferentCapacity = myStackScope3DifferentCapacity.AppendScope("S3B:");
//
//				heapScope3ADifferentCapacity.Append(myScope3Apair1);
//				heapScope3ADifferentCapacity.Append(myScope3Apair2);
//				heapScope3ADifferentCapacity.Append(myScope3Apair3);
//
//				heapScope3BDifferentCapacity.Append(myScope3Bpair1);
//				heapScope3BDifferentCapacity.Append(myScope3Bpair2);
//				heapScope3BDifferentCapacity.Append(myScope3Bpair3);
//
//				Scope::pt_datumPtr_index pair = myStackScope3DifferentCapacity.FindContainedScope(heapScope3ADifferentCapacity);
//				Datum* datum = pair.first;
//				datum->Reserve(4);
//
//				pair = myStackScope3DifferentCapacity.FindContainedScope(heapScope3BDifferentCapacity);
//				datum = pair.first;
//				datum->Reserve(4);
//
//#pragma endregion

				//Testing on all valid Datum types. When comparing equivalent datums using == opearator. Same type, size and capacity.
				{
					//Datum int
					areEqual = (myIntDatum1 != myIntDatum2);
					Assert::IsFalse(areEqual);

					//Datum float
					areEqual = (myFloatDatum1 != myFloatDatum2);
					Assert::IsFalse(areEqual);

					//Datum string
					areEqual = (myStringDatum1 != myStringDatum2);
					Assert::IsFalse(areEqual);

					//Datum vector				
					areEqual = (myVecDatum1 != myVecDatum2);
					Assert::IsFalse(areEqual);

					//Datum Matrix
					areEqual = (myMatrixDatum1 != myMatrixDatum2);
					Assert::IsFalse(areEqual);

					//Datum RTTI*
					areEqual = (myRTTIDatum1 != myRTTIDatum2);
					Assert::IsFalse(areEqual);

					//Datum Scope*
					Scope::pt_datumPtr_index myPair = myStackScope1ControlGroup.FindContainedScope(heapScope1AControlGroup);

					Datum* foundDatum = myPair.first;
					std::size_t index = myPair.second;

					Scope& foundScope = foundDatum->Get<Scope>(index);

					Datum* foundRTTIDatum1Equivalent = foundScope.Find("SC1A: A RTTI* Datum 1");
					foundRTTIDatum1Equivalent;
					areEqual = (*foundRTTIDatum1Equivalent != myRTTIDatum1);
					Assert::IsFalse(areEqual);

				}

				//Testing on all valid Datum types. When comparing non-equivalent datums using == opearator. Differemt type, same size and same capacity.
				{

					//int to float
					areEqual = (myIntDatum1 != myFloatDatum1);
					Assert::IsTrue(areEqual);

					//float to string
					areEqual = (myFloatDatum1 != myStringDatum1);
					Assert::IsTrue(areEqual);

					//string to vector
					areEqual = (myStringDatum1 != myVecDatum1);
					Assert::IsTrue(areEqual);

					//vector to matrix				
					areEqual = (myVecDatum1 != myMatrixDatum1);
					Assert::IsTrue(areEqual);

					//matrix to RTTI*
					areEqual = (myMatrixDatum1 != myRTTIDatum1);
					Assert::IsTrue(areEqual);


					Scope::pt_datumPtr_index myPair = myStackScope1ControlGroup.FindContainedScope(heapScope1AControlGroup);
					Datum* foundDatum = myPair.first;
					std::size_t index = myPair.second;
					Scope& foundScope = foundDatum->Get<Scope>(index);
					Datum* foundRTTIDatum1Equivalent = foundScope.Find("SC1A: A RTTI* Datum 1");

					//RTTI* to Scope*
					areEqual = (myRTTIDatum1 != *foundDatum); //foundDatum is Datum of type Table
					Assert::IsTrue(areEqual);

					//Scope* to int
					areEqual = (*foundRTTIDatum1Equivalent != myIntDatum1);
					Assert::IsTrue(areEqual);

				}

				//Testing on all valid Datum types. When comparing non-equivalent datums using == opearator. Same type, different size and same capacity.
				{
					//int datums of different sizes
					areEqual = (myIntDatum1 != myIntDatum1DiffSize);
					Assert::IsTrue(areEqual);

					//float datums of different sizes
					areEqual = (myFloatDatum1 != myFloatDatum1DiffSize);
					Assert::IsTrue(areEqual);

					//string datums of different sizes
					areEqual = (myStringDatum1 != myStringDatum1DiffSize);
					Assert::IsTrue(areEqual);

					//vector datums of different sizes				
					areEqual = (myVecDatum1 != myVecDatum1DiffSize);
					Assert::IsTrue(areEqual);

					//matrix datums of different sizes
					areEqual = (myMatrixDatum1 != myMatrixDatum1DiffSize);
					Assert::IsTrue(areEqual);

					//RTTI* datums of different sizes
					areEqual = (myRTTIDatum1 != myRTTIDatum1DiffSize);
					Assert::IsTrue(areEqual);

					Scope::pt_datumPtr_index myPair = myStackScope1ControlGroup.FindContainedScope(heapScope1AControlGroup);
					Datum* foundTableDatumControlGroup = myPair.first;

					myPair = myStackScope2DifferentSize.FindContainedScope(heapScope2ADifferentSize);
					Datum* foundTableDatumDifferentSize = myPair.first;

					//Scope* to int
					areEqual = (*foundTableDatumControlGroup != *foundTableDatumDifferentSize);
					Assert::IsTrue(areEqual);

				}

				//Testing on all valid Datum types. When comparing non-equivalent datums using == opearator. Same type, same size and different capacity.
				//{
				//	//int datums of different sizes
				//	areEqual = (myIntDatum1 != myIntDatum2DiffCapacity);
				//	Assert::IsTrue(areEqual);

				//	//float datums of different sizes
				//	areEqual = (myFloatDatum1 != myFloatDatum1DiffCapacity);
				//	Assert::IsTrue(areEqual);

				//	//string datums of different sizes
				//	areEqual = (myStringDatum1 != myStringDatum1DiffCapacity);
				//	Assert::IsTrue(areEqual);

				//	//vector datums of different sizes				
				//	areEqual = (myVecDatum1 != myVecDatum1DiffCapacity);
				//	Assert::IsTrue(areEqual);

				//	//matrix datums of different sizes
				//	areEqual = (myMatrixDatum1 != myMatrixDatum1DiffCapacity);
				//	Assert::IsTrue(areEqual);

				//	//RTTI* datums of different sizes
				//	areEqual = (myRTTIDatum1 != myRTTIDatum1DiffCapacity);
				//	Assert::IsTrue(areEqual);

				//	Scope::pt_datumPtr_index myPair = myStackScope1ControlGroup.FindContainedScope(heapScope1AControlGroup);
				//	Datum* foundTableDatumControlGroup = myPair.first;

				//	myPair = myStackScope3DifferentCapacity.FindContainedScope(heapScope3ADifferentCapacity);
				//	Datum* foundTableDatumDifferentCapacity = myPair.first;

				//	//Scope* to int
				//	areEqual = (*foundTableDatumControlGroup != *foundTableDatumDifferentCapacity);
				//	Assert::IsTrue(areEqual);

				//}

			}

		}
		TEST_METHOD(DatumSet)
		{
			//Testing on all valid Datum types.
			{
				glm::vec4 myVec(1.0f, 1.0f, 1.0, 1.0);
				glm::mat4 myMat(1.0f);
				Foo myFoo(1);
				RTTI* myRTTI = &myFoo;

				Datum* myDatumTypes1 = new Datum[static_cast<std::size_t>(DatumTypes::End)]
				{ 1, 1.0f, ""s, myVec, myMat, myRTTI };


				glm::vec4 myVec2(2.0f, 2.0f, 2.0f, 2.0f);
				glm::mat4 myMat2(1.0f);
				Foo myFoo2(1);
				RTTI* myRTTI2 = &myFoo2;
				
				Datum* myDatumTypes2 = new Datum[static_cast<std::size_t>(DatumTypes::End)]
				{ 2, 2.0f, "Alex"s, myVec2, myMat2, myRTTI2 };

				myDatumTypes1[0].Set(myDatumTypes2[0].Get<int>(0), 0);
				Assert::AreEqual(2, myDatumTypes1[0].Get<int>(0));

				myDatumTypes1[1].Set(myDatumTypes2[1].Get<float>(0), 0);
				Assert::AreEqual(2.0f, myDatumTypes1[1].Get<float>(0));

				myDatumTypes1[2].Set(myDatumTypes2[2].Get<std::string>(0), 0);
				Assert::AreEqual("Alex"s, myDatumTypes1[2].Get<std::string>(0));

				myDatumTypes1[3].Set(myDatumTypes2[3].Get<glm::vec4>(0), 0);
				Assert::AreEqual(myVec2, myDatumTypes1[3].Get<glm::vec4>(0));

				myDatumTypes1[4].Set(myDatumTypes2[4].Get<glm::mat4>(0), 0);
				Assert::AreEqual(myMat2, myDatumTypes1[4].Get<glm::mat4>(0));

				myDatumTypes1[5].Set(myDatumTypes2[5].Get<RTTI*>(0), 0);
				Assert::AreEqual(myRTTI2, myDatumTypes1[5].Get<RTTI*>(0));

				delete[] myDatumTypes1;
				delete[] myDatumTypes2;
			}

			//Calling Set of type that is not equal to Datums type
			{
				glm::vec4 myVec(1.0f, 1.0f, 1.0, 1.0);
				glm::mat4 myMat(1.0f);
				Foo myFoo(1);
				RTTI* myRTTI = &myFoo;

				Datum* myDatumTypes1 = new Datum[static_cast<std::size_t>(DatumTypes::End)]
				{ 1, 1.0f, ""s, myVec, myMat, myRTTI };

				//setting int to pointer
				Assert::ExpectException<std::runtime_error>([&myDatumTypes1, &myRTTI]() {myDatumTypes1[0].Set(myRTTI, 0);});
				//setting float to Matrix
				Assert::ExpectException<std::runtime_error>([&myDatumTypes1, &myMat]() {myDatumTypes1[1].Set(myMat, 0); });
				//setting std::string to glm::vec4
				Assert::ExpectException<std::runtime_error>([&myDatumTypes1, &myVec]() {myDatumTypes1[2].Set(myVec, 0); });
				//setting glm::vec4 std::string
				Assert::ExpectException<std::runtime_error>([&myDatumTypes1]() {myDatumTypes1[3].Set("", 0); });
				//setting glm::mat4 to float
				Assert::ExpectException<std::runtime_error>([&myDatumTypes1]() {myDatumTypes1[4].Set(1.0f, 0); });
				//setting RTTI* to int
				Assert::ExpectException<std::runtime_error>([&myDatumTypes1]() {myDatumTypes1[5].Set(1, 0); });


				delete[] myDatumTypes1;
			}
		}
		TEST_METHOD(DatumGet)
		{

			//Default Datum
			{
				Datum myDatum;
				Assert::ExpectException<std::runtime_error>([&myDatum]() {myDatum.Get<int>(0); });
				Assert::ExpectException<std::runtime_error>([&myDatum]() {myDatum.Get<float>(0); });
				Assert::ExpectException<std::runtime_error>([&myDatum]() {myDatum.Get<std::string>(0); });
				Assert::ExpectException<std::runtime_error>([&myDatum]() {myDatum.Get<glm::vec4>(0); });
				Assert::ExpectException<std::runtime_error>([&myDatum]() {myDatum.Get<glm::mat4>(0); });
				Assert::ExpectException<std::runtime_error>([&myDatum]() {myDatum.Get<RTTI*>(0); });
			}

			//Testing on all valid Datum types.
			{
				glm::vec4 myVec(1.0f, 1.0f, 1.0, 1.0);
				glm::mat4 myMat(1.0f);
				Foo myFoo(1);
				RTTI* myRTTI = &myFoo;

				Datum* myDatumTypes1 = new Datum[static_cast<std::size_t>(DatumTypes::End)]
				{ 1, 1.0f, ""s, myVec, myMat, myRTTI };

				int myInt = myDatumTypes1[0].Get<int>(0);
				Assert::AreEqual(1, myInt);

				float myFloat = myDatumTypes1[1].Get<float>(0);
				Assert::AreEqual(1.0f, myFloat);

				std::string myString = myDatumTypes1[2].Get<std::string>(0);
				Assert::AreEqual(""s, myString);

				glm::vec4 myOtherVec = myDatumTypes1[3].Get<glm::vec4>(0);
				Assert::AreEqual(myVec, myOtherVec);

				glm::mat4 myOtherMat = myDatumTypes1[4].Get<glm::mat4>(0);
				Assert::AreEqual(myMat, myOtherMat);

				RTTI* myOtherRTTI = myDatumTypes1[5].Get<RTTI*>(0);
				Assert::AreEqual(myRTTI, myOtherRTTI);

				delete[] myDatumTypes1;

			}


			//Testing on all Valid Datum types but calling the incorrect Get
			{

				glm::vec4 myVec(1.0f, 1.0f, 1.0, 1.0);
				glm::mat4 myMat(1.0f);
				Foo myFoo(1);
				RTTI* myRTTI = &myFoo;

				Datum* myDatumTypes1 = new Datum[static_cast<std::size_t>(DatumTypes::End)]
				{ 1, 1.0f, ""s, myVec, myMat, myRTTI };

				//getting pointer from int
				Assert::ExpectException<std::runtime_error>([&myDatumTypes1]() {myDatumTypes1[0].Get<RTTI*>(0); });
				//getting matrix from float
				Assert::ExpectException<std::runtime_error>([&myDatumTypes1]() {myDatumTypes1[1].Get<glm::mat4>(0); });
				//getting glm::vec4 from std::string
				Assert::ExpectException<std::runtime_error>([&myDatumTypes1]() {myDatumTypes1[2].Get<glm::vec4>(0); });
				//getting std::string from glm::vec4
				Assert::ExpectException<std::runtime_error>([&myDatumTypes1]() {myDatumTypes1[3].Get<std::string>(0); });
				//getting glm::mat4 from float
				Assert::ExpectException<std::runtime_error>([&myDatumTypes1]() {myDatumTypes1[4].Get<float>(0); });
				//getting int from RTTI
				Assert::ExpectException<std::runtime_error>([&myDatumTypes1]() {myDatumTypes1[5].Get<int>(0); });

				delete[] myDatumTypes1;

			}
		}
		TEST_METHOD(DatumGetConst)
		{
			//Default Datum
			{
				const Datum myDatum;
				Assert::ExpectException<std::runtime_error>([&myDatum]() {myDatum.Get<int>(0); });
				Assert::ExpectException<std::runtime_error>([&myDatum]() {myDatum.Get<float>(0); });
				Assert::ExpectException<std::runtime_error>([&myDatum]() {myDatum.Get<std::string>(0); });
				Assert::ExpectException<std::runtime_error>([&myDatum]() {myDatum.Get<glm::vec4>(0); });
				Assert::ExpectException<std::runtime_error>([&myDatum]() {myDatum.Get<glm::mat4>(0); });
				Assert::ExpectException<std::runtime_error>([&myDatum]() {myDatum.Get<RTTI*>(0); });
			}

			//Testing on all valid Datum types.
			{
				glm::vec4 myVec(1.0f, 1.0f, 1.0, 1.0);
				glm::mat4 myMat(1.0f);
				Foo myFoo(1);
				RTTI* myRTTI = &myFoo;

				const Datum* myDatumTypes1 = new Datum[static_cast<std::size_t>(DatumTypes::End)]
				{ 1, 1.0f, ""s, myVec, myMat, myRTTI };

				int myInt = myDatumTypes1[0].Get<int>(0);
				Assert::AreEqual(1, myInt);

				float myFloat = myDatumTypes1[1].Get<float>(0);
				Assert::AreEqual(1.0f, myFloat);

				std::string myString = myDatumTypes1[2].Get<std::string>(0);
				Assert::AreEqual(""s, myString);

				glm::vec4 myOtherVec = myDatumTypes1[3].Get<glm::vec4>(0);
				Assert::AreEqual(myVec, myOtherVec);

				glm::mat4 myOtherMat = myDatumTypes1[4].Get<glm::mat4>(0);
				Assert::AreEqual(myMat, myOtherMat);

				RTTI* myOtherRTTI = myDatumTypes1[5].Get<RTTI*>(0);
				Assert::AreEqual(myRTTI, myOtherRTTI);

				delete[] myDatumTypes1;

			}


			//Testing on all Valid Datum types but calling the incorrect Get
			{

				glm::vec4 myVec(1.0f, 1.0f, 1.0, 1.0);
				glm::mat4 myMat(1.0f);
				Foo myFoo(1);
				RTTI* myRTTI = &myFoo;

				const Datum* myDatumTypes1 = new Datum[static_cast<std::size_t>(DatumTypes::End)]
				{ 1, 1.0f, ""s, myVec, myMat, myRTTI };

				//getting pointer from int
				Assert::ExpectException<std::runtime_error>([&myDatumTypes1]() {myDatumTypes1[0].Get<RTTI*>(0); });
				//getting matrix from float
				Assert::ExpectException<std::runtime_error>([&myDatumTypes1]() {myDatumTypes1[1].Get<glm::mat4>(0); });
				//getting glm::vec4 from std::string
				Assert::ExpectException<std::runtime_error>([&myDatumTypes1]() {myDatumTypes1[2].Get<glm::vec4>(0); });
				//getting std::string from glm::vec4
				Assert::ExpectException<std::runtime_error>([&myDatumTypes1]() {myDatumTypes1[3].Get<std::string>(0); });
				//getting glm::mat4 from float
				Assert::ExpectException<std::runtime_error>([&myDatumTypes1]() {myDatumTypes1[4].Get<float>(0); });
				//getting int from RTTI
				Assert::ExpectException<std::runtime_error>([&myDatumTypes1]() {myDatumTypes1[5].Get<int>(0); });

				delete[] myDatumTypes1;

			}

		}
		TEST_METHOD(DatumSetFromString)
		{

			//SetFromString to defaulted datum
			{
				std::string myString = "123";
				Datum myDatum;
				Assert::ExpectException<std::runtime_error>([&myDatum, &myString]() {myDatum.SetFromString(myString, 0); });
			}

			//Accessing Beyond Index
			{
				std::string myString = "123";
				Datum myDatum(111);
				Assert::ExpectException<std::runtime_error>([&myDatum, &myString]() {myDatum.SetFromString(myString, 1); });
			}

			////ToString passing with token that won't match with Datum type
			//{
			//	std::string myString = "123";
			//	Datum myDatum(111.f);
			//	Assert::ExpectException<std::runtime_error>([&myDatum, &myString]() {myDatum.SetFromString(myString, 0); });
			//}

			//On all valid Datum types
			{
				int myCompareInt = 123;
				float myCompareFloat = 123.0f;
				std::string myCompareString = "123";
				glm::vec4 myCompareVec(1.0f, 2.0f, 3.0f, 4.0f);
				glm::mat4 myCompareMat(1.0f, 2.0f, 3.0f, 4.0f, 1.0f, 2.0f, 3.0f, 4.0f, 1.0f, 2.0f, 3.0f, 4.0f, 1.0f, 2.0f, 3.0f, 4.0f);

				std::string myIntString = "123";
				std::string myFloatString = "123";
				std::string myString = "123";
				std::string myVecString = "vec4(1.000000, 2.000000, 3.000000, 4.000000)";
				std::string myMatString = "mat4x4((1.000000,2.000000,3.000000,4.000000), (1.000000,2.000000,3.000000,4.000000), (1.000000,2.000000,3.000000,4.000000), (1.000000,2.000000,3.000000,4.000000))";


				glm::vec4 myOtherVec(1.0f, 2.0f, 3.0f, 4.0f);
				glm::mat4 myOtherMat(1.0f, 2.0f, 3.0f, 4.0f, 1.0f, 2.0f, 3.0f, 4.0f, 1.0f, 2.0f, 3.0f, 4.0f, 1.0f, 2.0f, 3.0f, 4.0f);

				Datum myDatum1(1);
				Datum myDatum2(1.0f);
				Datum myDatum3("Hello"s);
				Datum myDatum4(myOtherVec);
				Datum myDatum5(myOtherMat);

				myDatum1.SetFromString(myIntString, 0);
				myDatum2.SetFromString(myFloatString, 0);
				myDatum3.SetFromString(myString, 0);
				myDatum4.SetFromString(myVecString, 0);
				myDatum5.SetFromString(myMatString, 0);

				Assert::AreEqual(myCompareInt, myDatum1.Get<int>(0));
				Assert::AreEqual(myCompareFloat, myDatum2.Get<float>(0));
				Assert::AreEqual(myCompareString, myDatum3.Get<std::string>(0));
				Assert::AreEqual(myCompareVec, myDatum4.Get<glm::vec4>(0));
				Assert::AreEqual(myCompareMat, myDatum5.Get<glm::mat4>(0));
			}
		}
		TEST_METHOD(DatumToString)
		{
			//Default Datum
			{
				Datum myDatum;
				Assert::ExpectException<std::runtime_error>([&myDatum]() {myDatum.ToString(0); });
			}

			//Testing all valid Datum Types
			{
				int myInt = 123;
				float myFloat = 123.0f;
				std::string myString = "123";
				glm::vec4 myVec(1.0f, 2.0f, 3.0f, 4.0f);
				glm::mat4 myMat(1.0f, 2.0f, 3.0f, 4.0f, 1.0f, 2.0f, 3.0f, 4.0f, 1.0f, 2.0f, 3.0f, 4.0f, 1.0f, 2.0f, 3.0f, 4.0f);
				RTTI* myRTTI;
				Foo myFoo(1);
				myRTTI = &myFoo;

				std::string myCompareIntString = "i(123)";
				std::string myCompareFloatString = "f(123.000000)";
				std::string myCompareString = "s(123)";
				std::string myCompareVecString = "vec4(1.000000, 2.000000, 3.000000, 4.000000)";
				std::string myCompareMatString = "mat4x4((1.000000, 2.000000, 3.000000, 4.000000), (1.000000, 2.000000, 3.000000, 4.000000), (1.000000, 2.000000, 3.000000, 4.000000), (1.000000, 2.000000, 3.000000, 4.000000))";
				std::string myCompareFooString = "Foo";

				Datum myDatum1(myInt);
				Datum myDatum2(myFloat);
				Datum myDatum3(myString);
				Datum myDatum4(myVec);
				Datum myDatum5(myMat);
				Datum myDatum6(myRTTI);

				std::string myDatumIntString;
				myDatumIntString = myDatum1.ToString(0);
				Assert::AreEqual(myCompareIntString, myDatumIntString);

				std::string myDatumFloatString;
				myDatumFloatString = myDatum2.ToString(0);;
				Assert::AreEqual(myCompareFloatString, myDatumFloatString);

				std::string myDatumStringString;
				myDatumStringString = myDatum3.ToString(0);;
				Assert::AreEqual(myCompareString, myDatumStringString);

				std::string myDatumVectorString;
				myDatumVectorString = myDatum4.ToString(0);;
				Assert::AreEqual(myCompareVecString, myDatumVectorString);

				std::string myDatumMatrixString;
				myDatumMatrixString = myDatum5.ToString(0);
				Assert::AreEqual(myCompareMatString, myDatumMatrixString);

				std::string myDatumRTTIString;
				myDatumRTTIString = myDatum6.ToString(0);
				Assert::AreEqual(myCompareFooString, myDatumRTTIString);

			}
		}
		TEST_METHOD(DatumPushBack)
		{
			//Default Datum
			{

				glm::vec4 myVec(1.0f, 1.0f, 1.0f, 1.0f);
				glm::mat4 myMat(1.0f);
				Foo myFoo(1);
				RTTI* myRTTI = &myFoo;

				Datum myDatum1;
				myDatum1.PushBack(1);
				Assert::AreEqual(DatumTypes::Integer, myDatum1.Type());
				Assert::AreEqual(static_cast<size_t>(1), myDatum1.Size());
				Assert::AreEqual(static_cast<size_t>(1), myDatum1.Capacity());
				myDatum1.PushBack(1);
				myDatum1.PushBack(1);
				Assert::AreEqual(static_cast<size_t>(3), myDatum1.Size());
				Assert::AreEqual(static_cast<size_t>(4), myDatum1.Capacity());

				Datum myDatum2;
				myDatum2.PushBack(1.0f);
				Assert::AreEqual(DatumTypes::Float, myDatum2.Type());
				Assert::AreEqual(static_cast<size_t>(1), myDatum2.Size());
				Assert::AreEqual(static_cast<size_t>(1), myDatum2.Capacity());
				myDatum2.PushBack(1.0f);
				myDatum2.PushBack(1.0f);
				Assert::AreEqual(static_cast<size_t>(3), myDatum2.Size());
				Assert::AreEqual(static_cast<size_t>(4), myDatum2.Capacity());

				Datum myDatum3;
				myDatum3.PushBack(""s);
				Assert::AreEqual(DatumTypes::String, myDatum3.Type());
				Assert::AreEqual(static_cast<size_t>(1), myDatum3.Size());
				Assert::AreEqual(static_cast<size_t>(1), myDatum3.Capacity());
				myDatum3.PushBack(""s);
				myDatum3.PushBack(""s);
				Assert::AreEqual(static_cast<size_t>(3), myDatum3.Size());
				Assert::AreEqual(static_cast<size_t>(4), myDatum3.Capacity());

				Datum myDatum4;
				myDatum4.PushBack(myVec);
				Assert::AreEqual(DatumTypes::Vector4, myDatum4.Type());
				Assert::AreEqual(static_cast<size_t>(1), myDatum4.Size());
				Assert::AreEqual(static_cast<size_t>(1), myDatum4.Capacity());
				myDatum4.PushBack(myVec);
				myDatum4.PushBack(myVec);
				Assert::AreEqual(static_cast<size_t>(3), myDatum4.Size());
				Assert::AreEqual(static_cast<size_t>(4), myDatum4.Capacity());

				Datum myDatum5;
				myDatum5.PushBack(myMat);
				Assert::AreEqual(DatumTypes::Matrix, myDatum5.Type());
				Assert::AreEqual(static_cast<size_t>(1), myDatum5.Size());
				Assert::AreEqual(static_cast<size_t>(1), myDatum5.Capacity());
				myDatum5.PushBack(myMat);
				myDatum5.PushBack(myMat);
				Assert::AreEqual(static_cast<size_t>(3), myDatum5.Size());
				Assert::AreEqual(static_cast<size_t>(4), myDatum5.Capacity());


				Datum myDatum6;
				myDatum6.PushBack(myRTTI);
				Assert::AreEqual(DatumTypes::Pointer, myDatum6.Type());
				Assert::AreEqual(static_cast<size_t>(1), myDatum6.Size());
				Assert::AreEqual(static_cast<size_t>(1), myDatum6.Capacity());
				myDatum6.PushBack(myRTTI);
				myDatum6.PushBack(myRTTI);
				Assert::AreEqual(DatumTypes::Pointer, myDatum6.Type());
				Assert::AreEqual(static_cast<size_t>(3), myDatum6.Size());
				Assert::AreEqual(static_cast<size_t>(4), myDatum6.Capacity());
			}

			//Pushing Back on External Datum
			{

				glm::vec4 myPushBackVec(1.0f, 1.0f, 1.0f, 1.0f);
				glm::mat4 myPushBackMat(1.0f);
				Foo myPushBackFoo(1);
				RTTI* myPushBackRTTI = &myPushBackFoo;

				const int SIZE = 3;

				Foo myFoo1(1);
				Foo myFoo2(2);
				Foo myFoo3(3);
				RTTI* myRTTI1 = &myFoo1;
				RTTI* myRTTI2 = &myFoo2;
				RTTI* myRTTI3 = &myFoo3;


				int myIntArray[SIZE] = { 0,1,2 };
				float myFloatArray[SIZE] = { 0.f,1.f,2.f };
				std::string myStringArray[SIZE] = { "","","" };
				glm::vec4 myVecArray[SIZE] = { { 1.0f, 1.0f, 1.0f, 1.0f}, { 1.0f, 1.0f, 1.0f, 1.0f}, { 1.0f, 1.0f, 1.0f, 1.0f} };
				glm::mat4 myMatArray[SIZE] = { {1.0f, 1.0f, 1.0f, 1.0f,1.0f, 1.0f, 1.0f, 1.0f,1.0f, 1.0f, 1.0f, 1.0f,1.0f, 1.0f, 1.0f, 1.0f},
												{1.0f, 1.0f, 1.0f, 1.0f,1.0f, 1.0f, 1.0f, 1.0f,1.0f, 1.0f, 1.0f, 1.0f,1.0f, 1.0f, 1.0f, 1.0f},
												{1.0f, 1.0f, 1.0f, 1.0f,1.0f, 1.0f, 1.0f, 1.0f,1.0f, 1.0f, 1.0f, 1.0f,1.0f, 1.0f, 1.0f, 1.0f} };

				RTTI* myRTTIArray[SIZE] = { myRTTI1, myRTTI2, myRTTI3 };

				Datum myDatum1;
				myDatum1.SetStorage(myIntArray, SIZE);
				Assert::ExpectException<std::runtime_error>([&myDatum1]() {myDatum1.PushBack(1); });

				Datum myDatum2;
				myDatum2.SetStorage(myFloatArray, SIZE);
				Assert::ExpectException<std::runtime_error>([&myDatum2]() {myDatum2.PushBack(1.0f); });

				Datum myDatum3;
				myDatum3.SetStorage(myStringArray, SIZE);
				Assert::ExpectException<std::runtime_error>([&myDatum3]() {myDatum3.PushBack(""s); });

				Datum myDatum4;
				myDatum4.SetStorage(myVecArray, SIZE);
				Assert::ExpectException<std::runtime_error>([&myDatum4, &myPushBackVec]() {myDatum4.PushBack(myPushBackVec); });

				Datum myDatum5;
				myDatum5.SetStorage(myMatArray, SIZE);
				Assert::ExpectException<std::runtime_error>([&myDatum5, &myPushBackMat]() {myDatum5.PushBack(myPushBackMat); });

				Datum myDatum6;
				myDatum6.SetStorage(myRTTIArray, SIZE);
				Assert::ExpectException<std::runtime_error>([&myDatum6, &myPushBackRTTI]() {myDatum6.PushBack(myPushBackRTTI); });

			}

			//PushBack type that is not of the type Datum is set to
			{
				glm::vec4 myVec(1.0f, 1.0f, 1.0f, 1.0f);
				glm::mat4 myMat(1.0f);
				Foo myFoo(1);
				RTTI* myRTTI = &myFoo;

				Datum myDatum1;
				myDatum1.PushBack(1);
				Assert::ExpectException<std::runtime_error>([&myDatum1, &myRTTI]() {myDatum1.PushBack(myRTTI); }); //Pushing a RTTI Pointer to Datum of type Integer


				Datum myDatum2;
				myDatum2.PushBack(1.0f);
				Assert::ExpectException<std::runtime_error>([&myDatum2]() {myDatum2.PushBack(1); }); //Pushing a Integer to Datum of type Float

				Datum myDatum3;
				myDatum3.PushBack(""s);
				Assert::ExpectException<std::runtime_error>([&myDatum3]() {myDatum3.PushBack(1.0f); }); //Pushing a Float to Datum of type String


				Datum myDatum4;
				myDatum4.PushBack(myVec);
				Assert::ExpectException<std::runtime_error>([&myDatum4]() {myDatum4.PushBack(""s); }); //Pushing a String to Datum of type Vector

				Datum myDatum5;
				myDatum5.PushBack(myMat);
				Assert::ExpectException<std::runtime_error>([&myDatum5, &myVec]() {myDatum5.PushBack(myVec); }); //Pushing a Vector to Datum of type Matrix


				Datum myDatum6;
				myDatum6.PushBack(myRTTI);
				Assert::ExpectException<std::runtime_error>([&myDatum6, &myMat]() {myDatum6.PushBack(myMat); }); //Pushing an Matrix to Datum of type pointer

			}
		}
		TEST_METHOD(DatumPopBack)
		{
			//Test on default constructed Datum
			//Should fail silently. Nothing should happen.
			{
				Datum myDatum;
				myDatum.PopBack();

				Assert::AreEqual(DatumTypes::Unknown, myDatum.Type());
				Assert::AreEqual(static_cast<size_t>(0), myDatum.Size());
				Assert::AreEqual(static_cast<size_t>(0), myDatum.Capacity());

			}

			//Test on all valid Datum Types
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

				Scope* myScope1 = new Scope;
				Scope* myScope2 = new Scope;
				Scope* myScope3 = new Scope;

				Datum myIntDatum{ 1,2,3 };
				Datum myFloatDatum{ 1.0f,2.0f,3.0f };
				Datum myStringDatum{ "a"s,"b"s,"c"s };
				Datum myVectorDatum{ myVec1,myVec2,myVec3 };
				Datum myMatrixDatum{ myMat1,myMat2,myMat3 };
				Datum myRTTIDatum{ myRTTI1,myRTTI2,myRTTI3 };
				Datum myScopeDatum{ myScope1, myScope2, myScope3 };

				myIntDatum.PopBack();
				Assert::AreEqual(static_cast<std::size_t>(2), myIntDatum.Size());
				std::pair<int32_t*, size_t> myPairI = myIntDatum.Find(3);
				Assert::IsNull(myPairI.first);

				myFloatDatum.PopBack();
				Assert::AreEqual(static_cast<std::size_t>(2), myFloatDatum.Size());
				std::pair<float*, size_t> myPairF = myFloatDatum.Find(3.0f);
				Assert::IsNull(myPairF.first);

				myStringDatum.PopBack();
				Assert::AreEqual(static_cast<std::size_t>(2), myStringDatum.Size());
				std::pair<std::string*, size_t> myPairS = myStringDatum.Find("c"s);
				Assert::IsNull(myPairS.first);

				myVectorDatum.PopBack();
				Assert::AreEqual(static_cast<std::size_t>(2), myVectorDatum.Size());
				std::pair<glm::vec4*, size_t> myPairV = myVectorDatum.Find(myVec3);
				Assert::IsNull(myPairV.first);

				myMatrixDatum.PopBack();
				Assert::AreEqual(static_cast<std::size_t>(2), myMatrixDatum.Size());
				std::pair<glm::mat4*, size_t> myPairM = myMatrixDatum.Find(myMat3);
				Assert::IsNull(myPairM.first);

				myRTTIDatum.PopBack();
				Assert::AreEqual(static_cast<std::size_t>(2), myRTTIDatum.Size());
				std::pair<RTTI*, size_t> myPairR = myRTTIDatum.Find(myRTTI3);
				Assert::IsNull(myPairR.first);

				myScopeDatum.PopBack();
				Assert::AreEqual(static_cast<std::size_t>(2), myScopeDatum.Size());
				std::pair<Scope*, size_t> myPairT = myScopeDatum.Find(*myScope3);
				Assert::IsNull(myPairT.first);

				delete myScope1;
				delete myScope2;
				delete myScope3;
			}
		}
		TEST_METHOD(DatumFront)
		{
			//Defaulted Datum
			{
				Datum myDatum;
				Assert::ExpectException<std::runtime_error>([&myDatum]() {myDatum.Front<int>(); });
				Assert::ExpectException<std::runtime_error>([&myDatum]() {myDatum.Front<float>(); });
				Assert::ExpectException<std::runtime_error>([&myDatum]() {myDatum.Front<std::string>(); });
				Assert::ExpectException<std::runtime_error>([&myDatum]() {myDatum.Front<glm::vec4>(); });
				Assert::ExpectException<std::runtime_error>([&myDatum]() {myDatum.Front<glm::mat4>(); });
				Assert::ExpectException<std::runtime_error>([&myDatum]() {myDatum.Front<FieaGameEngine::RTTI*>(); });
			}

			//Datum with set type but no items. Size == 0. exception
			//Also attempting to use Front and getting a different type. exception
			{
				Datum myDatum1;
				myDatum1.SetType(DatumTypes::Integer);
				Assert::ExpectException<std::runtime_error>([&myDatum1]() {myDatum1.Front<int>(); });
				Assert::ExpectException<std::runtime_error>([&myDatum1]() {myDatum1.Front<FieaGameEngine::RTTI*>(); });

				Datum myDatum2;
				myDatum2.SetType(DatumTypes::Float);
				Assert::ExpectException<std::runtime_error>([&myDatum2]() {myDatum2.Front<float>(); });
				Assert::ExpectException<std::runtime_error>([&myDatum2]() {myDatum2.Front<int>(); });

				Datum myDatum3;
				myDatum3.SetType(DatumTypes::String);
				Assert::ExpectException<std::runtime_error>([&myDatum3]() {myDatum3.Front<std::string>(); });
				Assert::ExpectException<std::runtime_error>([&myDatum3]() {myDatum3.Front<float>(); });

				Datum myDatum4;
				myDatum4.SetType(DatumTypes::Vector4);
				Assert::ExpectException<std::runtime_error>([&myDatum4]() {myDatum4.Front<glm::vec4>(); });
				Assert::ExpectException<std::runtime_error>([&myDatum4]() {myDatum4.Front<std::string>(); });

				Datum myDatum5;
				myDatum5.SetType(DatumTypes::Matrix);
				Assert::ExpectException<std::runtime_error>([&myDatum5]() {myDatum5.Front<glm::mat4>(); });
				Assert::ExpectException<std::runtime_error>([&myDatum5]() {myDatum5.Front<glm::vec4>(); });

				Datum myDatum6;
				myDatum6.SetType(DatumTypes::Pointer);
				Assert::ExpectException<std::runtime_error>([&myDatum6]() {myDatum6.Front<FieaGameEngine::RTTI*>(); });
				Assert::ExpectException<std::runtime_error>([&myDatum6]() {myDatum6.Front<glm::mat4>(); });
			}

			//Calling front on all valid Datum types
			{
				const glm::vec4 myVec(1.0f, 1.0f, 1.0, 1.0);
				const glm::mat4 myMat(1.0f);
				Foo myFoo(1);
				RTTI* myRTTI = &myFoo;
				const Datum* myDatumTypes = new Datum[static_cast<std::size_t>(DatumTypes::End)]
				{ 1, 1.0f, ""s, myVec, myMat, myRTTI };

				Assert::AreEqual(static_cast<int32_t>(1), myDatumTypes[0].Front<int>());
				Assert::AreEqual(static_cast<float>(1.0f), myDatumTypes[1].Front<float>());
				Assert::AreEqual(""s, myDatumTypes[2].Front<std::string>());
				Assert::AreEqual(myVec, myDatumTypes[3].Front<glm::vec4>());
				Assert::AreEqual(myMat, myDatumTypes[4].Front<glm::mat4>());
				Assert::AreEqual(myRTTI, myDatumTypes[5].Front<FieaGameEngine::RTTI*>());
				
				delete[] myDatumTypes;
			}
		}
		TEST_METHOD(DatumFrontConst)
		{
			//Defaulted Datum
			{
				const Datum myDatum;
				Assert::ExpectException<std::runtime_error>([&myDatum]() {myDatum.Back<int>(); });
				Assert::ExpectException<std::runtime_error>([&myDatum]() {myDatum.Back<float>(); });
				Assert::ExpectException<std::runtime_error>([&myDatum]() {myDatum.Back<std::string>(); });
				Assert::ExpectException<std::runtime_error>([&myDatum]() {myDatum.Back<glm::vec4>(); });
				Assert::ExpectException<std::runtime_error>([&myDatum]() {myDatum.Back<glm::mat4>(); });
				Assert::ExpectException<std::runtime_error>([&myDatum]() {myDatum.Back<FieaGameEngine::RTTI*>(); });
			}

			//attempting to use Back and getting a different type. exception
			{	
				glm::vec4 myVec(1.0f, 1.0f, 1.0, 1.0);
				glm::mat4 myMat(1.0f);
				Foo myFoo(1);
				RTTI* myRTTI = &myFoo;

				const Datum myDatum1(1);
				Assert::ExpectException<std::runtime_error>([&myDatum1]() {myDatum1.Back<FieaGameEngine::RTTI*>(); });

				const Datum myDatum2(1.0f);
				Assert::ExpectException<std::runtime_error>([&myDatum2]() {myDatum2.Back<int>(); });

				const Datum myDatum3(""s);
				Assert::ExpectException<std::runtime_error>([&myDatum3]() {myDatum3.Back<float>(); });

				const Datum myDatum4(myVec);
				Assert::ExpectException<std::runtime_error>([&myDatum4]() {myDatum4.Back<std::string>(); });

				const Datum myDatum5(myMat);
				Assert::ExpectException<std::runtime_error>([&myDatum5]() {myDatum5.Back<glm::vec4>(); });

				const Datum myDatum6(myRTTI);
				Assert::ExpectException<std::runtime_error>([&myDatum6]() {myDatum6.Back<glm::mat4>(); });
			}

			//Calling Back on all valid Datum types
			{
				glm::vec4 myVec(1.0f, 1.0f, 1.0, 1.0);
				glm::mat4 myMat(1.0f);
				Foo myFoo(1);
				RTTI* myRTTI = &myFoo;
				const Datum* myDatumTypes = new Datum[static_cast<std::size_t>(DatumTypes::End)]
				{ 1, 1.0f, ""s, myVec, myMat, myRTTI };

				Assert::AreEqual(static_cast<int32_t>(1), myDatumTypes[0].Back<int>());
				Assert::AreEqual(static_cast<float>(1.0f), myDatumTypes[1].Back<float>());
				Assert::AreEqual(""s, myDatumTypes[2].Back<std::string>());
				Assert::AreEqual(myVec, myDatumTypes[3].Back<glm::vec4>());
				Assert::AreEqual(myMat, myDatumTypes[4].Back<glm::mat4>());
				Assert::AreEqual(myRTTI, myDatumTypes[5].Back<FieaGameEngine::RTTI*>());

				delete[] myDatumTypes;
			}
		}
		TEST_METHOD(DatumBack)
		{
			//Defaulted Datum
			{
				Datum myDatum;
				Assert::ExpectException<std::runtime_error>([&myDatum]() {myDatum.Back<int>(); });
				Assert::ExpectException<std::runtime_error>([&myDatum]() {myDatum.Back<float>(); });
				Assert::ExpectException<std::runtime_error>([&myDatum]() {myDatum.Back<std::string>(); });
				Assert::ExpectException<std::runtime_error>([&myDatum]() {myDatum.Back<glm::vec4>(); });
				Assert::ExpectException<std::runtime_error>([&myDatum]() {myDatum.Back<glm::mat4>(); });
				Assert::ExpectException<std::runtime_error>([&myDatum]() {myDatum.Back<FieaGameEngine::RTTI*>(); });
			}

			//Datum with set type but no items. Size == 0. exception
			//Also attempting to use Back and getting a different type. exception
			{
				Datum myDatum1;
				myDatum1.SetType(DatumTypes::Integer);
				Assert::ExpectException<std::runtime_error>([&myDatum1]() {myDatum1.Back<int>(); });
				Assert::ExpectException<std::runtime_error>([&myDatum1]() {myDatum1.Back<FieaGameEngine::RTTI*>(); });

				Datum myDatum2;
				myDatum2.SetType(DatumTypes::Float);
				Assert::ExpectException<std::runtime_error>([&myDatum2]() {myDatum2.Back<float>(); });
				Assert::ExpectException<std::runtime_error>([&myDatum2]() {myDatum2.Back<int>(); });

				Datum myDatum3;
				myDatum3.SetType(DatumTypes::String);
				Assert::ExpectException<std::runtime_error>([&myDatum3]() {myDatum3.Back<std::string>(); });
				Assert::ExpectException<std::runtime_error>([&myDatum3]() {myDatum3.Back<float>(); });

				Datum myDatum4;
				myDatum4.SetType(DatumTypes::Vector4);
				Assert::ExpectException<std::runtime_error>([&myDatum4]() {myDatum4.Back<glm::vec4>(); });
				Assert::ExpectException<std::runtime_error>([&myDatum4]() {myDatum4.Back<std::string>(); });

				Datum myDatum5;
				myDatum5.SetType(DatumTypes::Matrix);
				Assert::ExpectException<std::runtime_error>([&myDatum5]() {myDatum5.Back<glm::mat4>(); });
				Assert::ExpectException<std::runtime_error>([&myDatum5]() {myDatum5.Back<glm::vec4>(); });

				Datum myDatum6;
				myDatum6.SetType(DatumTypes::Pointer);
				Assert::ExpectException<std::runtime_error>([&myDatum6]() {myDatum6.Back<FieaGameEngine::RTTI*>(); });
				Assert::ExpectException<std::runtime_error>([&myDatum6]() {myDatum6.Back<glm::mat4>(); });
			}

			//Calling Back on all valid Datum types
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

				Scope* myScope1 = new Scope;
				Scope* myScope2 = new Scope;
				Scope* myScope3 = new Scope;

				Datum myIntDatum{ 1,2,3 };
				Datum myFloatDatum{ 1.0f,2.0f,3.0f };
				Datum myStringDatum{ "a"s,"b"s,"c"s };
				Datum myVectorDatum{ myVec1,myVec2,myVec3 };
				Datum myMatrixDatum{ myMat1,myMat2,myMat3 };
				Datum myRTTIDatum{ myRTTI1,myRTTI2,myRTTI3 };
				Datum myScopeDatum{ myScope1, myScope2, myScope3 };

				Assert::AreEqual(myIntDatum.Get<int32_t>(2), myIntDatum.Back<int32_t>());
				Assert::AreEqual(myFloatDatum.Get<float>(2), myFloatDatum.Back<float>());
				Assert::AreEqual(myStringDatum.Get<std::string>(2), myStringDatum.Back<std::string> ());
				Assert::AreEqual(myVectorDatum.Get<glm::vec4>(2), myVectorDatum.Back<glm::vec4>());
				Assert::AreEqual(myMatrixDatum.Get<glm::mat4>(2), myMatrixDatum.Back<glm::mat4>());
				Assert::AreEqual(myRTTIDatum.Get<RTTI*>(2), myRTTIDatum.Back<RTTI*>());
				Assert::AreEqual(myScopeDatum.Get<Scope>(2), myScopeDatum.Back<Scope>());

				delete myScope1;
				delete myScope2;
				delete myScope3;

			}
		}
		TEST_METHOD(DatumBackConst)
		{
			//Defaulted Datum
			{
				const Datum myDatum;
				Assert::ExpectException<std::runtime_error>([&myDatum]() {myDatum.Back<int>(); });
				Assert::ExpectException<std::runtime_error>([&myDatum]() {myDatum.Back<float>(); });
				Assert::ExpectException<std::runtime_error>([&myDatum]() {myDatum.Back<std::string>(); });
				Assert::ExpectException<std::runtime_error>([&myDatum]() {myDatum.Back<glm::vec4>(); });
				Assert::ExpectException<std::runtime_error>([&myDatum]() {myDatum.Back<glm::mat4>(); });
				Assert::ExpectException<std::runtime_error>([&myDatum]() {myDatum.Back<FieaGameEngine::RTTI*>(); });
			}

			//attempting to use Back and getting a different type. exception
			{
				glm::vec4 myVec(1.0f, 1.0f, 1.0, 1.0);
				glm::mat4 myMat(1.0f);
				Foo myFoo(1);
				RTTI* myRTTI = &myFoo;

				const Datum myDatum1(1);
				Assert::ExpectException<std::runtime_error>([&myDatum1]() {myDatum1.Back<FieaGameEngine::RTTI*>(); });

				const Datum myDatum2(1.0f);
				Assert::ExpectException<std::runtime_error>([&myDatum2]() {myDatum2.Back<int>(); });

				const Datum myDatum3(""s);
				Assert::ExpectException<std::runtime_error>([&myDatum3]() {myDatum3.Back<float>(); });

				const Datum myDatum4(myVec);
				Assert::ExpectException<std::runtime_error>([&myDatum4]() {myDatum4.Back<std::string>(); });

				const Datum myDatum5(myMat);
				Assert::ExpectException<std::runtime_error>([&myDatum5]() {myDatum5.Back<glm::vec4>(); });

				const Datum myDatum6(myRTTI);
				Assert::ExpectException<std::runtime_error>([&myDatum6]() {myDatum6.Back<glm::mat4>(); });
			}

			//Calling Back on all valid Datum types
			{
				glm::vec4 myVec(1.0f, 1.0f, 1.0, 1.0);
				glm::mat4 myMat(1.0f);
				Foo myFoo(1);
				RTTI* myRTTI = &myFoo;
				const Datum* myDatumTypes = new Datum[static_cast<std::size_t>(DatumTypes::End)]
				{ 1, 1.0f, ""s, myVec, myMat, myRTTI };

				Assert::AreEqual(static_cast<int32_t>(1), myDatumTypes[0].Back<int>());
				Assert::AreEqual(static_cast<float>(1.0f), myDatumTypes[1].Back<float>());
				Assert::AreEqual(""s, myDatumTypes[2].Back<std::string>());
				Assert::AreEqual(myVec, myDatumTypes[3].Back<glm::vec4>());
				Assert::AreEqual(myMat, myDatumTypes[4].Back<glm::mat4>());
				Assert::AreEqual(myRTTI, myDatumTypes[5].Back<FieaGameEngine::RTTI*>());

				delete[] myDatumTypes;
			}
		}
		TEST_METHOD(DatumRemove)
		{
			//Default Datum
			{
				glm::vec4 myVec1(1.0f, 1.0f, 1.0f, 1.0f);
				glm::mat4 myMat1(1.0f);

				glm::vec4 myVec2(2.0f, 2.0f, 2.0f, 2.0f);
				glm::mat4 myMat2(2.0f);

				glm::vec4 myVec3(3.0f, 3.0f, 3.0f, 3.0f);
				glm::mat4 myMat3(3.0f);

				Foo myFoo1(1);
				RTTI* myRTTI1 = &myFoo1;

				Foo myFoo2(2);
				RTTI* myRTTI2 = &myFoo2;

				Foo myFoo3(3);
				RTTI* myRTTI3 = &myFoo3;

				{
					Datum myDatum1;
					myDatum1.PushBack(1);
					myDatum1.PushBack(2);
					myDatum1.PushBack(3);
					Assert::AreEqual(static_cast<size_t>(3), myDatum1.Size());
					Assert::AreEqual(static_cast<size_t>(4), myDatum1.Capacity());

					myDatum1.Remove(2);
					Assert::AreEqual(static_cast<size_t>(2), myDatum1.Size());

					auto [datumRef, isFound] = myDatum1.Find(2);
					Assert::IsFalse(isFound);
				}

				{
					Datum myDatum;
					myDatum.PushBack(1.0f);
					myDatum.PushBack(2.0f);
					myDatum.PushBack(3.0f);
					Assert::AreEqual(static_cast<size_t>(3), myDatum.Size());
					Assert::AreEqual(static_cast<size_t>(4), myDatum.Capacity());

					myDatum.Remove(2.0f);
					Assert::AreEqual(static_cast<size_t>(2), myDatum.Size());


					auto [datumRef, isFound] = myDatum.Find(2.0f);
					Assert::IsFalse(isFound);
				}

				{
					Datum myDatum;
					myDatum.PushBack("a"s);
					myDatum.PushBack("b"s);
					myDatum.PushBack("c"s);
					Assert::AreEqual(static_cast<size_t>(3), myDatum.Size());
					Assert::AreEqual(static_cast<size_t>(4), myDatum.Capacity());

					myDatum.Remove("b"s);
					Assert::AreEqual(static_cast<size_t>(2), myDatum.Size());


					auto [datumRef, isFound] = myDatum.Find("b"s);
					Assert::IsFalse(isFound);
				}
				
				{
					Datum myDatum;
					myDatum.PushBack(myVec1);
					myDatum.PushBack(myVec2);
					myDatum.PushBack(myVec3);
					Assert::AreEqual(static_cast<size_t>(3), myDatum.Size());
					Assert::AreEqual(static_cast<size_t>(4), myDatum.Capacity());

					myDatum.Remove(myVec2);
					Assert::AreEqual(static_cast<size_t>(2), myDatum.Size());


					auto [datumRef, isFound] = myDatum.Find(myVec2);
					Assert::IsFalse(isFound);
				}

				{
					Datum myDatum;
					myDatum.PushBack(myMat1);
					myDatum.PushBack(myMat2);
					myDatum.PushBack(myMat3);
					Assert::AreEqual(static_cast<size_t>(3), myDatum.Size());
					Assert::AreEqual(static_cast<size_t>(4), myDatum.Capacity());

					myDatum.Remove(myMat2);
					Assert::AreEqual(static_cast<size_t>(2), myDatum.Size());

					
					auto [DatumRef, isFound] = myDatum.Find(myMat2);
					Assert::IsFalse(isFound);
				}
				

				{

					Datum myDatum;
					myDatum.PushBack(myRTTI1);
					myDatum.PushBack(myRTTI2);
					myDatum.PushBack(myRTTI3);
					Assert::AreEqual(static_cast<size_t>(3), myDatum.Size());
					Assert::AreEqual(static_cast<size_t>(4), myDatum.Capacity());

					myDatum.Remove(myRTTI2);
					Assert::AreEqual(static_cast<size_t>(2), myDatum.Size());


					auto [datumRef, isFound] = myDatum.Find(myRTTI2);
					Assert::IsFalse(isFound);
				}				

				/*{

					Scope myScope;
					myScope.AppendScope("firstScope");
					myScope.AppendScope("firstScope");
					myScope.AppendScope("firstScope");



					Datum* myDatum = myScope.Find("firstScope");


					Assert::AreEqual(static_cast<size_t>(3), myDatum->Size());
					Assert::AreEqual(static_cast<size_t>(4), myDatum->Capacity());

					myDatum->RemoveAt(1);
					Assert::AreEqual(static_cast<size_t>(2), myDatum->Size());

				}*/

			}


		}
		TEST_METHOD(DatumRemoveAt)
		{
			//Accessing beyond size of array
			{
				glm::vec4 myPushBackVec(1.0f, 1.0f, 1.0f, 1.0f);
				glm::mat4 myPushBackMat(1.0f);
				Foo myPushBackFoo(1);
				RTTI* myPushBackRTTI = &myPushBackFoo;

				const int SIZE = 3;

				Foo myFoo1(1);
				Foo myFoo2(2);
				Foo myFoo3(3);
				RTTI* myRTTI1 = &myFoo1;
				RTTI* myRTTI2 = &myFoo2;
				RTTI* myRTTI3 = &myFoo3;

				int myIntArray[SIZE] = { 0,1,2 };
				float myFloatArray[SIZE] = { 0.f,1.f,2.f };
				std::string myStringArray[SIZE] = { "","","" };
				glm::vec4 myVecArray[SIZE] = { { 1.0f, 1.0f, 1.0f, 1.0f}, { 1.0f, 1.0f, 1.0f, 1.0f}, { 1.0f, 1.0f, 1.0f, 1.0f} };
				glm::mat4 myMatArray[SIZE] = { {1.0f, 1.0f, 1.0f, 1.0f,1.0f, 1.0f, 1.0f, 1.0f,1.0f, 1.0f, 1.0f, 1.0f,1.0f, 1.0f, 1.0f, 1.0f},
												{1.0f, 1.0f, 1.0f, 1.0f,1.0f, 1.0f, 1.0f, 1.0f,1.0f, 1.0f, 1.0f, 1.0f,1.0f, 1.0f, 1.0f, 1.0f},
												{1.0f, 1.0f, 1.0f, 1.0f,1.0f, 1.0f, 1.0f, 1.0f,1.0f, 1.0f, 1.0f, 1.0f,1.0f, 1.0f, 1.0f, 1.0f} };

				RTTI* myRTTIArray[SIZE] = { myRTTI1, myRTTI2, myRTTI3 };

				Datum myDatum1;
				myDatum1.SetStorage(myIntArray, SIZE);
				Assert::ExpectException<std::runtime_error>([&myDatum1]() {myDatum1.RemoveAt(3); });

				Datum myDatum2;
				myDatum2.SetStorage(myFloatArray, SIZE);
				Assert::ExpectException<std::runtime_error>([&myDatum2]() {myDatum2.RemoveAt(3); });

				Datum myDatum3;
				myDatum3.SetStorage(myStringArray, SIZE);
				Assert::ExpectException<std::runtime_error>([&myDatum3]() {myDatum3.RemoveAt(3); });

				Datum myDatum4;
				myDatum4.SetStorage(myVecArray, SIZE);
				Assert::ExpectException<std::runtime_error>([&myDatum4, &myPushBackVec]() {myDatum4.RemoveAt(3); });

				Datum myDatum5;
				myDatum5.SetStorage(myMatArray, SIZE);
				Assert::ExpectException<std::runtime_error>([&myDatum5, &myPushBackMat]() {myDatum5.RemoveAt(3); });

				Datum myDatum6;
				myDatum6.SetStorage(myRTTIArray, SIZE);
				Assert::ExpectException<std::runtime_error>([&myDatum6, &myPushBackRTTI]() {myDatum6.RemoveAt(3); });

			}

			//External Datum
			{

				glm::vec4 myPushBackVec(1.0f, 1.0f, 1.0f, 1.0f);
				glm::mat4 myPushBackMat(1.0f);
				Foo myPushBackFoo(1);
				RTTI* myPushBackRTTI = &myPushBackFoo;

				const int SIZE = 3;

				Foo myFoo1(1);
				Foo myFoo2(2);
				Foo myFoo3(3);
				RTTI* myRTTI1 = &myFoo1;
				RTTI* myRTTI2 = &myFoo2;
				RTTI* myRTTI3 = &myFoo3;

				int myIntArray[SIZE] = { 0,1,2 };
				float myFloatArray[SIZE] = { 0.f,1.f,2.f };
				std::string myStringArray[SIZE] = { "","","" };
				glm::vec4 myVecArray[SIZE] = { { 1.0f, 1.0f, 1.0f, 1.0f}, { 1.0f, 1.0f, 1.0f, 1.0f}, { 1.0f, 1.0f, 1.0f, 1.0f} };
				glm::mat4 myMatArray[SIZE] = { {1.0f, 1.0f, 1.0f, 1.0f,1.0f, 1.0f, 1.0f, 1.0f,1.0f, 1.0f, 1.0f, 1.0f,1.0f, 1.0f, 1.0f, 1.0f},
												{1.0f, 1.0f, 1.0f, 1.0f,1.0f, 1.0f, 1.0f, 1.0f,1.0f, 1.0f, 1.0f, 1.0f,1.0f, 1.0f, 1.0f, 1.0f},
												{1.0f, 1.0f, 1.0f, 1.0f,1.0f, 1.0f, 1.0f, 1.0f,1.0f, 1.0f, 1.0f, 1.0f,1.0f, 1.0f, 1.0f, 1.0f} };

				RTTI* myRTTIArray[SIZE] = { myRTTI1, myRTTI2, myRTTI3 };

				Datum myDatum1;
				myDatum1.SetStorage(myIntArray, SIZE);
				Assert::ExpectException<std::runtime_error>([&myDatum1]() {myDatum1.RemoveAt(1); });

				Datum myDatum2;
				myDatum2.SetStorage(myFloatArray, SIZE);
				Assert::ExpectException<std::runtime_error>([&myDatum2]() {myDatum2.RemoveAt(1); });

				Datum myDatum3;
				myDatum3.SetStorage(myStringArray, SIZE);
				Assert::ExpectException<std::runtime_error>([&myDatum3]() {myDatum3.RemoveAt(1); });

				Datum myDatum4;
				myDatum4.SetStorage(myVecArray, SIZE);
				Assert::ExpectException<std::runtime_error>([&myDatum4, &myPushBackVec]() {myDatum4.RemoveAt(1); });

				Datum myDatum5;
				myDatum5.SetStorage(myMatArray, SIZE);
				Assert::ExpectException<std::runtime_error>([&myDatum5, &myPushBackMat]() {myDatum5.RemoveAt(1); });

				Datum myDatum6;
				myDatum6.SetStorage(myRTTIArray, SIZE);
				Assert::ExpectException<std::runtime_error>([&myDatum6, &myPushBackRTTI]() {myDatum6.RemoveAt(1); });

			}

			//Internal Datum
			{
				glm::vec4 myVec1(1.0f, 1.0f, 1.0f, 1.0f);
				glm::mat4 myMat1(1.0f);

				glm::vec4 myVec2(2.0f, 2.0f, 2.0f, 2.0f);
				glm::mat4 myMat2(2.0f);

				glm::vec4 myVec3(3.0f, 3.0f, 3.0f, 3.0f);
				glm::mat4 myMat3(3.0f);

				Foo myFoo1(1);
				RTTI* myRTTI1 = &myFoo1;

				Foo myFoo2(2);
				RTTI* myRTTI2 = &myFoo2;

				Foo myFoo3(3);
				RTTI* myRTTI3 = &myFoo3;

				Datum myDatum1;
				myDatum1.PushBack(1);
				myDatum1.PushBack(2);
				myDatum1.PushBack(3);
				Assert::AreEqual(static_cast<size_t>(3), myDatum1.Size());
				Assert::AreEqual(static_cast<size_t>(4), myDatum1.Capacity());

				{
					auto [datumRef, isFound] = myDatum1.Find(2);
					Assert::IsTrue(isFound);
				}

				myDatum1.RemoveAt(1);

				Assert::AreEqual(static_cast<size_t>(2), myDatum1.Size());

				{
					auto [datumRef, isFound] = myDatum1.Find(2);
					Assert::IsFalse(isFound);
				}


				Datum myDatum2;
				myDatum2.PushBack(1.0f);
				myDatum2.PushBack(2.0f);
				myDatum2.PushBack(3.0f);
				Assert::AreEqual(static_cast<size_t>(3), myDatum2.Size());
				Assert::AreEqual(static_cast<size_t>(4), myDatum2.Capacity());

				{
					auto [datumRef, isFound] = myDatum2.Find(2.0f);
					Assert::IsTrue(isFound);
				}

				myDatum2.RemoveAt(1);

				Assert::AreEqual(static_cast<size_t>(2), myDatum2.Size());

				{
					auto [datumRef, isFound] = myDatum2.Find(2.0f);
					Assert::IsFalse(isFound);
				}



				Datum myDatum3;
				myDatum3.PushBack("a"s);
				myDatum3.PushBack("b"s);
				myDatum3.PushBack("c"s);
				Assert::AreEqual(static_cast<size_t>(3), myDatum3.Size());
				Assert::AreEqual(static_cast<size_t>(4), myDatum3.Capacity());

				{
					auto [datumRef, isFound] = myDatum3.Find("b"s);
					Assert::IsTrue(isFound);
				}


				myDatum3.RemoveAt(1);

				Assert::AreEqual(static_cast<size_t>(2), myDatum3.Size());

				{
					auto [datumRef, isFound] = myDatum3.Find("b"s);
					Assert::IsFalse(isFound);
				}


				Datum myDatum4;
				myDatum4.PushBack(myVec1);
				myDatum4.PushBack(myVec2);
				myDatum4.PushBack(myVec3);
				Assert::AreEqual(static_cast<size_t>(3), myDatum4.Size());
				Assert::AreEqual(static_cast<size_t>(4), myDatum4.Capacity());

				{
					auto [datumRef, isFound] = myDatum4.Find(myVec2);
					Assert::IsTrue(isFound);
				}


				myDatum4.RemoveAt(1);

				Assert::AreEqual(static_cast<size_t>(2), myDatum4.Size());

				{
					auto [datumRef, isFound] = myDatum4.Find(myVec2);
					Assert::IsFalse(isFound);
				}

				Datum myDatum5;
				myDatum5.PushBack(myMat1);
				myDatum5.PushBack(myMat2);
				myDatum5.PushBack(myMat3);
				Assert::AreEqual(static_cast<size_t>(3), myDatum5.Size());
				Assert::AreEqual(static_cast<size_t>(4), myDatum5.Capacity());


				{
					auto [datumRef, isFound] = myDatum5.Find(myMat2);
					Assert::IsTrue(isFound);
				}


				myDatum5.RemoveAt(1);

				Assert::AreEqual(static_cast<size_t>(2), myDatum5.Size());

				{
					auto [datumRef, isFound] = myDatum5.Find(myMat2);
					Assert::IsFalse(isFound);
				}


				Datum myDatum6;
				myDatum6.PushBack(myRTTI1);
				myDatum6.PushBack(myRTTI2);
				myDatum6.PushBack(myRTTI3);
				Assert::AreEqual(static_cast<size_t>(3), myDatum6.Size());
				Assert::AreEqual(static_cast<size_t>(4), myDatum6.Capacity());

				{
					auto [datumRef, isFound] = myDatum6.Find(myRTTI2);
					Assert::IsTrue(isFound);
				}


				myDatum6.RemoveAt(1);

				Assert::AreEqual(static_cast<size_t>(2), myDatum6.Size());

				{
					auto [datumRef, isFound] = myDatum6.Find(myRTTI2);
					Assert::IsFalse(isFound);
				}



			}			
		}
		TEST_METHOD(DatumFind)
		{
			//Value is in Datum
			{
				glm::vec4 myVec1(1.0f, 1.0f, 1.0f, 1.0f);
				glm::mat4 myMat1(1.0f);

				glm::vec4 myVec2(2.0f, 2.0f, 2.0f, 2.0f);
				glm::mat4 myMat2(2.0f);

				glm::vec4 myVec3(3.0f, 3.0f, 3.0f, 3.0f);
				glm::mat4 myMat3(3.0f);

				Foo myFoo1(1);
				RTTI* myRTTI1 = &myFoo1;

				Foo myFoo2(2);
				RTTI* myRTTI2 = &myFoo2;

				Foo myFoo3(3);
				RTTI* myRTTI3 = &myFoo3;

				Datum myDatum1;
				myDatum1.PushBack(1);
				myDatum1.PushBack(2);
				myDatum1.PushBack(3);

				{
					auto [datumRef, isFound] = myDatum1.Find(2);
					Assert::IsTrue(isFound);
				}

				Datum myDatum2;
				myDatum2.PushBack(1.0f);
				myDatum2.PushBack(2.0f);
				myDatum2.PushBack(3.0f);

				{
					auto [datumRef, isFound] = myDatum2.Find(2.0f);
					Assert::IsTrue(isFound);
				}

				Datum myDatum3;
				myDatum3.PushBack("a"s);
				myDatum3.PushBack("b"s);
				myDatum3.PushBack("c"s);

				{
					auto [datumRef, isFound] = myDatum3.Find("b"s);
					Assert::IsTrue(isFound);
				}

				Datum myDatum4;
				myDatum4.PushBack(myVec1);
				myDatum4.PushBack(myVec2);
				myDatum4.PushBack(myVec3);

				{
					auto [datumRef, isFound] = myDatum4.Find(myVec2);
					Assert::IsTrue(isFound);
				}

				Datum myDatum5;
				myDatum5.PushBack(myMat1);
				myDatum5.PushBack(myMat2);
				myDatum5.PushBack(myMat3);

				{
					auto [datumRef, isFound] = myDatum5.Find(myMat2);
					Assert::IsTrue(isFound);
				}

				Datum myDatum6;
				myDatum6.PushBack(myRTTI1);
				myDatum6.PushBack(myRTTI2);
				myDatum6.PushBack(myRTTI3);

				{
					auto [datumRef, isFound] = myDatum6.Find(myRTTI2);
					Assert::IsTrue(isFound);
				}
			}

			//Value is not in Datum
			{
				glm::vec4 myVec1(1.0f, 1.0f, 1.0f, 1.0f);
				glm::mat4 myMat1(1.0f);

				glm::vec4 myVec2(2.0f, 2.0f, 2.0f, 2.0f);
				glm::mat4 myMat2(2.0f);

				glm::vec4 myVec3(3.0f, 3.0f, 3.0f, 3.0f);
				glm::mat4 myMat3(3.0f);

				Foo myFoo1(1);
				RTTI* myRTTI1 = &myFoo1;

				Foo myFoo2(2);
				RTTI* myRTTI2 = &myFoo2;

				Foo myFoo3(3);
				RTTI* myRTTI3 = &myFoo3;

				Datum myDatum1;
				myDatum1.PushBack(1);
				myDatum1.PushBack(3);

				{
					auto [datumRef, isFound] = myDatum1.Find(2);
					Assert::IsFalse(isFound);
				}

				Datum myDatum2;
				myDatum2.PushBack(1.0f);
				myDatum2.PushBack(3.0f);

				{
					auto [datumRef, isFound] = myDatum2.Find(2.0f);
					Assert::IsFalse(isFound);
				}

				Datum myDatum3;
				myDatum3.PushBack("a"s);
				myDatum3.PushBack("c"s);

				{
					auto [datumRef, isFound] = myDatum3.Find("b"s);
					Assert::IsFalse(isFound);
				}

				Datum myDatum4;
				myDatum4.PushBack(myVec1);
				myDatum4.PushBack(myVec3);

				{
					auto [datumRef, isFound] = myDatum4.Find(myVec2);
					Assert::IsFalse(isFound);
				}

				Datum myDatum5;
				myDatum5.PushBack(myMat1);
				myDatum5.PushBack(myMat3);

				{
					auto [datumRef, isFound] = myDatum5.Find(myMat2);
					Assert::IsFalse(isFound);
				}

				Datum myDatum6;
				myDatum6.PushBack(myRTTI1);
				myDatum6.PushBack(myRTTI3);

				{
					auto [datumRef, isFound] = myDatum6.Find(myRTTI2);
					Assert::IsFalse(isFound);
				}

			}

			//Searching for type that is not compatible with Datum type
			{
				glm::vec4 myVec1(1.0f, 1.0f, 1.0f, 1.0f);
				glm::mat4 myMat1(1.0f);

				glm::vec4 myVec2(2.0f, 2.0f, 2.0f, 2.0f);
				glm::mat4 myMat2(2.0f);

				glm::vec4 myVec3(3.0f, 3.0f, 3.0f, 3.0f);
				glm::mat4 myMat3(3.0f);

				Foo myFoo1(1);
				RTTI* myRTTI1 = &myFoo1;

				Foo myFoo2(2);
				RTTI* myRTTI2 = &myFoo2;

				Foo myFoo3(3);
				RTTI* myRTTI3 = &myFoo3;

				Datum myDatum1;
				myDatum1.PushBack(1);
				myDatum1.PushBack(2);
				myDatum1.PushBack(3);

				//Searching for RTTI pointer in a Integer type Datum
				Assert::ExpectException<std::runtime_error>([&myDatum1, &myRTTI2]() {myDatum1.Find(myRTTI2); });

				Datum myDatum2;
				myDatum2.PushBack(1.0f);
				myDatum2.PushBack(2.0f);
				myDatum2.PushBack(3.0f);

				//Searching for Integer in a Float type Datum
				Assert::ExpectException<std::runtime_error>([&myDatum2]() {myDatum2.Find(2); });


				Datum myDatum3;
				myDatum3.PushBack("a"s);
				myDatum3.PushBack("b"s);
				myDatum3.PushBack("c"s);

				//Searching for Float in a String type Datum
				Assert::ExpectException<std::runtime_error>([&myDatum3]() {myDatum3.Find(2.0f); });

				Datum myDatum4;
				myDatum4.PushBack(myVec1);
				myDatum4.PushBack(myVec2);
				myDatum4.PushBack(myVec3);

				//Searching for String in a Vector type Datum
				Assert::ExpectException<std::runtime_error>([&myDatum4]() {myDatum4.Find("b"s); });

				Datum myDatum5;
				myDatum5.PushBack(myMat1);
				myDatum5.PushBack(myMat2);
				myDatum5.PushBack(myMat3);

				//Searching for Vector in a Matrix type Datum
				Assert::ExpectException<std::runtime_error>([&myDatum5, &myVec2]() {myDatum5.Find(myVec2); });
				Datum myDatum6;
				myDatum6.PushBack(myRTTI1);
				myDatum6.PushBack(myRTTI2);
				myDatum6.PushBack(myRTTI3);

				//Searching for Matrix in a RTTI pointer type Datum
				Assert::ExpectException<std::runtime_error>([&myDatum6, &myMat2]() {myDatum6.Find(myMat2); });
			}
		}
		TEST_METHOD(DatumCapacity)
		{
			{
				Datum myDatum(1);

				Assert::AreEqual(static_cast<size_t>(1), myDatum.Capacity());
			}
		}
		TEST_METHOD(DatumShrinkToFit)
		{
			//Internal Datum
			{
				glm::vec4 myVec1(1.0f, 1.0f, 1.0f, 1.0f);
				glm::mat4 myMat1(1.0f);

				Foo myFoo1(1);
				RTTI* myRTTI1 = &myFoo1;

				Datum myDatum1(1);
				Assert::AreEqual(static_cast<size_t>(1), myDatum1.Capacity());
				Assert::AreEqual(static_cast<size_t>(1), myDatum1.Size());
				myDatum1.Reserve(10);
				Assert::AreEqual(static_cast<size_t>(10), myDatum1.Capacity());
				myDatum1.ShrinkToFit();
				Assert::AreEqual(static_cast<size_t>(1), myDatum1.Capacity());
				Assert::AreEqual(static_cast<size_t>(1), myDatum1.Size());


				Datum myDatum2(1.0f);
				Assert::AreEqual(static_cast<size_t>(1), myDatum2.Capacity());
				Assert::AreEqual(static_cast<size_t>(1), myDatum2.Size());
				myDatum2.Reserve(10);
				Assert::AreEqual(static_cast<size_t>(10), myDatum2.Capacity());
				myDatum2.ShrinkToFit();
				Assert::AreEqual(static_cast<size_t>(1), myDatum2.Capacity());
				Assert::AreEqual(static_cast<size_t>(1), myDatum2.Size());

				Datum myDatum3("1");
				Assert::AreEqual(static_cast<size_t>(1), myDatum3.Capacity());
				Assert::AreEqual(static_cast<size_t>(1), myDatum3.Size());
				myDatum3.Reserve(10);
				Assert::AreEqual(static_cast<size_t>(10), myDatum3.Capacity());
				myDatum3.ShrinkToFit();
				Assert::AreEqual(static_cast<size_t>(1), myDatum3.Capacity());
				Assert::AreEqual(static_cast<size_t>(1), myDatum3.Size());

				Datum myDatum4(myVec1);
				Assert::AreEqual(static_cast<size_t>(1), myDatum4.Capacity());
				Assert::AreEqual(static_cast<size_t>(1), myDatum4.Size());
				myDatum4.Reserve(10);
				Assert::AreEqual(static_cast<size_t>(10), myDatum4.Capacity());
				myDatum4.ShrinkToFit();
				Assert::AreEqual(static_cast<size_t>(1), myDatum4.Capacity());
				Assert::AreEqual(static_cast<size_t>(1), myDatum4.Size());

				Datum myDatum5(myMat1);
				Assert::AreEqual(static_cast<size_t>(1), myDatum5.Capacity());
				Assert::AreEqual(static_cast<size_t>(1), myDatum5.Size());
				myDatum5.Reserve(10);
				Assert::AreEqual(static_cast<size_t>(10), myDatum5.Capacity());
				myDatum5.ShrinkToFit();
				Assert::AreEqual(static_cast<size_t>(1), myDatum5.Capacity());
				Assert::AreEqual(static_cast<size_t>(1), myDatum5.Size());

				Datum myDatum6(myRTTI1);
				Assert::AreEqual(static_cast<size_t>(1), myDatum6.Capacity());
				Assert::AreEqual(static_cast<size_t>(1), myDatum6.Size());
				myDatum6.Reserve(10);
				Assert::AreEqual(static_cast<size_t>(10), myDatum6.Capacity());
				myDatum6.ShrinkToFit();
				Assert::AreEqual(static_cast<size_t>(1), myDatum6.Capacity());
				Assert::AreEqual(static_cast<size_t>(1), myDatum6.Size());


			}

			//External Datum
			{

				glm::vec4 myPushBackVec(1.0f, 1.0f, 1.0f, 1.0f);
				glm::mat4 myPushBackMat(1.0f);
				Foo myPushBackFoo(1);
				RTTI* myPushBackRTTI = &myPushBackFoo;

				const int SIZE = 3;

				Foo myFoo1(1);
				Foo myFoo2(2);
				Foo myFoo3(3);
				RTTI* myRTTI1 = &myFoo1;
				RTTI* myRTTI2 = &myFoo2;
				RTTI* myRTTI3 = &myFoo3;

				int myIntArray[SIZE] = { 0,1,2 };
				float myFloatArray[SIZE] = { 0.f,1.f,2.f };
				std::string myStringArray[SIZE] = { "","","" };
				glm::vec4 myVecArray[SIZE] = { { 1.0f, 1.0f, 1.0f, 1.0f}, { 1.0f, 1.0f, 1.0f, 1.0f}, { 1.0f, 1.0f, 1.0f, 1.0f} };
				glm::mat4 myMatArray[SIZE] = { {1.0f, 1.0f, 1.0f, 1.0f,1.0f, 1.0f, 1.0f, 1.0f,1.0f, 1.0f, 1.0f, 1.0f,1.0f, 1.0f, 1.0f, 1.0f},
												{1.0f, 1.0f, 1.0f, 1.0f,1.0f, 1.0f, 1.0f, 1.0f,1.0f, 1.0f, 1.0f, 1.0f,1.0f, 1.0f, 1.0f, 1.0f},
												{1.0f, 1.0f, 1.0f, 1.0f,1.0f, 1.0f, 1.0f, 1.0f,1.0f, 1.0f, 1.0f, 1.0f,1.0f, 1.0f, 1.0f, 1.0f} };

				RTTI* myRTTIArray[SIZE] = { myRTTI1, myRTTI2, myRTTI3 };

				Datum myDatum1;
				myDatum1.SetStorage(myIntArray, SIZE);
				Assert::ExpectException<std::runtime_error>([&myDatum1]() {myDatum1.ShrinkToFit(); });

				Datum myDatum2;
				myDatum2.SetStorage(myFloatArray, SIZE);
				Assert::ExpectException<std::runtime_error>([&myDatum2]() {myDatum2.ShrinkToFit(); });

				Datum myDatum3;
				myDatum3.SetStorage(myStringArray, SIZE);
				Assert::ExpectException<std::runtime_error>([&myDatum3]() {myDatum3.ShrinkToFit(); });

				Datum myDatum4;
				myDatum4.SetStorage(myVecArray, SIZE);
				Assert::ExpectException<std::runtime_error>([&myDatum4, &myPushBackVec]() {myDatum4.ShrinkToFit(); });

				Datum myDatum5;
				myDatum5.SetStorage(myMatArray, SIZE);
				Assert::ExpectException<std::runtime_error>([&myDatum5, &myPushBackMat]() {myDatum5.ShrinkToFit(); });

				Datum myDatum6;
				myDatum6.SetStorage(myRTTIArray, SIZE);
				Assert::ExpectException<std::runtime_error>([&myDatum6, &myPushBackRTTI]() {myDatum6.ShrinkToFit(); });

			}

			//Shrink to fit on empty list will free the data.
			{
				glm::vec4 myVec1(1.0f, 1.0f, 1.0f, 1.0f);
				glm::mat4 myMat1(1.0f);

				Foo myFoo1(1);

				RTTI* myRTTI1 = &myFoo1;

				Datum myDatum1(1);
				Assert::IsTrue(myDatum1.IsDataSet());
				myDatum1.Clear();
				myDatum1.ShrinkToFit();
				Assert::IsFalse(myDatum1.IsDataSet());

				
				Datum myDatum2(1.0f);
				Assert::IsTrue(myDatum2.IsDataSet());
				myDatum2.Clear();
				myDatum2.ShrinkToFit();
				Assert::IsFalse(myDatum2.IsDataSet());

				Datum myDatum3("1");
				Assert::IsTrue(myDatum3.IsDataSet());
				myDatum3.Clear();
				myDatum3.ShrinkToFit();
				Assert::IsFalse(myDatum3.IsDataSet());

				Datum myDatum4(myVec1);
				Assert::IsTrue(myDatum4.IsDataSet());
				myDatum4.Clear();
				myDatum4.ShrinkToFit();
				Assert::IsFalse(myDatum4.IsDataSet());

				Datum myDatum5(myMat1);
				Assert::IsTrue(myDatum5.IsDataSet());
				myDatum5.Clear();
				myDatum5.ShrinkToFit();
				Assert::IsFalse(myDatum5.IsDataSet());

				Datum myDatum6(myRTTI1);
				Assert::IsTrue(myDatum6.IsDataSet());
				myDatum6.Clear();
				myDatum6.ShrinkToFit();
				Assert::IsFalse(myDatum6.IsDataSet());

			}

		}
		TEST_METHOD(DatumIsEmpty)
		{
			{
				glm::vec4 myVec1(1.0f, 1.0f, 1.0f, 1.0f);
				glm::mat4 myMat1(1.0f);

				Foo myFoo1(1);

				RTTI* myRTTI1 = &myFoo1;

				Datum myDatum1(1);
				Assert::IsFalse(myDatum1.IsEmpty());
				myDatum1.Clear();
				Assert::IsTrue(myDatum1.IsEmpty());


				Datum myDatum2(1.0f);
				Assert::IsFalse(myDatum2.IsEmpty());
				myDatum2.Clear();
				Assert::IsTrue(myDatum2.IsEmpty());

				Datum myDatum3("1");
				Assert::IsFalse(myDatum3.IsEmpty());
				myDatum3.Clear();
				Assert::IsTrue(myDatum3.IsEmpty());

				Datum myDatum4(myVec1);
				Assert::IsFalse(myDatum4.IsEmpty());
				myDatum4.Clear();
				Assert::IsTrue(myDatum4.IsEmpty());

				Datum myDatum5(myMat1);
				Assert::IsFalse(myDatum5.IsEmpty());
				myDatum5.Clear();
				Assert::IsTrue(myDatum5.IsEmpty());

				Datum myDatum6(myRTTI1);
				Assert::IsFalse(myDatum6.IsEmpty());
				myDatum6.Clear();
				Assert::IsTrue(myDatum6.IsEmpty());

			}
		}
		TEST_METHOD(DatumIsExternal)
		{
			{
				const int SIZE = 3;

				Foo myFoo1(1);
				Foo myFoo2(2);
				Foo myFoo3(3);
				RTTI* myRTTI1 = &myFoo1;
				RTTI* myRTTI2 = &myFoo2;
				RTTI* myRTTI3 = &myFoo3;

				int myIntArray[SIZE] = { 0,1,2 };
				float myFloatArray[SIZE] = { 0.f,1.f,2.f };
				std::string myStringArray[SIZE] = { "","","" };
				glm::vec4 myVecArray[SIZE] = { { 1.0f, 1.0f, 1.0f, 1.0f}, { 1.0f, 1.0f, 1.0f, 1.0f}, { 1.0f, 1.0f, 1.0f, 1.0f} };
				glm::mat4 myMatArray[SIZE] = { {1.0f, 1.0f, 1.0f, 1.0f,1.0f, 1.0f, 1.0f, 1.0f,1.0f, 1.0f, 1.0f, 1.0f,1.0f, 1.0f, 1.0f, 1.0f},
												{1.0f, 1.0f, 1.0f, 1.0f,1.0f, 1.0f, 1.0f, 1.0f,1.0f, 1.0f, 1.0f, 1.0f,1.0f, 1.0f, 1.0f, 1.0f},
												{1.0f, 1.0f, 1.0f, 1.0f,1.0f, 1.0f, 1.0f, 1.0f,1.0f, 1.0f, 1.0f, 1.0f,1.0f, 1.0f, 1.0f, 1.0f} };

				RTTI* myRTTIArray[SIZE] = { myRTTI1, myRTTI2, myRTTI3 };

				Datum myDatum1;
				Assert::IsFalse(myDatum1.IsExternal());
				myDatum1.SetStorage(myIntArray, SIZE);
				Assert::IsTrue(myDatum1.IsExternal());

				Datum myDatum2;
				Assert::IsFalse(myDatum2.IsExternal());
				myDatum2.SetStorage(myFloatArray, SIZE);
				Assert::IsTrue(myDatum2.IsExternal());

				Datum myDatum3;
				Assert::IsFalse(myDatum3.IsExternal());
				myDatum3.SetStorage(myStringArray, SIZE);
				Assert::IsTrue(myDatum3.IsExternal());

				Datum myDatum4;
				Assert::IsFalse(myDatum4.IsExternal());
				myDatum4.SetStorage(myVecArray, SIZE);
				Assert::IsTrue(myDatum4.IsExternal());

				Datum myDatum5;
				Assert::IsFalse(myDatum5.IsExternal());
				myDatum5.SetStorage(myMatArray, SIZE);
				Assert::IsTrue(myDatum5.IsExternal());

				Datum myDatum6;
				Assert::IsFalse(myDatum6.IsExternal());
				myDatum6.SetStorage(myRTTIArray, SIZE);
				Assert::IsTrue(myDatum6.IsExternal());

			}

		}
		TEST_METHOD(DatumIsDataSet)
		{
			glm::vec4 myVec1(1.0f, 1.0f, 1.0f, 1.0f);
			glm::mat4 myMat1(1.0f);

			Foo myFoo1(1);

			RTTI* myRTTI1 = &myFoo1;

			Datum myDatum1;
			Assert::IsFalse(myDatum1.IsDataSet());
			myDatum1 = 1;
			Assert::IsTrue(myDatum1.IsDataSet());


			Datum myDatum2;
			Assert::IsFalse(myDatum2.IsDataSet());
			myDatum2 = 1.0f;
			Assert::IsTrue(myDatum2.IsDataSet());


			Datum myDatum3;
			Assert::IsFalse(myDatum3.IsDataSet());
			myDatum3 = "1";
			Assert::IsTrue(myDatum3.IsDataSet());


			Datum myDatum4;
			Assert::IsFalse(myDatum4.IsDataSet());
			myDatum4 = myVec1;
			Assert::IsTrue(myDatum4.IsDataSet());


			Datum myDatum5;
			Assert::IsFalse(myDatum5.IsDataSet());
			myDatum5 = myMat1;
			Assert::IsTrue(myDatum5.IsDataSet());


			Datum myDatum6;
			Assert::IsFalse(myDatum6.IsDataSet());
			myDatum6 = myRTTI1;
			Assert::IsTrue(myDatum6.IsDataSet());


		}
		TEST_METHOD(DatumTypeCastConstructor)
		{
			glm::vec4 myVec1(1.0f, 1.0f, 1.0f, 1.0f);
			glm::mat4 myMat1(1.0f);

			Foo myFoo1(1);

			RTTI* myRTTI1 = &myFoo1;

			Datum myDatum1(1);
			Assert::AreEqual(DatumTypes::Integer, myDatum1.Type());
			Assert::AreEqual(static_cast<size_t>(1), myDatum1.Size());
			Assert::AreEqual(static_cast<size_t>(1), myDatum1.Capacity());


			Datum myDatum2(1.0f);
			Assert::AreEqual(DatumTypes::Float, myDatum2.Type());
			Assert::AreEqual(static_cast<size_t>(1), myDatum2.Size());
			Assert::AreEqual(static_cast<size_t>(1), myDatum2.Capacity());


			Datum myDatum3("1"s);
			Assert::AreEqual(DatumTypes::String, myDatum3.Type());
			Assert::AreEqual(static_cast<size_t>(1), myDatum3.Size());
			Assert::AreEqual(static_cast<size_t>(1), myDatum3.Capacity());


			Datum myDatum4(myVec1);
			Assert::AreEqual(DatumTypes::Vector4, myDatum4.Type());
			Assert::AreEqual(static_cast<size_t>(1), myDatum4.Size());
			Assert::AreEqual(static_cast<size_t>(1), myDatum4.Capacity());


			Datum myDatum5(myMat1);
			Assert::AreEqual(DatumTypes::Matrix, myDatum5.Type());
			Assert::AreEqual(static_cast<size_t>(1), myDatum5.Size());
			Assert::AreEqual(static_cast<size_t>(1), myDatum5.Capacity());


			Datum myDatum6(myRTTI1);
			Assert::AreEqual(DatumTypes::Pointer, myDatum6.Type());
			Assert::AreEqual(static_cast<size_t>(1), myDatum6.Size());
			Assert::AreEqual(static_cast<size_t>(1), myDatum6.Capacity());
		}
		TEST_METHOD(DatumInitializerList)
		{

			glm::vec4 myVec1(1.0f, 1.0f, 1.0f, 1.0f);
			glm::mat4 myMat1(1.0f);

			glm::vec4 myVec2(2.0f, 2.0f, 2.0f, 2.0f);
			glm::mat4 myMat2(2.0f);

			glm::vec4 myVec3(3.0f, 3.0f, 3.0f, 3.0f);
			glm::mat4 myMat3(3.0f);

			Foo myFoo1(1);
			RTTI* myRTTI1 = &myFoo1;

			Foo myFoo2(2);
			RTTI* myRTTI2 = &myFoo2;

			Foo myFoo3(3);
			RTTI* myRTTI3 = &myFoo3;

			Datum myDatum1{1,2,3};
			Assert::AreEqual(static_cast<size_t>(3), myDatum1.Size());
			Assert::AreEqual(static_cast<size_t>(3), myDatum1.Capacity());
			Assert::AreEqual(DatumTypes::Integer, myDatum1.Type());

			Datum myDatum2{ 1.0f, 2.0f, 3.0f };
			Assert::AreEqual(static_cast<size_t>(3), myDatum2.Size());
			Assert::AreEqual(static_cast<size_t>(3), myDatum2.Capacity());
			Assert::AreEqual(DatumTypes::Float, myDatum2.Type());

			Datum myDatum3{ "a"s,"b"s,"c"s };
			Assert::AreEqual(static_cast<size_t>(3), myDatum3.Size());
			Assert::AreEqual(static_cast<size_t>(3), myDatum3.Capacity());
			Assert::AreEqual(DatumTypes::String, myDatum3.Type());

			Datum myDatum4{ myVec1 ,myVec2, myVec3};
			Assert::AreEqual(static_cast<size_t>(3), myDatum4.Size());
			Assert::AreEqual(static_cast<size_t>(3), myDatum4.Capacity());
			Assert::AreEqual(DatumTypes::Vector4, myDatum4.Type());

			Datum myDatum5{ myMat1, myMat3, myMat3 };
			Assert::AreEqual(static_cast<size_t>(3), myDatum5.Size());
			Assert::AreEqual(static_cast<size_t>(3), myDatum5.Capacity());
			Assert::AreEqual(DatumTypes::Matrix, myDatum5.Type());

			Datum myDatum6{ myRTTI1, myRTTI2, myRTTI3};
			Assert::AreEqual(static_cast<size_t>(3), myDatum6.Size());
			Assert::AreEqual(static_cast<size_t>(3), myDatum6.Capacity());
			Assert::AreEqual(DatumTypes::Pointer, myDatum6.Type());

		}

	private:
		static _CrtMemState _startMemState; //static class members are shared in every instance of the class
	};

	_CrtMemState DatumTest::_startMemState; //There is needed to separate declaration from definition. Changes in C++17
}