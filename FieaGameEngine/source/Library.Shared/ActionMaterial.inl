#include "Game.h"
namespace FieaGameEngine
{

	//ADDED 7/20/2022

	inline std::string ActionMaterial::ToString() const { return "Material"s; }

	inline gsl::owner<ActionMaterial*> ActionMaterial::Clone() const { return new ActionMaterial(*this); }


	inline D3D_PRIMITIVE_TOPOLOGY ActionMaterial::Topology() const
	{
		return mTopology;
	}

	inline void ActionMaterial::SetTopology(D3D_PRIMITIVE_TOPOLOGY topology)
	{
		mTopology = topology;
	}

	inline winrt::com_ptr<ID3D11InputLayout> ActionMaterial::InputLayout() const
	{
		return mInputLayout;
	}

	inline void ActionMaterial::SetInputLayout(winrt::com_ptr<ID3D11InputLayout> inputLayout)
	{
		assert(inputLayout != nullptr);
		mInputLayout = inputLayout;
	}

	template <typename T>
	std::shared_ptr<T> ActionMaterial::GetShader()
	{
		ShaderStages shaderStage = TypeShaderStageMap.at(T::TypeIdClass());
		std::shared_ptr<Shader> shader = GetShader(shaderStage);
		return std::static_pointer_cast<T>(shader);
	}

	template <typename T>
	void ActionMaterial::SetShader(std::shared_ptr<T> shader)
	{
		ShaderStages shaderStage = TypeShaderStageMap.at(T::TypeIdClass());
		SetShader(shaderStage, shader);
	}

	inline void ActionMaterial::SetShader(ShaderStages shaderStage, std::shared_ptr<Shader> shader)
	{
		assert(ShaderStageIsProgrammable(shaderStage));
		mShaderStageData[shaderStage].Shader = shader;
	}

	inline void ActionMaterial::SetShaderClassInstance(ShaderStages shaderStage, ID3D11ClassInstance* classInstance)
	{
		assert(ShaderStageIsProgrammable(shaderStage));
		mShaderStageData[shaderStage].ShaderClassInstance = classInstance;
	}

	inline void ActionMaterial::SetConstantBuffer(ShaderStages shaderStage, ID3D11Buffer* constantBuffer)
	{
		ClearConstantBuffers(shaderStage);
		AddConstantBuffer(shaderStage, constantBuffer);
	}

	inline void ActionMaterial::AddConstantBuffer(ShaderStages shaderStage, ID3D11Buffer* constantBuffer)
	{
		mShaderStageData[shaderStage].ConstantBuffers.push_back(constantBuffer);
	}

	inline void ActionMaterial::AddConstantBuffers(ShaderStages shaderStage, const gsl::span<ID3D11Buffer*>& constantBuffers)
	{
		auto& registeredConstantBuffers = mShaderStageData[shaderStage].ConstantBuffers;
		registeredConstantBuffers.insert(registeredConstantBuffers.end(), constantBuffers.begin(), constantBuffers.end());
	}

	inline void ActionMaterial::SetShaderResource(ShaderStages shaderStage, ID3D11ShaderResourceView* shaderResource)
	{
		ClearShaderResources(shaderStage);
		AddShaderResource(shaderStage, shaderResource);
	}

	inline void ActionMaterial::AddShaderResource(ShaderStages shaderStage, ID3D11ShaderResourceView* shaderResource)
	{
		auto& shaderResources = mShaderStageData[shaderStage].ShaderResources;
		shaderResources.push_back(shaderResource);

		if (EmptySRVs.size() < shaderResources.size())
		{
			EmptySRVs.push_back(nullptr);
		}
	}

	inline void ActionMaterial::AddShaderResources(ShaderStages shaderStage, const gsl::span<ID3D11ShaderResourceView*>& shaderResources)
	{
		auto& registeredShaderResources = mShaderStageData[shaderStage].ShaderResources;
		registeredShaderResources.insert(registeredShaderResources.end(), shaderResources.begin(), shaderResources.end());

		if (EmptySRVs.size() < registeredShaderResources.size())
		{
			EmptySRVs.resize(registeredShaderResources.size(), nullptr);
		}
	}

	inline void ActionMaterial::SetSamplerState(ShaderStages shaderStage, ID3D11SamplerState* samplerState)
	{
		ClearSamplerStates(shaderStage);
		AddSamplerState(shaderStage, samplerState);
	}

	inline void ActionMaterial::AddSamplerState(ShaderStages shaderStage, ID3D11SamplerState* samplerState)
	{
		mShaderStageData[shaderStage].SamplerStates.push_back(samplerState);
	}

	inline void ActionMaterial::AddSamplerStates(ShaderStages shaderStage, const gsl::span<ID3D11SamplerState*>& samplerStates)
	{
		auto& registeredSamplerStates = mShaderStageData[shaderStage].SamplerStates;
		registeredSamplerStates.insert(registeredSamplerStates.end(), samplerStates.begin(), samplerStates.end());
	}

	inline std::function<void()> ActionMaterial::DrawCallback() const
	{
		return mDrawCallback;
	}

	inline void ActionMaterial::SetDrawCallback(std::function<void()> callback)
	{
		mDrawCallback = callback;
	}

	inline std::function<void()> ActionMaterial::UpdateMaterialCallback() const
	{
		return mUpdateMaterialCallback;
	}

	inline void ActionMaterial::SetUpdateMaterialCallback(std::function<void()> callback)
	{
		mUpdateMaterialCallback = callback;
	}

	template <size_t _Count>
	void ActionMaterial::UnbindShaderResources(gsl::not_null<ID3D11DeviceContext*> direct3DDeviceContext, ShaderStages shaderStage, std::uint32_t startSlot)
	{
		static std::array<ID3D11ShaderResourceView*, _Count> emptySRVs{ nullptr };

		auto& shaderStageCallInfo = ShaderStageCalls.at(shaderStage);
		shaderStageCallInfo.SetShaderResources(direct3DDeviceContext.get(), startSlot, _Count, emptySRVs.data());
	}

	template <size_t _Count>
	void ActionMaterial::UnbindShaderResources(ShaderStages shaderStage, std::uint32_t startSlot)
	{
		ActionMaterial::UnbindShaderResources<_Count>(mGame->Direct3DDeviceContext(), shaderStage, startSlot);
	}

	inline bool ActionMaterial::AutoUnbindShaderResourcesEnabled() const
	{
		return mAutoUnbindShaderResourcesEnabled;
	}

	inline void ActionMaterial::SetAutoUnbindShaderResourcesEnabled(bool enabled)
	{
		mAutoUnbindShaderResourcesEnabled = enabled;
	}
}