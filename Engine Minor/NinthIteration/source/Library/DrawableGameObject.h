#pragma once
#include "Common.h"
#include "DrawableGameComponent.h"
#include "PhysiscEngine/Particle.h"
#include "PhysiscEngine/RigidBody.h"

using namespace PhysicsEngine;

namespace Library
{
	class Effect;
	class Model;
	class Mesh;
	class DiffuseLightingMaterial;
	class DirectionalLight;
	class Material;
	class DrawableGameObject : public DrawableGameComponent 
	{
		RTTI_DECLARATIONS(DrawableGameObject, DrawableGameComponent)
	public:
		DrawableGameObject(Game& game, Camera& camera, const std::string& modelPath, DirectionalLight& directionalLight, int materialType);
		~DrawableGameObject();

		virtual void Initialize() override;
		virtual void Update(const GameTime& gameTime) override;
		virtual void Draw(const GameTime& gameTime) override;

		void SetPosition(float x, float y, float z);
		void SetTexture(std::wstring);
		void SetScale(float s);
		void SetSpecularPower(float s);

		Rigidbody * rigidBody = new Rigidbody();

	private:
		void CreateIndexBuffer(Model& mModel);
		void InitializeMeshes();
		void setUniforms();

		std::string mModelPath;
		XMFLOAT4X4 mWorldMatrix;
		XMFLOAT4X4 mScaleMatrix;

		std::vector<UINT> mIndices;

		//meshes, materials, shaders, vertexb, indexb
		std::vector<Mesh*> mMeshes;
		std::vector<Material*> mMaterials;
		std::vector<ID3D11Buffer*> mVertexBuffers;
		std::vector<ID3D11Buffer*> mIndexBuffers;
		std::vector<UINT> mIndexCounts;
		std::vector<std::wstring> mTexturePaths;
		std::vector<ID3D11ShaderResourceView*> mShaderViews;

		//color and light
		XMCOLOR mAmbientColor;
		DirectionalLight* mDirectionalLight;

		//pos
		float mSpecularPower;
		XMFLOAT3 mPosition;
		enum mMaterialTypes : int{BASIC, DIFFUSELIGHTING, PHONG};
		mMaterialTypes mMaterialType;
	};
}

