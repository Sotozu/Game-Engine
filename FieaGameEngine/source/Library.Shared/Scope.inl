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


	template<typename T>
	void Scope::WorkOnAllChildrenOfType(std::function<void(T&)> func)
	{
		T* childType{ nullptr };
		for (std::size_t index = 0; index < _orderVector.Size(); ++index)
		{
			Datum& datum = _orderVector[index]->second;

			if (datum.Type() == DatumTypes::Table)
			{
				for (std::size_t indexD = 0; indexD < datum.Size(); ++indexD)
				{
					Scope& nestedScope = datum.Get<Scope>(indexD);
					childType = nestedScope.As<T>();
					if (childType != nullptr)
					{
						func(*childType);
					}
					nestedScope.WorkOnAllChildrenOfType<T>(func);
				}
			}

		}
	}

	template<typename T>
	Vector<T*> Scope::FindAllChildrenOfType()
	{
		Vector<T*> childrenOfType;
		childrenOfType.Reserve(10);
		T* childType{ nullptr };
		for (std::size_t index = 0; index < _orderVector.Size(); ++index)
		{
			Datum& datum = _orderVector[index]->second;

			if (datum.Type() == DatumTypes::Table)
			{
				for (std::size_t indexD = 0; indexD < datum.Size(); ++indexD)
				{
					Scope& nestedScope = datum.Get<Scope>(indexD);
					childType = nestedScope.As<T>();
					if (childType != nullptr)
					{
						childrenOfType.PushBack(childType);
					}

					Vector<T*> otherChildren = nestedScope.FindAllChildrenOfType<T>();

					for (auto c : otherChildren)
					{
						childrenOfType.PushBack(c);
					}

				}
			}

		}

		return childrenOfType;
	}


	template<typename T>
	Vector<const T* const> Scope::FindAllChildrenOfType() const
	{
		Vector<const T* const> childrenOfType;
		childrenOfType.Reserve(10);
		T* childType{ nullptr };
		for (std::size_t index = 0; index < _orderVector.Size(); ++index)
		{
			Datum& datum = _orderVector[index]->second;

			if (datum.Type() == DatumTypes::Table)
			{
				for (std::size_t indexD = 0; indexD < datum.Size(); ++indexD)
				{
					Scope& nestedScope = datum.Get<Scope>(indexD);
					childType = nestedScope.As<T>();
					if (childType != nullptr)
					{
						childrenOfType.PushBack(childType);
					}

					Vector<const T* const> otherChildren = nestedScope.FindAllChildrenOfType<T>();

					for (auto c : otherChildren)
					{
						childrenOfType.PushBack(c);
					}

				}
			}

		}

		return childrenOfType;
	}

}
