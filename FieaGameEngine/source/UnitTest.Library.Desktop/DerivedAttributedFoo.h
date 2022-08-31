#pragma once
#include "AttributedFoo.h"

namespace UnitTestLibraryDesktop
{
	class DerivedAttributedFoo : public AttributedFoo
	{
		RTTI_DECLARATIONS(DerivedAttributedFoo, AttributedFoo)

	public:
		static const int ArraySize = 10;

		DerivedAttributedFoo();

		virtual ~DerivedAttributedFoo() override = default;
		DerivedAttributedFoo(const DerivedAttributedFoo& rhs) = default;
		DerivedAttributedFoo(DerivedAttributedFoo&& rhs) noexcept = default;

		DerivedAttributedFoo& operator=(const DerivedAttributedFoo& rhs) = default;
		DerivedAttributedFoo& operator=(DerivedAttributedFoo&& rhs) noexcept = default;


		int32_t _DAFexternalInteger{ 0 }; //using acronym DAF Derived Attributed Foo
		float _DAFexternalFloat{ 0.0f };
		std::string _DAFexternalString{ "" };
		glm::vec4 _DAFexternalVector{ 0 };
		glm::mat4 _DAFexternalMatrix{ 0 };
		RTTI* _DAFexternalRTTI{ nullptr };

		//Foo* DAFmyFoo{ nullptr };


		int32_t _DAFexternalIntegerArray[ArraySize];
		float _DAFexternalFloatArray[ArraySize];
		std::string _DAFexternalStringArray[ArraySize];
		glm::vec4 _DAFexternalVectorArray[ArraySize];
		glm::mat4 _DAFexternalMatrixArray[ArraySize];
		RTTI* _DAFexternalRTTIArray[ArraySize];

		//Foo* DAFmyFooArray[ArraySize]{ nullptr };

		bool Equals(const RTTI * rhs) const;


		StaticRegisterMethod(DerivedAttributedFoo, AttributedFoo);
		static const Vector<Signature> Signatures();

	protected:
		DerivedAttributedFoo(std::size_t typeId);

	};
	ConcreteFactory(DerivedAttributedFoo, Scope);
}

