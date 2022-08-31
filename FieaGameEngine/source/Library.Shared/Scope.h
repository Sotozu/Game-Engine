#pragma once
#include <initializer_list>
#include <string>
#include <gsl/pointers>
#include <functional>
#include "Datum.h"
#include "UnorderedMap.h"
#include "Vector.h"
#include "RTTI.h"
#include <vector>

using namespace std::string_literals; //Should I place this in the pch.h file? string literals were not working before until wrote this.

namespace FieaGameEngine {

	class Datum;
	
	class Scope : public RTTI
	{
		friend class Attributed;
		RTTI_DECLARATIONS(Scope, RTTI);
	public:

		using pt_string_datum = typename std::pair<const std::string, Datum>;
		using pt_datumPtr_index = typename std::pair<Datum*, std::size_t>;
		using pt_constDatumPtr_index = typename std::pair<const Datum*, std::size_t>;
		using pt_scopeRef_bool = typename std::pair<Scope&, bool>;
	public:

		virtual ~Scope() override;

		/// <summary>
		/// Typecast Constructor
		/// Sets the capacity of the _UMap and _orderVector
		/// Sets the parentScope to nullptr;
		/// </summary>
		/// <param name="size"></param>
		Scope(std::size_t capacity = 11);

		/// <summary>
		/// Appends a pair of type string and datum
		/// If any of the names repeat in the pairs then it will insert the first pair in the list with the name and ignore the others.
		/// </summary>
		/// <param name="list">pairs of string datum</param>
		Scope(std::initializer_list<pt_string_datum*> list);

		/// <summary>
		/// Copy Constructor
		/// Recursively creates the scope and all scopes associated with the passed in parameter.
		/// </summary>
		/// <param name="rhs">A scope whos contents will be copied.</param>
		Scope(const Scope& rhs);

		/// <summary>
		/// OO Assignment
		/// Will make a copy of the Scope passed in.
		/// </summary>
		/// <param name="rhs">A reference to a scope that we wish to copy</param>
		/// <returns>
		/// A reference to itself
		/// </returns>
		Scope& operator=(const Scope& rhs);

		/// <summary>
		/// Move Constructor.
		/// Takes the contents of the rhs and takes ownership of heap allocated data and shallows copies data. Zeros out passed in Scope.
		/// </summary>
		/// <param name="rhs">A rvalue reference to a scope whose ownership will be handled by the constructing scope</param>
		Scope(Scope&& rhs) noexcept;
		
		/// <summary>
		/// Move Assignment
		/// Clears itself of current held data.
		/// Takes the contents of the rhs and takes ownership of heap allocated data and shallow copies data. Zeros out passed in Scope.
		/// </summary>
		/// <param name="rhs">A rvalue reference to a scope whose ownership will be handled by the constructing scope</param>
		/// <returns>
		/// A reference to itself.
		/// </returns>
		Scope& operator=(Scope&& rhs) noexcept;

		/// <summary>
		/// Will remove all content from the scope and all nested scopes.
		/// </summary>
		void Clear();

		/// <summary>
		/// Performs a local search in the Scope. 
		/// Finds a Datum with the associated name passed into the method. 
		/// </summary>
		/// <param name="name">The name of the (name, datum) pair we are looking for.</param>
		/// <returns>
		/// If found returns a valid pointer to a mutable datum within the Scope with the associated name.
		/// If NOT found returns nullptr.
		/// </returns>
		Datum* Find(const std::string& name);

		/// <summary>
		/// Performs a local search in the Scope. 
		/// Finds a Datum with the associated name passed into the method. 
		/// </summary>
		/// <param name="name">The name of the (name, datum) pair we are looking for.</param>
		/// <returns>
		/// If found returns a valid pointer to a CONST datum within the Scope with the associated name.
		/// If NOT found returns nullptr.
		/// </returns>
		const Datum* Find(const std::string& name) const;

		/// <summary>
		/// Searches DOWN the hierarchy of scope for a datum associated with the name passed in. Returns back the first datum found with the associated name.
		/// </summary>
		/// <param name="name">he name of the (name, datum) pair we are looking for.</param>
		/// <param name="foundInScope">A pointer to a pointer. This will point to the Scope POINTER that has our found datum.</param>
		/// <returns>
		/// IF FOUND: Passed in parameter foundInScope will return a valid pointer to the pointer of a scope that holds the found datum.
		/// IF FOUND: Method returns a valid pointer to a mutable datum within the Scope with the associated name.
		/// IF NOT FOUND: Passed in parameter foundInScope will contain nullptr;
		/// IF NOT FOUND: Method returns a nullptr.
		/// </returns>
		Datum* SearchDown(const std::string& name, Scope** foundInScope);
		
		/// <summary>
		/// Searches DOWN the hierarchy of scope for a datum associated with the name passed in. Returns back the first datum found with the associated name.
		/// </summary>
		/// <param name="name">he name of the (name, datum) pair we are looking for.</param>
		/// <param name="foundInScope">A pointer to a pointer. This will point to the Scope POINTER that has our found datum.</param>
		/// <returns>
		/// IF FOUND: Passed in parameter foundInScope will return a valid pointer to the pointer of a scope that holds the found datum.
		/// IF FOUND: Method returns a valid pointer to a const datum within the Scope with the associated name.
		/// IF NOT FOUND: Passed in parameter foundInScope will contain nullptr;
		/// IF NOT FOUND: Method returns a nullptr.
		/// </returns>
		const Datum* SearchDown(const std::string& name, const Scope** foundInScope) const;
		
		/// <summary>
		/// Searches DOWN the hierarchy of scope for a datum associated with the name passed in.
		/// </summary>
		/// <param name="name">he name of the (name, datum) pair we are looking for.</param>
		/// <returns>
		/// IF FOUND: Method returns a valid pointer to a mutable datum within the Scope with the associated name.
		/// IF NOT FOUND: Method returns a nullptr.
		/// </returns>
		Datum* SearchDown(const std::string& name);

		/// <summary>
		/// Searches DOWN the hierarchy of scope for a datum associated with the name passed in. Returns back the first datum found with the associated name.
		/// </summary>
		/// <param name="name">he name of the (name, datum) pair we are looking for.</param>
		/// <returns>
		/// IF FOUND: Method returns a valid pointer to a const datum within the Scope with the associated name.
		/// IF NOT FOUND: Method returns a nullptr.
		/// </returns>
		const Datum* SearchDown(const std::string& name) const;

		/// <summary>
		/// Searches UP the hierarchy of scope for a datum associated with the name passed in. Returns back the first datum found with the associated name.
		/// </summary>
		/// <param name="name">he name of the (name, datum) pair we are looking for.</param>
		/// <param name="foundInScope">A pointer to a pointer. This will point to the Scope POINTER that has our found datum.</param>
		/// <returns>
		/// IF FOUND: Passed in parameter foundInScope will return a valid pointer to the pointer of a scope that holds the found datum.
		/// IF FOUND: Method returns a valid pointer to a mutable datum within the Scope with the associated name.
		/// IF NOT FOUND: Passed in parameter foundInScope will contain nullptr;
		/// IF NOT FOUND: Method returns a nullptr.
		/// </returns>
		Datum* SearchUp(const std::string& name, Scope** foundInScope);

		/// <summary>
		/// Searches UP the hierarchy of scope for a datum associated with the name passed in. Returns back the first datum found with the associated name.
		/// </summary>
		/// <param name="name">he name of the (name, datum) pair we are looking for.</param>
		/// <param name="foundInScope">A pointer to a pointer. This will point to the Scope POINTER that has our found datum.</param>
		/// <returns>
		/// IF FOUND: Passed in parameter foundInScope will return a valid pointer to the pointer of a scope that holds the found datum.
		/// IF FOUND: Method returns a valid pointer to a const datum within the Scope with the associated name.
		/// IF NOT FOUND: Passed in parameter foundInScope will contain nullptr;
		/// IF NOT FOUND: Method returns a nullptr.
		/// </returns>
		const Datum* SearchUp(const std::string& name, const Scope** foundInScope) const;

		/// <summary>
		/// Searches UP the hierarchy of scope for a datum associated with the name passed in.
		/// </summary>
		/// <param name="name">he name of the (name, datum) pair we are looking for.</param>
		/// <returns>
		/// IF FOUND: Method returns a valid pointer to a mutable datum within the Scope with the associated name.
		/// IF NOT FOUND: Method returns a nullptr.
		/// </returns>
		Datum* SearchUp(const std::string& name);

		/// <summary>
		/// Searches UP the hierarchy of scope for a datum associated with the name passed in. Returns back the first datum found with the associated name.
		/// </summary>
		/// <param name="name">he name of the (name, datum) pair we are looking for.</param>
		/// <returns>
		/// IF FOUND: Method returns a valid pointer to a const datum within the Scope with the associated name.
		/// IF NOT FOUND: Method returns a nullptr.
		/// </returns>
		const Datum* SearchUp(const std::string& name) const;

		/// <summary>
		/// Takes a string name and attempts to insert a pair of string datum where the datum is defaulted into the Scope. 
		/// If pair exists with the name passed into the parameter it will return a reference to the datum associated with the name.
		/// If this is a unique name then it will return a reference to the defaulted datum associated with the name and update the order vector.
		/// </summary>
		/// <param name="name">This is the name we wish to append into our scope.</param>
		/// <returns>
		/// If name already exists nothing is appended and a reference to the data associated with the name is returned.
		/// Returns a referenced to a newly created and defaulted datum that's associated with the newly inserted name.
		/// </returns>
		Datum& Append(const std::string& name);

		/// <summary>
		/// Takes a string name and datum. Attempts to insert a pair of string datum where the datum the one passed in to the method. 
		/// If pair exists with the name passed into the parameter it will return a reference to the datum associated with the name.
		/// If this is a unique name then it will return a reference to the datum that was passed in and stored in the scope.
		/// </summary>
		/// <param name="name">This is the name we wish to append into our scope.</param>
		/// <param name="Datum">This is the datum that we wish to insert into the scope.</param>
		/// <returns>
		/// If name already exists nothing is appended and a reference to the data associated with the name is returned.
		/// Returns a referenced to a newly created and defaulted datum that's associated with the newly inserted name.
		/// </returns>
		Datum& Append(const std::string& name, Datum& datum);

		/// <summary>
		/// Takes a pair of (string, datum). Attempts to insert a pair into the scope.
		/// If this is a unique pair with it's name then it will return a reference to the datum that was passed in and stored in the scope.
		/// </summary>
		/// <param name="pair">pair of (string, datum)</param>
		/// <returns>
		/// If pairs string name already exists nothing is appended and a reference to the data associated with the name is returned.
		/// Returns a referenced to a newly created and defaulted datum that's associated with the newly inserted name.
		/// </returns>
		Datum& Append(pt_string_datum& pair);
	
		/// <summary>
		/// Takes a constant string and returns a reference to a Scope with the associated name.
		/// If scope does not exist then one is created. Otherwise returns reference to one found.
		/// </summary>
		/// <param name="name">Name associated with the scope we are attemptign to append into the scope.</param>
		/// <returns>
		/// A reference to scope created/found.
		/// </returns>
		Scope& AppendScope(const std::string& name);

		/// <summary>
		///  Adopts a scope into another scope.
		/// </summary>
		/// <param name="name">A string (the name of key for the Datum to use for storing the child)</param>
		/// <param name="scope">A reference to a Scope (the child to adopt)</param>
		void Adopt(const std::string& name, Scope& scope);

		/// <summary>
		/// Returns a pointer to the parent of the scope. If one does not exist it returns nullptr.
		/// </summary>
		/// <returns>
		/// If scope has a parent then it returns a valid pointer to the parent.
		/// If scope has NO parent then it returns nullptr.
		/// </returns>
		Scope* GetParent() const;

		/// <summary>
		/// Takes a string name and attempts to insert a pair of string datum where the datum is defaulted into the Scope. 
		/// If pair exists with the name passed into the parameter it will return a reference to the datum associated with the name.
		/// If this is a unique name then it will return a reference to the defaulted datum associated with the name and update the order vector.
		/// </summary>
		/// <param name="name">This is the name we wish to append into our scope.</param>
		/// <returns>
		/// If name already exists nothing is appended and a reference to the data associated with the name is returned.
		/// Returns a referenced to a newly created and defaulted datum that's associated with the newly inserted name.
		/// </returns>
		Datum& operator[](const std::string& name);

		/// <summary>
		/// Retrieves a reference to the datum found at the index.
		/// </summary>
		/// <param name="index">A number associated with the order in which items were placed into the Scope</param>
		/// <returns>
		///	A mutable reference to the datum found at the index
		/// </returns>
		Datum& operator[](uint32_t index);

		/// <summary>
		/// Retrieves a reference to the datum found at the index.
		/// </summary>
		/// <param name="index">A number associated with the order in which items were placed into the Scope</param>
		/// <returns>
		///	A const reference to the datum found at the index
		/// </returns>
		const Datum& operator[](uint32_t index) const;

		/// <summary>
		/// Compares one scope to another.
		/// Returns true if is scope is equal and all nested scopes are equal. 
		/// Scope is considered equal if they contain the same number of items and if the items are equivalent regardless of order.
		/// </summary>
		/// <param name="rhs">The scope we wish to compare against.</param>
		/// <returns>
		/// Returns true if scopes contain the same items and if the items are equal.
		/// Returns false if scope does not contain the same number of items and/or if items are not equal.
		/// </returns>
		bool operator==(const Scope& rhs) const;

		/// <summary>
		/// Compares one scope to another.
		/// Returns true if is scope is NOT equal.
		/// Scope is considered NOT equal if they don't contain the same number of items and/or if the items are not equivalent regardless of order.
		/// </summary>
		/// <param name="rhs">The scope we wish to compare against.</param>
		/// <returns>
		/// Returns true if scope does not contain the same number of items and/or if items are not equal.
		/// Returns false if scopes contain the same items and if the items are equal.
		/// </returns>
		bool operator!=(const Scope& rhs) const;
		
		/// <summary>
		/// Virtual method from base class RTTI.
		/// For Scope it performs the operator== method. Compares scopes.
		/// </summary>
		/// <param name="rhs">A base class pointer that in this case will be a pointer to Scope.</param>
		/// <returns>
		/// Returns true if scopes contain the same items and if the items are equal.
		/// Returns false if scope does not contain the same number of items and/or if items are not equal.
		/// </returns>
		bool Equals(const RTTI* rhs) const override;

		/// <summary>
		/// Removes scope from parent.
		/// Makes sure no reference of scope is found in parent.
		/// User must delete this scope OR adopt it into a scope that will call delete on it.
		/// </summary>
		/// <returns>
		/// A pair of Scope Reference and bool.
		/// Scope Reference is a reference to itself
		/// bool is true if it was removed from a parent. 
		/// bool is false if there was no parent to remove this scope from.
		/// </returns>
		pt_scopeRef_bool Orphan();

		/// <summary>
		/// Performs a local search of the Scope passed in.
		/// </summary>
		/// <param name="scope"> Scope to look for within calling Scope.</param>
		/// <returns>
		/// A pair of Datum Pointer and std::size_t (index)
		/// Datum Pointer contains a pointer to the datum that contains the Scope.
		/// If the Scope is not found then it returns nullptr.
		/// std::size_t the index where the scope is found within the datum returned.
		/// If the Scope is not found the std::size_t index is 0.
		/// </returns>
		pt_datumPtr_index FindContainedScope(const Scope& scope);

		/// <summary>
		/// Performs a local search of the Scope passed in.
		/// </summary>
		/// <param name="scope"> Scope to look for within calling Scope.</param>
		/// <returns>
		/// A pair of Datum Pointer and std::size_t (index)
		/// Datum Pointer contains a pointer to a const datum that contains the Scope.
		/// If the Scope is not found then it returns nullptr.
		/// std::size_t the index where the scope is found within the datum returned.
		/// If the Scope is not found the std::size_t index is 0.
		/// </returns>
		pt_constDatumPtr_index FindContainedScope(const Scope& scope) const;

		/// <summary>
		/// Creates a heap allocated copy of the Scope.
		/// Users responsibility to have this scope adopted into a scope that will call delete it on it. Or the user must exlicitly delete.
		/// </summary>
		/// <returns>
		/// A reference to a heap allocated copy of the Scope.
		/// </returns>
		virtual gsl::owner<Scope*> Clone() const;

		/// <summary>
		/// Returns the number of items in the scope.
		/// </summary>
		/// <returns>
		/// Number of items in the scope.
		/// </returns>
		std::size_t NumberOfItems() const;
		

		
		/// <summary>
		/// Returns the number of buckets that a scope has in its hash map.
		/// </summary>
		/// <returns>
		/// Number of buckets that this scope has in its hash map.
		/// </returns>
		std::size_t NumberOfBuckets() const; //TO DO: Create test for this as of 04/03/2022

		/// <summary>
		/// The Scope stores data in a hash map. If the scopes hash map were to a high load factor the user may want to rehash their map to a larger size.
		/// </summary>
		/// <param name="size">The number of buckets that the scopes hashmap will have.</param>
		void ReHashMap(std::size_t size);

		/// <summary>
		/// The Scope stores data in a hash map. This returns the load factor of that hash map.
		/// </summary>
		/// <returns></returns>
		float LoadFactorPercentage() const;
		
		/// <summary>
		/// Determines if the scope passed in is an ancestor of the scope calling the method.
		/// </summary>
		/// <param name="scope">The scope that may be the ancestor of the calling scope</param>
		/// <returns>
		/// True if the scope passed in is an ancestor of the scope calling this method.
		/// False if the scope passed in is NOT an ancestor of the scope calling this method. This may also mean it is no way related to the scope.
		/// </returns>
		bool IsAncestorOf(const Scope& scope) const;

		/// <summary>
		/// Determines if the scope passed in is a descendant of the scope calling the method.
		/// </summary>
		/// <param name="scope">The scope that may be a descendant of the calling scope</param>
		/// <returns>
		/// True if the scope passed in is a descendant of the scope calling this method.
		/// False if the scope passed in is NOT a descendant of the scope calling this method. This may also mean it is no way related to the scope.
		/// </returns>
		bool IsDescendantOf(const Scope& scope) const;

		/// <summary>
		/// If working in confidence that a value exists within the scope then this can be used to located and retrieve that value.
		/// </summary>
		/// <param name="string">The key of the value found in the Scopes hash map</param>
		/// <returns>A mutable reference to the value found in the Scope</returns>
		Datum& At(const std::string& string);

		/// <summary>
		/// If working in confidence that a value exists within the scope then this can be used to located and retrieve that value.
		/// </summary>
		/// <param name="string">The key of the value found in the Scopes hash map</param>
		/// <returns>A mutable reference to the value found in the Scope</returns>

		const Datum& At(const std::string& string) const;


		/// <summary>
		/// Returns the parent scope as the type specified. Performs type checking.
		/// Most common use case is to return the parent as a GameObject.
		/// </summary>
		/// <typeparam name="ParentsType">Parents Class Type</typeparam>
		/// <returns>
		/// A mutable reference to the parent scope as a certain type.
		/// </returns>
		template<typename ParentsType>
		ParentsType& GetParent() const;

		template<typename T> //ADDED 6/20/2022
		Vector<T*> FindAllChildrenOfType();


		template<typename T>
		Vector<const T* const> FindAllChildrenOfType() const;


		template<typename T> //ADDED 6/20/2022
		void WorkOnAllChildrenOfType(std::function<void(T&)>);




	private:
		std::string ToString() const override;
		Scope* parentScope{ nullptr };
		UnorderedMap<const std::string, Datum> _UMap{11};
		Vector<pt_string_datum*> _orderVector{11};
	};
	
}

#include "Scope.inl"