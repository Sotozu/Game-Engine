// Pauls code DirectXEssentialLiveLessons
#pragma once

#include <windows.h>
#include <string>
#include <functional>
#include <vector>
#include <memory>
#include <filesystem>

namespace FieaGameEngine
{
	/// <summary>
	/// Encapsulates Win32 API calls in a class using functional programming practices.
	/// </summary>
	class UtilityWin32 final
	{
	public:

		using WndProcHandler = std::function<bool(HWND, UINT, WPARAM, LPARAM)>;

		/// <summary>
		/// Creates a window with specific settings passed through parameters.
		/// </summary>
		/// <param name="window">Contains window class information.</param>
		/// <param name="windowHandle">HWNDs are essentially pointers (IntPtr) with values that make them (sort of) point to a window-structure data.</param>
		/// <param name="instance">The operating system uses this value to identify the executable (EXE) when it is loaded in memory.</param>
		/// <param name="className">"Identifier to help distinguish this window from the sea of windows on the operation system."</param>
		/// <param name="windowTitle">Title will be displayed at the top of the window.</param>
		/// <param name="renderTargetSize">typdef that holds the windows dimensions</param>
		/// <param name="showCommand">Set the specified windows show state. Received from the program entry point arguments list.</param>
		static void InitializeWindow(WNDCLASSEX& window, HWND& windowHandle, HINSTANCE instance, const std::wstring& className, const std::wstring& windowTitle, const SIZE& renderTargetSize, int showCommand);
		
		/// <summary>
		/// 
		/// </summary>
		/// <param name="windowHandle"></param>
		/// <param name="message"></param>
		/// <param name="wParam"></param>
		/// <param name="lParam"></param>
		/// <returns></returns>
		static LRESULT WINAPI WndProc(HWND windowHandle, UINT message, WPARAM wParam, LPARAM lParam);

		/// <summary>
		/// Helper function used to center the window to the middle of the screen.
		/// Implemented in the InitializeWindow method.
		/// </summary>
		/// <param name="windowSize">Typdef that holds the windows dimensions</param>
		/// <returns>The POINT structure defines the x- and y-coordinates of a point.</returns>
		static POINT CenterWindow(const SIZE& windowSize);

		/// <summary>
		/// Return an object of type path that describes the executable directory path.
		/// </summary>
		/// <returns>Executable directory path</returns>
		static std::filesystem::path ExecutableDirectory();

		/// <summary>
		/// Returns a reference to a vector of Window Handlers (effectively a vector of function pointers). 
		/// </summary>
		/// <returns>Non mutable reference to a vector of Window Handlers.</returns>
		static const std::vector<std::shared_ptr<WndProcHandler>>& WndProcHandlers();

		/// <summary>
		/// Adds Window Handler to the vector of Window Handlers. 
		/// </summary>
		/// <param name="handler">A Window Handler to be added to the vector of Window Handlers.</param>
		static void AddWndProcHandler(std::shared_ptr<WndProcHandler> handler);

		/// <summary>
		/// Removes a Window Handler from the vector of Window Handlers. 
		/// </summary>
		/// <param name="handler">A Window Handler to be removed from the vector of Window Handlers.</param>
		static void RemoveWndProcHandler(std::shared_ptr<WndProcHandler> handler);

		UtilityWin32() = delete;
		UtilityWin32(const UtilityWin32&) = delete;
		UtilityWin32& operator=(const UtilityWin32&) = delete;
		UtilityWin32(UtilityWin32&&) = delete;
		UtilityWin32& operator=(UtilityWin32&&) = delete;
		~UtilityWin32() = default;

	private:
		/// <summary>
		/// Accessible globally since it has been made static
		/// </summary>
		static std::vector<std::shared_ptr<WndProcHandler>> sWndProcHandlers;
	};
}