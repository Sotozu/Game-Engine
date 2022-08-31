#include "pch.h"
#include "VertexShader.h"
#include "GameException.h"
#include "D3DCompiler.h"


using namespace std;
using namespace gsl;
using namespace DirectX;
using namespace winrt;

namespace FieaGameEngine
{
	RTTI_DEFINITIONS(VertexShader)

	VertexShader::VertexShader(const vector<char>& compiledShader, const com_ptr<ID3D11VertexShader>& vertexShader, const std::wstring& filename) :
		mCompiledShader(compiledShader), mShader(vertexShader), _shaderFileName(filename)
	{
	}

	VertexShader::VertexShader(vector<char>&& compiledShader, const com_ptr<ID3D11VertexShader>& vertexShader, const std::wstring& filename) :
		mCompiledShader(compiledShader), mShader(vertexShader), _shaderFileName(filename)
	{
	}

	const vector<char>& VertexShader::CompiledShader() const
	{
		return mCompiledShader;
	}

	com_ptr<ID3D11VertexShader> VertexShader::Shader() const
	{
		return mShader;
	}

	com_ptr<ID3D11InputLayout> VertexShader::InputLayout() const
	{
		return mInputLayout;
	}

	void VertexShader::CreateInputLayout(not_null<ID3D11Device*> device, const span<const D3D11_INPUT_ELEMENT_DESC>& inputElementDescriptions, bool releaseCompiledShader)
	{
		mInputLayout = nullptr;
		ThrowIfFailed(device->CreateInputLayout(inputElementDescriptions.data(), narrow_cast<uint32_t>(inputElementDescriptions.size()), mCompiledShader.data(), mCompiledShader.size(), mInputLayout.put()), "ID3D11Device::CreateInputLayout() failed.");

		if (releaseCompiledShader)
		{
			mCompiledShader.clear();
			mCompiledShader.shrink_to_fit();
		}
	}

	HRESULT VertexShader::ReadShaderFileToBLob()
	{
		LPCWSTR sw = _shaderFileName.c_str();
		return D3DReadFileToBlob(sw, &_shaderData);
	}

	//OLD VERSION USED IN ActionMeshRenderer::Initialize
	void VertexShader::CreateInputLayout(ID3D11Device* pD3DDevice, ID3D11InputLayout** pInputLayout)
	{
		// Reflect shader info
		ID3D11ShaderReflection* pVertexShaderReflection = NULL;
		ReadShaderFileToBLob();
		ThrowIfFailed(D3DReflect( _shaderData->GetBufferPointer(),_shaderData->GetBufferSize(), IID_ID3D11ShaderReflection, (void**)&pVertexShaderReflection), "D3DReflect failed");

		// Get shader info
		D3D11_SHADER_DESC shaderDesc;
		pVertexShaderReflection->GetDesc(&shaderDesc);

		mInputLayoutDescription.clear();

		// Read input layout description from shader info
		for (uint32_t i = 0; i < shaderDesc.InputParameters; i++)
		{
			D3D11_SIGNATURE_PARAMETER_DESC paramDesc;
			pVertexShaderReflection->GetInputParameterDesc(i, &paramDesc);

			// fill out input element desc
			D3D11_INPUT_ELEMENT_DESC elementDesc;
			elementDesc.SemanticName = paramDesc.SemanticName;
			elementDesc.SemanticIndex = paramDesc.SemanticIndex;
			elementDesc.InputSlot = 0;
			elementDesc.AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
			elementDesc.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
			elementDesc.InstanceDataStepRate = 0;

			// determine DXGI format
			if (paramDesc.Mask == 1)
			{
				if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32) elementDesc.Format = DXGI_FORMAT_R32_UINT;
				else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32) elementDesc.Format = DXGI_FORMAT_R32_SINT;
				else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32) elementDesc.Format = DXGI_FORMAT_R32_FLOAT;
			}
			else if (paramDesc.Mask <= 3)
			{
				if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32) elementDesc.Format = DXGI_FORMAT_R32G32_UINT;
				else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32) elementDesc.Format = DXGI_FORMAT_R32G32_SINT;
				else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32) elementDesc.Format = DXGI_FORMAT_R32G32_FLOAT;
			}
			else if (paramDesc.Mask <= 7)
			{
				if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32) elementDesc.Format = DXGI_FORMAT_R32G32B32_UINT;
				else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32) elementDesc.Format = DXGI_FORMAT_R32G32B32_SINT;
				else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32) elementDesc.Format = DXGI_FORMAT_R32G32B32_FLOAT;
			}
			else if (paramDesc.Mask <= 15)
			{
				if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32) elementDesc.Format = DXGI_FORMAT_R32G32B32A32_UINT;
				else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32) elementDesc.Format = DXGI_FORMAT_R32G32B32A32_SINT;
				else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32) elementDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
			}

			//save element desc
			mInputLayoutDescription.push_back(elementDesc);
		}
		
		// Try to create Input Layout
		ThrowIfFailed( pD3DDevice->CreateInputLayout(&mInputLayoutDescription[0], static_cast<UINT>(mInputLayoutDescription.size()), _shaderData->GetBufferPointer(), _shaderData->GetBufferSize(), pInputLayout));
		
		//ThrowIfFailed(pD3DDevice->CreateInputLayout(&mInputLayoutDescription[0], static_cast<UINT>(mInputLayoutDescription.size()), _shaderData->GetBufferPointer(), _shaderData->GetBufferSize(), mInputLayout.put()));

		//Free allocation shader reflection memory
		pVertexShaderReflection->Release();
	}

	//NEW VERSION USED IN Material::Intialize
	void VertexShader::CreateInputLayout(ID3D11Device* pD3DDevice)
	{
		// Reflect shader info
		ID3D11ShaderReflection* pVertexShaderReflection = NULL;
		ReadShaderFileToBLob();
		ThrowIfFailed(D3DReflect(_shaderData->GetBufferPointer(), _shaderData->GetBufferSize(), IID_ID3D11ShaderReflection, (void**)&pVertexShaderReflection), "D3DReflect failed");

		// Get shader info
		D3D11_SHADER_DESC shaderDesc;
		pVertexShaderReflection->GetDesc(&shaderDesc);

		//mInputLayoutDescription.clear();

		// Read input layout description from shader info
		for (uint32_t i = 0; i < shaderDesc.InputParameters; i++)
		{
			D3D11_SIGNATURE_PARAMETER_DESC paramDesc;
			pVertexShaderReflection->GetInputParameterDesc(i, &paramDesc);

			// fill out input element desc
			D3D11_INPUT_ELEMENT_DESC elementDesc;
			elementDesc.SemanticName = paramDesc.SemanticName;
			elementDesc.SemanticIndex = paramDesc.SemanticIndex;
			elementDesc.InputSlot = 0;
			elementDesc.AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
			elementDesc.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
			elementDesc.InstanceDataStepRate = 0;

			// determine DXGI format
			if (paramDesc.Mask == 1)
			{
				if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32) elementDesc.Format = DXGI_FORMAT_R32_UINT;
				else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32) elementDesc.Format = DXGI_FORMAT_R32_SINT;
				else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32) elementDesc.Format = DXGI_FORMAT_R32_FLOAT;
			}
			else if (paramDesc.Mask <= 3)
			{
				if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32) elementDesc.Format = DXGI_FORMAT_R32G32_UINT;
				else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32) elementDesc.Format = DXGI_FORMAT_R32G32_SINT;
				else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32) elementDesc.Format = DXGI_FORMAT_R32G32_FLOAT;
			}
			else if (paramDesc.Mask <= 7)
			{
				if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32) elementDesc.Format = DXGI_FORMAT_R32G32B32_UINT;
				else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32) elementDesc.Format = DXGI_FORMAT_R32G32B32_SINT;
				else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32) elementDesc.Format = DXGI_FORMAT_R32G32B32_FLOAT;
			}
			else if (paramDesc.Mask <= 15)
			{
				if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32) elementDesc.Format = DXGI_FORMAT_R32G32B32A32_UINT;
				else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32) elementDesc.Format = DXGI_FORMAT_R32G32B32A32_SINT;
				else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32) elementDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
			}

			mInputLayoutDescription.push_back(elementDesc);

		}

		// Try to create Input Layout
		ThrowIfFailed(pD3DDevice->CreateInputLayout(&mInputLayoutDescription[0], static_cast<UINT>(mInputLayoutDescription.size()), _shaderData->GetBufferPointer(), _shaderData->GetBufferSize(), mInputLayout.put()));

		//Free allocation shader reflection memory
		pVertexShaderReflection->Release();
	}

	std::vector<D3D11_INPUT_ELEMENT_DESC>& VertexShader::GetInputLayoutDescription()
	{
		return mInputLayoutDescription;
	}


}