#pragma once

#include <gsl/gsl>
#include <DirectXMath.h>
#include <vector>
#include <memory>
#include <string>
#include <cstdint>

namespace FieaGameEngine
{
	class ModelResource;
	class Bone;
	class Keyframe;
	class OutputStreamHelper;
	class InputStreamHelper;

	struct BoneAnimationData final
	{
		std::uint32_t BoneIndex{ 0 };
		std::vector<std::shared_ptr<Keyframe>> Keyframes;

		bool operator==(const BoneAnimationData& rhs) const
		{
			return BoneIndex == rhs.BoneIndex && Keyframes == rhs.Keyframes;
		}
	};

    class BoneAnimation final
    {
    public:
		BoneAnimation(ModelResource& model, InputStreamHelper& streamHelper);
		BoneAnimation(ModelResource& model, const BoneAnimationData& boneAnimationData);
		BoneAnimation(ModelResource& model, BoneAnimationData&& boneAnimationData);
		BoneAnimation(const BoneAnimation&) = default;
		BoneAnimation(BoneAnimation&& rhs) = default;
		BoneAnimation& operator=(const BoneAnimation& rhs) = default;
		BoneAnimation& operator=(BoneAnimation&& rhs) = default;
		~BoneAnimation() = default;

		Bone& GetBone();
		std::vector<std::shared_ptr<Keyframe>>& Keyframes();

		std::uint32_t GetTransform(float time, DirectX::XMFLOAT4X4& transform) const;
		void GetTransformAtKeyframe(std::uint32_t keyframeIndex, DirectX::XMFLOAT4X4& transform) const;
		void GetInteropolatedTransform(float time, DirectX::XMFLOAT4X4& transform) const;

		void Save(OutputStreamHelper& streamHelper);

		bool operator==(const BoneAnimation& rhs) const;

		bool operator!=(const BoneAnimation& rhs) const;

    private:
		void Load(InputStreamHelper& streamHelper);
		std::uint32_t FindKeyframeIndex(float time) const;

		ModelResource* mModel;
		std::weak_ptr<Bone> mBone;
		std::vector<std::shared_ptr<Keyframe>> mKeyframes;
    };
}
