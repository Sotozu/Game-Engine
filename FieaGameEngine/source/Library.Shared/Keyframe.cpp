#include "pch.h"
#include "Keyframe.h"
#include "VectorHelper.h"
#include "StreamHelper.h"

using namespace DirectX;

namespace FieaGameEngine
{
	Keyframe::Keyframe(InputStreamHelper& streamHelper)
	{
		Load(streamHelper);
	}

	Keyframe::Keyframe(float time, const XMFLOAT3& translation, const XMFLOAT4& rotationQuaternion, const XMFLOAT3& scale) :
		mTime(time), mTranslation(translation), mRotationQuaternion(rotationQuaternion), mScale(scale)
    {
    }

	float Keyframe::Time() const
	{
		return mTime;
	}

	const XMFLOAT3& Keyframe::Translation() const
	{	
		return mTranslation;
	}

	const XMFLOAT4& Keyframe::RotationQuaternion() const
	{
		return mRotationQuaternion;
	}

	const XMFLOAT3& Keyframe::Scale() const
	{
		return mScale;
	}

	XMVECTOR Keyframe::TranslationVector() const
	{
		return XMLoadFloat3(&mTranslation);
	}

	XMVECTOR Keyframe::RotationQuaternionVector() const
	{
		return XMLoadFloat4(&mRotationQuaternion);
	}

	XMVECTOR Keyframe::ScaleVector() const
	{
		return XMLoadFloat3(&mScale);
	}

	XMMATRIX Keyframe::Transform() const
	{
		static XMVECTOR rotationOrigin = XMLoadFloat4(&Vector4Helper::Zero);

		return XMMatrixAffineTransformation(ScaleVector(), rotationOrigin, RotationQuaternionVector(), TranslationVector());
	}

	void Keyframe::Save(OutputStreamHelper& streamHelper)
	{
		streamHelper << mTime;
		streamHelper << mTranslation.x << mTranslation.y << mTranslation.z;
		streamHelper << mRotationQuaternion.x << mRotationQuaternion.y << mRotationQuaternion.z << mRotationQuaternion.w;
		streamHelper << mScale.x << mScale.y << mScale.z;
	}

	void Keyframe::Load(InputStreamHelper& streamHelper)
	{
		streamHelper >> mTime;
		streamHelper >> mTranslation.x >> mTranslation.y >> mTranslation.z;
		streamHelper >> mRotationQuaternion.x >> mRotationQuaternion.y >> mRotationQuaternion.z >> mRotationQuaternion.w;
		streamHelper >> mScale.x >> mScale.y >> mScale.z;
	}

	//Added 7/23/2022

	bool Keyframe::operator==(const Keyframe& rhs) const
	{
		return mTime == rhs.mTime &&
			mTranslation.x == rhs.mTranslation.x && mTranslation.y == rhs.mTranslation.y && mTranslation.z == rhs.mTranslation.z &&
			mRotationQuaternion.x == rhs.mRotationQuaternion.x && mRotationQuaternion.y == rhs.mRotationQuaternion.y && mRotationQuaternion.z == rhs.mRotationQuaternion.z && mRotationQuaternion.w == rhs.mRotationQuaternion.w &&
			mScale.x == rhs.mScale.x && mScale.y == rhs.mScale.y && mScale.z == rhs.mScale.z;
	}

	bool Keyframe::operator!=(const Keyframe& rhs) const
	{
		return !(operator==(rhs));
	}


}
