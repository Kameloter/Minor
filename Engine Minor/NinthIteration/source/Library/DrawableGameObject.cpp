#include <sstream>
#include <iostream>
#include "DrawableGameObject.h"
#include "MatrixHelper.h"
#include "Effect.h"
#include "Model.h"
#include "Utility.h"
#include "DiffuseLightingMaterial.h"
#include "BasicMaterial.h"
#include "PhongMaterial.h"
#include "Game.h"
#include "Mesh.h"
#include <DDSTextureLoader.h>
#include "GameException.h"
#include "Camera.h"
#include <WICTextureLoader.h>
#include "VectorHelper.h"
#include "DirectionalLight.h"
#include "Material.h"


namespace Library
{
	RTTI_DEFINITIONS(DrawableGameObject)

		DrawableGameObject::DrawableGameObject(Game& game, Camera& camera, const std::string& modelPath, DirectionalLight& directionalLight, int materialType)
		: DrawableGameComponent(game, camera), mModelPath(modelPath),
		mAmbientColor(1, 0, 0, 0), mDirectionalLight(&directionalLight), mIndices(), 
		mMeshes(), mMaterials(), mVertexBuffers(), mIndexBuffers(), mIndexCounts(), mTexturePaths(), mShaderViews(),
		mWorldMatrix(MatrixHelper::Identity), mScaleMatrix(MatrixHelper::Identity), mPosition(Vector3Helper::Zero), mSpecularPower(25.0f)
	{
		mMaterialType = static_cast<mMaterialTypes>(materialType);
	}

	DrawableGameObject::~DrawableGameObject()
	{
	}

	void DrawableGameObject::Initialize()
	{
		SetCurrentDirectory(Utility::ExecutableDirectory().c_str());

		//create model
		std::unique_ptr<Model> model(new Model(*mGame, mModelPath, true));
		
		//store all meshes in vector
		mMeshes = model->Meshes();
		InitializeMeshes();
	}

	void DrawableGameObject::Update(const GameTime& gameTime)
	{
		rigidBody->Integrate(gameTime.ElapsedGameTime());
		//cout << rigidBody->GetPosition() << endl;
		SetPosition(rigidBody->GetPosition().x, rigidBody->GetPosition().y, rigidBody->GetPosition().z);
		Matrix4 mat = rigidBody->GetTransform();
		mWorldMatrix._11 = mat.data[0];
		mWorldMatrix._12 = mat.data[1];
		mWorldMatrix._13 = mat.data[2];
		mWorldMatrix._14 = 0;
		mWorldMatrix._21 = mat.data[4];
		mWorldMatrix._22 = mat.data[5];
		mWorldMatrix._23 = mat.data[6];
		mWorldMatrix._24 = 0;
		mWorldMatrix._31 = mat.data[8];
		mWorldMatrix._32 = mat.data[9];
		mWorldMatrix._33 = mat.data[10];
		mWorldMatrix._34 = 0;
		//cout << mat << endl;
		mWorldMatrix._41 = mat.data[3];
		mWorldMatrix._42 = mat.data[7];
		mWorldMatrix._43 = mat.data[11];
		mWorldMatrix._44 = 1;
	}

	void DrawableGameObject::setUniforms() 
	{


	}

	void DrawableGameObject::Draw(const GameTime& gametime)
	{
		ID3D11DeviceContext* direct3DDeviceContext = mGame->Direct3DDeviceContext();
		direct3DDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		UINT offset = 0;

		XMMATRIX scaleMatrix = XMLoadFloat4x4(&mScaleMatrix);

		XMMATRIX worldMatrix = XMLoadFloat4x4(&mWorldMatrix) * scaleMatrix;
		MatrixHelper::SetTranslation(worldMatrix, mPosition);
		XMMATRIX wvp = worldMatrix  * mCamera->ViewMatrix() * mCamera->ProjectionMatrix();
		XMVECTOR ambientColor = XMLoadColor(&mAmbientColor);

		for (unsigned int i = 0; i < mMeshes.size(); i++){
			Pass* pass = mMaterials[i]->CurrentTechnique()->Passes().at(0);
			ID3D11InputLayout* inputLayout = mMaterials[i]->InputLayouts().at(pass);
			direct3DDeviceContext->IASetInputLayout(inputLayout);

			UINT stride = mMaterials[i]->VertexSize();
			direct3DDeviceContext->IASetVertexBuffers(0, 1, &mVertexBuffers[i], &stride, &offset);
			direct3DDeviceContext->IASetIndexBuffer(mIndexBuffers[i], DXGI_FORMAT_R32_UINT, 0);
			
			switch (mMaterialType)
			{
			case Library::DrawableGameObject::BASIC:
			{
				BasicMaterial* material = mMaterials[i]->As<BasicMaterial>();
				material->WorldViewProjection() << wvp;
				break;
			}
			case Library::DrawableGameObject::DIFFUSELIGHTING:
			{
				DiffuseLightingMaterial* material = mMaterials[i]->As<DiffuseLightingMaterial>();
				material->WorldViewProjection() << wvp;
				material->World() << worldMatrix;
				material->AmbientColor() << ambientColor;
				material->LightColor() << mDirectionalLight->ColorVector();
				material->LightDirection() << mDirectionalLight->DirectionVector();
				material->ColorTexture() << mShaderViews[i];
				break;
			}
			case Library::DrawableGameObject::PHONG:
			{
				PhongMaterial* material = mMaterials[i]->As<PhongMaterial>();
				material->WorldViewProjection() << wvp;
				material->World() << worldMatrix;
				material->LightDirection() << mDirectionalLight->DirectionVector();
				material->CameraPosition() << mCamera->PositionVector();
				material->AmbientColor() << ambientColor;
				material->ColorTexture() << mShaderViews[i];
				material->SpecularPower() << mSpecularPower;
				break;
			}
			default:
				break;
			}

			pass->Apply(0, direct3DDeviceContext);

			direct3DDeviceContext->DrawIndexed(mIndexCounts[i], 0, 0);
		}
	}

	void DrawableGameObject::InitializeMeshes() {

		for (unsigned int i = 0; i < mMeshes.size(); i++){

			Effect* mEffect = new Effect(*mGame);
			ID3D11Buffer* mVertexBuffer;
			ID3D11Buffer* mIndexBuffer;
			Mesh* mMesh = mMeshes.at(i);
			switch (mMaterialType)
			{
			case Library::DrawableGameObject::BASIC:
			{
				mEffect->LoadCompiledEffect(L"Content\\Effects\\BasicEffect.cso");
				BasicMaterial* mMaterial = new BasicMaterial();
				mMaterial->Initialize(mEffect);
				mMaterial->CreateVertexBuffer(mGame->Direct3DDevice(), *mMesh, &mVertexBuffer);
				mMaterials.push_back(mMaterial);
				break;
			}
			case Library::DrawableGameObject::DIFFUSELIGHTING:
			{
				mEffect->LoadCompiledEffect(L"Content\\Effects\\DiffuseLighting.cso");
				DiffuseLightingMaterial* mMaterial = new DiffuseLightingMaterial();
				mMaterial->Initialize(mEffect);
				mMaterial->CreateVertexBuffer(mGame->Direct3DDevice(), *mMesh, &mVertexBuffer);
				mMaterials.push_back(mMaterial);
				break;
			}
			case Library::DrawableGameObject::PHONG:
			{
				mEffect->LoadCompiledEffect(L"Content\\Effects\\Phong.cso");
				PhongMaterial* mMaterial = new PhongMaterial();
				mMaterial->Initialize(mEffect);
				mMaterial->CreateVertexBuffer(mGame->Direct3DDevice(), *mMesh, &mVertexBuffer);
				mMaterials.push_back(mMaterial);
				break;
			}
			default:
				mEffect->LoadCompiledEffect(L"Content\\Effects\\BasicEffect.cso");
				BasicMaterial* mMaterial = new BasicMaterial();
				mMaterial->Initialize(mEffect);
				mMaterial->CreateVertexBuffer(mGame->Direct3DDevice(), *mMesh, &mVertexBuffer);
				mMaterials.push_back(mMaterial);
				break;
			}
			//mEffect->LoadCompiledEffect(L"Content\\Effects\\DiffuseLighting.cso");
			//DiffuseLightingMaterial* mMaterial = new DiffuseLightingMaterial();
			mVertexBuffers.push_back(mVertexBuffer);
			mMesh->CreateIndexBuffer(&mIndexBuffer);
			mIndexBuffers.push_back(mIndexBuffer);
			mIndexCounts.push_back(mMesh->Indices().size());
			ID3D11ShaderResourceView* mTextureShaderResourceView;
			HRESULT hr = DirectX::CreateWICTextureFromFile(mGame->Direct3DDevice(), mGame->Direct3DDeviceContext(), mTexturePaths[i].c_str(), nullptr, &mTextureShaderResourceView);
			if (FAILED(hr))
			{
				throw GameException("CreateWICTextureFromFile() failed. At least provide one texture", hr);
			}
			mShaderViews.push_back(mTextureShaderResourceView);
		}
	}

	void DrawableGameObject::SetPosition(float x, float y, float z) {
		mPosition = XMFLOAT3(x, y, z);
	}

	void DrawableGameObject::SetTexture(std::wstring textureName){
		mTexturePaths.push_back(textureName);
	}
	
	void DrawableGameObject::SetScale(float s)
	{
		XMStoreFloat4x4(&mScaleMatrix, XMMatrixScaling(s, s, s));
	}

	void DrawableGameObject::SetSpecularPower(float s)
	{
		mSpecularPower = s;
	}
}
