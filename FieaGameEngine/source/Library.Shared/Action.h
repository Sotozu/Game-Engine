#pragma once
#include "Attributed.h"
#include "GameTime.h"

namespace FieaGameEngine
{
	class Action : public Attributed
	{
		RTTI_DECLARATIONS(Action, Attributed);
	public:
		Action();
		virtual ~Action() = default;

		Action(const Action& rhs) = default;
		Action(Action&& rhs) noexcept = default;

		Action& operator=(const Action& rhs) = default;
		Action& operator=(Action&& rhs) noexcept = default;
		/// <summary>
		/// Derived actions will implement their unique Update method.
		/// </summary>
		/// <param name="gameTime">A reference to the game time</param>
		virtual void Update(const GameTime& gameTime) = 0;
		
		/// <summary>
		/// Returns const reference of the actions name.
		/// This name serves as a description of the action.
		/// </summary>
		/// <returns>A const reference of the actions name.</returns>
		const std::string& GetName() const;
		
		/// <summary>
		/// Returns mutable reference of the actions name.
		/// This name serves as a description of the action.
		/// </summary>
		/// <returns>A mutable reference of the actions name.</returns>
		std::string& GetName();

		/// <summary>
		/// Returns a string that identifies this class.
		/// Overridden function from RTTI.
		/// </summary>
		/// <returns>A string that identifies this class</returns>
		inline std::string ToString() const override;

		/// <summary>
		/// Compares one Action with another.
		/// Overridden function from RTTI.
		/// </summary>
		/// <param name="rhs">The Action to compare against</param>
		/// <returns>
		/// TRUE if the Action is equivalent.
		/// FALSE if the Action is not equivalent
		/// </returns>
		bool Equals(const RTTI* rhs) const override;


		/// <summary>
		/// Sets the name of the action.
		/// This name serves as a description of the action.
		/// </summary>
		/// <param name="name">The description for the action.</param>
		void SetName(const std::string& name);

		/// <summary>
		/// Method is used for registering this types signatures and for classes that inherit from GameObject.
		/// Used by TypeRegistry class
		/// </summary>
		/// <returns>
		/// This classes unique signatures.
		/// </returns>
		static const Vector<Signature> Signatures();

		/// <summary>
		/// Macros is defined in TypeRegistry header file.
		/// This is used to register this class with the TypeRegistry.
		/// Will start a recursive call that will register all classes that are inherited.
		/// </summary>
		/// <param name="">Derived Class</param>
		/// <param name="">Base Class</param>
		StaticRegisterMethod(Action, Attributed);

		/// <summary>
		/// The descriptor for the Action.
		/// </summary>
		std::string _name;

	protected:

		/// <summary>
		/// Protected constructor that is implemented in inheriting Actions who need to pass on their TypeId to Attributed.
		/// </summary>
		/// <param name="typeId">The derived classes TypeId</param>
		Action(std::size_t typeId);
	private:


	};

}

#include "Action.inl"
 








