#pragma once
// Standard Libraries
#include <crtdbg.h>
#include <cstdint> 

// Microsoft Libraries
#include <CppUnitTest.h>
#include <gsl/pointers>

inline std::size_t operator "" _z(unsigned long long int x)
{
	return static_cast<size_t>(x);
}

//inline std::chrono::milliseconds operator "" _ms(unsigned long long int x)
//{
//	return static_cast<std::chrono::milliseconds>(x);
//}

