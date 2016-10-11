#include "RenderingGame.h"
#include "GameException.h"
#include "Keyboard.h"
#include "Mouse.h"
#include "FpsComponent.h"
#include "ColorHelper.h"
#include "FirstPersonCamera.h"
#include "RenderStateHelper.h"
#include "RasterizerStates.h"
#include "SamplerStates.h"
#include "Grid.h"
#include "DrawableGameObject.h"
#include "Model.h"
#include "DirectionalLight.h"
#include <iostream>
#include "DiffuseLightingMaterial.h"
#include "PhysiscEngine/Vector3.h"
#include "PhysiscEngine/Particle.h"
#include "PhysiscEngine/ParticleForceGenerator.h"
#include "PhysiscEngine/ParticleLink.h"
#include "PhysiscEngine/ParticleWorld.h"
#include "PhysiscEngine/Matrix.h"
#include "PhysiscEngine/ForceGenerators.h"
#include <vector>


namespace Rendering
{
    const XMVECTORF32 RenderingGame::BackgroundColor = ColorHelper::Black;
	DrawableGameObject* mChair;
	DrawableGameObject* mBallf;
	Gravity grav = Gravity(Vector3(0, -1, 0));
	//
	float time=0.0f;
	//ParticleWorld pworld = ParticleWorld(5,5);

	std::vector<DrawableGameObject*> Particles;

    RenderingGame::RenderingGame(HINSTANCE instance, const std::wstring& Class, const std::wstring& Title, int showCommand)
        :  Game(instance, Class, Title, showCommand),
		mFpsComponent(nullptr), mDirectionalLight(nullptr),
		   mDirectInput(nullptr), mKeyboard(nullptr), mMouse(nullptr), mRenderStateHelper(nullptr)
    {
        mDepthStencilBufferEnabled = true;
        mMultiSamplingEnabled = true;

		AllocConsole();
		freopen("CONOUT$", "w", stdout);
    }

    RenderingGame::~RenderingGame()
    {
    }

	void RenderingGame::InitializeDrawables() {

		Grid* mGrid = new Grid(*this, *mCamera);
		mComponents.push_back(mGrid);

		mChair = new DrawableGameObject(*this, *mCamera, "Content\\Models\\Ball_Chair\\ball_chair(blender).obj", *mDirectionalLight, MaterialType::PHONG);
		mChair->rigidBody->SetPosition(Vector3(0.0f, 0, 0.0f));
		mChair->rigidBody->SetMass(10.0f);
		mChair->rigidBody->SetPosition(1, 0, 0);
		mChair->rigidBody->SetLinearDamping(.99f);
		mChair->rigidBody->SetAngularDamping(.99f);

		Matrix3 inertiaTensor = Matrix3();
		float a = mChair->rigidBody->GetMass() * 2/5 * 3 *3 ;
		inertiaTensor.data[0] = a;
		inertiaTensor.data[4] = a;
		inertiaTensor.data[8] = a;

		mChair->rigidBody->SetInertiaTensor(inertiaTensor);

		mChair->SetTexture(L"Content\\Textures\\Ball_Chair\\shell_color.jpg"); mChair->SetTexture(L"Content\\Textures\\Ball_Chair\\pillows_color.jpg"); mChair->SetTexture(L"Content\\Textures\\Ball_Chair\\pillows_color.jpg"); mChair->SetTexture(L"Content\\Textures\\Ball_Chair\\trim_color.jpg"); mChair->SetTexture(L"Content\\Textures\\Ball_Chair\\padding_color.jpg");
		mChair->SetScale(0.2f);
		mComponents.push_back(mChair);
		
		mBallf = new DrawableGameObject(*this, *mCamera, "Content\\Models\\Sphere.obj", *mDirectionalLight, MaterialType::PHONG);
		//mBallf->setPos(Vector3(0.0f, -40.5f, 0.0f));
		mBallf->rigidBody->SetPosition(Vector3(0.0f, 0, 0.0f));
		mBallf->rigidBody->SetMass(10.0f);
		mBallf->rigidBody->SetPosition(0, 0, 0);
		mBallf->rigidBody->SetLinearDamping(.99f);
		mBallf->rigidBody->SetAngularDamping(.99f);
		mBallf->rigidBody->SetInertiaTensor(inertiaTensor);

		mBallf->SetTexture(L"Content\\Textures\\Metal_Texture.jpg");
		mBallf->SetScale(0.2f);
		mComponents.push_back(mBallf);

		//mChair->setMass(20.0f);
		//mChair->setVelocity(Vector3(0.0f, 0.0f, 0.0f));
		//mChair->setAcceleration(Vector3(0.0f, -3.0f, 0.0f));
		//mChair->setDamping(0.9f);

		//mBallf->setMass(2.0f);
		//mBallf->setVelocity(Vector3(0.0f, 0.0f, 0.0f));
		//mChair->setAcceleration(Vector3(0.0f, -3.0f, 0.0f));
		//mBallf->setDamping(0.9f);

		//pworld.particles.push_back(mChair);
		//pworld.particles.push_back(mBallf);
		//pworld.registery.Add(mChair, new ParticleSpring(mBallf, 2, 3));
		//pworld.registery.Add(mBallf, new ParticleSpring(mChair, 1, 2));

		//for (int i = 0; i < 5; i++)
		//{
		//	DrawableGameObject * mBallf1 = new DrawableGameObject(*this, *mCamera, "Content\\Models\\Sphere.obj", *mDirectionalLight, MaterialType::PHONG);
		//	mBallf1->setPos(Vector3(0.0f, 0.0f, i * -10.0f));
		//	mBallf1->SetTexture(L"Content\\Textures\\Metal_Texture.jpg");
		//	mBallf1->SetScale(0.2f);
		//	mComponents.push_back(mBallf1);

		//	mBallf1->setMass(2.0f);
		//	mBallf1->setVelocity(Vector3(0.0f, 0.0f, 0.0f));
		//	//mChair->setAcceleration(Vector3(0.0f, -3.0f, 0.0f));
		//	mBallf1->setDamping(0.9f);

		//	pworld.particles.push_back(mBallf1);

		//	Particles.push_back(mBallf1);

		//	pworld.registery.Add(mBallf1, new ParticleGravity(Vector3(0, -0.1f, 0)));

		//	

		//	//for (vector<DrawableGameObject*>::iterator i = Particles.begin(); i != Particles.end(); i++){
		//	if (i != 0){
		//		ParticleCable * cable2 = new ParticleCable();
		//		cable2->particles[0] = mBallf1;
		//		cable2->particles[1] = Particles[i-1];
		//		cable2->maxLenght = 2;
		//		cable2->restitution = .5;
		//		pworld.contactGenerators.push_back(cable2);
		//		//}
		//	}
		//	else
		//	{
		//		ParticleCable * cable1 = new ParticleCable();
		//		cable1->particles[0] = mChair;
		//		cable1->particles[1] = mBallf1;
		//		cable1->maxLenght = 2;
		//		cable1->restitution = .5;
		//		pworld.contactGenerators.push_back(cable1);
		//	}

		//}

	/*	ParticleCable * cable = new ParticleCable();
		cable->particles[0] = mBallf;
		cable->particles[1] = mChair;
		cable->maxLenght = 5;
		cable->restitution = .5;
		pworld.contactGenerators.push_back(cable);*/

		//std::cout << cable->restitution << std::endl;
		

		//balls
	/*	float count = 0;
		for (double i = 0; i < 4; i++){
			for (double j = 0; j < 4; j++){
				DrawableGameObject* mBall = new DrawableGameObject(*this, *mCamera, "Content\\Models\\Sphere.obj", *mDirectionalLight, MaterialType::PHONG);
				mBall->SetPosition(5.0f + (2.3 * i), 1.0f + (2.3 * j), -12.0f);
				mBall->SetTexture(L"Content\\Textures\\Metal_Texture.jpg");
				mBall->SetScale(0.2f);
				count += .5f;
				float specularPower = count;
				mBall->SetSpecularPower(specularPower);
				mComponents.push_back(mBall);
			}
		}*/

		Matrix4 matrix1;
		matrix1.data[0] = 2;
		matrix1.data[1] = 3;
		matrix1.data[2] = 4;
		matrix1.data[3] = 6;
		matrix1.data[4] = 8;
		matrix1.data[5] = 2;
		matrix1.data[6] = 10;
		matrix1.data[7] = 7;
		matrix1.data[8] = 9;
		matrix1.data[9] = 9;
		matrix1.data[10] = 2;
		matrix1.data[11] = 11;

		//Matrix4 matrix2;
		//matrix2.data[0] = 1;
		//matrix2.data[1] = 3;
		//matrix2.data[2] = 4;
		//matrix2.data[3] = 6;
		//matrix2.data[4] = 8;
		//matrix2.data[5] = 12;
		//matrix2.data[6] = 10;
		//matrix2.data[7] = 7;
		//matrix2.data[8] = 9;
		//matrix2.data[9] = 9;
		//matrix2.data[10] = 10;
		//matrix2.data[11] = 11;

		//matrix.Invert();
		Matrix4 inverseMatrix = matrix1.Inverse();
		Matrix4 matrix =matrix1 * inverseMatrix;

		Vector3 testVec = Vector3(1, 2, 3);

	/*	std::cout << matrix.data[0] << " " << matrix.data[1] << " " << matrix.data[2] << " " << matrix.data[3] << std::endl;
		std::cout << matrix.data[4] << " " << matrix.data[5] << " " << matrix.data[6] << " " << matrix.data[7] << std::endl;
		std::cout << matrix.data[8] << " " << matrix.data[9] << " " << matrix.data[10] << " " << matrix.data[11] << std::endl;*/
	}

    void RenderingGame::Initialize()
    {
        if (FAILED(DirectInput8Create(mInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (LPVOID*)&mDirectInput, nullptr)))
        {
            throw GameException("DirectInput8Create() failed");
        }

        mKeyboard = new Keyboard(*this, mDirectInput);
        mComponents.push_back(mKeyboard);
        mServices.AddService(Keyboard::TypeIdClass(), mKeyboard);
    
        mMouse = new Mouse(*this, mDirectInput);
        mComponents.push_back(mMouse);
        mServices.AddService(Mouse::TypeIdClass(), mMouse);

        mCamera = new FirstPersonCamera(*this);
        mComponents.push_back(mCamera);
        mServices.AddService(Camera::TypeIdClass(), mCamera);
    
        mFpsComponent = new FpsComponent(*this);
		mFpsComponent->Initialize();

		mDirectionalLight = new DirectionalLight(*this);
		mComponents.push_back(mDirectionalLight);

		InitializeDrawables();

		RasterizerStates::Initialize(mDirect3DDevice);
		SamplerStates::BorderColor = ColorHelper::Black;
		SamplerStates::Initialize(mDirect3DDevice);

		mRenderStateHelper = new RenderStateHelper(*this);

        Game::Initialize();

		mCamera->SetPosition(0.0f, 0.0f, 25.0f);
    }

    void RenderingGame::Shutdown()
    {	
        DeleteObject(mRenderStateHelper);
		DeleteObject(mKeyboard);
        DeleteObject(mMouse);
        DeleteObject(mFpsComponent);
        DeleteObject(mCamera);

        ReleaseObject(mDirectInput);
		RasterizerStates::Release();
		SamplerStates::Release();

        Game::Shutdown();
    }

    void RenderingGame::Update(const GameTime &gameTimes)
    {
		mFpsComponent->Update(gameTimes);
		float deltaTime = (float)gameTimes.TotalGameTime() - time;
		Spring spring = Spring(Vector3(0, 1, 0), Vector3(0, 0, 0), mBallf->rigidBody, 1, 2);

		spring.UpdateForce(mChair->rigidBody, deltaTime);
		grav.UpdateForce(mChair->rigidBody, deltaTime);

        if (mKeyboard->WasKeyPressedThisFrame(DIK_ESCAPE))
        {
            Exit();
        }

        Game::Update(gameTimes);

		time = (float)gameTimes.TotalGameTime();

    }

    void RenderingGame::Draw(const GameTime &gameTimes)
    {
		mDirectContext->ClearRenderTargetView(mRenderTargetView, reinterpret_cast<const float*>(&BackgroundColor));
		mDirectContext->ClearDepthStencilView(mDepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
        Game::Draw(gameTimes);

		mRenderStateHelper->SaveAll();
		mFpsComponent->Draw(gameTimes);
		mRenderStateHelper->RestoreAll();
        
        HRESULT hr = mSwapChain->Present(0, 0);
        if (FAILED(hr))
        {
            throw GameException("IDXGISwapChain::Present() failed.", hr);
        }
    }
}