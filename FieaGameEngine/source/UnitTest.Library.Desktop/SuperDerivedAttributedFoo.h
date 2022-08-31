#pragma once
#include "DerivedAttributedFoo.h"

namespace UnitTestLibraryDesktop
{
	class SuperDerivedAttributedFoo : public DerivedAttributedFoo
	{
		RTTI_DECLARATIONS(SuperDerivedAttributedFoo, DerivedAttributedFoo);

	public:
		static const int ArraySize = 10;

		SuperDerivedAttributedFoo();

		virtual ~SuperDerivedAttributedFoo() override = default;
		SuperDerivedAttributedFoo(const SuperDerivedAttributedFoo& rhs) = default;
		SuperDerivedAttributedFoo(SuperDerivedAttributedFoo&& rhs) noexcept = default;

		SuperDerivedAttributedFoo& operator=(const SuperDerivedAttributedFoo& rhs) = default;
		SuperDerivedAttributedFoo& operator=(SuperDerivedAttributedFoo&& rhs) noexcept = default;


		int32_t _SDAFexternalInteger{ 0 }; //using acronym DAF Derived Attributed Foo
		float _SDAFexternalFloat{ 0.0f };
		std::string _SDAFexternalString{ "" };
		glm::vec4 _SDAFexternalVector{ 0 };
		glm::mat4 _SDAFexternalMatrix{ 0 };
		RTTI* _SDAFexternalRTTI{ nullptr };

		//Foo* DAFmyFoo{ nullptr };


		int32_t _SDAFexternalIntegerArray[ArraySize];
		float _SDAFexternalFloatArray[ArraySize];
		std::string _SDAFexternalStringArray[ArraySize];
		glm::vec4 _SDAFexternalVectorArray[ArraySize];
		glm::mat4 _SDAFexternalMatrixArray[ArraySize];
		RTTI* _SDAFexternalRTTIArray[ArraySize];

		//Foo* DAFmyFooArray[ArraySize]{ nullptr };

		bool Equals(const RTTI* rhs) const;

		StaticRegisterMethod(SuperDerivedAttributedFoo, DerivedAttributedFoo);
		static const Vector<Signature> Signatures();

	protected:
		SuperDerivedAttributedFoo(std::size_t typeId);

	};
	ConcreteFactory(SuperDerivedAttributedFoo, Scope);
}

