#pragma once

#include "Common.h"
#include "Game.h"

using namespace Library;

namespace Library
{
    class Keyboard;
    class Mouse;
    class FirstPersonCamera;
    class FpsComponent;
	class RenderStateHelper;
	class Grid;
	class DrawableGameObject;
	class DirectionalLight;
}

namespace Rendering
{
	class DiffuseLightingDemo;

    class RenderingGame : public Game
    {
    public:
        RenderingGame(HINSTANCE instance, const std::wstring& Class, const std::wstring& Title, int showCommand);
        ~RenderingGame();

        virtual void Initialize() override;		
        virtual void Update(const GameTime& gameTime) override;
        virtual void Draw(const GameTime& gameTime) override;

    protected:
        virtual void Shutdown() override;

    private:
        static const XMVECTORF32 BackgroundColor;

        LPDIRECTINPUT8 mDirectInput;
		FirstPersonCamera* mCamera;
		FpsComponent* mFpsComponent;
		RenderStateHelper* mRenderStateHelper;
		DirectionalLight* mDirectionalLight;
        Keyboard* mKeyboard;
        Mouse* mMouse;
		void InitializeDrawables();

		enum MaterialType : int{BASIC, DIFFUSELIGHTING, PHONG};
    };
}