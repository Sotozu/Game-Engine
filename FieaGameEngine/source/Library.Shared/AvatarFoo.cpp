#include "pch.h"
#include "AvatarFoo.h"
#include "ActionMeshRenderer.h"
#include "ContentManager.h"
#include "Model.h"

#include <utility>

using namespace FieaGameEngine;

namespace FieaGameEngine
{
	RTTI_DEFINITIONS(AvatarFoo);

	AvatarFoo::AvatarFoo() : GameObject(AvatarFoo::TypeIdClass()),
		_hitPoints{ 100 },
		_isTakingDamage{ false }

	{}

	//Protected version for inheriting classes to use
	AvatarFoo::AvatarFoo(std::size_t typeId) : GameObject(typeId),
		_hitPoints{ 100 },
		_isTakingDamage{ false }
	{}


	gsl::owner<AvatarFoo*> AvatarFoo::Clone() const
	{
		return new AvatarFoo(*this);
	}

	void AvatarFoo::Update(const GameState& gameState)
	{
		_hitPoints++;

		//X
		if(_game->GetKeyBoard()->IsKeyDown(Keys::NumPad4))
		{
			GetTransform()._position.x -= 0.05f;
		}
		if (_game->GetKeyBoard()->IsKeyDown(Keys::NumPad6))
		{
			GetTransform()._position.x += 0.05f;
		}

		//Y
		if (_game->GetKeyBoard()->IsKeyDown(Keys::NumPad8))
		{
			GetTransform()._position.y += 0.05f;
		}
		if (_game->GetKeyBoard()->IsKeyDown(Keys::NumPad2))
		{
			GetTransform()._position.y -= 0.05f;
		}

		//Z
		if (_game->GetKeyBoard()->IsKeyDown(Keys::NumPad7))
		{
			GetTransform()._position.z += 0.05f;
		}
		if (_game->GetKeyBoard()->IsKeyDown(Keys::NumPad9))
		{
			GetTransform()._position.z -= 0.05f;
		}

		//Rotation
		if (_game->GetKeyBoard()->IsKeyDown(Keys::Divide))
		{
			GetTransform()._rotation.x += 0.05f;
		}
		if (_game->GetKeyBoard()->IsKeyDown(Keys::Multiply))
		{
			GetTransform()._rotation.y += 0.05f;
		}
		if (_game->GetKeyBoard()->IsKeyDown(Keys::Subtract))
		{
			GetTransform()._rotation.z += 0.05f;
		}

		//Scale
		if (_game->GetKeyBoard()->IsKeyDown(Keys::O))
		{
			GetTransform()._rotation.x += 0.05f;
		}
		if (_game->GetKeyBoard()->IsKeyDown(Keys::Multiply))
		{
			GetTransform()._rotation.y += 0.05f;
		}
		if (_game->GetKeyBoard()->IsKeyDown(Keys::Subtract))
		{
			GetTransform()._rotation.z += 0.05f;
		}


		//Scale
		if (_game->GetKeyBoard()->IsKeyDown(Keys::O))
		{
			GetTransform()._scale.x += 0.05f;
			GetTransform()._scale.y += 0.05f;
			GetTransform()._scale.z += 0.05f;
		}
		if (_game->GetKeyBoard()->IsKeyDown(Keys::P))
		{
			GetTransform()._scale.x -= 0.05f;
			GetTransform()._scale.y -= 0.05f;
			GetTransform()._scale.z -= 0.05f;
		}

		//ChangeMesh
		if (_game->GetKeyBoard()->WasKeyReleasedThisFrame(Keys::Space))
		{
			auto actionMR = Find("_actions")->Get<Scope>().As<ActionMeshRenderer>();
			if (actionMR != nullptr)
			{
				if (_modelSwitch == true)
				{
					auto mr = _game->GetContentManager().Load<ModelResource>(Utility::ToWideString("Models\\Rocks\\Rock1D.fbx.bin"));
					actionMR->ChangeModel(mr);
				}
				else
				{
					auto mr = _game->GetContentManager().Load<ModelResource>(Utility::ToWideString("Models\\Swords\\broadSword.fbx.bin"));
					actionMR->ChangeModel(mr);

				}
				_modelSwitch = !_modelSwitch;
				//actionMR->ChangeModel("Models\\Rocks\\Rock2.fbx.bin");
			}
		}


		GameObject::Update(gameState);
	}


	FieaGameEngine::Vector<FieaGameEngine::Signature> AvatarFoo::Signatures()
	{
		return Vector<Signature>
		{
			{"_hitPoints"s, DatumTypes::Integer, 1, offsetof(AvatarFoo, _hitPoints)},
			{ "_isTakingDamage"s, DatumTypes::Boolean, 1, offsetof(AvatarFoo, _isTakingDamage) }

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