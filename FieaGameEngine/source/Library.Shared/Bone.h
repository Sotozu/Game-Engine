#pragma once

#include "SceneNode.h"
#include "MatrixHelper.h"

namespace FieaGameEngine
{
	class BoneVertexWeights final
	{
	public:
		struct VertexWeight
		{
			float Weight;
			std::uint32_t BoneIndex;

			VertexWeight(float weight, std::uint32_t boneIndex) :
				Weight(weight), BoneIndex(boneIndex) { }
		};
		
		const std::vector<VertexWeight>& Weights() const;
		void AddWeight(float weight, std::uint32_t boneIndex);

		static const std::uint32_t MaxBoneWeightsPerVertex = 4U;

	private:
		std::vector<VertexWeight> mWeights;
	};

    class Bone final : public SceneNode
    {
		RTTI_DECLARATIONS(Bone, SceneNode)

    public:
		explicit Bone(InputStreamHelper& streamHelper);
		Bone(const std::string& name, std::uint32_t index, const DirectX::XMFLOAT4X4& offsetTransform);
		Bone(const Bone&) = default;
		Bone(Bone&&) = default;
		Bone& operator=(const Bone&) = default;
		Bone& operator=(Bone&&) = default;
		~Bone() = default;

		std::uint32_t Index() const;
		void SetIndex(std::uint32_t index);

		const DirectX::XMFLOAT4X4& OffsetTransform() const;
		DirectX::XMMATRIX OffsetTransformMatrix() const;

		virtual void Save(OutputStreamHelper& streamHelper) override;

		bool operator==(const Bone& rhs) const;

		bool operator!=(const Bone& rhs) const;
		
    private:
		virtual void Load(InputStreamHelper& streamHelper) override;

		std::uint32_t mIndex{ 0 };										// Index into the model's bone container
		DirectX::XMFLOAT4X4 mOffsetTransform{ MatrixHelper::Identity };	// Transforms from mesh space to bone space
    };
}
