#include "pch.h"
#include "Attributed.h"
namespace FieaGameEngine
{
	RTTI_DEFINITIONS(Attributed)

	//Typecast Constructor
	Attributed::Attributed(std::size_t typeId) : _typeId(typeId)
	{
		PopulatePrescribedAttributes(typeId);
	}
	//Copy Constructor
	Attributed::Attributed(const Attributed& rhs) : Scope(rhs)
	{
		ReAssignExternalAttributes(rhs._typeId);
	}

	//Move Constructor
	Attributed::Attributed(Attributed&& rhs) noexcept : Scope(std::move(rhs))
	{
		ReAssignExternalAttributes(rhs._typeId);
	}

	//Copy Assignment
	Attributed& Attributed::operator=(const Attributed& rhs)
	{
		Scope::operator=(rhs);
		ReAssignExternalAttributes(rhs._typeId);
		return *this;
	}

	void Attributed::PopulatePrescribedAttributes(std::size_t typeId)
	{
		//EDIT: 6/21/2022
		//NEEDED TO DO THIS BECAUSE POINTER AND BOOLEAN CONTINUE TO BE MIXED UP BY THE COMPILER.
		Datum& dat = (*this)["this"s];
		dat.SetType(DatumTypes::Pointer);
		dat.PushBack(this);

		//Special datum "this" is assigned in all Scope constructors so there is no need to do it here. Already handled.
		const Vector<Signature>& mySignatures = TypeRegistry::GetSignatures(typeId);

		//iterator through the signatures and construct Scope that represents our object
		for (std::size_t index = 0; index < mySignatures.Size(); ++index)
		{
			Datum& myDatum = Append(mySignatures[index]._name);

			myDatum.SetType(mySignatures[index]._type); //Will throw exception when parent and child have an attribute of the same name but different types.

			if (myDatum.Type() == DatumTypes::Table) //Special case for scopes
			{
				
				if (mySignatures[index]._numElements >= 1) 
				{ 
					myDatum.Reserve(mySignatures[index]._numElements);
				}
			}
			else
			{
				//calculate byte offset
				void* position = reinterpret_cast<uint8_t*>(this) + mySignatures[index]._byteOffSet;

				myDatum.SetStorage(myDatum.Type(), position, mySignatures[index]._numElements);
			}
		}	
	}


	bool Attributed::IsPrescribedAttribute(const std::string& name) const
	{
		bool isFound = false;
		const Vector<Signature>& mySignatures = TypeRegistry::GetSignatures(_typeId);

		if (name == "this") { return true; }

		for (std::size_t index = 0; index < mySignatures.Size(); ++index)
		{
			if (name == mySignatures[index]._name)
			{
				isFound = true;
				break;
			}
		}
		return isFound;
	}





	void Attributed::ReAssignExternalAttributes(std::size_t typeId) 		
	{
		(*this)["this"s] = this;

		const Vector<Signature>& mySignatures = TypeRegistry::GetSignatures(typeId);

		for (std::size_t index = 0; index < mySignatures.Size(); ++index) //iterate through all items in the scope struct except for the first one
		{
			if (index >= 18) //Delete this after testing
			{
				int i = 0;
				i;
				index;
			}
			Datum& myDatum = (*_orderVector[index + 1]).second; //Skip "this" datum at index 0 of the _orderVector
			std::string name = (*_orderVector[index + 1]).first;
			std::string SignatureName = mySignatures[index]._name;
			void* position = reinterpret_cast<uint8_t*>(this) + mySignatures[index]._byteOffSet;
			if (myDatum.Type() != DatumTypes::Table) 
			{ 
				myDatum.SetStorage(myDatum.Type(), position, mySignatures[index]._numElements); 
			}
		}
		/*HOW DO WE REASSIGN EXTERNAL AUXILLARY ATTRIBUTES*/
	}



	void Attributed::Register()
	{
		TypeRegistry::RegisterAttributedBaseClass(Attributed::TypeIdClass(), Signatures());
	}


	

}