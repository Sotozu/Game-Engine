#include "pch.h"
#include "IJsonParseHelper.h"

namespace FieaGameEngine
{
#pragma region IJsonParseHelper

	RTTI_DEFINITIONS(IJsonParseHelper)

#pragma endregion

#pragma region Data

	RTTI_DEFINITIONS(HelperData)

	void FieaGameEngine::HelperData::IncrementDepth() { ++_depth; }

	void FieaGameEngine::HelperData::DecrementDepth() { --_depth; }

	std::size_t FieaGameEngine::HelperData::Depth() { return _depth; }

#pragma endregion

}