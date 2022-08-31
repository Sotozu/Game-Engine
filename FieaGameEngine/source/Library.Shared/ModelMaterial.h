#pragma once

#include <string>
#include <map>
#include <vector>
#include <gsl\gsl>

namespace FieaGameEngine
{
	class ModelResource;
	class OutputStreamHelper;
	class InputStreamHelper;

    enum class TextureType
    {
        Diffuse = 0,
        SpecularMap,
        Ambient,
        Emissive,
        Heightmap,
        NormalMap,
        SpecularPowerMap,
        DisplacementMap,
        LightMap,
        End
    };

	struct ModelMaterialData final
	{
		std::string Name;
		std::map<TextureType, std::vector<std::string>> Textures;

        bool operator==(const ModelMaterialData& rhs) const
        {
            return Name == rhs.Name &&
                Textures == rhs.Textures;
        }

		bool operator!=(const ModelMaterialData& rhs) const
		{
            return !(operator==(rhs));
		}

	};

    class ModelMaterial
    {
    public:
		ModelMaterial(ModelResource& model, InputStreamHelper& streamHelper);
		ModelMaterial(ModelResource& model, ModelMaterialData&& modelMaterialData);
		ModelMaterial(const ModelMaterial&) = default;
		ModelMaterial(ModelMaterial&&) = default;
		ModelMaterial& operator=(const ModelMaterial&) = default;
		ModelMaterial& operator=(ModelMaterial&&) = default;
		~ModelMaterial() = default;

        ModelResource& GetModel();
        const std::string& Name() const;
        const std::map<TextureType, std::vector<std::string>>& Textures() const;

		void Save(OutputStreamHelper& streamHelper) const;

		bool operator==(const ModelMaterial& rhs) const;
		bool operator!=(const ModelMaterial& rhs) const;

    private:
		void Load(InputStreamHelper& streamHelper);

        gsl::not_null<ModelResource*> mModel;
		ModelMaterialData mData;
    };
}