#include "pch.h"
#include "ActionExpression.h"
#include <string>
#include <iostream>
#include <sstream>


namespace FieaGameEngine
{

	RTTI_DEFINITIONS(ActionExpression);

	ActionExpression::ActionExpression() : Action(ActionExpression::TypeIdClass())
	{}

	//ADDED 6/21/2022
	ActionExpression::ActionExpression(Game& game) : Action(game, ActionExpression::TypeIdClass())
	{}

	ActionExpression::ActionExpression(std::size_t typeId) : Action(typeId)
	{}

	//ADDED 6/21/2022
	ActionExpression::ActionExpression(Game& game, std::size_t typeId) : Action(game, typeId)
	{}

	void ActionExpression::Update(const GameState& gameState)
	{
		UNREFERENCED_PARAMETER(gameState);
		if (_isEnabled)
		{
			InfixToPostfix();
			EvaluatePostfixEpression();
		}
	}

	std::string ActionExpression::InfixToPostfix()
	{

		std::stringstream _infixExpression{ _expression };
		std::string _token;

		while (std::getline(_infixExpression, _token, ' ')) //grab characters until a space is found
		{
			if (!_token.empty())
			{
				if (IsVariable(_token)) { _queue.push(_token); } //Handle if the token is a variable name "_health"
				else if (IsNumber(_token)) { _queue.push(_token); } //Handle if the token is a number
				else { HandleMathSymbol(_token); } //Handle if the token is math symbol
			}
		}

		while (!_stack.IsEmpty()) //empty stack into the queue
		{
			_queue.push(_stack.Top());
			_stack.Pop();
		}
		std::string _postFixExpression;
		return _postFixExpression;
	}


	const Vector<Signature> ActionExpression::Signatures()
	{
		return Vector<Signature>
		{
			{ "_expression", DatumTypes::String, 1, offsetof(ActionExpression, _expression) },
		};
	}

	bool ActionExpression::IsVariable(const std::string& token) const
	{
		if (token[0] == 95 || (token[0] >= 65 && token[0] <= 90) || (token[0] >= 97 && token[0] <= 122)) { return true; } //'_', 'A - Z' and 'a - z'
		else { return false; }
	}

	bool ActionExpression::IsEvalutionSybmol(const std::string& token)
	{
		if (token == "=" || token == "==") { return true; }
		else { return false; }
	}



	bool ActionExpression::IsNumber(const std::string& token) const
	{
		if (token[0] >= 48 && token[0] <= 57) { return true; } //'0 - 9'
		else { return false; }
	}

	void ActionExpression::HandleMathSymbol(const std::string& _token)
	{
		for (std::size_t index = 0; index < _operators->size(); ++index)
		{

				//1. Parentheses
				if (_token == "(")
				{
					_stack.Push(_token);
				}
				else if (_token == ")")
				{
					//Special case for closing parentheses. You want to pop all the operators of the stack and queue them until
					//we encounter the parentheses close.

					while (_stack.Top() != "(")
					{
						_queue.push(_stack.Top());
						_stack.Pop();
					}
					_stack.Pop(); //Once found we want to discard it.
				}
				//2. Exponents
				else if (_token == "^")
				{
					_stack.Push(_token);
				}
				//3. Multiplication && Division
				else if (_token == "*" || _token == "/" || _token == "%")
				{
					if (!_stack.IsEmpty())
					{
						std::string _operatorInStack = _stack.Top();
						if (_operatorInStack == "^") //If the operator at the top of the stack has higher precedence
						{
							_queue.push(_stack.Top()); //Add the higher precedence operator to the queue
							_stack.Pop(); //remove the higher precedence operator from the stack
						}
					}
					_stack.Push(_token);
				}
				//4. Addition && Subtraction
				else if (_token == "+" || _token == "-")
				{
					if (!_stack.IsEmpty())
					{
						std::string _operatorInStack = _stack.Top();
						if (_operatorInStack == "^" || _operatorInStack == "*" || _operatorInStack == "/") //If the operator at the top of the stack has higher precedence
						{
							_queue.push(_stack.Top()); //Add the higher precedence operator to the queue
							_stack.Pop(); //remove the higher precedence operator from the stack
						}

					}
					_stack.Push(_token);

				}
				else if (_token == "=" || _token == "==")
				{
					_stack.Push(_token);
				}
		}
	}


	bool ActionExpression::Equals(const RTTI* rhs) const
	{
		const ActionExpression* action = rhs->As<ActionExpression>();
		if (action == nullptr) { return false; }

		return _expression == action->_expression;
	}

	void ActionExpression::EvaluatePostfixEpression()
	{

		while (!_queue.empty())
		{
			const std::string _token = _queue.front();
			_queue.pop();
			if (IsVariable(_token))
			{
				_datumStack.Push(Datum(*SearchUp(_token))); //This will 
			}
			else if (IsNumber(_token))
			{
				_datumStack.Push(Datum(std::stof(_token)));
			}
			else if (IsEvalutionSybmol(_token))
			{
				Datum rhsDatum = _datumStack.Top(); // this will get us the value on the right of the evaluations
				_datumStack.Pop();
				Datum lhsDatum = _datumStack.Top();
				_datumStack.Pop();

				if (_token == "=")
				{
					if (lhsDatum.Type() == DatumTypes::Integer)
					{
						lhsDatum.Get<int>() = static_cast<int>(rhsDatum.Get<float>());
					}
					else if (lhsDatum.Type() == DatumTypes::Float)
					{
						lhsDatum.Get<float>() = rhsDatum.Get<float>();
					}
				}
				else if (_token == "==")
				{
					Datum newDatum;
					if (lhsDatum == rhsDatum)
					{
						newDatum = 1.0f; //true
					}
					else
					{
						newDatum = 0.0f; //false
					}
					_datumStack.Pop();
					_datumStack.Push(newDatum);
				}
			}
			else //must be a math symbol
			{
				float numA = _datumStack.Top().Get<float>();
				_datumStack.Pop();
				float numB = _datumStack.Top().Get<float>();
				_datumStack.Pop();

				if (_token == "*")
				{
					_datumStack.Push(Datum(numB * numA));
				}
				else if (_token == "/")
				{
					_datumStack.Push(Datum(numB / numA));
				}
				else if (_token == "-")
				{
					_datumStack.Push(Datum(numB - numA));
				}
				else if (_token == "+")
				{

					_datumStack.Push(Datum(numB + numA));
				}
				else if (_token == "^")
				{
					_datumStack.Push(Datum(std::pow(numB, numA)));
				}
			}
		}
			
	}
}




