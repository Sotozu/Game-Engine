#include "pch.h"
#include "AvatarFoo.h"


using namespace FieaGameEngine;

namespace UnitTestLibraryDesktop
{
	RTTI_DEFINITIONS(AvatarFoo);

	AvatarFoo::AvatarFoo() : GameObject(AvatarFoo::TypeIdClass()),
		_hitPoints{ 100 },
		_isTakingDamage{ false }

	{}

	//Protected version for inheriting classes to use
	AvatarFoo::AvatarFoo(std::size_t typeId) : GameObject(typeId),
		_hitPoints{ 100 },
		_isTakingDamage{false}
	{}


	gsl::owner<AvatarFoo*> AvatarFoo::Clone() const
	{
		return new AvatarFoo(*this);
	}

	void AvatarFoo::Update(const GameState& gameState)
	{
		_hitPoints++;
		GameObject::Update(gameState);
	}


	FieaGameEngine::Vector<FieaGameEngine::Signature> AvatarFoo::Signatures()
	{
		return Vector<Signature>
		{
			{"_hitPoints"s, DatumTypes::Integer, 1, offsetof(AvatarFoo, _hitPoints)},
			{"_isTakingDamage"s, DatumTypes::Boolean, 1, offsetof(AvatarFoo, _isTakingDamage)}

		};
	}


	bool AvatarFoo::Equals(const RTTI* rhs) const
	{
		const AvatarFoo* myAvatarFoo = rhs->As<AvatarFoo>();
		if (myAvatarFoo == nullptr) { return false; }

		return _name == myAvatarFoo->_name &&
			GetTransform()._position == myAvatarFoo->GetTransform()._position &&
			GetTransform()._rotation == myAvatarFoo->GetTransform()._rotation &&
			GetTransform()._scale == myAvatarFoo->GetTransform()._scale &&
			_hitPoints == myAvatarFoo->_hitPoints &&
			(*this).At("_children"s) == (*myAvatarFoo).At("_children"s);

	}


	std::string AvatarFoo::ToString() const
	{
		return "AvatarFoo"s;
	}


}