#include "pch.h"
#include "Model.h"
#include "Utility.h"

using namespace std;
using namespace gsl;
using namespace DirectX;

namespace FieaGameEngine
{
	RTTI_DEFINITIONS(Model);


	Model::Model() : GameObject(Model::TypeIdClass())
	{
	}

	Model::Model(Game& game) : GameObject(game, Model::TypeIdClass())
	{
	}

	Model::Model(const string& filename) : GameObject(Model::TypeIdClass()),
		_filePath(filename)
	{
		auto& content = _game->Content();
		_modelResource = content.Load<ModelResource>(Utility::ToWideString(filename));
	}

	Model::Model(Game& game, const std::string& filename) : GameObject(game, Model::TypeIdClass()),
		_filePath(filename)
	{
		auto& content = _game->Content();
		_modelResource = content.Load<ModelResource>(Utility::ToWideString(filename));
	}

	inline std::string Model::ToString() const
	{
		return "Model"s;
	}

	bool Model::Equals(const RTTI* rhs) const
	{
		const Model* model = rhs->As<Model>();
		if (model == nullptr) { return false; }

		return _filePath == model->_filePath;
	}

	void Model::Update(const GameState& gameState)
	{
		UNREFERENCED_PARAMETER(gameState);
	}

	const Vector<Signature> Model::Signatures()
	{
		return Vector<Signature> {
			{"_filePath", DatumTypes::String, 1, offsetof(Model, _filePath)}
		};
	}

	void Model::Initialize()
	{
		GameObject::Initialize();
		auto& content = _game->Content();
		assert(_filePath != ""); //Make sure that it is not empty.
		_modelResource = content.Load<ModelResource>(Utility::ToWideString(_filePath));
	}

	bool Model::HasMeshes() const
	{
		return _modelResource->HasMeshes();
	}

	bool Model::HasMaterials() const
	{
		return _modelResource->HasMeshes();
	}

	bool Model::HasAnimations() const
	{
		return _modelResource->HasAnimations();
	}

	const vector<shared_ptr<Mesh>>& Model::Meshes() const
	{
		return _modelResource->Meshes();
	}

	const vector<shared_ptr<ModelMaterial>>& Model::Materials() const
	{
		return _modelResource->Materials();
	}

	const vector<shared_ptr<AnimationClip>>& Model::Animations() const
	{
		return _modelResource->Animations();
	}

	const map<string, shared_ptr<AnimationClip>>& Model::AnimationsByName() const
	{
		return _modelResource->AnimationsByName();
	}

	const vector<shared_ptr<Bone>> Model::Bones() const
	{
		return _modelResource->Bones();
	}

	const map<string, uint32_t> Model::BoneIndexMapping() const
	{
		return _modelResource->BoneIndexMapping();
	}

	shared_ptr<SceneNode> Model::RootNode() const
	{
		return _modelResource->RootNode();
	}

	ModelData& Model::Data()
	{
		return _modelResource->Data();
	}

	void Model::Save(const string& filename) const
	{
		_modelResource->Save(filename);
	}

	void Model::Save(ofstream& file) const
	{
		_modelResource->Save(file);
	}

	void Model::SetModelPath(std::string filePath)
	{
		if (filePath == "") { throw std::runtime_error("Empty string parameter is not allowed."); }
		_filePath = filePath;
		auto& content = _game->Content();
		_modelResource = content.Load<ModelResource>(Utility::ToWideString(_filePath));
	}

	const std::string& Model::GetModelPath() const
	{
		return _filePath;
	}
}
