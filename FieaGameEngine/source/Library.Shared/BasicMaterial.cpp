#include "pch.h"
#include "BasicMaterial.h"
#include "Game.h"
#include "GameException.h"
#include "VertexDeclarations.h"
#include "VertexShader.h"
#include "PixelShader.h"

using namespace std;
using namespace gsl;
using namespace DirectX;

namespace FieaGameEngine
{
	RTTI_DEFINITIONS(BasicMaterial)

	BasicMaterial::BasicMaterial(Game& game) : Material(game, BasicMaterial::TypeIdClass())
	{
	}	

	void BasicMaterial::SetSurfaceColor(const DirectX::XMFLOAT4& color)
	{
		SetSurfaceColor(reinterpret_cast<const float*>(&color));
	}

	uint32_t BasicMaterial::VertexSize() const
	{
		return sizeof(VertexPosition);
	}

	void BasicMaterial::Initialize()
	{


		_vertexShaderFilePath = "Shaders\\BasicVS.cso";
		_pixelShaderFilePath = "Shaders\\BasicPS.cso";

		Material::Initialize();


		assert(_vertexShaderFilePath != "");
		assert(_pixelShaderFilePath != "");

		auto& content = mGame->Content();
		auto vertexShader = content.Load<VertexShader>(Utility::ToWideString(_vertexShaderFilePath));
		SetShader(vertexShader);

		auto pixelShader = content.Load<PixelShader>(Utility::ToWideString(_pixelShaderFilePath));
		SetShader(pixelShader);

		auto direct3DDevice = mGame->Direct3DDevice();
		vertexShader->CreateInputLayout<VertexPosition>(direct3DDevice);
		SetInputLayout(vertexShader->InputLayout());

		//auto& content = mGame->Content();


		//auto vertexShader = content.Load<VertexShader>(L"Shaders\\BasicVS.cso");
		//SetShader(vertexShader);

		//auto pixelShader = content.Load<PixelShader>(L"Shaders\\BasicPS.cso");
		//SetShader(pixelShader);

		//vertexShader->CreateInputLayout<VertexPosition>(direct3DDevice);
		//SetInputLayout(vertexShader->InputLayout());

		//Here is the heart of the issue.
		//The constant buffers in my "Standard Shader"/"General Purpose Shader" need to cover a large set of data.
		//For one the vertex shader: The constant buffer needs to be able to cover all standard vertex data.
		//Vertices
		//Normals
		//Tangents
		//BiNormals
		//TextureCoordinates
		//VertexColors
		//FaceCount
		//Indices

		D3D11_BUFFER_DESC constantBufferDesc{ 0 };
		constantBufferDesc.ByteWidth = sizeof(XMFLOAT4X4);
		constantBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		ThrowIfFailed(direct3DDevice->CreateBuffer(&constantBufferDesc, nullptr, mVSConstantBuffer.put()), "ID3D11Device::CreateBuffer() failed.");
		AddConstantBuffer(ShaderStages::VS, mVSConstantBuffer.get());

		constantBufferDesc.ByteWidth = sizeof(XMFLOAT4);		
		ThrowIfFailed(direct3DDevice->CreateBuffer(&constantBufferDesc, nullptr, mPSConstantBuffer.put()), "ID3D11Device::CreateBuffer() failed.");
		AddConstantBuffer(ShaderStages::PS, mPSConstantBuffer.get());

		SetSurfaceColor(Colors::White.f);
	}

	void BasicMaterial::UpdateTransform(CXMMATRIX worldViewProjectionMatrix)
	{
		mGame->Direct3DDeviceContext()->UpdateSubresource(mVSConstantBuffer.get(), 0, nullptr, worldViewProjectionMatrix.r, 0, 0);
	}

	void BasicMaterial::UpdateTransform(FXMMATRIX worldViewProjectionMatrix, CXMMATRIX worldMatrix)
	{
		UNREFERENCED_PARAMETER(worldMatrix);
		mGame->Direct3DDeviceContext()->UpdateSubresource(mVSConstantBuffer.get(), 0, nullptr, worldViewProjectionMatrix.r, 0, 0);
	}


	void BasicMaterial::SetSurfaceColor(const float* color)
	{
		mGame->Direct3DDeviceContext()->UpdateSubresource(mPSConstantBuffer.get(), 0, nullptr, color, 0, 0);
	}

#pragma region Attributed Required Methods

	void BasicMaterial::Update(const GameState& gameState)
	{
		UNREFERENCED_PARAMETER(gameState);
	}

	bool BasicMaterial::Equals(const RTTI* rhs) const
	{
		const BasicMaterial* material = rhs->As<BasicMaterial>();
		if (material == nullptr) { return false; }

		return _vertexShaderFilePath == material->_vertexShaderFilePath &&
			_pixelShaderFilePath == material->_pixelShaderFilePath;
	}

	BasicMaterial::BasicMaterial() : Material(BasicMaterial::TypeIdClass())
	{

	}


	const Vector<Signature> BasicMaterial::Signatures()
	{
		return Vector<Signature>
		{
		};

	}

	//Required interface for Material 7/31/2022
	void BasicMaterial::UpdateCameraPosition(const DirectX::XMFLOAT3& position)
	{
		UNREFERENCED_PARAMETER(position);
	}

	const std::vector<D3D11_INPUT_ELEMENT_DESC>& BasicMaterial::GetInputElementDescription()
	{
		return VertexPosition::GetDescription();
	}

	void BasicMaterial::CreateVertexBuffer(gsl::not_null<ID3D11Device*> device, const Mesh& mesh, gsl::not_null<ID3D11Buffer**> vertexBuffer)
	{
		VertexPosition::CreateVertexBuffer(device, mesh, vertexBuffer);
	}

#pragma endregion

}