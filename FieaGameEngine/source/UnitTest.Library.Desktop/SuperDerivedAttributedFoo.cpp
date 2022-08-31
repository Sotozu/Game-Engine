#include "pch.h"
#include "SuperDerivedAttributedFoo.h"


using namespace FieaGameEngine;

namespace UnitTestLibraryDesktop
{
	RTTI_DEFINITIONS(SuperDerivedAttributedFoo);

	SuperDerivedAttributedFoo::SuperDerivedAttributedFoo() : DerivedAttributedFoo(SuperDerivedAttributedFoo::TypeIdClass())
	{}
	SuperDerivedAttributedFoo::SuperDerivedAttributedFoo(std::size_t typeId) : DerivedAttributedFoo(typeId),
		_SDAFexternalVector{},
		_SDAFexternalMatrix{},
		_SDAFexternalIntegerArray{},
		_SDAFexternalFloatArray{},
		_SDAFexternalVectorArray{},
		_SDAFexternalMatrixArray{}
	{}

	const Vector<Signature> SuperDerivedAttributedFoo::Signatures()
	{
		return Vector<Signature>
		{

			{"_SDAFexternalInteger", DatumTypes::Integer, 1, offsetof(SuperDerivedAttributedFoo, _SDAFexternalInteger)},
			{ "_SDAFexternalFloat", DatumTypes::Float, 1, offsetof(SuperDerivedAttributedFoo, _SDAFexternalFloat) },
			{ "_SDAFexternalString", DatumTypes::String, 1, offsetof(SuperDerivedAttributedFoo, _SDAFexternalString) },
			{ "_SDAFexternalVector", DatumTypes::Vector4, 1, offsetof(SuperDerivedAttributedFoo, _SDAFexternalVector) },
			{ "_SDAFexternalMatrix", DatumTypes::Matrix, 1, offsetof(SuperDerivedAttributedFoo, _SDAFexternalMatrix) },
			{ "_SDAFexternalRTTI", DatumTypes::Pointer, 1, offsetof(SuperDerivedAttributedFoo, _SDAFexternalRTTI) },
			{ "_SDAFscope", DatumTypes::Table, 0, 0 },
				//{"_classFoo", DatumTypes::Pointer, 1, offsetof(DerivedAttributedFoo, myFoo) },

			{ "_SDAFexternalIntegerArray", DatumTypes::Integer, ArraySize, offsetof(SuperDerivedAttributedFoo, _SDAFexternalIntegerArray) },
			{ "_SDAFexternalFloatArray", DatumTypes::Float, ArraySize, offsetof(SuperDerivedAttributedFoo, _SDAFexternalFloatArray) },
			{ "_SDAFexternalStringArray", DatumTypes::String, ArraySize, offsetof(SuperDerivedAttributedFoo, _SDAFexternalStringArray) },
			{ "_SDAFexternalVectorArray", DatumTypes::Vector4, ArraySize, offsetof(SuperDerivedAttributedFoo, _SDAFexternalVectorArray) },
			{ "_SDAFexternalMatrixArray", DatumTypes::Matrix, ArraySize, offsetof(SuperDerivedAttributedFoo, _SDAFexternalMatrixArray) },
			{ "_SDAFexternalRTTIArray", DatumTypes::Pointer, ArraySize, offsetof(SuperDerivedAttributedFoo, _SDAFexternalRTTIArray) },
			{ "_SDAFnestedScope", DatumTypes::Table, ArraySize, 0 }
				//{ "_classFooArray", DatumTypes::Pointer, ArraySize, offsetof(DerivedAttributedFoo, myFooArray) }


		};
	}

	bool SuperDerivedAttributedFoo::Equals(const RTTI* rhs) const
	{
		const SuperDerivedAttributedFoo* const rhsFoo = rhs->As<SuperDerivedAttributedFoo>();
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

			_SDAFexternalInteger == rhsFoo->_SDAFexternalInteger &&
			_SDAFexternalFloat == rhsFoo->_SDAFexternalFloat &&
			_SDAFexternalString == rhsFoo->_SDAFexternalString &&
			_SDAFexternalVector == rhsFoo->_SDAFexternalVector &&
			_SDAFexternalMatrix == rhsFoo->_SDAFexternalMatrix &&
			_SDAFexternalRTTI == rhsFoo->_SDAFexternalRTTI &&
			std::equal(std::begin(_SDAFexternalIntegerArray), std::end(_SDAFexternalIntegerArray), std::begin(rhsFoo->_SDAFexternalIntegerArray)) &&
			std::equal(std::begin(_SDAFexternalFloatArray), std::end(_SDAFexternalFloatArray), std::begin(rhsFoo->_SDAFexternalFloatArray)) &&
			std::equal(std::begin(_SDAFexternalStringArray), std::end(_SDAFexternalStringArray), std::begin(rhsFoo->_SDAFexternalStringArray)) &&
			std::equal(std::begin(_SDAFexternalVectorArray), std::end(_SDAFexternalVectorArray), std::begin(rhsFoo->_SDAFexternalVectorArray)) &&
			std::equal(std::begin(_SDAFexternalMatrixArray), std::end(_SDAFexternalMatrixArray), std::begin(rhsFoo->_SDAFexternalMatrixArray)) &&
			std::equal(std::begin(_SDAFexternalRTTIArray), std::end(_SDAFexternalRTTIArray), std::begin(rhsFoo->_SDAFexternalRTTIArray)) &&

			(*this).At("_SDAFnestedScope"s) == (*rhsFoo).At("_SDAFnestedScope"s);
	}



}