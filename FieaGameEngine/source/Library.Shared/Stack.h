#pragma once

#include "SList.h"

namespace FieaGameEngine
{
	template <typename T>
	class Stack final
	{
	public:
		/// <summary>
		/// Constructs a new element on the stack by passing the elements parameter(s).
		/// </summary>
		/// <typeparam name="...Args">Template parameter pack.</typeparam>
		/// <param name="...args">Parameter elements to construct a new element on the stack.</param>
		template <typename... Args>
		void Emplace(Args&&... args);

		/// <summary>
		/// Pushes an element onto the stack. Creates a copy.
		/// </summary>
		/// <param name="value">The value to be pushed onto the stack.</param>
		void Push(const T& value);

		/// <summary>
		/// Pushes an element onto the stack by using move semantics.
		/// </summary>
		/// <param name="value">The value to be pushed onto the stack.</param>
		void Push(T&& value);

		/// <summary>
		/// Removes the element at the top of the stack.
		/// </summary>
		void Pop();

		/// <summary>
		/// Returns a mutable reference to the item at the top of the stack.
		/// </summary>
		/// <returns>A mutable T reference.</returns>
		T& Top();

		/// <summary>
		/// Returns a NON mutable reference to the item at the top of the stack.
		/// </summary>
		/// <returns>A NON mutable T reference.</returns>
		const T& Top() const;
		
		/// <summary>
		/// Returns the number of elements in the stack.
		/// </summary>
		/// <returns>Number of elements in the stack.</returns>
		std::size_t Size() const;

		/// <summary>
		/// Checks if the stack is empty.
		/// </summary>
		/// <returns>
		/// TRUE if the stack is empty.
		/// FALSE if the stack is NOT empty.
		/// </returns>
		bool IsEmpty() const;

		/// <summary>
		/// Removes all elements in the stack.
		/// </summary>
		void Clear();

	private:
		SList<T> _list;
	};
}

#include "Stack.inl"