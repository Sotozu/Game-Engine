#include "pch.h"
#include "AttributedFoo.h"

using namespace FieaGameEngine;

namespace UnitTestLibraryDesktop
{
	RTTI_DEFINITIONS(AttributedFoo);

	AttributedFoo::AttributedFoo() : Attributed(AttributedFoo::TypeIdClass())
	{}

	//protected version
	AttributedFoo::AttributedFoo(std::size_t typeId) : Attributed(typeId), 
		_externalVector{},
		_externalMatrix{}, 
		_externalIntegerArray{},
		_externalFloatArray{}, 
		_externalVectorArray{},
		_externalMatrixArray{}
	{}

	const Vector<Signature> AttributedFoo::Signatures()
	{
		return Vector<Signature> 
		{
			{"_externalInteger", DatumTypes::Integer, 1, offsetof(AttributedFoo, _externalInteger)},
			{"_externalFloat", DatumTypes::Float, 1, offsetof(AttributedFoo, _externalFloat)},
			{"_externalString", DatumTypes::String, 1, offsetof(AttributedFoo, _externalString)},
			{"_externalVector", DatumTypes::Vector4, 1, offsetof(AttributedFoo, _externalVector)},
			{"_externalMatrix", DatumTypes::Matrix, 1, offsetof(AttributedFoo, _externalMatrix)},
			{"_externalRTTI", DatumTypes::Pointer, 1, offsetof(AttributedFoo, _externalRTTI)},
			{"_scope", DatumTypes::Table, 0, 0},
			//{"_classFoo", DatumTypes::Pointer, 1, offsetof(AttributedFoo, myFoo) },

			{ "_externalIntegerArray", DatumTypes::Integer, ArraySize, offsetof(AttributedFoo, _externalIntegerArray) },
			{ "_externalFloatArray", DatumTypes::Float, ArraySize, offsetof(AttributedFoo, _externalFloatArray) },
			{ "_externalStringArray", DatumTypes::String, ArraySize, offsetof(AttributedFoo, _externalStringArray) },
			{ "_externalVectorArray", DatumTypes::Vector4, ArraySize, offsetof(AttributedFoo, _externalVectorArray) },
			{ "_externalMatrixArray", DatumTypes::Matrix, ArraySize, offsetof(AttributedFoo, _externalMatrixArray) },
			{ "_externalRTTIArray", DatumTypes::Pointer, ArraySize, offsetof(AttributedFoo, _externalRTTIArray) },
			{ "_nestedScope", DatumTypes::Table, ArraySize, 0 }
			//{ "_classFooArray", DatumTypes::Pointer, ArraySize, offsetof(AttributedFoo, myFooArray) }


		};
	}

	bool AttributedFoo::Equals(const RTTI* rhs) const
	{
		const AttributedFoo* const rhsFoo = rhs->As<AttributedFoo>();
		if (rhsFoo == nullptr)
		{
			return false;
		}

		return _externalInteger == rhsFoo->_externalInteger &&
			_externalFloat == rhsFoo->_externalFloat &&
			_externalString == rhsFoo->_externalString &&
			_externalVector == rhsFoo->_externalVector &&
			_externalMatrix == rhsFoo->_externalMatrix &&
			_externalRTTI == rhsFoo->_externalRTTI &&
			std::equal(std::begin(_externalIntegerArray), std::end(_externalIntegerArray), std::begin(rhsFoo->_externalIntegerArray)) &&
			std::equal(std::begin(_externalFloatArray), std::end(_externalFloatArray), std::begin(rhsFoo->_externalFloatArray)) &&
			std::equal(std::begin(_externalStringArray), std::end(_externalStringArray), std::begin(rhsFoo->_externalStringArray)) &&
			std::equal(std::begin(_externalVectorArray), std::end(_externalVectorArray), std::begin(rhsFoo->_externalVectorArray)) &&
			std::equal(std::begin(_externalMatrixArray), std::end(_externalMatrixArray), std::begin(rhsFoo->_externalMatrixArray)) &&
			std::equal(std::begin(_externalRTTIArray), std::end(_externalRTTIArray), std::begin(rhsFoo->_externalRTTIArray)) &&

			(*this).At("_nestedScope"s) == (*rhsFoo).At("_nestedScope"s);
	}
}