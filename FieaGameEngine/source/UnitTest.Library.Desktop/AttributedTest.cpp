#include "pch.h"
#include "ToStringSpecializations.h"
#include "Attributed.h"
#include "TypeRegistry.h"
#include "AttributedFoo.h"
#include "DerivedAttributedFoo.h"
#include "SuperDerivedAttributedFoo.h"
#include "Foo.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTestLibraryDesktop
{

	TEST_CLASS(AttributedTest)
	{
	public:
		TEST_METHOD_INITIALIZE(Initialize) //Before every test this is called.
		{
			//TypeRegistry::RegisterType<AttributedFoo, Attributed>();
			TypeRegistry::RegisterType<SuperDerivedAttributedFoo, DerivedAttributedFoo>();
			//TypeRegistry::RegisterType<DerivedAttributedFoo>();

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
		TEST_METHOD(AttributedConstructor)
		{
			AttributedFoo myAttributedFoo;

			Assert::IsTrue(myAttributedFoo.Is("Attributed"));
			Assert::IsTrue(myAttributedFoo.Is("Scope"));

			Assert::IsFalse(myAttributedFoo.Is("Foo"));
		}
		TEST_METHOD(AttributedCopyConstructor)
		{
			{
				Foo myFoo{ 1 };
				RTTI* myRTTI = &myFoo;

				AttributedFoo myAttributedFoo;

				myAttributedFoo._externalInteger = 10;
				myAttributedFoo._externalFloat = 10.0f;
				myAttributedFoo._externalString = "A";
				myAttributedFoo._externalVector = { 1,2,3,4 };
				myAttributedFoo._externalMatrix = { {1,2,3,4},{1,2,3,4},{1,2,3,4},{1,2,3,4} };
				myAttributedFoo._externalRTTI = myRTTI;

				AttributedFoo myOtherAttributedFoo(myAttributedFoo);

				Assert::AreEqual(myAttributedFoo._externalInteger, myOtherAttributedFoo._externalInteger);
				Assert::AreEqual(myAttributedFoo._externalFloat, myOtherAttributedFoo._externalFloat);
				Assert::AreEqual(myAttributedFoo._externalString, myOtherAttributedFoo._externalString);
				Assert::AreEqual(myAttributedFoo._externalVector, myOtherAttributedFoo._externalVector);
				Assert::AreEqual(myAttributedFoo._externalMatrix, myOtherAttributedFoo._externalMatrix);
				Assert::AreEqual(myAttributedFoo._externalRTTI, myOtherAttributedFoo._externalRTTI);

				Assert::AreNotSame(myAttributedFoo._externalInteger, myOtherAttributedFoo._externalInteger);
				Assert::AreNotSame(myAttributedFoo._externalFloat, myOtherAttributedFoo._externalFloat);
				Assert::AreNotSame(myAttributedFoo._externalString, myOtherAttributedFoo._externalString);
				Assert::AreNotSame(myAttributedFoo._externalVector, myOtherAttributedFoo._externalVector);
				Assert::AreNotSame(myAttributedFoo._externalMatrix, myOtherAttributedFoo._externalMatrix);
				Assert::AreNotSame(myAttributedFoo._externalRTTI, myOtherAttributedFoo._externalRTTI);
			}

			//Added Auxilary Attributes
			{
				Foo myFoo{ 1 };
				RTTI* myRTTI = &myFoo;

				AttributedFoo myAttributedFoo;

				myAttributedFoo._externalInteger = 10;
				myAttributedFoo._externalFloat = 10.0f;
				myAttributedFoo._externalString = "A";
				myAttributedFoo._externalVector = { 1,2,3,4 };
				myAttributedFoo._externalMatrix = { {1,2,3,4},{1,2,3,4},{1,2,3,4},{1,2,3,4} };
				myAttributedFoo._externalRTTI = myRTTI;

				Datum& myDatum = myAttributedFoo.Append("AuxiliaryInteger");
				myDatum.SetType(DatumTypes::Integer);

				myDatum = 999;

				AttributedFoo myOtherAttributedFoo(myAttributedFoo);

				Assert::AreEqual(myAttributedFoo._externalInteger, myOtherAttributedFoo._externalInteger);
				Assert::AreEqual(myAttributedFoo._externalFloat, myOtherAttributedFoo._externalFloat);
				Assert::AreEqual(myAttributedFoo._externalString, myOtherAttributedFoo._externalString);
				Assert::AreEqual(myAttributedFoo._externalVector, myOtherAttributedFoo._externalVector);
				Assert::AreEqual(myAttributedFoo._externalMatrix, myOtherAttributedFoo._externalMatrix);
				Assert::AreEqual(myAttributedFoo._externalRTTI, myOtherAttributedFoo._externalRTTI);


				Assert::AreNotSame(myAttributedFoo._externalInteger, myOtherAttributedFoo._externalInteger);
				Assert::AreNotSame(myAttributedFoo._externalFloat, myOtherAttributedFoo._externalFloat);
				Assert::AreNotSame(myAttributedFoo._externalString, myOtherAttributedFoo._externalString);
				Assert::AreNotSame(myAttributedFoo._externalVector, myOtherAttributedFoo._externalVector);
				Assert::AreNotSame(myAttributedFoo._externalMatrix, myOtherAttributedFoo._externalMatrix);
				Assert::AreNotSame(myAttributedFoo._externalRTTI, myOtherAttributedFoo._externalRTTI);

				Datum* myDatum1 = myAttributedFoo.Find("AuxiliaryInteger");
				Datum* myDatum2 = myOtherAttributedFoo.Find("AuxiliaryInteger");

				Assert::AreEqual(*myDatum1, *myDatum2);
			}

		}
		TEST_METHOD(AttributeCopyAssignment) 
		{
			Foo myFoo{ 1 };
			RTTI* myRTTI = &myFoo;

			AttributedFoo myAttributedFoo;

			myAttributedFoo._externalInteger = 10;
			myAttributedFoo._externalFloat = 10.0f;
			myAttributedFoo._externalString = "A";
			myAttributedFoo._externalVector = { 1,2,3,4 };
			myAttributedFoo._externalMatrix = { {1,2,3,4},{1,2,3,4},{1,2,3,4},{1,2,3,4} };
			myAttributedFoo._externalRTTI = myRTTI;

			AttributedFoo myOtherAttributedFoo;

			myOtherAttributedFoo = myAttributedFoo;
			
			Assert::AreEqual(myAttributedFoo._externalInteger, myOtherAttributedFoo._externalInteger);
			Assert::AreEqual(myAttributedFoo._externalFloat, myOtherAttributedFoo._externalFloat);
			Assert::AreEqual(myAttributedFoo._externalString, myOtherAttributedFoo._externalString);
			Assert::AreEqual(myAttributedFoo._externalVector, myOtherAttributedFoo._externalVector);
			Assert::AreEqual(myAttributedFoo._externalMatrix, myOtherAttributedFoo._externalMatrix);
			Assert::AreEqual(myAttributedFoo._externalRTTI, myOtherAttributedFoo._externalRTTI);

			Assert::AreNotSame(myAttributedFoo._externalInteger, myOtherAttributedFoo._externalInteger);
			Assert::AreNotSame(myAttributedFoo._externalFloat, myOtherAttributedFoo._externalFloat);
			Assert::AreNotSame(myAttributedFoo._externalString, myOtherAttributedFoo._externalString);
			Assert::AreNotSame(myAttributedFoo._externalVector, myOtherAttributedFoo._externalVector);
			Assert::AreNotSame(myAttributedFoo._externalMatrix, myOtherAttributedFoo._externalMatrix);
			Assert::AreNotSame(myAttributedFoo._externalRTTI, myOtherAttributedFoo._externalRTTI);

		}
		TEST_METHOD(AttributeMoveConstructor)
		{
			Foo myFoo{ 1 };
			RTTI* myRTTI = &myFoo;
			std::string myStr = "A";
			glm::vec4 myVec{ 1,2,3,4 };
			glm::mat4 myMat{ {1,2,3,4},{1,2,3,4},{1,2,3,4},{1,2,3,4} };

			AttributedFoo myAttributedFoo;
			myAttributedFoo._externalInteger = 10;
			myAttributedFoo._externalFloat = 10.0f;
			myAttributedFoo._externalString = myStr;
			myAttributedFoo._externalVector = { 1,2,3,4 };
			myAttributedFoo._externalMatrix = { {1,2,3,4},{1,2,3,4},{1,2,3,4},{1,2,3,4} };
			myAttributedFoo._externalRTTI = myRTTI;

			AttributedFoo myOtherAttributedFoo(std::move(myAttributedFoo));

			Assert::AreEqual(10, myOtherAttributedFoo._externalInteger);
			Assert::AreEqual(10.0f, myOtherAttributedFoo._externalFloat);
			Assert::AreEqual(myStr, myOtherAttributedFoo._externalString);
			Assert::AreEqual(myVec, myOtherAttributedFoo._externalVector);
			Assert::AreEqual(myMat, myOtherAttributedFoo._externalMatrix);
			Assert::AreEqual(myRTTI, myOtherAttributedFoo._externalRTTI);
		}
		TEST_METHOD(AttributeMoveAssignment)
		{
			Foo myFoo{ 1 };
			RTTI* myRTTI = &myFoo;
			std::string myStr = "A";
			glm::vec4 myVec{ 1,2,3,4 };
			glm::mat4 myMat{ {1,2,3,4},{1,2,3,4},{1,2,3,4},{1,2,3,4} };

			AttributedFoo myAttributedFoo;
			myAttributedFoo._externalInteger = 10;
			myAttributedFoo._externalFloat = 10.0f;
			myAttributedFoo._externalString = myStr;
			myAttributedFoo._externalVector = { 1,2,3,4 };
			myAttributedFoo._externalMatrix = { {1,2,3,4},{1,2,3,4},{1,2,3,4},{1,2,3,4} };
			myAttributedFoo._externalRTTI = myRTTI;

			AttributedFoo myOtherAttributedFoo;
			
			myOtherAttributedFoo = (std::move(myAttributedFoo));

			Assert::AreEqual(10, myOtherAttributedFoo._externalInteger);
			Assert::AreEqual(10.0f, myOtherAttributedFoo._externalFloat);
			Assert::AreEqual(myStr, myOtherAttributedFoo._externalString);
			Assert::AreEqual(myVec, myOtherAttributedFoo._externalVector);
			Assert::AreEqual(myMat, myOtherAttributedFoo._externalMatrix);
			Assert::AreEqual(myRTTI, myOtherAttributedFoo._externalRTTI);


		}
		TEST_METHOD(AttributedPopulate)
		{
			AttributedFoo myAttributedFoo;

			Assert::IsTrue(myAttributedFoo.Is("Attributed"));
			Assert::IsTrue(myAttributedFoo.Is("Scope"));

			//myAttributedFoo.myFoo = new Foo;

			//for (std::size_t index = 0; index < myAttributedFoo.ArraySize; ++index)
			//{
			//	myAttributedFoo.myFooArray[index] = new Foo;
			//}

			Datum* datumPtrThis = myAttributedFoo.Find("this"s);
			Datum* datumInt = myAttributedFoo.Find("_externalInteger"s);
			Datum* datumFloat = myAttributedFoo.Find("_externalFloat"s);
			Datum* datumString = myAttributedFoo.Find("_externalString"s);
			Datum* datumVector = myAttributedFoo.Find("_externalVector"s);
			Datum* datumMatrix = myAttributedFoo.Find("_externalMatrix"s);
			Datum* datumRTTI = myAttributedFoo.Find("_externalRTTI"s); 
			//Datum* datumRTTIFooPtr = myAttributedFoo.Find("_classFoo"); 
			
			
			Datum* datumIntArray = myAttributedFoo.Find("_externalIntegerArray"s);
			Datum* datumFloatArray = myAttributedFoo.Find("_externalFloatArray"s);
			Datum* datumStringArray = myAttributedFoo.Find("_externalStringArray"s);
			Datum* datumVectorArray = myAttributedFoo.Find("_externalVectorArray"s);
			Datum* datumMatrixArray = myAttributedFoo.Find("_externalMatrixArray"s);
			Datum* datumRTTIArray = myAttributedFoo.Find("_externalRTTIArray"s);
			//Datum* datumRTTIFooPtrArray = myAttributedFoo.Find("_classFooArray");


			AttributedFoo& referenceToAttributedFoo = *(static_cast<AttributedFoo*>(datumPtrThis->Get<RTTI*>()));

			Assert::AreSame(referenceToAttributedFoo, myAttributedFoo);
			Assert::AreSame(datumInt->Get<int32_t>(), myAttributedFoo._externalInteger);
			Assert::AreSame(datumFloat->Get<float>(), myAttributedFoo._externalFloat);
			Assert::AreSame(datumString->Get<std::string>(), myAttributedFoo._externalString);
			Assert::AreSame(datumVector->Get<glm::vec4>(), myAttributedFoo._externalVector);
			Assert::AreSame(datumMatrix->Get<glm::mat4>(), myAttributedFoo._externalMatrix);
			Assert::AreSame(datumRTTI->Get<RTTI*>(), myAttributedFoo._externalRTTI);

			//Assert::AreSame(*datumRTTIFooPtr->Get<RTTI*>(), *reinterpret_cast<RTTI*>(myAttributedFoo.myFoo));
			
			//ALL THE ARRAYS ARE THE SAME SIZE SO THIS FOR LOOP WILL BE ABLE TO COVER THE ITEMS IN ALL TYPES PRESENT IN THE TEST

			for (std::size_t index = 0; index < datumIntArray->Size(); ++index)
			{
				Assert::AreSame(datumIntArray->Get<int32_t>(index), myAttributedFoo._externalIntegerArray[index]);
				Assert::AreSame(datumFloatArray->Get<float>(index), myAttributedFoo._externalFloatArray[index]);
				Assert::AreSame(datumStringArray->Get<std::string>(index), myAttributedFoo._externalStringArray[index]);
				Assert::AreSame(datumVectorArray->Get<glm::vec4>(index), myAttributedFoo._externalVectorArray[index]);
				Assert::AreSame(datumMatrixArray->Get<glm::mat4>(index), myAttributedFoo._externalMatrixArray[index]);
				Assert::AreSame(datumRTTIArray->Get<RTTI*>(index), myAttributedFoo._externalRTTIArray[index]);

				//Assert::AreSame(*datumRTTIFooPtrArray->Get<RTTI*>(index), *reinterpret_cast<RTTI*>(myAttributedFoo.myFooArray[index]));

			}

		}
		TEST_METHOD(AttribuedIsAttribute)
		{
			AttributedFoo myAttributedFoo;

		
			Assert::IsTrue(myAttributedFoo.IsAttribute("this"));
			Assert::IsTrue(myAttributedFoo.IsAttribute("_externalInteger"));
			Assert::IsTrue(myAttributedFoo.IsAttribute("_externalFloat"));
			Assert::IsTrue(myAttributedFoo.IsAttribute("_externalString"));
			Assert::IsTrue(myAttributedFoo.IsAttribute("_externalVector"));
			Assert::IsTrue(myAttributedFoo.IsAttribute("_externalMatrix"));
			Assert::IsTrue(myAttributedFoo.IsAttribute("_externalRTTI"));

			Assert::IsTrue(myAttributedFoo.IsAttribute("_externalIntegerArray"));
			Assert::IsTrue(myAttributedFoo.IsAttribute("_externalFloatArray"));
			Assert::IsTrue(myAttributedFoo.IsAttribute("_externalStringArray"));
			Assert::IsTrue(myAttributedFoo.IsAttribute("_externalVectorArray"));
			Assert::IsTrue(myAttributedFoo.IsAttribute("_externalMatrixArray"));
			Assert::IsTrue(myAttributedFoo.IsAttribute("_externalRTTIArray"));

			Datum& newDatum = myAttributedFoo.Append("MyNewVaraible");

			newDatum.SetType(DatumTypes::Integer);
			newDatum = 10;

			Assert::IsTrue(myAttributedFoo.IsAttribute("MyNewVaraible"));

			Assert::IsFalse(myAttributedFoo.IsAttribute("NOT HERE!"));
		}
		TEST_METHOD(AttributedIsPrescribedAttribute)
		{
			AttributedFoo myAttributedFoo;

			Assert::IsTrue(myAttributedFoo.IsPrescribedAttribute("this"));

			Assert::IsTrue(myAttributedFoo.IsPrescribedAttribute("_externalInteger"));
			Assert::IsTrue(myAttributedFoo.IsPrescribedAttribute("_externalFloat"));
			Assert::IsTrue(myAttributedFoo.IsPrescribedAttribute("_externalString"));
			Assert::IsTrue(myAttributedFoo.IsPrescribedAttribute("_externalVector"));
			Assert::IsTrue(myAttributedFoo.IsPrescribedAttribute("_externalMatrix"));
			Assert::IsTrue(myAttributedFoo.IsPrescribedAttribute("_externalRTTI"));
			//Assert::IsTrue(myAttributedFoo.IsPrescribedAttribute("_classFoo"));

			Assert::IsTrue(myAttributedFoo.IsPrescribedAttribute("_externalIntegerArray"));
			Assert::IsTrue(myAttributedFoo.IsPrescribedAttribute("_externalFloatArray"));
			Assert::IsTrue(myAttributedFoo.IsPrescribedAttribute("_externalStringArray"));
			Assert::IsTrue(myAttributedFoo.IsPrescribedAttribute("_externalVectorArray"));
			Assert::IsTrue(myAttributedFoo.IsPrescribedAttribute("_externalMatrixArray"));
			Assert::IsTrue(myAttributedFoo.IsPrescribedAttribute("_externalRTTIArray"));
			//Assert::IsTrue(myAttributedFoo.IsPrescribedAttribute("_classFooArray"));


			Datum& newDatum = myAttributedFoo.Append("MyNewVaraible");

			newDatum.SetType(DatumTypes::Integer);
			newDatum = 10;

			Assert::IsFalse(myAttributedFoo.IsPrescribedAttribute("MyNewVaraible"));

		}
		TEST_METHOD(AttributedIsAuxiliaryAttribute)
		{

			AttributedFoo myAttributedFoo;

			Assert::IsFalse(myAttributedFoo.IsAuxilaryAttribute("this"));

			Assert::IsFalse(myAttributedFoo.IsAuxilaryAttribute("_externalInteger"));
			Assert::IsFalse(myAttributedFoo.IsAuxilaryAttribute("_externalFloat"));
			Assert::IsFalse(myAttributedFoo.IsAuxilaryAttribute("_externalString"));
			Assert::IsFalse(myAttributedFoo.IsAuxilaryAttribute("_externalVector"));
			Assert::IsFalse(myAttributedFoo.IsAuxilaryAttribute("_externalMatrix"));
			Assert::IsFalse(myAttributedFoo.IsAuxilaryAttribute("_externalRTTI"));
			//Assert::IsTrue(myAttributedFoo.IsPrescribedAttribute("_classFoo"));

			Assert::IsFalse(myAttributedFoo.IsAuxilaryAttribute("_externalIntegerArray"));
			Assert::IsFalse(myAttributedFoo.IsAuxilaryAttribute("_externalFloatArray"));
			Assert::IsFalse(myAttributedFoo.IsAuxilaryAttribute("_externalStringArray"));
			Assert::IsFalse(myAttributedFoo.IsAuxilaryAttribute("_externalVectorArray"));
			Assert::IsFalse(myAttributedFoo.IsAuxilaryAttribute("_externalMatrixArray"));
			Assert::IsFalse(myAttributedFoo.IsAuxilaryAttribute("_externalRTTIArray"));
			//Assert::IsFalse(myAttributedFoo.IsAuxilaryAttribute("_classFooArray"));


			Datum& newDatum = myAttributedFoo.Append("MyNewVaraible");

			newDatum.SetType(DatumTypes::Integer);
			newDatum = 10;

			Assert::IsTrue(myAttributedFoo.IsAuxilaryAttribute("MyNewVaraible"));
		}

		TEST_METHOD(AttributedFooEquals)
		{
			
			
			Foo myFoo{ 1 };
			RTTI* myRTTI = &myFoo;

			AttributedFoo myAttributedFoo;

			myAttributedFoo._externalInteger = 10;
			myAttributedFoo._externalFloat = 10.0f;
			myAttributedFoo._externalString = "A";
			myAttributedFoo._externalVector = { 1,2,3,4 };
			myAttributedFoo._externalMatrix = { {1,2,3,4},{1,2,3,4},{1,2,3,4},{1,2,3,4} };
			myAttributedFoo._externalRTTI = myRTTI;

			AttributedFoo myOtherAttributedFoo;

			myOtherAttributedFoo = myAttributedFoo;

			Assert::AreEqual(myAttributedFoo._externalInteger, myOtherAttributedFoo._externalInteger);
			Assert::AreEqual(myAttributedFoo._externalFloat, myOtherAttributedFoo._externalFloat);
			Assert::AreEqual(myAttributedFoo._externalString, myOtherAttributedFoo._externalString);
			Assert::AreEqual(myAttributedFoo._externalVector, myOtherAttributedFoo._externalVector);
			Assert::AreEqual(myAttributedFoo._externalMatrix, myOtherAttributedFoo._externalMatrix);
			Assert::AreEqual(myAttributedFoo._externalRTTI, myOtherAttributedFoo._externalRTTI);

			Assert::AreNotSame(myAttributedFoo._externalInteger, myOtherAttributedFoo._externalInteger);
			Assert::AreNotSame(myAttributedFoo._externalFloat, myOtherAttributedFoo._externalFloat);
			Assert::AreNotSame(myAttributedFoo._externalString, myOtherAttributedFoo._externalString);
			Assert::AreNotSame(myAttributedFoo._externalVector, myOtherAttributedFoo._externalVector);
			Assert::AreNotSame(myAttributedFoo._externalMatrix, myOtherAttributedFoo._externalMatrix);
			Assert::AreNotSame(myAttributedFoo._externalRTTI, myOtherAttributedFoo._externalRTTI);


			Assert::IsTrue(myAttributedFoo.Equals(&myOtherAttributedFoo));

			//CHANGING ONE DATUM!
			myOtherAttributedFoo._externalInteger = 999;

			Assert::IsFalse(myAttributedFoo.Equals(&myOtherAttributedFoo));

		}
		TEST_METHOD(AttributedDerivedAttributedFoo)
		{
			Foo myFoo1{ 1 };
			RTTI* myRTTI1 = &myFoo1;
			Foo myFoo2{ 2 };
			RTTI* myRTTI2 = &myFoo2;

			DerivedAttributedFoo myDerivedAttributedFoo;

			myDerivedAttributedFoo._externalInteger = 10;
			myDerivedAttributedFoo._externalFloat = 10.0f;
			myDerivedAttributedFoo._externalString = "A";
			myDerivedAttributedFoo._externalVector = { 1,2,3,4 };
			myDerivedAttributedFoo._externalMatrix = { {1,2,3,4},{1,2,3,4},{1,2,3,4},{1,2,3,4} };
			myDerivedAttributedFoo._externalRTTI = myRTTI1;

			for (std::int32_t index = 0; index < 3; ++index) {

				float indexF = static_cast<float>(index);

				myDerivedAttributedFoo._externalIntegerArray[index] = index;
				myDerivedAttributedFoo._externalFloatArray[index] = indexF;
				myDerivedAttributedFoo._externalStringArray[index] = ("A" + index);
				myDerivedAttributedFoo._externalVectorArray[index] = { indexF,indexF,indexF,indexF };
				myDerivedAttributedFoo._externalMatrixArray[index] = { {indexF,indexF,indexF,indexF},{indexF,indexF,indexF,indexF},{indexF,indexF,indexF,indexF},{indexF,indexF,indexF,indexF} };
				myDerivedAttributedFoo._externalRTTIArray[index] = myRTTI1;

			}

			myDerivedAttributedFoo._DAFexternalInteger = 10;
			myDerivedAttributedFoo._DAFexternalFloat = 10.0f;
			myDerivedAttributedFoo._DAFexternalString = "A";
			myDerivedAttributedFoo._DAFexternalVector = { 1,2,3,4 };
			myDerivedAttributedFoo._DAFexternalMatrix = { {1,2,3,4},{1,2,3,4},{1,2,3,4},{1,2,3,4} };
			myDerivedAttributedFoo._DAFexternalRTTI = myRTTI2;

			for (std::int32_t index = 0; index < myDerivedAttributedFoo.ArraySize; ++index) {

				float indexF = static_cast<float>(index);

				myDerivedAttributedFoo._DAFexternalIntegerArray[index] = index;
				myDerivedAttributedFoo._DAFexternalFloatArray[index] = indexF;
				myDerivedAttributedFoo._DAFexternalStringArray[index] = ("A" + index);
				myDerivedAttributedFoo._DAFexternalVectorArray[index] = { indexF,indexF,indexF,indexF };
				myDerivedAttributedFoo._DAFexternalMatrixArray[index] = { {indexF,indexF,indexF,indexF},{indexF,indexF,indexF,indexF},{indexF,indexF,indexF,indexF},{indexF,indexF,indexF,indexF} };
				myDerivedAttributedFoo._DAFexternalRTTIArray[index] = myRTTI2;

			}


			DerivedAttributedFoo myOtherDerivedAttributedFoo;

			myOtherDerivedAttributedFoo = myDerivedAttributedFoo;

			Assert::AreEqual(myDerivedAttributedFoo._externalInteger, myOtherDerivedAttributedFoo._externalInteger);
			Assert::AreEqual(myDerivedAttributedFoo._externalFloat,   myOtherDerivedAttributedFoo._externalFloat);
			Assert::AreEqual(myDerivedAttributedFoo._externalString,  myOtherDerivedAttributedFoo._externalString);
			Assert::AreEqual(myDerivedAttributedFoo._externalVector,  myOtherDerivedAttributedFoo._externalVector);
			Assert::AreEqual(myDerivedAttributedFoo._externalMatrix,  myOtherDerivedAttributedFoo._externalMatrix);
			Assert::AreEqual(myDerivedAttributedFoo._externalRTTI,    myOtherDerivedAttributedFoo._externalRTTI);

			Assert::AreNotSame(myDerivedAttributedFoo._externalInteger, myOtherDerivedAttributedFoo._externalInteger);
			Assert::AreNotSame(myDerivedAttributedFoo._externalFloat,   myOtherDerivedAttributedFoo._externalFloat);
			Assert::AreNotSame(myDerivedAttributedFoo._externalString,  myOtherDerivedAttributedFoo._externalString);
			Assert::AreNotSame(myDerivedAttributedFoo._externalVector,  myOtherDerivedAttributedFoo._externalVector);
			Assert::AreNotSame(myDerivedAttributedFoo._externalMatrix,  myOtherDerivedAttributedFoo._externalMatrix);
			Assert::AreNotSame(myDerivedAttributedFoo._externalRTTI,    myOtherDerivedAttributedFoo._externalRTTI);

			for (std::size_t index = 0; index < 3; ++index) 
			{

				Assert::AreEqual(myDerivedAttributedFoo._externalIntegerArray[index], myOtherDerivedAttributedFoo._externalIntegerArray[index]);
				Assert::AreEqual(myDerivedAttributedFoo._externalFloatArray[index],   myOtherDerivedAttributedFoo._externalFloatArray[index]);
				Assert::AreEqual(myDerivedAttributedFoo._externalStringArray[index],  myOtherDerivedAttributedFoo._externalStringArray[index]);
				Assert::AreEqual(myDerivedAttributedFoo._externalVectorArray[index],  myOtherDerivedAttributedFoo._externalVectorArray[index]);
				Assert::AreEqual(myDerivedAttributedFoo._externalMatrixArray[index],  myOtherDerivedAttributedFoo._externalMatrixArray[index]);
				Assert::AreEqual(myDerivedAttributedFoo._externalRTTIArray[index],    myOtherDerivedAttributedFoo._externalRTTIArray[index]);

				Assert::AreNotSame(myDerivedAttributedFoo._externalIntegerArray[index], myOtherDerivedAttributedFoo._externalIntegerArray[index]);
				Assert::AreNotSame(myDerivedAttributedFoo._externalFloatArray[index],   myOtherDerivedAttributedFoo._externalFloatArray[index]);
				Assert::AreNotSame(myDerivedAttributedFoo._externalStringArray[index],  myOtherDerivedAttributedFoo._externalStringArray[index]);
				Assert::AreNotSame(myDerivedAttributedFoo._externalVectorArray[index],  myOtherDerivedAttributedFoo._externalVectorArray[index]);
				Assert::AreNotSame(myDerivedAttributedFoo._externalMatrixArray[index],  myOtherDerivedAttributedFoo._externalMatrixArray[index]);
				Assert::AreNotSame(myDerivedAttributedFoo._externalRTTIArray[index],    myOtherDerivedAttributedFoo._externalRTTIArray[index]);

			}

			Assert::AreEqual(myDerivedAttributedFoo._DAFexternalInteger, myOtherDerivedAttributedFoo._DAFexternalInteger);
			Assert::AreEqual(myDerivedAttributedFoo._DAFexternalFloat,	 myOtherDerivedAttributedFoo._DAFexternalFloat);
			Assert::AreEqual(myDerivedAttributedFoo._DAFexternalString,  myOtherDerivedAttributedFoo._DAFexternalString);
			Assert::AreEqual(myDerivedAttributedFoo._DAFexternalVector,  myOtherDerivedAttributedFoo._DAFexternalVector);
			Assert::AreEqual(myDerivedAttributedFoo._DAFexternalMatrix,  myOtherDerivedAttributedFoo._DAFexternalMatrix);
			Assert::AreEqual(myDerivedAttributedFoo._DAFexternalRTTI,	 myOtherDerivedAttributedFoo._DAFexternalRTTI);

			Assert::AreNotSame(myDerivedAttributedFoo._DAFexternalInteger, myOtherDerivedAttributedFoo._DAFexternalInteger);
			Assert::AreNotSame(myDerivedAttributedFoo._DAFexternalFloat,   myOtherDerivedAttributedFoo._DAFexternalFloat);
			Assert::AreNotSame(myDerivedAttributedFoo._DAFexternalString,  myOtherDerivedAttributedFoo._DAFexternalString);
			Assert::AreNotSame(myDerivedAttributedFoo._DAFexternalVector,  myOtherDerivedAttributedFoo._DAFexternalVector);
			Assert::AreNotSame(myDerivedAttributedFoo._DAFexternalMatrix,  myOtherDerivedAttributedFoo._DAFexternalMatrix);
			Assert::AreNotSame(myDerivedAttributedFoo._DAFexternalRTTI,    myOtherDerivedAttributedFoo._DAFexternalRTTI);

			for (std::size_t index = 0; index < myDerivedAttributedFoo.ArraySize; ++index)
			{

				Assert::AreEqual(myDerivedAttributedFoo._DAFexternalIntegerArray[index], myOtherDerivedAttributedFoo._DAFexternalIntegerArray[index]);
				Assert::AreEqual(myDerivedAttributedFoo._DAFexternalFloatArray[index],   myOtherDerivedAttributedFoo._DAFexternalFloatArray[index]);
				Assert::AreEqual(myDerivedAttributedFoo._DAFexternalStringArray[index],  myOtherDerivedAttributedFoo._DAFexternalStringArray[index]);
				Assert::AreEqual(myDerivedAttributedFoo._DAFexternalVectorArray[index],  myOtherDerivedAttributedFoo._DAFexternalVectorArray[index]);
				Assert::AreEqual(myDerivedAttributedFoo._DAFexternalMatrixArray[index],  myOtherDerivedAttributedFoo._DAFexternalMatrixArray[index]);
				Assert::AreEqual(myDerivedAttributedFoo._DAFexternalRTTIArray[index],    myOtherDerivedAttributedFoo._DAFexternalRTTIArray[index]);

				Assert::AreNotSame(myDerivedAttributedFoo._DAFexternalIntegerArray[index], myOtherDerivedAttributedFoo._DAFexternalIntegerArray[index]);
				Assert::AreNotSame(myDerivedAttributedFoo._DAFexternalFloatArray[index],   myOtherDerivedAttributedFoo._DAFexternalFloatArray[index]);
				Assert::AreNotSame(myDerivedAttributedFoo._DAFexternalStringArray[index],  myOtherDerivedAttributedFoo._DAFexternalStringArray[index]);
				Assert::AreNotSame(myDerivedAttributedFoo._DAFexternalVectorArray[index],  myOtherDerivedAttributedFoo._DAFexternalVectorArray[index]);
				Assert::AreNotSame(myDerivedAttributedFoo._DAFexternalMatrixArray[index],  myOtherDerivedAttributedFoo._DAFexternalMatrixArray[index]);
				Assert::AreNotSame(myDerivedAttributedFoo._DAFexternalRTTIArray[index],    myOtherDerivedAttributedFoo._DAFexternalRTTIArray[index]);

			}


			Assert::IsTrue(myDerivedAttributedFoo.Equals(&myOtherDerivedAttributedFoo));

			//CHANGING ONE DATUM!
			myOtherDerivedAttributedFoo._externalInteger = 999;

			Assert::IsFalse(myDerivedAttributedFoo.Equals(&myOtherDerivedAttributedFoo));
		}
		TEST_METHOD(SuperDerivedAttributedFooTest)
		{
			Foo myFoo1{ 1 };
			RTTI* myRTTI1 = &myFoo1;
			Foo myFoo2{ 2 };
			RTTI* myRTTI2 = &myFoo2;

			SuperDerivedAttributedFoo mySuperDerivedAttributedFoo;

			mySuperDerivedAttributedFoo._externalInteger = 10;
			mySuperDerivedAttributedFoo._externalFloat = 10.0f;
			mySuperDerivedAttributedFoo._externalString = "A";
			mySuperDerivedAttributedFoo._externalVector = { 1,2,3,4 };
			mySuperDerivedAttributedFoo._externalMatrix = { {1,2,3,4},{1,2,3,4},{1,2,3,4},{1,2,3,4} };
			mySuperDerivedAttributedFoo._externalRTTI = myRTTI1;

			for (std::int32_t index = 0; index < 3; ++index) {

				float indexF = static_cast<float>(index);

				mySuperDerivedAttributedFoo._externalIntegerArray[index] = index;
				mySuperDerivedAttributedFoo._externalFloatArray[index] = indexF;
				mySuperDerivedAttributedFoo._externalStringArray[index] = ("A" + index);
				mySuperDerivedAttributedFoo._externalVectorArray[index] = { indexF,indexF,indexF,indexF };
				mySuperDerivedAttributedFoo._externalMatrixArray[index] = { {indexF,indexF,indexF,indexF},{indexF,indexF,indexF,indexF},{indexF,indexF,indexF,indexF},{indexF,indexF,indexF,indexF} };
				mySuperDerivedAttributedFoo._externalRTTIArray[index] = myRTTI1;

			}

			mySuperDerivedAttributedFoo._DAFexternalInteger = 10;
			mySuperDerivedAttributedFoo._DAFexternalFloat = 10.0f;
			mySuperDerivedAttributedFoo._DAFexternalString = "A";
			mySuperDerivedAttributedFoo._DAFexternalVector = { 1,2,3,4 };
			mySuperDerivedAttributedFoo._DAFexternalMatrix = { {1,2,3,4},{1,2,3,4},{1,2,3,4},{1,2,3,4} };
			mySuperDerivedAttributedFoo._DAFexternalRTTI = myRTTI2;

			for (std::int32_t index = 0; index < mySuperDerivedAttributedFoo.ArraySize; ++index) {

				float indexF = static_cast<float>(index);

				mySuperDerivedAttributedFoo._DAFexternalIntegerArray[index] = index;
				mySuperDerivedAttributedFoo._DAFexternalFloatArray[index] = indexF;
				mySuperDerivedAttributedFoo._DAFexternalStringArray[index] = ("A" + index);
				mySuperDerivedAttributedFoo._DAFexternalVectorArray[index] = { indexF,indexF,indexF,indexF };
				mySuperDerivedAttributedFoo._DAFexternalMatrixArray[index] = { {indexF,indexF,indexF,indexF},{indexF,indexF,indexF,indexF},{indexF,indexF,indexF,indexF},{indexF,indexF,indexF,indexF} };
				mySuperDerivedAttributedFoo._DAFexternalRTTIArray[index] = myRTTI2;

			}

			mySuperDerivedAttributedFoo._SDAFexternalInteger = 10;
			mySuperDerivedAttributedFoo._SDAFexternalFloat = 10.0f;
			mySuperDerivedAttributedFoo._SDAFexternalString = "A";
			mySuperDerivedAttributedFoo._SDAFexternalVector = { 1,2,3,4 };
			mySuperDerivedAttributedFoo._SDAFexternalMatrix = { {1,2,3,4},{1,2,3,4},{1,2,3,4},{1,2,3,4} };
			mySuperDerivedAttributedFoo._SDAFexternalRTTI = myRTTI2;

			for (std::int32_t index = 0; index < mySuperDerivedAttributedFoo.ArraySize; ++index) {

				float indexF = static_cast<float>(index);

				mySuperDerivedAttributedFoo._SDAFexternalIntegerArray[index] = index;
				mySuperDerivedAttributedFoo._SDAFexternalFloatArray[index] = indexF;
				mySuperDerivedAttributedFoo._SDAFexternalStringArray[index] = ("A" + index);
				mySuperDerivedAttributedFoo._SDAFexternalVectorArray[index] = { indexF,indexF,indexF,indexF };
				mySuperDerivedAttributedFoo._SDAFexternalMatrixArray[index] = { {indexF,indexF,indexF,indexF},{indexF,indexF,indexF,indexF},{indexF,indexF,indexF,indexF},{indexF,indexF,indexF,indexF} };
				mySuperDerivedAttributedFoo._SDAFexternalRTTIArray[index] = myRTTI2;

			}


			SuperDerivedAttributedFoo myOtherSuperDerivedAttributedFoo;

			myOtherSuperDerivedAttributedFoo = mySuperDerivedAttributedFoo;

			Assert::AreEqual(mySuperDerivedAttributedFoo._externalInteger, myOtherSuperDerivedAttributedFoo._externalInteger);
			Assert::AreEqual(mySuperDerivedAttributedFoo._externalFloat, myOtherSuperDerivedAttributedFoo._externalFloat);
			Assert::AreEqual(mySuperDerivedAttributedFoo._externalString, myOtherSuperDerivedAttributedFoo._externalString);
			Assert::AreEqual(mySuperDerivedAttributedFoo._externalVector, myOtherSuperDerivedAttributedFoo._externalVector);
			Assert::AreEqual(mySuperDerivedAttributedFoo._externalMatrix, myOtherSuperDerivedAttributedFoo._externalMatrix);
			Assert::AreEqual(mySuperDerivedAttributedFoo._externalRTTI, myOtherSuperDerivedAttributedFoo._externalRTTI);

			Assert::AreNotSame(mySuperDerivedAttributedFoo._externalInteger, myOtherSuperDerivedAttributedFoo._externalInteger);
			Assert::AreNotSame(mySuperDerivedAttributedFoo._externalFloat, myOtherSuperDerivedAttributedFoo._externalFloat);
			Assert::AreNotSame(mySuperDerivedAttributedFoo._externalString, myOtherSuperDerivedAttributedFoo._externalString);
			Assert::AreNotSame(mySuperDerivedAttributedFoo._externalVector, myOtherSuperDerivedAttributedFoo._externalVector);
			Assert::AreNotSame(mySuperDerivedAttributedFoo._externalMatrix, myOtherSuperDerivedAttributedFoo._externalMatrix);
			Assert::AreNotSame(mySuperDerivedAttributedFoo._externalRTTI, myOtherSuperDerivedAttributedFoo._externalRTTI);

			for (std::size_t index = 0; index < 3; ++index)
			{

				Assert::AreEqual(mySuperDerivedAttributedFoo._externalIntegerArray[index], myOtherSuperDerivedAttributedFoo._externalIntegerArray[index]);
				Assert::AreEqual(mySuperDerivedAttributedFoo._externalFloatArray[index], myOtherSuperDerivedAttributedFoo._externalFloatArray[index]);
				Assert::AreEqual(mySuperDerivedAttributedFoo._externalStringArray[index], myOtherSuperDerivedAttributedFoo._externalStringArray[index]);
				Assert::AreEqual(mySuperDerivedAttributedFoo._externalVectorArray[index], myOtherSuperDerivedAttributedFoo._externalVectorArray[index]);
				Assert::AreEqual(mySuperDerivedAttributedFoo._externalMatrixArray[index], myOtherSuperDerivedAttributedFoo._externalMatrixArray[index]);
				Assert::AreEqual(mySuperDerivedAttributedFoo._externalRTTIArray[index], myOtherSuperDerivedAttributedFoo._externalRTTIArray[index]);

				Assert::AreNotSame(mySuperDerivedAttributedFoo._externalIntegerArray[index], myOtherSuperDerivedAttributedFoo._externalIntegerArray[index]);
				Assert::AreNotSame(mySuperDerivedAttributedFoo._externalFloatArray[index], myOtherSuperDerivedAttributedFoo._externalFloatArray[index]);
				Assert::AreNotSame(mySuperDerivedAttributedFoo._externalStringArray[index], myOtherSuperDerivedAttributedFoo._externalStringArray[index]);
				Assert::AreNotSame(mySuperDerivedAttributedFoo._externalVectorArray[index], myOtherSuperDerivedAttributedFoo._externalVectorArray[index]);
				Assert::AreNotSame(mySuperDerivedAttributedFoo._externalMatrixArray[index], myOtherSuperDerivedAttributedFoo._externalMatrixArray[index]);
				Assert::AreNotSame(mySuperDerivedAttributedFoo._externalRTTIArray[index], myOtherSuperDerivedAttributedFoo._externalRTTIArray[index]);

			}

			Assert::AreEqual(mySuperDerivedAttributedFoo._DAFexternalInteger, myOtherSuperDerivedAttributedFoo._DAFexternalInteger);
			Assert::AreEqual(mySuperDerivedAttributedFoo._DAFexternalFloat, myOtherSuperDerivedAttributedFoo._DAFexternalFloat);
			Assert::AreEqual(mySuperDerivedAttributedFoo._DAFexternalString, myOtherSuperDerivedAttributedFoo._DAFexternalString);
			Assert::AreEqual(mySuperDerivedAttributedFoo._DAFexternalVector, myOtherSuperDerivedAttributedFoo._DAFexternalVector);
			Assert::AreEqual(mySuperDerivedAttributedFoo._DAFexternalMatrix, myOtherSuperDerivedAttributedFoo._DAFexternalMatrix);
			Assert::AreEqual(mySuperDerivedAttributedFoo._DAFexternalRTTI, myOtherSuperDerivedAttributedFoo._DAFexternalRTTI);

			Assert::AreNotSame(mySuperDerivedAttributedFoo._DAFexternalInteger, myOtherSuperDerivedAttributedFoo._DAFexternalInteger);
			Assert::AreNotSame(mySuperDerivedAttributedFoo._DAFexternalFloat, myOtherSuperDerivedAttributedFoo._DAFexternalFloat);
			Assert::AreNotSame(mySuperDerivedAttributedFoo._DAFexternalString, myOtherSuperDerivedAttributedFoo._DAFexternalString);
			Assert::AreNotSame(mySuperDerivedAttributedFoo._DAFexternalVector, myOtherSuperDerivedAttributedFoo._DAFexternalVector);
			Assert::AreNotSame(mySuperDerivedAttributedFoo._DAFexternalMatrix, myOtherSuperDerivedAttributedFoo._DAFexternalMatrix);
			Assert::AreNotSame(mySuperDerivedAttributedFoo._DAFexternalRTTI, myOtherSuperDerivedAttributedFoo._DAFexternalRTTI);

			for (std::size_t index = 0; index < mySuperDerivedAttributedFoo.ArraySize; ++index)
			{

				Assert::AreEqual(mySuperDerivedAttributedFoo._DAFexternalIntegerArray[index], myOtherSuperDerivedAttributedFoo._DAFexternalIntegerArray[index]);
				Assert::AreEqual(mySuperDerivedAttributedFoo._DAFexternalFloatArray[index], myOtherSuperDerivedAttributedFoo._DAFexternalFloatArray[index]);
				Assert::AreEqual(mySuperDerivedAttributedFoo._DAFexternalStringArray[index], myOtherSuperDerivedAttributedFoo._DAFexternalStringArray[index]);
				Assert::AreEqual(mySuperDerivedAttributedFoo._DAFexternalVectorArray[index], myOtherSuperDerivedAttributedFoo._DAFexternalVectorArray[index]);
				Assert::AreEqual(mySuperDerivedAttributedFoo._DAFexternalMatrixArray[index], myOtherSuperDerivedAttributedFoo._DAFexternalMatrixArray[index]);
				Assert::AreEqual(mySuperDerivedAttributedFoo._DAFexternalRTTIArray[index], myOtherSuperDerivedAttributedFoo._DAFexternalRTTIArray[index]);

				Assert::AreNotSame(mySuperDerivedAttributedFoo._DAFexternalIntegerArray[index], myOtherSuperDerivedAttributedFoo._DAFexternalIntegerArray[index]);
				Assert::AreNotSame(mySuperDerivedAttributedFoo._DAFexternalFloatArray[index], myOtherSuperDerivedAttributedFoo._DAFexternalFloatArray[index]);
				Assert::AreNotSame(mySuperDerivedAttributedFoo._DAFexternalStringArray[index], myOtherSuperDerivedAttributedFoo._DAFexternalStringArray[index]);
				Assert::AreNotSame(mySuperDerivedAttributedFoo._DAFexternalVectorArray[index], myOtherSuperDerivedAttributedFoo._DAFexternalVectorArray[index]);
				Assert::AreNotSame(mySuperDerivedAttributedFoo._DAFexternalMatrixArray[index], myOtherSuperDerivedAttributedFoo._DAFexternalMatrixArray[index]);
				Assert::AreNotSame(mySuperDerivedAttributedFoo._DAFexternalRTTIArray[index], myOtherSuperDerivedAttributedFoo._DAFexternalRTTIArray[index]);

			}


			Assert::AreEqual(mySuperDerivedAttributedFoo._SDAFexternalInteger, myOtherSuperDerivedAttributedFoo._SDAFexternalInteger);
			Assert::AreEqual(mySuperDerivedAttributedFoo._SDAFexternalFloat, myOtherSuperDerivedAttributedFoo._SDAFexternalFloat);
			Assert::AreEqual(mySuperDerivedAttributedFoo._SDAFexternalString, myOtherSuperDerivedAttributedFoo._SDAFexternalString);
			Assert::AreEqual(mySuperDerivedAttributedFoo._SDAFexternalVector, myOtherSuperDerivedAttributedFoo._SDAFexternalVector);
			Assert::AreEqual(mySuperDerivedAttributedFoo._SDAFexternalMatrix, myOtherSuperDerivedAttributedFoo._SDAFexternalMatrix);
			Assert::AreEqual(mySuperDerivedAttributedFoo._SDAFexternalRTTI, myOtherSuperDerivedAttributedFoo._SDAFexternalRTTI);

			Assert::AreNotSame(mySuperDerivedAttributedFoo._SDAFexternalInteger, myOtherSuperDerivedAttributedFoo._SDAFexternalInteger);
			Assert::AreNotSame(mySuperDerivedAttributedFoo._SDAFexternalFloat, myOtherSuperDerivedAttributedFoo._SDAFexternalFloat);
			Assert::AreNotSame(mySuperDerivedAttributedFoo._SDAFexternalString, myOtherSuperDerivedAttributedFoo._SDAFexternalString);
			Assert::AreNotSame(mySuperDerivedAttributedFoo._SDAFexternalVector, myOtherSuperDerivedAttributedFoo._SDAFexternalVector);
			Assert::AreNotSame(mySuperDerivedAttributedFoo._SDAFexternalMatrix, myOtherSuperDerivedAttributedFoo._SDAFexternalMatrix);
			Assert::AreNotSame(mySuperDerivedAttributedFoo._SDAFexternalRTTI, myOtherSuperDerivedAttributedFoo._SDAFexternalRTTI);

			for (std::size_t index = 0; index < mySuperDerivedAttributedFoo.ArraySize; ++index)
			{

				Assert::AreEqual(mySuperDerivedAttributedFoo._SDAFexternalIntegerArray[index], myOtherSuperDerivedAttributedFoo._SDAFexternalIntegerArray[index]);
				Assert::AreEqual(mySuperDerivedAttributedFoo._SDAFexternalFloatArray[index], myOtherSuperDerivedAttributedFoo._SDAFexternalFloatArray[index]);
				Assert::AreEqual(mySuperDerivedAttributedFoo._SDAFexternalStringArray[index], myOtherSuperDerivedAttributedFoo._SDAFexternalStringArray[index]);
				Assert::AreEqual(mySuperDerivedAttributedFoo._SDAFexternalVectorArray[index], myOtherSuperDerivedAttributedFoo._SDAFexternalVectorArray[index]);
				Assert::AreEqual(mySuperDerivedAttributedFoo._SDAFexternalMatrixArray[index], myOtherSuperDerivedAttributedFoo._SDAFexternalMatrixArray[index]);
				Assert::AreEqual(mySuperDerivedAttributedFoo._SDAFexternalRTTIArray[index], myOtherSuperDerivedAttributedFoo._SDAFexternalRTTIArray[index]);

				Assert::AreNotSame(mySuperDerivedAttributedFoo._SDAFexternalIntegerArray[index], myOtherSuperDerivedAttributedFoo._SDAFexternalIntegerArray[index]);
				Assert::AreNotSame(mySuperDerivedAttributedFoo._SDAFexternalFloatArray[index], myOtherSuperDerivedAttributedFoo._SDAFexternalFloatArray[index]);
				Assert::AreNotSame(mySuperDerivedAttributedFoo._SDAFexternalStringArray[index], myOtherSuperDerivedAttributedFoo._SDAFexternalStringArray[index]);
				Assert::AreNotSame(mySuperDerivedAttributedFoo._SDAFexternalVectorArray[index], myOtherSuperDerivedAttributedFoo._SDAFexternalVectorArray[index]);
				Assert::AreNotSame(mySuperDerivedAttributedFoo._SDAFexternalMatrixArray[index], myOtherSuperDerivedAttributedFoo._SDAFexternalMatrixArray[index]);
				Assert::AreNotSame(mySuperDerivedAttributedFoo._SDAFexternalRTTIArray[index], myOtherSuperDerivedAttributedFoo._SDAFexternalRTTIArray[index]);

			}


			Assert::IsTrue(mySuperDerivedAttributedFoo.Equals(&myOtherSuperDerivedAttributedFoo));

			//CHANGING ONE DATUM!
			myOtherSuperDerivedAttributedFoo._externalInteger = 999;

			Assert::IsFalse(mySuperDerivedAttributedFoo.Equals(&myOtherSuperDerivedAttributedFoo));

		}

	private:
		static _CrtMemState _startMemState; //static class members are shared in every instance of the class
	};

	_CrtMemState AttributedTest::_startMemState; //There is needed to separate declaration from definition. Changes in C++17
}