#include "DiffuseLightingMaterial.h"
#include "GameException.h"
#include "Mesh.h"
#include <iostream>

namespace Library
{
	RTTI_DEFINITIONS(DiffuseLightingMaterial)

		DiffuseLightingMaterial::DiffuseLightingMaterial()
		: Material("main11"),
		INITIALIZATION(WorldViewProjection), INITIALIZATION(World),
		INITIALIZATION(AmbientColor), INITIALIZATION(LightColor),
		INITIALIZATION(LightDirection), INITIALIZATION(ColorTexture)
	{
	}

	DEFINE(DiffuseLightingMaterial, WorldViewProjection)
		DEFINE(DiffuseLightingMaterial, World)
		DEFINE(DiffuseLightingMaterial, AmbientColor)
		DEFINE(DiffuseLightingMaterial, LightColor)
		DEFINE(DiffuseLightingMaterial, LightDirection)
		DEFINE(DiffuseLightingMaterial, ColorTexture)

	void DiffuseLightingMaterial::Initialize(Effect* effect)
	{
		Material::Initialize(effect);

			RETRIEVE(WorldViewProjection)
			RETRIEVE(World)
			RETRIEVE(AmbientColor)
			RETRIEVE(LightColor)
			RETRIEVE(LightDirection)
			RETRIEVE(ColorTexture)

			D3D11_INPUT_ELEMENT_DESC inputElementDescriptions[] =
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }
		};

		CreateInputLayout("main11", "p0", inputElementDescriptions, ARRAYSIZE(inputElementDescriptions));
	}

	void DiffuseLightingMaterial::CreateVertexBuffer(ID3D11Device* device, const Mesh& mesh, ID3D11Buffer** vertexBuffer) const
	{
		const std::vector<XMFLOAT3>& sourceVertices = mesh.Vertices();
		std::vector<XMFLOAT3>* textureCoordinates = mesh.TextureCoordinates().at(0);
		assert(textureCoordinates->size() == sourceVertices.size());
		const std::vector<XMFLOAT3>& normals = mesh.Normals();
		assert(textureCoordinates->size() == sourceVertices.size());

		std::vector<DiffuseLightingMaterialVertex> vertices;
		vertices.reserve(sourceVertices.size());
		for (UINT i = 0; i < sourceVertices.size(); i++)
		{
			XMFLOAT3 pos = sourceVertices.at(i);
			XMFLOAT3 uv = textureCoordinates->at(i);
			XMFLOAT3 normal = normals.at(i);
			vertices.push_back(DiffuseLightingMaterialVertex(XMFLOAT4(pos.x, pos.y, pos.z, 1.0f), XMFLOAT2(uv.x, uv.y), normal));
		}

		CreateVertexBuffer(device, &vertices[0], vertices.size(), vertexBuffer);
	}

	void DiffuseLightingMaterial::CreateVertexBuffer(ID3D11Device* device, DiffuseLightingMaterialVertex* vertices, UINT vertexCount, ID3D11Buffer** vertexBuffer) const
	{
		D3D11_BUFFER_DESC vertexBufferDesc;
		ZeroMemory(&vertexBufferDesc, sizeof(vertexBufferDesc));
		vertexBufferDesc.ByteWidth = VertexSize() * vertexCount;
		vertexBufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
		vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

		D3D11_SUBRESOURCE_DATA vertexSubResourceData;
		ZeroMemory(&vertexSubResourceData, sizeof(vertexSubResourceData));
		vertexSubResourceData.pSysMem = vertices;
		if (FAILED(device->CreateBuffer(&vertexBufferDesc, &vertexSubResourceData, vertexBuffer)))
		{
			throw GameException("ID3D11Device::CreateBuffer() failed.");
		}
	}

	UINT DiffuseLightingMaterial::VertexSize() const
	{
		return sizeof(DiffuseLightingMaterialVertex);
	}
}