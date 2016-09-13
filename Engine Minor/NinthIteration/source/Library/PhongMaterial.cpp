#include "PhongMaterial.h"
#include "GameException.h"
#include "Mesh.h"
#include "ColorHelper.h"

namespace Library
{
    RTTI_DEFINITIONS(PhongMaterial)	

		PhongMaterial::PhongMaterial()
        : Material("main11"),
		INITIALIZATION(WorldViewProjection), INITIALIZATION(World),
		INITIALIZATION(SpecularColor), INITIALIZATION(SpecularPower),
		INITIALIZATION(LightDirection), INITIALIZATION(CameraPosition),
		INITIALIZATION(AmbientColor), INITIALIZATION(LightColor),
		INITIALIZATION(ColorTexture)
    {
    }

		DEFINE(PhongMaterial, WorldViewProjection)
		DEFINE(PhongMaterial, World)
		DEFINE(PhongMaterial, SpecularColor)
		DEFINE(PhongMaterial, SpecularPower)
		DEFINE(PhongMaterial, LightDirection)
		DEFINE(PhongMaterial, CameraPosition)
		DEFINE(PhongMaterial, AmbientColor)
		DEFINE(PhongMaterial, LightColor)
		DEFINE(PhongMaterial, ColorTexture)

		void PhongMaterial::Initialize(Effect* effect)
    {
        Material::Initialize(effect);

		RETRIEVE(WorldViewProjection)
		RETRIEVE(World)
		RETRIEVE(SpecularColor)
		RETRIEVE(SpecularPower)
		RETRIEVE(LightDirection)
		RETRIEVE(CameraPosition)
		RETRIEVE(AmbientColor)
		RETRIEVE(LightColor)
		RETRIEVE(ColorTexture)
			

        D3D11_INPUT_ELEMENT_DESC inputElementDescriptions[] =
        {
            { "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
            { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }
        };

        CreateInputLayout("main11", "p0", inputElementDescriptions, ARRAYSIZE(inputElementDescriptions));
    }

	void PhongMaterial::CreateVertexBuffer(ID3D11Device* device, const Mesh& mesh, ID3D11Buffer** vertexBuffer) const
    {
		const std::vector<XMFLOAT3>& sourceVertices = mesh.Vertices();
		std::vector<XMFLOAT3>* textureCoordinates = mesh.TextureCoordinates().at(0);
		assert(textureCoordinates->size() == sourceVertices.size());
		const std::vector<XMFLOAT3>& normals = mesh.Normals();
		assert(textureCoordinates->size() == sourceVertices.size());

		std::vector<PhongMaterialVertex> vertices;
		vertices.reserve(sourceVertices.size());
		for (UINT i = 0; i < sourceVertices.size(); i++)
		{
			XMFLOAT3 pos = sourceVertices.at(i);
			XMFLOAT3 uv = textureCoordinates->at(i);
			XMFLOAT3 normal = normals.at(i);
			vertices.push_back(PhongMaterialVertex(XMFLOAT4(pos.x, pos.y, pos.z, 1.0f), XMFLOAT2(uv.x, uv.y), normal));
		}

		CreateVertexBuffer(device, &vertices[0], vertices.size(), vertexBuffer);
    }

	void PhongMaterial::CreateVertexBuffer(ID3D11Device* device, PhongMaterialVertex* vertices, UINT vertexCount, ID3D11Buffer** vertexBuffer) const
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

	UINT PhongMaterial::VertexSize() const
    {
        return sizeof(PhongMaterialVertex);
    }
}