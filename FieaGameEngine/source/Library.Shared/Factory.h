#pragma once
#include <string>
#include <gsl/pointers>
#include "UnorderedMap.h"

namespace FieaGameEngine
{
	/// <summary>
	/// Allows us to express the intention of creating objects, based on the name of their type. 
	/// The Abstract Factory design pattern accomplishes this goal. 
	/// Implements a generic form of the Abstract Factory, which is suitable for use with any interface class that has a default constructor. 
	/// The game engine will use this Factory to create objects by their type name.
	/// </summary>
	/// <typeparam name="AbstractProductType">The base class of all derived objects which the concrete facotoreis create.</typeparam>
	template<typename AbstractProductType>
	class Factory
	{
	public:
		/// <summary>
		/// If a concrete factory exists that can produce the class passed in then it will return a base class pointer of that heap allocated object.
		/// </summary>
		/// <param name="name">Name of the class we wish to create</param>
		/// <returns>
		/// If successful a base class pointer to a newly created object on the heap.
		/// If NOT successful returns a nullptr.
		/// </returns>
		inline static gsl::owner<AbstractProductType*> Create(const std::string& name);

		/// <summary>
		/// Default constructor
		/// No class member variables that require special initialization
		/// </summary>
		Factory() = default;

		/// <summary>
		/// Class is abstract and use case of class will not require this.
		/// </summary>
		/// <param name="">Factory to copy</param>
		Factory(const Factory&) = delete;

		/// <summary>
		/// Class is abstract and use case of class will not require this.
		/// </summary>
		/// <param name="">Factory to copy</param>
		Factory& operator=(const Factory&) = delete;

		/// <summary>
		/// Class is abstract and use case of class will not require this.
		/// </summary>
		/// <param name="">Factory to move</param>
		Factory(Factory&&) noexcept = delete;

		/// <summary>
		/// Class is abstract and use case of class will not require this.
		/// </summary>
		/// <param name="">Factory to move</param>
		Factory& operator=(Factory&&) noexcept = delete;

		/// <summary>
		/// Class is abstract and thus deriving classes will override the destructor.
		/// </summary>
		virtual ~Factory() = default;

	protected:
		using TKey = const std::string;
		using TData = const Factory<AbstractProductType>*;

		using pairType = std::pair<TKey, TData>;
		using UMapFactories = UnorderedMap<TKey, TData>;

		/// <summary>
		/// 
		/// </summary>
		/// <param name="name"></param>
		/// <returns></returns>
		inline static Factory<AbstractProductType>* Find(const std::string* name);

		/// <summary>
		/// Will add a concrete factory to the static hash map of factories. This is called on instantiation of a concrete factory.
		/// </summary>
		/// <param name="factory">a reference to a concrete factory</param>
		inline static void Add(const Factory<AbstractProductType>& factory);

		/// <summary>
		/// Will remove a concrete factory from the static hash map of factories. This is called on destruction of a concrete factory.
		/// </summary>
		/// <param name="factory">a reference to a concrete factory</param>
		inline static void Remove(const Factory<AbstractProductType>& factory);

		/// <summary>
		/// Will be called by a concrete factory to create an instance of what it is meant to construct.
		/// </summary>
		/// <returns>
		/// A pointer to a heap allocated object that a concrete constructor creates.
		/// </returns>
		virtual gsl::owner<AbstractProductType*> Create() const = 0;

		/// <summary>
		/// Returns the name of the class that a concert factory creates.
		/// </summary>
		/// <returns>
		/// A reference to a string which is the name of the concrete class this factory creates.
		/// </returns>
		virtual const std::string& ClassName() const = 0;


	private:
		/// <summary>
		/// Hash map of concrete factories
		/// </summary>
		inline static UMapFactories _factories;

	};

	/// <summary>
	/// Creates a concrete factory class for each class that can be instantiated in the engine.
	/// Macro will be invoked in header file of class that can be instantiated in the game.
	/// <param name="ConcreteFactoryClass">The name of the class that can be instantiated</param>
	/// <param name="AbstractFactoryGroup">The base class of the class that can be instantiated</param>
	/// </summary>

#define ConcreteFactory(ConcreteFactoryClass, AbstractFactoryGroup)\
class ConcreteFactoryClass ## Factory final : public FieaGameEngine::Factory<AbstractFactoryGroup>\
{\
public:\
	ConcreteFactoryClass ## Factory():_className(#ConcreteFactoryClass) {Add(*this);}\
	~ConcreteFactoryClass ## Factory() {Remove(*this);}\
	const std::string& ClassName()const override {return _className;}\
	gsl::owner<AbstractFactoryGroup*>Create() const override {return new ConcreteFactoryClass;}\
private:\
	std::string _className;\
};\

}

#include "Factory.inl"

