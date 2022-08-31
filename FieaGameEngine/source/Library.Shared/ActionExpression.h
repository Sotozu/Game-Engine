#pragma once
#include "Action.h"
#include "Factory.h"
//#include "GameTime.h"
#include <queue>
#include <stack>
#include "Stack.h"


namespace FieaGameEngine
{
	class ActionExpression : public Action
	{
		RTTI_DECLARATIONS(ActionExpression, Action)
	public:
		StaticRegisterMethod(ActionExpression, Action);

		/// <summary>
		/// Converts the string expression into an actual expression and executes it.
		/// </summary>
		/// <param name="gameTime">A reference to the game time</param>
		void Update(const GameState& gameState) override;

		/// <summary>
		/// Default Constructor
		/// Required for Factory and de-serialization
		/// </summary>
		ActionExpression();

		//ADDED 6/21/2022
		ActionExpression(Game& game);

		virtual ~ActionExpression() = default;

		ActionExpression(const ActionExpression&) = default;
		ActionExpression(ActionExpression&&) noexcept = default;

		ActionExpression& operator=(const ActionExpression&) = default;
		ActionExpression& operator=(ActionExpression&&) noexcept = default;

		/// <summary>
		/// Converts the infix expression to a post fix expression.
		/// </summary>
		/// <returns>The post fix version of the expression</returns>
		std::string InfixToPostfix();

		/// <summary>
		/// Converts the post fix expression from a string into a format that can be logically executed.
		/// </summary>
		void EvaluatePostfixEpression();

		/// <summary>
		/// Method is used for registering this types signatures and for classes that inherit from Action.
		/// Used by TypeRegistry class
		/// </summary>
		/// <returns>
		/// This classes unique signatures.
		/// </returns>
		static const Vector<Signature> Signatures();

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
		inline gsl::owner<ActionExpression*> Clone() const override;

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

	protected:
		ActionExpression(std::size_t typeId);

		//ADDED 6/21/2022
		ActionExpression(Game& game, std::size_t typeId);
	private:
		bool IsVariable(const std::string& token) const;
		bool IsNumber(const std::string& token) const;
		void HandleMathSymbol(const std::string& token);
		bool IsEvalutionSybmol(const std::string& token);

		Stack<std::string> _stack;
		std::queue<std::string> _queue;

		Stack <Datum> _datumStack;


		std::string _expression;

		std::string _operators[11]{ { "(" }, {")"}, {"^"},{"%"}, {"*"}, {"/"}, {"+"}, {"-"}, {"!="}, {"=="}, {"="} };

	};

	ConcreteFactory(ActionExpression, Action)

}

#include  "ActionExpression.inl"

