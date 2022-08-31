#include "pch.h"
#include "IJsonCoordinator.h"
#include <fstream>
#include <iostream>

namespace FieaGameEngine
{
	//Destructor
	IJsonCoordinator::~IJsonCoordinator()
	{
		for (std::size_t index = 0; index < _myHelpers.Size(); ++index)
		{
			delete _myHelpers[index];
		}
		_myHelpers.Clear();
	}

	//Copy Constructor
	IJsonCoordinator::IJsonCoordinator(IJsonCoordinator& rhs)
	{
		for (std::size_t index = 0; index < rhs._myHelpers.Size(); ++index)
		{
			_myHelpers.PushBack(rhs._myHelpers[index]->Create());
		}
	}

	gsl::owner<IJsonCoordinator*> IJsonCoordinator::Clone()
	{
		return new IJsonCoordinator(*this);
	}

}