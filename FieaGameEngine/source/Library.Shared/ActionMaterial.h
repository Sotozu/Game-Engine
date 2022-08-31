#pragma once

#include "DirectXHelper.h"
#include "Action.h"
#include "Factory.h"
#include "Utility.h"
#include "Shader.h"
#include <map>
#include <vector>
#include <functional>

namespace FieaGameEngine
{
	class Game;

	class ActionMaterial : public Action
	{
		RTTI_DECLARATIONS(ActionMaterial, RTTI)

	public:
		struct ShaderStageData
		{
			std::shared_ptr<Shader> Shader;
			ID3D11ClassInstance* ShaderClassInstance{ nullptr };
			std::vector<ID3D11Buffer*> ConstantBuffers;
			std::vector<ID3D11ShaderResourceView*> ShaderResources;
			std::vector<ID3D11SamplerState*> SamplerStates;
		};

		/// <summary>
		/// Default Constructor
		/// Required for Factory and de-serialization
		/// </summary>
		ActionMaterial();


		explicit ActionMaterial(Game& game, D3D_PRIMITIVE_TOPOLOGY topology = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);


		//START - ADDED 7/20/2022

		/// <summary>
		/// Returns a string that identifies this class.
		/// Overridden function from RTTI.
		/// </summary>
		/// <returns>A string that identifies this class</returns>
		inline std::string ToString() const override;


		/// <summary>
		/// Clones the calling Action. Heap constructs this object and it's on the user to manage its memory.
		/// Overridden function from Scope.
		/// </summary>
		/// <returns>
		/// A pointer to heap allocated clone of the calling Action.
		/// </returns>
		inline gsl::owner<ActionMaterial*> Clone() const override;

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
		StaticRegisterMethod(ActionMaterial, Action);


		//END - ADDED 7/20/2022

		D3D_PRIMITIVE_TOPOLOGY Topology() const;
		void SetTopology(D3D_PRIMITIVE_TOPOLOGY topology);

		winrt::com_ptr<ID3D11InputLayout> InputLayout() const;
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
		virtual std::uint32_t VertexSize() const;

	private:
		std::string  _materialFilePath;
		std::string  _vertexShaderFilePath;
		std::string  _pixelShaderFilePath;

		struct ShaderStageCallInfo
		{
			std::function<void(ID3D11DeviceContext&, const ShaderStageData&)> SetShader;
			std::function<void(ID3D11DeviceContext*, UINT, UINT, ID3D11Buffer* const*)> SetConstantBuffers;
			std::function<void(ID3D11DeviceContext*, UINT, UINT, ID3D11ShaderResourceView* const*)> SetShaderResources;
			std::function<void(ID3D11DeviceContext*, UINT, UINT, ID3D11SamplerState* const*)> SetSamplers;
		};

		static void SetVSShader(ID3D11DeviceContext& direct3DDeviceContext, const ShaderStageData& shaderStageData);
		static void SetPSShader(ID3D11DeviceContext& direct3DDeviceContext, const ShaderStageData& shaderStageData);

	protected:
		static const std::map<ShaderStages, RTTI::IdType> ShaderStageTypeMap;
		static const std::map<RTTI::IdType, ShaderStages> TypeShaderStageMap;
		static const std::map<ShaderStages, ShaderStageCallInfo> ShaderStageCalls;
		static std::vector<ID3D11ShaderResourceView*> EmptySRVs;

		virtual void BeginDraw();
		virtual void EndDraw();

		gsl::not_null<Game*> mGame;
		D3D_PRIMITIVE_TOPOLOGY mTopology;
		winrt::com_ptr<ID3D11InputLayout> mInputLayout;
		std::map<ShaderStages, ShaderStageData> mShaderStageData;
		std::function<void()> mDrawCallback;
		std::function<void()> mUpdateMaterialCallback;
		bool mAutoUnbindShaderResourcesEnabled{ false };
	};

	ConcreteFactory(ActionMaterial, Action);

}

#include "ActionMaterial.inl"