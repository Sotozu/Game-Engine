#include "pch.h"
#include "TextureMaterial.h"
#include "Game.h"
#include "GameException.h"
#include "VertexDeclarations.h"
#include "VertexShader.h"
#include "PixelShader.h"
#include "Texture2D.h"

using namespace std;
using namespace std::string_literals;
using namespace gsl;
using namespace winrt;
using namespace DirectX;

namespace FieaGameEngine
{
	RTTI_DEFINITIONS(TextureMaterial);

	TextureMaterial::TextureMaterial(Game& game, shared_ptr<Texture2D> colorMap) :
		Material(game, TextureMaterial::TypeIdClass()), mColorMap(move(colorMap))
	{
	}

	com_ptr<ID3D11SamplerState> TextureMaterial::SamplerState() const
	{
		return mSamplerState;
	}

	void TextureMaterial::SetSamplerState(com_ptr<ID3D11SamplerState> samplerState)
	{
		assert(samplerState != nullptr);
		mSamplerState = samplerState;
		Material::SetSamplerState(ShaderStages::PS, mSamplerState.get());
	}

	shared_ptr<Texture2D> TextureMaterial::ColorMap() const
	{
		return mColorMap;
	}

	void TextureMaterial::SetColorMap(shared_ptr<Texture2D> texture)
	{
		assert(texture != nullptr);
		mColorMap = move(texture);
		ResetPixelShaderResources();
	}

	uint32_t TextureMaterial::VertexSize() const
	{
		return sizeof(VertexPositionTexture);
	}

	void TextureMaterial::Initialize()
	{
		Material::Initialize();

		assert(_vertexShaderFilePath != "");
		assert(_pixelShaderFilePath != "");

		auto& content = mGame->Content();
		auto vertexShader = content.Load<VertexShader>(Utility::ToWideString(_vertexShaderFilePath));
		SetShader(vertexShader);

		auto pixelShader = content.Load<PixelShader>(Utility::ToWideString(_pixelShaderFilePath));
		SetShader(pixelShader);

		auto direct3DDevice = mGame->Direct3DDevice();
		vertexShader->CreateInputLayout<VertexPositionTexture>(direct3DDevice);
		SetInputLayout(vertexShader->InputLayout());

		auto colorMap = mGame->Content().Load<Texture2D>(Utility::ToWideString(_colorMap));
		mColorMap = std::move(colorMap);

		D3D11_BUFFER_DESC constantBufferDesc{ 0 };
		constantBufferDesc.ByteWidth = sizeof(VertexCBufferPerObject);
		constantBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		ThrowIfFailed(direct3DDevice->CreateBuffer(&constantBufferDesc, nullptr, mVertexCBufferPerObject.put()), "ID3D11Device::CreateBuffer() failed.");
		AddConstantBuffer(ShaderStages::VS, mVertexCBufferPerObject.get());

		auto direct3DDeviceContext = mGame->Direct3DDeviceContext();
		direct3DDeviceContext->UpdateSubresource(mVertexCBufferPerObject.get(), 0, nullptr, &mVertexCBufferPerObjectData, 0, 0);

		ResetPixelShaderResources();
		AddSamplerState(ShaderStages::PS, mSamplerState.get());
	}

	void TextureMaterial::UpdateCameraPosition(const XMFLOAT3& position)
	{
		UNREFERENCED_PARAMETER(position);
		//mPixelCBufferPerFrameData.CameraPosition = position;
		//mPixelCBufferPerFrameDataDirty = true;
	}

	void TextureMaterial::UpdateTransform(FXMMATRIX worldViewProjectionMatrix, CXMMATRIX worldMatrix)
	{
		UNREFERENCED_PARAMETER(worldMatrix);
		XMStoreFloat4x4(&mVertexCBufferPerObjectData.WorldViewProjection, worldViewProjectionMatrix);
		mGame->Direct3DDeviceContext()->UpdateSubresource(mVertexCBufferPerObject.get(), 0, nullptr, &mVertexCBufferPerObjectData, 0, 0);
	}


	//Simply exists because required by interface. This is from BasicMaterial. Which does not leverage material system appropriately.
	void TextureMaterial::UpdateTransform(CXMMATRIX worldViewProjectionMatrix)
	{
		XMStoreFloat4x4(&mVertexCBufferPerObjectData.WorldViewProjection, worldViewProjectionMatrix);
		mGame->Direct3DDeviceContext()->UpdateSubresource(mVertexCBufferPerObject.get(), 0, nullptr, &mVertexCBufferPerObjectData, 0, 0);
	}


	void TextureMaterial::BeginDraw()
	{
		Material::BeginDraw();
	}

	void TextureMaterial::ResetPixelShaderResources()
	{
		Material::ClearShaderResources(ShaderStages::PS);
		ID3D11ShaderResourceView* shaderResources[] = { mColorMap->ShaderResourceView().get()};
		Material::AddShaderResources(ShaderStages::PS, shaderResources);

	}

#pragma region REQUIRED METHODS TO SUPPORT RTTI AND ATTRIBUTED DESIGN
	TextureMaterial::TextureMaterial() : Material(TextureMaterial::TypeIdClass())
	{
	}

	gsl::owner<TextureMaterial*> TextureMaterial::Clone() const
	{
		return new TextureMaterial(*this);
	}

	void TextureMaterial::Update(const GameState& gameState)
	{
		UNREFERENCED_PARAMETER(gameState);
	}

	bool TextureMaterial::Equals(const RTTI* rhs) const
	{
		const TextureMaterial* material = rhs->As<TextureMaterial>();
		if (material == nullptr) { return false; }

		return _colorMap == material->_colorMap;
	}

	inline std::string TextureMaterial::ToString() const
	{
		return "TextureMaterial"s;
	}

	const Vector<Signature> TextureMaterial::Signatures()
	{
		return Vector<Signature>
		{
			{ "_colorMap", DatumTypes::String, 1, offsetof(TextureMaterial, _colorMap) },
		};
	}

	TextureMaterial::TextureMaterial(std::size_t typeId) : Material(typeId)
	{

	}

	const std::vector<D3D11_INPUT_ELEMENT_DESC>& TextureMaterial::GetInputElementDescription()
	{
		return VertexPositionTexture::GetDescription();
	}

	void TextureMaterial::CreateVertexBuffer(gsl::not_null<ID3D11Device*> device, const Mesh& mesh, gsl::not_null<ID3D11Buffer**> vertexBuffer)
	{
		VertexPositionTexture::CreateVertexBuffer(device, mesh, vertexBuffer);
	}

#pragma endregion

}