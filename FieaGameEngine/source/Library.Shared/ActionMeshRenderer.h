#pragma once
#include "Action.h"
#include "Factory.h"
#include "Utility.h"
#include "ColorHelper.h"
#include "VertexDeclarations.h"
#include "GameException.h"
#include "DrawableGameComponent.h"
#include "Model.h"
#include "Mesh.h"
#include "Camera.h"
#include "MatrixHelper.h"
#include "Material.h"

namespace FieaGameEngine
{
	class VertexShader;
	class PixelShader;

	class ActionMeshRenderer final : public Action
	{
		RTTI_DECLARATIONS(ActionMeshRenderer, Action);

	public:

		/// <summary>
		/// Returns a string that identifies this class.
		/// Overridden function from RTTI.
		/// </summary>
		/// <returns>A string that identifies this class</returns>
		inline std::string ToString() const override;


		/// <summary>
		/// Clones the calling Action. Heap constructs this object and it's on the user to manage its memory.
		/// Overridden function from Scope.
		/// </summary>
		/// <returns>
		/// A pointer to heap allocated clone of the calling Action.
		/// </returns>
		inline gsl::owner<ActionMeshRenderer*> Clone() const override;

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

		//ADDED 6/22/2022
		virtual void Initialize() override;

		/// <summary>
		/// Can set the member variables of the Action after construction. 
		/// Actions will not be automatically populated when conjured by the user.
		/// </summary>
		/// <param name="name">Name that uniquely identifies this Action</param>
		/// <param name="step">The amount to increment an integer by everytime update is called</param>
		/// <param name="targetKey">Name of member variable we wish to increment. Use case target is usually the parent objects data member</param>
		inline void Init(const std::string& name, const std::string& filePath);

		/// <summary>
		/// </summary>
		/// <param name="step">The amount to increment an integer by everytime update is called</param>
		/// <param name="targetKey">Name of member variable we wish to increment.Use case target is usually the parent objects data member</param>
		void Init(const std::string& filePath);

		/// <summary>
		/// </summary>
		/// <param name="gameState">reference to a class that governs the state of the game</param>
		void Update(const GameState& gameState) override;

		/// <summary>
		/// Sets up the D3D pipeline to execute a draw call form the GPU.
		/// </summary>
		/// <param name="gameState">A reference to the gamestate. Used to obtain time and other important information required per frame.</param>
		void Draw(const GameState& gameState);

		/// <summary>
		/// Default Constructor
		/// Required for Factory and de-serialization
		/// </summary>
		ActionMeshRenderer();

		//ADDED 6/21/2022
		ActionMeshRenderer(Game& game);

		~ActionMeshRenderer();
		ActionMeshRenderer(const ActionMeshRenderer&) = default;
		ActionMeshRenderer(ActionMeshRenderer&&) noexcept = default;

		ActionMeshRenderer& operator=(const ActionMeshRenderer&) = default;
		ActionMeshRenderer& operator=(ActionMeshRenderer&&) noexcept = default;

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
		StaticRegisterMethod(ActionMeshRenderer, Action);

		/// <summary>
		/// Name of an integer member variable we wish to target.
		/// Use case is an integer member variable of the parent object or an object up the hierarchy.
		/// </summary>

		bool Visible() const;

		void SetVisible(bool visible);

		Camera* GetCamera();

		void SetCamera(Camera& camera);
	protected:
		/// Protected constructor that is implemented in inheriting Objects who need to pass on their TypeId to Attributed.
		/// </summary>
		/// <param name="typeId">The derived classes TypeId</param>
		ActionMeshRenderer(std::size_t typeId);

		//ADDED 6/21/2022
		ActionMeshRenderer(Game& game, std::size_t typeId);

	private:
		bool _visible;

		struct CBufferPerObject
		{
			DirectX::XMFLOAT4X4 WorldViewProjection;
		};

		/// <summary>
		/// Programmatically creates a vertex-buffer. 
		/// Gets this informations from the vertex shaders InputLayoutDescription.
		/// </summary>
		void CreateVertexBuffer(ID3D11Device* pD3DDevice);
		void CreateIndexBuffer(gsl::not_null<ID3D11Device*> device, gsl::not_null<ID3D11Buffer**> indexBuffer);

		DirectX::XMFLOAT4X4 _worldMatrix{ MatrixHelper::Identity };

		CBufferPerObject _cBufferPerObject;
		winrt::com_ptr<ID3D11InputLayout> _inputLayout;

		winrt::com_ptr<ID3D11Buffer> _vertexBuffer;
		winrt::com_ptr<ID3D11Buffer> _indexBuffer;
		winrt::com_ptr<ID3D11Buffer> _constantBuffer;


		winrt::com_ptr<ID3D11ShaderResourceView> _colorTexture;

		std::shared_ptr<VertexShader> _vertexShader;
		std::shared_ptr<PixelShader> _pixelShader;

		std::uint32_t _indexCount{ 0 };
		bool _updateConstantBuffer{ true };

		Camera* _camera{ nullptr };
		Model* _model;
		Material* _material;
		Mesh* _staticMesh;

		bool _updateMaterial{ false };
	//	DirectX::XMFLOAT4X4 _worldMatrix{ FieaGameEngine::MatrixHelper::Identity };

		//Testing this out because model currently loads funny at times and i think it's because the buffer space is freed.
		void* _vbuf{nullptr};

		Transform const* _parentTransform{ nullptr };
	};

	ConcreteFactory(ActionMeshRenderer, Action);
}

#include "ActionMeshRenderer.inl"
