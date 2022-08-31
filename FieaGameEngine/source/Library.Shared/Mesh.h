#pragma once

#include <gsl\gsl>
#include <d3d11.h>
#include "GameObject.h"
#include "TypeRegistry.h"
#include "Factory.h"
#include "Bone.h"

namespace FieaGameEngine
{
	class ModelResource;
	class Material;
	class ModelMaterial;
	class OutputStreamHelper;
	class InputStreamHelper;

	struct MeshData final
	{
		std::shared_ptr<ModelMaterial> Material;
		std::string Name;
		std::vector<DirectX::XMFLOAT3> Vertices;
		std::vector<DirectX::XMFLOAT3> Normals;
		std::vector<DirectX::XMFLOAT3> Tangents;
		std::vector<DirectX::XMFLOAT3> BiNormals;
		std::vector<std::vector<DirectX::XMFLOAT3>> TextureCoordinates;
		std::vector<std::vector<DirectX::XMFLOAT4>> VertexColors;
		std::uint32_t FaceCount{ 0 };
		std::vector<std::uint32_t> Indices;
		std::vector<BoneVertexWeights> BoneWeights;
		
		bool operator==(const MeshData& rhs) const
		{
			return Material == rhs.Material &&
				Name == rhs.Name &&
				Vertices.size() == rhs.Vertices.size() &&
				Normals.size() == rhs.Normals.size() &&
				Tangents.size() == rhs.Tangents.size() &&
				BiNormals.size() == rhs.BiNormals.size() &&
				TextureCoordinates.size() == rhs.TextureCoordinates.size() &&
				VertexColors.size() == rhs.VertexColors.size() &&
				FaceCount == rhs.FaceCount &&
				Indices == rhs.Indices &&
				BoneWeights.size() == rhs.BoneWeights.size();
		}
	};

	class Mesh final : public GameObject
	{
		RTTI_DECLARATIONS(Mesh, GameObject);

#pragma region REQUIRED METHODS TO SUPPORT RTTI AND ATTRIBUTED DESIGN
	public:

		/// <summary>
		/// Clones the calling GameObject. Heap constructs this object and it's on the user to manage its memory.
		/// Overridden function from Scope.
		/// </summary>
		/// <returns>
		/// A pointer to heap allocated clone of the calling GameObject.
		/// </returns>
		gsl::owner<Mesh*> Clone() const override;


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
		StaticRegisterMethod(Mesh, GameObject);

		/// <summary>
		/// Used as a "constructor after the Mesh has been instantiated.
		/// </summary>
		void Init(ModelResource& model, InputStreamHelper& streamHelper);
		void Init(ModelResource& model, MeshData&& meshData);


		std::string _name;
#pragma endregion
	public:


		Mesh();
		Mesh(ModelResource& model, InputStreamHelper& streamHelper);
		Mesh(ModelResource& model, MeshData&& meshData);
		Mesh(const Mesh&) = default;
		Mesh(Mesh&&) = default;
		Mesh& operator=(const Mesh&) = default;
		Mesh& operator=(Mesh&&) = default;
		~Mesh() = default;

		ModelResource& GetModel();
		std::shared_ptr<ModelMaterial> GetMaterial();
		const std::string& Name() const;

		const std::vector<DirectX::XMFLOAT3>& Vertices() const;
		const std::vector<DirectX::XMFLOAT3>& Normals() const;
		const std::vector<DirectX::XMFLOAT3>& Tangents() const;
		const std::vector<DirectX::XMFLOAT3>& BiNormals() const;
		const std::vector<std::vector<DirectX::XMFLOAT3>>& TextureCoordinates() const;
		const std::vector<std::vector<DirectX::XMFLOAT4>>& VertexColors() const;
		std::uint32_t FaceCount() const;
		const std::vector<std::uint32_t>& Indices() const;
		const std::vector<BoneVertexWeights>& BoneWeights() const;

		void CreateIndexBuffer(ID3D11Device& device, gsl::not_null<ID3D11Buffer**> indexBuffer);
		void CreateIndexBuffer(gsl::not_null<ID3D11Device*> device, gsl::not_null<ID3D11Buffer**> indexBuffer);
		void Save(OutputStreamHelper& streamHelper) const;

		bool operator==(const Mesh& rhs) const;

		bool operator!=(const Mesh& rhs) const;
	private:
		void Load(InputStreamHelper& streamHelper);

		ModelResource* mModel;
		MeshData mData;
	};

	/// <summary>
	/// Macro is defined in Factory header file
	/// Will create a class that creates GameObjects and registers itself with the Factory Group. 
	/// </summary>
	/// <param name="">Concrete to be made in the factory</param>
	/// <param name="">The group to which this concrete object belongs to. The base class designed to be the defining type for the group.</param>
	ConcreteFactory(Mesh, Scope);

}