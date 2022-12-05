#include "pch.h"
#include "SkyboxMaterial.h"
#include "Game.h"
#include "GameException.h"
#include "VertexShader.h"
#include "PixelShader.h"
#include "VertexDeclarations.h"
#include "TextureCube.h"
#include "RasterizerStates.h"

using namespace std;
using namespace gsl;
using namespace DirectX;
using namespace winrt;

namespace FieaGameEngine
{
	RTTI_DEFINITIONS(SkyboxMaterial)

	SkyboxMaterial::SkyboxMaterial(Game& game, const shared_ptr<TextureCube>& texture, const com_ptr<ID3D11SamplerState>& samplerState) :
		Material(game),
		mTexture(texture), mSamplerState(samplerState)
	{
	}

	com_ptr<ID3D11SamplerState> SkyboxMaterial::SamplerState() const
	{
		return mSamplerState;
	}

	void SkyboxMaterial::SetSamplerState(const com_ptr<ID3D11SamplerState>& samplerState)
	{
		assert(samplerState != nullptr);
		mSamplerState = samplerState;
		Material::SetSamplerState(ShaderStages::PS, mSamplerState.get());
	}

	shared_ptr<TextureCube> SkyboxMaterial::Texture() const
	{
		return mTexture;
	}

	void SkyboxMaterial::SetTexture(shared_ptr<TextureCube> texture)
	{
		assert(texture != nullptr);
		mTexture = move(texture);
		Material::SetShaderResource(ShaderStages::PS, mTexture->ShaderResourceView().get());
	}

	uint32_t SkyboxMaterial::VertexSize() const
	{
		return sizeof(VertexPosition);
	}

	void SkyboxMaterial::Initialize()
	{
		Material::Initialize();

		auto& content = mGame->GetContentManager();
		auto vertexShader = content.Load<VertexShader>(L"Shaders\\SkyboxVS.cso");
		SetShader(vertexShader);

		auto pixelShader = content.Load<PixelShader>(L"Shaders\\SkyboxPS.cso");
		SetShader(pixelShader);

		auto direct3DDevice = mGame->Direct3DDevice();
		vertexShader->CreateInputLayout<VertexPosition>(direct3DDevice);
		SetInputLayout(vertexShader->InputLayout());

		D3D11_BUFFER_DESC constantBufferDesc{ 0 };
		constantBufferDesc.ByteWidth = sizeof(XMFLOAT4X4);
		constantBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		ThrowIfFailed(mGame->Direct3DDevice()->CreateBuffer(&constantBufferDesc, nullptr, mConstantBuffer.put()), "ID3D11Device::CreateBuffer() failed.");
		AddConstantBuffer(ShaderStages::VS, mConstantBuffer.get());

		AddShaderResource(ShaderStages::PS, mTexture->ShaderResourceView().get());
		AddSamplerState(ShaderStages::PS, mSamplerState.get());
	}

	void SkyboxMaterial::BeginDraw()
	{
		Material::BeginDraw();

		mGame->Direct3DDeviceContext()->RSSetState(RasterizerStates::DisabledCulling.get());
	}

	void SkyboxMaterial::EndDraw()
	{
		Material::EndDraw();

		mGame->Direct3DDeviceContext()->RSSetState(nullptr);
	}
	void SkyboxMaterial::UpdateTransform(FXMMATRIX worldViewProjectionMatrix, CXMMATRIX worldMatrix)
	{
		UNREFERENCED_PARAMETER(worldMatrix);
		mGame->Direct3DDeviceContext()->UpdateSubresource(mConstantBuffer.get(), 0, nullptr, worldViewProjectionMatrix.r, 0, 0);
	}

	void SkyboxMaterial::UpdateTransform(CXMMATRIX worldViewProjectionMatrix)
	{
		mGame->Direct3DDeviceContext()->UpdateSubresource(mConstantBuffer.get(), 0, nullptr, worldViewProjectionMatrix.r, 0, 0);
	}

	//Required interface for Material 7/31/2022
	void SkyboxMaterial::UpdateCameraPosition(const DirectX::XMFLOAT3& position)
	{
		UNREFERENCED_PARAMETER(position);
	}


	const std::vector<D3D11_INPUT_ELEMENT_DESC>& SkyboxMaterial::GetInputElementDescription()
	{
		return VertexPosition::GetDescription();
	}
	void SkyboxMaterial::CreateVertexBuffer(gsl::not_null<ID3D11Device*> device, const Mesh& mesh, gsl::not_null<ID3D11Buffer**> vertexBuffer)
	{
		VertexPosition::CreateVertexBuffer(device, mesh, vertexBuffer);

	}

}