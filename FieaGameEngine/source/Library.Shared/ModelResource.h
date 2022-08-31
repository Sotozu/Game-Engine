#pragma once

#include <vector>
#include <map>
#include <string>
#include <fstream>
#include "RTTI.h"

namespace FieaGameEngine
{
	class Game;
	class Mesh;
	class ModelMaterial;
	class SceneNode;
	class Bone;
	class AnimationClip;
	class OutputStreamHelper;
	class InputStreamHelper;

	struct ModelData final
	{
		std::vector<std::shared_ptr<Mesh>> Meshes;
		std::vector<std::shared_ptr<ModelMaterial>> Materials;
		std::vector<std::shared_ptr<AnimationClip>> Animations;
		std::map<std::string, std::shared_ptr<AnimationClip>> AnimationsByName;
		std::vector<std::shared_ptr<Bone>> Bones;
		std::map<std::string, std::uint32_t> BoneIndexMapping;
		std::shared_ptr<SceneNode> RootNode;

		bool operator==(const ModelData& rhs) const
		{
			return Meshes == rhs.Meshes &&
				Materials == rhs.Materials &&
				Animations == rhs.Animations &&
				AnimationsByName == rhs.AnimationsByName &&
				Bones == rhs.Bones &&
				BoneIndexMapping == rhs.BoneIndexMapping && 
				RootNode == rhs.RootNode;	
		}

		bool operator!=(const ModelData& rhs) const 
		{
			return !(operator==(rhs));
		}
	};

	class ModelResource final : public RTTI
	{
		RTTI_DECLARATIONS(ModelResource, RTTI);

	public:
		ModelResource();
		ModelResource(const std::string& filename);
		ModelResource(std::ifstream& file);
		ModelResource(ModelData&& modelData);
		ModelResource(const ModelResource&) = default;
		ModelResource(ModelResource&&) = default;
		ModelResource& operator=(const ModelResource&) = default;
		ModelResource& operator=(ModelResource&&) = default;
		virtual ~ModelResource() = default;

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


	private:
		void Load(const std::string& filename);
		void Load(std::ifstream& file);

		void SaveSkeleton(OutputStreamHelper& streamHelper, const std::shared_ptr<SceneNode>& sceneNode) const;
		std::shared_ptr<SceneNode> LoadSkeleton(InputStreamHelper& streamHelper, std::shared_ptr<SceneNode> parentSceneNode);

		ModelData mData;
	};
}