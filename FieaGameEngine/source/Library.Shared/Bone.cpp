#include "pch.h"
#include "Bone.h"
#include "StreamHelper.h"

using namespace std;
using namespace DirectX;

namespace FieaGameEngine
{
#pragma region BoneVertexWeights

	const std::vector<BoneVertexWeights::VertexWeight>& BoneVertexWeights::Weights() const
	{
		return mWeights;
	}

	void BoneVertexWeights::AddWeight(float weight, uint32_t boneIndex)
	{
		if (mWeights.size() == MaxBoneWeightsPerVertex)
		{
			throw exception("Maximum number of bone weights per vertex exceeded.");
		}

		mWeights.emplace_back(weight, boneIndex);
	}
#pragma endregion

	RTTI_DEFINITIONS(Bone)

	Bone::Bone(InputStreamHelper& streamHelper)
	{
		Load(streamHelper);
	}

	Bone::Bone(const std::string& name, uint32_t index, const XMFLOAT4X4& offsetTransform) :
		SceneNode(name), mIndex(index), mOffsetTransform(offsetTransform)
    {
    }

	uint32_t Bone::Index() const
	{
		return mIndex;
	}

	void Bone::SetIndex(uint32_t index)
	{
		mIndex = index;
	}

	const XMFLOAT4X4& Bone::OffsetTransform() const
	{
		return mOffsetTransform;
	}

	XMMATRIX Bone::OffsetTransformMatrix() const
	{
		return XMLoadFloat4x4(&mOffsetTransform);
	}

	void Bone::Save(OutputStreamHelper& streamHelper)
	{
		SceneNode::Save(streamHelper);

		streamHelper << mIndex << mOffsetTransform;
	}

	void Bone::Load(InputStreamHelper& streamHelper)
	{
		SceneNode::Load(streamHelper);

		streamHelper >> mIndex >> mOffsetTransform;
	}

	//ADDED 7/23/2022
	bool Bone::operator==(const Bone& rhs) const
	{

		return mIndex == rhs.mIndex &&
			mOffsetTransform.m[0][0] == rhs.mOffsetTransform.m[0][0] && mOffsetTransform.m[0][1] == rhs.mOffsetTransform.m[0][1] && mOffsetTransform.m[0][2] == rhs.mOffsetTransform.m[0][2] && mOffsetTransform.m[0][3] == rhs.mOffsetTransform.m[0][3] &&
			mOffsetTransform.m[1][0] == rhs.mOffsetTransform.m[1][0] && mOffsetTransform.m[1][1] == rhs.mOffsetTransform.m[1][1] && mOffsetTransform.m[1][2] == rhs.mOffsetTransform.m[1][2] && mOffsetTransform.m[1][3] == rhs.mOffsetTransform.m[1][3] &&
			mOffsetTransform.m[2][0] == rhs.mOffsetTransform.m[2][0] && mOffsetTransform.m[2][1] == rhs.mOffsetTransform.m[2][1] && mOffsetTransform.m[2][2] == rhs.mOffsetTransform.m[2][2] && mOffsetTransform.m[2][3] == rhs.mOffsetTransform.m[2][3] &&
			mOffsetTransform.m[3][0] == rhs.mOffsetTransform.m[3][0] && mOffsetTransform.m[3][1] == rhs.mOffsetTransform.m[3][1] && mOffsetTransform.m[3][2] == rhs.mOffsetTransform.m[3][2] && mOffsetTransform.m[3][3] == rhs.mOffsetTransform.m[3][3];
	}


	bool Bone::operator!=(const Bone& rhs) const
	{
		return !(operator==(rhs));
	}

}
