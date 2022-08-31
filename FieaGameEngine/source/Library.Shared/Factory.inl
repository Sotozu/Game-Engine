#pragma once
#include "Factory.h"

namespace FieaGameEngine
{
	template<typename T>
	Factory<T>*  Factory<T>::Find(const std::string* name)
	{
		auto [it, isFound] = _factories.Find(name);
		if (isFound)
		{
			//auto& [associatedName, foundFactory] = *it;
			//return foundFactory;
		}
		return nullptr;
	}

	template<typename T>
	T* Factory<T>::Create(const std::string& name)
	{
		T* createdItem{nullptr};

		auto [it, isFound] = _factories.Find(name);
		if (isFound) 
		{
			auto& [associatedName, foundFactory] = *it;
			createdItem = foundFactory->Create();
		}
		return createdItem;
	}

	template<typename AbstractProductType>
	void Factory<AbstractProductType>::Add(const Factory<AbstractProductType>& factory)
	{
		pairType myPair(factory.ClassName(), &factory);
		_factories.Insert(myPair);
	}

	template<typename AbstractProductType>
	void Factory<AbstractProductType>::Remove(const Factory<AbstractProductType>& factory)
	{
		_factories.Remove(factory.ClassName());
	}

}