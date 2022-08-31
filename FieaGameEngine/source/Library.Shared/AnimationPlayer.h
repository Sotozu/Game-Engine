#pragma once

#include "GameComponent.h"
#include "MatrixHelper.h"
#include <memory>
#include <DirectXMath.h>

namespace FieaGameEngine
{
	class GameTime;
	class ModelResource;
	class SceneNode;
	class AnimationClip;

    class AnimationPlayer final : GameComponent
    {
		RTTI_DECLARATIONS(AnimationPlayer, GameComponent)

    public:        
		AnimationPlayer(Game& game, std::shared_ptr<ModelResource> model, bool interpolationEnabled = true);
		AnimationPlayer(const AnimationPlayer&) = default;
		AnimationPlayer(AnimationPlayer&&) = default;
		AnimationPlayer& operator=(const AnimationPlayer&) = default;
		AnimationPlayer& operator=(AnimationPlayer&&) = default;
		~AnimationPlayer() = default;

		const std::shared_ptr<ModelResource>& GetModel() const;
		const std::shared_ptr<AnimationClip>& CurrentClip() const;
		float CurrentTime() const;
		std::uint32_t CurrentKeyframe() const;
		const std::vector<DirectX::XMFLOAT4X4>& BoneTransforms() const;
		
		bool InterpolationEnabled() const;
		void SetInterpolationEnabled(bool enabled);

		bool IsPlayingClip() const;
		bool IsClipLooped() const;

		void StartClip(const std::shared_ptr<AnimationClip>& clip);
		void PauseClip();
		void ResumeClip();
		void RestartClip();
		void SetCurrentKeyFrame(std::uint32_t keyframe);
		void GetBindPose();

		virtual void Update(const GameState& gameState) override;

    private:
		void GetBindPose(SceneNode& sceneNode);
		void GetBindPoseBottomUp(SceneNode& sceneNode);
		void GetPose(float time, SceneNode& sceneNode);
		void GetPoseAtKeyframe(std::uint32_t keyframe, SceneNode& sceneNode);
		void GetInterpolatedPose(float time, SceneNode& sceneNode);		

		std::shared_ptr<ModelResource> mModel;
		std::shared_ptr<AnimationClip> mCurrentClip;
		float mCurrentTime{ 0.0f };
		std::uint32_t mCurrentKeyframe{ 0 };
		std::map<SceneNode*, DirectX::XMFLOAT4X4> mToRootTransforms;
		std::vector<DirectX::XMFLOAT4X4> mFinalTransforms;
		DirectX::XMFLOAT4X4 mInverseRootTransform{ MatrixHelper::Identity };
		bool mInterpolationEnabled;
		bool mIsPlayingClip{ false };
		bool mIsClipLooped{ true };
    };
}
