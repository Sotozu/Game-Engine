#pragma once
#include "Vector.h"
#include "Action.h"
#include "Factory.h"
//#include "GameTime.h"
#include "ActionList.h"

namespace FieaGameEngine
{

	class ActionDestroyAction : public Action
	{
		RTTI_DECLARATIONS(ActionDestroyAction, Action);

	public:

		/// <summary>
		/// Give the GameState class access to private member variables _actionsToDestroy
		/// and private method GetActionsToDestroy
		/// </summary>
		friend class GameState;

		/// <summary>
		/// Clones the calling Action. Heap constructs this object and it's on the user to manage its memory.
		/// Overridden function from Scope.
		/// </summary>
		/// <returns>
		/// A pointer to heap allocated clone of the calling Action.
		/// </returns>
		inline gsl::owner<ActionDestroyAction*> Clone() const override;

		/// <summary>
		/// Macros is defined in TypeRegistry header file.
		/// This is used to register this class with the TypeRegistry.
		/// Will start a recursive call that will register all classes that are inherited.
		/// </summary>
		/// <param name="">Derived Class</param>
		/// <param name="">Base Class</param>
		StaticRegisterMethod(ActionDestroyAction, Action);

		/// <summary>
		/// Default Constructor
		/// Required for Factory and de-serialization
		/// </summary>
		ActionDestroyAction();
		
		//ADDED 6/21/2022
		ActionDestroyAction(Game& game);

		~ActionDestroyAction() = default;

		/// <summary>
		/// Copy Constructor
		/// </summary>
		/// <param name="rhs">ActionDestroyAction to copy</param>
		ActionDestroyAction(const ActionDestroyAction& rhs);

		/// <summary>
		/// Move Constructor
		/// </summary>
		/// <param name="rhs">ActionDestroyAction to move</param>
		/// <returns></returns>
		ActionDestroyAction(ActionDestroyAction&& rhs) noexcept;

		/// <summary>
		/// Copy Assignment
		/// </summary>
		/// <param name="rhs">ActionDestroyAction to copy</param>
		/// <returns>A mutable reference to this object</returns>
		ActionDestroyAction& operator=(const ActionDestroyAction& rhs);

		/// <summary>
		/// Move Assignment
		/// </summary>
		/// <param name="rhs">ActionDestroyAction to move</param>
		/// <returns>A mutable reference to this object</returns>
		ActionDestroyAction& operator=(ActionDestroyAction&& rhs) noexcept;

		/// <summary>
		/// Method is used for registering this types signatures and for classes that inherit from Action.
		/// Used by TypeRegistry class
		/// </summary>
		/// <returns>
		/// This classes unique signatures.
		/// </returns>
		static const Vector<Signature> Signatures();

		/// <summary>
		/// Compares one GameObject with another.
		/// Overridden function from RTTI.
		/// </summary>
		/// <param name="rhs">The GameObject to compare against</param>
		/// <returns>
		/// TRUE if the GameObject is equivalent.
		/// FALSE if the GameObject is not equivalent
		/// </returns>
		bool Equals(const RTTI* rhs) const override;


		/// <summary>
		/// Target is identified by their base class name member.
		/// Target is local to where this action is located. 
		/// Will be the target of destruction.
		/// </summary>
		/// <param name="targetKey">Actions name</param>
		inline void SetTargetAction(const std::string& targetActionName);

		/// <summary>
		/// Get a mutable reference of the _targetActionName member.
		/// </summary>
		/// <returns>A mutable reference of the _targetActionName member.</returns>
		inline std::string& GetTargetActionName();

		/// <summary>
		/// Get a const reference of the _targetActionName member.
		/// </summary>
		/// <returns>A const reference of the _targetActionName member.</returns>
		inline const std::string& GetTargetActionName() const;

		/// <summary>
		/// Returns a string that identifies this class.
		/// Overridden function from RTTI.
		/// </summary>
		/// <returns>A string that identifies this class</returns>
		inline 	std::string ToString() const override;

		/// <summary>
		/// Can set the member variables of the Action after construction. Actions will not be automatically populated when conjured by the user.
		/// </summary>
		/// <param name="name">Name that uniquely identifies this action</param>
		/// <param name="_targetActionName">Unique name of the action targeted for destruction</param>
		inline void Init(const std::string& name, const std::string& _targetActionName = {});

		/// <summary>
		/// Can set the member variables of the Action after construction. Actions will not be automatically populated when conjured by the user.
		/// </summary>
		/// <param name="_targetActionName">Unique name of the action targeted for destruction</param>
		inline void Init(const std::string& _targetActionName);

		/// <summary>
		/// Destroys an action targeted in the local object where this action is being called from.
		/// If _targetActionName member is empty then this update will not do anything.
		/// </summary>
		/// <param name="gameTime">A reference to the game time</param>
		void Update(const GameState& gameState) override;

		/// <summary>
		/// Name of the targets member name. Will be what we use to search for the target.
		/// </summary>
		std::string  _targetActionName;

	protected:

		/// <summary>
		/// Constructor used when a class derived from this class is constructed
		/// </summary>
		/// <param name="typeId">The class typeId passed from a class that derives from this class</param>
		ActionDestroyAction(std::size_t typeId);

		//ADDED 6/21/2022
		ActionDestroyAction(Game& game, std::size_t typeId);
	private:


		/// <summary>
		/// Vector of actions to destroy requires a pair of std::size_t and Datum*. 
		/// This is the convenience type to represent this pair.
		/// </summary>
		using PairType = std::pair<std::size_t, Datum*>;

		/// <summary>
		/// When this ActionDestroyAction locates the action to destroy its location into a vector called _actionsToDestroy.
		/// Once the core Update phase of the game is complete the GameState class will call its finalize update method where the actions located for destruction
		///  will be destroyed from their respective GameObject/ActionList etc.
		/// </summary>
		inline static Vector<PairType> _actionsToDestroy;

		/// <summary>
		/// Returns the a mutable reference to the _actionsToDestroy vector. Used by GameState.
		/// </summary>
		/// <returns>A mutable reference to the _actionsToDestroy vector</returns>
		static Vector<PairType>& GetActionsToDestroy();

	};
	/// <summary>
	/// Macro is defined in Factory header file
	/// Will create a class that creates ActionCreateAction objects and registers itself with the Factory Group. 
	/// </summary>
	/// <param name="">Concrete to be made in the factory</param>
	/// <param name="">The group to which this concrete object belongs to. The base class designed to be the defining type for the group.</param>
	ConcreteFactory(ActionDestroyAction, Action);

}

#include "ActionDestroyAction.inl"