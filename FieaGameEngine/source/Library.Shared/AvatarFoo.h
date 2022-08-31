#pragma once
#include "GameObject.h"
#include "TypeRegistry.h"
#include "Vector.h"
#include <gsl/pointers>


using namespace FieaGameEngine;

namespace FieaGameEngine
{
	class AvatarFoo : public FieaGameEngine::GameObject
	{
		RTTI_DECLARATIONS(AvatarFoo, FieaGameEngine::GameObject);
	public:
		/// <summary>
		/// Default Constructor
		/// </summary>
		AvatarFoo();

		/// <summary>
		/// Clones the calling AvatarFoo. Heap constructs this object and it's on the user to manage its memory.
		/// Overridden function from Scope.
		/// </summary>
		/// <returns>
		/// A pointer to heap allocated clone of the calling AvatarFoo.
		/// </returns>
		gsl::owner< AvatarFoo*> Clone() const override;

		/// <summary>
		/// Update method will update the AvatarFoo and then recursively update all it's children GameObjects.
		/// Overridden function from GameObject
		/// </summary>
		/// <param name="gameTime">A reference to the game time</param>
		virtual void Update(const GameState& gameState) override;

		/// <summary>
		/// Compares one AvatarFoo with another.
		/// Overridden function from RTTI.
		/// </summary>
		/// <param name="rhs">The AvatarFoo to compare against</param>
		/// <returns>
		/// TRUE if the AvatarFoo is equivalent.
		/// FALSE if the AvatarFoo is not equivalent
		/// </returns>
		bool Equals(const RTTI* rhs) const override;

		/// <summary>
		/// Returns a string that identifies this class.
		/// Overridden function from RTTI.
		/// </summary>
		/// <returns>A string that identifies this class</returns>
		std::string ToString() const override;

		/// <summary>
		/// Method is used for registering this types signatures and for classes that inherit from AvatarFoo.
		/// Used by TypeRegistry class
		/// </summary>
		/// <returns>
		/// This classes unique signatures.
		/// </returns>
		static FieaGameEngine::Vector<FieaGameEngine::Signature> Signatures();

		/// <summary>
		/// Macros is defined in TypeRegistry header file.
		/// This is used to register this class with the TypeRegistry.
		/// Will start a recursive call that will register all classes that are inherited.
		/// </summary>
		/// <param name="">Derived Class</param>
		/// <param name="">Base Class</param>
		StaticRegisterMethod(AvatarFoo, GameObject);
		int _hitPoints;
		bool _isTakingDamage;
	protected:
		AvatarFoo(std::size_t typeId);

	};
	ConcreteFactory(AvatarFoo, Scope);
}

