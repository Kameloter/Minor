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
#include <vector>


namespace Rendering
{
    const XMVECTORF32 RenderingGame::BackgroundColor = ColorHelper::Black;
	DrawableGameObject* mChair;
	DrawableGameObject* mBallf;
	float time=0.0f;
	ParticleWorld pworld = ParticleWorld(5,5);

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

		mChair = new DrawableGameObject(*this, *mCamera, "Content\\Models\\Sphere.obj", *mDirectionalLight, MaterialType::PHONG);
		mChair->setPos(Vector3(0.0f, 0, 0.0f));
		mChair->SetTexture(L"Content\\Textures\\Metal_Texture.jpg");
		mChair->SetScale(0.2f);
		mComponents.push_back(mChair);
		
		mBallf = new DrawableGameObject(*this, *mCamera, "Content\\Models\\Sphere.obj", *mDirectionalLight, MaterialType::PHONG);
		mBallf->setPos(Vector3(0.0f, -40.5f, 0.0f));
		mBallf->SetTexture(L"Content\\Textures\\Metal_Texture.jpg");
		mBallf->SetScale(0.2f);
		mComponents.push_back(mBallf);

		mChair->setMass(20.0f);
		mChair->setVelocity(Vector3(0.0f, 0.0f, 0.0f));
		//mChair->setAcceleration(Vector3(0.0f, -3.0f, 0.0f));
		mChair->setDamping(0.9f);

		mBallf->setMass(2.0f);
		mBallf->setVelocity(Vector3(0.0f, 0.0f, 0.0f));
		//mChair->setAcceleration(Vector3(0.0f, -3.0f, 0.0f));
		mBallf->setDamping(0.9f);

		pworld.particles.push_back(mChair);
		pworld.particles.push_back(mBallf);
		pworld.registery.Add(mChair, new ParticleSpring(mBallf, 2, 3));
		pworld.registery.Add(mBallf, new ParticleSpring(mChair, 1, 2));

		for (int i = 0; i < 5; i++)
		{
			DrawableGameObject * mBallf1 = new DrawableGameObject(*this, *mCamera, "Content\\Models\\Sphere.obj", *mDirectionalLight, MaterialType::PHONG);
			mBallf1->setPos(Vector3(0.0f, 0.0f, i * -10.0f));
			mBallf1->SetTexture(L"Content\\Textures\\Metal_Texture.jpg");
			mBallf1->SetScale(0.2f);
			mComponents.push_back(mBallf1);

			mBallf1->setMass(2.0f);
			mBallf1->setVelocity(Vector3(0.0f, 0.0f, 0.0f));
			//mChair->setAcceleration(Vector3(0.0f, -3.0f, 0.0f));
			mBallf1->setDamping(0.9f);

			pworld.particles.push_back(mBallf1);

			Particles.push_back(mBallf1);

			pworld.registery.Add(mBallf1, new ParticleGravity(Vector3(0, -0.1f, 0)));

			

			//for (vector<DrawableGameObject*>::iterator i = Particles.begin(); i != Particles.end(); i++){
			if (i != 0){
				ParticleCable * cable2 = new ParticleCable();
				cable2->particles[0] = mBallf1;
				cable2->particles[1] = Particles[i-1];
				cable2->maxLenght = 2;
				cable2->restitution = .5;
				pworld.contactGenerators.push_back(cable2);
				//}
			}
			else
			{
				ParticleCable * cable1 = new ParticleCable();
				cable1->particles[0] = mChair;
				cable1->particles[1] = mBallf1;
				cable1->maxLenght = 2;
				cable1->restitution = .5;
				pworld.contactGenerators.push_back(cable1);
			}

		}

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
		
		pworld.StartFrame();
		
		for (vector<DrawableGameObject*>::iterator i = Particles.begin(); i != Particles.end(); i++){
			(*i)->SetPosition((*i)->getPos().x, (*i)->getPos().y, (*i)->getPos().z );
		}

		mChair->SetPosition(mChair->getPos().x, mChair->getPos().y, mChair->getPos().z);
		mBallf->SetPosition(mBallf->getPos().x, mBallf->getPos().y, mBallf->getPos().z);

        if (mKeyboard->WasKeyPressedThisFrame(DIK_ESCAPE))
        {
            Exit();
        }


        Game::Update(gameTimes);

		pworld.RunPhysics(deltaTime);
		//std::cout << mChair->getPos().x << " " << mChair->getPos().y << " " << mChair->getPos().z << std::endl;

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