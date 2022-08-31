#include "Attributed.h"

namespace FieaGameEngine 
{
	inline std::string Attributed::ToString() const { return "Attributed"; }
	inline UnorderedMap<const std::string, Datum>::it_bool_pair Attributed::IsAttributeInternal(const std::string& name) { return _UMap.Find(name); }
	inline const Vector<Signature> Attributed::Signatures() { return Vector<Signature> { }; }

	inline bool Attributed::IsAttribute(const std::string& name) const
	{
		auto [it, isFound] = _UMap.Find(name);
		return isFound;
	}

	inline bool Attributed::IsAuxilaryAttribute(const std::string& name) const
	{
		if (!IsAttribute(name)) { return false; } //if it does not exist in the scope then false
		if (IsPrescribedAttribute(name)) { return false; } //It exists in the scope...now is it prescribed. If it prescribed return false?
		return true;
	}

	inline Datum& Attributed::AppendAuxiliaryAttribute(const std::string& name)
	{
		if (IsPrescribedAttribute(name)) { throw std::runtime_error("Cannot overwrite prescribed attribute with auxiliary attribute."); }
		auto [it, isFound] = IsAttributeInternal(name); //it is an iterator that when dereferenced returns a pair of type string and datum
		if (isFound) { return (*it).second; } //returns reference to attribute with the same name that already exists.
		else { return Append(name); }
	}


	inline Attributed& Attributed::operator=(Attributed&& rhs) noexcept
	{
		Scope::operator=(std::move(rhs));
		ReAssignExternalAttributes(rhs._typeId);
		return *this;
	}


}