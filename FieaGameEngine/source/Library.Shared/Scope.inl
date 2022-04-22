#pragma once
#include "Scope.h"

namespace FieaGameEngine
{
	inline Scope* Scope::GetParent() const
	{
		return parentScope;
	}

	inline Datum& Scope::operator[](const std::string& name)
	{
		if (name.empty()) { throw std::runtime_error("Empty string parameter is not allowed."); }
		return Append(name);
	}

	inline Datum& Scope::operator[](uint32_t index)
	{
		Datum& myDatum = _orderVector[index]->second;
		return myDatum;
	}


	inline const Datum& Scope::operator[](uint32_t index) const
	{
		const Datum& myDatum = _orderVector[index]->second;
		return myDatum;
	}

	inline bool Scope::operator!=(const Scope& rhs) const
	{
		return (!operator==(rhs));
	}

	inline gsl::owner<Scope*> Scope::Clone() const
	{
		return new Scope(*this);
	}

	inline std::size_t Scope::NumberOfItems() const
	{
		return _orderVector.Size();
	}

	inline 	std::size_t Scope::NumberOfBuckets() const
	{
		return _UMap.NumBuckets();
	}


	/// <summary>
	/// Returns a percentage value that represents the load factor of the map
	/// </summary>
	/// <returns>Returns a float type that represents the load factor of the map as a percentage.</returns>
	inline float Scope::LoadFactorPercentage() const
	{
		return _UMap.LoadFactorPercentage();
	}

	inline bool Scope::IsDescendantOf(const Scope& scope) const
	{
		return scope.IsAncestorOf(*this);
	}


	inline std::string Scope::ToString() const
	{
		return "Scope";
	}

	template<typename ParentsType>
	inline ParentsType& Scope::GetParent() const
	{
		assert(parentScope != nullptr);
		assert(parentScope->Is(ParentsType::TypeIdClass())); //We are using assert "Is"
		return static_cast<ParentsType&>(*parentScope);
	}
}
