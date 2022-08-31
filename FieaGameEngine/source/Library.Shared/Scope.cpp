#include "pch.h"
#include "Scope.h"


namespace FieaGameEngine
{

	RTTI_DEFINITIONS(Scope)

	//Constructor
	Scope::Scope(std::size_t capacity) :_UMap(capacity), _orderVector(capacity), parentScope(nullptr)
	{}

	//Constructor Initializer List
	Scope::Scope(std::initializer_list<pt_string_datum*> list) : _orderVector(list.size())
	{

		for (const auto& value : list)
		{
			Append(*value);
		}
	}

	//Copy Constructor
	Scope::Scope(const Scope& rhs) : _UMap(rhs._UMap.NumBuckets()), _orderVector(rhs._orderVector.Size())
	{

		for (std::size_t index = 0; index < rhs._orderVector.Size(); ++index) //Traverse rhs _orderVector.
		{

			Datum& datum = (*rhs._orderVector[index]).second; //Parse pairs in _orderVector
			const std::string& name = (*rhs._orderVector[index]).first;

			Datum myNewDatum;


			if (datum.Type() == DatumTypes::Table) //If the datum is of type Table (Scope) then we must perform these operation again. Recursive can be made.
			{
				myNewDatum.SetType(DatumTypes::Table); //required to set so that reserve can allocate a datum type size amount
				myNewDatum.Reserve(datum.Size());
				for (std::size_t indexD = 0; indexD < datum.Size(); ++indexD) //For each Scope within the datum we will need to copy construct
				{
					Scope& nestedScope = datum.Get<Scope>(indexD);

					Scope* myNewScope = nestedScope.Clone(); //recursive call of copy constructor. Creates new scope and repeats process.

					(*myNewScope).parentScope = this; //This will execute once the stack begins to unwind itself

					myNewDatum.PushBack(*myNewScope); //Make sure we are pushing back a reference to a scope and not a pointer. Otherwise RTTI version will be invoked.
				}
			}
			else
			{
				myNewDatum = datum; //If not of type Scope then this is an easy copy. BUT NEED TO ASK PAUL ABOUT RTTI POINTERS!
			}

			pt_string_datum myPair(name, myNewDatum); //Create a pair with our newly created datum copy and copy name.
			auto [UMap_It, isFound] = _UMap.Insert(myPair); //Insert the pair into the copy _UMap and retrieve a _UMap Iterator
			_orderVector.PushBack(&*UMap_It); //dereferencing a _UMap iterator will give us a reference to the data in the _UMap that the iterator points to.

		}

	}

	//OO Assignment
	Scope& Scope::operator=(const Scope& rhs)
	{
		Clear();

		_orderVector.Reserve(rhs._orderVector.Size()); //Reserve the number of items present in the rhs _orderVector.
		_UMap.ReHashMap(rhs._UMap.NumBuckets()); //Set the hash map to have the same number of buckets as the rhs

		for (std::size_t index = 0; index < rhs._orderVector.Size(); ++index) //Traverse rhs _orderVector.
		{

			Datum& datum = (*rhs._orderVector[index]).second; //Parse pairs in _orderVector
			const std::string& name = (*rhs._orderVector[index]).first;

			Datum myNewDatum;


			if (datum.Type() == DatumTypes::Table) //If the datum is of type Table (Scope) then we must perform these operation again. Recursive can be made.
			{
				myNewDatum.SetType(DatumTypes::Table); //required to set so that reserve can allocate a datum type size amount
				myNewDatum.Reserve(datum.Size());
				for (std::size_t indexD = 0; indexD < datum.Size(); ++indexD) //For each Scope within the datum we will need to copy construct
				{
					Scope& nestedScope = datum.Get<Scope>(indexD);

					Scope* myNewScope = nestedScope.Clone(); //recursive call of copy constructor. Creates new scope and repeats process.

					(*myNewScope).parentScope = this; //This will execute once the stack begins to unwind itself

					myNewDatum.PushBack(*myNewScope); //Make sure we are pushing back a reference to a scope and not a pointer. Otherwise RTTI version will be invoked.
				}
			}
			else
			{
				myNewDatum = datum; //If not of type Scope then this is an easy copy. BUT NEED TO ASK PAUL ABOUT RTTI POINTERS!
			}

			pt_string_datum myPair(name, myNewDatum); //Create a pair with our newly created datum copy and copy name.
			auto [UMap_It, isFound] = _UMap.Insert(myPair); //Insert the pair into the copy _UMap and retrieve a _UMap Iterator
			_orderVector.PushBack(&*UMap_It); //dereferencing a _UMap iterator will give us a reference to the data in the _UMap that the iterator points to.

		}
		return *this;
	}

	//Move Constructor
	Scope::Scope(Scope&& rhs) noexcept : _UMap(std::move(rhs._UMap)), _orderVector(std::move(rhs._orderVector))
	{
		rhs.Orphan(); //Remove reference of rhs pointer from it's parent.

		for (std::size_t indexV = 0; indexV < _orderVector.Size(); ++indexV)
		{
			Datum& datum = _orderVector[indexV]->second;
			if (datum.Type() == DatumTypes::Table)
			{
				for (std::size_t indexD = 0; indexD < datum.Size(); ++indexD)
				{
					Scope& myScope = datum.Get<Scope>(indexD);
					myScope.parentScope = this;
				}
			}
		}

	}

	//OO Move Assignment
	Scope& Scope::operator=(Scope&& rhs) noexcept
	{

		rhs.Orphan();
		_UMap = std::move(rhs._UMap); //Clears contents of _UMap and then moves contents of other _UMap.
		_orderVector = std::move(rhs._orderVector); //Clears contents of _orderVector and then moves contents of other _orderVector.
		for (std::size_t indexV = 0; indexV < _orderVector.Size(); ++indexV)
		{
			Datum& datum = _orderVector[indexV]->second;
			if (datum.Type() == DatumTypes::Table)
			{
				for (std::size_t indexD = 0; indexD < datum.Size(); ++indexD)
				{
					Scope& myScope = datum.Get<Scope>(indexD);
					myScope.parentScope = this;
				}
			}
		}

		return *this;
	}

	//Destructor
	Scope::~Scope() 
	{
		Orphan();
		Clear();
	}




	//Recursively Clear
	void Scope::Clear()
	{
		for (std::size_t index = 0; index < _orderVector.Size(); ++index) //Traverse _orderVector of Scope
		{
			Datum& datum = (*_orderVector[index]).second;
			if (datum.Type() == DatumTypes::Table) //If it is scope we need to perform this all over again. recursive call will be implemented
			{

				while (!datum.IsEmpty())
				{
					Scope* myScope = &datum.Get<Scope>(0);

					myScope->Orphan(); //every time we orphan the datum will shift the data to a lower index.

					delete myScope;
				}
			}
			else
			{
				datum.Clear();
			}
		}
		_orderVector.Clear();
		_UMap.Clear();
	}


	//Find
	Datum* Scope::Find(const std::string& name)
	{
		if (name.empty()) { throw std::runtime_error("Empty string parameter is not allowed."); }
		auto [UMap_It,isFound] = _UMap.Find(name);

		if (isFound == false) { return nullptr; }

		Datum& myDatum = (*UMap_It).second;

		return &myDatum;
	}

	//Find Const
	const Datum* Scope::Find(const std::string& name) const
	{
		if (name.empty()) { throw std::runtime_error("Empty string parameter is not allowed."); }
		auto [It, isFound] = _UMap.Find(name);

		if (isFound == false) { return nullptr; }

		const Datum& datum = (*It).second;

		return &datum;
	}

#pragma region SearchDown
	Datum* Scope::SearchDown(const std::string& name, Scope** searchScope) 
	{
		if (name.empty()) { throw std::runtime_error("Empty string parameter is not allowed."); }
		*searchScope = this;
		Datum* foundDatum = Find(name);
		if (foundDatum == nullptr)
		{
			for (std::size_t index = 0; index < _orderVector.Size(); ++index)
			{
				Datum& datum = _orderVector[index]->second;
				if (datum.Type() == DatumTypes::Table)
				{
					for (std::size_t indexD = 0; indexD < datum.Size(); ++indexD)
					{
						Scope& nestedScope = datum.Get<Scope>(indexD);

						foundDatum = nestedScope.SearchDown(name, searchScope);
						if (foundDatum != nullptr) { return foundDatum; };
					}
				}
			}
		}
		if (foundDatum == nullptr) { *searchScope = nullptr; }
		return foundDatum;
	}

	const Datum* Scope::SearchDown(const std::string& name, const Scope** searchScope) const
	{
		if (name.empty()) { throw std::runtime_error("Empty string parameter is not allowed."); }
		*searchScope = this;
		const Datum* foundDatum = Find(name);
		if (foundDatum == nullptr)
		{
			for (std::size_t index = 0; index < _orderVector.Size(); ++index)
			{
				Datum& datum = _orderVector[index]->second;
				if (datum.Type() == DatumTypes::Table)
				{
					for (std::size_t indexD = 0; indexD < datum.Size(); ++indexD)
					{
						Scope& nestedScope = datum.Get<Scope>(indexD);

						foundDatum = nestedScope.SearchDown(name, searchScope);
						if (foundDatum != nullptr) 
						{ 
							return foundDatum; 
						};
					}
				}
			}
		}

		if (foundDatum == nullptr) {*searchScope = nullptr;}
		return foundDatum;
	}


	Datum* Scope::SearchDown(const std::string& name)
	{
		if (name.empty()) { throw std::runtime_error("Empty string parameter is not allowed."); }
		Scope* dummyScope = nullptr;
		dummyScope;
		return SearchDown(name, &dummyScope);
	}

	const Datum* Scope::SearchDown(const std::string& name) const
	{
		if (name.empty()) { throw std::runtime_error("Empty string parameter is not allowed."); }
		const Scope* dummyScope;
		dummyScope;
		return SearchDown(name, &dummyScope);

	}
#pragma endregion

#pragma region SearchUp
	Datum* Scope::SearchUp(const std::string& name, Scope** searchScope)
	{

		if (name.empty()) { throw std::runtime_error("Empty string parameter is not allowed."); }
		*searchScope = this;
		Datum* foundDatum = Find(name);

		if (foundDatum == nullptr && parentScope != nullptr) //if not found in initial search then go up the hierarchy and Search
		{
			foundDatum = parentScope->SearchUp(name, searchScope);
		}

		if (foundDatum == nullptr) { *searchScope = nullptr; }
		return foundDatum;
	}

	const Datum* Scope::SearchUp(const std::string& name, const Scope** searchScope) const
	{
		if (name.empty()) { throw std::runtime_error("Empty string parameter is not allowed."); }
		*searchScope = this;
		const Datum* foundDatum = Find(name);

		if (foundDatum == nullptr && parentScope != nullptr) //if not found in initial search then go up the hierarchy and Search
		{
			foundDatum = parentScope->SearchUp(name, searchScope);
		}

		if (foundDatum == nullptr) { *searchScope = nullptr; }
		return foundDatum;
	}

	Datum* Scope::SearchUp(const std::string& name)
	{
		if (name.empty()) { throw std::runtime_error("Empty string parameter is not allowed."); }
		Scope* dummyScope = nullptr;
		dummyScope;
		return SearchUp(name, &dummyScope);
	}

	const Datum* Scope::SearchUp(const std::string& name) const
	{
		if (name.empty()) { throw std::runtime_error("Empty string parameter is not allowed."); }
		const Scope* dummyScope;
		dummyScope;
		return SearchUp(name, &dummyScope);

	}
#pragma endregion

	Datum& Scope::Append(const std::string& name)
	{
		if (name.empty()) { throw std::runtime_error("Empty string parameter is not allowed."); }
		Datum myDatum;
		pt_string_datum appendPair(name, myDatum); //Attempts to append default constructed datum
		
		auto[UMap_It, isFound] = _UMap.Insert(appendPair); //Will return a reference of newly inserted pair OR if an item already exists it will return a reference to that.
														   //We can know if we inserted a new item by the isFound boolean.

		Datum& datum = (*UMap_It).second;
		if (!isFound) //If it was NOT found then we need to insert
		{
			_orderVector.PushBack(&(*UMap_It));
		}
		return datum;
	}


	Datum& Scope::Append(const std::string& name, Datum& datum)
	{
		if (name.empty()) { throw std::runtime_error("Empty string parameter is not allowed."); }

		pt_string_datum appendPair(name, datum); //Attempts to append default constructed datum

		auto [UMap_It, isFound] = _UMap.Insert(appendPair); //Will return a reference of newly inserted pair OR if an item already exists it will return a reference to that.
														   //We can know if we inserted a new item by the isFound boolean.

		if (!isFound) //If it was NOT found then we need to insert
		{
			_orderVector.PushBack(&(*UMap_It));
			if (datum.Type() == DatumTypes::Table)
			{
				for (std::size_t index = 0; index < datum.Size(); ++index) //Make sure that all scopes appended have their parent scope set to this
				{
					Scope& myScope = datum.Get<Scope>(index);
					myScope.parentScope = this;
				}
			}

		}

		return (*UMap_It).second;
	}

	Datum& Scope::Append(pt_string_datum& pair)
	{
		if ((pair.first).empty()) { throw std::runtime_error("Empty string parameter is not allowed."); }

		auto [UMap_It, isFound] = _UMap.Insert(pair); //Will return a reference of newly inserted pair OR if an item already exists it will return a reference to that.
														   //We can know if we inserted a new item by the isFound boolean.

		if (!isFound) //If it was NOT found then we need to insert
		{
			_orderVector.PushBack(&(*UMap_It));

			Datum& datum = pair.second;
			if (datum.Type() == DatumTypes::Table)
			{
				for (std::size_t index = 0; index < datum.Size(); ++index) //Make sure that all scopes appended have their parent scope set to this
				{
					Scope& myScope = datum.Get<Scope>(index);
					myScope.parentScope = this;
				}
			}
		}

		return (*UMap_It).second;
	}


	//AppendScope
	Scope& Scope::AppendScope(const std::string& name)
	{
		if (name.empty()) { throw std::runtime_error("Empty string parameter is not allowed."); }

		Datum* myDatum;
		myDatum = Find(name);
		if (myDatum != nullptr)  //found datum with the name/key
		{
			if (myDatum->Type() != DatumTypes::Unknown && myDatum->Type() != DatumTypes::Table) { throw std::runtime_error("Datum type is not Table."); }

			Scope* myScope = new Scope; //Heap allocate scope
			myScope->parentScope = this; //Assign its parent
			myDatum->PushBack(*myScope); //PushBack to datum. Make sure we are passing a reference to a scope and not just a pointer. Reason being that otherwise RTTI will be invoked.
			return *myScope;
			
		}
		else //Create scope and append it
		{
			Scope* myScope = new Scope; //Heap allocate scope
			myScope->parentScope = this; //Assign its parent
			pt_string_datum appendScopePair(name, *myScope); //Make sure that datum is taking in a scope reference and not a pointer. Otherwise it will treat it as RTTI

			auto [UMap_It, isFound] = _UMap.Insert(appendScopePair);
			_orderVector.PushBack(&*UMap_It);
			
			return *myScope;
		}
	}

	void Scope::Adopt(const std::string& name, Scope& scope)
	{
		if (name.empty()) { throw std::runtime_error("Empty string parameter is not allowed."); }

		if (this != scope.GetParent()) //If scope is already adopted in this scope DO NOTHING
		{
			if (this != &scope) //Adopting self is not allowed DO NOTHING
			{
				Datum* myDatum;
				myDatum = Find(name);


				if (myDatum != nullptr)  //found datum with the name/key
				{
					if (myDatum->Type() != DatumTypes::Unknown && myDatum->Type() != DatumTypes::Table) { throw std::runtime_error("Datum type is not Table."); }

					scope.Orphan();

					scope.parentScope = this;
					myDatum->PushBack(scope);
						
				}
				else
				{
					scope.Orphan();

					scope.parentScope = this; //Assign its parent

					pt_string_datum appendScopePair(name, scope); //Create pair

					auto [UMap_It, isFound] = _UMap.Insert(appendScopePair);
					_orderVector.PushBack(&*UMap_It);
				}

			}

		}
	}


	

	


	bool Scope::operator==(const Scope& rhs) const
	{
		if (_orderVector.Size() != rhs._orderVector.Size()) { return false; } //early out

		for (std::size_t index = 0; index < _orderVector.Size(); ++index)
		{

			const std::string& rhsName = (rhs._orderVector[index])->first;
			const Datum& rhsDatum = (rhs._orderVector[index])->second;


			auto [it, isFound] = _UMap.Find(rhsName); //Within our UMap determine if a key exists that is found in the rhs vector
			if (isFound == false) { return false; } //If the key does not exist then these are not equivalent

													//Now let's see if the data that the key in our UMap points too is equivalent to the one associated to with the rhs vector
			
			const Datum& lhsDatum = (*it).second;

			if (lhsDatum != rhsDatum) { return false; }

			
		}
		return true;
	}

	bool Scope::Equals(const RTTI* rhs) const
	{		
		if (rhs->Is(Scope::TypeIdClass()))
		{
			const Scope& myScope = reinterpret_cast<const Scope&>(*rhs);
			return operator==(myScope);
		}
		return false;
	}

	typename Scope::pt_scopeRef_bool Scope::Orphan()
	{
		pt_scopeRef_bool NotFoundPair(*this, false); //Returns a reference to this scope and a bool if it indeed orphaned

		Scope* myParentScope = GetParent();

		if (myParentScope == nullptr) { return NotFoundPair; }; //If there is no parent then return

		auto [datumPtr, index] = myParentScope->FindContainedScope(*this); //Otherwise we must go to the parent and remove this scope
																		   //Returns a pointer to the datum and index to where this scope is found

		Scope& returnScope = (*datumPtr).Get<Scope>(index);//Get a pointer to ourselves

		this->parentScope = nullptr; //Set this scopes parentScope pointer to nullptr.

		(*datumPtr).RemoveAt(index); //Remove a reference this scope from our parent. That way the parent can't call delete on this scope. Officially removed from parent.
		pt_scopeRef_bool isFoundPair(returnScope, true); //Returns a reference to this scope and a bool if it indeed orphaned

		return isFoundPair;

	}

	typename Scope::pt_datumPtr_index Scope::FindContainedScope(const Scope& scope)
	{
		//Search the _orderVector for Datums of type Table
		pt_datumPtr_index myPair(nullptr, 0);

		if (scope == *this) { return myPair; } //early out
	
		for (std::size_t indexV = 0; indexV < _orderVector.Size(); ++indexV)
		{
			Datum& datum = _orderVector[indexV]->second;
			if (datum.Type() == DatumTypes::Table) //Iterate and compare scopes in the datum of type table
			{
				auto [ptr_value, index] = datum.Find(scope);
				if (ptr_value != nullptr)
				{
					myPair.first = &datum;
					myPair.second = index;
					return myPair;
				}
			}
		}
		return myPair;
	}

	typename Scope::pt_constDatumPtr_index Scope::FindContainedScope(const Scope& scope) const
	{
		//Search the _orderVector for Datums of type Table
		pt_constDatumPtr_index myPair(nullptr, 0);

		if (scope == *this) { return myPair; } //early out

		for (std::size_t indexV = 0; indexV < _orderVector.Size(); ++indexV)
		{
			const Datum& datum = _orderVector[indexV]->second;
			if (datum.Type() == DatumTypes::Table) //Iterate and compare scopes in the datum of type table
			{
				auto [ptr_value, index] = datum.Find(scope);
				if (ptr_value != nullptr)
				{
					myPair.first = &datum;
					myPair.second = index;
					return myPair;
				}
			}
		}
		return myPair;
	}


	/// <summary>
	/// Rehashes the Unordered Map in the Scope that is used to store the items in Scope. Preserves order of insert.
	/// </summary>
	/// <param name="size">Number of buckets to rehash the map to.</param>
	void Scope::ReHashMap(std::size_t size)
	{
		size;
		Datum names; //Holds keys/names to datums in order

		for (std::size_t index = 0; index < _orderVector.Size(); ++index)
		{
			std::string keyName = _orderVector[index]->first;
			names.PushBack(keyName);
		}

		_UMap.ReHashMap(size);

		for (std::size_t index = 0; index < names.Size(); ++index)
		{
			auto [it, isFound]  = _UMap.Find(names.Get<std::string>(index));

			_orderVector[index] = &*it; //places found pair reference into order vector position
		}
	}

	

	bool Scope::IsAncestorOf(const Scope& scope) const
	{
		bool result = false;
		Scope* parent = scope.parentScope;
		while (parent != nullptr)
		{
			if (parent == this)
			{
				result = true;
				break;
			}
			parent = parent->parentScope;
		}

		return result;
	}

	Datum& Scope::At(const std::string& string)
	{
		Datum* datum = Find(string);
		if (datum == nullptr)
		{
			throw std::runtime_error("Expected entry.");
		}

		return *datum;
	}

	const Datum& Scope::At(const std::string& string) const
	{
		const Datum* datum = Find(string);
		if (datum == nullptr)
		{
			throw std::runtime_error("Expected entry.");
		}

		return *datum;
	}

}
