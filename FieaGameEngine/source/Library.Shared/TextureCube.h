#pragma once

#include "Texture.h"

namespace FieaGameEngine
{
	class TextureCube final : public Texture
	{
		RTTI_DECLARATIONS(TextureCube, Texture)

	public:
		TextureCube(const TextureCube&) = default;
		TextureCube& operator=(const TextureCube&) = default;
		TextureCube(TextureCube&&) = default;
		TextureCube& operator=(TextureCube&&) = default;
		~TextureCube() = default;

	private:
		friend class TextureCubeReader;
		friend class TextureCubeReaderPaul; //TO DO** DELETE THIS EVENTUALLY

		TextureCube(const winrt::com_ptr<ID3D11ShaderResourceView>& shaderResourceView);
	};
}