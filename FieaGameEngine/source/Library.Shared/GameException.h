// Pauls code DirectXEssentialLiveLessons
#pragma once

#include <windows.h>
#include <stdexcept>
#include <string>

namespace FieaGameEngine
{
	/// <summary>
	/// This class is wrapper class that provides some additional functionality to the std::runtime_error
	/// </summary>
	class GameException final : public std::runtime_error
	{
	public:
		/// <summary>
		/// Constructor will take be populated by a message and an HRESULT
		/// This will provides more information because the HRESULT is a data type that contains windows error/warning information.
		/// </summary>
		/// <param name="message">A message written by the library author describing the error.</param>
		/// <param name="hr">A windows data type that provides windows realted error/warning information.</param>
		GameException(const char* const message, HRESULT hr = S_OK);

		/// <summary>
		/// Return the HRESULT data which provides windows related error/warning information.
		/// </summary>
		/// <returns>HRESULT data</returns>
		HRESULT HR() const;

		/// <summary>
		/// What's wrong. A message written by the user/author describing the error.
		/// </summary>
		/// <returns>A wide string with the description of what's wrong.</returns>
		std::wstring whatw() const;

	private:
		/// <summary>
		/// Stored HRESULT when constructed.
		/// </summary>
		HRESULT mHR;
	};

	/// <summary>
	/// Custom throw method that allows the user to more descriptively use throws.
	/// </summary>
	/// <param name="hr">HRESULT from a windows</param>
	/// <param name="message">User/author written description of the error.</param>
	inline void ThrowIfFailed(HRESULT hr, const char* const message = "")
	{
		if (FAILED(hr))
		{
			throw GameException(message, hr);
		}
	}
}