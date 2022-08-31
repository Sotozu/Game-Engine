#pragma once

#include <vector>
#include "Shader.h"

namespace FieaGameEngine
{
	class VertexShader final : public Shader
	{
		RTTI_DECLARATIONS(VertexShader, Shader)

	public:
		VertexShader(const VertexShader&) = default;
		VertexShader& operator=(const VertexShader&) = default;
		VertexShader(VertexShader&&) = default;
		VertexShader& operator=(VertexShader&&) = default;
		~VertexShader() = default;

		const std::vector<char>& CompiledShader() const;
		winrt::com_ptr<ID3D11VertexShader> Shader() const;
		winrt::com_ptr<ID3D11InputLayout> InputLayout() const;

		template <typename T>
		void CreateInputLayout(gsl::not_null<ID3D11Device*> device, bool releaseCompiledShader = false)
		{
			CreateInputLayout(device, T::InputElements, releaseCompiledShader);
		}

		void CreateInputLayout(gsl::not_null<ID3D11Device*> device, const gsl::span<const D3D11_INPUT_ELEMENT_DESC>& inputElementDescriptions, bool releaseCompiledShader = false);
		
		/// <summary>
		/// Creates Input Layout Description Structure(s) that represents the data that the shader requires for processing.
		/// </summary>
		/// <param name="pD3DDevice">D3D Device</param>
		/// <param name="pInputLayout"></param>
		void CreateInputLayout(ID3D11Device* pD3DDevice, ID3D11InputLayout** pInputLayout);
		void CreateInputLayout(ID3D11Device* pD3DDevice);
		std::vector<D3D11_INPUT_ELEMENT_DESC>& GetInputLayoutDescription();

	private:
		HRESULT ReadShaderFileToBLob();

		friend class VertexShaderReader;
		VertexShader(const std::vector<char>& compiledShader, const winrt::com_ptr<ID3D11VertexShader>& vertexShader, const std::wstring&);
		VertexShader(std::vector<char>&& compiledShader, const winrt::com_ptr<ID3D11VertexShader>& vertexShader, const std::wstring&);

		//ADDED 7/5/2022
		const std::wstring _shaderFileName;
		std::vector<char> mCompiledShader;
		winrt::com_ptr<ID3D11VertexShader> mShader;
		winrt::com_ptr<ID3D11InputLayout> mInputLayout;

		std::vector<D3D11_INPUT_ELEMENT_DESC> mInputLayoutDescription;

		ID3DBlob* _shaderData;
		bool descriptionWritten{ false };
	};
}
