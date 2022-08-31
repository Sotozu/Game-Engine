#pragma once

#include "ContentTypeReader.h"
#include "TextureCube.h"

namespace FieaGameEngine
{
	class TextureCubeReader : public ContentTypeReader<TextureCube>
	{
		RTTI_DECLARATIONS(TextureCubeReader, AbstractContentTypeReader)

	public:
		TextureCubeReader(Game& renderer);
		TextureCubeReader(const TextureCubeReader&) = default;
		TextureCubeReader& operator=(const TextureCubeReader&) = default;
		TextureCubeReader(TextureCubeReader&&) = default;
		TextureCubeReader& operator=(TextureCubeReader&&) = default;
		~TextureCubeReader() = default;

	protected:
		virtual std::shared_ptr<TextureCube> _Read(const std::wstring& assetName) override;
	};
}