#include "pch.h"
#include "Light.h"
#include "LightRegistry.h"

using namespace DirectX;
using namespace DirectX::PackedVector;

namespace FieaGameEngine
{
	RTTI_DEFINITIONS(Light);


	Light::Light() : GameObject(Light::TypeIdClass())
	{
		LightRegistry::AddLight(*this);
	}

	Light::Light(std::size_t typeId) : GameObject(typeId)	
	{
		LightRegistry::AddLight(*this);
	}

#pragma region REQUIRED METHODS TO SUPPORT RTTI AND ATTRIBUTED DESIGN

	gsl::owner<Light*> Light::Clone() const
	{
		return new Light(*this);
	}

	void Light::Update(const GameState& gameState)
	{
		UNREFERENCED_PARAMETER(gameState);
		GameObject::Update(gameState);
	}

	
	void Light::Draw(const GameState& gameState)
	{
		UNREFERENCED_PARAMETER(gameState);
	}

	
	bool Light::Equals(const RTTI* rhs) const
	{
		const Light* light = rhs->As<Light>();
		if (light == nullptr) { return false; }

		return mColor.x == light->mColor.x && mColor.y == light->mColor.y && mColor.z == light->mColor.z && mColor.w == light->mColor.w;
	}

	
	inline std::string Light::ToString() const
	{
		return "Light"s;
	}

	const Vector<Signature> Light::Signatures()
	{
		return Vector<Signature> {
			{ "mColor", DatumTypes::Vector4, 1, offsetof(Light, mColor)}
		};
	}

#pragma endregion
}