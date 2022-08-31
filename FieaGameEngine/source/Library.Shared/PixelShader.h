#pragma once

#include <vector>
#pragma once //TO DO added this trying to resolve
/*
1>PixelShaderReader.obj : warning LNK4006: "public: class std::shared_ptr<class FieaGameEngine::PixelShader> __cdecl FieaGameEngine::PixelShaderWithClassLinkageReader::operator()(class std::basic_string<wchar_t,struct std::char_traits<wchar_t>,class std::allocator<wchar_t> > const &)const " (??RPixelShaderWithClassLinkageReader@FieaGameEngine@@QEBA?AV?$shared_ptr@VPixelShader@FieaGameEngine@@@std@@AEBV?$basic_string@_WU?$char_traits@_W@std@@V?$allocator@_W@2@@3@@Z) already defined in PixelShaderReaderPaul.obj; second definition ignored
1>LINK : error LNK1218: warning treated as error; no output file generated
*/
#include "Shader.h"

namespace FieaGameEngine
{
	class PixelShader final : public Shader
	{
		RTTI_DECLARATIONS(PixelShader, Shader)

	public:
		PixelShader(const winrt::com_ptr<ID3D11PixelShader>& pixelShader);
		PixelShader(const PixelShader&) = default;
		PixelShader& operator=(const PixelShader&) = default;
		PixelShader(PixelShader&&) = default;
		PixelShader& operator=(PixelShader&&) = default;
		~PixelShader() = default;

		winrt::com_ptr<ID3D11PixelShader> Shader() const;



	private:
		winrt::com_ptr<ID3D11PixelShader> mShader;
	};
}
