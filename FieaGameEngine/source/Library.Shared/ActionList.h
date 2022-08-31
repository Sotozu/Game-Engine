#pragma once
#include "Action.h"
#include "Factory.h"
#include "Vector.h"
//#include "GameTime.h"


namespace FieaGameEngine
{
	class ActionList : public Action
	{
		RTTI_DECLARATIONS(ActionList, Action);

	public:

		/// <summary>
		/// Will iterate through its list of actions and execute them sequentially.
		/// </summary>
		/// <param name="gameTime">A reference to GameTime</param>
		void Update(const GameState& gameState) override;


		/// <summary>
		/// Clones the calling Action. Heap constructs this object and it's on the user to manage its memory.
		/// Overridden function from Scope.
		/// </summary>
		/// <returns>
		/// A pointer to heap allocated clone of the calling Action.
		/// </returns>
		inline gsl::owner<ActionList*> Clone() const override;

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
		/// Returns a string that identifies this class.
		/// Overridden function from RTTI.
		/// </summary>
		/// <returns>A string that identifies this class</returns>
		inline std::string ToString() const override;

		/// <summary>
		/// Default Constructor
		/// Required for Factory and de-serialization
		/// </summary>
		ActionList();
		
		//ADDED 6/21/2022
		ActionList(Game& game);

		~ActionList() override;
		ActionList(const ActionList&) = default;
		ActionList(ActionList&&) noexcept = default;

		ActionList& operator=(const ActionList&) = default;
		ActionList& operator=(ActionList&&) noexcept = default;


		/// <summary>
		/// Method is used for registering this types signatures and for classes that inherit from Action.
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
		StaticRegisterMethod(ActionList, Action);
		

		/// <summary>
		/// Clears all actions in the action list.
		/// Review if this requires to qu the destruction of actions
		/// </summary>
		void Clear();

	protected:
		/// <summary>
		/// Protected constructor that is implemented in inheriting Objects who need to pass on their TypeId to Attributed.
		/// </summary>
		/// <param name="typeId">The derived classes TypeId</param>
		ActionList(std::size_t typeId);

		//ADDED 6/21/2022
		ActionList(Game& game, std::size_t typeId);

	private:
		Datum* _actionListDatum;
	};

	/// <summary>
	/// Macro is defined in Factory header file
	/// Will create a class that creates GameObjects and registers itself with the Factory Group. 
	/// </summary>
	/// <param name="">Concrete to be made in the factory</param>
	/// <param name="">The group to which this concrete object belongs to. The base class designed to be the defining type for the group.</param>
	ConcreteFactory(ActionList, Action);

}

#include "ActionList.inl"


