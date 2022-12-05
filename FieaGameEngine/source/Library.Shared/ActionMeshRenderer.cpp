#include "pch.h"
#include "ActionMeshRenderer.h"
#include "Utility.h"
#include "ColorHelper.h"
#include "Camera.h"
#include "VertexDeclarations.h"
#include "Game.h"
#include "GameException.h"
#include "Mesh.h"
#include "SamplerStates.h"
#include "D3DCompiler.h"
#include <cstdint>
#include "VertexShader.h"
#include "PixelShader.h"
#include "FirstPersonCamera.h"
#include "MatrixHelper.h"



using namespace std;
using namespace gsl;
using namespace DirectX;

namespace FieaGameEngine
{
	RTTI_DEFINITIONS(ActionMeshRenderer);

	ActionMeshRenderer::ActionMeshRenderer() : Action(ActionMeshRenderer::TypeIdClass()),
		_camera{ nullptr },
		_visible{ true }
	{
	}

	ActionMeshRenderer::ActionMeshRenderer(Game& game) : Action(game, ActionMeshRenderer::TypeIdClass()),
		_visible{ true }
	{
		//_game will be initialized in the Action base class
		_camera = (Camera*)game.Services().GetService(Camera::TypeIdClass());
	}

	ActionMeshRenderer::ActionMeshRenderer(std::size_t typeId) : Action(typeId),
		_visible{ true },
		_camera{ nullptr }
	{
	}

	ActionMeshRenderer::ActionMeshRenderer(Game& game, std::size_t typeId) : Action(game, typeId),
		_visible{ true }

	{
		//_game will be initialized in the Action base class
		_camera = (Camera*)game.Services().GetService(Camera::TypeIdClass());
	}

	bool ActionMeshRenderer::Equals(const RTTI* rhs) const
	{
		const ActionMeshRenderer* action = rhs->As<ActionMeshRenderer>();
		if (action == nullptr) { return false; }

		return _name == action->_name;
	}

	const Vector<Signature> ActionMeshRenderer::Signatures()
	{
		return Vector<Signature>
		{
			{ "_visible", DatumTypes::Boolean, 1, offsetof(ActionMeshRenderer, _visible) },
			{ "_model", DatumTypes::Table, 0, 0 },
			{ "_material", DatumTypes::Table, 0, 0 },
		};
	}

	//Set's up the _game with the game pointer
	void ActionMeshRenderer::Initialize()
	{
		//Call the base class initialize method first as it sets up things for us.
		Action::Initialize();

		if (_game != nullptr) //Game is not set when first de serialized.
		{
			//Get a pointer to the camera
			_camera = (Camera*)_game->Services().GetService(Camera::TypeIdClass());

			auto direct3DDevice = _game->Direct3DDevice();
			//auto& content = _game->GetContentManager();


			if (_model == nullptr)
			{

				_model = Find("_model")->Get<Scope>().As<Model>();

				if(_model == nullptr) { throw std::runtime_error("Model not set in ActionMeshRenderer"); }
			}

			if (_material == nullptr)
			{
				_material = Find("_material")->Get<Scope>().As<Material>();

				if (_material == nullptr) { throw std::runtime_error("Material not set in ActionMeshRenderer"); }
				_material->Initialize();
			}

			auto staticMesh = _model->Meshes().at(0).get();

			_material->CreateVertexBuffer(direct3DDevice, *staticMesh, _vertexBuffer.put());
			staticMesh->CreateIndexBuffer(*direct3DDevice, not_null<ID3D11Buffer**>(_indexBuffer.put()));
			_indexCount = narrow<uint32_t>(staticMesh->Indices().size());

#pragma region NEW 7/31/2022

			auto updateMaterialFunc = [this]() { _updateMaterial = true; };
			_camera->AddViewMatrixUpdatedCallback(updateMaterialFunc);
			_camera->AddProjectionMatrixUpdatedCallback(updateMaterialFunc);


			auto firstPersonCamera = _camera->As<FirstPersonCamera>();
			if (firstPersonCamera != nullptr)
			{
				firstPersonCamera->AddPositionUpdatedCallback([this]() {
					_material->UpdateCameraPosition(_camera->Position());
					});
			}
#pragma endregion

			auto updateTransformFunc = [this]() { _updateMaterial = true; };
			//ActionMeshRenderer should never be the root game object.
			//Always have a GameObject Parent.
			 Scope::GetParent()->As<GameObject>()->AddTransformUpdateCallback(updateTransformFunc);
		}


		//Get a pointer to the parent transform
		_parentTransform = &GetParent()->As<GameObject>()->GetTransform();
		
	}

	void ActionMeshRenderer::ChangeModel(const std::shared_ptr<ModelResource> m)
	{
		if (_model != nullptr) { delete _model; }
		_model = new Model(m);
		Adopt("ActionMeshRenderer", *_model);
		_model->GameObject::Initialize();
		auto direct3DDevice = _game->Direct3DDevice();
		auto staticMesh = _model->Meshes().at(0).get();
		_material->CreateVertexBuffer(direct3DDevice, *staticMesh, _vertexBuffer.put());
		staticMesh->CreateIndexBuffer(*direct3DDevice, not_null<ID3D11Buffer**>(_indexBuffer.put()));
		_indexCount = narrow<uint32_t>(staticMesh->Indices().size());
	}

	//void ActionMeshRenderer::ChangeModel(const std::string& filePath)
	//{
	//	if (_model != nullptr) { delete _model; }
	//	_model = new Model();
	//	_model->_
	//	Adopt("ActionMeshRenderer", *_model);
	//	//_model->Initialize();
	//	auto direct3DDevice = _game->Direct3DDevice();
	//	auto staticMesh = _model->Meshes().at(0).get();
	//	staticMesh->CreateIndexBuffer(*direct3DDevice, not_null<ID3D11Buffer**>(_indexBuffer.put()));

	//}

	bool ActionMeshRenderer::Visible() const
	{
		return _visible;
	}

	void ActionMeshRenderer::SetVisible(bool visible)
	{
		_visible = visible;
	}

	Camera* ActionMeshRenderer::GetCamera()
	{
		return _camera;
	}

	void ActionMeshRenderer::SetCamera(Camera& camera)
	{
		_camera = &camera;
	}

	void ActionMeshRenderer::Update(const GameState& gameState)
	{

		//XMStoreFloat4x4(&_worldMatrix, XMMatrixRotationY(_parentTransform->_rotation.y));
		_updateMaterial = true;

		UNREFERENCED_PARAMETER(gameState);
	}

	void ActionMeshRenderer::Draw(const GameState&)
	{
#pragma region NEW 7/31/2022
		if (_visible)
		{
			if (_updateMaterial)
			{
				//This needs to the parents _worldMatrix
				XMMATRIX worldMatrix = XMLoadFloat4x4(&_worldMatrix);

				XMFLOAT3 translation{ _parentTransform->_position.x, _parentTransform->_position.y, _parentTransform->_position.z };

				//Scale
				//Rotate
				//Translate
				
				XMMATRIX ScaleMatrix = XMMatrixScaling(_parentTransform->_scale.x, _parentTransform->_scale.y, _parentTransform->_scale.z) * worldMatrix;

				float rotation = Radian* _parentTransform->_rotation.x;
				float pitch = Radian* _parentTransform->_rotation.y;
				float yaw = Radian* _parentTransform->_rotation.z;

				XMMATRIX rotationMatrix = XMMatrixRotationRollPitchYaw(rotation, pitch, yaw);

				worldMatrix = worldMatrix * ScaleMatrix * rotationMatrix;

				MatrixHelper::SetTranslation(worldMatrix, translation);

				XMMATRIX wvp = XMMatrixTranspose(worldMatrix * _camera->ViewProjectionMatrix());

				_material->UpdateTransform(wvp, XMMatrixTranspose(worldMatrix));

				_updateMaterial = false;
			}

			_material->DrawIndexed(not_null<ID3D11Buffer*>(_vertexBuffer.get()), not_null<ID3D11Buffer*>(_indexBuffer.get()), _indexCount);
		}
#pragma endregion

	}

	void ActionMeshRenderer::CreateIndexBuffer(gsl::not_null<ID3D11Device*> device, gsl::not_null<ID3D11Buffer**> indexBuffer)
	{
		auto staticMesh = _model->Meshes().at(0).get();
		D3D11_BUFFER_DESC indexBufferDesc{ 0 };
		indexBufferDesc.ByteWidth = narrow_cast<uint32_t>(sizeof(uint32_t) * staticMesh->Indices().size());
		indexBufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
		indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;

		D3D11_SUBRESOURCE_DATA indexSubResourceData{ 0 };
		indexSubResourceData.pSysMem = &staticMesh->Indices()[0];

		ThrowIfFailed(device->CreateBuffer(&indexBufferDesc, &indexSubResourceData, indexBuffer), "ID3D11Device::CreateBuffer() failed.");

	}

	void ActionMeshRenderer::CreateVertexBuffer(ID3D11Device* direct3DDevice)
	{

		//PART 1: Create a buffer that contains the data specified in the InputLayoutDescription. Make sure that this buffers data is from the mesh.
		//const auto&[isFound, data] = _material->GetDataForShaderStage(ShaderStages::VS);
		//assert(isFound);
		//VertexShader* vs = reinterpret_cast<VertexShader*>(data->Shader.get());
		//const auto& inputLayoutDescription = _vertexShader->GetInputLayoutDescription();
		const auto& inputLayoutDescription = _material->GetInputElementDescription();

		size_t totalBitsPerVertex{ 0 };

		//Calls functions that call placement new of type specified in the Input Layout Description. This will allow us to create the vertexBuffer programmatically
		//given the input layout description in the VertexShader.
		std::vector<std::function<void(char* bufferStart, int& bufferOffset, size_t vertexIndex)>> setVertexBufferElements;

		auto staticMesh = _model->Meshes().at(0).get();


		//iterate through each D3D11_INPUT_ELEMENT_DESC and sum up the total sizes.
		//Also create a function that will be used to place data into the buffer.
		for (const auto& descriptionStruct : inputLayoutDescription)
		{
			totalBitsPerVertex += DirectX::DXGI_FORMAT_BitsPerPixel(descriptionStruct.Format);
			UINT semanticIndex = descriptionStruct.SemanticIndex;

			if (descriptionStruct.SemanticName == "POSITION"s)
			{
				const vector<XMFLOAT3>& sourceVertices = staticMesh->Vertices();

				//Create method to fill buffer with an element of position vertex data from mesh.
				setVertexBufferElements.push_back([this, sourceVertices](char* bufferStart, int& bufferOffset, size_t vertexIndex)
					{
						const XMFLOAT3& position = sourceVertices.at(vertexIndex);

						new (bufferStart + bufferOffset) DirectX::XMFLOAT4(position.x, position.y, position.z, 1.0f);

						bufferOffset += sizeof(DirectX::XMFLOAT4);
					});
			}

			else if (descriptionStruct.SemanticName == "TEXCOORD"s)
			{
				const vector<XMFLOAT3>& textureCoordinates = staticMesh->TextureCoordinates().at(semanticIndex);
				assert(textureCoordinates.size() == staticMesh->Vertices().size());

				//Method to fill buffer with an element of texture-coordinate vertex data from mesh.
				setVertexBufferElements.push_back([this, textureCoordinates](char* bufferStart, int& bufferOffset, size_t vertexIndex)
					{
						const XMFLOAT3& uv = textureCoordinates.at(vertexIndex);

						new (bufferStart + bufferOffset) DirectX::XMFLOAT2(uv.x, uv.y);

						bufferOffset += sizeof(DirectX::XMFLOAT2);
					});
			}

			else if (descriptionStruct.SemanticName == "COLOR"s)
			{
				const vector<XMFLOAT4>& vertexColors = staticMesh->VertexColors().at(semanticIndex);
				assert(vertexColors.size() == staticMesh->Vertices().size());

				//Method to fill buffer with an element of texture-coordinate vertex data from mesh.
				setVertexBufferElements.push_back([this, vertexColors](char* bufferStart, int& bufferOffset, size_t vertexIndex)
					{
						const XMFLOAT4& color = vertexColors.at(vertexIndex);

						new (bufferStart + bufferOffset) DirectX::XMFLOAT4(color);

						bufferOffset += sizeof(DirectX::XMFLOAT4);
					});
			}

			else if (descriptionStruct.SemanticName == "NORMAL"s)
			{
				const vector<XMFLOAT3>& sourceNormals = staticMesh->Normals();
				assert(sourceNormals.size() == staticMesh->Vertices().size());

				//Method to fill buffer with an element of texture-coordinate vertex data from mesh.
				setVertexBufferElements.push_back([this, sourceNormals](char* bufferStart, int& bufferOffset, size_t vertexIndex)
					{
						const XMFLOAT3& normal = sourceNormals.at(vertexIndex);

						new (bufferStart + bufferOffset) DirectX::XMFLOAT3(normal);

						bufferOffset += sizeof(DirectX::XMFLOAT3);
					});
			}

			else if (descriptionStruct.SemanticName == "TANGENT"s)
			{
				const vector<XMFLOAT3>& sourceTangents = staticMesh->Tangents();
				assert(sourceTangents.size() == staticMesh->Vertices().size());

				//Method to fill buffer with an element of texture-coordinate vertex data from mesh.
				setVertexBufferElements.push_back([this, sourceTangents](char* bufferStart, int& bufferOffset, size_t vertexIndex)
					{
						const XMFLOAT3& tangent = sourceTangents.at(vertexIndex);

						new (bufferStart + bufferOffset) DirectX::XMFLOAT3(tangent);

						bufferOffset += sizeof(DirectX::XMFLOAT3);
					});
			}

			else if (descriptionStruct.SemanticName == "PSIZE"s)
			{
				const vector<XMFLOAT3>& sourceTangents = staticMesh->Tangents();
				assert(sourceTangents.size() == staticMesh->Vertices().size());

				//Method to fill buffer with an element of texture-coordinate vertex data from mesh.
				setVertexBufferElements.push_back([this, sourceTangents](char* bufferStart, int& bufferOffset, size_t vertexIndex)
					{
						const XMFLOAT3& tangent = sourceTangents.at(vertexIndex);

						new (bufferStart + bufferOffset) DirectX::XMFLOAT3(tangent);

						bufferOffset += sizeof(DirectX::XMFLOAT3);
					});
			}
		}

		//Left in this format to describe what is being calculated
		size_t numberOfVerticies = staticMesh->Vertices().size();
		size_t bitsPerByte = 8;
		size_t BytesPerVertex{ totalBitsPerVertex / bitsPerByte };
		size_t BytesInBuffer{ BytesPerVertex * numberOfVerticies };
		_vbuf = malloc(BytesInBuffer);
		assert(_vbuf != nullptr);
		char* bufferStart = reinterpret_cast<char*>(_vbuf);
		int bufferIndex{ 0 };

		//Fill in the buffer with data elements from the the mesh.
		for (size_t i = 0; i < numberOfVerticies; ++i)
		{

			for (size_t j = 0; j < setVertexBufferElements.size(); ++j)
			{
				setVertexBufferElements[j](bufferStart, bufferIndex, i);
			}
		}

		D3D11_BUFFER_DESC vertexBufferDesc{ 0 };
		vertexBufferDesc.ByteWidth = static_cast<UINT>(BytesInBuffer);
		vertexBufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
		vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

		D3D11_SUBRESOURCE_DATA vertexSubResourceData{ 0 };
		vertexSubResourceData.pSysMem = _vbuf;
		ThrowIfFailed(direct3DDevice->CreateBuffer(&vertexBufferDesc, &vertexSubResourceData, not_null<ID3D11Buffer**>(_vertexBuffer.put())), "ID3D11Device::CreateBuffer() failed."); //This is from GameException class
		//free(_vbuf);

	}

	ActionMeshRenderer::~ActionMeshRenderer()
	{
		if (_vbuf != nullptr) { free(_vbuf); }
	}

}
