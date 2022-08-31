#pragma once
#include "Vector.h"
#include "Action.h"
#include "Factory.h"
//#include "GameTime.h"
#include "ActionList.h"

namespace FieaGameEngine
{
	/// <summary>
	/// Action will create another action and append it to the _actions of the owning GameObject
	/// </summary>
	class ActionCreateAction : public Action
	{
		RTTI_DECLARATIONS(ActionCreateAction, Action);

	public:
		/// <summary>
		/// Give the GameState class access to private member variables _actionsCreated
		/// and private method GetActionsCreated
		/// </summary>
		friend class GameState;

		/// <summary>
		/// Macros is defined in TypeRegistry header file.
		/// This is used to register this class with the TypeRegistry.
		/// Will start a recursive call that will register all classes that are inherited.
		/// </summary>
		/// <param name="">Derived Class</param>
		/// <param name="">Base Class</param>
		StaticRegisterMethod(ActionCreateAction, Action);

		/// <summary>
		/// Clones the calling Action. Heap constructs this object and it's on the user to manage its memory.
		/// Overridden function from Scope.
		/// </summary>
		/// <returns>
		/// A pointer to heap allocated clone of the calling Action.
		/// </returns>
		inline gsl::owner<ActionCreateAction*> Clone() const override;

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
		/// REQUIRES SEARCH TO BE UPDATED TO TAKE A PATH TO THE TARGETKEY.
		/// Set the path to a GameObject/ActionList as a target for this action to work on.
		/// Will create an action on that target.
		/// </summary>
		/// <param name="targetKey">Path to the GameObject/ActionList</param>
		inline void SetTarget(const std::string& targetKey);
		
		/// <summary>
		/// Returns a mutable reference of the target path
		/// </summary>
		/// <returns>Returns a mutable reference of the target path</returns>
		inline std::string& GetTarget();

		/// <summary>
		/// Returns a const reference of the target path
		/// </summary>
		/// <returns>Returns a const reference of the target path</returns>
		inline const std::string& GetTarget() const;

		/// <summary>
		/// Set the action to that ActionCreateAction will create.
		/// </summary>
		/// <param name="actionToCreate">Name of the action to create</param>
		inline void SetActionToCreate(const std::string& actionToCreate);

		/// <summary>
		/// Returns a mutable reference of the name of the action to create
		/// </summary>
		/// <returns>Returns a mutable reference of the name of the action to create</returns>
		inline std::string& GetActionToCreate();

		/// <summary>
		/// Returns a const reference of the name of the action to create
		/// </summary>
		/// <returns>Returns a const reference of the name of the action to create</returns>
		inline const std::string& GetActionToCreate() const;


		/// <summary>
		/// Returns a string that identifies this class.
		/// Overridden function from RTTI.
		/// </summary>
		/// <returns>A string that identifies this class</returns>
		inline std::string ToString() const override;

		/// <summary>
		/// Can set the member variables of the Action after construction. 
		/// Actions will not be automatically populated when conjured by the user.
		/// </summary>
		/// <param name="name">Name that uniquely identifies this Action</param>
		/// <param name="actionToCreate">Name of action to create</param>
		/// <param name="targetKey">The GameObject/ActionList to create actions for</param>
		inline void Init(const std::string& name, const std::string& actionToCreate, const std::string& targetKey = {});

		/// <summary>
		/// Can set the member variables of the Action after construction. 
		/// Actions will not be automatically populated when conjured by the user.
		/// </summary>
		/// <param name="actionToCreate">Name of action to create</param>
		/// <param name="targetKey">The GameObject/ActionList to create actions for</param>
		inline void Init(const std::string& actionToCreate, const std::string& targetKey = {});

		/// <summary>
		/// Creates an action and assigns it to the GameObject/ActionList etc that is target.
		/// If _actionToCreate member is empty then this update will not do anything.
		/// </summary>
		/// <param name="gameTime">A reference to the game time</param>
		void Update(const GameState& gameState) override;

		/// <summary>
		/// Default Constructor
		/// Used by factory on de-serialization
		/// </summary>
		ActionCreateAction();

		//ADDED 6/21/2022
		ActionCreateAction(Game& game);
		
		~ActionCreateAction() = default;

		/// <summary>
		/// Copy Constructor
		/// </summary>
		/// <param name="rhs">Action to copy</param>
		ActionCreateAction(const ActionCreateAction& rhs);

		/// <summary>
		/// Move Constructor
		/// </summary>
		/// <param name="rhs">Action to move</param>
		ActionCreateAction(ActionCreateAction&& rhs) noexcept;

		/// <summary>
		/// Copy Assignment
		/// </summary>
		/// <param name="rhs">Action to copy</param>
		/// <returns>A mutable reference to itself</returns>
		ActionCreateAction& operator=(const ActionCreateAction&);

		/// <summary>
		/// Move Assignment
		/// </summary>
		/// <param name="rhs">Action to move</param>
		/// <returns>A mutable reference to itself;</returns>
		ActionCreateAction& operator=(ActionCreateAction&&) noexcept;

		/// <summary>
		/// Method is used for registering this types signatures and for classes that inherit from Action.
		/// Used by TypeRegistry class
		/// </summary>
		/// <returns>
		/// This classes unique signatures.
		/// </returns>
		static const Vector<Signature> Signatures();


		/// <summary>
		/// Class name of the action to create.
		/// </summary>
		std::string  _actionToCreate;

		/// <summary>
		/// Name of the object which the action will created for.
		/// If empty then defaulted to the parent object.
		/// </summary>
		std::string  _targetKey;

	protected:

		/// <summary>
		/// Constructor used when a class derived from this class is constructed
		/// </summary>
		/// <param name="typeId">The class typeId passed from a class that derives from this class</param>
		ActionCreateAction(std::size_t typeId);

		//ADDED 6/21/2022
		ActionCreateAction(Game& game, std::size_t typeId);
	private:

		/// <summary>
		/// Vector of actions to create requires a pair of Action* and Scope*. 
		/// This is the convenience type to represent this pair.
		/// </summary>
		using PairType = std::pair<Action*, Scope*>;

		/// <summary>
		/// When this ActionCreateAction creates a action it stores what it created and where to append it into a vector called _actionsCreated.
		/// Once the core Update phase of the game is complete the GameState class will call its finalize update method where the actions created in this
		/// update will be appended (linked) to their respective GameObject/ActionList etc.
		/// </summary>
		inline static Vector<PairType> _actionsCreated;

		/// <summary>
		/// Returns the a mutable reference to the _actionsCreated vector. Used by GameState.
		/// </summary>
		/// <returns>A mutable reference to the _actionsCreated vector</returns>
		inline static Vector<PairType>& GetActionsCreated();

	};

	/// <summary>
	/// Macro is defined in Factory header file
	/// Will create a class that creates ActionCreateAction objects and registers itself with the Factory Group. 
	/// </summary>
	/// <param name="">Concrete to be made in the factory</param>
	/// <param name="">The group to which this concrete object belongs to. The base class designed to be the defining type for the group.</param>
	ConcreteFactory(ActionCreateAction, Action);

}

#include "ActionCreateAction.inl"