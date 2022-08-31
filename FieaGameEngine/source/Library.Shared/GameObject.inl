//#pragma once
//#include "GameObject.h"
//
//namespace FieaGameEngine
//{
//
//	template<typename T>
//	void Scope::WorkOnAllChildrenOfType(std::function<void(T&)> func)
//	{
//		T* childType{ nullptr };
//		for (std::size_t index = 0; index < _orderVector.Size(); ++index)
//		{
//			Datum& datum = _orderVector[index]->second;
//
//			if (datum.Type() == DatumTypes::Table)
//			{
//				for (std::size_t indexD = 0; indexD < datum.Size(); ++indexD)
//				{
//					Scope& nestedScope = datum.Get<Scope>(indexD);
//					childType = nestedScope.As<T>();
//					if (childType != nullptr)
//					{
//						func(*childType);
//					}
//
//					nestedScope.WorkOnAllChildrenOfType<T>(func);
//				}
//			}
//
//		}
//	}
//
//	template<typename T>
//	std::vector<T*> Scope::FindAllChildrenOfType()
//	{
//		std::vector<T*> childrenOfType;
//		childrenOfType.reserve(10);
//		T* childType{ nullptr };
//		for (std::size_t index = 0; index < _orderVector.Size(); ++index)
//		{
//			Datum& datum = _orderVector[index]->second;
//
//			if (datum.Type() == DatumTypes::Table)
//			{
//				for (std::size_t indexD = 0; indexD < datum.Size(); ++indexD)
//				{
//					Scope& nestedScope = datum.Get<Scope>(indexD);
//					childType = nestedScope.As<T>();
//					if (childType != nullptr)
//					{
//						childrenOfType.push_back(childType);
//					}
//
//					std::vector<T*> otherChildren = nestedScope.FindAllChildrenOfType<T>();
//
//					for (auto c : otherChildren)
//					{
//						childrenOfType.push_back(c);
//					}
//
//				}
//			}
//
//		}
//
//		return childrenOfType;
//	}
//
//
//	template<typename T>
//	std::vector<const T* const> Scope::FindAllChildrenOfType() const
//	{
//		std::vector<const T* const> childrenOfType;
//		childrenOfType.reserve(10);
//		T* childType{ nullptr };
//		for (std::size_t index = 0; index < _orderVector.Size(); ++index)
//		{
//			Datum& datum = _orderVector[index]->second;
//
//			if (datum.Type() == DatumTypes::Table)
//			{
//				for (std::size_t indexD = 0; indexD < datum.Size(); ++indexD)
//				{
//					Scope& nestedScope = datum.Get<Scope>(indexD);
//					childType = nestedScope.As<T>();
//					if (childType != nullptr)
//					{
//						childrenOfType.push_back(childType);
//					}
//
//					std::vector<const T* const> otherChildren = nestedScope.FindAllChildrenOfType<T>();
//
//					for (auto c : otherChildren)
//					{
//						childrenOfType.push_back(c);
//					}
//
//				}
//			}
//
//		}
//
//		return childrenOfType;
//	}
//
//}