#pragma once

#include "DirectXHelper.h"
#include "Shader.h"
#include <map>
#include <vector>
#include <functional>
#include "Attributed.h"
#include "TypeRegistry.h"
#include "Factory.h"

using namespace gsl;

namespace FieaGameEngine
{
	class Game;
	class GameState;
	class Mesh;

	class Material : public Attributed
	{
		RTTI_DECLARATIONS(Material, Attributed)

	public:
#pragma region Attributed Required Methods

		/// <summary>
		/// Default Constructor
		/// Required for Factory and de-serialization
		/// </summary>
		Material(D3D_PRIMITIVE_TOPOLOGY topology = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		Material(Game& game, std::size_t typeId, D3D_PRIMITIVE_TOPOLOGY topology = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);


		/// <summary>
		/// Returns a string that identifies this class.
		/// Overridden function from RTTI.
		/// </summary>
		/// <returns>A string that identifies this class</returns>
		inline std::string ToString() const override;

		/// <summary>
		/// Compares one GameObject with another.
		/// Overridden function from RTTI.
		/// </summary>
		/// <param name="rhs">The GameObject to compare against</param>
		/// <returns>
		/// TRUE if the GameObject is equivalent.
		/// FALSE if the GameObject is not equivalent
		/// </returns>
		bool Equals(const RTTI* rhs) const override;


		/// <summary>
		/// Derived actions will implement their unique Update method.
		/// </summary>
		/// <param name="gameTime">A reference to the game time</param>
		virtual void Update(const GameState& gameState);


		/// <summary>
		/// Method is used for registering this types signatures and for classes that inherit from Action.
		/// Used by TypeRegistry class
		/// </summary>
		/// <returns>
		/// This classes unique signatures.
		/// </returns>
		static const Vector<Signature> Signatures();


		/// <summary>
		/// Macros is defined in TypeRegistry header file.
		/// This is used to register this class with the TypeRegistry.
		/// Will start a recursive call that will register all classes that are inherited.
		/// </summary>
		/// <param name="">Derived Class</param>
		/// <param name="">Base Class</param>
		StaticRegisterMethod(Material, Attributed);

		virtual void UpdateTransform(DirectX::CXMMATRIX worldViewProjectionMatrix) = 0;
		virtual void UpdateTransform(DirectX::FXMMATRIX worldViewProjectionMatrix, DirectX::CXMMATRIX worldMatrix) = 0;

		virtual void UpdateCameraPosition(const DirectX::XMFLOAT3&) = 0;

		virtual void CreateVertexBuffer(gsl::not_null<ID3D11Device*> device, const Mesh& mesh, gsl::not_null<ID3D11Buffer**> vertexBuffer) = 0;

	protected:
		Material(std::size_t typeId, D3D_PRIMITIVE_TOPOLOGY topology = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		std::string  _vertexShaderFilePath;
		std::string  _pixelShaderFilePath;


#pragma endregion
	public:

		struct ShaderStageData
		{
			std::shared_ptr<Shader> Shader;
			ID3D11ClassInstance* ShaderClassInstance{nullptr};
			std::vector<ID3D11Buffer*> ConstantBuffers;
			std::vector<ID3D11ShaderResourceView*> ShaderResources;
			std::vector<ID3D11SamplerState*> SamplerStates;
		};

		Material(Game& game, D3D_PRIMITIVE_TOPOLOGY topology = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		D3D_PRIMITIVE_TOPOLOGY Topology() const;
		void SetTopology(D3D_PRIMITIVE_TOPOLOGY topology);

		winrt::com_ptr<ID3D11InputLayout> InputLayout() const;
		virtual const std::vector<D3D11_INPUT_ELEMENT_DESC>& GetInputElementDescription() = 0;

		void SetInputLayout(winrt::com_ptr<ID3D11InputLayout> inputLayout);

		std::shared_ptr<Shader> GetShader(ShaderStages shaderStage);
		void SetShader(ShaderStages shaderStage, std::shared_ptr<Shader> shader);

		template <typename T>
		std::shared_ptr<T> GetShader();

		template <typename T>
		void SetShader(std::shared_ptr<T> shader);

		ID3D11ClassInstance* GetShaderClassInstance(ShaderStages shaderStage);
		void SetShaderClassInstance(ShaderStages shaderStage, ID3D11ClassInstance* classInstance);

		void SetConstantBuffer(ShaderStages shaderStage, ID3D11Buffer* constantBuffer);
		void AddConstantBuffer(ShaderStages shaderStage, ID3D11Buffer* constantBuffer);
		void AddConstantBuffers(ShaderStages shaderStage, const gsl::span<ID3D11Buffer*>& constantBuffers);
		void RemoveConstantBuffer(ShaderStages shaderStage, ID3D11Buffer* constantBuffer);
		void ClearConstantBuffers(ShaderStages shaderStage);

		void SetShaderResource(ShaderStages shaderStage, ID3D11ShaderResourceView* shaderResource);
		void AddShaderResource(ShaderStages shaderStage, ID3D11ShaderResourceView* shaderResource);
		void AddShaderResources(ShaderStages shaderStage, const gsl::span<ID3D11ShaderResourceView*>& shaderResources);
		void RemoveShaderResource(ShaderStages shaderStage, ID3D11ShaderResourceView* shaderResource);
		void ClearShaderResources(ShaderStages shaderStage);

		void SetSamplerState(ShaderStages shaderStage, ID3D11SamplerState* samplerState);
		void AddSamplerState(ShaderStages shaderStage, ID3D11SamplerState* samplerState);
		void AddSamplerStates(ShaderStages shaderStage, const gsl::span<ID3D11SamplerState*>& samplerStates);
		void RemoveSamplerState(ShaderStages shaderStage, ID3D11SamplerState* samplerState);
		void ClearSamplerStates(ShaderStages shaderStage);

		std::function<void()> DrawCallback() const;
		void SetDrawCallback(std::function<void()> callback);

		std::function<void()> UpdateMaterialCallback() const;
		void SetUpdateMaterialCallback(std::function<void()> callback);

		std::pair<bool, ShaderStageData*> GetDataForShaderStage(ShaderStages shaderStage);
		bool AutoUnbindShaderResourcesEnabled() const;
		void SetAutoUnbindShaderResourcesEnabled(bool enabled);
		
		template <size_t _Count>
		static void UnbindShaderResources(gsl::not_null<ID3D11DeviceContext*> direct3DDeviceContext, ShaderStages shaderStage, std::uint32_t startSlot = 0);

		template <size_t _Count>
		void UnbindShaderResources(ShaderStages shaderStage, std::uint32_t startSlot = 0);

		virtual void Initialize();
		virtual void Draw();
		virtual void Draw(gsl::not_null<ID3D11Buffer*> vertexBuffer, std::uint32_t vertexCount, std::uint32_t startVertexLocation = 0, std::uint32_t offset = 0);
		virtual void DrawIndexed(gsl::not_null<ID3D11Buffer*> vertexBuffer, gsl::not_null<ID3D11Buffer*> indexBuffer, std::uint32_t indexCount, DXGI_FORMAT format = DXGI_FORMAT_R32_UINT, std::uint32_t startIndexLocation = 0, std::uint32_t baseVertexLocation = 0, std::uint32_t vertexOffset = 0, std::uint32_t indexOffset = 0);
		virtual std::uint32_t VertexSize() const = 0;
	private:
		struct ShaderStageCallInfo
		{
			std::function<void(ID3D11DeviceContext&, const ShaderStageData&)> SetShader;
			std::function<void(ID3D11DeviceContext*, UINT, UINT, ID3D11Buffer* const *)> SetConstantBuffers;
			std::function<void(ID3D11DeviceContext*, UINT, UINT, ID3D11ShaderResourceView* const *)> SetShaderResources;
			std::function<void(ID3D11DeviceContext*, UINT, UINT, ID3D11SamplerState* const *)> SetSamplers;			
		};

		static void SetVSShader(ID3D11DeviceContext& direct3DDeviceContext, const ShaderStageData& shaderStageData);		
		static void SetPSShader(ID3D11DeviceContext& direct3DDeviceContext, const ShaderStageData& shaderStageData);

		//std::shared_ptr<Shader>& GetVSShader();
		//std::shared_ptr<Shader>& GetPSShader();
	protected:
		static const std::map<ShaderStages, RTTI::IdType> ShaderStageTypeMap;
		static const std::map<RTTI::IdType, ShaderStages> TypeShaderStageMap;
		static const std::map<ShaderStages, ShaderStageCallInfo> ShaderStageCalls;
		static std::vector<ID3D11ShaderResourceView*> EmptySRVs;

		virtual void BeginDraw();
		virtual void EndDraw();

		Game* mGame;
		D3D_PRIMITIVE_TOPOLOGY mTopology;
		winrt::com_ptr<ID3D11InputLayout> mInputLayout;
		std::map<ShaderStages, ShaderStageData> mShaderStageData;

		std::function<void()> mDrawCallback;
		std::function<void()> mUpdateMaterialCallback;
		bool mAutoUnbindShaderResourcesEnabled{ false };
	};

}

#include "Material.inl" 