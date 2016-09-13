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

namespace Rendering
{
    const XMVECTORF32 RenderingGame::BackgroundColor = ColorHelper::Black;

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

		DrawableGameObject* mChair = new DrawableGameObject(*this, *mCamera, "Content\\Models\\Ball_Chair\\ball_chair(blender).obj", *mDirectionalLight, MaterialType::PHONG);
		mChair->SetPosition(0.0f, 0.0f, 0.0f);
		mChair->SetTexture(L"Content\\Textures\\Ball_Chair\\shell_color.jpg"); mChair->SetTexture(L"Content\\Textures\\Ball_Chair\\pillows_color.jpg"); mChair->SetTexture(L"Content\\Textures\\Ball_Chair\\pillows_color.jpg"); mChair->SetTexture(L"Content\\Textures\\Ball_Chair\\trim_color.jpg"); mChair->SetTexture(L"Content\\Textures\\Ball_Chair\\padding_color.jpg");
		mComponents.push_back(mChair);

		//balls
		float count = 0;
		for (double i = 0; i < 4; i++){
			for (double j = 0; j < 4; j++){
				DrawableGameObject* mBall = new DrawableGameObject(*this, *mCamera, "Content\\Models\\Sphere.obj", *mDirectionalLight, MaterialType::PHONG);
				mBall->SetPosition(5.0f + (2.3 * i), 1.0f + (2.3 * j), -12.0f);
				mBall->SetTexture(L"Content\\Textures\\Metal_Texture.jpg");
				mBall->SetScale(0.2f);
				count += .5f;
				float specularPower = count;
				std::cout << specularPower << std::endl;
				mBall->SetSpecularPower(specularPower);
				mComponents.push_back(mBall);
			}
		}
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

        if (mKeyboard->WasKeyPressedThisFrame(DIK_ESCAPE))
        {
            Exit();
        }

        Game::Update(gameTimes);

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