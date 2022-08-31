#include "Factory.h"
#include "RTTI.h"

using namespace FieaGameEngine;
namespace UnitTestLibraryDesktop
{
	class FooFactory final 
	{
	public:
		friend Factory<RTTI>;
		FooFactory();
	protected:
		~FooFactory();
	private:
	};
}
