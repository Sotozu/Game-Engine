#pragma once
#include "Attributed.h"
#include "TypeRegistry.h"
#include "Factory.h"
#include "Foo.h"

using namespace FieaGameEngine;

namespace UnitTestLibraryDesktop
{

	class AttributedFoo : public Attributed
	{
		RTTI_DECLARATIONS(AttributedFoo, Attributed)

	public:
		static const int ArraySize = 3;

		AttributedFoo();

		virtual ~AttributedFoo() = default;
		AttributedFoo(const AttributedFoo& rhs) = default;
		AttributedFoo(AttributedFoo&& rhs) noexcept = default;

		AttributedFoo& operator=(const AttributedFoo& rhs) = default;
		AttributedFoo& operator=(AttributedFoo&& rhs) noexcept = default;


		int32_t _externalInteger{ 0 };
		float _externalFloat{ 0.0f };
		std::string _externalString{ "" };
		glm::vec4 _externalVector{ 0 };
		glm::mat4 _externalMatrix{ 0 };
		RTTI* _externalRTTI{ nullptr };

		//Foo* myFoo{ nullptr };


		int32_t _externalIntegerArray[ArraySize];
		float _externalFloatArray[ArraySize];
		std::string _externalStringArray[ArraySize];
		glm::vec4 _externalVectorArray[ArraySize];
		glm::mat4 _externalMatrixArray[ArraySize];
		RTTI* _externalRTTIArray[ArraySize];

		//Foo* myFooArray[ArraySize]{ nullptr };

		bool Equals(const RTTI* rhs) const override;

		StaticRegisterMethod(AttributedFoo, Attributed);
		static const Vector<Signature> Signatures();

	protected:
		AttributedFoo(std::size_t typeId);
	private:
	};

	ConcreteFactory(AttributedFoo, Scope);
}


