#include "TypeRegistry.h"
#include "Attributed.h"
namespace FieaGameEngine
{
	template<typename Class>
	bool TypeRegistry::IsTypeRegistered()
	{
		return GetInstance().IIsTypeRegistered<Class>();
	}

	template<typename Derived, typename Base>
	void TypeRegistry::RegisterType()
	{
		GetInstance().IRegisterType<Derived, Base>();
	}


	template<typename Class>
	void TypeRegistry::RegisterType()
	{
		GetInstance().IRegisterType<Class>();
	}


	template<typename Class>
	bool TypeRegistry::IIsTypeRegistered()
	{
		std::size_t typeId = Class::TypeIdClass();
		auto [it, isFound] = GetInstance()._RegistryUMap.Find(typeId);
		it;
		return isFound;
	}

	template<typename Derived, typename Base>
	void TypeRegistry::IRegisterType()
	{
		if (IsTypeRegistered<Derived>()) { return; } //If the types is already registered then don't bother running the rest of this method.

		//This will be recursive method. Review macro in Attributed header file
		if (!IsTypeRegistered<Base>()){ Base::Register(); }
		
		//const Vector<Signature>& signaturesParent = Base::Signatures(); //Access the static method Signatures from class U
		const Vector<Signature>& signaturesParent = GetSignatures(Base::TypeIdClass()); //Access the static method Signatures from class U
		const Vector<Signature>& signaturesChild = Derived::Signatures(); //Access the static method Signatures from class T


		Vector<Signature> myCombinedSignatures = signaturesParent;

		//Check to see if the parent has any members that have the same name and type. If they do then we need to set the parents attribute offset to the derived class version.
		//this is because they need to refer to the same data. Paul 3-14-2022 11:16AM

		// 04/09/2022 Is this because if a class like a MEAN MONSTER has a health variable (signature) and so does its base class MONSTER. Then we want to only refer to one of these. Specifically the one for the most derived class.
		for (std::size_t indexParent = 0; indexParent < myCombinedSignatures.Size(); ++indexParent)
		{
			for (std::size_t indexChild = 0; indexChild < signaturesChild.Size(); ++indexChild)
			{
				if (myCombinedSignatures[indexParent]._name == signaturesChild[indexChild]._name && myCombinedSignatures[indexParent]._type == signaturesChild[indexChild]._type) //If the name and the type are the same
				{
					myCombinedSignatures[indexParent]._byteOffSet = signaturesChild[indexChild]._byteOffSet;
				}
			}

		}

		myCombinedSignatures.Reserve(signaturesParent.Size() + signaturesChild.Size());

		for (std::size_t index = 0; index < signaturesChild.Size(); ++index)
		{
			myCombinedSignatures.PushBack(signaturesChild[index]);
		}

		//RegisterTypeInformation(T::TypeIdClass(), myCombinedSignatures);//With
		GetInstance()._RegistryUMap.Insert(std::make_pair(Derived::TypeIdClass(), myCombinedSignatures));

	}


	template<typename Class>
	void TypeRegistry::IRegisterType()
	{
		if (IsTypeRegistered<Class>()) 
		{ 
			return; 
		}
		else 
		{ 
			GetInstance()._RegistryUMap.Insert(std::make_pair(Class::TypeIdClass(), Class::Register())); 
		}

	}


	inline void TypeRegistry::RegisterTypeInformation(std::size_t typeId, const Vector<Signature>& signatures)
	{
		pairType myPair(typeId, signatures);
		GetInstance()._RegistryUMap.Insert(myPair);
	}

	inline typename const TypeRegistry::TData& TypeRegistry::GetSignatures(std::size_t typeId)
	{
		return GetInstance().IGetSignatures(typeId);
	}
	inline typename const TypeRegistry::TData& TypeRegistry::IGetSignatures(std::size_t typeId)
	{
		auto [it, isFound] = GetInstance()._RegistryUMap.Find(typeId);
		if (isFound == false) { throw std::runtime_error("Type not registered OR Type has no RTTI."); }
		return (*it).second;
	}

	inline TypeRegistry& TypeRegistry::GetInstance()
	{
		static TypeRegistry _instance;
		return _instance;
	}


	inline void TypeRegistry::RegisterAttributedBaseClass(std::size_t typeId, const Vector<Signature>& signatures) // will be empty and only called once
	{
		GetInstance()._RegistryUMap.Insert(std::make_pair(typeId, signatures));
	}

}
