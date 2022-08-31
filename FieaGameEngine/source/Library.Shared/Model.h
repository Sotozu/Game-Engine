#pragma once
#include "GameObject.h"
#include "ModelResource.h"

namespace FieaGameEngine
{
	//Forward Declaration
	//class ModelResource;
	//class Game;
	//class Mesh;
	//class ModelData;
	//class ModelMaterial;
	//class SceneNode;
	//class Bone;
	//class AnimationClip;
	//class OutputStreamHelper;
	//class InputStreamHelper;

	class Model final : public GameObject
	{
		RTTI_DECLARATIONS(Model, GameObject);
	public:

		/// <summary>
		/// Default Constructor
		/// Required for Factory and de-serialization
		/// </summary>
		Model();
		Model(const std::string& filename);
		Model(Game& game);
		Model(Game& game, const std::string& filename);
		Model(const Model&) = default;
		Model(Model&&) = default;
		Model& operator=(const Model&) = default;
		Model& operator=(Model&&) = default;
		virtual ~Model() = default;


		/// <summary>
		/// Returns a string that identifies this class.
		/// Overridden function from RTTI.
		/// </summary>
		/// <returns>A string that identifies this class</returns>
		inline std::string ToString() const override;

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
		/// Derived actions will implement their unique Update method.
		/// </summary>
		/// <param name="gameTime">A reference to the game time</param>
		virtual void Update(const GameState& gameState);


		/// <summary>
		/// Method is used for registering this types signatures and for classes that inherit from Action.
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
		StaticRegisterMethod(Model, GameObject);

		bool HasMeshes() const;
		bool HasMaterials() const;
		bool HasAnimations() const;

		const std::vector<std::shared_ptr<Mesh>>& Meshes() const;
		const std::vector<std::shared_ptr<ModelMaterial>>& Materials() const;
		const std::vector<std::shared_ptr<AnimationClip>>& Animations() const;
		const std::map<std::string, std::shared_ptr<AnimationClip>>& AnimationsByName() const;
		const std::vector<std::shared_ptr<Bone>> Bones() const;
		const std::map<std::string, std::uint32_t> BoneIndexMapping() const;
		std::shared_ptr<SceneNode> RootNode() const;

		ModelData& Data();
		void Save(const std::string& filename) const;
		void Save(std::ofstream& file) const;
		virtual void Initialize() override;


		void SetModelPath(std::string _filePath);
		const std::string& GetModelPath() const;
	private:
		std::string  _filePath{""};
		std::shared_ptr<ModelResource> _modelResource{nullptr};
	};

	/// <summary>
	/// Macro is defined in Factory header file
	/// Will create a class that creates GameObjects and registers itself with the Factory Group. 
	/// </summary>
	/// <param name="">Concrete to be made in the factory</param>
	/// <param name="">The group to which this concrete object belongs to. The base class designed to be the defining type for the group.</param>
	ConcreteFactory(Model, Scope);

}

#include "Model.inl"

