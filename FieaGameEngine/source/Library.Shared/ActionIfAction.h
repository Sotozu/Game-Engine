#pragma once
#include "Action.h"
#include "ActionList.h"
#include "Factory.h"

namespace FieaGameEngine
{
	class ActionIfAction : public Action
	{
		RTTI_DECLARATIONS(ActionIfAction, Action);

	public:

		/// <summary>
		/// Macros is defined in TypeRegistry header file.
		/// This is used to register this class with the TypeRegistry.
		/// Will start a recursive call that will register all classes that are inherited.
		/// </summary>
		/// <param name="">Derived Class</param>
		/// <param name="">Base Class</param>
		StaticRegisterMethod(ActionIfAction, Action);

		/// <summary>
		/// Takes in a bool reference and will execute behavior based on it's state.
		/// </summary>
		/// <param name="condition">A reference to a bool</param>
		inline void SetCondition(const bool& condition);

		/// <summary>
		/// Clones the calling Action. Heap constructs this object and it's on the user to manage its memory.
		/// Overridden function from Scope.
		/// </summary>
		/// <returns>
		/// A pointer to heap allocated clone of the calling Action.
		/// </returns>
		gsl::owner<ActionIfAction*> Clone() const override;

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
		/// Returns a pointer to the True action list.
		/// </summary>
		/// <returns>
		/// Returns a valid pointer if the true action block is set
		/// Returns an invalid pointer if the true action block is NOT set.
		/// </returns>
		ActionList* GetTrueActionList() const;

		/// <summary>
		/// Returns a pointer to the False action list.
		/// </summary>
		/// <returns>
		/// Returns a valid pointer if the false action block is set
		/// Returns an invalid pointer if the false action block is NOT set.
		/// </returns>
		ActionList* GetFalseActionList() const;
		
		/// <summary>
		/// Returns a string that identifies this class.
		/// Overridden function from RTTI.
		/// </summary>
		/// <returns>A string that identifies this class</returns>
		std::string ToString() const override;

		/// <summary>
		/// ActionIfAction must be parented to a GameObject, ActionList etc. or this method won't work.
		/// The ActionLists passed in MUST BE HEAPALLOCATED.
		/// Memory is now being managed by ActionIfAction.
		/// </summary>
		/// <param name="name">Name that uniquely identifies this action</param>
		/// <param name="condition">A boolean to watch and exact true and false blocks based on state</param>
		/// <param name="trueActions">HEAP ALLOCATED Statements to execute</param>
		/// <param name="falseActions">HEAP ALLOCATED Statements to execute</param>
		void Init(const std::string& name, const bool& condition, ActionList& trueActions, ActionList& falseActions);
	
		/// <summary>
		/// ActionIfAction must be parented to a GameObject, ActionList etc. or this method won't work.
		/// The ActionLists passed in MUST BE HEAPALLOCATED.
		/// Memory is now being managed by ActionIfAction.
		/// </summary>
		/// <param name="condition">A boolean to watch and exact true and false blocks based on state</param>
		/// <param name="trueActions">HEAP ALLOCATED Statements to execute</param>
		/// <param name="falseActions">HEAP ALLOCATED Statements to execute</param>
		void Init(const bool& condition, ActionList& trueActions, ActionList& falseActions);

		/// <summary>
		/// ActionIfAction must be parented to a GameObject, ActionList etc. or this method won't work.
		/// The ActionLists passed in MUST BE HEAPALLOCATED.
		/// Memory is now being managed by ActionIfAction.
		/// </summary>
		/// <param name="trueActions">ActionList to be the true actions</param>
		/// <param name="falseActions">ActionList to be the false actions</param>
		void SetTrueFalseActions(ActionList& trueActions, ActionList& falseActions);

		/// <summary>
		/// ActionIfAction must be parented to a GameObject, ActionList etc. or this method won't work.
		/// The ActionLists passed in MUST BE HEAPALLOCATED.
		/// Memory is now being managed by ActionIfAction.
		/// </summary>
		/// <param name="trueActions">ActionList to be the false actions</param>
		void SetFalseActions(ActionList& falseActions);

		/// <summary>
		/// ActionIfAction must be parented to a GameObject, ActionList etc. or this method won't work.
		/// The ActionLists passed in MUST BE HEAPALLOCATED.
		/// Memory is now being managed by ActionIfAction.
		/// </summary>
		/// <param name="trueActions">ActionList to be the true actions</param>
		void SetTrueActions(ActionList& trueActions);

		/// <summary>
		/// Will check the _target to determine if the condition is true or false.
		/// Will execute _true or _false ActionList depending on the state of the _target condition.
		/// </summary>
		/// <param name="gameTime">A reference to the game time</param>
		void Update(const GameState& gameState) override;

		/// <summary>
		/// Default Constructor
		/// Required for Factory and de-serialization
		/// </summary>
		ActionIfAction();

		//ADDED 6/21/2022
		ActionIfAction(Game& game);

		virtual ~ActionIfAction() override;

		/// <summary>
		/// Copy Constructor
		/// </summary>
		/// <param name="rhs">The ActionIfAction to copy</param>
		ActionIfAction(const ActionIfAction& rhs);

		/// <summary>
		/// Move Constructor
		/// </summary>
		/// <param name="rhs">The ActionIfAction to move</param>
		ActionIfAction(ActionIfAction&& rhs) noexcept;

		/// <summary>
		/// Copy Assignment
		/// </summary>
		/// <param name="rhs">The ActionIfAction to copy</param>
		/// <returns>A mutable reference to this object</returns>
		ActionIfAction& operator=(const ActionIfAction& rhs);

		/// <summary>
		/// Move Assignment
		/// </summary>
		/// <param name="rhs">The ActionIfAction to move</param>
		/// <returns>A mutable reference to this object</returns>
		ActionIfAction& operator=(ActionIfAction&& rhs) noexcept;

		/// <summary>
		/// Method is used for registering this types signatures and for classes that inherit from GameObject.
		/// Used by TypeRegistry class
		/// </summary>
		/// <returns>
		/// This classes unique signatures.
		/// </returns>
		static const Vector<Signature> Signatures();

		/// <summary>
		/// The variable used as the condition.
		/// Use case is to be the name of a boolean in parent object.
		/// </summary>
		std::string  _targetKey;


		Datum* _target; //TO DO: REVIEW IF THIS CAN BE SET FROM JSON
		//currently not designed for assignment in json.
		//memory is not being managed by the hierarchy

		/// <summary>
		/// Clears out the string members in the class (_targetKey, _target) and
		/// deletes the Datum * _target because it is the only data that is being managed by this class.
		/// </summary>
		void Clear();

	protected:
		/// <summary>
		/// Protected constructor that is implemented in inheriting Actions who need to pass on their TypeId to Attributed.
		/// </summary>
		/// <param name="typeId">The derived classes TypeId</param>
		ActionIfAction(std::size_t typeId);

		//ADDED 6/21/2022
		ActionIfAction(Game& game, std::size_t typeId);

		/// <summary>
		/// A pointer to an Action list that exists within the classes Scope hierarchy.
		/// Once assigned the hierarchy will own the memory of the ActionList.
		/// MUST BE HEAPALLOCATED.
		/// </summary>
		ActionList* _true;

		/// <summary>
		/// A pointer to an Action list that exists within the classes Scope hierarchy.
		/// Once assigned the hierarchy will own the memory of the ActionList.
		/// MUST BE HEAPALLOCATED.
		/// </summary>
		ActionList* _false;

		/// <summary>
		/// A helper function used when running the update method to determine if the _true, _false and _target have been set.
		/// </summary>
		/// <returns>
		/// True when _false, _true and _target are not nullptr. Have been set.
		/// False when one of the following is nullptr _false, _true and _target.
		/// </returns>
		bool CheckTrueFalseSettings();
	private:
	};

	/// <summary>
	/// Macro is defined in Factory header file
	/// Will create a class that creates GameObjects and registers itself with the Factory Group. 
	/// </summary>
	/// <param name="">Concrete to be made in the factory</param>
	/// <param name="">The group to which this concrete object belongs to. The base class designed to be the defining type for the group.</param>
	ConcreteFactory(ActionIfAction, Action);


};

#include "ActionIfAction.inl"