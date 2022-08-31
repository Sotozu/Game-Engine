#pragma once
#include "Action.h"
#include "Factory.h"
//#include "GameTime.h"

namespace FieaGameEngine
{
	class ActionIntegerIncrement : public Action
	{
		RTTI_DECLARATIONS(ActionIntegerIncrement, Action);

	public:


		/// <summary>
		/// Returns a string that identifies this class.
		/// Overridden function from RTTI.
		/// </summary>
		/// <returns>A string that identifies this class</returns>
		inline std::string ToString() const override;


		/// <summary>
		/// Clones the calling Action. Heap constructs this object and it's on the user to manage its memory.
		/// Overridden function from Scope.
		/// </summary>
		/// <returns>
		/// A pointer to heap allocated clone of the calling Action.
		/// </returns>
		inline gsl::owner<ActionIntegerIncrement*> Clone() const override;

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
		/// Can set the member variables of the Action after construction. 
		/// Actions will not be automatically populated when conjured by the user.
		/// </summary>
		/// <param name="name">Name that uniquely identifies this Action</param>
		/// <param name="step">The amount to increment an integer by everytime update is called</param>
		/// <param name="targetKey">Name of member variable we wish to increment.Use case target is usually the parent objects data member</param>
		inline void Init(const std::string& name, int32_t step, const std::string& targetKey);

		/// <summary>
		/// Can set the member variables of the Action after construction. 
		/// Actions will not be automatically populated when conjured by the user.
		/// </summary>
		/// <param name="step">The amount to increment an integer by everytime update is called</param>
		/// <param name="targetKey">Name of member variable we wish to increment.Use case target is usually the parent objects data member</param>
		inline void Init(int32_t step, const std::string& targetKey);

		/// <summary>
		/// Will increment a target integer by the amount assigned to step member
		/// </summary>
		/// <param name="gameState">reference to a class that governs the state of the game</param>
		void Update(const GameState& gameState) override;

		/// <summary>
		/// Returns a pointer to the target which we wish to increment.
		/// </summary>
		/// <returns>
		/// If target is set then it returns a valid pointer to the target.
		/// if target is NOT set then it returns nullptr
		/// </returns>
		inline const Datum* const GetTarget() const;

		/// <summary>
		/// Returns a pointer to the target which we wish to increment.
		/// </summary>
		/// <returns>
		/// If target is set then it returns a valid pointer to the target.
		/// if target is NOT set then it returns nullptr
		/// </returns>
		inline Datum* GetTarget();

		/// <summary>
		/// Default Constructor
		/// Required for Factory and de-serialization
		/// </summary>
		ActionIntegerIncrement();
		
		//ADDED 6/21/2022
		ActionIntegerIncrement(Game& game);
		
		~ActionIntegerIncrement() = default;
		ActionIntegerIncrement(const ActionIntegerIncrement&) = default;
		ActionIntegerIncrement(ActionIntegerIncrement&&) noexcept = default;

		ActionIntegerIncrement& operator=(const ActionIntegerIncrement&) = default;
		ActionIntegerIncrement& operator=(ActionIntegerIncrement&&) noexcept = default;

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
		StaticRegisterMethod(ActionIntegerIncrement, Action);

		/// <summary>
		/// Name of an integer member variable we wish to target.
		/// Use case is an integer member variable of the parent object or an object up the hierarchy.
		/// </summary>
		std::string  _targetKey;

		/// <summary>
		/// The amount to increase or decrease the integer by.
		/// </summary>
		int32_t _step;

	protected:
		/// <summary>
		/// Protected Constructor used by class that is to inherit from this class when passing down it's RTTI id
		/// </summary>
		/// <param name="typeId">RTTI id of parent class</param>
		ActionIntegerIncrement(std::size_t typeId);
		
		//ADDED 6/21/2022
		ActionIntegerIncrement(Game& game, std::size_t typeId);

	private:
		Datum* _target;
	};

	ConcreteFactory(ActionIntegerIncrement, Action);

}

#include "ActionIntegerIncrement.inl"