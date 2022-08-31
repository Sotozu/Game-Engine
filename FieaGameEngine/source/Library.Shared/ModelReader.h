#pragma once

#include "ContentTypeReader.h"
#include "ModelResource.h"
#include "Game.h"

namespace FieaGameEngine
{
	class ModelReader : public ContentTypeReader<ModelResource>
	{
		RTTI_DECLARATIONS(ModelReader, AbstractContentTypeReader)

	public:
		ModelReader(Game& game);
		ModelReader(const ModelReader&) = default;
		ModelReader& operator=(const ModelReader&) = default;
		ModelReader(ModelReader&&) = default;
		ModelReader& operator=(ModelReader&&) = default;
		~ModelReader() = default;

	protected:
		virtual std::shared_ptr<ModelResource> _Read(const std::wstring& assetName) override;
	};
}