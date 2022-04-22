#pragma once
#include <cstdint>
#include "RTTI.h"
#include "Factory.h"
using namespace FieaGameEngine;
/// <summary>
/// A user-defined type that is used in Unit Tests.
/// </summary>
namespace UnitTestLibraryDesktop
{
	class Foo : public RTTI
	{
		RTTI_DECLARATIONS(Foo, RTTI);

	public:

		/// <summary>
		/// Initializes the Foo object with default values on declaration.
		/// </summary>
		Foo();

		/// <summary>
		/// Copy constructor that initializes the Foo object with another Foo object.
		/// </summary>
		/// <param name="right">The Foo object that is used to initialize the calling Foo object.</param>
		Foo(const Foo& right);

		/// <summary>
		/// Initializes the Foo object with the value in parameter.
		/// </summary>
		/// <param name="data">An int32_t that initializes the Foo class member variable _data.</param>
		Foo(std::int32_t data);

		~Foo();

		/// <summary>
		/// Returns a reference to member variable _data in a Foo object.
		/// </summary>
		/// <returns>A reference to the member variable _data in the Foo object.</returns>
		std::int32_t& Data() const;

		/// <summary>
		/// Overloading the assignment operator so that a Foo object can be assigned to another Foo object.
		/// If the parameter is a reference to itself then the function returns a reference to itself.
		/// </summary>
		/// <param name="right">The Foo object that _data is being copied from.</param>
		/// <returns>A reference to itself.</returns>
		Foo& operator=(const Foo& right);

		/// <summary>
		/// Overloading the equals operator so that a Foo object can be compared against another Foo object.
		/// Performs a comparison of memory location to see if referencing the same object.
		/// Performs a comparison of value to see if they are referencing equivalent values.
		/// </summary>
		/// <param name="right">The Foo object that it is comparing against.</param>
		/// <returns>TRUE if contents are equal. FALSE if contents are not equal.</returns>
		bool operator==(const Foo& right) const;

		/// <summary>
		/// Overloading the not equals operator so that a Foo object can be compared against another Foo object.
		/// Calls the operator== and passes the Foo object. Returns the inverse of the result.
		/// </summary>
		/// <param name="right">The Foo object that it is comparing against.</param>
		/// <returns>TRUE if contents are not equal. FALSE if contents are equal.</returns>
		bool operator!=(const Foo& right) const;

		std::string ToString() const override;

	private:
		std::int32_t* _data;
	};

	/// <summary>
	/// Functor that takes two Foo's as parameters and determines if they are equal.
	/// </summary>
	struct FooEqualityFunctor
	{
		bool operator()(const Foo& left, const Foo& right) const;
	};

	ConcreteFactory(Foo, RTTI)
}
