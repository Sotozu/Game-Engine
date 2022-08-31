#include "pch.h"
#include "DerivedAttributedFoo.h"


using namespace FieaGameEngine;

namespace UnitTestLibraryDesktop
{
	RTTI_DEFINITIONS(DerivedAttributedFoo)

		DerivedAttributedFoo::DerivedAttributedFoo() : AttributedFoo(DerivedAttributedFoo::TypeIdClass())
	{}

	DerivedAttributedFoo::DerivedAttributedFoo(std::size_t typeId) : AttributedFoo(typeId),
		_DAFexternalVector{},
		_DAFexternalMatrix{},
		_DAFexternalIntegerArray{},
		_DAFexternalFloatArray{},
		_DAFexternalVectorArray{},
		_DAFexternalMatrixArray{}
	{}
	const Vector<Signature> DerivedAttributedFoo::Signatures()
	{
		return Vector<Signature>
		{

			{"_DAFexternalInteger", DatumTypes::Integer, 1, offsetof(DerivedAttributedFoo, _DAFexternalInteger)},
			{ "_DAFexternalFloat", DatumTypes::Float, 1, offsetof(DerivedAttributedFoo, _DAFexternalFloat) },
			{ "_DAFexternalString", DatumTypes::String, 1, offsetof(DerivedAttributedFoo, _DAFexternalString) },
			{ "_DAFexternalVector", DatumTypes::Vector4, 1, offsetof(DerivedAttributedFoo, _DAFexternalVector) },
			{ "_DAFexternalMatrix", DatumTypes::Matrix, 1, offsetof(DerivedAttributedFoo, _DAFexternalMatrix) },
			{ "_DAFexternalRTTI", DatumTypes::Pointer, 1, offsetof(DerivedAttributedFoo, _DAFexternalRTTI) },
			{ "_DAFscope", DatumTypes::Table, 0, 0 },
				//{"_classFoo", DatumTypes::Pointer, 1, offsetof(AttributedFoo, myFoo) },

			{ "_DAFexternalIntegerArray", DatumTypes::Integer, ArraySize, offsetof(DerivedAttributedFoo, _DAFexternalIntegerArray) },
			{ "_DAFexternalFloatArray", DatumTypes::Float, ArraySize, offsetof(DerivedAttributedFoo, _DAFexternalFloatArray) },
			{ "_DAFexternalStringArray", DatumTypes::String, ArraySize, offsetof(DerivedAttributedFoo, _DAFexternalStringArray) },
			{ "_DAFexternalVectorArray", DatumTypes::Vector4, ArraySize, offsetof(DerivedAttributedFoo, _DAFexternalVectorArray) },
			{ "_DAFexternalMatrixArray", DatumTypes::Matrix, ArraySize, offsetof(DerivedAttributedFoo, _DAFexternalMatrixArray) },
			{ "_DAFexternalRTTIArray", DatumTypes::Pointer, ArraySize, offsetof(DerivedAttributedFoo, _DAFexternalRTTIArray) },
			{ "_DAFnestedScope", DatumTypes::Table, ArraySize, 0 }
				//{ "_classFooArray", DatumTypes::Pointer, ArraySize, offsetof(AttributedFoo, myFooArray) }


		};
	}

	bool DerivedAttributedFoo::Equals(const RTTI* rhs) const
	{
		const DerivedAttributedFoo* const rhsFoo = rhs->As<DerivedAttributedFoo>();
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

			(*this).At("_nestedScope"s) == (*rhsFoo).At("_nestedScope"s) &&

			_DAFexternalInteger == rhsFoo->_DAFexternalInteger &&
			_DAFexternalFloat == rhsFoo->_DAFexternalFloat &&
			_DAFexternalString == rhsFoo->_DAFexternalString &&
			_DAFexternalVector == rhsFoo->_DAFexternalVector &&
			_DAFexternalMatrix == rhsFoo->_DAFexternalMatrix &&
			_DAFexternalRTTI == rhsFoo->_DAFexternalRTTI &&
			std::equal(std::begin(_DAFexternalIntegerArray), std::end(_DAFexternalIntegerArray), std::begin(rhsFoo->_DAFexternalIntegerArray)) &&
			std::equal(std::begin(_DAFexternalFloatArray), std::end(_DAFexternalFloatArray), std::begin(rhsFoo->_DAFexternalFloatArray)) &&
			std::equal(std::begin(_DAFexternalStringArray), std::end(_DAFexternalStringArray), std::begin(rhsFoo->_DAFexternalStringArray)) &&
			std::equal(std::begin(_DAFexternalVectorArray), std::end(_DAFexternalVectorArray), std::begin(rhsFoo->_DAFexternalVectorArray)) &&
			std::equal(std::begin(_DAFexternalMatrixArray), std::end(_DAFexternalMatrixArray), std::begin(rhsFoo->_DAFexternalMatrixArray)) &&
			std::equal(std::begin(_DAFexternalRTTIArray), std::end(_DAFexternalRTTIArray), std::begin(rhsFoo->_DAFexternalRTTIArray)) &&

			(*this).At("_DAFnestedScope"s) == (*rhsFoo).At("_DAFnestedScope"s);
	}



}