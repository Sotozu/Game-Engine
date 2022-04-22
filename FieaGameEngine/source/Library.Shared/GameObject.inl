//#include "GameObject.h"
//
//namespace FieaGameEngine
//{
//	template<typename T>
//	T* const GameObject::CreateAction()
//	{
//		if (Action::TypeIdClass() != T::TypeIdClass()) {std::runtime_error throw("Template parameter is not of type Action!") };
//
//		T* myNewAction = new T;
//		
//		Datum* myGameObjectActions = Find("_actions");
//		myGameObjectActions->PushBack(myNewAction);
//
//		return myNewAction;
//	}
//
//}