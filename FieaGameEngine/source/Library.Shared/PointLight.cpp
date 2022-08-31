#include "pch.h"
#include "PointLight.h"

using namespace DirectX;

namespace FieaGameEngine
{
	RTTI_DEFINITIONS(PointLight);
#pragma region REQUIRED METHODS TO SUPPORT RTTI AND ATTRIBUTED DESIGN

	gsl::owner<PointLight*> PointLight::Clone() const
	{
		return new PointLight(*this);
	}

	void PointLight::Update(const GameState& gameState)
	{
		if (_game->GetKeyBoard()->IsKeyDown(Keys::NumPad4))
		{
			GetTransform()._position.x += 0.5f;
		}
		if (_game->GetKeyBoard()->IsKeyDown(Keys::NumPad6))
		{
			GetTransform()._position.x -= 0.5f;
		}

		if (_game->GetKeyBoard()->IsKeyDown(Keys::NumPad8))
		{
			GetTransform()._position.y += 0.5f;
		}
		if (_game->GetKeyBoard()->IsKeyDown(Keys::NumPad2))
		{
			GetTransform()._position.y -= 0.5f;
		}

		//
		if (_game->GetKeyBoard()->IsKeyDown(Keys::NumPad7))
		{
			GetTransform()._position.z += 0.5f;
		}
		if (_game->GetKeyBoard()->IsKeyDown(Keys::NumPad9))
		{
			GetTransform()._position.z -= 0.5f;
		}

		//Rotation
		if (_game->GetKeyBoard()->IsKeyDown(Keys::Divide))
		{
			GetTransform()._rotation.x += 0.5f;
		}
		if (_game->GetKeyBoard()->IsKeyDown(Keys::Multiply))
		{
			GetTransform()._rotation.y += 0.5f;
		}
		if (_game->GetKeyBoard()->IsKeyDown(Keys::Subtract))
		{
			GetTransform()._rotation.z += 0.5f;
		}

		Light::Update(gameState);
	}

	void PointLight::Draw(const GameState& gameState)
	{
		UNREFERENCED_PARAMETER(gameState);

	}

	bool PointLight::Equals(const RTTI* rhs) const
	{
		const PointLight* pointLight = rhs->As<PointLight>();
		if (pointLight == nullptr) { return false; }

		return mColor.x == pointLight->mColor.x && mColor.y == pointLight->mColor.y && mColor.z == pointLight->mColor.z && mColor.w == pointLight->mColor.w &&
			mRadius == pointLight->mRadius;

	}

	inline std::string PointLight::ToString() const
	{
		return "PointLight"s;
	}

	const Vector<Signature> PointLight::Signatures()
	{
		return Vector<Signature> {
			{ "mRadius", DatumTypes::Float, 1, offsetof(PointLight, mRadius)}
		};
	}


#pragma endregion

	PointLight::PointLight( float radius) : Light(PointLight::TypeIdClass()),
		mRadius{ radius }
	{

	}


	PointLight::PointLight(std::size_t typeId, float radius) : Light(typeId),
		mRadius{ radius }
	{
	}

	float PointLight::Radius() const
	{
		return mRadius;
	}

	void PointLight::SetRadius(float value)
	{
		mRadius = value;
	}
}