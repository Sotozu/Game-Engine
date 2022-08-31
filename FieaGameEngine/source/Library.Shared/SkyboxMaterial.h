#pragma once

#include "Material.h"
#include "SamplerStates.h"

namespace FieaGameEngine
{
	class TextureCube;

	class SkyboxMaterial final : public Material
	{
		RTTI_DECLARATIONS(SkyboxMaterial, Material)

	public:
		SkyboxMaterial(Game& game, const std::shared_ptr<TextureCube>& texture, const winrt::com_ptr<ID3D11SamplerState>& samplerState = SamplerStates::TrilinearClamp);
		SkyboxMaterial(const SkyboxMaterial&) = default;
		SkyboxMaterial& operator=(const SkyboxMaterial&) = default;
		SkyboxMaterial(SkyboxMaterial&&) = default;
		SkyboxMaterial& operator=(SkyboxMaterial&&) = default;
		~SkyboxMaterial() = default;

		winrt::com_ptr<ID3D11SamplerState> SamplerState() const;
		void SetSamplerState(const winrt::com_ptr<ID3D11SamplerState>& samplerState);

		std::shared_ptr<TextureCube> Texture() const;
		void SetTexture(std::shared_ptr<TextureCube> texture);

		virtual std::uint32_t VertexSize() const override;
		virtual void Initialize() override;

		virtual void UpdateTransform(DirectX::CXMMATRIX worldViewProjectionMatrix) override;
		virtual void UpdateTransform(DirectX::FXMMATRIX worldViewProjectionMatrix, DirectX::CXMMATRIX worldMatrix) override;

		virtual void UpdateCameraPosition(const DirectX::XMFLOAT3& position) override;

		//NEEDS TO BE CHANGED. JUST NEED TO IMPLEMENT FOR NOW FOR INTERFACE COMPATABILITY
		virtual const std::vector<D3D11_INPUT_ELEMENT_DESC>& GetInputElementDescription() override;
		virtual void CreateVertexBuffer(gsl::not_null<ID3D11Device*> device, const Mesh& mesh, gsl::not_null<ID3D11Buffer**> vertexBuffer) override;

	private:
		virtual void BeginDraw() override;
		virtual void EndDraw() override;

		winrt::com_ptr<ID3D11Buffer> mConstantBuffer;
		std::shared_ptr<TextureCube> mTexture;
		winrt::com_ptr<ID3D11SamplerState> mSamplerState;
	};
}