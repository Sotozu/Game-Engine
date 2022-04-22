#include "pch.h"
#include "GameObject.h"

namespace FieaGameEngine
{

#pragma region GameObject
	RTTI_DEFINITIONS(GameObject);

	/// <summary>
	/// Default Constructor
	/// </summary>
	GameObject::GameObject() : Attributed(GameObject::TypeIdClass()),
		_name{ "" }
	{}

	/// <summary>
	/// Private constructor used in the CTOR initialization line for inheriting classes.
	/// </summary>
	GameObject::GameObject(std::size_t typeId) : Attributed(typeId),
		_name{ "" }
	{}


	GameObject::GameObject(const std::string& name, const Transform& transform) : Attributed(GameObject::TypeIdClass()),
		_name(name),
		_transform(transform)
	{}

	GameObject::GameObject(const std::string& name, const glm::vec4& position, const glm::vec4& rotation, const glm::vec4& scale) : Attributed(GameObject::TypeIdClass()),
		_name(name),
		_transform(position, rotation, scale)
	{}

	void GameObject::Update(const GameTime& gameTime)
	{
		Datum* _myActions = Scope::Find("_actions");
		if (_myActions != nullptr) //There are actions
		{
			_myActions->Size(); //this tells me the number of children I have.
			for (std::size_t index = 0; index < _myActions->Size(); ++index)
			{
				Action* _action = _myActions->Get<Scope>(index).As<Action>(); //Downcast so that we get a game object.
				assert(_action != nullptr);
				_action->Update(gameTime);
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
				myGameObject->Update(gameTime);
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
			{ "_position", DatumTypes::Vector, 1, offsetof(GameObject, _transform._position) },
			{ "_rotation", DatumTypes::Vector, 1, offsetof(GameObject, _transform._rotation) },
			{ "_scale", DatumTypes::Vector, 1, offsetof(GameObject, _transform._scale) },
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

#pragma endregion
}
