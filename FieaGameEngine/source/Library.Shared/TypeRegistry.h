#pragma once
#include "Vector.h"
#include "Datum.h"
#include "RTTI.h"
#include "Scope.h"
#include "UnorderedMap.h"
#include "Attributed.h"

namespace FieaGameEngine
{
	/// <summary>
	/// A struct that contains the data required for our internal scope to mirror one element of data of the class it is being derived from.
	/// </summary>
	struct Signature
	{
		std::string _name;
		DatumTypes _type;
		std::size_t _numElements;
		std::size_t _byteOffSet;
	};

	/// <summary>
	/// A singleton that contains a list of signatures for each class that our engine will use.
	/// Any new classes that our engine will work with and are derived from Scope-Attribute must be registered with the TypeRegistry
	/// </summary>
	class TypeRegistry final
	{
		/// <summary>
		/// Used for accessing the method RegisterAttributedBaseClass();
		/// </summary>
		

	public:
		friend class Attributed;

		using TKey = typename RTTI::IdType;
		using TData = typename Vector<Signature>;
		using pairType = typename std::pair<TKey, TData>;

		/// <summary>
		/// The simplified publicly accessible method that calls the more complex internal version and implementation of what this function does.
		/// T is the class (Type) that is going to be looked for within the TypeRegistry.
		/// </summary>
		/// <typeparam name="T">The class is being asked if is registered.</typeparam>
		/// <returns>
		/// True if the type is registered.
		/// False if the type is NOT registered.
		/// </returns>
		template<typename Class>
		static bool IsTypeRegistered();

		/// <summary>
		/// The simplified publicly accessible method that calls the more complex internal version and implementation of what this function does.
		/// T is the class (Type) that is going to be registered with the TypeRegistry.
		/// </summary>
		/// <typeparam name="T">The class (Type) that is going to be registered with the TypeRegistry.</typeparam>
		template<typename Derived, typename Base>
		static void RegisterType();


		template<typename Class>
		static void RegisterType();


		/// <summary>
		/// Retrieves a reference to a Vector of type Signature for the type.
		/// This information represented the prescribed members of the type.
		/// </summary>
		/// <param name="typeId">The identifier and key required to search the Unordered map for it's associated data, being the Vector of type Signature.</param>
		/// <returns>
		/// A reference to a Vector Signature containing the information of the prescribed members of the associated class.
		/// </returns>
		static const TData& GetSignatures(std::size_t typeId);


	private:
		static TypeRegistry& GetInstance();

		UnorderedMap<TKey, TData> _RegistryUMap; //I experienced issues making this static. Ask Paul about this.

		TypeRegistry() = default;
		TypeRegistry(const TypeRegistry&) = delete; //Don't want the TypeRegistry to be copyable
		TypeRegistry& operator=(const TypeRegistry&) = delete; //Don't want the TypeRegistry to be assigned

		/// <summary>
		/// Holds TypeId and Vector of type Signature pairs.
		/// Container that holds lists of prescribed members for each class.
		/// Is used to create mirroring scopes structures of classes.
		/// </summary>

		/// <summary>
		/// Places key and data into the UnorderedMap.
		/// This will associate a classes ID with it's list of prescribed members.
		/// </summary>
		/// <param name="typeId">A classes unique identifier</param>
		/// <param name="signatures">A list of signatures that describe the data layout of the class.</param>
		static void RegisterTypeInformation(std::size_t typeId, const Vector<Signature>& signatures);



		/// <summary>
		/// The simplified publicly accessible method that calls the more complex internal version and implementation of what this function does.
		/// T is the class (Type) that is going to be looked for within the TypeRegistry.
		/// </summary>
		/// <typeparam name="T">The class is being asked if is registered.</typeparam>
		/// <returns>
		/// True if the type is registered.
		/// False if the type is NOT registered.
		/// </returns>
		template<typename Class>
		static bool IIsTypeRegistered();

		/// <summary>
		/// The simplified publicly accessible method that calls the more complex internal version and implementation of what this function does.
		/// T is the class (Type) that is going to be registered with the TypeRegistry.
		/// </summary>
		/// <typeparam name="T">The class (Type) that is going to be registered with the TypeRegistry.</typeparam>
		template<typename T, typename U>
		static void IRegisterType();


		/// <summary>
		/// The simplified publicly accessible method that calls the more complex internal version and implementation of what this function does.
		/// T is the class (Type) that is going to be registered with the TypeRegistry.
		/// </summary>
		/// <typeparam name="T">The class (Type) that is going to be registered with the TypeRegistry.</typeparam>
		template<typename Class>
		static void IRegisterType(); //TO DO: Need to document this.


		static const TData& IGetSignatures(std::size_t typeId);

		//Only called by the Attributed Base Class
		static void RegisterAttributedBaseClass(std::size_t, const Vector<Signature>&);
	};

//All classes that derive from Attributed will require This macro. EXCEPT Attributed itself.
//Place in the private section of classes.

#define StaticRegisterMethod(Derived, Base)\
	static Vector<Signature> Register()\
	{\
		if (!TypeRegistry::IsTypeRegistered<Base>()) { Base::Register(); }\
		TypeRegistry::RegisterType<Derived, Base>();\
		return TypeRegistry::GetSignatures(Derived::TypeIdClass());\
	}\

}

#include "TypeRegistry.inl"

