#pragma once
#include <glm/glm.hpp>
#include "Attributed.h"
#include "TypeRegistry.h"
#include "Factory.h"
#include "VertexDeclarations.h"
#include "Vector.h"
#include "GameTime.h"
#include "Game.h"
#include "Action.h"

namespace FieaGameEngine
{
	class Game;
	//class Action;
	class Mesh;
	/// <summary>
	/// Used to store the the position, rotation and scale of a GameObject
	/// </summary>
	class Transform final
	{
	public:
		glm::vec4 _position;
		glm::vec4 _rotation;
		glm::vec4 _scale;

		/// <summary>
		/// Default constructor.
		/// </summary>
		Transform();

		/// <summary>
		/// Takes the following information for a GameObject: Position, Rotation and Scale.
		/// </summary>
		/// <param name="position">Position of the GameObject</param>
		/// <param name="rotation">Rotation of the GameObject</param>
		/// <param name="scale">Scale of the GameObject</param>
		Transform(const glm::vec4& position, const glm::vec4& rotation, const glm::vec4& scale);

		/// <summary>
		/// Copies transform values.
		/// </summary>
		/// <param name="">A Transform to copy from</param>
		Transform(const Transform&);

		/// <summary>
		/// Copies transform values.
		/// </summary>
		/// <param name="">A Transform to copy from</param>
		/// <returns>A mutable reference to this Transform object</returns>
		Transform& operator=(const Transform&);

		/// <summary>
		/// Author declared and compiler defined move constructor.
		/// </summary>
		/// <param name="">A Transform to move from</param>
		Transform(Transform&&) noexcept = default;


		/// <summary>
		/// Author declared and compiler defined move assignment
		/// </summary>
		/// <param name="">A Transform to move from</param>
		/// <returns>A mutable reference to this Transform object</returns>
		Transform& operator=(Transform&&) noexcept = default;

		~Transform() = default;

		Transform& operator+=(const Transform&);
	};

	class GameObject : public Attributed
	{
		RTTI_DECLARATIONS(GameObject, Attributed);
	public:

		/// <summary>
		/// Clones the calling GameObject. Heap constructs this object and it's on the user to manage its memory.
		/// Overridden function from Scope.
		/// </summary>
		/// <returns>
		/// A pointer to heap allocated clone of the calling GameObject.
		/// </returns>
		gsl::owner<GameObject*> Clone() const override;

		/// <summary>
		/// Update method will update the GameObject and then recursively update all it's children GameObjects.
		/// Made virtual so any derived classes from GameObject can have their own update method and be called polymorphically.
		/// </summary>
		/// <param name="gameTime">A reference to the game time</param>
		virtual void Update(const GameState& gameState);
		
		//ADDED 6/22/2022
		virtual void Draw(const GameState& gameState);

		/// <summary>
		/// Compares one GameObject with another.
		/// Overridden function from RTTI.
		/// </summary>
		/// <param name="rhs">The GameObject to compare against</param>
		/// <returns>
		/// TRUE if the GameObject is equivalent.
		/// FALSE if the GameObject is not equivalent
		/// </returns>
		bool Equals(const RTTI* rhs) const override;

		/// <summary>
		/// Returns a string that identifies this class.
		/// Overridden function from RTTI.
		/// </summary>
		/// <returns>A string that identifies this class</returns>
		inline std::string ToString() const override;

		/// <summary>
		/// Gets the actions datum which contains the actions for this GameObject.
		/// </summary>
		/// <returns></returns>
		Datum* GetActions();

		/// <summary>
		/// Creates and action and appends it to the GameObjects actions list.
		/// </summary>
		/// <param name="actionClassName">Name of the Action class to append</param>
		/// <param name="name">Assign the Action class to be created a name</param>
		/// <returns>A base reference to the action created.</returns>
		Action& CreateAction(const std::string& actionClassName, const std::string& name);
																								   
		/// <summary>
		/// Default Constructor.
		/// </summary>
		GameObject();

		//ADDED 6/20/2022
		GameObject(Game& game);

		GameObject* GetParentGameObject();

		//Each GameObject in the game will have it's member variable _game to be set to the game passed in. They will be subject to calls and updates from the game.
		void SetGame(Game& game);

		/// <summary>
		/// Constructor that takes in a string for name and a transform which is a container for position, rotation and scale.
		/// </summary>
		/// <param name="name">string for name</param>
		/// <param name="transform">Container for position, rotation and scale</param>
		GameObject(const std::string& name, const Transform& transform);

		/// <summary>
		/// Constructor that takes in a string for name and position, rotation and scale.
		/// </summary>
		/// <param name="name">string for name</param>
		/// <param name="position">A vector for position</param>
		/// <param name="rotation">A vector for rotation</param>
		/// <param name="scale">A vector for scale</param>
		GameObject(const std::string& name, const glm::vec4& position, const glm::vec4& rotation, const glm::vec4& scale);

		/// <summary>
		/// Author declared and compiler defined copy constructor
		/// </summary>
		/// <param name="">A GameObject to copy</param>
		GameObject(const GameObject&) = default;

		/// <summary>
		/// Author declared and compiler defined move constructor
		/// </summary>
		/// <param name="">A GameObject to move</param>
		GameObject(GameObject&&) noexcept = default;

		/// <summary>
		/// Author declared and compiler defined copy assignment
		/// </summary>
		/// <param name="">A GameObject to copy</param>
		/// <returns>A mutable reference to this GameObject</returns>
		GameObject& operator=(const GameObject&) = default;

		/// <summary>
		/// Author declared and compiler defined move assignment
		/// </summary>
		/// <param name="">A GameObject to move</param>
		/// <returns>A mutable reference to this GameObject</returns>
		GameObject& operator=(GameObject&&) noexcept = default;

		virtual ~GameObject() = default;

		/// <summary>
		/// Method is used for registering this types signatures and for classes that inherit from GameObject.
		/// Used by TypeRegistry class
		/// </summary>
		/// <returns>
		/// This classes unique signatures.
		/// </returns>
		static const Vector<Signature> Signatures();

		/// <summary>
		/// Macros is defined in TypeRegistry header file.
		/// This is used to register this class with the TypeRegistry.
		/// Will start a recursive call that will register all classes that are inherited.
		/// </summary>
		/// <param name="">Derived Class</param>
		/// <param name="">Base Class</param>
		StaticRegisterMethod(GameObject, Attributed);

		std::string _name;

		//template<typename T> //ADDED 6/20/2022
		//std::vector<T*> FindAllChildrenOfType();


		//template<typename T>
		//std::vector<const T* const> FindAllChildrenOfType() const;


		//template<typename T> //ADDED 6/20/2022
		//void WorkOnAllChildrenOfType(std::function<void(T&)>);

		//ADDED 6/21/2022
		const Game* const GetGame() const;
		Game* GetGame();

		//ADDED 6/21/2022
		//Not a fan of this. Will like to make game a singleton to provide better access to the game.
		virtual void Initialize();
		void UpdateTransform();

		void Translate(glm::vec4& translate);
		void Rotate(glm::vec4& translate);
		void Scale(glm::vec4& translate);

		void Translate(float x, float y, float z);
		void Rotate(float x, float y, float z);
		void Scale(float x, float y, float z);

		void TranslateX(const float x);
		void TranslateY(const float y);
		void TranslateZ(const float z);

		void RotateX(const float x);
		void RotateY(const float y);
		void RotateZ(const float z);

		void ScaleX(const float x);
		void ScaleY(const float y);
		void ScaleZ(const float z);

		const Transform& GetTransform() const;
		Transform& GetTransform();

		void AddTransformUpdateCallback(std::function<void()> callback)
		{
			mTransformUpdateCallback.push_back(callback);
		}

		//DirectX::XMFLOAT4X4 _worldMatrix{ MatrixHelper::Identity };
		bool IsTransformDataDirty() const;
	protected:


		void Init(const std::string& name, const Transform& transform);
		void Init(const std::string& name, const glm::vec4& position, const glm::vec4& rotation, const glm::vec4& scale);

		/// <summary>
		/// Protected constructor that is implemented in inheriting Objects who need to pass on their TypeId to Attributed.
		/// </summary>
		/// <param name="typeId">The derived classes TypeId</param>
		GameObject(std::size_t typeId);

		//ADDED 6/21/2022
		GameObject(Game& game, std::size_t typeId);

		Game* _game;


		bool _transformDataDirty{ true };
		std::vector<std::function<void()>> mTransformUpdateCallback;
		Transform _transform;

	private:
		GameObject* _parent{nullptr};
	};

	/// <summary>
	/// Macro is defined in Factory header file
	/// Will create a class that creates GameObjects and registers itself with the Factory Group. 
	/// </summary>
	/// <param name="">Concrete to be made in the factory</param>
	/// <param name="">The group to which this concrete object belongs to. The base class designed to be the defining type for the group.</param>
	ConcreteFactory(GameObject, Scope);
}

//#include "GameObject.inl"