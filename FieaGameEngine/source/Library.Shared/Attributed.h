#pragma once
#include <cstddef>
#include "Vector.h"
#include "Scope.h"
#include "TypeRegistry.h"

namespace FieaGameEngine
{
	/// <summary>
	/// Allows us to create dynamic data structures at run-time that represents the inheriting structure.
	/// Creates the “schema” at compile-time, to mirror native classes.
	/// Does not support the event where a parent and a child or any derived class has an attribute of the same. All Attribute names must be unique.
	/// </summary>
	class Attributed : public Scope
	{
		RTTI_DECLARATIONS(Attributed, Scope)

	public:

		friend class TypeRegistry;

		/// <summary>
		/// Takes a string and checks if it belongs to an attribute that is prescribed to our mirroring scope structure.
		/// </summary>
		/// <param name="name">A string, a name, that we are inquiring if it belongs to a prescribed attribute of our mirroring scope structure.</param>
		/// <returns>
		/// True if a attribute exists within our mirroring scope structure that has the same name AND is prescribed.
		/// False if a no attribute with the name exists in our mirroring scope structure AND/OR if it is not prescribed.
		/// </returns>
		bool IsPrescribedAttribute(const std::string& name) const;

		/// <summary>
		/// Takes a string and checks if it is a attribute of our mirroring scope structure.
		/// </summary>
		/// <param name="name">A string, a name, that we are inquiring if it belongs to a attribute of our mirroring scope structure.</param>
		/// <returns>
		/// True if a attribute exists within our mirroring scope structure that has the same name.
		/// False if a no attribute with the name exists in our mirroring scope structure.
		/// </returns>
		bool IsAttribute(const std::string& name) const;

		/// <summary>
		/// Checks to see if a attribute of our mirroring scope structure is not prescribed. That it is not part of the standard structure for the object.
		/// </summary>
		/// <param name="name">A string, a name, that we are inquiring if it is an auxiliary attribute of our mirroring scope structure.</param>
		/// <returns>
		/// True if a attribute with a matching name is found within the mirroring scope structure AND it is not a prescribed attribute.
		/// False if a attribute is not found with the name AND/OR if it is prescribed attribute.
		/// </returns>
		bool IsAuxilaryAttribute(const std::string& name) const;

		/// <summary>
		/// Adds an attribute that is not prescribed into our mirroring scope structure.
		/// If an attribute with the name passed into the parameter already exists then no attribute is appended and instead a reference to that already existing attribute is returned.
		/// </summary>
		/// <param name="name">The name of the attribute that we wish to add to our mirroring scope structure</param>
		/// <returns>
		/// A reference to a newly appended attribute OR if already existing then a referencer to that one.
		/// </returns>
		Datum& AppendAuxiliaryAttribute(const std::string& name);

		/// <summary>
		/// Generates a list of signatures that represent the Attributes of the object that inherits from this class.
		/// </summary>
		/// <returns>A list of signatures</returns>
		static const Vector<Signature> Signatures();

		/// <summary>
		/// Will be called to register this class with the type registry. 
		/// There is a macro within the the TypeRegistry header file that should be applied to all classes that derive from Attributed.
		/// </summary>
		static void Register();

	protected:

		/// <summary>
		/// TypeCast Constructor
		/// Class is meant to be inherited from.
		/// Inheriting class passes its TypeId so that Attributed can perform calls to the TypeRegistry and construct mirroring scope structure.
		/// </summary>
		/// <param name="typeId">Inheriting class TypeId</param>
		Attributed(std::size_t typeId);

		/// <summary>
		/// virtual delete so that polymorphic destruction can invoke inherited destructor
		/// </summary>
		virtual ~Attributed() = default;
		/// <summary>
		/// Copy Constructor
		/// Creates a copy of an Attributed object
		/// </summary>
		/// <param name="rhs">Reference to an attribute we wish to copy</param>
		Attributed(const Attributed& rhs);

		/// <summary>
		/// Move Constructor
		/// Moves data from one Attribute into another.
		/// </summary>
		/// <param name="rhs">Attributed object we wish to move</param>
		Attributed(Attributed&& rhs) noexcept;

		/// <summary>
		/// Copy Assignment Operator
		/// Copies an Attributed object.
		/// </summary>
		/// <param name="rhs">The Attributed object we wish to copy.</param>
		/// <returns>A reference to itself</returns>
		Attributed& operator=(const Attributed& rhs);
		
		/// <summary>
		/// Move Assignment Operator
		/// Moves data from one Attribute into another.
		/// </summary>
		/// <param name="rhs">Attributed object we wish to move</param>
		/// <returns>A reference to itself</returns>
		Attributed& operator=(Attributed&& rhs) noexcept;

	private:

		/// <summary>
		/// Returns a pair of type UnorderedMap::Iterator and boolean.
		/// The UnorderedMap::Iterator is dereferenced to obtain a pointer to the datum (attribute) it points to.
		/// The boolean is true if datum (attribute) is found. False if the datum (attribute) is not found.
		/// </summary>
		/// <param name="name">The name of the attribute the we are searching for.</param>
		/// <returns>A pair of type UnorderedMap::Iterator and boolean.</returns>
		UnorderedMap<const std::string, Datum>::it_bool_pair IsAttributeInternal(const std::string& name);

		/// <summary>
		/// Re-assigns the external attributes to point to the correct locations
		/// </summary>
		/// <param name="typeId"></param>
		void ReAssignExternalAttributes(std::size_t typeId);

		/// <summary>
		/// String version of this class.
		/// </summary>
		/// <returns>A string representation of this class.</returns>
		std::string ToString() const override;

		/// <summary>
		/// Creates the mirroring scope structure of the class that inherits from Attributed.
		/// </summary>
		/// <param name="typeId">The RTTI typeId of the class we are mirroring</param>
		void PopulatePrescribedAttributes(std::size_t typeId);
		std::size_t _typeId{ 0 };
	};
}
#include "Attributed.inl"