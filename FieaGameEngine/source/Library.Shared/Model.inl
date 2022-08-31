//namespace FieaGameEngine
//{
//	inline std::string Model::ToString() const
//	{
//		return "Model"s;
//	}
//
//
//	inline bool Model::HasMeshes() const
//	{
//		return _modelResource->HasMeshes();
//	}
//
//	inline bool Model::HasMaterials() const
//	{
//		return _modelResource->HasMeshes();
//	}
//
//	inline bool Model::HasAnimations() const
//	{
//		return _modelResource->HasAnimations();
//	}
//
//	inline const vector<shared_ptr<Mesh>>& Model::Meshes() const
//	{
//		return _modelResource->Meshes();
//	}
//
//	inline const vector<shared_ptr<ModelMaterial>>& Model::Materials() const
//	{
//		return _modelResource->Materials();
//	}
//
//	inline const vector<shared_ptr<AnimationClip>>& Model::Animations() const
//	{
//		return _modelResource->Animations();
//	}
//
//	inline const map<std::string, shared_ptr<AnimationClip>>& Model::AnimationsByName() const
//	{
//		return _modelResource->AnimationsByName();
//	}
//
//	inline const vector<shared_ptr<Bone>> Model::Bones() const
//	{
//		return _modelResource->Bones();
//	}
//
//	inline const map<std::string, uint32_t> Model::BoneIndexMapping() const
//	{
//		return _modelResource->BoneIndexMapping();
//	}
//
//	inline shared_ptr<SceneNode> Model::RootNode() const
//	{
//		return _modelResource->RootNode();
//	}
//
//	
//
//	inline void Model::Save(const string& filename) const
//	{
//		_modelResource->Save(filename);
//	}
//
//	inline void Model::Save(ofstream& file) const
//	{
//		_modelResource->Save(file);
//	}
//
//}