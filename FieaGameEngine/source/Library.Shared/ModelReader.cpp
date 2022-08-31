#include "pch.h"
#include "ModelReader.h"
#include "Utility.h"

using namespace std;

namespace FieaGameEngine
{
	RTTI_DEFINITIONS(ModelReader)

	ModelReader::ModelReader(Game& game) :
		ContentTypeReader(game, ModelResource::TypeIdClass())
	{
	}

	shared_ptr<ModelResource> ModelReader::_Read(const wstring& assetName)
	{
		return make_shared<ModelResource>(Utility::ToString(assetName));
	}
}