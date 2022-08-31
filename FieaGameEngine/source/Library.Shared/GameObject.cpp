#include "pch.h"
#include "GameObject.h"
//#include "Game.h"
#include "ActionMeshRenderer.h"
namespace FieaGameEngine
{

#pragma region GameObject
	RTTI_DEFINITIONS(GameObject);

	/// <summary>
	/// Default Constructor
	/// </summary>
	GameObject::GameObject() : Attributed(GameObject::TypeIdClass()),
		_name{ "" },
		_game{nullptr}
	{
		
	}

	GameObject::GameObject(Game& game) : Attributed(GameObject::TypeIdClass()),
		_name{""},
		_game{&game}
	{}


	/// <summary>
	/// Private constructor used in the CTOR initialization line for inheriting classes.
	/// </summary>
	GameObject::GameObject(std::size_t typeId) : Attributed(typeId),
		_name{ "" },
		_game{ nullptr }

	{}

	//ADDED 6/21/2022
	GameObject::GameObject(Game& game, std::size_t typeId) : Attributed(typeId),
		_name{ "" },
		_game{ &game }

	{}




	GameObject::GameObject(const std::string& name, const Transform& transform) : Attributed(GameObject::TypeIdClass()),
		_name(name),
		_transform(transform)
	{}

	GameObject::GameObject(const std::string& name, const glm::vec4& position, const glm::vec4& rotation, const glm::vec4& scale) : Attributed(GameObject::TypeIdClass()),
		_name(name),
		_transform(position, rotation, scale)
	{}

	void GameObject::Update(const GameState& gameState)
	{
		/*if (_parent->_transformDataDirty == true)
		{
			_transformDataDirty = true;
		}*/

		if (_transformDataDirty)
		{
			//auto parentTransform = _parent->GetTransform();

			//+= parentTransform._position
			for (auto& callback : mTransformUpdateCallback)
			{
				callback();
			}

		}

		Datum* _myActions = Scope::Find("_actions");
		if (_myActions != nullptr) //There are actions
		{
			_myActions->Size(); //this tells me the number of children I have.
			for (std::size_t index = 0; index < _myActions->Size(); ++index)
			{
				Action* _action = _myActions->Get<Scope>(index).As<Action>(); //Downcast so that we get a game object.
				assert(_action != nullptr);
				_action->Update(gameState);
			}

		}

		Datum* _myChildrenScope = Scope::Find("_children"); //the children tag is a reserved tag that will be used to mark children in the attributed mirroring structure.
		if (_myChildrenScope != nullptr) //There are children
		{
			_myChildrenScope->Size(); //this tells me the number of children I have.
			for (std::size_t index = 0; index < _myChildrenScope->Size(); ++index)
			{
				GameObject* myGameObject = _myChildrenScope->Get<Scope>(index).As<GameObject>(); //Downcast so that we get a game object.
				assert(myGameObject != nullptr);
				myGameObject->Update(gameState);
			}

		}
		_transformDataDirty = false;
	}

	void GameObject::Draw(const GameState& gameState)
	{
		Datum* _myActions = Scope::Find("_actions");
		if (_myActions != nullptr) //There are actions
		{
			_myActions->Size(); //this tells me the number of children I have.
			for (std::size_t index = 0; index < _myActions->Size(); ++index)
			{
				//TEMPORARY JUST FOR TESTING
				ActionMeshRenderer* _action = _myActions->Get<Scope>(index).As<ActionMeshRenderer>(); //Downcast so that we get a game object.
				assert(_action != nullptr);
				_action->Draw(gameState);
			}

		}

		Datum* _myChildrenScope = Scope::Find("_children"); //the children tag is a reserved tag that will be used to mark children in the attributed mirroring structure.
		if (_myChildrenScope != nullptr) //There are children
		{
			_myChildrenScope->Size(); //this tells me the number of children I have.
			for (std::size_t index = 0; index < _myChildrenScope->Size(); ++index)
			{
				GameObject* myGameObject = _myChildrenScope->Get<Scope>(index).As<GameObject>(); //Downcast so that we get a game object.
				assert(myGameObject != nullptr);
				myGameObject->Draw(gameState);
			}

		}

	}


	//TO DO: Implement
	bool GameObject::Equals(const RTTI* rhs) const
	{
		const GameObject* myGameObject = rhs->As<GameObject>();
		if (myGameObject == nullptr) { return false; }

		return _name == myGameObject->_name &&
			_transform._position == myGameObject->_transform._position &&
			_transform._rotation == myGameObject->_transform._rotation &&
			_transform._scale == myGameObject->_transform._scale &&
			(*this).At("_children"s) == (*myGameObject).At("_children"s);
	}

	const Vector<Signature> GameObject::Signatures()
	{
		return Vector<Signature> {
			{"_name", DatumTypes::String, 1, offsetof(GameObject, _name)},
			{ "_position", DatumTypes::Vector4, 1, offsetof(GameObject, _transform._position) },
			{ "_rotation", DatumTypes::Vector4, 1, offsetof(GameObject, _transform._rotation) },
			{ "_scale", DatumTypes::Vector4, 1, offsetof(GameObject, _transform._scale) },
			{ "_children", DatumTypes::Table, 0,0 },
			{ "_actions", DatumTypes::Table, 0,0 },
		};
	}

	gsl::owner<GameObject*> GameObject::Clone() const
	{
		return new GameObject(*this);
	}

	inline std::string GameObject::ToString() const
	{
		return "GameObject"s;
	}

	Datum* GameObject::GetActions()
	{
		return Scope::Find("_actions");
	}

	void GameObject::Init(const std::string& name, const glm::vec4& position, const glm::vec4& rotation, const glm::vec4& scale)
	{
		_name = name;
		_transform._position = position;
		_transform._rotation = rotation;
		_transform._scale = scale;
	}
	void GameObject::Init(const std::string& name, const Transform& transform)
	{
		_name = name;
		_transform = transform;
	}

	Action& GameObject::CreateAction(const std::string& actionClassName, const std::string& name) //TO DO Assignment 14.
	{
		Scope* myActionScope = Factory<Action>::Create(actionClassName);

		assert(myActionScope != nullptr); //will be nullptr if create method was not successful. This could be that the class name passed in is not of type Action.

		Action* myAction = myActionScope->As<Action>();
		myAction->SetName(name);
		Adopt("_actions", *myActionScope);
		return *myAction;
	}

	GameObject* GameObject::GetParentGameObject()
	{
		Scope* scopeParent = GetParent();
		GameObject* gParent{ nullptr };

		while (scopeParent != nullptr)
		{
			gParent = scopeParent->As<GameObject>();
			if (gParent != nullptr)
			{
				break;
			}
			scopeParent = scopeParent->GetParent();
		}

		return gParent;
	}

	void GameObject::SetGame(Game& game)
	{
		_game = &game;
	}

	const Game* const GameObject::GetGame() const { return _game; }

	Game* GameObject::GetGame() { return _game; }

	//ADDED 6/21/2022
	//Actions must be associated with a GameObject.
	//Find game it's parent game object and use it's _game member variable to populate this objects _game member variable.
	void GameObject::Initialize()
	{
		_game = &Game::GetInstance();
		_parent = Scope::GetParent()->As<GameObject>();
		auto updateTransformFunc = [this]() { _transformDataDirty = true; };

		if (_parent != nullptr) //This is not ideal. The only one that this is checking against is the root game object.
		{
			_parent->AddTransformUpdateCallback(updateTransformFunc);

			_transform._position += _parent->_transform._position;
			_transform._rotation += _parent->_transform._rotation;
			_transform._scale += _parent->_transform._scale;
		}
	}

	void GameObject::UpdateTransform()
	{

	}

	void GameObject::Translate(glm::vec4& position)
	{
		_transform._position = position;

		_transformDataDirty = true;
	}
	void GameObject::Rotate(glm::vec4& rotation)
	{
		_transform._rotation = rotation;

		_transformDataDirty = true;
	}
	void GameObject::Scale(glm::vec4& scale)
	{
		_transform._scale = scale;

		_transformDataDirty = true;
	}


	void GameObject::Translate(float x, float y, float z)
	{
		_transform._position.x = x;
		_transform._position.y = y;
		_transform._position.z = z;

		_transformDataDirty = true;
	}
	void GameObject::Rotate(float x, float y, float z)
	{
		_transform._rotation.x = x;
		_transform._rotation.y = y;
		_transform._rotation.z = z;

		_transformDataDirty = true;

	}
	void GameObject::Scale(float x, float y, float z)
	{
		_transform._scale.x = x;
		_transform._scale.y = y;
		_transform._scale.z = z;

		_transformDataDirty = true;

	}

	void GameObject::TranslateX(const float x)
	{
		_transform._position.x = x;
		_transformDataDirty = true;

	}
	void GameObject::TranslateY(const float y)
	{
		_transform._position.x = y;
		_transformDataDirty = true;

	}
	void GameObject::TranslateZ(const float z)
	{
		_transform._position.x = z;
		_transformDataDirty = true;

	}

	void GameObject::RotateX(const float x)
	{
		_transform._rotation.x = x;
		_transformDataDirty = true;

	}
	void GameObject::RotateY(const float y)
	{
		_transform._rotation.y = y;
		_transformDataDirty = true;


	}
	void GameObject::RotateZ(const float z)
	{
		_transform._rotation.z = z;
		_transformDataDirty = true;

	}

	void GameObject::ScaleX(const float x)
	{
		_transform._scale.x = x;
		_transformDataDirty = true;

	}
	void GameObject::ScaleY(const float y)
	{
		_transform._scale.y = y;
		_transformDataDirty = true;

	}
	void GameObject::ScaleZ(const float z)
	{
		_transform._scale.z = z;
		_transformDataDirty = true;

	}


	const Transform& GameObject::GetTransform() const
	{
		return _transform;
	}

	Transform& GameObject::GetTransform()
	{
		_transformDataDirty = true;
		return _transform;
	}

	bool GameObject::IsTransformDataDirty() const
	{
		return _transformDataDirty;
	}

#pragma endregion

#pragma region Transform
	//RTTI_DEFINITIONS(Transform);

	Transform::Transform() : //Attributed(Transform::TypeIdClass())
		_position{ 0 },
		_rotation{ 0 },
		_scale{ 0 }
	{}

	Transform::Transform(const glm::vec4& position, const glm::vec4& rotation, const glm::vec4& scale) : //Attributed(Transform::TypeIdClass()),
		_position{ position },
		_rotation{ rotation },
		_scale{ scale }
	{}


	Transform::Transform(const Transform& rhs) :
		_position{ rhs._position },
		_rotation{ rhs._rotation },
		_scale{ rhs._scale }
	{}


	Transform& Transform::operator=(const Transform& rhs)
	{
		_position = rhs._position;
		_rotation = rhs._rotation;
		_scale = rhs._scale;
		return *this;
	}

	Transform& Transform::operator+=(const Transform& rhs)
	{
		_position += rhs._position;
		_rotation += rhs._rotation;
		_scale += rhs._scale;

		return *this;
	}

#pragma endregion
}
