#pragma once

#include <chrono>
/// <summary>
/// Paul's code
/// </summary>
namespace FieaGameEngine
{
	class GameTime final
	{
	public:

		using Time = std::chrono::steady_clock;
		using ms = std::chrono::milliseconds;
		using float_sec = std::chrono::duration<float>;
		using float_time_point = std::chrono::time_point<Time, float_sec>;

		const std::chrono::high_resolution_clock::time_point& CurrentTime() const;
		void SetCurrentTime(const std::chrono::high_resolution_clock::time_point& currentTime);
		
		const std::chrono::milliseconds& TotalGameTime() const;
		void SetTotalGameTime(const std::chrono::milliseconds& totalGameTime);

		const std::chrono::milliseconds& ElapsedGameTime() const;		
		void SetElapsedGameTime(const std::chrono::milliseconds& elapsedGameTime);

		std::chrono::duration<float> TotalGameTimeSeconds() const;
		std::chrono::duration<float> ElapsedGameTimeSeconds() const;

	private:
		std::chrono::high_resolution_clock::time_point mCurrentTime;
		std::chrono::milliseconds mTotalGameTime{ 0 };
		std::chrono::milliseconds mElapsedGameTime{ 0 };
	};
}