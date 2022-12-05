#include "pch.h"
#include "PointLightMaterial.h"
#include "Game.h"
#include "GameException.h"
#include "VertexDeclarations.h"
#include "VertexShader.h"
#include "PixelShader.h"
#include "Texture2D.h"
#include "LightRegistry.h"
#include "light.h"
using namespace std;
using namespace std::string_literals;
using namespace gsl;
using namespace winrt;
using namespace DirectX;

namespace FieaGameEngine
{
	RTTI_DEFINITIONS(PointLightMaterial)

	PointLightMaterial::PointLightMaterial(Game& game, shared_ptr<Texture2D> colorMap, shared_ptr<Texture2D> specularMap) :
		Material(game, PointLightMaterial::TypeIdClass()), mColorMap(move(colorMap)), mSpecularMap(move(specularMap))
	{
	}

	com_ptr<ID3D11SamplerState> PointLightMaterial::SamplerState() const
	{
		return mSamplerState;
	}

	void PointLightMaterial::SetSamplerState(com_ptr<ID3D11SamplerState> samplerState)
	{
		assert(samplerState != nullptr);
		mSamplerState = samplerState;
		Material::SetSamplerState(ShaderStages::PS, mSamplerState.get());
	}

	shared_ptr<Texture2D> PointLightMaterial::ColorMap() const
	{
		return mColorMap;
	}

	void PointLightMaterial::SetColorMap(shared_ptr<Texture2D> texture)
	{
		assert(texture != nullptr);
		mColorMap = move(texture);
		ResetPixelShaderResources();
	}

	shared_ptr<Texture2D> PointLightMaterial::SpecularMap() const
	{
		return mSpecularMap;
	}

	void PointLightMaterial::SetSpecularMap(shared_ptr<Texture2D> texture)
	{
		assert(texture != nullptr);
		mSpecularMap = move(texture);
		ResetPixelShaderResources();
	}

	const XMFLOAT4& PointLightMaterial::AmbientColor() const
	{
		return mPixelCBufferPerFrameData.AmbientColor;
	}

	void PointLightMaterial::SetAmbientColor(const XMFLOAT4& color)
	{
		mPixelCBufferPerFrameData.AmbientColor = color;
		mPixelCBufferPerFrameDataDirty = true;
	}

	const XMFLOAT3& PointLightMaterial::LightPosition() const
	{
		return mPixelCBufferPerFrameData.LightPosition;
	}

	void PointLightMaterial::SetLightPosition(const XMFLOAT3& position)
	{
		mPixelCBufferPerFrameData.LightPosition = position;
		mPixelCBufferPerFrameDataDirty = true;

		mVertexCBufferPerFrameData.LightPosition = position;
		mVertexCBufferPerFrameDataDirty = true;
	}

	const float PointLightMaterial::LightRadius() const
	{
		return mVertexCBufferPerFrameData.LightRadius;
	}

	void PointLightMaterial::SetLightRadius(float radius)
	{
		mVertexCBufferPerFrameData.LightRadius = radius;
		mVertexCBufferPerFrameDataDirty = true;
	}

	const XMFLOAT4& PointLightMaterial::LightColor() const
	{
		return mPixelCBufferPerFrameData.LightColor;
	}

	void PointLightMaterial::SetLightColor(const XMFLOAT4& color)
	{
		mPixelCBufferPerFrameData.LightColor = color;
		mPixelCBufferPerFrameDataDirty = true;
	}

	const DirectX::XMFLOAT3& PointLightMaterial::SpecularColor() const
	{
		return mPixelCBufferPerObjectData.SpecularColor;
	}

	void PointLightMaterial::SetSpecularColor(const DirectX::XMFLOAT3& color)
	{
		mPixelCBufferPerObjectData.SpecularColor = color;
		mPixelCBufferPerObjectDataDirty = true;
	}

	const float PointLightMaterial::SpecularPower() const
	{
		return mPixelCBufferPerObjectData.SpecularPower;
	}

	void PointLightMaterial::SetSpecularPower(float power)
	{
		mPixelCBufferPerObjectData.SpecularPower = power;
		mPixelCBufferPerObjectDataDirty = true;
	}

	uint32_t PointLightMaterial::VertexSize() const
	{
		return sizeof(VertexPositionTextureNormal);
	}

	void PointLightMaterial::Initialize()
	{
		Material::Initialize();

		assert(_vertexShaderFilePath != "");
		assert(_pixelShaderFilePath != "");

		auto& content = mGame->GetContentManager();
		auto vertexShader = content.Load<VertexShader>(Utility::ToWideString(_vertexShaderFilePath));
		SetShader(vertexShader);

		auto pixelShader = content.Load<PixelShader>(Utility::ToWideString(_pixelShaderFilePath));
		SetShader(pixelShader);

		auto direct3DDevice = mGame->Direct3DDevice();
		vertexShader->CreateInputLayout<VertexPositionTextureNormal>(direct3DDevice);
		SetInputLayout(vertexShader->InputLayout());

		auto colorMap = content.Load<Texture2D>(Utility::ToWideString(_colorMap));
		auto specularMap = content.Load<Texture2D>(Utility::ToWideString(_specularMap));

		mColorMap = std::move(colorMap);
		mSpecularMap = std::move(specularMap);

		D3D11_BUFFER_DESC constantBufferDesc{ 0 };
		constantBufferDesc.ByteWidth = sizeof(VertexCBufferPerFrame);
		constantBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		ThrowIfFailed(direct3DDevice->CreateBuffer(&constantBufferDesc, nullptr, mVertexCBufferPerFrame.put()), "ID3D11Device::CreateBuffer() failed.");
		AddConstantBuffer(ShaderStages::VS, mVertexCBufferPerFrame.get());

		constantBufferDesc.ByteWidth = sizeof(VertexCBufferPerObject);
		ThrowIfFailed(direct3DDevice->CreateBuffer(&constantBufferDesc, nullptr, mVertexCBufferPerObject.put()), "ID3D11Device::CreateBuffer() failed.");
		AddConstantBuffer(ShaderStages::VS, mVertexCBufferPerObject.get());

		constantBufferDesc.ByteWidth = sizeof(PixelCBufferPerFrame);
		ThrowIfFailed(direct3DDevice->CreateBuffer(&constantBufferDesc, nullptr, mPixelCBufferPerFrame.put()), "ID3D11Device::CreateBuffer() failed.");
		AddConstantBuffer(ShaderStages::PS, mPixelCBufferPerFrame.get());

		constantBufferDesc.ByteWidth = sizeof(PixelCBufferPerObject);
		ThrowIfFailed(direct3DDevice->CreateBuffer(&constantBufferDesc, nullptr, mPixelCBufferPerObject.put()), "ID3D11Device::CreateBuffer() failed.");
		AddConstantBuffer(ShaderStages::PS, mPixelCBufferPerObject.get());

		auto direct3DDeviceContext = mGame->Direct3DDeviceContext();
		direct3DDeviceContext->UpdateSubresource(mVertexCBufferPerFrame.get(), 0, nullptr, &mVertexCBufferPerFrameData, 0, 0);
		direct3DDeviceContext->UpdateSubresource(mVertexCBufferPerObject.get(), 0, nullptr, &mVertexCBufferPerObjectData, 0, 0);
		direct3DDeviceContext->UpdateSubresource(mPixelCBufferPerFrame.get(), 0, nullptr, &mPixelCBufferPerFrameData, 0, 0);
		direct3DDeviceContext->UpdateSubresource(mPixelCBufferPerObject.get(), 0, nullptr, &mPixelCBufferPerObjectData, 0, 0);

		ResetPixelShaderResources();
		AddSamplerState(ShaderStages::PS, mSamplerState.get());
	}

	void PointLightMaterial::UpdateCameraPosition(const XMFLOAT3& position)
	{
		mPixelCBufferPerFrameData.CameraPosition = position;
		mPixelCBufferPerFrameDataDirty = true;
	}

	void PointLightMaterial::UpdateTransform(FXMMATRIX worldViewProjectionMatrix, CXMMATRIX worldMatrix)
	{
		XMStoreFloat4x4(&mVertexCBufferPerObjectData.WorldViewProjection, worldViewProjectionMatrix);
		XMStoreFloat4x4(&mVertexCBufferPerObjectData.World, worldMatrix);
		mGame->Direct3DDeviceContext()->UpdateSubresource(mVertexCBufferPerObject.get(), 0, nullptr, &mVertexCBufferPerObjectData, 0, 0);
	}


	//Simply exists because required by interface. This is from BasicMaterial. Which does not leverage material system appropriately.
	void PointLightMaterial::UpdateTransform(CXMMATRIX worldViewProjectionMatrix)
	{
		XMStoreFloat4x4(&mVertexCBufferPerObjectData.WorldViewProjection, worldViewProjectionMatrix);
		mGame->Direct3DDeviceContext()->UpdateSubresource(mVertexCBufferPerObject.get(), 0, nullptr, &mVertexCBufferPerObjectData, 0, 0);
	}


	void PointLightMaterial::BeginDraw()
	{
		Material::BeginDraw();

		Light* light = LightRegistry::GetLight();

		if (light != nullptr)
		{
			FXMVECTOR position{ light->GetTransform()._position.x,light->GetTransform()._position.x, light->GetTransform()._position.x, };

			XMFLOAT3 materialPosition;
			XMStoreFloat3(&materialPosition, position);
			SetLightPosition(materialPosition);
		}



		auto direct3DDeviceContext = mGame->Direct3DDeviceContext();

		if (mVertexCBufferPerFrameDataDirty)
		{
			direct3DDeviceContext->UpdateSubresource(mVertexCBufferPerFrame.get(), 0, nullptr, &mVertexCBufferPerFrameData, 0, 0);
			mVertexCBufferPerFrameDataDirty = false;
		}

		if (mPixelCBufferPerFrameDataDirty)
		{
			direct3DDeviceContext->UpdateSubresource(mPixelCBufferPerFrame.get(), 0, nullptr, &mPixelCBufferPerFrameData, 0, 0);
			mPixelCBufferPerFrameDataDirty = false;
		}

		if (mPixelCBufferPerObjectDataDirty)
		{
			direct3DDeviceContext->UpdateSubresource(mPixelCBufferPerObject.get(), 0, nullptr, &mPixelCBufferPerObjectData, 0, 0);
			mPixelCBufferPerObjectDataDirty = false;
		}
	}

	void PointLightMaterial::ResetPixelShaderResources()
	{
		Material::ClearShaderResources(ShaderStages::PS);
		ID3D11ShaderResourceView* shaderResources[] = { mColorMap->ShaderResourceView().get(), mSpecularMap->ShaderResourceView().get() };
		Material::AddShaderResources(ShaderStages::PS, shaderResources);
	}

#pragma region REQUIRED METHODS TO SUPPORT RTTI AND ATTRIBUTED DESIGN
		PointLightMaterial::PointLightMaterial() : Material(PointLightMaterial::TypeIdClass())
		{

		}

		gsl::owner<PointLightMaterial*> PointLightMaterial::Clone() const
		{
			return new PointLightMaterial(*this);
		}

		void PointLightMaterial::Update(const GameState& gameState)
		{
			UNREFERENCED_PARAMETER(gameState);
		}

		bool PointLightMaterial::Equals(const RTTI* rhs) const
		{
			const PointLightMaterial* material = rhs->As<PointLightMaterial>();
			if (material == nullptr) { return false; }

			return _colorMap == material->_colorMap &&
				_specularMap == material->_specularMap;
		}

		inline std::string PointLightMaterial::ToString() const
		{
			return "PointLightMaterial"s;
		}

		const Vector<Signature> PointLightMaterial::Signatures()
		{
			return Vector<Signature>
			{
				{ "_colorMap", DatumTypes::String, 1, offsetof(PointLightMaterial, _colorMap) },
				{ "_specularMap", DatumTypes::String, 1, offsetof(PointLightMaterial, _specularMap) },
			};
		}

		PointLightMaterial::PointLightMaterial(std::size_t typeId) : Material(typeId)
		{

		}

		const std::vector<D3D11_INPUT_ELEMENT_DESC>& PointLightMaterial::GetInputElementDescription()
		{
			return VertexPositionTextureNormal::GetDescription();
		}

		void PointLightMaterial::CreateVertexBuffer(gsl::not_null<ID3D11Device*> device, const Mesh& mesh, gsl::not_null<ID3D11Buffer**> vertexBuffer)
		{
			VertexPositionTextureNormal::CreateVertexBuffer(device, mesh, vertexBuffer);
		}


#pragma endregion

}