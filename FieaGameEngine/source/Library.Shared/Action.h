#pragma once
#include "Attributed.h"


namespace FieaGameEngine
{
	class GameState;
	class GameObject;
	class Game;

	class Action : public Attributed
	{
		RTTI_DECLARATIONS(Action, Attributed);
	public:
		/// <summary>
		/// Default constructor
		/// Required for Factory system (De-serialization process)
		/// </summary>
		Action();
		
		/// <summary>
		/// Use case is when instantiating a new action in the game from within the scope of the game.
		/// </summary>
		/// <param name="game"></param>
		/// I don't like this design because adding a new action at runtime will require getting a reference to the camera.
		/// This means when a new Action is created we need to call initialize and a pointer to the game.
		/// It's possible to just screw it and make the game a singleton. But...to my understanding this may not be good?
		/// I can't recall exactly but I have been communicated to in the past that I should shy from singleton use if I can, it's an anti-pattern.
		/// What I can do is I can call an initialize method that will search up the hierarchy for a parent game object and take the pointer from there.
		Action(Game& game); 

		virtual ~Action() = default;

		Action(const Action& rhs) = default;
		Action(Action&& rhs) noexcept = default;

		Action& operator=(const Action& rhs) = default;
		Action& operator=(Action&& rhs) noexcept = default;

		/// <summary>
		/// Will set if the action is enabled.
		/// Used in the Update method to determine if the update will execute it's code
		/// </summary>
		/// <param name="enable">bool to enable the action</param>
		void SetEnabled(bool enable);

		/// <summary>
		/// Returns true if the action is enabled.
		/// Returns false if the action is disabled.
		/// </summary>
		/// <returns>
		/// Returns true if the action is enabled.
		/// Returns false if the action is disabled.
		/// </returns>
		bool IsEnabled();

		/// <summary>
		/// Derived actions will implement their unique Update method.
		/// </summary>
		/// <param name="gameTime">A reference to the game time</param>
		virtual void Update(const GameState& gameState) = 0;
		
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

		bool _isEnabled;

		void SetGame(Game& game);
		Game* GetGame();
		virtual void Initialize();
	protected:
		//ADDED 6/21/2022
		//Not a fan of this. Will like to make game a singleton to provide better access to the game.

		/// <summary>
		/// Protected constructor that is implemented in inheriting Actions who need to pass on their TypeId to Attributed.
		/// </summary>
		/// <param name="typeId">The derived classes TypeId</param>
		Action(std::size_t typeId);

		/// <summary>
		/// Protected constructor that is implemented in inheriting Actions who need to pass on their TypeId to Attributed.
		/// </summary>
		/// <param name="typeId">The derived classes TypeId</param>
		Action(Game& game, std::size_t typeId);

		Game* _game;
	private:


	};

}

#include "Action.inl"
 








