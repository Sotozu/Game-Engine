#include "pch.h"
#include "StaticMesh.h"
#include "Utility.h"
#include "ColorHelper.h"
#include "Camera.h"
#include "VertexDeclarations.h"
#include "Game.h"
#include "GameException.h"
#include "Model.h"
#include "Mesh.h"
#include "SamplerStates.h"
#include "D3DCompiler.h"
#include <cstdint>
#include "VertexShader.h"
#include "PixelShader.h"




using namespace std;
using namespace gsl;
using namespace DirectX;
namespace FieaGameEngine
{
	RTTI_DEFINITIONS(StaticMesh);

	StaticMesh::StaticMesh() : Action(StaticMesh::TypeIdClass()),
		_meshFilePath{ "" },
		_camera{ nullptr },
		_visible{true}
	{
	}

	StaticMesh::StaticMesh(Game& game) : Action(game, StaticMesh::TypeIdClass()),
		_meshFilePath{ "" },
		_visible{ true }
	{
		//_game will be initialized in the Action base class
		_camera = (Camera*)game.Services().GetService(Camera::TypeIdClass());
	}

	StaticMesh::StaticMesh(std::size_t typeId) : Action(typeId),
		_meshFilePath{ "" },
		_visible{ true },
		_camera{ nullptr }
	{
	}

	StaticMesh::StaticMesh(Game& game, std::size_t typeId) : Action(game, typeId),
		_meshFilePath{ "" },
		_visible{ true }

	{
		//_game will be initialized in the Action base class
		_camera = (Camera*)game.Services().GetService(Camera::TypeIdClass());
	}

	bool StaticMesh::Equals(const RTTI* rhs) const
	{
		const StaticMesh* action = rhs->As<StaticMesh>();
		if (action == nullptr) { return false; }

		return _name == action->_name &&
			_meshFilePath == action->_meshFilePath;
	}


	void StaticMesh::Update(const GameState& gameState)
	{
		UNREFERENCED_PARAMETER(gameState);
		/*if (mAnimationEnabled)
		{
			mRotationAngle += gameState._gTime.ElapsedGameTimeSeconds().count() * RotationRate;
			XMStoreFloat4x4(&_worldMatrix, XMMatrixRotationY(mRotationAngle));
			mUpdateConstantBuffer = true;
		}*/

	}


	const Vector<Signature> StaticMesh::Signatures()
	{
		return Vector<Signature>
		{
			{ "_meshFilePath", DatumTypes::String, 1, offsetof(StaticMesh, _meshFilePath) },
			{ "_vertexShaderFilePath", DatumTypes::String, 1, offsetof(StaticMesh, _vertexShaderFilePath) },
			{ "_pixelShaderFilePath", DatumTypes::String, 1, offsetof(StaticMesh, _pixelShaderFilePath) },
			{ "_visible", DatumTypes::Boolean, 1, offsetof(StaticMesh, _visible) }
		};
	}

	//Set's up the _game with the game pointer
	void StaticMesh::Initialize()
	{
		//Call the base class initialize method first as it sets up things for us.
		Action::Initialize();
		if (_game != nullptr) //Game is not set when first de serialized.
		{
			//Get a pointer to the camera
			_camera = (Camera*)_game->Services().GetService(Camera::TypeIdClass());

			auto direct3DDevice = _game->Direct3DDevice();
			auto& content = _game->Content();

			assert(_vertexShaderFilePath != "");
			assert(_pixelShaderFilePath != "");
			assert(_meshFilePath != "");

			_vertexShader = content.Load<VertexShader>(Utility::ToWideString(_vertexShaderFilePath));
			_pixelShader = content.Load<PixelShader>(Utility::ToWideString(_pixelShaderFilePath));
			_staticModel = content.Load<Model>(Utility::ToWideString(_meshFilePath));

			//Vertex shader class will asses what the input layout will be using shader reflection to buinputLayoutDescription an input layout description structure.
			_vertexShader->CreateInputLayout(direct3DDevice, _inputLayout.put());

			_staticMesh = _staticModel->Meshes().at(0).get();

			CreateVertexBuffer(direct3DDevice);

			CreateIndexBuffer(direct3DDevice, *_staticMesh, not_null<ID3D11Buffer**>(_indexBuffer.put()));

			_indexCount = narrow<uint32_t>(_staticMesh->Indices().size());

			D3D11_BUFFER_DESC constantBufferDesc{ 0 };
			constantBufferDesc.ByteWidth = narrow_cast<uint32_t>(sizeof(CBufferPerObject));
			constantBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
			ThrowIfFailed(_game->Direct3DDevice()->CreateBuffer(&constantBufferDesc, nullptr, _constantBuffer.put()), "ID3D11Device::CreateBuffer() failed.");

			// Load a texture
			const wstring textureName = L"Content\\Textures\\EarthComposite.dds"s;
			ThrowIfFailed(CreateDDSTextureFromFile(_game->Direct3DDevice(), textureName.c_str(), nullptr, _colorTexture.put()), "CreateDDSTextureFromFile() failed.");

			auto updateConstantBufferFunc = [this]() { _updateConstantBuffer = true; };
			_camera->AddViewMatrixUpdatedCallback(updateConstantBufferFunc);
			_camera->AddProjectionMatrixUpdatedCallback(updateConstantBufferFunc);
			
		}

	}

	bool StaticMesh::Visible() const
	{
		return _visible;
	}

	void StaticMesh::SetVisible(bool visible)
	{
		_visible = visible;
	}

	Camera* StaticMesh::GetCamera()
	{
		return _camera;
	}

	void StaticMesh::SetCamera(Camera& camera)
	{
		_camera = &camera;
	}

	void StaticMesh::Draw(const GameState&)
	{
		if (_visible)
		{
			//assert(mCamera != nullptr);

			ID3D11DeviceContext* direct3DDeviceContext = _game->Direct3DDeviceContext();
			direct3DDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
			direct3DDeviceContext->IASetInputLayout(_inputLayout.get());

			//uint32_t stride = VertexPositionColor::VertexSize();
			uint32_t stride = VertexPositionTexture::VertexSize(); //TODO: Modularize this
			uint32_t offset = 0; //TODO: Research more about the use of offset. I think this is used in batch rendering.

			const auto vertexBuffers = _vertexBuffer.get();
			direct3DDeviceContext->IASetVertexBuffers(0, 1, &vertexBuffers, &stride, &offset);
			direct3DDeviceContext->IASetIndexBuffer(_indexBuffer.get(), DXGI_FORMAT_R32_UINT, 0);

			direct3DDeviceContext->VSSetShader(_vertexShader->Shader().get(), nullptr, 0);
			direct3DDeviceContext->PSSetShader(_pixelShader->Shader().get(), nullptr, 0);

			if (_updateConstantBuffer)
			{
				const XMMATRIX worldMatrix = XMLoadFloat4x4(&_worldMatrix);

				XMMATRIX wvp = worldMatrix * _camera->ViewProjectionMatrix();

				wvp = XMMatrixTranspose(wvp);

				//Stored the WVP Matrix into the CBufferPerObject so that it can be passed to the GPU
				XMStoreFloat4x4(&_cBufferPerObject.WorldViewProjection, wvp);

				direct3DDeviceContext->UpdateSubresource(_constantBuffer.get(), 0, nullptr, &_cBufferPerObject, 0, 0);

				_updateConstantBuffer = false;
			}

			const auto vsConstantBuffers = _constantBuffer.get();
			direct3DDeviceContext->VSSetConstantBuffers(0, 1, &vsConstantBuffers);

			const auto psShaderResources = _colorTexture.get();
			direct3DDeviceContext->PSSetShaderResources(0, 1, &psShaderResources);

			const auto psSamplers = SamplerStates::TrilinearWrap.get(); //TODO: review this and determine how to modularize this at runtime.
			direct3DDeviceContext->PSSetSamplers(0, 1, &psSamplers);

			direct3DDeviceContext->DrawIndexed(_indexCount, 0, 0);
		}
	}

	void StaticMesh::CreateIndexBuffer(gsl::not_null<ID3D11Device*> device, const Mesh& mesh, gsl::not_null<ID3D11Buffer**> indexBuffer)
	{
		D3D11_BUFFER_DESC indexBufferDesc{ 0 };
		indexBufferDesc.ByteWidth = narrow_cast<uint32_t>(sizeof(uint32_t) * mesh.Indices().size());
		indexBufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
		indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;

		D3D11_SUBRESOURCE_DATA indexSubResourceData{ 0 };
		indexSubResourceData.pSysMem = &mesh.Indices()[0];

		ThrowIfFailed(device->CreateBuffer(&indexBufferDesc, &indexSubResourceData, indexBuffer), "ID3D11Device::CreateBuffer() failed.");

	}

	void StaticMesh::CreateVertexBuffer(ID3D11Device* direct3DDevice)
	{
		//PART 1: Create a buffer that contains the data specified in the InputLayoutDescription. Make sure that this buffers data is from the mesh.

		const auto& inputLayoutDescription = _vertexShader->GetInputLayoutDescription();

		size_t totalBitsPerVertex{ 0 };

		//Calls functions that call placement new of type specified in the Input Layout Description. This will allow us to create the vertexBuffer programmatically
		//given the input layout description in the VertexShader.
		std::vector<std::function<void(char* bufferStart, int& bufferOffset, size_t vertexIndex)>> setVertexBufferElements;

		//iterate through each D3D11_INPUT_ELEMENT_DESC and sum up the total sizes.
		//Also create a function that will be used to place data into the buffer.
		for (const auto& descriptionStruct : inputLayoutDescription)
		{
			totalBitsPerVertex += DirectX::DXGI_FORMAT_BitsPerPixel(descriptionStruct.Format);
			UINT semanticIndex = descriptionStruct.SemanticIndex;

			if (descriptionStruct.SemanticName == "POSITION"s)
			{
				const vector<XMFLOAT3>& sourceVertices = _staticMesh->Vertices();
				
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
				const vector<XMFLOAT3>& textureCoordinates = _staticMesh->TextureCoordinates().at(semanticIndex);
				assert(textureCoordinates.size() == _staticMesh->Vertices().size());

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
				const vector<XMFLOAT4>& vertexColors = _staticMesh->VertexColors().at(semanticIndex);
				assert(vertexColors.size() == _staticMesh->Vertices().size());

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
				const vector<XMFLOAT3>& sourceNormals = _staticMesh->Normals();
				assert(sourceNormals.size() == _staticMesh->Vertices().size());

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
				const vector<XMFLOAT3>& sourceTangents = _staticMesh->Tangents();
				assert(sourceTangents.size() == _staticMesh->Vertices().size());

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
				const vector<XMFLOAT3>& sourceTangents = _staticMesh->Tangents();
				assert(sourceTangents.size() == _staticMesh->Vertices().size());

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
		size_t numberOfVerticies = _staticMesh->Vertices().size();
		size_t bitsPerByte = 8;
		size_t BytesPerVertex{ totalBitsPerVertex / bitsPerByte };
		size_t BytesInBuffer{ BytesPerVertex * numberOfVerticies };
		void* vPtr = malloc(BytesInBuffer);
		assert(vPtr != nullptr);
		char* bufferStart = reinterpret_cast<char*>(vPtr);
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
		vertexSubResourceData.pSysMem = vPtr;
		ThrowIfFailed(direct3DDevice->CreateBuffer(&vertexBufferDesc, &vertexSubResourceData, not_null<ID3D11Buffer**>(_vertexBuffer.put())), "ID3D11Device::CreateBuffer() failed."); //This is from GameException class
		free(vPtr);

	}
}