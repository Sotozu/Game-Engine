#pragma once

#include <DirectXColors.h>
#include "Material.h"
#include "VectorHelper.h"
#include "MatrixHelper.h"
#include "SamplerStates.h"

namespace FieaGameEngine
{
	class Texture2D;
}

namespace FieaGameEngine
{
	class Light;
	class PointLightMaterial : public Material
	{
	friend Light;
		RTTI_DECLARATIONS(PointLightMaterial, Material);
#pragma region REQUIRED METHODS TO SUPPORT RTTI AND ATTRIBUTED DESIGN
	public:
		/// <summary>
		/// Default Constructor required for Factory
		/// </summary>
		PointLightMaterial();

		/// <summary>
		/// Clones the calling GameObject. Heap constructs this object and it's on the user to manage its memory.
		/// Overridden function from Scope.
		/// </summary>
		/// <returns>
		/// A pointer to heap allocated clone of the calling GameObject.
		/// </returns>
		gsl::owner<PointLightMaterial*> Clone() const override;

		/// <summary>
		/// Override function from GameObject
		/// Update method will update the GameObject and then recursively update all it's children GameObjects.
		/// Made virtual so any derived classes from GameObject can have their own update method and be called polymorphically.
		/// </summary>
		/// <param name="gameTime">A reference to the game time</param>
		void Update(const GameState& gameState) override;

		/// <summary>
		/// Overridden function from RTTI.
		/// Compares one object with another.
		/// </summary>
		/// <param name="rhs">The object to compare against</param>
		/// <returns>
		/// TRUE if the object is equivalent.
		/// FALSE if the object is not equivalent
		/// </returns>
		bool Equals(const RTTI* rhs) const override;

		/// <summary>
		/// Overridden function from RTTI.
		/// Returns a string that identifies this class.
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
		StaticRegisterMethod(PointLightMaterial, Material);

		virtual const std::vector<D3D11_INPUT_ELEMENT_DESC>& GetInputElementDescription() override;
		virtual void CreateVertexBuffer(gsl::not_null<ID3D11Device*> device, const Mesh& mesh, gsl::not_null<ID3D11Buffer**> vertexBuffer) override;

	protected:

		/// <summary>
		/// Protected constructor that is implemented in inheriting Objects who need to pass on their TypeId to Attributed.
		/// </summary>
		/// <param name="typeId">The derived classes TypeId</param>
		PointLightMaterial(std::size_t typeId);

		std::string _colorMap{ "" };
		std::string _specularMap{ "" };

#pragma endregion


	public:
		PointLightMaterial(Game& game, std::shared_ptr<Texture2D> colormap, std::shared_ptr<Texture2D> specularMap);
		PointLightMaterial(const PointLightMaterial&) = default;
		PointLightMaterial& operator=(const PointLightMaterial&) = default;
		PointLightMaterial(PointLightMaterial&&) = default;
		PointLightMaterial& operator=(PointLightMaterial&&) = default;
		virtual ~PointLightMaterial() override = default;

		winrt::com_ptr<ID3D11SamplerState> SamplerState() const;
		void SetSamplerState(winrt::com_ptr<ID3D11SamplerState> samplerState);

		std::shared_ptr<Texture2D> ColorMap() const;
		void SetColorMap(std::shared_ptr<Texture2D> texture);

		std::shared_ptr<Texture2D> SpecularMap() const;
		void SetSpecularMap(std::shared_ptr<Texture2D> texture);

		const DirectX::XMFLOAT4& AmbientColor() const;
		void SetAmbientColor(const DirectX::XMFLOAT4& color);

		const DirectX::XMFLOAT3& LightPosition() const;
		void SetLightPosition(const DirectX::XMFLOAT3& position);

		const float LightRadius() const;
		void SetLightRadius(float radius);

		const DirectX::XMFLOAT4& LightColor() const;
		void SetLightColor(const DirectX::XMFLOAT4& color);

		const DirectX::XMFLOAT3& SpecularColor() const;
		void SetSpecularColor(const DirectX::XMFLOAT3& color);

		const float SpecularPower() const;
		void SetSpecularPower(float power);

		virtual std::uint32_t VertexSize() const override;
		virtual void Initialize() override;

		virtual void UpdateCameraPosition(const DirectX::XMFLOAT3& position) override;
		virtual void UpdateTransform(DirectX::FXMMATRIX worldViewProjectionMatrix, DirectX::CXMMATRIX worldMatrix) override;
		virtual void UpdateTransform(DirectX::CXMMATRIX worldViewProjectionMatrix) override;

	private:
		struct VertexCBufferPerFrame
		{
			DirectX::XMFLOAT3 LightPosition{ Vector3Helper::Zero };
			float LightRadius{ 50.0f };
		};

		struct VertexCBufferPerObject
		{
			DirectX::XMFLOAT4X4 WorldViewProjection{ MatrixHelper::Identity };
			DirectX::XMFLOAT4X4 World{ MatrixHelper::Identity };
		};

		struct PixelCBufferPerFrame
		{
			DirectX::XMFLOAT3 CameraPosition{ Vector3Helper::Zero };
			float Padding;
			DirectX::XMFLOAT4 AmbientColor{ DirectX::Colors::Black };
			DirectX::XMFLOAT3 LightPosition{ 0.0f, 0.0f, 0.0f };
			float Padding2;
			DirectX::XMFLOAT4 LightColor{ DirectX::Colors::White };
		};

		struct PixelCBufferPerObject
		{
			DirectX::XMFLOAT3 SpecularColor{ 1.0f, 1.0f, 1.0f };
			float SpecularPower{ 128.0f };
		};

		virtual void BeginDraw() override;

		void ResetPixelShaderResources();

		winrt::com_ptr<ID3D11Buffer> mVertexCBufferPerFrame;
		winrt::com_ptr<ID3D11Buffer> mVertexCBufferPerObject;
		winrt::com_ptr<ID3D11Buffer> mPixelCBufferPerFrame;
		winrt::com_ptr<ID3D11Buffer> mPixelCBufferPerObject;
		VertexCBufferPerFrame mVertexCBufferPerFrameData;
		VertexCBufferPerObject mVertexCBufferPerObjectData;
		PixelCBufferPerFrame mPixelCBufferPerFrameData;
		PixelCBufferPerObject mPixelCBufferPerObjectData;
		bool mVertexCBufferPerFrameDataDirty{ true };
		bool mPixelCBufferPerFrameDataDirty{ true };
		bool mPixelCBufferPerObjectDataDirty{ true };
		std::shared_ptr<Texture2D> mColorMap;
		std::shared_ptr<Texture2D> mSpecularMap;
		winrt::com_ptr<ID3D11SamplerState> mSamplerState{ SamplerStates::TrilinearClamp };
	};

	/// <summary>
	/// Macro is defined in Factory header file
	/// Will create a class that creates this type of object and registers itself with the Factory Group. 
	/// </summary>
	/// <param name="">Concrete to be made in the factory</param>
	/// <param name="">The group to which this concrete object belongs to. The base class designed to be the defining type for the group.</param>
	ConcreteFactory(PointLightMaterial, Scope);

}